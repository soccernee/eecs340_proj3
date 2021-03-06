#ifndef _node
#define _node

#include <new>
#include <iostream>
#include <deque>


class RoutingMessage;
class Table;
class Link;
class SimulationContext;

#include "table.h"

using namespace std;

class Node {
 private:
  unsigned number;
  SimulationContext    *context;
  double   bw;
  double   lat;

#if defined(GENERIC)
  public:
  //
  // Students will WRITE THESE
  //
  virtual void LinkHasBeenUpdated(const Link *l);
  virtual void ProcessIncomingRoutingMessage(const RoutingMessage *m);
  virtual void TimeOut();
  virtual Node *GetNextHop(const Node *destination);
  virtual Table *GetRoutingTable() const;

  virtual ostream & Print(ostream &os) const;
#endif

#if defined(LINKSTATE)

  private:
  Table *thisNodeTable;
  virtual void sendRoutingUpdate();

  public:
  //
  // Students will WRITE THESE
  //
  virtual void LinkHasBeenUpdated(const Link *l);
  virtual void ProcessIncomingRoutingMessage(const RoutingMessage *m);
  virtual void TimeOut();
  virtual Node *GetNextHop(const Node *destination);
  virtual Table *GetRoutingTable() const;

  virtual ostream & Print(ostream &os) const;
#endif

#if defined(DISTANCEVECTOR)
// students will add protocol-specific data here
  private:
  Table *thisNodeTable;
  virtual void sendRoutingUpdate();

  public:
  virtual void setTable (Table *tbl);
  //
  // Students will WRITE THESE
  //
  virtual void LinkHasBeenUpdated(const Link *l);
  virtual void ProcessIncomingRoutingMessage(const RoutingMessage *m);
  virtual void TimeOut();
  virtual Node *GetNextHop(const Node *destination);
  virtual Table *GetRoutingTable() const;

  virtual ostream & Print(ostream &os) const;

#endif



 public:
  Node(const unsigned n, SimulationContext *c, double b, double l);
  Node();
  Node(const Node &rhs);
  Node & operator=(const Node &rhs);
  virtual ~Node();

  virtual bool Matches(const Node &rhs) const;

  virtual void SetNumber(const unsigned n);
  virtual unsigned GetNumber() const;

  virtual void SetLatency(const double l);
  virtual double GetLatency() const;
  virtual void SetBW(const double b);
  virtual double GetBW() const;

  virtual void SendToNeighbors( RoutingMessage *m);
  virtual void SendToNeighbor( Node *n,  RoutingMessage *m);
  virtual deque<Node*> *GetNeighbors();
  virtual void SetTimeOut(const double timefromnow);





};

inline ostream & operator<<(ostream &os, const Node &n) { return n.Print(os);}


#endif
