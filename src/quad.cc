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

Quad::Quad( std::string op, std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny, Type* destinyType)
  : _operator     ( op            )
  , _leftOperand  ( leftOperand   )
  , _leftType     ( leftType      )
  , _rightOperand ( rightOperand  )
  , _rightType    ( rightType     )
  , _destiny      ( destiny       )
  , _destinyType  ( destinyType   )
{}

JumpQuad::JumpQuad(std::string op, std::string leftOperand, std::string rightOperand, std::string destiny)
 : Quad(op, leftOperand, rightOperand, destiny)
{}

void Quad::print()
{
  std::cout <<  _operator       << " " 
            <<  _leftOperand    << " "       
            <<  _rightOperand   << " "  
            <<  _destiny        << " ";
}

map<string, Type*> Quad::tablaTemporales;

SumQuad::SumQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("+", leftOperand, rightOperand, destiny)
{}

SumQuad::SumQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny)
  : Quad("+", leftOperand, leftType, rightOperand, rightType, destiny)
  {}

SumQuad::SumQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny, Type* destinyType)
  : Quad("+", leftOperand, leftType, rightOperand, rightType, destiny, destinyType)
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

SubQuad::SubQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny, Type* destinyType)
  : Quad("-", leftOperand, leftType, rightOperand, rightType, destiny, destinyType)
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

DivQuad::DivQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny, Type* destinyType)
  : Quad("/", leftOperand, leftType, rightOperand, rightType, destiny, destinyType)
  {}

std::string DivQuad::toSpim() {
 
    if (_leftType->isInt()) {  
        return "div "+ _leftOperand + " " +  _rightOperand + "\n   mflo " + _destiny + "\n" ;
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

MulQuad::MulQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny, Type* destinyType)
  : Quad("*", leftOperand, leftType, rightOperand, rightType, destiny, destinyType)
  {}

std::string MulQuad::toSpim() {
 
    if (_leftType->isInt()) {  
        return "mult " + _leftOperand + ", " +  _rightOperand + "\n   mflo " + _destiny + "\n" ;;
       
    } else if (_leftType->isFloat()) {
        return "mul.s " +  _destiny + " " + _leftOperand + " " +  _rightOperand;
    };

    return "";
};


RemQuad::RemQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("%", leftOperand, rightOperand, destiny)
{}

std::string RemQuad::toSpim() {
  if (_leftType->isInt()) 
      return "div "+ _leftOperand + " " +  _rightOperand + "\n   mfhi " + _destiny + "\n" ;
     
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
        return "sub " +  _destiny + ", $0 " + _leftOperand;
       
    } else if (_leftType->isFloat()) {
        return "neg.s "+  _destiny + ", " + _leftOperand;
    };

    return "sub " +  _destiny + ", $0 " + _leftOperand;

    return "";
};

LessThanQuad::LessThanQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad("<", leftOperand, rightOperand, destiny)
{}

std::string LessThanQuad::toSpim() {
  return "blt " + _leftOperand + " " + _rightOperand  + " bloque" + _destiny;
};

LessEqualQuad::LessEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad("=<", leftOperand, rightOperand, destiny)
{}

std::string LessEqualQuad::toSpim() {
  return "ble " + _leftOperand + " " + _rightOperand  + " bloque" + _destiny;
};

GreaterThanQuad::GreaterThanQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad(">", leftOperand, rightOperand, destiny)
{}

GreaterThanQuad::GreaterThanQuad(std::string leftOperand, std::string rightOperand)
  : JumpQuad(">", leftOperand, rightOperand, "")
{}

std::string GreaterThanQuad::toSpim() {
  return "bgt " + _leftOperand + " " + _rightOperand  + " bloque" + _destiny;
};

GreaterEqualQuad::GreaterEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad(">=", leftOperand, rightOperand, destiny)
{}

std::string GreaterEqualQuad::toSpim() {
  return "bge " + _leftOperand + " " + _rightOperand  + " bloque" + _destiny;
};

EqualQuad::EqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad("==", leftOperand, rightOperand, destiny)
{}

