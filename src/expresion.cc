#ifndef EXPRESSION_CC
#define EXPRESSION_CC
#include "expresion.hh"
#include "definition.hh"

extern TablaSimbolos* tablaSimbolos;

using namespace std;

Expression::Expression() {}

std::string Expression::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Expresión\n";
}

PolarExpr::PolarExpr(std::string valor)
  : valor ( valor )
  {}

std::string PolarExpr::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + getValue();
}

std::string PolarExpr::getValue()
{
  return valor;
}

void PolarExpr::check()
{
  this->set_type(PolarType::getInstance());
}

KodiakExpr::KodiakExpr(std::string valor)
  : valor ( valor )
  {}

std::string KodiakExpr::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + getValue();
}

std::string KodiakExpr::getValue()
{
  return valor;
}

void KodiakExpr::check()
{
  this->set_type(KodiakType::getInstance());
}

HormigueroExpr::HormigueroExpr(std::string valor)
  : valor ( valor )
  {}

std::string HormigueroExpr::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "\"" + getValue() + "\"";
}

std::string HormigueroExpr::getValue()
{
  return valor;
}

void HormigueroExpr::check()
{
  set_type(new HormigueroType());
}

MalayoExpr::MalayoExpr(std::string valor)
  : valor ( valor )
  {}

std::string MalayoExpr::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "'" + getValue() + "'";
}

std::string MalayoExpr::getValue()
{
  return valor;
}

void MalayoExpr::check()
{
  this->set_type(MalayoType::getInstance());
}

PandaExpr::PandaExpr(std::string valor)
  : valor ( valor )
  {}

std::string PandaExpr::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + getValue();
}

std::string PandaExpr::getValue()
{
  return valor;
}

void PandaExpr::check()
{
  this->set_type(PandaType::getInstance());
}

void TrueExpr::toIntermediateGoto(IntermediateGen *intGen)
{
  _trueList = intGen->genEmpty("goto");
}

void TrueExpr::backpatch(bool con, int jumpDes, IntermediateGen *intGen)
{
  if(con)
  {
    intGen->gen(_trueList, jumpDes);
  }
}

void FalseExpr::toIntermediateGoto(IntermediateGen *intGen)
{
  _falseList = intGen->genEmpty("goto");
}

void FalseExpr::backpatch(bool con, int jumpDes, IntermediateGen *intGen)
{
  if(!con)
  {
    intGen->gen(_falseList, jumpDes);
  }
}

Sum::Sum(Expression* izq, Expression* der)
  : izq ( izq )
  , der ( der )
  {}

std::string Sum::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Suma\n" + padding + "Operando izquierdo:\n" + izq->to_string(nesting+1) + "\n" + padding + "Operando derecho:\n" + der->to_string(nesting+1) + "\n";
}

void Sum::check()
{
  izq->check();
  der->check();
  Type* t1 = izq->get_type();
  Type* t2 = der->get_type();

  if ( t1 == t2 and (t1 == PolarType::getInstance() or t1 == KodiakType::getInstance()) ) {
    this->set_type(t1);
  } else if ( t1 == ErrorType::getInstance() or t2 == ErrorType::getInstance() ) {
    this->set_type(ErrorType::getInstance());
  } else {
    error("Cannot apply operator '+' between " + t1->to_string() + " and " + t2->to_string() + "types\n");
    this->set_type(ErrorType::getInstance());
  }
}

void Sum::toIntermediate(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen("+",izq->getTemp(), der->getTemp(),temp, 
              "   // Suma, linea " + std::to_string(get_first_line()));  
  setTemp(temp);
}

Substraction::Substraction(Expression* izq, Expression* der)
  : izq ( izq )
  , der ( der )
  {}

std::string Substraction::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Resta\n" + padding + "Operando izquerdo:\n" + izq->to_string(nesting+1) + "\n" + padding + "Operando derecho:\n" + der->to_string(nesting+1) + "\n";
}

void Substraction::check()
{
  izq->check();
  der->check();
  Type* t1 = izq->get_type();
  Type* t2 = der->get_type();

  if ( t1 == t2 and (t1 == PolarType::getInstance() or t1 == KodiakType::getInstance()) ) {
    this->set_type(t1);
  } else if ( t1 == ErrorType::getInstance() or t2 == ErrorType::getInstance() ) {
    this->set_type(ErrorType::getInstance());
  } else {
    error("Cannot apply operator '-' between " + t1->to_string() + " and " + t2->to_string() + "types\n");
    this->set_type(ErrorType::getInstance());
  }
}

void Substraction::toIntermediate(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen("-",izq->getTemp(), der->getTemp(),temp,
                  "   // Resta, linea " + std::to_string(get_first_line()));  
  setTemp(temp);
}

Multiplication::Multiplication(Expression* izq, Expression* der)
  : izq ( izq )
  , der ( der )
  {}

std::string Multiplication::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Multiplicación\n" + padding + "Operando izquerdo:\n" + izq->to_string(nesting+1) + "\n" + padding + "Operando derecho:\n" + der->to_string(nesting+1) + "\n";
}

