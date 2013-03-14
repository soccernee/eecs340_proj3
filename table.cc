#include "table.h"

#if defined(GENERIC)
ostream & Table::Print(ostream &os) const
{
  // WRITE THIS
  os << "Table()";
  return os;
}
#endif

#if defined(LINKSTATE)

ostream & Table::Print(ostream &os) const
{
  // WRITE THIS
  os << "Table()";
  return os;
}
#endif

#if defined(DISTANCEVECTOR)

Table::Table() {
    cerr << "table constructor" << endl;
}

void Table::tableInit(unsigned nodeNumber) {
    cerr << "table init" << endl;
    map<unsigned,double> initMap;
    initMap.insert(pair<unsigned, double> (nodeNumber,0));
    nodeTotalMap.insert (pair<unsigned,map<unsigned,double> >(nodeNumber, initMap));
    thisNodeNumber = nodeNumber;
}

 void Table::setMap(map<unsigned, map<unsigned,double> > thisMap) {
    nodeTotalMap = thisMap;
 }

 bool Table::updateMap(unsigned mapNumber, map<unsigned,double> nodeMap){
    cerr << "Table: updateVector" << endl;

    cerr << "Updating neighbor " << mapNumber << "'s row in table" << endl;
    if(nodeTotalMap.find(mapNumber) != nodeTotalMap.end()) {
        cerr << "Deleting neighbor " << mapNumber << "'s current row in table" << endl;
        nodeTotalMap.erase(mapNumber);
    }
    cerr << "Inserting new row in table" << endl;
    nodeTotalMap.insert(make_pair(mapNumber, nodeMap));

    return updateVectorsThroughNeighbor(mapNumber);
 }

  void Table::addRowIfNotExists(unsigned rowNumber) {
    if(nodeTotalMap.find(rowNumber) == nodeTotalMap.end()) {
      cerr << "Creating new row in table with index " << rowNumber << endl;
      map<unsigned, double> newRow;
      nodeTotalMap.insert(make_pair(rowNumber, newRow));
    }

  }
 bool Table::updateVectorsThroughNeighbor(unsigned neighborNumber) {
  cerr << "Updating my row through neighbor " << neighborNumber << endl;
   bool ourDistanceVectorHasBeenUpdated = false;

   map<unsigned,map<unsigned,double> >::iterator ourDistanceVector = nodeTotalMap.find(thisNodeNumber);
   map<unsigned, map<unsigned, double>>::iterator neighborDistanceVector = nodeTotalMap.find(neighborNumber);

   double distanceFromUsToNeighbor = ourDistanceVector->second.find(thisNodeNumber)->second;
    cerr << "Distance from me to neighbor " << distanceFromUsToNeighbor << endl;
   for (map<unsigned,double>::iterator iter = neighborDistanceVector->second.begin(); iter != neighborDistanceVector->second.end(); iter++) {
      cerr << "Updating my vector for destination " << iter.first << endl;


      double distanceFromNewNodeToDestination = iter->second;
      cerr << "Distance from neighbor to destination " << distanceFromNewNodeToDestination << endl;


      double newDistance = distanceFromUsToNewNode + distanceFromNewNodeToDestination;

      cerr << "Distance from me to destination via neighbor" << newDistance << endl;

      double oldDistance = numeric_limits<double>::infinity();
      double ourCurrentDistanceToDestInTable = ourDistanceVector->second.find(iter->first);
      if(ourCurrentDistanceToDestInTable != ourDistanceVector->second.end()) {
        oldDistance = ourCurrentDistanceToDestInTable->second;
      }

      cerr << "Old distance from me to destination" << oldDistance;

      if(newDistance < oldDistance) {
          //We have a new distance vector for node iter->first
          cerr << "Vector through neighbor " << neighborNumber << " is less than current distance, updating my row" << endl;
          //If the destination is not currently in the table, insert it
          if(ourCurrentDistanceToDestInTable == ourDistanceVector->second.end()) {
              cerr << "This is a new entry in my routing table row" << endl;
              ourDistanceVector->second.insert(make_pair(iter->first, newDistance));
          } else {
                cerr << "Updating current entry in my routing table row" << endl;
              ourCurrentDistanceToDestInTable->second = newDistance;
          }
          ourDistanceVectorHasBeenUpdated = true;
          //Also update forwarding table
      }
    }
    return ourDistanceVectorHasBeenUpdated;
 }


 unsigned Table::getNodePath(unsigned destNode) {
    unsigned nextNode;
    cerr << "Get Node Path" << endl;
    double shortPath = numeric_limits<double>::infinity();
    double distanceToFirstNode;
    double distanceFromFirstToSecond;
    for ( map<unsigned, map<unsigned,double> >::iterator iter = nodeTotalMap.begin(); iter != nodeTotalMap.end(); iter++) {
        cerr << "checking firstnode = " << iter->first << endl;
        distanceToFirstNode = ((nodeTotalMap.find(thisNodeNumber))->second.find(iter->first))->second;
        if (iter->second.find(destNode) != iter->second.end()) {
            distanceFromFirstToSecond = (iter->second.find(destNode))->second;
        }
        else {
            distanceFromFirstToSecond = numeric_limits<double>::infinity();
        }

        if ((distanceToFirstNode + distanceFromFirstToSecond) < shortPath) {
            nextNode = iter->first;
            shortPath = (distanceToFirstNode + distanceFromFirstToSecond);
        }
    }

    cerr << "nextNode = " << nextNode << endl;
    return nextNode;
 }

//Returns true if the entry is new
bool Table::updateSingleEntry(unsigned neighborNumber, unsigned nodeNumber, double newValue) {
  map<unsigned, map<unsigned, double>>::iterator rowToUpdate = nodeTotalMap.find(neighborNumber);
  map<unsigned, double>::iterator nodeToUpdate = rowToUpdate->second.find(nodeNumber);
  if(nodeToUpdate == rowToUpdate->second.end()) {
    rowToUpdate->second.insert(make_pair(nodeNumber, newValue));
    return true;
  } else {
    nodeToUpdate->second = newValue;
    return false;
  }
}

ostream &Table::Print(ostream &os) const
{
  // WRITE THIS
  os << "Table()" << endl;
  os << "=========================" << endl;
  /*
  for ( map<unsigned, map<unsigned,double> >::iterator iter = nodeTotalMap.begin(); iter != nodeTotalMap.end(); iter++) {
        os << "Vector Number: " << (iter->first) << " ==   ";
        for (map<unsigned,double>::iterator looper = iter->second.begin(); looper != iter->second.end();looper++) {
            os << (looper->first) << "=>" << (looper->second) << "   ";
        }
        os << endl;

  }
  */
  //copy(nodeTotalVector.begin(), nodeTotalVector.end(), ostream_iterator<vector<double> >(os, "\n"));
  return os;
}
#endif
