#ifndef GENERATOR_HH
#define GENERATOR_HH

#define TEMPCHAR "#"
#include<string>
#include<fstream>
#include<stdio.h>
#include"TablaSimbolos.hh"
#include"quad.hh"
#include <iostream>

class QuadContainer
{
  public:
    QuadContainer (Quad* quad, unsigned int numberQuad, bool isLeader);
    void setLeader     () { _isLeader = true; }
    void print         ();

    Quad*         _quad;
    unsigned int  _numberQuad;
    bool          _isLeader;
};

class Generator {};

class IntermediateGen : public Generator
{
public:
  IntermediateGen(const std::string &file, TablaSimbolos * symbolTable);
  void close();
  void gen(std::string op, std::string arg1, std::string arg2, std::string des, std::string com);
  void gen(std::string op, std::string arg1, std::string arg2, std::string des);
  void gen(std::string id);
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
  void emptySlot()              { _file << std::string(30, ' ') << '\n'; _quadNumber++; }
  TablaSimbolos *_symbolTable;
  void print();

private:
  std::vector<QuadContainer*>*  _totalQuadList;
  std::string         _nameTempFile;
  std::ofstream       _file;
  unsigned int        _tempNumber;
  unsigned int        _quadNumber;
  std::string write(std::string op, std::string arg1, std::string arg2, std::string des);
};

#endif