void Multiplication::check()
{
  izq->check();
  der->check();
  Type* t1 = izq->get_type();
  Type* t2 = der->get_type();

  if ( t1 == t2 and (t1 == PolarType::getInstance() or t1 == KodiakType::getInstance()) ) {
    this->set_type(t1);
  } else if ( t1 == ErrorType::getInstance() or t2 == ErrorType::getInstance() ) {
    this->set_type(ErrorType::getInstance());
  } else {
    error("Cannot apply operator '*' between " + t1->to_string() + " and " + t2->to_string() + "types\n");
    this->set_type(ErrorType::getInstance());
  }
}

void Multiplication::toIntermediate(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen("*",izq->getTemp(), der->getTemp(),temp,
                  "   // Multiplication, linea " + std::to_string(get_first_line()));  
  setTemp(temp);
}

Division::Division(Expression* izq, Expression* der)
  : izq ( izq )
  , der ( der )
  {}

std::string Division::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "División\n" + padding + "Operando izquerdo:\n" + izq->to_string(nesting+1) + "\n" + padding + "Operando derecho:\n" + der->to_string(nesting+1) + "\n";
}

void Division::check()
{
  izq->check();
  der->check();
  Type* t1 = izq->get_type();
  Type* t2 = der->get_type();

  if ( t1 == t2 and (t1 == PolarType::getInstance() or t1 == KodiakType::getInstance()) ) {
    this->set_type(t1);
  } else if ( t1 == ErrorType::getInstance() or t2 == ErrorType::getInstance() ) {
    this->set_type(ErrorType::getInstance());
  } else {
    error("Cannot apply operator '/' between " + t1->to_string() + " and " + t2->to_string() + "types\n");
    this->set_type(ErrorType::getInstance());
  }
}

void Division::toIntermediate(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen("/",izq->getTemp(), der->getTemp(),temp,
                "   // Division, linea " + std::to_string(get_first_line()));    
  setTemp(temp);
}

Remainder::Remainder(Expression* izq, Expression* der)
  : izq ( izq )
  , der ( der )
  {}

std::string Remainder::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Resto\n" + padding + "Operando izquerdo:\n" + izq->to_string(nesting+1) + "\n" + padding + "Operando derecho:\n" + der->to_string(nesting+1) + "\n";
}

void Remainder::check()
{
  izq->check();
  der->check();
  Type* t1 = izq->get_type();
  Type* t2 = der->get_type();

  if ( t1 == t2 and (t1 == PolarType::getInstance() or t1 == KodiakType::getInstance()) ) {
    this->set_type(t1);
  } else if ( t1 == ErrorType::getInstance() or t2 == ErrorType::getInstance() ) {
    this->set_type(ErrorType::getInstance());
  } else {
    error("Cannot apply operator '%' between " + t1->to_string() + " and " + t2->to_string() + "types\n");
    this->set_type(ErrorType::getInstance());
  }
}

void Remainder::toIntermediate(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen("%",izq->getTemp(), der->getTemp(),temp,
                "   // Modulo, linea " + std::to_string(get_first_line()));    
  setTemp(temp);
}

Power::Power(Expression* izq, Expression* der)
  : izq ( izq )
  , der ( der )
  {}

std::string Power::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Potenciación\n" + padding + "Operando izquerdo:\n" + izq->to_string(nesting+1) + "\n" + padding + "Operando derecho:\n" + der->to_string(nesting+1) + "\n";
}

void Power::check()
{
  izq->check();
  der->check();
  Type* t1 = izq->get_type();
  Type* t2 = der->get_type();

  if ( t1 == t2 and (t1 == PolarType::getInstance() or t1 == KodiakType::getInstance()) ) {
    this->set_type(t1);
  } else if ( t1 == ErrorType::getInstance() or t2 == ErrorType::getInstance() ) {
    this->set_type(ErrorType::getInstance());
  } else {
    error("Cannot apply operator '**' between " + t1->to_string() + " and " + t2->to_string() + "types\n");
    this->set_type(ErrorType::getInstance());
  }
}

void Power::toIntermediate(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen("**",izq->getTemp(), der->getTemp(),temp,
                "   // Potenciación, linea " + std::to_string(get_first_line()));    
  setTemp(temp);
}

Minus::Minus(Expression* operando)
  : operando ( operando )
  {}

std::string Minus::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Menos unario\n" + padding + "Operando:\n" + operando->to_string(nesting+1) + "\n";
}

void Minus::check()
{
  operando->check();
  Type* t = operando->get_type();

  if ( t == PolarType::getInstance() or t == KodiakType::getInstance() or t == ErrorType::getInstance() ) {
    this->set_type(t);
  } else {
    error("Cannot apply operator '-' to " + t->to_string() + " type\n");
    this->set_type(ErrorType::getInstance());
  }
}

void Minus::toIntermediate(IntermediateGen *intGen)
{
  operando->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen("um",operando->getTemp(), " ", temp,  
                  "   // Unario, linea " + std::to_string(get_first_line()));  
  setTemp(temp);
}

Less::Less(Expression* izq, Expression* der)
  : izq ( izq )
  , der ( der )
  {}

std::string Less::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Menor que\n" + padding + "Operando izquerdo:\n" + izq->to_string(nesting+1) + "\n" + padding + "Operando derecho:\n" + der->to_string(nesting+1) + "\n";
}

