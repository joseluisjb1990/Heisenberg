#include "FlowGraph.hh"

FlowGraph::FlowGraph(std::vector<Block*>* blockList, EntryBlock* entry, EndingBlock* ending)
  : _blockList( blockList )
  , _entry    ( entry     )
  , _ending   ( ending    )  
{}

FlowGraph::FlowGraph(std::vector<QuadContainer*>* quadList)
{
  QuadContainer* qc;

  quadList->front()->setLeader();                                                                     // La primera instruccion del TAC es lider
  quadList->back()->setLeader();

  for(std::vector<QuadContainer*>::iterator it = quadList->begin(); it != quadList->end() - 1; it++)
  {
    qc = (*it);
    if(qc->isJump())
    {
      quadList->at(qc->getAddress())->setLeader();                                                  // El destino del salto es lider
      (*(it + 1))->setLeader();                                                                     // La siguiente instruccion es lider.
    } 
  }

  Block* b;
  _blockList = new std::vector<Block*>;
  unsigned int bc = 0;

  for(std::vector<QuadContainer*>::iterator it = quadList->begin(); it != quadList->end() - 1; it++)
  {
    qc = (*it);
    b = new Block();
    while(true) { b->addQuad(qc->getQuad()); qc->addNumberBlock(bc); if ((*(it + 1))->isLeader()) break; else qc = *(++it); }
    _blockList->push_back(b);
    bc++;
  }

  for(std::vector<QuadContainer*>::iterator it = quadList->begin(); it != quadList->end() - 1; it++)
  {
    qc = (*it);
    if(qc->isJump()) { qc->replaceAddress(quadList->at(qc->getAddress())->getNumberBlock()); } 
  }
}

void FlowGraph::print()
{
  Block* b;
  int i = 0;

  for(std::vector<Block*>::iterator it = _blockList->begin(); it != _blockList->end(); it++)
  {
    b = (*it);
    std::cout << "Bloque " << i++ << std::endl; b->print();  
  }
}