#ifndef GENERATOR_CC
#define GENERATOR_CC

#include "generator.hh"

using namespace std;

QuadContainer::QuadContainer(Quad* quad, unsigned int numberQuad, bool isLeader)
  : _quad       ( quad       )
  , _numberQuad ( numberQuad )
  , _isLeader   ( isLeader   )
{} 

void QuadContainer::print()
{ 
  std::cout << _numberBlock << " " << _numberQuad << " " << _isLeader << " "; _quad->print(); cout << " / ";
  for(set<string>::iterator it = _quad->_liveVar.begin(); it != _quad->_liveVar.end(); it++)
    cout << (*it) << " ";

}

FlowGraph* IntermediateGen::optimize()
{
  FlowGraph* fw = new FlowGraph(_totalQuadList);
  _totalQuadList = fw->getQuadList();
  return fw;
}

void IntermediateGen::print()
{
  unsigned int i = 0;
  for(std::vector<QuadContainer*>::iterator it = _totalQuadList->begin(); it != _totalQuadList->end(); it++)
  {
    std::cout << i++ << " ";
    (*it)->print();
    cout << endl;
  }

  cout << endl << "-------------------------ALCANCE DE FUNCIONES-----------------------------" << endl;
  
  queue<unsigned int> qalc= _symbolTable->getAlcFunciones();
  int size = qalc.size();
  unsigned int x;

  for(int i = 1;  i <= size; i++)
  {
    x = qalc.front();
    cout << x << " | ";
    qalc.push(x);
  }

  cout << endl << "-------------------------TAMANIO NECESARIO PARA LAS FUNCIONES-----------------------------" << endl;
  
  qalc= _symbolTable->getTamFunciones();
  size = qalc.size();

  for(int i = 1;  i <= size; i++)
  {
    x = qalc.front();
    cout << x << " | ";
    qalc.push(x);
  }
}

IntermediateGen::IntermediateGen(const std::string &file, TablaSimbolos * symbolTable)
  : Generator()
  { 
    _nameTempFile = file + ".s";
    _file.open( _nameTempFile.c_str(), std::ofstream::out); 
    _quadNumber = 0;
    _tempNumber = 0;
    _symbolTable = symbolTable;
    _totalQuadList = new std::vector<QuadContainer*>();
  }

void IntermediateGen::close()
{
  if(_file != NULL) _file.close();
}

bool isTemp(string op)
{
  return boost::regex_match(op.c_str(), boost::regex("_t(.*)"));
}

void IntermediateGen::storeAll(map<string, string> msi, unsigned int alcAct, unsigned int alcTop)
{
  for(auto& t: msi)
  {
    Contenido* c = nullptr;
    cout << endl << "Debo hacer store de la variable " << t.first << " que esta en el registro " << t.second << " " << alcAct << " " << alcTop << endl;
    for(unsigned int i = alcAct; i <= alcTop; i++)
    {
      if(!isTemp(t.first))
      {
        c = _symbolTable->find_scope(t.first, i);
        if(c) break;
      }
    }

    if(c) _file << "   sw " << t.second << " , " << c->getOffset() << "($sp)" << endl; 
  }
}

void IntermediateGen::printSpim(TablaSimbolos* tSimbolos)
{

    QuadContainer* qc;
    Quad* q;
    vector<pair<bool, string>> arrPairs;
    
    _file << "        .text"  << std::endl; 

    queue<unsigned int> qAlc = _symbolTable->getAlcFunciones();
    queue<unsigned int> qTam = _symbolTable->getTamFunciones();

    unsigned int alc = 0;
    unsigned int tam = 0;
    
    RegisterAsigner* ra;
    ra = new RegisterAsigner(20);

    unsigned int actualScope = 0;
    
    for(vector<QuadContainer*>::iterator itq = _totalQuadList->begin() + 1; itq != _totalQuadList->end(); itq++)
    { 
        qc  = *itq;
        q   = qc->_quad; 
        
        if(actualScope != qc->getNumberBlock())
        {
          map<string, string> msi = ra->getModVar();
          storeAll(msi, alc, qAlc.front());
          actualScope = qc->getNumberBlock();
          ra = new RegisterAsigner(20);
        }

        std::string line;
        if (q->isTag()) 
        {
            alc = qAlc.front(); qAlc.pop();
            tam = qTam.front(); qTam.pop();
            if(qAlc.empty()) qAlc.push(alc);

            //if (q->isMain()) 
            _file << std::endl << q->toSpim() << std::endl << std::endl;

            _file << std::endl << "bloque" << qc->getNumberBlock() << ":" << std::endl << std::endl;

             // Imprime el Principio de la Funcion                                                            
            _file << std::endl << q->toSpim2() << std::endl << std::endl; 

            _file << std::endl << "    subu $sp $sp " << tam << endl;
              
        } 
        else 
        {

            if (qc->isLeader()) 
                _file << std::endl << "bloque" << qc->getNumberBlock() << ":" << std::endl << std::endl;


            if(q->useVariables())
            {
              arrPairs = ra->getReg(q);
              //qc->print();
              //ra->print();
              //cout << endl;
              if(ra->getSpillMode())
              {
                vector<pair<string, string>> v = ra->getVarToSpill();
                //for(auto& pis: v)
                  //cout << pis.first << ", " << pis.second << endl ;
                
                spillVariables(v, alc, qAlc.front());
                arrPairs = ra->getReg(q);
                ra->print();
              }

              // Guardamos los resultados en las variables necesarias

              susVariables(arrPairs, q, _file, alc, qAlc.front());
              if(qc->isJump())
              {
                map<string, string> msi = ra->getModVar();
                storeAll(msi, alc, qAlc.front());
                ra = new RegisterAsigner(20);
              }
            }
            
            line = q->toSpim();
        
            if (line != "") _file << "   " << line << std::endl; 
        }
    }
}

