#ifndef STATEMENT_CC
#define STATEMENT_CC
#include "statement.hh"
#include <iostream>
#include "type.hh"

extern TablaSimbolos* tablaSimbolos;

using namespace std;

bool Statement::checkFunction = false;
bool Statement::checkIter     = false;

Assign::Assign(std::vector<Expression *>* ids, std::vector<Expression*>* expr)
  : Statement()
  , _ids  ( ids  )
  , _expr ( expr )
  {}

std::string Assign::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  std::string str = padding + "Asignación\n";
  str += padding + "L-values:\n";
  for (unsigned int i=0; i < _ids->size(); ++i) {
    str += _ids->at(i)->to_string(nesting+1) + "\n";
  }
  str += padding + "Expresiones:\n";
  for (unsigned int i=0; i < _ids->size(); ++i) {
    str += _expr->at(i)->to_string(nesting+1) + "\n";
  }
  return str;
}

void Assign::check()
{
  Expression * auxid;
  Expression * auxex;
  Type * tauxid;
  Type * tauxex;
  bool ok = true;

  for(unsigned int i=0; i < _ids->size(); ++i)
  {
    auxid = _ids ->at(i);
    auxex = _expr->at(i);

    auxid->check();
    auxex->check();

    tauxid = auxid->get_type();
    tauxex = auxex->get_type();

    if(tauxid != ErrorType::getInstance() and tauxex != ErrorType::getInstance() and !tauxid->compareTypes(tauxex))
    {
       error("types in asignment " + auxid->to_string(0) + " = " + auxex->to_string(0) + " don't match");
       ok = false;
    }
  }

  if (ok) set_type(ExtintoType::getInstance());
  else    set_type(ErrorType::getInstance());
}

Function::Function(std::string name, std::vector<Type*>* parameterTypes, std::vector<Expression*>* parameters, Type* returnType)
  : Statement()
  , _name           ( name           )
  , _parameterTypes ( parameterTypes )
  , _parameters     ( parameters     )
  , _return         ( returnType     )
  {}

std::string Function::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  std::string str = padding + "Función\n" + padding + "Nombre:\n" + padding + padding + _name + "\n" + padding + "Parametros:\n";
  for (unsigned int i=0; i < _parameters->size(); ++i)
    str += _parameters->at(i)->to_string(nesting+1) + "\n";
  return str;
}

void Function::check()
{
  bool ok = true;
  for(unsigned int i=0; i < _parameters->size(); ++i) {
    _parameters->at(i)->check();
    Type* tipo = _parameterTypes->at(i);
    Type* tipoParam = _parameters->at(i)->get_type();
    if (!tipo->compareStructure(tipoParam)) {
      if (tipoParam != ErrorType::getInstance())
        ok = false;
        error("Trying to pass a parameter of type '" + tipoParam->to_string() + "' to function '" + _name + "' instead of '" + tipo->to_string() + "'");
    }
  }
  if (ok) {
    this->set_type(_return);
  } else {
    this->set_type(ErrorType::getInstance());
  }
}

If::If(Expression* condicion, Statement* instrucciones)
  : Statement()
  , _condicion     ( condicion     )
  , _instrucciones ( instrucciones )
  {}

std::string If::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "If\n" + padding + "Condición:\n" + _condicion->to_string(nesting+1) + "\n" + padding + "Instrucciones:\n" + _instrucciones->to_string(nesting+1) + "\n";
}

void If::check()
{
  _condicion->check();
  Type* t = _condicion->get_type();

  if(t != PandaType::getInstance() and t != ErrorType::getInstance())
  {
    error("Condition for 'si' must be a 'panda' type, instead of '" + t->to_string() + "'");
  }

  _instrucciones->check();

  if(_instrucciones->get_type() != ErrorType::getInstance() and t == PandaType::getInstance())
    set_type(ExtintoType::getInstance());
  else
    set_type(ErrorType::getInstance());

}

