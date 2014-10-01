#ifndef GENERATOR_CC
#define GENERATOR_CC

#include "generator.hh"

using namespace std;

IntermediateGen::IntermediateGen(const std::string &file, TablaSimbolos * symbolTable)
  : Generator()
  { 
    _nameTempFile = file + ".temp";
    _file.open( _nameTempFile.c_str(), std::ofstream::out); 
    _quadNumber = 0;
    _tempNumber = 0;
    _symbolTable = symbolTable;
  }

void IntermediateGen::close()
{
  if(_file != NULL) _file.close();
}

void IntermediateGen::gen(std::string op, std::string arg1, std::string arg2, std::string des)
{
  _file << getQuadNumber() << ":  " << op << " " << arg1 << " " << arg2 << " " << des << std::endl;
}

void IntermediateGen::gen(std::string id)
{
  _file << id << ":  " << std::endl;
}

long IntermediateGen::genEmpty(std::string op)
{
  _file << getQuadNumber() << ":  " << op << " ";
  long r = offset();
  _file << std::string(10, ' ') << std::endl;
  return r;
}

void IntermediateGen::gen(long offset, int jumpDes)
{
  long actualPos = IntermediateGen::offset();
  _file.seekp(offset);
  std::string c = std::to_string(jumpDes);
  _file.write(c.c_str(), c.size());
  _file.seekp(actualPos);
}
#endif
