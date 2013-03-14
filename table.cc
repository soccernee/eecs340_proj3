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

Table::Table(unsigned nodeNumber) {
    cerr << "table constructor" << endl;

    map<unsigned,double> initMap;
    initMap.insert(make_pair(nodeNumber,0.0));
    nodeTotalMap.insert(make_pair(nodeNumber, initMap));
    thisNodeNumber = nodeNumber;
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
   map<unsigned, map<unsigned, double> >::iterator neighborDistanceVector = nodeTotalMap.find(neighborNumber);


   double distanceFromUsToNeighbor = ourDistanceVector->second.find(thisNodeNumber)->second;
    cerr << "Distance from me to neighbor " << distanceFromUsToNeighbor << endl;
   for (map<unsigned,double>::iterator iter = neighborDistanceVector->second.begin(); iter != neighborDistanceVector->second.end(); iter++) {
      cerr << "Updating my vector for destination " << iter->first << endl;


      double distanceFromNewNodeToDestination = iter->second;
      cerr << "Distance from neighbor to destination " << distanceFromNewNodeToDestination << endl;


      double newDistance = distanceFromUsToNeighbor + distanceFromNewNodeToDestination;

      cerr << "Distance from me to destination via neighbor" << newDistance << endl;

      double oldDistance = numeric_limits<double>::infinity();
      map<unsigned, double>::iterator ourCurrentDistanceToDestInTable = ourDistanceVector->second.find(iter->first);
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
        map<unsigned, unsigned>::iterator forwardingTableEntry = forwardingTable.find(iter->first);
        if(forwardingTableEntry == forwardingTable.end()) {
          forwardingTable.insert(make_pair(iter->first, neighborNumber));
        } else {
          forwardingTableEntry->second = neighborNumber;
        }
        cerr << "Updating forwarding table to forward nodes destined to " << iter->first << " via neighboring node " << neighborNumber <<endl;
      }
    }
    cerr << *this << endl;
    return ourDistanceVectorHasBeenUpdated;
 }

 void Table::updateForwardingTable() {
        forwardingTable.clear();
        //iterate through each node in network
        for (map<unsigned, double>::iterator routingTableIter = nodeTotalMap.find(thisNodeNumber)->second.begin(); routingTableIter != nodeTotalMap.find(thisNodeNumber)->second.end(); routingTableIter++) {

                double shortestPath = 10000;
                unsigned nodeWithShortestPath;

                for (map<unsigned,map<unsigned,double> >::iterator neighborNodeIter = nodeTotalMap.begin(); neighborNodeIter != nodeTotalMap.end(); neighborNodeIter++) {

                    double distanceFromUsToNeighborNode = nodeTotalMap.find(thisNodeNumber)->second.find(neighborNodeIter->first)->second;
                    double distancefromNeighborNodeToDestNode = (nodeTotalMap.find(neighborNodeIter->first))->second.find(routingTableIter->first)->second;
                    double totalDistance = distancefromNeighborNodeToDestNode + distanceFromUsToNeighborNode;

                    if(totalDistance < shortestPath) {
                        shortestPath = totalDistance;
                        nodeWithShortestPath = neighborNodeIter->first;
                    }

                }

            forwardingTable.insert(make_pair(routingTableIter->first,nodeWithShortestPath));
            //first entry: destination Node in network
            //second entry: neighbor node that packet should be sent to in order to reach the destination Node

        }//end of for loop that iterates through each node in network
 } //end of functoin updateForwardingTable

 //Returns true if the entry is new
bool Table::updateSingleEntry(unsigned neighborNumber, unsigned nodeNumber, double newValue) {
  map<unsigned, map<unsigned, double> >::iterator rowToUpdate = nodeTotalMap.find(neighborNumber);
  map<unsigned, double>::iterator nodeToUpdate = rowToUpdate->second.find(nodeNumber);
  if(nodeToUpdate == rowToUpdate->second.end()) {
    rowToUpdate->second.insert(make_pair(nodeNumber, newValue));
    return true;
  } else {
    nodeToUpdate->second = newValue;
    return false;
  }
}

 unsigned Table::getNodePath(unsigned destNode) {

    return forwardingTable.find(destNode)->second;


/*
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
*/
 }

map<unsigned, double> Table::getRow(unsigned rowNumber) {
  return nodeTotalMap.find(rowNumber)->second;
}

ostream &Table::Print(ostream &os) const
{
  os << "Routing Table()" << endl;
  os << "=========================" << endl;
  for ( map<unsigned, map<unsigned,double> >::const_iterator iter = nodeTotalMap.begin(); iter != nodeTotalMap.end(); iter++) {
        os << "Vector Number: " << (iter->first) << " ==   ";
        for (map<unsigned,double>::const_iterator looper = iter->second.begin(); looper != iter->second.end();looper++) {
            os << (looper->first) << "=>" << (looper->second) << "   ";
        }
        os << endl;
  }

  os << "===========Forwarding Table============" << endl;
  for (map<unsigned, unsigned>::const_iterator forwardingTableIter = forwardingTable.begin(); forwardingTableIter != forwardingTable.end(); forwardingTableIter++) {
        os << "Dest Node: " << forwardingTableIter->first << "\t Neighbor Node: " << forwardingTableIter->second << endl;
  }
  os << endl;
  return os;
}
#endif
