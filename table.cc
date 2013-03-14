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

    if(nodeTotalMap.find(mapNumber) != nodeTotalMap.end()) {
        nodeTotalMap.erase(mapNumber);
    }
    nodeTotalMap.insert(make_pair(mapNumber, nodeMap));

    bool ourDistanceVectorHasBeenUpdated = false;

    map<unsigned,map<unsigned,double> >::iterator ourDistanceVector = nodeTotalMap.find(mapNumber);

    double distanceFromUsToNewNode = ourDistanceVector->map.find(mapNumber)->second;

    for (map<unsigned,double>::iterator iter = nodeMap.begin(); iter != nodeMap.end(); iter++) {

        double distanceFromNewNodeToDestination = nodeMap.find(iter->first)->second;

        double newDistance = distanceFromUsToNewNode + distanceFromNewNodeToDestination;

        if(newDistance < iter->second) {
            //We have a new distance vector for node iter->first
            map<unsigned, double>::iterator ourDistanceVectorToDest = ourDistanceVector->second.find(iter->first);

            //If the destination is not currently in the table, insert it
            if(ourDistanceVectorToDest == ourDistanceVector->second.end()) {
                ourDistanceVector->second.insert(make_pair(iter->first, newDistance));
            } else {
                ourDistanceVector->second.find(iter->first)->second = newDistance;
            }
            ourDistanceVectorHasBeenUpdated = true;
            //Also update forwarding table
        }
    }




    cerr << "done with UpdateVector" << endl;
    return ourDistanceVectorHasBeenUpdated;
 }

 map<unsigned, map<unsigned,double> > Table::getMap(){
    map<unsigned, map<unsigned,double> > tempMap = nodeTotalMap;
    return tempMap;
 }

 unsigned Table::getNodePath(unsigned destNode) {
    unsigned nextNode;
    cerr << "Get Node Path" << endl;
    double shortPath = 100000; //needs to be replaced with infinity
    double distanceToFirstNode;
    double distanceFromFirstToSecond;
    for ( map<unsigned, map<unsigned,double> >::iterator iter = nodeTotalMap.begin(); iter != nodeTotalMap.end(); iter++) {
        cerr << "checking firstnode = " << iter->first << endl;
        distanceToFirstNode = ((nodeTotalMap.find(thisNodeNumber))->second.find(iter->first))->second;
        if (iter->second.find(destNode) != iter->second.end()) {
            distanceFromFirstToSecond = (iter->second.find(destNode))->second;
        }
        else {
            distanceFromFirstToSecond = 100000; //needs to be replaced with infinity
        }

        if ((distanceToFirstNode + distanceFromFirstToSecond) < shortPath) {
            nextNode = iter->first;
            shortPath = (distanceToFirstNode + distanceFromFirstToSecond);
        }
    }

    cerr << "nextNode = " << nextNode << endl;;
    return nextNode;
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
