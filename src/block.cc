#include "block.hh"

Block::Block(std::vector<Quad*>* quadList, std::vector<Block*>* inBlocks, std::vector<Block*>* exitBlocks)
  : _quadList   ( quadList   )
  , _inBlocks   ( inBlocks   )
  , _exitBlocks ( exitBlocks )
{}

Block::Block()
{
  _quadList   = new std::vector<Quad*>();
  _inBlocks   = new std::vector<Block*>();
  _exitBlocks = new std::vector<Block*>();
}

void Block::print()
{
  for(std::vector<Quad*>::iterator it = _quadList->begin(); it != _quadList->end(); it++)
    (*it)->print();
}

EntryBlock::EntryBlock(std::vector<Block*>* exitBlocks)
  : Block(nullptr, nullptr, exitBlocks)
{}

EndingBlock::EndingBlock(std::vector<Block*>* entryBlocks)
  : Block(nullptr, entryBlocks, nullptr)
{}