void Less::check()
{
  izq->check();
  der->check();
  Type* t1 = izq->get_type();
  Type* t2 = der->get_type();

  if ( t1 == t2 and (t1 == PolarType::getInstance() or t1 == KodiakType::getInstance() or t1 == MalayoType::getInstance()) ) {
    this->set_type(PandaType::getInstance());
  } else if ( t1 == ErrorType::getInstance() or t2 == ErrorType::getInstance() ) {
    this->set_type(ErrorType::getInstance());
  } else {
    error("Cannot apply operator '<' between " + t1->to_string() + " and " + t2->to_string() + "types\n");
    this->set_type(ErrorType::getInstance());
  }
}

void Less::toIntermediateGoto(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);

  _trueList   = intGen->genEmpty("if " + izq->getTemp() + " < " + der->getTemp() + " goto");
  _falseList  = intGen->genEmpty("goto");
}

void Less::backpatch(bool con, int jumpDes, IntermediateGen *intGen)
{
  if(con) intGen->gen(_trueList , jumpDes); 
  else    intGen->gen(_falseList, jumpDes);
}

void Less::toIntermediate(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen("<",izq->getTemp(), der->getTemp(),temp,
                "   // Menor, linea " + std::to_string(get_first_line()));    
  setTemp(temp);
}


LessEqual::LessEqual(Expression* izq, Expression* der)
  : izq ( izq )
  , der ( der )
  {}

std::string LessEqual::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Menor o igual que\n" + padding + "Operando izquerdo:\n" + izq->to_string(nesting+1) + "\n" + padding + "Operando derecho:\n" + der->to_string(nesting+1) + "\n";
}

void LessEqual::check()
{
  izq->check();
  der->check();
  Type* t1 = izq->get_type();
  Type* t2 = der->get_type();

  if ( t1 == t2 and (t1 == PolarType::getInstance() or t1 == KodiakType::getInstance() or t1 == MalayoType::getInstance()) ) {
    this->set_type(PandaType::getInstance());
  } else if ( t1 == ErrorType::getInstance() or t2 == ErrorType::getInstance() ) {
    this->set_type(ErrorType::getInstance());
  } else {
    error("Cannot apply operator '=<' between " + t1->to_string() + " and " + t2->to_string() + "types\n");
    this->set_type(ErrorType::getInstance());
  }
}


void LessEqual::toIntermediateGoto(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);

  _trueList   = intGen->genEmpty("if " + izq->getTemp() + " =< " + der->getTemp() + " goto");
  _falseList  = intGen->genEmpty("goto");
}

void LessEqual::backpatch(bool con, int jumpDes, IntermediateGen *intGen)
{
  if(con) intGen->gen(_trueList , jumpDes); 
  else    intGen->gen(_falseList, jumpDes);
}

void LessEqual::toIntermediate(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen("=<",izq->getTemp(), der->getTemp(),temp,
                "   // MenorQue, linea " + std::to_string(get_first_line()));   
  setTemp(temp);
}

Greater::Greater(Expression* izq, Expression* der)
  : izq ( izq )
  , der ( der )
  {}

std::string Greater::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Mayor que\n" + padding + "Operando izquerdo:\n" + izq->to_string(nesting+1) + "\n" + padding + "Operando derecho:\n" + der->to_string(nesting+1) + "\n";
}

void Greater::check()
{
  izq->check();
  der->check();
  Type* t1 = izq->get_type();
  Type* t2 = der->get_type();

  if ( t1 == t2 and (t1 == PolarType::getInstance() or t1 == KodiakType::getInstance() or t1 == MalayoType::getInstance()) ) {
    this->set_type(PandaType::getInstance());
  } else if ( t1 == ErrorType::getInstance() or t2 == ErrorType::getInstance() ) {
    this->set_type(ErrorType::getInstance());
  } else {
    error("Cannot apply operator '>' between " + t1->to_string() + " and " + t2->to_string() + "types\n");
    this->set_type(ErrorType::getInstance());
  }
}

void Greater::toIntermediateGoto(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);

  _trueList   = intGen->genEmpty("if " + izq->getTemp() + " > " + der->getTemp() + " goto");
  _falseList  = intGen->genEmpty("goto");
}

void Greater::backpatch(bool con, int jumpDes, IntermediateGen *intGen)
{
  if(con) intGen->gen(_trueList , jumpDes); 
  else    intGen->gen(_falseList, jumpDes);
}

void Greater::toIntermediate(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen(">",izq->getTemp(), der->getTemp(),temp,
                "   // Mayor, linea " + std::to_string(get_first_line()));    
  setTemp(temp);
}

GreaterEqual::GreaterEqual(Expression* izq, Expression* der)
  : izq ( izq )
  , der ( der )
  {}

std::string GreaterEqual::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Mayor o igual que\n" + padding + "Operando izquerdo:\n" + izq->to_string(nesting+1) + "\n" + padding + "Operando derecho:\n" + der->to_string(nesting+1) + "\n";
}

