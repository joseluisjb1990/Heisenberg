#ifndef GENERATOR_HH
#define GENERATOR_HH

#define TEMPCHAR "#"
#include<string>
#include<fstream>
#include<stdio.h>
#include"TablaSimbolos.hh"
class Generator {};

class IntermediateGen : public Generator
{
public:
  IntermediateGen(const std::string &file, TablaSimbolos * symbolTable);
  void close();
  void gen(std::string op, std::string arg1, std::string arg2, std::string des, std::string com);
  void gen(std::string op, std::string arg1, std::string arg2, std::string des);
  void gen(std::string id);
  void gen(long offset, int jumpDes);
  void genComment(std::string comment);
  long genEmpty(std::string op);
  void genSpace();
  unsigned int getQuadNumber()  { return _quadNumber++; }
  unsigned int getTempNumber()  { return _tempNumber++; }
  unsigned int getQuad()        { return _quadNumber;   }
  long offset()                 { return _file.tellp(); } 
  std::string nextTemp()        { return  TEMPCHAR + std::to_string(getTempNumber());   }
  void emptySlot()              { _file << std::string(30, ' ') << '\n'; _quadNumber++; }
  TablaSimbolos *_symbolTable;
private:
  std::string _nameTempFile;
  std::ofstream _file;
  unsigned int _tempNumber;
  unsigned int _quadNumber;
};
#endif