bool If::checkReturn(Type* type) { return _instrucciones->checkReturn( type ); }

IfElse::IfElse(Expression* condicion, Statement* brazoTrue, Statement* brazoFalse)
  : Statement()
  , _condicion  ( condicion  )
  , _brazoTrue  ( brazoTrue  )
  , _brazoFalse ( brazoFalse )
  {}

std::string IfElse::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "IfElse\n" + padding + "Condición:\n" + _condicion->to_string(nesting+1) + "\n" + padding + "Instrucciones True:\n" + _brazoTrue->to_string(nesting+1) + "\n" + padding + "Instrucciones False:\n" + _brazoFalse->to_string(nesting+1) + "\n";
}

void IfElse::check()
{
  _condicion->check();
  Type* t = _condicion->get_type();

  if (t != PandaType::getInstance() and t != ErrorType::getInstance()) {
    error("Condition for 'si' must be a 'panda' type, instead of '" + t->to_string() + "'");
  }

  _brazoTrue->check();
  _brazoFalse->check();

  if (_brazoTrue->get_type() == ErrorType::getInstance() or _brazoFalse->get_type() == ErrorType::getInstance() or t != PandaType::getInstance()) {
    this->set_type(ErrorType::getInstance());
  } else {
    this->set_type(ExtintoType::getInstance());
  }

}

bool IfElse::checkReturn(Type* type)
{
  bool bt = _brazoTrue->checkReturn( type );
  bool bf = _brazoFalse->checkReturn( type );
  return  bt and bf;
}

Write::Write(Expression* expr)
  : Statement()
  , _expr( expr )
  {}

std::string Write::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Escribir:\n" + padding + "Expresión:\n" + _expr->to_string(nesting+1) + "\n";
}

void Write::check()
{
  _expr->check();
  Type* t = _expr->get_type();
  if( !dynamic_cast<HormigueroType*>(t)  and
      t != PandaType::getInstance()      and
      t != PolarType::getInstance()      and
      t != KodiakType::getInstance()     and
      t != MalayoType::getInstance()     and
      t != ErrorType::getInstance()
    )
  {
    error("error in function 'escribir' expression is of type '" + t->to_string() + "' instead of an escalar type");
    set_type(ErrorType::getInstance());
  } else
  {
    if(t == ErrorType::getInstance()) set_type(ErrorType::getInstance());
    else                              set_type(ExtintoType::getInstance());
  }
}

Read::Read(Expression* id)
  : Statement()
  , _id( id )
  {}

std::string Read::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Leer:\n" + padding + "Expresión:\n" + _id->to_string(nesting + 1) + '\n';
}

void Read::check()
{
  _id->check();

  Type* t = _id->get_type();

  if (dynamic_cast<HormigueroType*>(t) or
      dynamic_cast<CuevaType*>(t) or
      dynamic_cast<PardoType*>(t) or
      dynamic_cast<GrizzliType*>(t))
  {
    error("Error in function 'leer' cannot read a variable of type '" + t->to_string() + "'");
    this->set_type(ErrorType::getInstance());
    return;
  }

  this->set_type(ExtintoType::getInstance());
}

Body::Body( std::vector<Statement *>* listSta )
  : _listSta( listSta )
  {}

std::string Body::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  std::string str = padding + "Bloque:\n";
  for (unsigned int i=0; i < _listSta->size(); ++i) {
    str += _listSta->at(i)->to_string(nesting+1) + "\n";
  }
  return str;
}

void Body::check()
{
  bool ok = true;
  Statement* aux = nullptr;

  if (_listSta)
    for(std::vector<Statement*>::iterator it = _listSta->begin(); it != _listSta->end(); it++) {
      aux = *it;
      aux->check();

      if(aux->get_type() == ErrorType::getInstance()) ok = false;

      if(aux->isReturn()) setReturn();
    }

  if (ok) {
    this->set_type(ExtintoType::getInstance());
  } else {
    this->set_type(ErrorType::getInstance());
  }
}