void GreaterEqual::check()
{
  izq->check();
  der->check();
  Type* t1 = izq->get_type();
  Type* t2 = der->get_type();

  if ( t1 == t2 and (t1 == PolarType::getInstance() or t1 == KodiakType::getInstance() or t1 == MalayoType::getInstance()) ) {
    this->set_type(PandaType::getInstance());
  } else if ( t1 == ErrorType::getInstance() or t2 == ErrorType::getInstance() ) {
    this->set_type(ErrorType::getInstance());
  } else {
    error("Cannot apply operator '>=' between " + t1->to_string() + " and " + t2->to_string() + "types\n");
    this->set_type(ErrorType::getInstance());
  }
}

void GreaterEqual::toIntermediateGoto(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);

  _trueList   = intGen->genEmpty("if " + izq->getTemp() + " >= " + der->getTemp() + " goto");
  _falseList  = intGen->genEmpty("goto");
}

void GreaterEqual::backpatch(bool con, int jumpDes, IntermediateGen *intGen)
{
  if(con) intGen->gen(_trueList , jumpDes); 
  else    intGen->gen(_falseList, jumpDes);
}

void GreaterEqual::toIntermediate(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen(">=",izq->getTemp(), der->getTemp(),temp,
                "   // MayorQue, linea " + std::to_string(get_first_line()));   
  setTemp(temp);
}

Equal::Equal(Expression* izq, Expression* der)
  : izq ( izq )
  , der ( der )
  {}

std::string Equal::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Equivalencia\n" + padding + "Operando izquerdo:\n" + izq->to_string(nesting+1) + "\n" + padding + "Operando derecho:\n" + der->to_string(nesting+1) + "\n";
}

void Equal::check()
{
  izq->check();
  der->check();
  Type* t1 = izq->get_type();
  Type* t2 = der->get_type();

  if ( t1 == t2 and (t1 == PolarType::getInstance() or t1 == KodiakType::getInstance() or t1 == MalayoType::getInstance() or t1 == PandaType::getInstance()) ) {
    this->set_type(PandaType::getInstance());
  } else if ( t1 == ErrorType::getInstance() or t2 == ErrorType::getInstance() ) {
    this->set_type(ErrorType::getInstance());
  } else {
    error("Cannot apply operator '==' between " + t1->to_string() + " and " + t2->to_string() + " types\n");
    this->set_type(ErrorType::getInstance());
  }
}

void Equal::toIntermediateGoto(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);

  _trueList   = intGen->genEmpty("if " + izq->getTemp() + " == " + der->getTemp() + " goto");
  _falseList  = intGen->genEmpty("goto");
}

void Equal::backpatch(bool con, int jumpDes, IntermediateGen *intGen)
{
  if(con) intGen->gen(_trueList , jumpDes); 
  else    intGen->gen(_falseList, jumpDes);
}

void Equal::toIntermediate(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen("==",izq->getTemp(), der->getTemp(),temp,
                "   // Igual, linea " + std::to_string(get_first_line()));   
  setTemp(temp);
}


NotEqual::NotEqual(Expression* izq, Expression* der)
  : izq ( izq )
  , der ( der )
  {}

std::string NotEqual::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Inequivalencia\n" + padding + "Operando izquerdo:\n" + izq->to_string(nesting+1) + "\n" + padding + "Operando derecho:\n" + der->to_string(nesting+1) + "\n";
}

void NotEqual::check()
{
  izq->check();
  der->check();
  Type* t1 = izq->get_type();
  Type* t2 = der->get_type();

  if ( t1 == t2 and (t1 == PolarType::getInstance() or t1 == KodiakType::getInstance() or t1 == MalayoType::getInstance() or t1 == PandaType::getInstance()) ) {
    this->set_type(PandaType::getInstance());
  } else if ( t1 == ErrorType::getInstance() or t2 == ErrorType::getInstance() ) {
    this->set_type(ErrorType::getInstance());
  } else {
    error("Cannot apply operator '=/=' between " + t1->to_string() + " and " + t2->to_string() + "types\n");
    this->set_type(ErrorType::getInstance());
  }
}

void NotEqual::toIntermediateGoto(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);

  _trueList   = intGen->genEmpty("if " + izq->getTemp() + " =/= " + der->getTemp() + " goto");
  _falseList  = intGen->genEmpty("goto");
}

void NotEqual::backpatch(bool con, int jumpDes, IntermediateGen *intGen)
{
  if(con) intGen->gen(_trueList , jumpDes); 
  else    intGen->gen(_falseList, jumpDes);
}

void NotEqual::toIntermediate(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen("=/=",izq->getTemp(), der->getTemp(),temp,
                "   // Distinto, linea " + std::to_string(get_first_line()));    
  setTemp(temp);
}

And::And(Expression* izq, Expression* der)
  : izq ( izq )
  , der ( der )
  {}

std::string And::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Y\n" + padding + "Operando izquerdo:\n" + izq->to_string(nesting+1) + "\n" + padding + "Operando derecho:\n" + der->to_string(nesting+1) + "\n";
}

void And::check()
{
  izq->check();
  der->check();
  Type* t1 = izq->get_type();
  Type* t2 = der->get_type();

  if ( t1 == t2 and t1 == PandaType::getInstance() ) {
    this->set_type(t1);
  } else if ( t1 == ErrorType::getInstance() or t2 == ErrorType::getInstance() ) {
    this->set_type(ErrorType::getInstance());
  } else {
    error("Cannot apply operator '&' between " + t1->to_string() + " and " + t2->to_string() + "types\n");
    this->set_type(ErrorType::getInstance());
  }
}


