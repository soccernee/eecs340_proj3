#include "table.h"

#if defined(GENERIC)
ostream & Table::Print(ostream &os) const
{
  // WRITE THIS
  os << "Table()";
  return os;
}
#endif

#if defined(LINKSTATE)

ostream & Table::Print(ostream &os) const
{
  // WRITE THIS
  os << "Table()";
  return os;
}
#endif

#if defined(DISTANCEVECTOR)

 void Table::setVector(vector<vector<double> > thisVector) {
    nodeTotalVector = thisVector;
 }

 void Table::updateVector(int vectorNumber, vector<double> thisNodeVector){
    nodeTotalVector[vectorNumber] = thisNodeVector;
 }

 vector<vector<double> > Table::getVector(){
    vector<vector<double> > tempVector = nodeTotalVector;
    return tempVector;
 }

 unsigned Table::getNodePath(unsigned destNode) {
    unsigned nextNode = 0;
    int shortPath = 10000000;
    int i = 0;
    for (vector<vector<double> >::iterator iter = nodeTotalVector.begin(); iter != nodeTotalVector.end(); iter++) {
        if ((*iter)[destNode] < shortPath) {
            nextNode = i;
            shortPath = (*iter)[destNode];
        }
        i++;
    }
    cerr << "nextNode = " << nextNode << endl;;
    return nextNode;
 }

ostream &Table::Print(ostream &os) const
{
  // WRITE THIS

  os << "Table()" << endl;
/*
    int i = 0;
  for (vector<vector<double> >::iterator iter = nodeTotalVector.begin(); iter != nodeTotalVector.end(); iter++) {
        os << "Vector Number: " << i << endl;
        int j = 0;
        for (vector<double>::iterator looper = nodeTotalVector[i].begin(); looper != nodeTotalVector[i].end(); looper++) {
            os << j << ": latency = " << *looper << endl;
            j++;
        }
        i++;
  }
*/
  //copy(nodeTotalVector.begin(), nodeTotalVector.end(), ostream_iterator<vector<double> >(os, "\n"));
  return os;
}
#endif
