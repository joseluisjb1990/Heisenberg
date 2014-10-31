#include <string>
#include <iostream>
#include <vector>
#include "quad.hh"

class Block
{
  public:
    Block(std::vector<Quad*>* quadList, std::vector<Block*>* entryBlocks, std::vector<Block*>* exitBlocks);
    Block();
    void addQuad(Quad* q) { _quadList->push_back(q); }
    void print();

  private:
    std::vector<Quad*>*  _quadList;
    std::vector<Block*>* _inBlocks;
    std::vector<Block*>* _exitBlocks;
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