EqualQuad::EqualQuad(std::string leftOperand, std::string rightOperand)
  : JumpQuad("==", leftOperand, rightOperand, "")
{}

std::string EqualQuad::toSpim() {
  return "beq " + _leftOperand + " " + _rightOperand  + " bloque" + _destiny;
};

NotEqualQuad::NotEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : JumpQuad("=/=", leftOperand, rightOperand, destiny)
{}

std::string NotEqualQuad::toSpim() {
  return "bne " + _leftOperand + " " + _rightOperand  + " bloque" + _destiny;
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

std::string GotoQuad::toSpim()
{
  return "b bloque" + _destiny;
}

GotoQuad::GotoQuad(std::string destiny)
  : JumpQuad("goto", "", "", destiny)
{}

DespQuad::DespQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("[]=", leftOperand, rightOperand, destiny)
{}

std::string DespQuad::toSpim()
{
  return   "add "      + _leftOperand   + " "   + _leftOperand  + " "           + _destiny 
       +   "\n   sw "  + _rightOperand  + ", "  + "("           + _leftOperand  + ")"
       +   "\n   sub " + _leftOperand   + " "   + _leftOperand  + " "           + _destiny
       ;
}

DespEqualQuad::DespEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("=[]", leftOperand, rightOperand, destiny)
{}

std::string DespEqualQuad::toSpim()
{
  return   "add "      + _leftOperand   + " "   + _leftOperand  + " "           + _destiny 
       +   "\n   lw "  + _rightOperand  + ", "  + "("           + _leftOperand  + ")"
       +   "\n   sub " + _leftOperand   + " "   + _leftOperand  + " "           + _destiny
       ;
}

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
    return "sw 0($sp), " + _leftOperand + "\n    sub $sp, $sp, 4";
};

CallQuad::CallQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("call", leftOperand, rightOperand, destiny)
{}

CallQuad::CallQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny)
  : Quad("call", leftOperand, leftType, rightOperand, rightType, destiny)
{}

std::string CallQuad::toSpim() {
    return "sw 0($sp), " + _destiny + "\n    sub $sp, $sp, 4 \n    jal " + _leftOperand;
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


std::string FlagQuad::toSpim2() {
     
	std::string res = "";

	res+= "    sw 0($sp), $ra"  "\n    sub $sp, $sp, 4 \n";
	res+= "    sw 0($sp), $fp"  "\n    sub $sp, $sp, 4 \n";
	res+= "    add $fp, $sp, 8";
	return res;

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

WriteQuad::WriteQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny)
  : Quad("escribir", leftOperand, leftType, rightOperand, rightType, destiny)
{}

std::string WriteQuad::toSpim() {

    if (_leftType->isInt()) {   
    	return "move $a0, " + _leftOperand + "\n    li $v0, 1 \n    syscall";
    } else if (_leftType->isFloat()) {
        return "move.s $f12, " + _leftOperand + "\n    li $v0, 2 \n    syscall"; 
    } else if (_leftType->isString()) {
    	return "move $a0, " + _leftOperand + "\n    li $v0, 4 \n    syscall";
    };
	 
    return "";
}

ReadQuad::ReadQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny)
  : Quad("leer", leftOperand, leftType, rightOperand, rightType, destiny)
{}

std::string ReadQuad::toSpim() {

    if (_leftType->isInt()) {   
    	return "li $v0, 5 \n    syscall \n    move " + _destiny + ", $v0";
    } else if (_leftType->isFloat()) {
        return "li $v0, 6 \n    syscall \n    mfc1 " + _destiny + ", $f0";  
    };
	 
    return "";
}

EndQuad::EndQuad()
  : Quad("end", "", "", "")
{}

std::string EndQuad::toSpim() {
	return "li $v0, 10 \n    syscall";
}

BeginQuad::BeginQuad()
  : Quad("begin", "", "", "")
{}

LoadQuad::LoadQuad(std::string leftOperand, std::string destiny)
  : Quad("ld", leftOperand, "", destiny)
{}

StoreQuad::StoreQuad(std::string leftOperand, std::string destiny)
  : Quad("st", leftOperand, "", destiny)
{}
#endif
