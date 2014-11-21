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
  return fw;
}

void IntermediateGen::print()
{
  unsigned int i = 0;
  for(std::vector<QuadContainer*>::iterator it = _totalQuadList->begin(); it != _totalQuadList->end(); it++)
  {
    std::cout << i++ << " ";
    (*it)->print();
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

void IntermediateGen::printSpim(TablaSimbolos* tSimbolos)
{

    RegisterAsigner* ra = new RegisterAsigner(15);
    
    QuadContainer* qc, *qc1;
    Quad* q, *q1;
    vector<pair<bool, string>> arrPairs;
    
    _file << "        .text"  << std::endl; 

    for (unsigned int pos=1; pos < _totalQuadList->size() - 1; pos++) {
        
        std::string line;
        
        if (_totalQuadList->at(pos)->_quad->isTag()) {

            if (_totalQuadList->at(pos)->_quad->isMain()) 
                _file << std::endl << _totalQuadList->at(pos)->_quad->toSpim() << std::endl << std::endl;

            _file << std::endl << "bloque" << _totalQuadList-> at(pos)->getNumberBlock()
                                                                        << ":" << std::endl << std::endl; 

        } else {

            if (_totalQuadList->at(pos)->isLeader()) 
                _file << std::endl << "bloque" << _totalQuadList->at(pos)->getNumberBlock()
                                                                        << ":" << std::endl << std::endl; 
            qc  = _totalQuadList->at(pos);
            q   = qc->_quad; 

            // Obtenemos los registros necesarios para la instruccion
            //set<string> s = *it;

            if(q->useVariables())
            {
              arrPairs = ra->getReg(q);

              // Guardamos los resultados en las variables necesarias

              susVariables(arrPairs, q, _file);
            }
            
            line = q->toSpim();
        
            if (line != "") _file << "   " << line << std::endl; 
        }
    }
}

void IntermediateGen::susVariables(vector<pair<bool, string>> arrPairs, Quad* q, std::ofstream& file)
{
  string    regLeft, regRight, regRes;
  bool      loadLeft, loadRight, loadRes;

  loadLeft    = arrPairs.at(0).first;
  regLeft     = arrPairs.at(0).second;
  loadRight   = arrPairs.at(1).first;
  regRight    = arrPairs.at(1).second;
  loadRes     = arrPairs.at(2).first;
  regRes      = arrPairs.at(2).second;

  if(loadLeft)
  {
    if(q->_leftType->isConstant())
    {
      file << "   li " << regLeft << " " << q->_leftOperand << endl;
    }
  }

  if(loadRight)
  {
    if(q->_rightType->isConstant())
    {
      file << "   li " << regRight << " " << q->_rightOperand << endl;
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
