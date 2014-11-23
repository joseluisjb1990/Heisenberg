#ifndef QUAD_HH
#define QUAD_HH
#include<string>
#include<stdio.h>
#include<set>
#include<iostream>
#include<fstream>
#include<map>
#include"type.hh"

using namespace std;

class Quad
{
  public:
    Quad(std::string op, std::string leftOperand, std::string rightOperand, std::string destiny);
    Quad(std::string op , std::string leftOperand
                        , Type* leftType
                        , std::string rightOperand
                        , Type* rightType
                        , std::string destiny
                        );

    Quad(std::string op , std::string leftOperand
                        , Type* leftType
                        , std::string rightOperand
                        , Type* rightType
                        , std::string destiny
                        , Type* destinyType
                        );
    virtual void print();
    void setDestiny(std::string destiny)    { _destiny = destiny;  }
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
    virtual string getDefinedVariable()     { if(useVariables()) return _destiny; else return *(new std::string()); }
    virtual bool useVariables()             { return false; }
    virtual std::string toSpim()            { return "";    }
    void susVarReg(string regLeft, string regRight, string regDes)  {  
                                                                      if(!regLeft .empty()) _leftOperand  = regLeft; 
                                                                      if(!regRight.empty()) _rightOperand = regRight; 
                                                                      if(!regDes  .empty()) _destiny      = regDes; 
                                                                    }
    virtual bool isExpQuad()    { return false;   }
    virtual bool isAssign()     { return false;   }
    virtual bool isDesp()       { return false;   }
    virtual bool isDespEqual()  { return false;   }
    virtual bool isParam()      { return false;   }
    virtual bool isReturn()     { return false;   }
    virtual bool isCall()       { return false;   }
    virtual bool isJump()       { return false;   }
    virtual bool isTag()        { return false;   }
    virtual bool isEnd()        { return false;   }
    bool isLiveVar(string s) { return  _liveVar.find(s) != _liveVar.end();  }
    static map<string, Type*> tablaTemporales;
    std::string _operator     = *(new std::string());
    std::string _leftOperand  = *(new std::string());
    Type*       _leftType     = new EmptyType();
    std::string _rightOperand = *(new std::string());
    Type*       _rightType    = new EmptyType();
    std::string _destiny      = *(new std::string());
    Type*       _destinyType  = new EmptyType();
    set<string> _liveVar;
};

class SumQuad : public Quad
{
  public:
    SumQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny);
    SumQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny, Type* destinyType);
    SumQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
    std::string toSpim();
    bool isExpQuad() { return true; }
};

class SubQuad : public Quad
{
  public:
    SubQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny);
    SubQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    SubQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny, Type* destinyType);
    bool useVariables()             { return true; }
    bool isExpQuad() { return true; }
    std::string toSpim();

};

class DivQuad : public Quad
{
  public:
    DivQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny);
    DivQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    DivQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny, Type* destinyType);
    bool useVariables()             { return true; }
    bool isExpQuad() { return true; }
    std::string toSpim();
};

class MulQuad : public Quad
{
  public:

    MulQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny);
    MulQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    MulQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny, Type* destinyType);
    bool useVariables()             { return true; }
    bool isExpQuad() { return true; }
    std::string toSpim();

};

class RemQuad : public Quad
{
  public:
    RemQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
    bool isExpQuad() { return true; }
    std::string toSpim();
};

class PowQuad : public Quad
{
  public:
    PowQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool isExpQuad() { return true; }
    bool useVariables()             { return true; }
};

class UmQuad : public Quad
{
  public:
    UmQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()             { return true; }
    bool isExpQuad() { return true; }
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
    std::string toSpim();
};

class DespQuad : public Quad
{
  public:
    DespQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()              { return true; }
    bool isDesp()                    { return true;  }
    std::string toSpim();
};

class DespEqualQuad : public Quad
{
  public:
    DespEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    bool useVariables()  { return true; }
    bool isDespEqual()   { return true; }
    std::string toSpim();
};

class AssignQuad : public Quad
{
  public:
    AssignQuad(std::string leftOperand, std::string destiny);
    bool useVariables()             { return true; }
    bool isAssign()                 { return true; }
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
    bool useVariables()             { return true;     }
    bool isParam()                  { return true;    }
    std::string toSpim();
};

class CallQuad : public Quad
{
  public:
    CallQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
    CallQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny);
    std::string toSpim();
    bool isCall() { return true;  }
};

class ReturnQuad : public Quad
{
  public:
    ReturnQuad(std::string destiny);
    bool useVariables()               { return true;    }
    bool isReturn()                   { return true;    }
    std::string toSpim();
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

class LoadQuad : public Quad
{
  public:
    LoadQuad(std::string leftOperand, std::string destiny);
};

class StoreQuad : public Quad
{
  public:
    StoreQuad(std::string leftOperand, std::string destiny);
};

class EndQuad : public Quad
{
  public:
    EndQuad();
    EndQuad(string destiny);
    bool isEnd()        { return true;   }
};

class BeginQuad : public Quad
{
  public:
    BeginQuad();
};

class WriteQuad : public Quad
{
  public:
    WriteQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny);
    std::string toSpim();
};

class ReadQuad : public Quad
{
  public:
    ReadQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny);
    std::string toSpim();
};
#endif
