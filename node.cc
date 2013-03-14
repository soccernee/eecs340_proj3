#include "node.h"
#include "context.h"
#include "error.h"
#include "topology.h"

Node::Node(const unsigned n, SimulationContext *c, double b, double l) :
    number(n), context(c), bw(b), lat(l)
{

    if(context == NULL) {
      cerr << "Creating Node with no context" << endl;
    } else {
      cerr << "Creating node " << number << endl;
    }

    #if defined(DISTANCEVECTOR)
    thisNodeTable = new Table(number);
    #endif
}

Node::Node()
{ throw GeneralException(); }

Node::Node(const Node &rhs) :
  number(rhs.number), context(rhs.context), bw(rhs.bw), lat(rhs.lat) {
    cerr << "Creating node with copy constructor" << endl;
  }

Node & Node::operator=(const Node &rhs)
{
  cerr << "Creating node using equality operator" << endl;
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
void Node::SendToNeighbors( RoutingMessage *m) {
  cerr << "Send message from node " << number << " to all neighbors" << endl;
  deque<Node *> * neighbors = GetNeighbors();
  for(deque<Node *>::iterator i = neighbors->begin(); i != neighbors->end(); i++) {
    SendToNeighbor(*i, m);
  }
}

void Node::SendToNeighbor( Node *n,  RoutingMessage *m) {
  cerr << "Send message from node " << number << " to neighbor " << n->GetNumber() << endl;
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
  cerr << "Link updated in node " << number << " link " << *l << endl;
  bool isLinkChanged = thisNodeTable->addOrUpdateNeighbor(l->GetDest(), l->GetLatency());
  if(isLinkChanged) {
    sendRoutingUpdate();
  }

}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m) {
  cerr << "Procesing incoming routing message in node " << number << " from node " << m->sourceNodeNumber << endl;
    bool updated = thisNodeTable->updateMap(m->sourceNodeNumber, m->newTableRow);
    if(updated) {
      cerr << "Node " << number << "'s routing table was updated, sending update to neighbors" << endl;
      sendRoutingUpdate();
    }
}

void Node::TimeOut()
{
  cerr << *this << " got a timeout: ignored"<<endl;
}

Node *Node::GetNextHop(const Node *destination) {
    cerr << "Getting next hop to destination " << *destination << " from node " << number << endl;
    unsigned nextNode = thisNodeTable->getNodePath(destination->GetNumber());
    deque<Node *> * neighbors = GetNeighbors();
    for(deque<Node *>::iterator i = neighbors->begin(); i != neighbors->end(); i++) {
      if((*i)->GetNumber() == nextNode) {
        return *i;
      }
    }
    return NULL;
}

Table *Node::GetRoutingTable() const
{
    Table *tempTable = thisNodeTable;
    cerr << "GetRoutingTable: \n" << tempTable << endl;
    return tempTable;
}

void Node::sendRoutingUpdate()  {
  cerr << "Sending routing update from node " << number << endl;
  RoutingMessage * message = new RoutingMessage(number, thisNodeTable->getRow(number));
  SendToNeighbors(message);
}

ostream & Node::Print(ostream &os) const
{
  os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw;
  return os;
}
#endif
