#ifndef FLOWGRAPH_HH
#define FLOWGRAPH_HH
#include <string>
#include <iostream>
#include <vector>
#include "block.hh"
#include "quad.hh"
#include "quadcontainer.hh"

using namespace std;

class FlowGraph
{
  public:
    FlowGraph(std::vector<Block*>* blockList, EntryBlock* entry, EndingBlock* ending);
    FlowGraph(std::vector<QuadContainer*>* quadList);
    void print();
    vector<QuadContainer*>* getQuadList() { return _quadList; }
 
  private:
    vector<Block*>*           _blockList;
    vector<QuadContainer*>*   _quadList;   
    EntryBlock*               _entry;
    EndingBlock*              _ending;
};
#endif