bool Body::checkReturn(Type* type)
{
   bool ok = true;
   if(_listSta->size() != 0)
   {
     for(std::vector<Statement*>::iterator it = _listSta->begin(); it != _listSta->end(); ++it)
     {
       ok = ok and (*it)->checkReturn(type);
     }
   }
   return ok;
}

ComplexFor::ComplexFor(std::string id, Expression* begin, Expression* end, Expression* step, Statement* body)
  : Statement()
  , _id( id )
  , _begin( begin )
  , _end( end )
  , _step( step )
  , _body( body )
  {}

std::string ComplexFor::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return  padding + std::string("Iteracion Acotada con salto :\n")
        + padding + "Variable de iteracion:\n" + padding + padding + _id + "\n"
        + padding + "Expresion de Inicio:\n"   + _begin->to_string(nesting+1)
        + padding + "Expresion Final:\n"       + _end->to_string(nesting+1)
        + padding + "Paso:\n"                  + _step->to_string(nesting+1)
        + padding + "Cuerpo:\n"                + _body->to_string(nesting+1)
        ;
}

void ComplexFor::check()
{
  _begin->check();
  Type* tbegin = _begin->get_type();

  if (tbegin != PolarType::getInstance() and tbegin != ErrorType::getInstance()) {
    error("Lower bound for 'para' must be of type 'polar' instead of '" + tbegin->to_string() + "'");
    tbegin = ErrorType::getInstance();
  }

  _end->check();
  Type* tend = _end->get_type();

  if (tend != PolarType::getInstance() and tend != ErrorType::getInstance()) {
    error("Higher bound for 'para' must be of type 'polar' instead of '" + tend->to_string() + "'");
    tend = ErrorType::getInstance();
  }

  _step->check();
  Type* tstep = _step->get_type();

  if (tstep != PolarType::getInstance() and tstep != ErrorType::getInstance()) {
    error("step for 'para' must be of type 'polar' instead of '" + tstep->to_string() + "'");
    tstep = ErrorType::getInstance();
  }

  Statement::checkIter = true; _body->check(); Statement::checkIter = false;
  Type* tbody = _body->get_type();

  if (tbegin == ErrorType::getInstance() or
      tend == ErrorType::getInstance() or
      tstep == ErrorType::getInstance() or
      tbody == ErrorType::getInstance()) {
    this->set_type(ErrorType::getInstance());
  } else {
    this->set_type(ExtintoType::getInstance());
  }
}

bool ComplexFor::checkReturn(Type* type) { return _body->checkReturn(type); }

SimpleFor::SimpleFor(std::string id, Expression* begin, Expression* end, Statement* body)
  : Statement()
  , _id( id )
  , _begin( begin )
  , _end( end )
  , _body( body )
  {}

std::string SimpleFor::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return  padding + std::string("Iteracion Acotada simple:\n")
        + padding + "Variable de iteracion:\n"           + padding + _id + "\n"
        + padding + "Expresion de Inicio:\n"             + _begin->to_string(nesting+1) + "\n"
        + padding + "Expresion Final:\n"                 + _end->to_string(nesting+1) + "\n"
        + padding + "Cuerpo:\n"                          + _body->to_string(nesting+1) + "\n"
        ;
}

void SimpleFor::check()
{
  _begin->check();
  _end->check();

  Type* errortype = ErrorType::getInstance();
  Type* tb = _begin->get_type();
  Type* te = _end->get_type();

  if( tb != PolarType::getInstance() && tb != errortype)
  {
    error("initial expression inside 'para' is of type '" + tb->to_string() + "' instead of type 'polar'");
    tb = ErrorType::getInstance();
  }

  if( te != PolarType::getInstance() && te != errortype)
  {
    error("ending expression inside 'para' is of type '" + tb->to_string() + "' instead of type 'polar'");
    te = ErrorType::getInstance();
  }

  Statement::checkIter = true; _body->check(); Statement::checkIter = false;

  Type* tbody = _body->get_type();

  if(tb == errortype || te == errortype || tbody == errortype)
    set_type(errortype);
  else
    set_type(ExtintoType::getInstance());
}

