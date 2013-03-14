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

Table::Table() {
    cerr << "table constructor" << endl;
}

void Table::tableInit() {
    cerr << "table init" << endl;
    //nodeTotalMap.insert (std::pair<unsigned,double>(0,7));
}

 void Table::setMap(map<unsigned, map<unsigned,double> > thisMap) {
    nodeTotalMap = thisMap;
 }

 void Table::updateMap(unsigned mapNumber, map<unsigned,double> thisNodeMap){
    cerr << "Table: updateVector" << endl;
    nodeTotalMap.insert(pair<unsigned,map<unsigned,double> > (mapNumber, thisNodeMap));
    cerr << "done with UpdateVector" << endl;
 }

 map<unsigned, map<unsigned,double> > Table::getMap(){
    map<unsigned, map<unsigned,double> > tempMap = nodeTotalMap;
    return tempMap;
 }

 map<unsigned, double> Table::getRow(unsigned rowNumber) {
   return nodeTotalMap.find(rowNumber)->second;
 }

 unsigned Table::getNodePath(unsigned destNode) {
    unsigned nextNode = 0;
    cerr << "Get Node Path" << endl;
    double shortPath = numeric_limits<double>::infinity();
    for ( map<unsigned, map<unsigned,double> >::iterator iter = nodeTotalMap.begin(); iter != nodeTotalMap.end(); iter++) {
        cerr << "inside first loop" << endl;

        if ((iter->second.find(destNode))->second < shortPath) {
            nextNode = iter->first;
            shortPath = (iter->second.find(destNode))->second;
        }
    }

    cerr << "nextNode = " << nextNode << endl;
    return nextNode;
 }

void Table::updateSingleEntry(unsigned neighborNumber, unsigned nodeNumber, double newValue) {
  nodeTotalMap.find(neighborNumber)->second.find(nodeNumber)->second = newValue;
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
