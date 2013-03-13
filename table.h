#ifndef _table
#define _table


#include <iostream>
#include <stdlib.h>

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

 public:
 Table();
 void tableInit();
 void setMap(map<unsigned, map<unsigned,double> > thisMap);
 void updateMap(unsigned mapNumber, map<unsigned,double> thisNodeMap);
 map<unsigned, map<unsigned,double> > getMap();
 unsigned getNodePath(unsigned destNode);
 ostream & Print(ostream &os) const;
};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
