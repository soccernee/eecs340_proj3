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
    initMap.insert(make_pair(nodeNumber,0));
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

    double distanceFromUsToNewNode = ourDistanceVector->second.find(mapNumber)->second;

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

    updateForwardingTable();
    cerr << "done with UpdateVector" << endl;
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

 map<unsigned, map<unsigned,double> > Table::getMap(){
    map<unsigned, map<unsigned,double> > tempMap = nodeTotalMap;
    return tempMap;
 }

 unsigned Table::getNodePath(unsigned destNode) {

    return forwardingTable.find(destNode)->second;


/*
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
*/
 }

ostream & Table::Print(ostream &os) const
{
  // WRITE THIS
  os << "Routing Table()" << endl;
  os << "=========================" << endl;
  /*
  for ( map<unsigned, map<unsigned,double> >::iterator iter = nodeTotalMap.begin(); iter != nodeTotalMap.end(); iter++) {
        os << "Vector Number: " << (iter->first) << " ==   ";
        for (map<unsigned,double>::iterator looper = iter->second.begin(); looper != iter->second.end();looper++) {
            os << (looper->first) << "=>" << (looper->second) << "   ";
        }
        os << endl;
  }

  os << "===========Forwarding Table============" << endl;
  for (map<unsigned, unsigned>::iterator forwardingTableIter = forwardingTable.begin(); forwardingTableIter != forwardingTable.end(); forwardingTableIter++) {
        os << "Dest Node: " << forwardingTableIter->first << "\t Neighbor Node: " << forwardingTableIter->second << endl;
  }
  os << endl;
*/
  return os;
}
#endif
