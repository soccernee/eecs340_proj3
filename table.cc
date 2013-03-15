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


Table::Table(unsigned nodeNumber) {
    cerr << "table constructor" << endl;

    map<unsigned,double> initMap;
    initMap.insert(make_pair(nodeNumber,0.0));
    connectionsLinks.insert(make_pair(nodeNumber, initMap));
    thisNodeNumber = nodeNumber;
}

void Table::updateMap (unsigned neighborNode, map<unsigned,map<unsigned,double> >connectionsLinksFromNeighbor) {
    cerr << "Table: update from neighbors links" << endl;

        cerr << "Received connectionsLinkFromNeighbor from node: " << neighborNode << endl;
        for (map<unsigned, map<unsigned,double> >::iterator iter = connectionsLinksFromNeighbor.begin(); iter!= connectionsLinksFromNeighbor.end(); iter++ ) {

            map<unsigned, map<unsigned,double> >::iterator iterMyConnections = connectionsLinks.find(iter->first);
            for (map<unsigned,double>::iterator updateNetworkIter = iter->second.begin(); updateNetworkIter != iter->second.end(); updateNetworkIter++) {
                    allNodesInNetwork.insert(updateNetworkIter->first);
            }

            if (iterMyConnections == connectionsLinks.end()) {
                //add this row to the table
                cerr << "at node:" << thisNodeNumber <<  " adding new row to table for node " << iter->first<< endl;
                connectionsLinks.insert(make_pair(iter->first, iter->second));
            }
            else if (iterMyConnections->first != thisNodeNumber && (iterMyConnections->second.size() < iter->second.size())){
                //update table with new links
                cerr << "at node:" << thisNodeNumber <<  " updating row to table for node " << iter->first<< endl;
                connectionsLinks.erase(iterMyConnections);
                connectionsLinks.insert(make_pair(iter->first, iter->second));
            }//end else
        }//end for loop
        cerr << "end of UpdateMap" << endl;
        cerr << *this << endl;
}//end function

void Table::updateLinkCost(unsigned neighborNumber, double linkCost) {
    cerr << "Function call to updateLinkCost; cost to node " << neighborNumber << "=" << linkCost << endl;
    if (connectionsLinks.find(thisNodeNumber)->second.find(neighborNumber) == connectionsLinks.find(thisNodeNumber)->second.end()) {
        //this is a new link
        cerr << "this is a new link" << endl;
        allNodesInNetwork.insert(neighborNumber);
        connectionsLinks.find(thisNodeNumber)->second.insert(make_pair(neighborNumber,linkCost));
    }
    else {
        //update preexisting link
        cerr << "preexisting link" << endl;
        connectionsLinks.find(thisNodeNumber)->second.find(neighborNumber)->second = linkCost;
    }

    cerr << *this << endl;
}

void Table::performDijkstraAlgorithm() {

    allNodesInNetwork.erase(thisNodeNumber);
    std::copy(allNodesInNetwork.begin(), allNodesInNetwork.end(),std::inserter( uncalculatedNodes, uncalculatedNodes.begin() ) );

    distanceCalculatedNodes.erase(distanceCalculatedNodes.begin(), distanceCalculatedNodes.end());
    distanceCalculatedNodes.insert(thisNodeNumber);

    routingTableDistance.erase(routingTableDistance.begin(), routingTableDistance.end());
    routingTablePrevNode.erase(routingTablePrevNode.begin(), routingTablePrevNode.end());

    double inf = numeric_limits<double>::infinity();
    for (set<unsigned>::iterator allNodesIter = allNodesInNetwork.begin(); allNodesIter != allNodesInNetwork.end(); allNodesIter++) {
        map<unsigned,map<unsigned,double> >::iterator connectionIter = connectionsLinks.find(thisNodeNumber);
        if (connectionIter->second.find(*allNodesIter) != connectionIter->second.end())    {
            //neighbor node; therefore set distance = link cost
            routingTableDistance.insert(make_pair(*allNodesIter, connectionIter->second.find(*allNodesIter)->second));
            routingTablePrevNode.insert(make_pair(*allNodesIter, thisNodeNumber));
        }
        else {
            //non-neighbor node; set distance = infinity
            routingTableDistance.insert(make_pair(*allNodesIter, inf));
            routingTablePrevNode.insert(make_pair(*allNodesIter, NULL));
        }
    }



    while (uncalculatedNodes.size() != 0) {

        unsigned nodeShortestDistance = thisNodeNumber;
        double shortestDistance = numeric_limits<double>::infinity();

        //find shortest path
        for (set<unsigned>::iterator ii = uncalculatedNodes.begin(); ii != uncalculatedNodes.end(); ii++) {

            double distance = routingTableDistance.find(*ii)->second;
            if (distance < shortestDistance) {
                shortestDistance = distance;
                nodeShortestDistance = *ii;
            }

        }
        distanceCalculatedNodes.insert(nodeShortestDistance);
        uncalculatedNodes.erase(nodeShortestDistance);

        double distanceToShortestNode = routingTableDistance.find(nodeShortestDistance)->second;

        for (map<unsigned,double>::iterator neighborIter = connectionsLinks.find(nodeShortestDistance)->second.begin(); neighborIter != connectionsLinks.find(nodeShortestDistance)->second.end(); neighborIter++) {
            if (distanceToShortestNode + (neighborIter->second) < routingTableDistance.find(neighborIter->first)->second) {
                    routingTableDistance.find(neighborIter->first)->second = distanceToShortestNode + neighborIter->second;
                    routingTablePrevNode.find(neighborIter->first)->second = nodeShortestDistance;
            }
        }
        if (nodeShortestDistance == thisNodeNumber) break;

    }
    cerr << *this << endl;
    updateForwardingTable();
}

