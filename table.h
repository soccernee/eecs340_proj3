#ifndef _table
#define _table


#include <iostream>
#include <stdlib.h>
#include <limits>

using namespace std;





#if defined(GENERIC)
class Table {
  // Students should write this class

 public:
  ostream & Print(ostream &os) const;
};
#endif


#if defined(LINKSTATE)

#include "node.h"
#include "link.h"
#include <deque>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <iterator>


class Table {
  // Students should write this class
  private:

  map<unsigned, map<unsigned,double> > connectionsLinks;
  map<unsigned, double>routingTableDistance;
  map<unsigned, unsigned>routingTablePrevNode;
  map<unsigned, unsigned> forwardingTable;

  set<unsigned> allNodesInNetwork;
  set<unsigned> distanceCalculatedNodes;
  set<unsigned> uncalculatedNodes;

  unsigned thisNodeNumber;

  unsigned findNeighbor(unsigned destNode, unsigned prevNode);


  public:
  Table(unsigned nodeNumber);
  void updateMap (unsigned neighborNode, map<unsigned,map<unsigned,double> >connectionsLinksFromNeighbor);
  void updateLinkCost(unsigned neighborNumber, double linkCost);
  void performDijkstraAlgorithm();
  map<unsigned, map<unsigned,double> > getConnectionsLinks();
  unsigned getNodePath(unsigned destNode);
   void updateForwardingTable();


  ostream & Print(ostream &os) const;
};

#endif

#if defined(DISTANCEVECTOR)

#include "node.h"
#include "link.h"
#include <deque>
#include <map>
#include <iterator>

class Table {
private:

 map<unsigned, map<unsigned,double> > nodeTotalMap;
 map<unsigned, unsigned> forwardingTable;
 map<unsigned, double> linkCostTable;

 unsigned thisNodeNumber;

 public:
 Table(unsigned nodeNumber);
 bool addOrUpdateNeighbor(unsigned neighborNumber, double linkCost);
 bool updateMap(unsigned mapNumber, map<unsigned,double> nodeMap);
 void updateForwardingTable();
 bool updateVectorsThroughNeighbor(unsigned neighborNumber);
 void addRowIfNotExists(unsigned rowNumber);
 unsigned getNodePath(unsigned destNode);
 ostream & Print(ostream &os) const;
 map<unsigned, double> getRow(unsigned rowNumber);
};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