void And::toIntermediateGoto(IntermediateGen *intGen)
{
  izq->toIntermediateGoto(intGen);
  unsigned int pos = intGen->getQuad();
  der->toIntermediateGoto(intGen);

  izq->backpatch(true, pos, intGen);
}

void And::backpatch(bool con, int jumpDes, IntermediateGen *intGen)
{
  if(con) der->backpatch(true , jumpDes, intGen); 
  else
  {
    der->backpatch(false, jumpDes, intGen);
    izq->backpatch(false, jumpDes, intGen);
  }
}

void And::toIntermediate(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen("&",izq->getTemp(), der->getTemp(),temp,
                "   // Conjuncion, linea " + std::to_string(get_first_line()));    
  setTemp(temp);
}

Or::Or(Expression* izq, Expression* der)
  : izq ( izq )
  , der ( der )
  {}

std::string Or::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "O\n" + padding + "Operando izquerdo:\n" + izq->to_string(nesting+1) + "\n" + padding + "Operando derecho:\n" + der->to_string(nesting+1) + "\n";
}

void Or::check()
{
  izq->check();
  der->check();
  Type* t1 = izq->get_type();
  Type* t2 = der->get_type();

  if ( t1 == t2 and t1 == PandaType::getInstance() ) {
    this->set_type(t1);
  } else if ( t1 == ErrorType::getInstance() or t2 == ErrorType::getInstance() ) {
    this->set_type(ErrorType::getInstance());
  } else {
    error("Cannot apply operator '|' between " + t1->to_string() + " and " + t2->to_string() + "types\n");
    this->set_type(ErrorType::getInstance());
  }
}

void Or::toIntermediateGoto(IntermediateGen *intGen)
{
  izq->toIntermediateGoto(intGen);
  unsigned int pos = intGen->getQuad();
  der->toIntermediateGoto(intGen);

  izq->backpatch(false, pos, intGen);
}

void Or::backpatch(bool con, int jumpDes, IntermediateGen *intGen)
{
  if(!con) der->backpatch(false , jumpDes, intGen); 
  else
  {
    der->backpatch(true, jumpDes, intGen);
    izq->backpatch(true, jumpDes, intGen);
  }
}

void Or::toIntermediate(IntermediateGen *intGen)
{
  izq->toIntermediate(intGen);
  der->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen("|",izq->getTemp(), der->getTemp(),temp,
                "   // Disyuncion, linea " + std::to_string(get_first_line()));   
  setTemp(temp);
}


Not::Not(Expression* operando)
  : operando ( operando )
  {}

std::string Not::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "no\n" + padding + "Operando:\n" + operando->to_string(nesting+1) + "\n";
}

void Not::check()
{
  operando->check();
  Type* t = operando->get_type();

  if ( t == PandaType::getInstance() ) {
    this->set_type(t);
  } else {
    error("Cannot apply operator 'no' to " + t->to_string() + " types\n");
    this->set_type(ErrorType::getInstance());
  }
}

void Not::toIntermediateGoto(IntermediateGen *intGen)
{
  operando->toIntermediateGoto(intGen);

  _desTrue = intGen->getQuad();
  _trueList   = intGen->genEmpty("goto");
  _desFalse = intGen->getQuad();
  _falseList  = intGen->genEmpty("goto");
}

void Not::backpatch(bool con, int jumpDes, IntermediateGen *intGen)
{
  if(!con)
  {
    intGen->gen(_trueList , jumpDes);
    operando->backpatch(true, _desTrue, intGen);
  }
  else
  {
    intGen->gen(_falseList, jumpDes);
    operando->backpatch(false, _desFalse, intGen);
  }
}

void Not::toIntermediate(IntermediateGen *intGen)
{
  operando->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen("no",operando->getTemp(), "", temp,
                "   // Negacion, linea " + std::to_string(get_first_line()));    
  setTemp(temp);
}


SelectorExpr::SelectorExpr(Expression* condicion, Expression* brazoTrue, Expression* brazoFalse)
  : _condicion( condicion )
  , _brazoTrue( brazoTrue )
  , _brazoFalse( brazoFalse )
  {}

std::string SelectorExpr::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Expresión condicional\n" + padding + "Condición:\n" + _condicion->to_string(nesting+1) + "\n" + padding + "Brazo true:\n" + _brazoTrue->to_string(nesting+1) + "\n" + padding + "Brazo false:\n" + _brazoFalse->to_string(nesting+1) + "\n";
}

IDExpr::IDExpr(std::string nombre)
  : LValueExpr()
  , _nombre( nombre )
  {}

IDExpr::IDExpr(std::string nombre, Contenido* c)
  : LValueExpr()
  , _nombre   ( nombre  )
  , _tableRow ( c       )
  {}

std::string IDExpr::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + _nombre;
}

void IDExpr::check(){};

void IDExpr::toIntermediateGoto(IntermediateGen *intGen)
{
  _trueList   = intGen->genEmpty("if " + _nombre + " goto");
  _falseList  = intGen->genEmpty("goto");
}

void IDExpr::backpatch(bool con, int jumpDes, IntermediateGen *intGen)
{
  if(con) intGen->gen(_trueList , jumpDes); 
  else    intGen->gen(_falseList, jumpDes);
}

