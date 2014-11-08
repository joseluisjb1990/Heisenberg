#ifndef QUAD_HH
#define QUAD_HH
#include<string>
#include<stdio.h>
#include<set>
#include<iostream>
#include<fstream>
#include"type.hh"

using namespace std;

class Quad
{
  public:
    Quad(std::string op, std::string leftOperand, std::string rightOperand, std::string destiny);
    Quad(std::string op, std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, 
                                                                            std::string destiny);
    virtual void print();
    void setDestiny(std::string destiny)    { _destiny = destiny;  }
    virtual bool isJump()                   { return false;        }
    virtual bool isTag()                    { return false;        }
    virtual bool isMain()                   { return false;        }
    virtual int  getAddress()               { return 0;            }
    virtual set<string> getUsedVariables()  {  
                                               if(useVariables())
                                               {
                                                set<string> s = *(new set<string>());
                                                if(!_leftOperand.empty())  s.insert(_leftOperand);
                                                if(!_rightOperand.empty()) s.insert(_rightOperand);
                                                return s;
                                               }
                                               return *(new set<string>());
                                            }
    virtual string getDefinedVariable()     { if(useVariables()) return _destiny; else return *( new std::string()); }
    virtual bool useVariables()             { return false; }
    virtual std::string toSpim()            { return "";};
    
    std::string _operator     = *(new std::string());
    std::string _leftOperand  = *(new std::string());
    Type*       _leftType;
    std::string _rightOperand = *(new std::string());
    Type*       _rightType;  
    std::string _destiny      = *(new std::string());
};

class SumQuad : public Quad
{
  public:
    SumQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny);
    SumQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
    std::string toSpim();

};

class SubQuad : public Quad
{
  public:
    SubQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny);
    SubQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
    std::string toSpim();

};

class DivQuad : public Quad
{
  public:
    DivQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny);
    DivQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
    std::string toSpim();


};

class MulQuad : public Quad
{
  public:

    MulQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny);
    MulQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
    std::string toSpim();

};

class RemQuad : public Quad
{
  public:
    RemQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
    std::string toSpim();
};

class PowQuad : public Quad
{
  public:
    PowQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
};

class UmQuad : public Quad
{
  public:
    UmQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
    std::string toSpim();
};

class JumpQuad : public Quad
{
  public:
    JumpQuad(std::string op, std::string leftOperand, std::string rightOperand, std::string destiny);
    bool isJump()                           { return true;            }
    int  getAddress()                       { return atoi(_destiny.c_str());  }
    virtual bool useVariables()             { return true; }
};

class LessThanQuad : public JumpQuad
{
  public:
    LessThanQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    std::string toSpim();
};

class LessEqualQuad : public JumpQuad
{
  public:
    LessEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    std::string toSpim();
};

class GreaterThanQuad : public JumpQuad
{
  public:
    GreaterThanQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    GreaterThanQuad(std::string leftOperand, std::string rightOperand);
    std::string toSpim();
};

class GreaterEqualQuad : public JumpQuad
{
  public:
    GreaterEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    std::string toSpim();
};

class EqualQuad : public JumpQuad
{
  public:
    EqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    EqualQuad(std::string leftOperand, std::string rightOperand);
    std::string toSpim();
};

class NotEqualQuad : public JumpQuad
{
  public:
    NotEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    std::string toSpim();
};

class IdBoolQuad : public JumpQuad
{
  public:
    IdBoolQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    std::string toSpim();
};

class GotoQuad : public JumpQuad
{
  public:
    GotoQuad();
    GotoQuad(std::string destiny);
    bool useVariables()             { return false; }
};

class GotoLineQuad : public Quad
{
	public:
  		GotoLineQuad(std::string destiny);
};

class GotoEmptyQuad : public Quad
{
	public:
  		GotoEmptyQuad(std::string leftOperand, std::string rightOperand);
};

class GotoEmptyIdForQuad : public Quad
{
	public:
  		GotoEmptyIdForQuad(std::string leftOperand, std::string rightOperand);
      bool useVariables()             { return true; }
};

class DespQuad : public Quad
{
  public:
    DespQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
};

class DespEqualQuad : public Quad
{
  public:
    DespEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
};

class AssignQuad : public Quad
{
  public:
    AssignQuad(std::string leftOperand, std::string destiny);
    bool useVariables()             { return true; }
};

class AssignIndirectQuad : public Quad
{
  public:
    AssignIndirectQuad(std::string leftOperand, std::string destiny);
    bool useVariables()             { return true; }
};

class ParamQuad : public Quad
{
  public:
    ParamQuad(std::string destiny);
    bool useVariables()             { return true; }
};

class CallQuad : public Quad
{
  public:
    CallQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class ReturnQuad : public Quad
{
  public:
    ReturnQuad(std::string destiny);
    bool useVariables()             { return true; }
};

class FlagQuad : public Quad
{
  public:
    FlagQuad(std::string destiny);
    bool isTag()  { return true;  }
    bool isMain() { if (_operator == "oso") return true; else return false; }
    std::string toSpim();

};

class RefQuad : public Quad
{
  public:
    RefQuad(std::string leftOperand, std::string destiny);
    bool useVariables()             { return true; }
};

class AndQuad : public Quad
{
  public:
    AndQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
};

class OrQuad : public Quad
{
  public:
    OrQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
};

class NotQuad : public Quad
{
  public:
    NotQuad(std::string leftOperand, std::string destiny);
    bool useVariables()             { return true; }
};

class LessThanQuadExpr : public Quad
{
  public:
    LessThanQuadExpr(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
};

class LessEqualQuadExpr : public Quad
{
  public:
    LessEqualQuadExpr(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
};

class GreaterThanQuadExpr : public Quad
{
  public:
    GreaterThanQuadExpr(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
};

class GreaterEqualQuadExpr : public Quad
{
  public:
    GreaterEqualQuadExpr(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
};

class EqualQuadExpr : public Quad
{
  public:
    EqualQuadExpr(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
};

class NotEqualQuadExpr : public Quad
{
  public:
    NotEqualQuadExpr(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
};

class EndQuad : public Quad
{
  public:
    EndQuad();
};

class BeginQuad : public Quad
{
  public:
    BeginQuad();
};
#endif
