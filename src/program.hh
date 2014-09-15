#ifndef PROGRAM_HH
#define PROGRAM_HH

#include "statement.hh"
#include "definition.hh"

class Program : public Node
{
  public:
    Program(std::vector<Definition*>* definitions, Statement* statement);
    Program() {};
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);

  private:
    std::vector<Definition*>* _definitions;
    Statement* _statement;
};
#endif
