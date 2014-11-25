#ifndef GENERATOR_HH
#define GENERATOR_HH

#define TEMPCHAR "_"
#include<string>
#include<fstream>
#include<stdio.h>
#include"TablaSimbolos.hh"
#include"quad.hh"
#include"FlowGraph.hh"
#include"quadcontainer.hh"
#include <iostream>

class Generator {};

class IntermediateGen : public Generator
{
public:
  IntermediateGen(const std::string &file, TablaSimbolos * symbolTable);
  void close();
  FlowGraph* optimize();
  void gen(std::string op, std::string arg1, std::string arg2, std::string des, std::string com);
  void gen(std::string op, std::string arg1, std::string arg2, std::string des);
  void gen(std::string id);
  void spillVariables(vector<pair<int, string>> arrPairs, std::ofstream& file);
  void patch(unsigned int pos, int jumpDes);
  unsigned int gen(Quad* q);
  void genComment(std::string comment);
  long genEmpty(std::string op);
  void genSpace();
  unsigned int getQuadNumber()  { return _quadNumber++; }
  unsigned int getTempNumber()  { return _tempNumber++; }
  unsigned int getQuad()        { return _quadNumber;   }
  long offset()                 { return _file.tellp(); } 
  std::string nextTemp()        { return  TEMPCHAR + std::to_string(getTempNumber());   }
  TablaSimbolos *_symbolTable;
  void print();
  void printSpim(TablaSimbolos* tSimbolos);
  void susVariables(vector<pair<bool, string>> arrPairs, Quad* q, std::ofstream& file);
  void spillVariables(vector<pair<string, string>> arrPairs, unsigned int alcAct, unsigned int alcTop);
  void susVariables(vector<pair<bool, string>> arrPairs, Quad* q, std::ofstream& file, unsigned int alcAct, unsigned int alcTop);
  void storeAll(map<string, string> msi, unsigned int alcAct, unsigned int alcTop);
  void loadVariable(string var, string reg, unsigned int alcAct, unsigned int aclTop);

private:
  std::vector<QuadContainer*>*  _totalQuadList;
  std::string         _nameTempFile;
  std::ofstream       _file;
  unsigned int        _tempNumber;
  unsigned int        _quadNumber;
  std::string write(std::string op, std::string arg1, std::string arg2, std::string des);
  void writeOp(std::string op, std::string des, std::string arg1, std::string arg2);
  unsigned int _offsetTemp = 0;
  map<string, unsigned int> _mapOffTemp;
  unsigned int _totalTemp = 0;
  unsigned int _floatRegister = 0;
};

#endif
