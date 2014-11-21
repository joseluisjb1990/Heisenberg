#ifndef BLOCK_HH
#define BLOCK_HH
#include <string>
#include <iostream>
#include <vector>
#include <forward_list>
#include <set>
#include <string>
#include <utility>
#include "quadcontainer.hh"
#include "registerAsigner.hh"

using namespace std;

class Block
{
  public:
    Block(std::vector<QuadContainer*>* quadList, std::vector<Block*>* entryBlocks, std::vector<Block*>* exitBlocks);
    Block();
    void addQuad(QuadContainer* q) { _quadList->push_back(q); }
    void print();
    void setLiveVar();

  private:
    std::vector<QuadContainer*>* _quadList;
    std::vector<Block*>*         _inBlocks;
    std::vector<Block*>*         _exitBlocks;
    forward_list<set<string>>    _liveVariables;
};

class EntryBlock : public Block
{
  public:
    EntryBlock(std::vector<Block*>* exitBlocks);
};

class EndingBlock : public Block
{
  public:
    EndingBlock(std::vector<Block*>* entryBlocks);
};
#endif
