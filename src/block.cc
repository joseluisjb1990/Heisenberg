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

void Block::setLiveVar()
{
  if(_liveVariables.empty()) _liveVariables.push_back(*(new set<string>));
  Quad* q;
  set<string> s, s1;
  for(std::vector<Quad*>::iterator it = _quadList->end(); it != _quadList->begin(); it--)
  {
    q = (*it);
    //s = q->getUsedVariables();
  }
}

void Block::print()
{
  for(std::vector<Quad*>::iterator it = _quadList->begin(); it != _quadList->end(); it++)
    (*it)->print();

  for(std::vector<set<string>>::iterator it = _liveVariables.begin(); it != _liveVariables.end(); it++)
  {
    cout << endl;
    for(set<string>::iterator itSet = (*it).begin(); itSet != (*it).end(); itSet++)
      cout << *itSet << " "; 
  }
}

EntryBlock::EntryBlock(std::vector<Block*>* exitBlocks)
  : Block(nullptr, nullptr, exitBlocks)
{}

EndingBlock::EndingBlock(std::vector<Block*>* entryBlocks)
  : Block(nullptr, entryBlocks, nullptr)
{}
