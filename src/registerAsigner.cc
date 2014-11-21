#ifndef REGISTERASIGNER_CC
#define REGISTERASIGNER_CC

#include"registerAsigner.hh"
RegisterAsigner::RegisterAsigner(int numRegisters)
  : _totalRegisters( numRegisters )
{
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
  _mipsRegisters[16]   = "$8";
}

vector<string> eraseVec(vector<string> vs, string s)
{
  vector<string> nvector;
  for(vector<string>::iterator itn = vs.begin(); itn != vs.end(); itn++)
    if(s != *itn)
      nvector.push_back(*itn);
  return nvector;
}

vector<pair<bool,string>> RegisterAsigner::getReg(Quad* quad)
{
  string leftOp   = quad->_leftOperand;
  string rightOp  = quad->_rightOperand;
  string destiny  = quad->_destiny; 
  vector<string> vs;
  vs.push_back(leftOp); vs.push_back(rightOp); vs.push_back(destiny);

  vector<pair<bool, string>> vecPairs;
  

  if (quad->isAssign())
  {
    bool ok = true;
    pair<bool, int> res;

    if(inRegister(destiny))
    {
      _register[_variables[destiny]] = eraseVec(_register[_variables[destiny]], destiny);
      if(_register[_variables[destiny]].empty()) _occupiedReg--;
      _variables.erase(destiny);
      _variableMod = eraseVec(_variableMod,destiny);
    }

    if(!inRegister(leftOp))
    {
      if(_occupiedReg == _totalRegisters)
      {
        cleanRegisters();
        if(_occupiedReg == _totalRegisters)
        {
          spill(quad, vector<string>());
          ok = false;
        }
      }
    }

    if(ok)
    {
      res = getRegister(leftOp);

      _register[res.second].push_back(destiny);
      _variables[destiny] = res.second;
      
      vecPairs.push_back(*(new pair<bool, string> (res.first, _mipsRegisters[res.second])));
      vecPairs.push_back(*(new pair<bool, string> (false, "")));
      vecPairs.push_back(*(new pair<bool, string> (false, _mipsRegisters[res.second])));
      _variableMod.push_back(destiny);
    }
  }
  else if(quad->isExpQuad())
  {
    int r = cantInRegister(vs);

    cout << _occupiedReg << " " << r << endl;
    if(_occupiedReg + r > _totalRegisters)
    {  
      cleanRegisters();
      if(_occupiedReg + r <= _totalRegisters)
      {
        vecPairs = insertAll(vs);
        _variableMod.push_back(destiny);
      }
      else
      {
        spill(quad, vs);
        if(_occupiedReg + r > _totalRegisters)
          cout << endl << "EMERGENCIA EL SPILL NO FUE SUFICIENTE, CONO" << endl;
      }
    }
    else
    {
      vecPairs = insertAll(vs);
      _variableMod.push_back(destiny);
    }
  }
  return vecPairs;
}

int RegisterAsigner::cantInRegister(vector<string> vs)
{
  int r = 0;
  for(auto& s: vs)
    if(!s.empty())
      if(!inRegister(s))   
        r++;

  return r;
}

vector<pair<int, string>> RegisterAsigner::getVarToSpill()
{ 
  vector<pair<int,string>> v = _varToSpill; 
  _varToSpill.clear(); 
  return v;    
}

bool atLeastOne(string s, vector<string> vs)
{
  for(vector<string>::iterator it = vs.begin(); it != vs.end(); it++)
    if (s == *it) return true;

  return false;
}

bool find(vector<string> vs, string s)
{
  for(vector<string>::iterator it = vs.begin(); it != vs.end(); it++)
  {
    string sv = *it;
    if(sv == s)
      return true;
  }

  return false;
}

