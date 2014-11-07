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
  if(_liveVariables.empty()) _liveVariables.push_front(*(new set<string>()));
  Quad* q;
  set<string> s, s1, s2;
  string d;
  for(std::vector<Quad*>::iterator it = _quadList->end() -1; it != _quadList->begin() -1; it--)
  {
    q = *it;

    // Obtenemos las variables usadas por el quad
    s   = q->getUsedVariables();

    // Obtenemos las variables definidas por el quad.
    d = q->getDefinedVariable();

    // Obtenemos el conjutnto de variables vivas del tope de la lista.
    s1  = _liveVariables.front();
      
    //Hacemos copia del ultimo conjunto insertado en la lista de variables vivas
    _liveVariables.push_front(*(new set<string>(s1)));

    //Eliminamos las variables definidas en el quad
    if(!_liveVariables.front().empty()) _liveVariables.front().erase(d);

    //Insertamos las nuevas variables usadas en el quad
    _liveVariables.front().insert(s.begin(), s.end());

  }
}

void Block::print()
{
  if(_quadList->empty()) return;
  std::vector<Quad*>::iterator itq = _quadList->begin();
  
  for(std::forward_list<set<string>>::iterator it = _liveVariables.begin(); it != _liveVariables.end(); it++)
  {
    std::cout << "Variables vivas:  ";
    std::set<std::string> s = *it;

    if(!s.empty())
      for(std::set<std::string>::iterator its = s.begin(); its != s.end(); its++)
        std::cout << (*its) << " ";
    else
      std::cout << "Vacio";

    std::cout << std::endl;
    if(itq != _quadList->end()) { std::cout << "Quad :  "; (*itq)->print(); itq++; }
  }
}

EntryBlock::EntryBlock(std::vector<Block*>* exitBlocks)
  : Block(nullptr, nullptr, exitBlocks)
{}

EndingBlock::EndingBlock(std::vector<Block*>* entryBlocks)
  : Block(nullptr, entryBlocks, nullptr)
{}
