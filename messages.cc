#include "messages.h"


#if defined(GENERIC)
ostream &RoutingMessage::Print(ostream &os) const
{
  os << "RoutingMessage()";
  return os;
}
#endif


#if defined(LINKSTATE)

ostream &RoutingMessage::Print(ostream &os) const
{
  return os;
}

RoutingMessage::RoutingMessage()
{}


RoutingMessage::RoutingMessage(const RoutingMessage &rhs) {

}

#endif


#if defined(DISTANCEVECTOR)

ostream &RoutingMessage::Print(ostream &os) const
{
  os << "Routing message from node " << sourceNodeNumber;
  for(map<unsigned, double>::const_iterator i = newTableRow.begin(); i != newTableRow.end(); i++) {
    os << " DestNode" << i->first << ": " << i->second << ";";
  }
  return os;
}


RoutingMessage::RoutingMessage() {

}


RoutingMessage::RoutingMessage(const RoutingMessage &rhs) {
  sourceNodeNumber = rhs.sourceNodeNumber;
  newTableRow = rhs.newTableRow;
}

RoutingMessage::RoutingMessage (unsigned sourceNodeNumber, map<unsigned, double> newTableRow) {
  this->sourceNodeNumber = sourceNodeNumber;
  this->newTableRow = newTableRow;
}

#endif