bool SimpleFor::checkReturn(Type* type) { return _body->checkReturn(type); }

IdFor::IdFor(std::string id, std::string iterVar, Statement* body)
  : Statement()
  , _id( id )
  , _iterVar( iterVar )
  , _body( body )
  {}

std::string IdFor::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return  padding + std::string("Iteracion Acotada simple:\n")
        + padding + "Variable de iteracion:\n"           + padding + _id + "\n"
        + padding + "Cueva de Iteracion:\n"              + padding + _iterVar + "\n"
        + padding + "Cuerpo:\n"                          + _body->to_string(nesting+1) + "\n"
        ;
}

void IdFor::check()
{
  Statement::checkIter = true; _body->check(); Statement::checkIter = false;
  Type* t = _body->get_type();

  if (t == ErrorType::getInstance()) {
    this->set_type(ErrorType::getInstance());
  } else {
    this->set_type(ExtintoType::getInstance());
  }
}

bool IdFor::checkReturn(Type* type) { return _body->checkReturn(type); }

Return::Return()
  : Statement()
  {}

std::string Return::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Vomita sin etiqueta";
}

void Return::check()
{
  set_type(ExtintoType::getInstance());
}

bool Return::checkReturn(Type* type)
{
  if(!type->compareTypes(get_type()))
  {
    error("type '" + get_type()->to_string() + "' of return statement does not match the function return type '" + type->to_string() + "'");
    return false;
  } else return true;
}

ReturnExpr::ReturnExpr(Expression* expr)
  : Statement()
  , _expr( expr )
  {}

std::string ReturnExpr::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Vomita\n" + padding + "Expresión:\n" + _expr->to_string(nesting+1) + "\n";
}

void ReturnExpr::check()
{
  _expr->check();
  Type* t = _expr->get_type();

  if (t == ErrorType::getInstance()) {
    this->set_type(ErrorType::getInstance());
  } else {
    this->set_type(t);
  }
}

bool ReturnExpr::checkReturn(Type* type)
{
  if(!type->compareTypes(get_type()))
    {
    error("type '" + get_type()->to_string() + "' of return statement does not match the function return type '" + type->to_string() + "'");
    return false;
  } else return true;
}

Increase::Increase(std::string id)
  : Statement()
  , _id( id )
  {}

std::string Increase::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Incremento:\n" + padding + "Variable:\n" + padding + _id;
}

void Increase::check()
{
  Type* type = get_type();
  this->set_type(ExtintoType::getInstance());
  if(type != PolarType::getInstance())
  {
    error("Attempt to increase variable '"+ _id +"' of type '" + type->to_string() + "' instead of type 'polar'");
    set_type(ErrorType::getInstance());
  }
}

Decrement::Decrement(std::string id)
  : Statement()
  , _id( id )
  {}

std::string Decrement::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Decremento:\n" + padding + "Variable:\n" + padding + _id;
}

void Decrement::check()
{
  Type* t = this->get_type();

  this->set_type(ExtintoType::getInstance());

  if ( t != PolarType::getInstance()) {
    error("Attempt to decrement variable '"+ _id + "' of type '" + t->to_string() + "' instead of type 'polar'");
    this->set_type(ErrorType::getInstance());
  }
}

Continue::Continue()
  : Statement()
  {}

std::string Continue::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "fondoBlanco sin etiqueta";
}

void Continue::check()
{
  if(Statement::checkIter)
    this->set_type(ExtintoType::getInstance());
  else
  {
    this->set_type(ErrorType::getInstance());
    error("'fondoBlanco' statement outside an iteration");
  }
}

