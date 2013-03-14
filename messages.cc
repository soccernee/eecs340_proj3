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
    os << "Routing message from node " << sourceNodeNumber << endl;
    for (map<unsigned, map<unsigned,double> >::const_iterator i = costTable.begin(); i != costTable.end(); i++) {
        os << i->first << ": ";
        for (map<unsigned,double>::const_iterator j = i->second.begin(); j != i->second.end(); j++) {
            os << j->first << "(" << j->second << ") ";
        }
        os << endl;
    }

    return os;
}

RoutingMessage::RoutingMessage()
{}


RoutingMessage::RoutingMessage(const RoutingMessage &rhs) {
    sourceNodeNumber = rhs.sourceNodeNumber;
    costTable = rhs.costTable;
}

RoutingMessage::RoutingMessage(unsigned sourceNodeNumber, map<unsigned, map<unsigned,double> > costTable) {
    this->sourceNodeNumber = sourceNodeNumber;
    this->costTable = costTable;
}




#endif


#if defined(DISTANCEVECTOR)

ostream &RoutingMessage::Print(ostream &os) const
{
  os << "Routing message from node " << sourceNodeNumber << endl;
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