FunctionExpr::FunctionExpr(std::string name, std::vector<Type*>* parameterTypes, std::vector<Expression*>* parameters, Type* returnType)
  : Expression()
  , _name           ( name           )
  , _parameterTypes ( parameterTypes )
  , _parameters     ( parameters     )
  , _return         ( returnType     )
  {}

FunctionExpr::FunctionExpr( std::string name
                          , std::vector<Type*>* parameterTypes
                          , std::vector<Expression*>* parameters
                          , Type* returnType
                          , std::vector<Parameter*>* defParametros
                          )
  : Expression()
  , _name           ( name           )
  , _parameterTypes ( parameterTypes )
  , _parameters     ( parameters     )
  , _return         ( returnType     )
  , _defParametros  ( defParametros  )
  {}

std::string FunctionExpr::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  std::string str = padding + "Función\n" + padding + "Nombre:\n" + padding + padding + _name + "\n" + padding + "Parametros:\n";
  for (unsigned int i=0; i < _parameters->size(); ++i)
    str += _parameters->at(i)->to_string(nesting+1) + "\n";
  return str;
}

void FunctionExpr::check()
{
  bool ok = true;
  for(unsigned int i=0; i < _parameters->size(); ++i) {
    _parameters->at(i)->check();
    Type* tipo = _parameterTypes->at(i);
    Type* tipoParam = _parameters->at(i)->get_type();
    if ( !tipoParam->compareStructure(tipo) ) {
      ok = false;
      if (tipoParam != ErrorType::getInstance())
        error("Trying to pass a parameter of type '" + tipoParam->to_string() + "' to function '" + _name + "' instead of '" + tipo->to_string() + "'.");
    }
  }
  if (ok) {
    this->set_type(_return);
  } else {
    this->set_type(ErrorType::getInstance());
  }
}

void FunctionExpr::toIntermediateGoto(IntermediateGen *intGen)
{
  toIntermediateAux(intGen);
  _trueList   = intGen->genEmpty("if " + getTemp() + " goto");
  _falseList  = intGen->genEmpty("goto");
}

void FunctionExpr::toIntermediateAux(IntermediateGen *intGen)
{
  for (unsigned int i=0; i < _parameters->size(); ++i)
  {
    _parameters->at(i)->toIntermediate(intGen);

  }

  for (int i=(_parameters->size()-1); -1 < i; --i)
  {
    Expression* p = _parameters->at(i);
    if(_defParametros->at(i)->get_ref())
    {
      std::string t = intGen->nextTemp();
      intGen->gen("&", p->getTemp(), " ", t, "   // Acceso a Memoria");  
      intGen->gen("param", t," "," ", "   // Parametro " + std::to_string(i+1)); 
    } else
    {
      intGen->gen("param", p->getTemp()," "," ", "   // Parametro " + std::to_string(i+1));  
    } 
  }

  std::string temp = intGen->nextTemp();
  intGen->gen("call",_name,std::to_string(_parameters->size()),temp,
       "   // Llamada a Funcion, linea " + std::to_string(get_first_line()));
  setTemp(temp);
}

void FunctionExpr::backpatch(bool con, int jumpDes, IntermediateGen *intGen)
{
  if(con) intGen->gen(_trueList , jumpDes); 
  else    intGen->gen(_falseList, jumpDes);
}

void FunctionExpr::toIntermediate(IntermediateGen *intGen)
{
  toIntermediateAux(intGen);
}

AKodiakExpr::AKodiakExpr(Expression* parameter)
  : _parameter ( parameter     )
  {}

std::string AKodiakExpr::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Función predeterminada a_kodiak\n" + padding + "Parametro:\n" + _parameter->to_string(nesting+1) + "\n";
}

void AKodiakExpr::check()
{
  _parameter->check();
  Type* t = _parameter->get_type();

  if ( t == PolarType::getInstance() or t == KodiakType::getInstance() ) {
    this->set_type(KodiakType::getInstance());
  } else if ( t == ErrorType::getInstance() ) {
    this->set_type(ErrorType::getInstance());
  } else {
    error("Cannot apply function 'a_kodiak' to parameter of type '" + t->to_string() + "'");
    this->set_type(ErrorType::getInstance());
  }
}

void AKodiakExpr::toIntermediate(IntermediateGen *intGen)
{
  _parameter->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen("param",_parameter->getTemp(),""," ",
                    "   // Parametro 1");      
  intGen->gen("call","aKodiak ","1",temp,
                      "   // Llamada a Funcion, linea " + std::to_string(get_first_line()));  
  setTemp(temp);
}

APolarExpr::APolarExpr(Expression* parameter)
  : _parameter ( parameter     )
  {}

std::string APolarExpr::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Función predeterminada a_panda\n" + padding + "Parametro:\n" + _parameter->to_string(nesting+1) + "\n";
}

void APolarExpr::check()
{
  _parameter->check();
  Type* t = _parameter->get_type();

  if ( t == KodiakType::getInstance() or t == MalayoType::getInstance() or t == PolarType::getInstance() ) {
    this->set_type(PolarType::getInstance());
  } else if ( t == ErrorType::getInstance() ) {
    this->set_type(ErrorType::getInstance());
  } else {
    error("Cannot apply function 'a_panda' to parameter of type '" + t->to_string() + "'");
    this->set_type(ErrorType::getInstance());
  }
}

