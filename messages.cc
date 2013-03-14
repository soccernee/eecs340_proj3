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
  os << "Routing message from node " << sourceNodeNumber << " containing row " << newTableRow;
  return os;
}


RoutingMessage::RoutingMessage() {

}


RoutingMessage::RoutingMessage(const RoutingMessage &rhs) {
  sourceNodeNumber = rhs.sourceNodeNumber;
  newTableRow = rhs.newTableRow;
}

#endif

