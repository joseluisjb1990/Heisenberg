#ifndef REGISTERASIGNER_CC
#define REGISTERASIGNER_CC

#include"registerAsigner.hh"
RegisterAsigner::RegisterAsigner(int numRegisters)
  : _totalRegisters( numRegisters )
{
  _mipsRegisters[0]   = "$8";
  _mipsRegisters[1]   = "$9";
  _mipsRegisters[2]   = "$10";
  _mipsRegisters[3]   = "$11";
  _mipsRegisters[4]   = "$12";
  _mipsRegisters[5]   = "$13";
  _mipsRegisters[6]   = "$14";
  _mipsRegisters[7]   = "$15";
  _mipsRegisters[8]   = "$16";
  _mipsRegisters[9]   = "$17";
  _mipsRegisters[10]  = "$18";
  _mipsRegisters[11]  = "$19";
  _mipsRegisters[12]  = "$20";
  _mipsRegisters[13]  = "$21";
  _mipsRegisters[14]  = "$22";
  _mipsRegisters[15]  = "$23";
}

vector<pair<bool,string>> RegisterAsigner::getReg(Quad* quad)
{
  string leftOp   = quad->_leftOperand;
  string rightOp  = quad->_rightOperand;
  string destiny  = quad->_destiny; 
  vector<pair<bool, string>> vecPairs;

  if (quad->isAssign())
  {

    pair<bool, string> emptyPair(false, "");
    pair<bool, string> resultPair = setAssignPair(leftOp, destiny);
    pair<bool, string> destinyPair(false, resultPair.second);
    vecPairs.push_back(resultPair);
    vecPairs.push_back(emptyPair);
    vecPairs.push_back(destinyPair);
  }
  else if(quad->isExpQuad())
  {
    vecPairs.push_back(setOpPair(leftOp));
    vecPairs.push_back(setOpPair(rightOp));
    vecPairs.push_back(setOpPair(destiny));
  }
  
  return vecPairs;
}

pair<bool, string> RegisterAsigner::setAssignPair(string op, string destiny)
{
  pair<bool, string> p;
  if(_variables.find(op) == _variables.end())
  {
    if(_numRegisters != _totalRegisters) {
      _variables[op] = _numRegisters;
      _variables[destiny] = _numRegisters;
      _register[_numRegisters].push_back(op);
      _register[_numRegisters].push_back(destiny);
      p.first = true;
      p.second = _mipsRegisters[_numRegisters];
      _numRegisters++;
    } else
    {
      cout << "Tengo que reciclar registros";
      p.first   = false;
      p.second  = "R" + to_string(0);
    }
  } else
  {
    p.first   = false;
    int pos = _variables.find(op)->second;
    p.second  = _mipsRegisters[pos];
    _variables[destiny] = pos; 
  }
  return p;
}

pair<bool, string> RegisterAsigner::setOpPair(string op)
{
  pair<bool, string> p;
  if(!op.empty())
  {
    if(_variables.find(op) == _variables.end())
    {
      if(_numRegisters != _totalRegisters) {
        _variables[op] = _numRegisters;
        _register[_numRegisters].push_back(op);
        p.first = true;
        p.second = _mipsRegisters[_numRegisters];
        _numRegisters++;
      } else
      {
        cout << "Tengo que reciclar registros";
        p.first   = false;
        p.second  = "R" + to_string(0);
      }
    } else
    {
      p.first   = false;
      p.second  = _mipsRegisters[_variables.find(op)->second];
    }
  }
  else 
  {
    p.first   = false;
    p.second  = "RNULL";
  }
  return p;
}
#endif
