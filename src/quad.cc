#ifndef QUAD_CC
#define QUAD_CC
#include  "quad.hh"

Quad::Quad(std::string op, std::string leftOperand, std::string rightOperand, std::string destiny)
  : _operator     ( op            )
  , _leftOperand  ( leftOperand   )
  , _rightOperand ( rightOperand  )
  , _destiny      ( destiny       )
{}

Quad::Quad( std::string op, std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny)
  : _operator     ( op            )
  , _leftOperand  ( leftOperand   )
  , _leftType     ( leftType      )
  , _rightOperand ( rightOperand  )
  , _rightType    ( rightType     )
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

SumQuad::SumQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny)
  : Quad("+", leftOperand, leftType, rightOperand, rightType, destiny)
  {}

std::string SumQuad::toSpim() {
    //Quad::tablaTemporales->(destiny) = 
    if (_leftType->isInt()) {  
        return "add "+  _destiny + ", " + _leftOperand + ", " +  _rightOperand;
       
    } else if (_leftType->isFloat()) {
        return "add.s "+  _destiny + ", " + _leftOperand + ", " +  _rightOperand;
    };

    return "";
};

SubQuad::SubQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("-", leftOperand, rightOperand, destiny)
{}

SubQuad::SubQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny)
  : Quad("-", leftOperand, leftType, rightOperand, rightType, destiny)
{}

 std::string SubQuad::toSpim() {
 
    if (_leftType->isInt()) {  
        return "sub "+  _destiny + ", " + _leftOperand + ", " +  _rightOperand;
       
    } else if (_leftType->isFloat()) {
        return "sub.s "+  _destiny + ", " + _leftOperand + ", " +  _rightOperand;
    };

    return "";
};

DivQuad::DivQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("/", leftOperand, rightOperand, destiny)
{}

DivQuad::DivQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny)
  : Quad("/", leftOperand, leftType, rightOperand, rightType, destiny)
{}

std::string DivQuad::toSpim() {
 
    if (_leftType->isInt()) {  
        return "div "+  _destiny + ", " + _leftOperand + ", " +  _rightOperand;
       
    } else if (_leftType->isFloat()) {
        return "div.s "+  _destiny + ", " + _leftOperand + ", " +  _rightOperand;
    };

    return "";
};


MulQuad::MulQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("*", leftOperand, rightOperand, destiny)
{}

MulQuad::MulQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny)
  : Quad("*", leftOperand, leftType, rightOperand, rightType, destiny)
{}

std::string MulQuad::toSpim() {
 
    if (_leftType->isInt()) {  
        return "mul "+  _destiny + ", " + _leftOperand + ", " +  _rightOperand;
       
    } else if (_leftType->isFloat()) {
        return "mul.s "+  _destiny + ", " + _leftOperand + ", " +  _rightOperand;
    };

    return "";
};


RemQuad::RemQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("%", leftOperand, rightOperand, destiny)
{}

std::string RemQuad::toSpim() {
 
    if (_leftType->isInt()) 
        return "rem "+  _destiny + ", " + _leftOperand + ", " +  _rightOperand;
       
    return "";
};

PowQuad::PowQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("**", leftOperand, rightOperand, destiny)
{}

UmQuad::UmQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("um", leftOperand, rightOperand, destiny)
{}

std::string UmQuad::toSpim() {
 
    if (_leftType->isInt()) {  
        return "neg "+  _destiny + ", " + _leftOperand;
       
    } else if (_leftType->isFloat()) {
        return "neg.s "+  _destiny + ", " + _leftOperand;
    };

    return "";
};

LessThanQuad::LessThanQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad("<", leftOperand, rightOperand, destiny)
{}

std::string LessThanQuad::toSpim() {

    if (_rightOperand == "0") {  
        return "bltz "+ _leftOperand + ", bloque" + _destiny;

    } else if (_leftOperand == "0") { 
        return "bgtz "+ _rightOperand + ", bloque" + _destiny;

    } else {
        return "blt " + _leftOperand + ", " + _rightOperand  + ", bloque" + _destiny;
    }

};

