#include  "quad.hh"

Quad::Quad(std::string op, std::string leftOperand, std::string rightOperand, std::string destiny)
  : _operator     ( op            )
  , _leftOperand  ( leftOperand   )
  , _rightOperand ( rightOperand  )
  , _destiny      ( destiny       )
{}
  
void Quad::print()
{
  std::cout << _operator << " " << _leftOperand << " " << _rightOperand << " " << _destiny << '\n';
}

SumQuad::SumQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("+", leftOperand, rightOperand, destiny)
{}

SubQuad::SubQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("-", leftOperand, rightOperand, destiny)
{}

DivQuad::DivQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("/", leftOperand, rightOperand, destiny)
{}

MulQuad::MulQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("*", leftOperand, rightOperand, destiny)
{}

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
  : Quad("<", leftOperand, rightOperand, destiny)
{}

LessEqualQuad::LessEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("=<", leftOperand, rightOperand, destiny)
{}

GreaterThanQuad::GreaterThanQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad(">", leftOperand, rightOperand, destiny)
{}

GreaterEqualQuad::GreaterEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad(">=", leftOperand, rightOperand, destiny)
{}

EqualQuad::EqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("==", leftOperand, rightOperand, destiny)
{}

NotEqualQuad::NotEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("=/=", leftOperand, rightOperand, destiny)
{}

IdBoolQuad::IdBoolQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("if", leftOperand, rightOperand, destiny)
{}

GotoQuad::GotoQuad()
  : Quad("goto", "", "", "")
{}

DespQuad::DespQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("[]", leftOperand, rightOperand, destiny)
{}

ParamQuad::ParamQuad(std::string destiny)
  : Quad("param", "", "", destiny)
{}

CallQuad::CallQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("call", leftOperand, rightOperand, destiny)
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