void APolarExpr::toIntermediate(IntermediateGen *intGen)
{
  _parameter->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen("param",_parameter->getTemp(),""," ",
                    "   // Parametro 1");      
  intGen->gen("call","aPolar ","1",temp,
                      "   // Llamada a Funcion, linea " + std::to_string(get_first_line()));   
  setTemp(temp);
}

AMalayoExpr::AMalayoExpr(Expression* parameter)
  : _parameter ( parameter     )
  {}

std::string AMalayoExpr::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Función predeterminada a_malayo\n" + padding + "Parametro:\n" + _parameter->to_string(nesting+1) + "\n";
}

void AMalayoExpr::check()
{
  _parameter->check();
  Type* t = _parameter->get_type();

  if ( t == MalayoType::getInstance() or t == PolarType::getInstance() ) {
    this->set_type(MalayoType::getInstance());
  } else if ( t == ErrorType::getInstance() ) {
    this->set_type(ErrorType::getInstance());
  } else {
    error("Cannot apply function 'a_malayo' to parameter of type '" + t->to_string() + "'");
    this->set_type(ErrorType::getInstance());
  }
}

void AMalayoExpr::toIntermediate(IntermediateGen *intGen)
{
  _parameter->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen("param",_parameter->getTemp(),""," ",
                    "   // Parametro 1");      
  intGen->gen("call","aMalayo ","1",temp,
                      "   // Llamada a Funcion, linea " + std::to_string(get_first_line()));  
  setTemp(temp);
}

APandaExpr::APandaExpr(Expression* parameter)
  : _parameter ( parameter     )
  {}

std::string APandaExpr::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Función predeterminada a_panda\n" + padding + "Parametro:\n" + _parameter->to_string(nesting+1) + "\n";
}

void APandaExpr::check()
{
  _parameter->check();
  Type* t = _parameter->get_type();

  if ( t == PandaType::getInstance() or t == PolarType::getInstance() ) {
    this->set_type(PandaType::getInstance());
  } else if ( t == ErrorType::getInstance() ) {
    this->set_type(ErrorType::getInstance());
  } else {
    error("Cannot apply function 'a_panda' to parameter of type '" + t->to_string() + "'");
    this->set_type(ErrorType::getInstance());
  }
}

void APandaExpr::toIntermediate(IntermediateGen *intGen)
{
  _parameter->toIntermediate(intGen);
  std::string temp = intGen->nextTemp();
  intGen->gen("param",_parameter->getTemp(),""," ",
                    "   // Parametro 1");      
  intGen->gen("call","aPanda ","1",temp,
                      "   // Llamada a Funcion, linea " + std::to_string(get_first_line()));  
  setTemp(temp);
}

LonExpr::LonExpr(Expression* parameter)
  : _parameter ( parameter     )
  {}

std::string LonExpr::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Función predeterminada lon\n" + padding + "Parametro:\n" + _parameter->to_string(nesting+1) + "\n";
}

void LonExpr::check()
{
  _parameter->check();
  Type* t = _parameter->get_type();

  if ( t->isArray() or t->isHormiguero() ) {
    this->set_type(PolarType::getInstance());
  } else if ( t == ErrorType::getInstance() ) {
    this->set_type(ErrorType::getInstance());
  } else {
    error("Cannot apply function 'lon' to parameter of type '" + t->to_string() + "'");
    this->set_type(ErrorType::getInstance());
  }
}

void LonExpr::toIntermediate(IntermediateGen *intGen)
{
  _parameter->toIntermediate(intGen);
  std::string temp = intGen->nextTemp(); 
  intGen->gen("param",_parameter->getTemp(),""," ",
                    "   // Parametro 1");      
  intGen->gen("call","lon ","1",temp,
                      "   // Llamada a Funcion, linea " + std::to_string(get_first_line()));  
  setTemp(temp);
}

PardoExpr::PardoExpr(LValueExpr* pardo, IDExpr* campo)
  : LValueExpr()
  , _pardo ( pardo )
  , _campo ( campo )
  {}

PardoExpr::PardoExpr(LValueExpr* pardo, CuevaExpr* campo)
  : LValueExpr()
  , _pardo ( pardo )
  , _campo ( campo )
  {}

PardoExpr::PardoExpr(LValueExpr* pardo, IDExpr* campo, Contenido* c)
  : LValueExpr()
  , _pardo    ( pardo )
  , _campo    ( campo )
  , _tableRow ( c )
  {}

PardoExpr::PardoExpr(LValueExpr* pardo, CuevaExpr* campo, Contenido* c)
  : LValueExpr()
  , _pardo    ( pardo )
  , _campo    ( campo )
  , _tableRow ( c )
  {}

std::string PardoExpr::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Acceso a un pardo\n" + padding + "Pardo:\n" + _pardo->to_string(nesting+1) + "\n" + padding + "Campo:\n" + _campo->to_string(nesting+1) + "\n";
}

