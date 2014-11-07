#ifndef QUAD_CC
#define QUAD_CC
#include  "quad.hh"

Quad::Quad(std::string op, std::string leftOperand, std::string rightOperand, std::string destiny)
  : _operator     ( op            )
  , _leftOperand  ( leftOperand   )
  , _rightOperand ( rightOperand  )
  , _destiny      ( destiny       )
{}

Quad::Quad( std::string op, std::string leftOperand, Type* type, std::string rightOperand, std::string destiny)
  : _operator     ( op            )
  , _leftOperand  ( leftOperand   )
  , _type         ( type          )
  , _rightOperand ( rightOperand  )
  , _destiny      ( destiny       )
{}

JumpQuad::JumpQuad(std::string op, std::string leftOperand, std::string rightOperand, std::string destiny)
 : Quad(op, leftOperand, rightOperand, destiny)
{}

void Quad::print()
{
  std::cout << _operator << " " << _leftOperand << " " << _rightOperand << " " << _destiny << '\n';
}

SumQuad::SumQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("+", leftOperand, rightOperand, destiny)
{}

SumQuad::SumQuad(std::string leftOperand, Type* type, std::string rightOperand, std::string destiny)
  : Quad("+", leftOperand, type, rightOperand, destiny)
  {}

 std::string SumQuad::toSpim() {
 
    if (_type->isInt()) {

      return "add "+  _destiny + " " + _leftOperand + " " +  _rightOperand;
       
    } else if (_type->isFloat()) {
      return "add.s "+  _destiny + " " + _leftOperand + " " +  _rightOperand;
    };

    return "";
};

SubQuad::SubQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("-", leftOperand, rightOperand, destiny)
{}

SubQuad::SubQuad(std::string leftOperand, Type* type, std::string rightOperand, std::string destiny)
  : Quad("-", leftOperand, type, rightOperand, destiny)
{}

 std::string SubQuad::toSpim() {
 
    if (_type->isInt()) {
        
      return "sub "+  _destiny + " " + _leftOperand + " " +  _rightOperand;
       
    } else if (_type->isFloat()) {
      return "sub.s "+  _destiny + " " + _leftOperand + " " +  _rightOperand;
    };

    return "";
};

DivQuad::DivQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("/", leftOperand, rightOperand, destiny)
{}

DivQuad::DivQuad(std::string leftOperand, Type* type, std::string rightOperand, std::string destiny)
  : Quad("/", leftOperand, type, rightOperand, destiny)
{}

std::string DivQuad::toSpim() {
 
    if (_type->isInt()) {
        
      return "div "+  _destiny + " " + _leftOperand + " " +  _rightOperand;
       
    } else if (_type->isFloat()) {
      return "div.s "+  _destiny + " " + _leftOperand + " " +  _rightOperand;
    };

    return "";
};


MulQuad::MulQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("*", leftOperand, rightOperand, destiny)
{}

MulQuad::MulQuad(std::string leftOperand, Type* type, std::string rightOperand, std::string destiny)
  : Quad("*", leftOperand, type, rightOperand, destiny)
{}

std::string MulQuad::toSpim() {
 
    if (_type->isInt()) {
        
      return "mul "+  _destiny + " " + _leftOperand + " " +  _rightOperand;
       
    } else if (_type->isFloat()) {
      return "mul.s "+  _destiny + " " + _leftOperand + " " +  _rightOperand;
    };

    return "";
};


RemQuad::RemQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("%", leftOperand, rightOperand, destiny)
{}

PowQuad::PowQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("**", leftOperand, rightOperand, destiny)
{}

UmQuad::UmQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("um", leftOperand, rightOperand, destiny)
{}

LessThanQuad::LessThanQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad("<", leftOperand, rightOperand, destiny)
{}

LessEqualQuad::LessEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad("=<", leftOperand, rightOperand, destiny)
{}

GreaterThanQuad::GreaterThanQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad(">", leftOperand, rightOperand, destiny)
{}

GreaterThanQuad::GreaterThanQuad(std::string leftOperand, std::string rightOperand)
  : JumpQuad(">", leftOperand, rightOperand, "")
{}

GreaterEqualQuad::GreaterEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad(">=", leftOperand, rightOperand, destiny)
{}

EqualQuad::EqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad("==", leftOperand, rightOperand, destiny)
{}

EqualQuad::EqualQuad(std::string leftOperand, std::string rightOperand)
  : JumpQuad("==", leftOperand, rightOperand, "")
{}

NotEqualQuad::NotEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad("=/=", leftOperand, rightOperand, destiny)
{}

IdBoolQuad::IdBoolQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad("if", leftOperand, rightOperand, destiny)
{}

GotoLineQuad::GotoLineQuad(std::string destiny)
  : Quad("goto", "", "", destiny)
{}

GotoQuad::GotoQuad()
  : JumpQuad("goto", "", "", "")
{}

GotoQuad::GotoQuad(std::string destiny)
  : JumpQuad("goto", "", "", destiny)
{}

GotoEmptyQuad::GotoEmptyQuad(std::string leftOperand, std::string rightOperand)
  : Quad("if", leftOperand + " > ", rightOperand, " goto")
{}

GotoEmptyIdForQuad::GotoEmptyIdForQuad(std::string leftOperand, std::string rightOperand)
  : Quad("if", leftOperand + " = ", rightOperand, " goto")
{}

DespQuad::DespQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("[]", leftOperand, rightOperand, destiny)
{}

DespEqualQuad::DespEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("[]=", leftOperand, rightOperand, destiny)
{}

AssignQuad::AssignQuad(std::string leftOperand, std::string destiny)
  : Quad(":=", leftOperand, "", destiny)
{}

AssignIndirectQuad::AssignIndirectQuad(std::string leftOperand, std::string destiny)
  : Quad("*:=", leftOperand, "", destiny)
{}

ParamQuad::ParamQuad(std::string destiny)
  : Quad("param", destiny, "", "")
{}

CallQuad::CallQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("call", leftOperand, rightOperand, destiny)
{}

ReturnQuad::ReturnQuad(std::string destiny)
  : Quad("return", destiny, "", "")
{}

FlagQuad::FlagQuad(std::string destiny)
  : Quad(destiny, ":", "", "")
{}

RefQuad::RefQuad(std::string leftOperand, std::string destiny)
  : Quad("&", leftOperand, "", destiny)
{}

AndQuad::AndQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("&", leftOperand, rightOperand, destiny)
{}

OrQuad::OrQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("|", leftOperand, rightOperand, destiny)
{}

NotQuad::NotQuad(std::string leftOperand, std::string destiny)
  : Quad("!", leftOperand, "", destiny)
{}

LessThanQuadExpr::LessThanQuadExpr(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("<", leftOperand, rightOperand, destiny)
{}

LessEqualQuadExpr::LessEqualQuadExpr(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("=<", leftOperand, rightOperand, destiny)
{}

GreaterThanQuadExpr::GreaterThanQuadExpr(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad(">", leftOperand, rightOperand, destiny)
{}

GreaterEqualQuadExpr::GreaterEqualQuadExpr(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad(">=", leftOperand, rightOperand, destiny)
{}

EqualQuadExpr::EqualQuadExpr(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("==", leftOperand, rightOperand, destiny)
{}

NotEqualQuadExpr::NotEqualQuadExpr(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("=/=", leftOperand, rightOperand, destiny)
{}

EndQuad::EndQuad()
  : Quad("end", "", "", "")
{}

BeginQuad::BeginQuad()
  : Quad("begin", "", "", "")
{}

#endif
