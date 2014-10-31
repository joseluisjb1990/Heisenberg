#ifndef QUAD_HH
#define QUAD_HH
#include<string>
#include <iostream>

class Quad
{
  public:
    Quad(std::string op, std::string leftOperand, std::string rightOperand, std::string destiny);
    virtual void print();
    void setDestiny(std::string destiny) { _destiny = destiny; };

  private:
    std::string _operator;
    std::string _leftOperand;
    std::string _rightOperand;
    std::string _destiny;
};

class SumQuad : public Quad
{
  public:
    SumQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class SubQuad : public Quad
{
  public:
    SubQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class DivQuad : public Quad
{
  public:
    DivQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class RemQuad : public Quad
{
  public:
    RemQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class MulQuad : public Quad
{
  public:
    MulQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
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

class LessThanQuad : public Quad
{
  public:
    LessThanQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class LessEqualQuad : public Quad
{
  public:
    LessEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class GreaterThanQuad : public Quad
{
  public:
    GreaterThanQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class GreaterEqualQuad : public Quad
{
  public:
    GreaterEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class EqualQuad : public Quad
{
  public:
    EqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class NotEqualQuad : public Quad
{
  public:
    NotEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class IdBoolQuad : public Quad
{
  public:
    IdBoolQuad(std::string leftOperand, std::string rightOperand, std::string destiny);
};

class GotoQuad : public Quad
{
  public:
    GotoQuad();
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

#endif
