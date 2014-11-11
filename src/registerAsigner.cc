#ifndef REGISTERASIGNER_CC
#define REGISTERASIGNER_CC

#include"registerAsigner.hh"
RegisterAsigner::RegisterAsigner(int numRegisters)
  : _totalRegisters( numRegisters )
{}

vector<pair<bool,int>> RegisterAsigner::getReg(Quad* quad)
{
  // Haciendo unicamente el caso que de una operacion
  string leftOp   = quad->_leftOperand;
  string rightOp  = quad->_rightOperand;
  string destiny  = quad->_destiny; 
  vector<pair<bool, int>> vecPairs;

  vecPairs.push_back(setOpPair(leftOp));
  vecPairs.push_back(setOpPair(rightOp));
  vecPairs.push_back(setOpPair(destiny));
  return vecPairs;
}

pair<bool, int> RegisterAsigner::setOpPair(string op)
{
  pair<bool, int> p;
  if(!op.empty())
  {
    if(_variables.find(op) == _variables.end())
    {
      if(_numRegisters != _totalRegisters) {
        _variables[op] = _numRegisters;
        _register[_numRegisters].push_back(op);
        p.first = true;
        p.second = _numRegisters;
        _numRegisters++;
      } else
      {
        cout << "Tengo que reciclar registros";
        p.first   = false;
        p.second  = 0;
      }
    } else
    {
      p.first   = false;
      p.second  = _variables.find(op)->second;
    }
  }
  else 
  {
    p.first   = false;
    p.second  = 0;
  }

  return p;
}
#endif
