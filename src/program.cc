#ifndef PROGRAM_CC
#define PROGRAM_CC

#include "program.hh"

Program::Program(std::vector<Definition*>* definitions, Statement* statement)
  : Node()
  , _definitions( definitions )
  , _statement  ( statement   )
{}

std::string Program::to_string(int nesting)
{
  cout << "PROGRAM \n";

  if(_definitions)
    for(std::vector<Definition*>::iterator it = _definitions->begin(); it != _definitions->end(); ++it)
      cout << (*it)->to_string(nesting+1) + "\n";

  cout << _statement->to_string(nesting+1) + "\n";

  return "";
}

void Program::check()
{
  bool ok = true;
  if(_definitions) {
    for(std::vector<Definition*>::iterator it = _definitions->begin(); it != _definitions->end(); ++it)
    {
      (*it)->check();
      if((*it)->get_type() == ErrorType::getInstance()) ok = false;
    }
  }

  if(_statement) _statement->check();
  if(_statement->get_type() != ErrorType::getInstance() and ok) set_type(ExtintoType::getInstance());
  else set_type(ErrorType::getInstance());
}

void Program::toIntermediate(IntermediateGen *intGen)
{

  if (NULL != _definitions)
    for (unsigned int i=0; i < _definitions->size(); ++i)
      _definitions->at(i)->toIntermediate(intGen);

  _statement->toIntermediate(intGen);

  Quad* q = new EndQuad();
  intGen->gen(q);
}
#endif
