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
    Quad(std::string op, std::string leftOperand, Type* type, std::string rightOperand, std::string destiny);
    virtual void print();
    void setDestiny(std::string destiny)    { _destiny = destiny;  }
    virtual bool isJump()                   { return false;        }
    virtual bool isTag()                    { return false;        }
    virtual int  getAddress()               { return 0;            }
    virtual set<string> getUsedVariables()  {  set<string> s = *(new set<string>);
                                              return s; }
    virtual std::string toSpim()            { return "";};

    std::string _operator;
    std::string _leftOperand;
    Type* _type;
    std::string _rightOperand;
    std::string _destiny;
};

class SumQuad : public Quad
{
  public:
    SumQuad(std::string leftOperand, Type* type, std::string rightOperand, std::string destiny);
    SumQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    std::string toSpim();
};

class SubQuad : public Quad
{
  public:
    SubQuad(std::string leftOperand, Type* type, std::string rightOperand, std::string destiny);
    SubQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    std::string toSpim();
};

class DivQuad : public Quad
{
  public:
    DivQuad(std::string leftOperand, Type* type, std::string rightOperand, std::string destiny);
    DivQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    std::string toSpim();
};

class MulQuad : public Quad
{
  public:
    MulQuad(std::string leftOperand, Type* type, std::string rightOperand, std::string destiny);
    MulQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    std::string toSpim();
};

class RemQuad : public Quad
{
  public:
    RemQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class PowQuad : public Quad
{
  public:
    PowQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class UmQuad : public Quad
{
  public:
    UmQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class JumpQuad : public Quad
{
  public:
    JumpQuad(std::string op, std::string leftOperand, std::string rightOperand, std::string destiny);
    bool isJump()     { return true;            }
    int  getAddress() { return atoi(_destiny.c_str());  }
};

class LessThanQuad : public JumpQuad
{
  public:
    LessThanQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class LessEqualQuad : public JumpQuad
{
  public:
    LessEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class GreaterThanQuad : public JumpQuad
{
  public:
    GreaterThanQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class GreaterEqualQuad : public JumpQuad
{
  public:
    GreaterEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class EqualQuad : public JumpQuad
{
  public:
    EqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class NotEqualQuad : public JumpQuad
{
  public:
    NotEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class IdBoolQuad : public JumpQuad
{
  public:
    IdBoolQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class GotoQuad : public JumpQuad
{
  public:
    GotoQuad();
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
};

class DespQuad : public Quad
{
  public:
    DespQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class DespEqualQuad : public Quad
{
  public:
    DespEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class AssignQuad : public Quad
{
  public:
    AssignQuad(std::string leftOperand, std::string destiny);
};

class AssignIndirectQuad : public Quad
{
  public:
    AssignIndirectQuad(std::string leftOperand, std::string destiny);
};

class ParamQuad : public Quad
{
  public:
    ParamQuad(std::string destiny);
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
};

class FlagQuad : public Quad
{
  public:
    FlagQuad(std::string destiny);
    bool isTag()  { return true;  }

};

class RefQuad : public Quad
{
  public:
    RefQuad(std::string leftOperand, std::string destiny);
};

class AndQuad : public Quad
{
  public:
    AndQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class OrQuad : public Quad
{
  public:
    OrQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class NotQuad : public Quad
{
  public:
    NotQuad(std::string leftOperand, std::string destiny);
};

class LessThanQuadExpr : public Quad
{
  public:
    LessThanQuadExpr(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class LessEqualQuadExpr : public Quad
{
  public:
    LessEqualQuadExpr(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class GreaterThanQuadExpr : public Quad
{
  public:
    GreaterThanQuadExpr(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class GreaterEqualQuadExpr : public Quad
{
  public:
    GreaterEqualQuadExpr(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class EqualQuadExpr : public Quad
{
  public:
    EqualQuadExpr(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class NotEqualQuadExpr : public Quad
{
  public:
    NotEqualQuadExpr(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class EndQuad : public Quad
{
  public:
    EndQuad();
};

#endif
