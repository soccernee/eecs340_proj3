#ifndef _messages
#define _messages

#include <iostream>

#include "node.h"
#include "link.h"
#include <map>

#if defined(GENERIC)
struct RoutingMessage {
 public:
  ostream & Print(ostream &os) const;
};
#endif

#if defined(LINKSTATE)
struct RoutingMessage {

  RoutingMessage();
  RoutingMessage(const RoutingMessage &rhs);
  RoutingMessage &operator=(const RoutingMessage &rhs);
  RoutingMessage(unsigned sourceNodeNumber, map<unsigned, map<unsigned,double> > costTable);

  unsigned sourceNodeNumber;
  map<unsigned, map<unsigned,double> >costTable;

  ostream & Print(ostream &os) const;
};
#endif

#if defined(DISTANCEVECTOR)
struct RoutingMessage {

  RoutingMessage();
  RoutingMessage(const RoutingMessage &rhs);
  RoutingMessage &operator=(const RoutingMessage &rhs);
  RoutingMessage (unsigned sourceNodeNumber, map<unsigned, double> newTableRow);

  unsigned sourceNodeNumber;
  map<unsigned, double> newTableRow;

  ostream & Print(ostream &os) const;
};
#endif


inline ostream & operator<<(ostream &os, const RoutingMessage &m) { return m.Print(os);}

#endif