unsigned Table::findNeighbor(unsigned destNode, unsigned prevNode) {
    cerr << "find your Neighbor" << endl;
    cerr << "destNode = " << destNode << endl;
    cerr << "prevNode = " << prevNode << endl;
    if (prevNode == thisNodeNumber) {
        cerr << "return destNode = " << destNode << endl;
        return destNode;
    }
    else return findNeighbor(prevNode, routingTablePrevNode.find(prevNode)->second);
}


void Table::updateForwardingTable() {

    for (map<unsigned,unsigned>::iterator jj = routingTablePrevNode.begin(); jj != routingTablePrevNode.end(); jj++) {
        unsigned neighbor = findNeighbor(jj->first, jj->second);
        forwardingTable.insert(make_pair(jj->first, neighbor));
    }
}

map<unsigned, map<unsigned,double> > Table::getConnectionsLinks() {
    return connectionsLinks;
}

unsigned Table::getNodePath(unsigned destNode) {
    return forwardingTable.find(destNode)->second;
}


ostream & Table::Print(ostream &os) const
{
  // WRITE THIS
  os << endl << endl;
  os << "TABLES FOR NODE " << thisNodeNumber << endl;
  os << "==========Connection Costs Table==============" << endl;
  for ( map<unsigned, map<unsigned,double> >::const_iterator iter = connectionsLinks.begin(); iter != connectionsLinks.end(); iter++) {
        os << "Neighbor: " << (iter->first) << " ==   ";
        for (map<unsigned,double>::const_iterator looper = iter->second.begin(); looper != iter->second.end();looper++) {
            os << (looper->first) << "=>" << (looper->second) << "   ";
        }
        os << endl;
  }

   os << "===========Routing Table============" << endl;
   map<unsigned,unsigned>::const_iterator routingTablePrevNodeIter;
  for (map<unsigned, double>::const_iterator routingTableDistanceIter = routingTableDistance.begin(); routingTableDistanceIter != routingTableDistance.end(); routingTableDistanceIter++) {
        routingTablePrevNodeIter = routingTablePrevNode.find(routingTableDistanceIter->first);
        if (routingTablePrevNodeIter != routingTablePrevNode.end()) {
            os << "Node: " << routingTableDistanceIter->first << "\t Distance(previousNode): " << routingTableDistanceIter->second << "(" << routingTablePrevNodeIter->second << ")" <<endl;
        }
        else {
            os << "Node: " << routingTableDistanceIter->first << "\t Distance(previousNode): " << routingTableDistanceIter->second << "(---)" <<endl;
        }
  }

   os << "===========Forwarding Table============" << endl;
  for (map<unsigned, unsigned>::const_iterator forwardingTableIter = forwardingTable.begin(); forwardingTableIter != forwardingTable.end(); forwardingTableIter++) {
        os << "Dest Node: " << forwardingTableIter->first << "\t Neighbor Node: " << forwardingTableIter->second << endl;
  }

    os << "===========All Nodes in Network============" << endl;
   for (set<unsigned>::const_iterator nodesIter = allNodesInNetwork.begin(); nodesIter != allNodesInNetwork.end(); nodesIter++) {
        os << "\t" << *nodesIter;
   }
   os << endl << endl;;

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
    isLinkCostChanged = (oldLinkCost != linkCost);

    nodeTotalMap.find(thisNodeNumber)->second.find(neighborNumber)->second = linkCost;
    cerr << "UPDATE: new distance vector value from node " <<thisNodeNumber << " to node " << neighborNumber << " is changed to " << ((nodeTotalMap.find(thisNodeNumber))->second.find(neighborNumber))->second << endl;
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