LessEqualQuad::LessEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad("=<", leftOperand, rightOperand, destiny)
{}

std::string LessEqualQuad::toSpim() {

    if (_rightOperand == "0") {  
        return "blez "+ _leftOperand + ", bloque" + _destiny;

    } else if (_leftOperand == "0") { 
        return "bgez "+ _rightOperand + ", bloque" + _destiny;

    } else {
        return "ble " + _leftOperand + ", " + _rightOperand  + ", bloque" + _destiny;
    }

};

GreaterThanQuad::GreaterThanQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad(">", leftOperand, rightOperand, destiny)
{}

GreaterThanQuad::GreaterThanQuad(std::string leftOperand, std::string rightOperand)
  : JumpQuad(">", leftOperand, rightOperand, "")
{}

std::string GreaterThanQuad::toSpim() {

    if (_rightOperand == "0") {  
        return "bgtz "+ _leftOperand + ", bloque" + _destiny; 

    } else if (_leftOperand == "0") { 
        return "bltz "+ _rightOperand + ", bloque" + _destiny;

    } else {
        return "bgt " + _leftOperand + ", " + _rightOperand  + ", bloque" + _destiny;
    }

};

GreaterEqualQuad::GreaterEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad(">=", leftOperand, rightOperand, destiny)
{}

std::string GreaterEqualQuad::toSpim() {

    if (_rightOperand == "0") {  
        return "bgez "+ _leftOperand + ", bloque" + _destiny;

    } else if (_leftOperand == "0") { 
        return "blez "+ _rightOperand + ", bloque" + _destiny;

    } else {
        return "bge " + _leftOperand + ", " + _rightOperand  + ", bloque" + _destiny;
    }

};

EqualQuad::EqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad("==", leftOperand, rightOperand, destiny)
{}

EqualQuad::EqualQuad(std::string leftOperand, std::string rightOperand)
  : JumpQuad("==", leftOperand, rightOperand, "")
{}

std::string EqualQuad::toSpim() {

    if (_rightOperand == "0") {  
        return "beqz "+ _leftOperand + ", bloque" + _destiny;

    } else if (_leftOperand == "0") { 
        return "beqz "+ _rightOperand + ", bloque" + _destiny;

    } else {
        return "beq " + _leftOperand + ", " + _rightOperand  + ", bloque" + _destiny;
    }

};

NotEqualQuad::NotEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad("=/=", leftOperand, rightOperand, destiny)
{}

std::string NotEqualQuad::toSpim() {

    if (_rightOperand == "0") {  
        return "bnez "+ _leftOperand + ", bloque" + _destiny;

    } else if (_leftOperand == "0") { 
        return "bnez "+ _rightOperand + ", bloque" + _destiny;

    } else {
        return "bne " + _leftOperand + ", " + _rightOperand  + ", bloque" + _destiny;
    }

};

IdBoolQuad::IdBoolQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad("if", leftOperand, rightOperand, destiny)
{}

std::string IdBoolQuad::toSpim() {
    return "j bloque" + _destiny;
};

GotoQuad::GotoQuad()
  : JumpQuad("goto", "", "", "")
{}

GotoQuad::GotoQuad(std::string destiny)
  : JumpQuad("goto", "", "", destiny)
{}

std::string GotoQuad::toSpim() {
    return "j bloque" + _destiny;
};

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

std::string ParamQuad::toSpim() {
    return "sw 0($sp), " + _leftOperand + "\n" 
         + "    sub $sp, $sp, " + "#NUMERO TAMANO";

};

CallQuad::CallQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("call", leftOperand, rightOperand, destiny)
{}

std::string CallQuad::toSpim() {
    return "sub $sp, $sp, " + "#NUMERO TAMANO RESULTADO \n" 
         + "    j " + leftOperand;

};


ReturnQuad::ReturnQuad(std::string destiny)
  : Quad("return", destiny, "", "")
{}

FlagQuad::FlagQuad(std::string destiny)
  : Quad(destiny, ":", "", "")
{}

std::string FlagQuad::toSpim() {
 
    if (_operator == "oso") return "main:"; else return _operator + ":";
};

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
