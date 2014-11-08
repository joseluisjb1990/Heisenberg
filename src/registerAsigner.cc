#include"registerAsigner.hh"

RegisterAsigner::RegisterAsigner(int numRegisters)
  : _totalRegisters( numRegisters )
{}

pair<int,int> RegisterAsigner::getReg(Quad* quad, set<string> s)
{
  // Haciendo unicamente el caso que de una operacion
  string leftOp   = quad->_leftOperand;
  string rightOp  = quad->_rightOperand;
  string destiny  = quad->_destiny; 
  pair<int, int> p(-1, -1);
  
  if(_variables.find(leftOp) == _variables.end())
  {
    if(_numRegisters != _totalRegisters) {
      _variables[leftOp] = _numRegisters;
      _register[_numRegisters].push_back(leftOp);
      p.first = _numRegisters;
      _numRegisters++;
    } else
        cout << "Tengo que reciclar registros";
  }
  return p;
}
