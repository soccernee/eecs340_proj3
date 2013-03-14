#include "node.h"
#include "context.h"
#include "error.h"
#include "topology.h"

Node::Node(const unsigned n, SimulationContext *c, double b, double l) :
    number(n), context(c), bw(b), lat(l)
{
    cerr << "Node init" << endl;

    #if defined(DISTANCEVECTOR)
    thisNodeTable->tableInit(number);
    #endif
}

Node::Node()
{ throw GeneralException(); }

Node::Node(const Node &rhs) :
  number(rhs.number), context(rhs.context), bw(rhs.bw), lat(rhs.lat) {}

Node & Node::operator=(const Node &rhs)
{
  return *(new(this)Node(rhs));
}

void Node::SetNumber(const unsigned n)
{ number=n;}

unsigned Node::GetNumber() const
{ return number;}

void Node::SetLatency(const double l)
{ lat=l;}

double Node::GetLatency() const
{ return lat;}

void Node::SetBW(const double b)
{ bw=b;}

double Node::GetBW() const
{ return bw;}

Node::~Node()
{}

// Implement these functions  to post an event to the event queue in the event simulator
// so that the corresponding node can recieve the ROUTING_MESSAGE_ARRIVAL event at the proper time
void Node::SendToNeighbors(const RoutingMessage *m) {
  deque<Node *> * neighbors = GetNeighbors();
  for(deque<Node *>::iterator i = neighbors.begin(); i != neighbors.end(); i++) {
    SendToNeighbor(*i, m);
  }
}

void Node::SendToNeighbor(const Node *n, const RoutingMessage *m) {
  Link toMatch;
  toMatch.SetSrc(number);
  toMatch.SetDest(n->GetNumber());
  Link * matchingLink = context->FindMatchingLink(&toMatch);
  Event * event = new Event(context->GetTime() + matchingLink->GetLatency(), ROUTING_MESSAGE_ARRIVAL, n, m);
  context->PostEvent(event);
}

deque<Node*> *Node::GetNeighbors()
{
  return context->GetNeighbors(this);
}

void Node::SetTimeOut(const double timefromnow)
{
  context->TimeOut(this,timefromnow);
}


bool Node::Matches(const Node &rhs) const
{
  return number==rhs.number;
}


#if defined(GENERIC)
void Node::LinkHasBeenUpdated(const Link *l)
{
    cerr << "[GENERIC]  linkhasBeenUpdated";
  //cerr << *this << " got a link update: "<<*l<<endl;
  //Do Something generic:
  //SendToNeighbors(new RoutingMessage);
}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
    cerr << "[GENERIC]  processIncomingRoutingMessage  ";
  cerr << *this << " got a routing messagee: "<<*m<<" Ignored "<<endl;
}


void Node::TimeOut()
{
    cerr << "[GENERIC]  timeOut";
  cerr << *this << " got a timeout: ignored"<<endl;
}

Node *Node::GetNextHop(const Node *destination)
{
    cerr << "[GENERIC]  GetNextHop" << endl;

  return 0;
}

Table *Node::GetRoutingTable() const
{
    cerr << "[GENERIC]  GetRoutingTable called" << endl;
  return new Table;
}


ostream & Node::Print(ostream &os) const
{
  os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw<<")";
  return os;
}

#endif

#if defined(LINKSTATE)


void Node::LinkHasBeenUpdated(const Link *l)
{

  cerr << *this<<": Link Update: "<<*l<<endl;
}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
  cerr << *this << " Routing Message: "<<*m;
}

void Node::TimeOut()
{
  cerr << *this << " got a timeout: ignored"<<endl;
}

Node *Node::GetNextHop(const Node *destination) const
{
  // WRITE
  return 0;
}

Table *Node::GetRoutingTable() const
{
  // WRITE
  return 0;
}


ostream & Node::Print(ostream &os) const
{
  os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw<<")";
  return os;
}
#endif


#if defined(DISTANCEVECTOR)

void Node::setTable (Table *tbl) {
    cerr << "Set Table" << endl;
    thisNodeTable = tbl;
}

void Node::LinkHasBeenUpdated(const Link *l)
{
  cerr << "Updating link " << *l << endl;
  bool isLinkChanged = thisNodeTable->updateSingleEntry(number, l->GetDest, l->GetLatency());
  thisNodeTable->addRowIfNotExists(l->GetDest);
  thisNodeTable->updateVectorsThroughNeighbor(l->GetDest);
  if(isLinkChanged) {
    sendRoutingUpdate();
  }

}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m) {
    bool updated = thisNodeTable->updateMap(m->sourceNodeNumber, m->newTableRow);
    if(updated) {
      sendRoutingUpdate();
    }
}

void Node::TimeOut()
{
  cerr << *this << " got a timeout: ignored"<<endl;
}


Node *Node::GetNextHop(const Node *destination)
{
    cerr << "[DISTANCEVECTOR] GetNextHop" << endl;
    unsigned destinationNodeNumber = destination->GetNumber();
    cerr << "destination node = " << destinationNodeNumber << endl;
    unsigned nextNodeNumber = thisNodeTable->getNodePath(destinationNodeNumber);
    cerr << "Found out nextNode number" << endl;
    //now that we know the nextHop node number, we must find that node
    deque<Node*> * neighbors = GetNeighbors();
    for (deque<Node*>::iterator i = neighbors->begin(); i != neighbors->end(); i++) {
        if ((*iter)->GetNumber() == nextNodeNumber) {
            cerr << "GetNextHop: next node = " << **iter << endl;
            return *i;
        }
    }
    //note we should never get down here
    cerr << "Trouble. GetNextHop did not find the shortest path to the next hop and, by default, identifies the first neighbor as the shortest path." << endl;
    return *neighbors->begin();
}

Table *Node::GetRoutingTable() const
{
    Table *tempTable = thisNodeTable;
    cerr << "GetRoutingTable: \n" << tempTable << endl;
    return tempTable;
}

void Node::sendRoutingUpdate() const {
  RoutingMessage * message = new RoutingMessage(number, thisNodeTable->getRow(number));
  SendToNeighbors(message);
}

ostream & Node::Print(ostream &os) const
{
  os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw;
  return os;
}
#endif
