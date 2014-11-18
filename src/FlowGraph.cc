#ifndef FLOWGRAPH_CC
#define FLOWGRAPH_CC

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

    if(qc->isTag()) qc->setLeader();                                                                     // La siguiente instruccion es lider.
  }

  Block* b;
  _blockList = new std::vector<Block*>;
  b = new Block(); _blockList->push_back(b);

  unsigned int bc = 0;
  for(std::vector<QuadContainer*>::iterator it = quadList->begin(); it != quadList->end() - 1; it++)
  {
    qc = (*it);
    b = new Block();
    if(qc->isLeader())
      while(true) { b->addQuad(qc); qc->addNumberBlock(bc); if ((*(it + 1))->isLeader()) break; else qc = *(++it); }
    _blockList->push_back(b);
    bc++;
  }

  b = new Block(); qc = quadList->back(); qc->addNumberBlock(bc); b->addQuad(qc); _blockList->push_back(b);
    
  for(std::vector<QuadContainer*>::iterator it = quadList->begin(); it != quadList->end(); it++)
  {
    qc = (*it);
    if(qc->isJump()) { qc->replaceAddress(quadList->at(qc->getAddress())->getNumberBlock()); } 
  }

  for(std::vector<Block*>::iterator it = _blockList->begin(); it != _blockList->end(); it++)
    (*it)->setLiveVar(); 

  _quadList = quadList;

  setRegisters(15);
}

void FlowGraph::setRegisters(int cantRegisters)
{
  if(_quadList->empty()) return;
  
  RegisterAsigner* ra = new RegisterAsigner(cantRegisters);

  vector<pair<bool, string>> arrPairs;
  
  QuadContainer* qc, *qc1;
  Quad* q, *q1;
  string    regLeft, regRight, regRes;
  bool      loadLeft, loadRight, loadRes;

  for(vector<QuadContainer*>::iterator itq = _quadList->begin(); itq != _quadList->end(); itq++)
  {
    qc  = (*itq);
    q   = qc->_quad; 
    if(q->useVariables())
    {
      // Obtenemos los registros necesarios para la instruccion
      //set<string> s = *it;
      arrPairs = ra->getReg(q);

      // Guardamos los resultados en las variables necesarias

      loadLeft    = arrPairs.at(0).first;
      regLeft     = arrPairs.at(0).second;
      loadRight   = arrPairs.at(1).first;
      regRight    = arrPairs.at(1).second;
      loadRes     = arrPairs.at(2).first;
      regRes      = arrPairs.at(2).second;

      cout << "La variable izquierda "  << q->_leftOperand  << " Tiene el registro " << arrPairs.at(0).second << " " << arrPairs.at(0).first << endl;
      cout << "La variable derecha "    << q->_rightOperand << " Tiene el registro " << arrPairs.at(1).second << " " << arrPairs.at(1).first << endl;
      cout << "La variable destino "    << q->_destiny      << " Tiene el registro " << arrPairs.at(2).second << " " << arrPairs.at(2).first << endl;
    }
  }
}

void FlowGraph::print()
{
  QuadContainer* b;
  int i = 0;

  for(std::vector<QuadContainer*>::iterator it = _quadList->begin(); it != _quadList->end(); it++)
  {
    b = (*it);
    std::cout << i++; b->print();
  }
}
#endif
