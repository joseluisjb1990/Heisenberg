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

Quad::Quad( std::string op, std::string leftOperand, std::string rightOperand, std::string destiny, Type* destinyType)
  : _operator     ( op            )
  , _leftOperand  ( leftOperand   )
  , _rightOperand ( rightOperand  )
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

SumQuad::SumQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny, Type* destinyType)
  : Quad("+", leftOperand, leftType, rightOperand, rightType, destiny, destinyType)
  {}

std::string SumQuad::toSpim() {

    if (_leftType->isInt())
    {  
      return "add "+  _destiny + ", " + _leftOperand + ", " +  _rightOperand;
    } 
    else if (_leftType->isFloat())
    {
      return "mtc1 " + _leftOperand + " $f0\n   mtc1 " + _rightOperand + " $f1\n   add.s $f2 $f0 $f1\n    mfc1 " + _destiny + " $f2\n";
    };
    return "";
};

SubQuad::SubQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny, Type* destinyType)
  : Quad("-", leftOperand, leftType, rightOperand, rightType, destiny, destinyType)
  {}

std::string SubQuad::toSpim() {
 
    if (_leftType->isInt())
    {  
        return "sub "+  _destiny + ", " + _leftOperand + ", " +  _rightOperand;
    }
    else if (_leftType->isFloat())
    {
      return "mtc1 " + _leftOperand + " $f0\n   mtc1 " + _rightOperand + " $f1\n   sub.s $f2 $f0 $f1\n    mfc1 " + _destiny + " $f2\n";
    };

    return "";
};

DivQuad::DivQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny, Type* destinyType)
  : Quad("/", leftOperand, leftType, rightOperand, rightType, destiny, destinyType)
  {}

std::string DivQuad::toSpim() {
 
    if (_leftType->isInt())
    {  
      return "div "+ _leftOperand + " " +  _rightOperand + "\n   mflo " + _destiny + "\n" ;
    }
    else if (_leftType->isFloat())
    {
      return "mtc1 " + _leftOperand + " $f0\n   mtc1 " + _rightOperand + " $f1\n   div.s $f2 $f0 $f1\n    mfc1 " + _destiny + " $f2\n";
    };

    return "";
};


MulQuad::MulQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny, Type* destinyType)
  : Quad("*", leftOperand, leftType, rightOperand, rightType, destiny, destinyType)
  {}

std::string MulQuad::toSpim() {
 
    if (_leftType->isInt())
    {
        return "mult " + _leftOperand + ", " +  _rightOperand + "\n   mflo " + _destiny + "\n" ;;
    }   
    else if (_leftType->isFloat())
    {
      return "mtc1 " + _leftOperand + " $f0\n   mtc1 " + _rightOperand + " $f1\n   mul.s $f2 $f0 $f1\n    mfc1 " + _destiny + " $f2\n";
    };

    return "";
};

RemQuad::RemQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny, Type* destinyType)
  : Quad("%", leftOperand, leftType, rightOperand, rightType, destiny, destinyType)
  {}

std::string RemQuad::toSpim() {
  if (_leftType->isInt()) 
      return "div "+ _leftOperand + " " +  _rightOperand + "\n    mfhi " + _destiny + "\n" ;
     
  return "";
};

PowQuad::PowQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny, Type* destinyType)
  : Quad("**", leftOperand, leftType, rightOperand, rightType, destiny, destinyType)
  {}

UmQuad::UmQuad(std::string leftOperand, Type* leftType, std::string destiny, Type* destinyType)
  : Quad("um", leftOperand, leftType, "", new EmptyType(), destiny, destinyType)
  {}

std::string UmQuad::toSpim() {
 
    if (_leftType->isInt()) {  
        return "sub " +  _destiny + ", $0 " + _leftOperand;
       
    }   
    else if (_leftType->isFloat())
    {
      return "mtc1 " + _leftOperand + " $f0\n  neg.s $f2 $f0\n    mfc1 " + _destiny + " $f2\n";
    };

    return "sub " +  _destiny + ", $0 " + _leftOperand;

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
  return   "sub "      + _leftOperand   + ", "   + _leftOperand  + ", "           + _destiny 
       +   "\n   sw "  + _rightOperand  + ", "  + "("           + _leftOperand  + ")"
       +   "\n   add " + _leftOperand   + ", "   + _leftOperand  + ", "           + _destiny
       ;
}

DespEqualQuad::DespEqualQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("=[]", leftOperand, rightOperand, destiny)
{}

std::string DespEqualQuad::toSpim()
{
  return   "sub "      + _leftOperand   + ", "   + _leftOperand  + ", "           + _destiny 
       +   "\n   lw "  + _rightOperand  + ", "  + "("           + _leftOperand  + ")"
       +   "\n   add " + _leftOperand   + ", "   + _leftOperand  + ", "           + _destiny
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
    return "\n   # Codigo emitido por el param\n   sub $sp $sp 4\n   sw " + _leftOperand + " 0($sp) ";
};

CallQuad::CallQuad(std::string leftOperand, std::string rightOperand, std::string destiny)
  : Quad("call", leftOperand, rightOperand, destiny)
{}

CallQuad::CallQuad(std::string leftOperand, Type* leftType, std::string rightOperand, Type* rightType, std::string destiny)
  : Quad("call", leftOperand, leftType, rightOperand, rightType, destiny)
{}

CallQuad::CallQuad(std::string leftOperand, std::string rightOperand, std::string destiny, Type* returnType)
  : Quad("call", leftOperand, rightOperand, destiny, returnType)
{}

std::string CallQuad::toSpim() {
    if(_leftOperand == "escribir")
    {
      int flag = 0;
      if(_destinyType->isInt())
      {
        flag = 1;
        return "li $v0 " + to_string(flag) + "\n   lw $a0 ($sp)\n" + "   syscall\n   add $sp $sp 4\n"; 
      }
      else
      if(_destinyType->isFloat())
      {
        flag = 2;
        return "li $v0 " + to_string(flag) + "\n   lw $a0 ($sp)\n   mtc1 $a0 $f12\n" + "   syscall\n   add $sp $sp 4\n"; 
      }
      else
      if(_destinyType->isString())
      {
        flag = 4;
        return "";
      }
    }
    if(_leftOperand == "leer")
    {
      int flag = 0;
      if(_destinyType->isInt())
      {
        flag = 5;
        return "li $v0 " + to_string(flag) + "\n" + "   syscall\n   move " + _destiny + " $v0\n";
      }
      else
      if(_destinyType->isFloat())
      {
        flag = 6;
        return "";
      }
    }
    return "sub $sp, $sp, 4\n   jal " + _leftOperand + "\n   lw $10 0($sp)\n   add $sp $sp 4\n   add $sp $sp " + to_string(4 * atoi(_rightOperand.c_str())) + "\n";
};

ReturnQuad::ReturnQuad(std::string destiny)
  : Quad("return", destiny, "", "")
{}

std::string ReturnQuad::toSpim() {
    return "sw " + _leftOperand + " 4($fp)\n   b _epilog" + _destiny + "\n";
};

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

EndQuad::EndQuad(string destiny)
  : Quad("end", "", "", destiny)
{}

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
