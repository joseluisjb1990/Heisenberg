#ifndef GENERATOR_HH
#define GENERATOR_HH

#define TEMPCHAR "#"
#include<string>
#include<fstream>
#include<stdio.h>

class Generator {};

class IntermediateGen : public Generator
{
public:
  IntermediateGen(const std::string &file);
  void close();
  void gen(std::string op, std::string arg1, std::string arg2, std::string des);
  unsigned int getQuadNumber()  { return _quadNumber++; }
  unsigned int getTempNumber()  { return _tempNumber++; }
  std::string nextTemp()        { return  TEMPCHAR + std::to_string(getTempNumber()); }

private:
  std::string _nameTempFile;
  std::ofstream _file;
  unsigned int _tempNumber;
  unsigned int _quadNumber;
};
#endif
