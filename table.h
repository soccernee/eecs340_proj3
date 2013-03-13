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
#include <vector>
#include <iterator>

class Table {

vector<vector<double> > nodeTotalVector;

 public:
 Table();
 void setVector(vector<vector<double> > thisVector);
 void updateVector(int vectorNumber, vector<double> thisNodeVector);
 vector<vector<double> > getVector();
 unsigned getNodePath(unsigned destNode);
 ostream & Print(ostream &os) const;
};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