ContinueID::ContinueID(std::string id)
  : Statement()
  , _id( id )
  {}

std::string ContinueID::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "fondoBlanco:\n" + padding + "Etiqueta:\n" + padding + _id;
}

void ContinueID::check()
{
  if(Statement::checkIter)
    this->set_type(ExtintoType::getInstance());
  else
  {
    this->set_type(ErrorType::getInstance());
    error("'fondoBlanco' statement outside an itaration");
  }
}

Break::Break()
  : Statement()
  {}

std::string Break::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "roloePea sin etiqueta";
}

void Break::check()
{
  if(Statement::checkIter)
    this->set_type(ExtintoType::getInstance());
  else
  {
    this->set_type(ErrorType::getInstance());
    error("'roloePea' statement outside an iteration");
  }
}

BreakID::BreakID(std::string id)
  : Statement()
  , _id( id )
  {}

std::string BreakID::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "roloePea\n" + padding + "Etiqueta:\n" + padding + _id;
}

void BreakID::check()
{
  if(Statement::checkIter)
    this->set_type(ExtintoType::getInstance());
  else
  {
    this->set_type(ErrorType::getInstance());
    error("'roloePea' statement outside an iteration");
  }
}

While::While(Expression* expr, Statement* body)
  : _expr( expr )
  , _body( body )
  {}

std::string While::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return  padding + "Iteracion Indeterminada\n" + padding + "Condicion:\n"
        + _expr->to_string(nesting+1) + "\n"
        + padding + "Instrucción:\n"
        + _body->to_string(nesting+1) + "\n"
        ;
}

void While::check()
{
  _expr->check();
  Type* texp = _expr->get_type();

  if (texp != PandaType::getInstance() and texp != ErrorType::getInstance()) {
    error("Condition for 'mientras' must be a 'panda' type instead of '" + texp->to_string() + "'");
    texp = ErrorType::getInstance();
  }

  Statement::checkIter = true; _body->check(); Statement::checkIter = false;

  Type* tbody = _body->get_type();

  if (texp == ErrorType::getInstance() or tbody == ErrorType::getInstance()) {
    this->set_type(ErrorType::getInstance());
  } else {
    this->set_type(ExtintoType::getInstance());
  }
}

bool While::checkReturn(Type* type) { return _body->checkReturn(type); }

TagWhile::TagWhile(std::string id, Expression* expr, Statement* body)
  : _id   ( id )
  , _expr ( expr )
  , _body ( body )
  {}

std::string TagWhile::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return  padding + std::string("Iteracion Indeterminada con Etiqueta\n")
        + padding + "Etiqueta\n:"  + padding + _id + "\n"
        + padding + "Condición:\n" + _expr->to_string(nesting+1) + "\n"
        + padding + "Cuerpo:\n"    + _body->to_string(nesting+1) + "\n"
        ;
}

void TagWhile::check()
{
  _expr->check();
  Type* texp = _expr->get_type();

  if (texp != PandaType::getInstance() and texp != ErrorType::getInstance()) {
    error("Condition for 'mientras' must be a 'panda' type instead of '" + texp->to_string() + "'");
    texp = ErrorType::getInstance();
  }

  Statement::checkIter = true; _body->check(); Statement::checkIter = false;

  Type* tbody = _body->get_type();

  if (texp == ErrorType::getInstance() or tbody == ErrorType::getInstance()) {
    this->set_type(ErrorType::getInstance());
  } else {
    this->set_type(ExtintoType::getInstance());
  }
}

bool TagWhile::checkReturn(Type* type) {  return _body->checkReturn(type); }

Empty::Empty()
  : Statement()
  {}

Empty::Empty(Type* type)
  : Statement( ErrorType::getInstance() )
  {}

std::string Empty::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Instrucción vacia";
}

void Empty::check()
{
  this->set_type(ExtintoType::getInstance());
}

#endif