void PardoExpr::check()
{
  _pardo->check();
  if (!dynamic_cast<PardoType*>(_pardo->get_type())) {
    error("Trying to access a field in something that is of type '"+_pardo->get_type()->to_string()+"' instead of 'pardo'.");
    this->set_type(ErrorType::getInstance());
    return;
  }
  _campo->check();
  Type* tipo = _campo->get_type();
  this->set_type(tipo);
}

void PardoExpr::toIntermediate(IntermediateGen* intGen)
{
  if (_pardo  )  _pardo->toIntermediate(intGen);

  std::string temp = intGen->nextTemp();
  intGen->gen("+", std::to_string(_tableRow->getOffset()), _pardo->getTemp(), temp);
  setTemp(temp);
}

GrizzliExpr::GrizzliExpr(LValueExpr* grizzli, IDExpr* campo)
  : LValueExpr()
  , _grizzli ( grizzli )
  , _campo   ( campo   )
  {}

GrizzliExpr::GrizzliExpr(LValueExpr* grizzli, CuevaExpr* campo)
  : LValueExpr()
  , _grizzli ( grizzli )
  , _campo   ( campo   )
  {}

GrizzliExpr::GrizzliExpr(LValueExpr* grizzli, IDExpr* campo, Contenido* c)
  : LValueExpr()
  , _grizzli    ( grizzli )
  , _campo      ( campo   )
  , _tableRow   ( c       )
  {}

GrizzliExpr::GrizzliExpr(LValueExpr* grizzli, CuevaExpr* campo, Contenido* c)
  : LValueExpr()
  , _grizzli    ( grizzli )
  , _campo      ( campo   )
  , _tableRow   ( c       )
  {}

std::string GrizzliExpr::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Acceso a un grizzli\n" + padding + "Grizzli:\n" + _grizzli->to_string(nesting+1) + "\n" + padding + "Campo:\n" + _campo->to_string(nesting+1) + "\n";
}

void GrizzliExpr::check()
{
  _grizzli->check();
  if (!dynamic_cast<GrizzliType*>(_grizzli->get_type())) {
    error("Trying to access a field in something that is of type '"+_grizzli->get_type()->to_string()+"' instead of 'grizzli'.");
    this->set_type(ErrorType::getInstance());
    return;
  }
  _campo->check();
  Type* tipo = _campo->get_type();
  this->set_type(tipo);
}

void GrizzliExpr::toIntermediate(IntermediateGen *intGen)
{
  _grizzli->toIntermediate(intGen);
  setTemp(_grizzli->getTemp());
}

CuevaExpr::CuevaExpr(std::string cueva, std::vector<Expression*>* dimensions)
  : LValueExpr()
  , _cueva      ( cueva      )
  , _dimensions ( dimensions )
  {}

CuevaExpr::CuevaExpr(std::string cueva, std::vector<Expression*>* dimensions, Contenido *tableRow)
  : LValueExpr()
  , _cueva       ( cueva       )
  , _dimensions  ( dimensions  )
  { _tableRow = tableRow; 
    _cuevaType = dynamic_cast<CuevaType *> (tableRow->getTipo());
  }

std::string CuevaExpr::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  std::string str = padding + "Acceso a cueva\n" + padding + "Nombre:\n" + padding + padding + _cueva + "\n";
  for (unsigned int i=0; i < _dimensions->size(); ++i) {
    str += padding + "[\n" + _dimensions->at(i)->to_string(nesting+1) + "\n" + padding + "]\n";
  }
  return str;
}

void CuevaExpr::addDimension(Expression* dimension)
{
  _dimensions->push_back(dimension);
}

void CuevaExpr::check()
{
  Type* t = this->get_type();
  for (unsigned int i=0; i<_dimensions->size(); ++i) {
    if (dynamic_cast<CuevaType*>(t)) {
      t = dynamic_cast<CuevaType*>(t)->getTipo();
    } else {
      error("Trying to access a field on the 'cueva' that does not exist");
      this->set_type(ErrorType::getInstance());
      break;
    }
  }
  this->set_type(t);
}

void CuevaExpr::toIntermediate(IntermediateGen *intGen)
{
  std::vector<string> temps;
  Type *t = _cuevaType->getTipo();
  CuevaType* ct;
  for (unsigned int i=0; i < _dimensions->size(); ++i)
  {
  	_dimensions->at(i)->toIntermediate(intGen);
    std::string temp = intGen->nextTemp();

  	intGen->gen("*",_dimensions->at(i)->getTemp(), std::to_string(t->getSize()), temp,
                 "   // Desplazamiento Arreglo, linea " + std::to_string(get_first_line()));  
  	temps.push_back(temp);
    if(t->isArray()) { ct = dynamic_cast<CuevaType* > (t); t = ct->getTipo(); }
  }

  std::string temp = intGen->nextTemp();
  std::string aux = temps[0]; 

  for (unsigned int i=0; i < temps.size()-1; ++i)
  {
  	intGen->gen("+",aux,  temps[i+1] ,temp,
                            "   // Suma de Desplazamientos");  
  	aux  = temp;  
	temp = intGen->nextTemp();

  }

  intGen->gen("[]",_cueva, aux, temp,
                        "   // Acceso al Arreglo, linea " + std::to_string(get_first_line()));   
  setTemp(temp); 

}

void EmptyExpr::check() { this->set_type(ErrorType::getInstance()); }
	
#endif
