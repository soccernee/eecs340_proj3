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
class Table {
  // Students should write this class
 public:
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


 map<unsigned, map<unsigned,double> > nodeTotalMap;
 map<unsigned, unsigned> forwardingTable;


 unsigned thisNodeNumber;

 public:
 Table();
 void tableInit(unsigned nodeNumber);
 bool updateSingleEntry(unsigned neighborNumber, unsigned nodeNumber, double newValue);
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
