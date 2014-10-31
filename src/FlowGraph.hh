#include <string>
#include <iostream>
#include <vector>
#include "block.hh"
#include "quad.hh"
#include "quadcontainer.hh"

class FlowGraph
{
  public:
    FlowGraph(std::vector<Block*>* blockList, EntryBlock* entry, EndingBlock* ending);
    FlowGraph(std::vector<QuadContainer*>* quadList);
    void print();
 
  private:
    std::vector<Block*>*  _blockList;
    EntryBlock*           _entry;
    EndingBlock*          _ending;
};