void IntermediateGen::spillVariables(vector<pair<string, string>> arrPairs, unsigned int alcAct, unsigned int alcTop)
{
  Contenido* c = nullptr;
  for(auto& p: arrPairs)
  {
    for(unsigned int i = alcAct; i <= alcAct; i++)
    {
      c = _symbolTable->find_scope(p.second, i);
      if(c) break;
    }

    _file << "   sw " << p.first << " , " << c->getOffset() << "($sp)" << endl; 
  }
}

bool isLiteral(string op)
{
  return boost::regex_match(op.c_str(), boost::regex("([0-9])+"));
}

void IntermediateGen::susVariables(vector<pair<bool, string>> arrPairs, Quad* q, std::ofstream& file, unsigned int alcAct, unsigned int alcTop)
{
  string    regLeft, regRight, regRes;
  bool      loadLeft, loadRight, loadRes;

  Contenido* c = nullptr;
  loadLeft    = arrPairs.at(0).first;
  regLeft     = arrPairs.at(0).second;
  loadRight   = arrPairs.at(1).first;
  regRight    = arrPairs.at(1).second;
  loadRes     = arrPairs.at(2).first;
  regRes      = arrPairs.at(2).second;

  if(loadLeft)
  {
    if(isLiteral(q->_leftOperand))
    {
      file << "   li " << regLeft << " " << q->_leftOperand << endl;
    } else
    {
      for(unsigned int i = alcAct; i <= alcAct; i++)
      {
        c = _symbolTable->find_scope(q->_leftOperand, i);
        if(c) break;
      }

      if(c->getTipo()->isArray())
      {
        _file << "   move "   << regLeft << " "             << "$sp"  << endl;
        _file << "   addi "   << regLeft << " " << regLeft  << " "    << c->getOffset() << endl;
      }
      else
        _file << "   lw "   << regLeft << ", " << c->getOffset() << "($sp)" << endl; 
    }
  }

  if(loadRight)
  {
    if(isLiteral(q->_rightOperand))
    {
      file << "   li " << regRight << " " << q->_rightOperand << endl;
    } else
    {
      for(unsigned int i = alcAct; i <= alcAct; i++)
      {
        c = _symbolTable->find_scope(q->_rightOperand, i);
        if(c) break;
      }

      _file << "   lw " << regRight << ", " << c->getOffset() << "($sp)" << endl; 
    }
  }
  q->susVarReg(regLeft, regRight, regRes);
}

unsigned int IntermediateGen::gen(Quad* q)
{
  _totalQuadList->push_back(new QuadContainer(q, getQuadNumber(), false));
  return _totalQuadList->size() - 1;
}

void IntermediateGen::patch(unsigned int pos, int jumpDes)
{
  _totalQuadList->at(pos)->_quad->setDestiny(std::to_string(jumpDes)); 
}

void IntermediateGen::gen(std::string op, std::string arg1, std::string arg2, std::string des, std::string com)
{
  std::string r = write(op, arg1, arg2, des);
  _file   << r 
          << std::string(  40 
                        - r.size()
                        , ' '
                        )
            << com
            << std::endl
            ;
}

void IntermediateGen::gen(std::string op, std::string arg1, std::string arg2, std::string des)
{
  _file <<  write(op, arg1, arg2, des) << std::endl;
}

std::string IntermediateGen::write(std::string op, std::string arg1, std::string arg2, std::string des)
{
  std::string r = std::to_string(getQuadNumber());
  
  return r 
        +  ":  " 
        +  op  + " " 
        + arg1 + " " 
        + arg2 + " " 
        + des;
}

void IntermediateGen::writeOp(std::string op, std::string des, std::string arg1, std::string arg2)
{
  std::cout << "Fff";
  _file << op <<" " << des << " " << arg1 + " " << arg2 << " "; 
 
}


void IntermediateGen::gen(std::string id)
{
  _file << id << ":  " << std::endl;
}


long IntermediateGen::genEmpty(std::string op)
{
  std::string s = write(op, " "," ","");
  _file << s;
  long r = offset();
  _file << std::string(10, ' ') << std::endl;
  return r;
}


void IntermediateGen::genComment(std::string comment) { _file << comment << std::endl;  }
void IntermediateGen::genSpace()                      { _file << std::endl;             }
#endif
