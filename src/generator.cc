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
  std::cout << _numberBlock << " " << _numberQuad << " " << _isLeader << " "; _quad->print();
}

void IntermediateGen::optimize()
{
  FlowGraph* fw = new FlowGraph(_totalQuadList);
  fw->print();
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
    _nameTempFile = file + ".temp";
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

void IntermediateGen::printSpim()
{

    for (unsigned int pos=0; pos < _totalQuadList->size(); pos++) {
        
        std::string line;
        line = _totalQuadList->at(pos)->_quad->toSpim();
        
        if (line != "") _file << line << std::endl; 
    }

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