void RegisterAsigner::spill(Quad* q, vector<string> vs)
{
  vector<int> vi;
  bool mod = true;

  for(map<int, vector<string>>::iterator it = _register.begin(); it != _register.end(); it++)
  {
    int first               = (*it).first;
    vector<string> second   = (*it).second;

    mod = true;
    if(!second.empty())
    {
      for(vector<string>::iterator itv = second.begin(); itv != second.end(); itv++)
        if(q->isLiveVar(*itv) or atLeastOne(*itv, vs))
        {
          mod = false;
          break;
        }

      if(mod) vi.push_back(first);
    }
  }

  for(vector<int>::iterator itv = vi.begin(); itv != vi.end(); itv++)
  {
    int r = *itv;
    vector<string> vs = _register[r];
    for(vector<string>::iterator it = vs.begin(); it != vs.end(); it++)
    {
      string v = *it;
      if(!isLiteral(v))
      {
        if(find(_variableMod, v))
        {
          _varToSpill.push_back(pair<int, string> (r,v));
          vector<string> nvector;
          for(vector<string>::iterator itn = _variableMod.begin(); itn != _variableMod.end(); itn++)
            if(v != *itn)
              nvector.push_back(*itn);
          _variableMod = nvector;
        }
      }
      _variables.erase(v);
    }
    _register[r].clear();
    cout << "OC " << _occupiedReg << endl;
    _occupiedReg--;
  }
}

bool RegisterAsigner::isLiteral(string op)
{
  return boost::regex_match(op.c_str(), boost::regex("([0-9])+"));
}

void RegisterAsigner::cleanRegisters()
{
  for (auto& r: _register)
  {
    if(r.second.size() == 1)
      if(isLiteral(r.second.back()))
      {
        _variables.erase(r.second.back());
        r.second.clear();
        _occupiedReg--;
      }
  }
}

vector<pair<bool,string>> RegisterAsigner::insertAll(vector<string> vs)
{
  vector<pair<bool, string>> vecPairs;
  pair<bool, int> res;

  for(vector<string>::iterator it = vs.begin(); it != vs.end(); it++)
  {
    res = getRegister(*it);
    if(-1 == res.second) cout << endl << "ERROR EN EL GETREGISTER" << *it << endl;
    vecPairs.push_back(*(new pair<bool, string> (res.first, _mipsRegisters[res.second])));
  }

  return vecPairs;
}

bool RegisterAsigner::inRegister(string op)
{
  return _variables.find(op) != _variables.end();
}

int RegisterAsigner::findRegister()
{
  if(_register.empty())
    return 1;
  else
  {
    int i = 1;
    for(; i <= (int) _register.size(); i++)
    {
      if(_register.at(i).empty())
        return i;
      
      if(i == _totalRegisters)
        return -1;
    }
    return i;
  }
}

pair<bool, int> RegisterAsigner::getRegister(string op)
{
  pair<bool, int> p;
  if(_variables.find(op) == _variables.end())
  {
    // Busco un registro vacio
    int pos = findRegister();

    // Si no lo encontro, debemos reciclar registros
    if(-1 == pos)
    {
      p.first   = false;
      p.second  = -1;
    }
    // Si encontro un registro vacio, asignar ese registro 
    else
    {
      //Actualizamos el diccionario de variables 
      _variables[op] = pos;

      //Actualizamos el diccionario de registros
      _register[pos].push_back(op);

      //Retornamos el resultado.
      p.first   = true;
      p.second  = pos;

      //Sumamos uno a los registros ocupados
      _occupiedReg++;
    }
  } else
  {
    // Si la variable esta en un registro no la tengo que cargar
    p.first   = false;

    // Busco el reistro en el que esta la variable
    int pos   = _variables.find(op)->second;

    // Asigno el registro en el que esta la variable
    p.second  = pos;
  }
  return p;
}

void RegisterAsigner::print()
{
  cout << endl << "Contenido de los registros" << endl;
  for (auto& r: _register)
  {
    cout << r.first << ": ";
    for(auto& s: r.second)
      cout << s << " ";
    cout << " | ";
  }
  
  cout << endl << "Ubicacion de las variables" << endl;
  for(auto& v: _variables)
  {
    cout << v.first << ": " << v.second << " | ";
  }

  cout << endl << "Variables modificadas " << endl;
  for(auto& vm: _variableMod)
  {
    cout << vm << " | ";
  }

  cout << endl << "Variables para hacer spill " << endl;
  for(auto& vsp: _varToSpill)
  {
    cout << to_string(vsp.first) + ", " + vsp.second << " | ";
  }

  cout << endl;
}
#endif
