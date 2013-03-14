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


    //Update current routing table with discovered nodes from update
    map<unsigned, map<unsigned, double> >::iterator currentRow = nodeTotalMap.find(thisNodeNumber);
    for(map<unsigned, double>::iterator i = nodeMap.begin(); i != nodeMap.end(); i++) {
      unsigned newNodeNumber = i->first;

      map<unsigned, double>::iterator newNodeInCurrentRow = currentRow->second.find(newNodeNumber);
      if(newNodeInCurrentRow == currentRow->second.end()) {
        currentRow->second.insert(make_pair(newNodeNumber, numeric_limits<double>::infinity()));
        cerr << "Node " << thisNodeNumber << " discovered node " << newNodeNumber << " via update from node " << mapNumber << endl;
      }
    }

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
   cerr << endl << endl;
  cerr << "======Updating distance vector row for node " << thisNodeNumber << "======" << endl;
   bool ourDistanceVectorHasBeenUpdated = false;


   map<unsigned,map<unsigned,double> >::iterator ourDistanceVector = nodeTotalMap.find(thisNodeNumber);

   //Loop through all destinations
   for(map<unsigned, double>::iterator allDestinationsIterator = ourDistanceVector->second.begin(); allDestinationsIterator != ourDistanceVector->second.end(); allDestinationsIterator++) {
     unsigned destination = allDestinationsIterator->first;
     cerr << "Updating DV entry for destination node " << destination << endl;

     //Loop through neighbors
     for(map<unsigned, map<unsigned, double> >::iterator neighborsIterator = nodeTotalMap.begin(); neighborsIterator != nodeTotalMap.end(); neighborsIterator++) {
       unsigned neighbor = neighborsIterator->first;

       double distanceFromMeToNeighbor = numeric_limits<double>::infinity();
       double distanceFromNeighborToDestination = numeric_limits<double>::infinity();
       double oldDistance = numeric_limits<double>::infinity();



       map<unsigned, double>::iterator meToNeighbor = linkCostTable.find(neighbor);
       if(meToNeighbor != linkCostTable.end()) {
         distanceFromMeToNeighbor = meToNeighbor->second;
       }

       map<unsigned, double>::iterator neighborToDestination = neighborsIterator->second.find(destination);
       if(neighborToDestination != neighborsIterator->second.end()) {
         distanceFromNeighborToDestination = neighborToDestination->second;
       }

       map<unsigned, double>::iterator ourCurrentDistanceToDestInTable = ourDistanceVector->second.find(destination);
       if(ourCurrentDistanceToDestInTable != ourDistanceVector->second.end()) {
         oldDistance = ourCurrentDistanceToDestInTable->second;
       }

        double newDistance = distanceFromMeToNeighbor + distanceFromNeighborToDestination;

       cerr << "Distance from " << thisNodeNumber << " to neighbor " << neighbor << ": " <<  distanceFromMeToNeighbor << endl;
       cerr << "Distance from neighbor " << neighbor << " to destination " << destination << ": " << distanceFromNeighborToDestination << endl;
       cerr << "Old distance from node " << thisNodeNumber << " to destination " << destination << ": " <<  oldDistance << endl;
       cerr << "New distance from node " << thisNodeNumber << " to destination " << destination << ": " <<  newDistance << endl;
       if(newDistance < oldDistance) {
        //We have a new distance vector for node destination
        cerr << "Vector through neighbor " << neighbor << " is less than current distance, updating my row" << endl;
        //If the destination is not currently in the table, insert it
        if(ourCurrentDistanceToDestInTable == ourDistanceVector->second.end()) {
            cerr << "This is a new DV entry in the routing table row" << endl;
            ourDistanceVector->second.insert(make_pair(destination, newDistance));
        } else {
              cerr << "Updating current DV entry in my routing table row" << endl;
            ourCurrentDistanceToDestInTable->second = newDistance;
        }
        ourDistanceVectorHasBeenUpdated = true;

        //Also update forwarding table


        map<unsigned, unsigned>::iterator forwardingTableEntry = forwardingTable.find(destination);
        if(forwardingTableEntry == forwardingTable.end()) {
          forwardingTable.insert(make_pair(destination, neighbor));
        } else {
          forwardingTableEntry->second = neighbor;
        }
        cerr << "Updating forwarding table to forward nodes destined to " << destination << " via neighboring node " << neighbor <<endl;
      }


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

 //Returns true if entry is updated
bool Table::addOrUpdateNeighbor(unsigned neighborNumber, double linkCost) {
  bool isLinkCostChanged = false;

  map<unsigned, double>::iterator linkToUpdate = linkCostTable.find(neighborNumber);

  if(linkToUpdate == linkCostTable.end()) {
    cerr << "Adding to node " << thisNodeNumber << " neighbor " << neighborNumber << endl;
    //If this is a new link
    linkCostTable.insert(make_pair(neighborNumber, linkCost));
    map<unsigned, double> newRow;
    newRow.insert(make_pair(neighborNumber, 0.0));
    nodeTotalMap.insert(make_pair(neighborNumber, newRow));

    map<unsigned, map<unsigned, double> >::iterator myRow = nodeTotalMap.find(thisNodeNumber);
    if(myRow->second.find(neighborNumber) == myRow->second.end()) {
      myRow->second.insert(make_pair(neighborNumber, numeric_limits<double>::infinity()));
    }

    isLinkCostChanged = true;

  } else {
    cerr << "Updating link cost from node " << thisNodeNumber << " to neighbor " << neighborNumber << endl;
    cerr << "New cost is " << linkCost;
    //If this is a link update
    double oldLinkCost = linkToUpdate->second;
    linkToUpdate->second = linkCost;
    isLinkCostChanged = oldLinkCost != linkCost;
  }

  if(isLinkCostChanged) {
    cerr << "Link cost changed or was added" << endl;
    cerr << *this << endl;
    return updateVectorsThroughNeighbor(neighborNumber);

    return false;
  } else {
    return false;
  }

  /*map<unsigned, map<unsigned, double> >::iterator myRow = nodeTotalMap.find(thisNodeNumber);
  map<unsigned, double>::iterator entryToUpdate = myRow->second.find(nodeNumber);
  if(entryToUpdate)


  thisNodeTable->updateVectorsThroughNeighbor(l->GetDest());


  cerr << "Updating Table" << thisNodeNumber << "[" << thisNodeNumber << "," << nodeNumber << "] = " << newValue << endl;
  map<unsigned, map<unsigned, double> >::iterator rowToUpdate = nodeTotalMap.find(thisNodeNumber);
  map<unsigned, double>::iterator nodeToUpdate = rowToUpdate->second.find(nodeNumber);
  if(nodeToUpdate == rowToUpdate->second.end()) {
    //New entry in table
    rowToUpdate->second.insert(make_pair(nodeNumber, newValue));
    forwardingTable.insert(make_pair(nodeNumber, nodeNumber));
    return true;
  } else {
    double oldValue = nodeToUpdate->second;
    nodeToUpdate->second = newValue;
    return oldValue != newValue;
  }*/
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
  os << endl << endl << endl;
  os << "TABLES FOR NODE " << thisNodeNumber << endl;
  os << "==========Routing Table==============" << endl;
  for ( map<unsigned, map<unsigned,double> >::const_iterator iter = nodeTotalMap.begin(); iter != nodeTotalMap.end(); iter++) {
        os << "Neighbor: " << (iter->first) << " ==   ";
        for (map<unsigned,double>::const_iterator looper = iter->second.begin(); looper != iter->second.end();looper++) {
            os << (looper->first) << "=>" << (looper->second) << "   ";
        }
        os << endl;
  }

  os << "===========Forwarding Table============" << endl;
  for (map<unsigned, unsigned>::const_iterator forwardingTableIter = forwardingTable.begin(); forwardingTableIter != forwardingTable.end(); forwardingTableIter++) {
        os << "Dest Node: " << forwardingTableIter->first << "\t Neighbor Node: " << forwardingTableIter->second << endl;
  }

  os << "===========Link Cost Table============" << endl;
  for (map<unsigned, double>::const_iterator linkCostTableIter = linkCostTable.begin(); linkCostTableIter != linkCostTable.end(); linkCostTableIter++) {
        os << "Neighbor Node: " << linkCostTableIter->first << "\t Cost: " << linkCostTableIter->second << endl;
  }

  os << endl << endl << endl;
  return os;
}
#endif
