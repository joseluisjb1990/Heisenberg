#ifndef STATEMENT_CC
#define STATEMENT_CC
#include "statement.hh"
#include <iostream>
#include "type.hh"

#define SEPSIZE 100
#define SEP     '-'

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

void Assign::toIntermediate(IntermediateGen *intGen)
{
  for (unsigned int i=0; i < _ids->size(); ++i) {
    Expression* id   = _ids->at(i);
    Expression* expr = _expr->at(i);

    id->toIntermediate(intGen);
    expr->toIntermediate(intGen);

    Quad* q;
    if(id->isArray())
    {
      std::string temp = intGen->nextTemp();
      q = new DespQuad(id->getArrayName(), id->getTemp(), temp);
      intGen->gen(q);
      // intGen->gen("[]", id->getTemp(), id->getArrayName(), temp);
      id->setTemp(temp);
    }

    if(expr->isArray())
    {
      // intGen->gen("[]=", expr->getArrayName(), expr->getTemp(), id->getTemp(),"   // Asignacion indirecta, linea " + std::to_string(get_first_line()));
      q = new DespEqualQuad(expr->getArrayName(), expr->getTemp(), id->getTemp());
      intGen->gen(q);
    }
    else
    {
      q = new AssignQuad(expr->getTemp(), id->getTemp());
      intGen->gen(q);
      // intGen->gen(":=", expr->getTemp() , " " , id->getTemp(), "   // Asignacion directa, linea " + std::to_string(get_first_line()));
    }
  }
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

Function::Function( std::string name
                  , std::vector<Type*>* parameterTypes
                  , std::vector<Expression*>* parameters
                  , Type* returnType
                  , std::vector<bool>* parametros
                  )
  : Statement()
  , _name           ( name           )
  , _parameterTypes ( parameterTypes )
  , _parameters     ( parameters     )
  , _return         ( returnType     )
  , _defParametros  ( parametros     )
  {}  

std::string Function::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  std::string str = padding + "Función\n" + padding + "Nombre:\n" + padding + padding + _name + "\n" + padding + "Parametros:\n";
  if(_parameters)
   for (unsigned int i=0; i < _parameters->size(); ++i)
     str += _parameters->at(i)->to_string(nesting+1) + "\n";
   return str;
}

void Function::check()
{
  bool ok = true;
  if(_parameters)
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

void Function::toIntermediate(IntermediateGen *intGen)
{

  if(_parameters)
    for (unsigned int i=0; i < _parameters->size(); ++i)
      _parameters->at(i)->toIntermediate(intGen);

  Quad* q;
  std::string t;
  Expression* p;
  
  if(_parameters)
  { 
    for (int i=(_parameters->size()-1); -1 < i; --i)
      if(_defParametros->at(i))
      {
        p = _parameters->at(i);
        t = intGen->nextTemp();
        q = new RefQuad(p->getTemp(), t);
        intGen->gen(q);
        p->setTemp(t);
      }
    for (int i=(_parameters->size()-1); -1 < i; --i)
    {
      p = _parameters->at(i);
      q = new ParamQuad(p->getTemp());
      intGen->gen(q);
    } 
  }

  t = intGen->nextTemp();
  if(_parameters)
  {
    q = new CallQuad(_name, std::to_string(_parameters->size()), t);
    intGen->gen(q);
  }
  else
  {
    q = new CallQuad(_name, "0", t);
    intGen->gen(q);
  } 
  setTemp(t);
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

void If::toIntermediate(IntermediateGen *intGen)
{
  _condicion->toIntermediateGoto(intGen);
  _condicion->backpatch(true, intGen->getQuad(), intGen);
  _instrucciones->toIntermediate(intGen);
}

void If::nextInst(int nextInst, IntermediateGen *intGen)
{
  _condicion->backpatch(false, nextInst, intGen);
  _instrucciones->nextInst(nextInst, intGen);
}

void If::nextInstContinue(int nextInst, IntermediateGen *intGen)
{
  _instrucciones->nextInstContinue(nextInst, intGen);
}

void If::nextInstBreak(int nextInst, IntermediateGen *intGen)
{
  _instrucciones->nextInstBreak(nextInst, intGen);
}

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

void IfElse::toIntermediate(IntermediateGen *intGen)
{
  _condicion->toIntermediateGoto(intGen);
  _condicion->backpatch(true, intGen->getQuad(), intGen);
  _brazoTrue->toIntermediate(intGen);
  Quad* q = new GotoQuad();
  _nextInst = intGen->gen(q);
  _condicion->backpatch(false, intGen->getQuad(), intGen);
  _brazoFalse->toIntermediate(intGen);
}

void IfElse::nextInst(int nextInst, IntermediateGen *intGen)
{
  intGen->patch(_nextInst, nextInst);
  _brazoTrue->nextInst(nextInst, intGen);
  _brazoFalse->nextInst(nextInst, intGen);
}

void IfElse::nextInstContinue(int nextInst, IntermediateGen *intGen)
{
  _brazoTrue->nextInstContinue(nextInst, intGen);
  _brazoFalse->nextInstContinue(nextInst, intGen);
}

void IfElse::nextInstBreak(int nextInst, IntermediateGen *intGen)
{
  _brazoTrue  ->nextInstBreak(nextInst, intGen);
  _brazoFalse ->nextInstBreak(nextInst, intGen);
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

void Write::toIntermediate(IntermediateGen *intGen)
{
  _expr->toIntermediate(intGen);
  Quad* q = new ParamQuad(_expr->getTemp());
  intGen->gen(q);
  // intGen->gen("param", _expr->getTemp(), "", "");
  std::string temp = intGen->nextTemp();
  q = new CallQuad("escribir", "1", temp);
  intGen->gen(q);
  // intGen->gen("call", "escribir", "1", temp, "   // Escritura por salida estandar, linea " + std::to_string(get_first_line()));
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

void Read::toIntermediate(IntermediateGen *intGen)
{
  _id->toIntermediate(intGen);
  Quad* q = new ParamQuad(_id->getTemp());
  intGen->gen(q);
  // intGen->gen("param", _id->getTemp(), "", "");
  std::string temp = intGen->nextTemp();
  q = new CallQuad("leer", "1", temp);
  intGen->gen(q);
  // intGen->gen("call", "leer", "1", temp, "   // Lectura por entrada estandar, linea " + std::to_string(get_first_line()));
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

void Body::toIntermediate(IntermediateGen *intGen)
{
  for(std::vector<Statement*>::iterator it = _listSta->begin(); it != _listSta->end(); it++) {
    (*it)->toIntermediate(intGen);
    (*it)->nextInst(intGen->getQuad(), intGen);
  }
}

void Body::toIntermediateTag(IntermediateGen *intGen, std::string tag, int pos)
{
  for(std::vector<Statement*>::iterator it = _listSta->begin(); it != _listSta->end(); it++) {
    (*it)->toIntermediateTag(intGen, tag, pos);
  }
}

void Body::toIntermediateTagBreak(IntermediateGen *intGen, std::string tag, int pos)
{
  for(std::vector<Statement*>::iterator it = _listSta->begin(); it != _listSta->end(); it++) {
    (*it)->toIntermediateTagBreak(intGen, tag, pos);
  }
}

void Body::nextInstContinue(int nextInt, IntermediateGen *intGen)
{
  for(std::vector<Statement*>::iterator it = _listSta->begin(); it != _listSta->end(); it++) {
    (*it)->nextInstContinue(nextInt, intGen);
  }
}

void Body::nextInstBreak(int nextInt, IntermediateGen *intGen)
{
  for(std::vector<Statement*>::iterator it = _listSta->begin(); it != _listSta->end(); it++) {
    (*it)->nextInstBreak(nextInt, intGen);
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

void ComplexFor::toIntermediate(IntermediateGen *intGen)
{
  intGen->genComment("// Codigo de la iteracion acotada, linea " + std::to_string(get_first_line()));
  _begin->toIntermediate(intGen);
  _end->toIntermediate(intGen);

  _step->toIntermediate(intGen);
  Quad* q;

  q = new AssignQuad(_begin->getTemp(),_id);
  intGen->gen(q);
  //intGen->gen(":=", _begin->getTemp(), " ",  _id, "   // Variable de Iteracion ");

  unsigned int pos = intGen->getQuad();

  q = new GotoEmptyQuad(_id, _end->getTemp());
  _nextInst = intGen->gen(q);
  //_nextInst = intGen->genEmpty("if " + _id + " > " + _end->getTemp() + " goto");

  _body->toIntermediate(intGen);

  q = new SumQuad( _id, _step->getTemp(), _id); 
  intGen->gen(q);     
  //intGen->gen("+", _id, _step->getTemp(), _id, "   // Incremento del Iterador");

  q = new GotoLineQuad(std::to_string(pos));
  intGen->gen(q);
  //intGen->gen("goto", " ", " ", std::to_string(pos), "   // Fin de la Iteracion");
}

void ComplexFor::nextInst(int nextInst, IntermediateGen *intGen)
{
   intGen->patch(_nextInst, nextInst);
  _body->nextInst(nextInst, intGen);
}

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

SimpleFor::SimpleFor(std::string id, Expression* begin, Expression* end, Statement* body)
  : Statement()
  , _id( id )
  , _begin( begin )
  , _end( end )
  , _body( body )
  {}

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

void SimpleFor::toIntermediate(IntermediateGen *intGen)
{
  intGen->genComment("// Codigo intermedio para la iteracion acotada, linea " + std::to_string(get_first_line()));
  _begin->toIntermediate(intGen);
  _end->toIntermediate(intGen);

  Quad* q;

  q = new AssignQuad(_begin->getTemp(),_id);
  intGen->gen(q);
  //intGen->gen(":=", _begin->getTemp(), " ",  _id, "   // Variable de Iteracion ");

  unsigned int pos = intGen->getQuad();
  
  q = new GotoEmptyQuad(_id, _end->getTemp());
  _nextInst = intGen->gen(q);
  //_nextInst = intGen->genEmpty("if " + _id + " > " + _end->getTemp() + " goto");

  _body->toIntermediate(intGen);
  
  q = new SumQuad( _id, "1", _id); 
  intGen->gen(q);
  //intGen->gen("+", _id, "1", _id, "   // Incremento del Iterador");

  q = new GotoLineQuad(std::to_string(pos));
  intGen->gen(q);
  //intGen->gen("goto", " ", " ", std::to_string(pos), "   // Fin de la Iteracion");

}

void SimpleFor::nextInst(int nextInst, IntermediateGen *intGen)
{
   intGen->patch(_nextInst, nextInst);
  _body->nextInst(nextInst, intGen);
}

IdFor::IdFor(std::string id, std::string iterVar, Statement* body)
  : Statement()
  , _id( id )
  , _iterVar( iterVar )
  , _body( body )
  {}

IdFor::IdFor(std::string id, std::string iterVar, Statement* body, Contenido* c)
  : Statement()
  , _id      ( id      )
  , _iterVar ( iterVar )
  , _body    ( body    )
  , _tableRow( c       )
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

void IdFor::toIntermediate(IntermediateGen *intGen)
{
  intGen->genComment("// Codigo intermedio para la iteracion acotada, linea " + std::to_string(get_first_line()));
  CuevaType*  t     = dynamic_cast<CuevaType *> (_tableRow->getTipo());
  std::string tc    = std::to_string(t->getTipo()->getSize());
  std::string temp  = intGen->nextTemp();
  std::string temp2  = intGen->nextTemp();
 
  Quad* q;

  q = new AssignQuad("0",temp);
  intGen->gen(q);
  //intGen->gen(":=", "0", " ",  temp, "   // Variable de Iteracion ");
  
  unsigned int pos = intGen->getQuad();

  q = new GotoEmptyIdForQuad(temp, std::to_string(t->getLongitud() -1));
  _nextInst = intGen->gen(q);
  //_nextInst = intGen->genEmpty("if " + temp + " = " + std::to_string(t->getLongitud() -1) + " goto");

  q = new MulQuad( tc,temp, temp2); 
  intGen->gen(q);
  //intGen->gen("*", tc, temp, temp2, "   // Desplazamiento Arreglo");  

  q = new DespQuad( _iterVar,temp2, _id); 
  intGen->gen(q);
  //intGen->gen("[]", _iterVar, temp2, _id, "   // Acceso al Arreglo, linea ");

  _body->toIntermediate(intGen);

  q = new SumQuad( temp, "1", temp); 
  intGen->gen(q);
  //intGen->gen("+",temp,"1",temp, "   // Incremento del Iterador");

  q = new GotoLineQuad(std::to_string(pos));
  intGen->gen(q);
  //intGen->gen("goto", " ", " ", std::to_string(pos), "   // Fin de la Iteracion");

}

void IdFor::nextInst(int nextInst, IntermediateGen *intGen)
{
   intGen->patch(_nextInst, nextInst);
  _body->nextInst(nextInst, intGen);
}

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

void ReturnExpr::toIntermediate(IntermediateGen *intGen)
{
   _expr->toIntermediate(intGen);

  Quad* q;

  q = new ReturnQuad(_expr->getTemp());
  intGen->gen(q);
  /*intGen->gen("return",_expr->getTemp(), "","", 
  	"   // Retorno de funcion, linea " + std::to_string(get_first_line()));  */
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

void Increase::toIntermediate(IntermediateGen *intGen)
{ 
  Quad* q;

  q = new SumQuad( _id, "1", _id); 
  intGen->gen(q);
  //intGen->gen("+",_id,"1",_id, "   // Incremento, linea " + std::to_string(get_first_line()));  

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

void Decrement::toIntermediate(IntermediateGen *intGen)
{  
  Quad* q;

  q = new SubQuad( _id, "1", _id); 
  intGen->gen(q);
  //intGen->gen("-",_id,"1",_id, "// Decremento, linea " + std::to_string(get_first_line()));  

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

void Continue::toIntermediate(IntermediateGen *intGen)
{
  Quad* q;

  q = new GotoQuad(); 
  _nextInst = intGen->gen(q);

}

void Continue::nextInstContinue(int nextInst, IntermediateGen *intGen)
{
   intGen->patch(_nextInst, nextInst);
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
    error("'fondoBlanco' statement outside an iteration");
  }
}

void ContinueID::toIntermediate(IntermediateGen *intGen)
{
   //intGen->genComment("// Codigo generado por el continue con tag. id = " + _id);

    Quad* q;
    q = new GotoQuad(); 
    _nextInst = intGen->gen(q);
    //_nextInst = intGen->genEmpty("goto");
    //intGen->genSpace();
}

void ContinueID::toIntermediateTag(IntermediateGen *intGen, std::string tag, int pos)
{
  if(tag.compare(_id) == 0)
  {
  	intGen->patch(_nextInst, pos);
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

void Break::toIntermediate(IntermediateGen *intGen)
{
   Quad* q;

   q = new GotoQuad(); 
   _nextInst = intGen->gen(q);
   //_nextInst = intGen->genEmpty("goto");
}

void Break::nextInstBreak(int nextInst, IntermediateGen *intGen)
{
  std::cout << "Estoy generando codigo intermedio para el break " << nextInst << std::endl;
   intGen->patch(_nextInst, nextInst);
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

void BreakID::toIntermediate(IntermediateGen *intGen)
{
    //intGen->genComment("// Codigo generado por el break con tag. id = " + _id);

    Quad* q;
    q = new GotoQuad(); 
    _nextInst = intGen->gen(q);
   //_nextInst = intGen->genEmpty("goto");
   //intGen->genSpace();
}

void BreakID::toIntermediateTagBreak(IntermediateGen *intGen, std::string tag, int pos)
{
  if(tag.compare(_id) == 0)
  {   
  	intGen->patch(_nextInst, pos);
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


void While::toIntermediate(IntermediateGen *intGen)
{
  int pos = intGen->getQuad();
  _expr->toIntermediateGoto(intGen);
  _expr->backpatch(true, intGen->getQuad(), intGen);
  _body->toIntermediate(intGen);
  _body->nextInstContinue(pos, intGen);

  Quad* q;
  q = new GotoLineQuad(std::to_string(pos));
  intGen->gen(q);
  //intGen->gen("goto",std::to_string(pos), "", "","   // Fin de la Iteracion");  

}

void While::nextInst(int nextInst, IntermediateGen *intGen)
{
  _expr->backpatch(false, nextInst, intGen);
  _body->nextInstBreak(nextInst, intGen);
}

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

void TagWhile::toIntermediate(IntermediateGen *intGen)
{
  //intGen->genComment("Iteracion indeterminada con tag = " + _id);
  int pos = intGen->getQuad();
  _expr->toIntermediateGoto(intGen);
  _expr->backpatch(true, intGen->getQuad(), intGen);
  _body->toIntermediate(intGen);
  _body->nextInstContinue(pos, intGen);
  _body->toIntermediateTag(intGen, _id, pos);

  Quad* q;
  q = new GotoLineQuad(std::to_string(pos));
  intGen->gen(q);
  //intGen->gen("goto",std::to_string(pos), "", "", "   // Fin de la Iteracion con tag = " + _id);
}

void TagWhile::nextInst(int nextInst, IntermediateGen *intGen)
{
  _expr->backpatch(false, nextInst, intGen);
  _body->nextInstBreak(nextInst, intGen);
  _body->toIntermediateTagBreak(intGen, _id, nextInst);
}

void While::toIntermediateTag   (IntermediateGen *intGen, std::string tag, int pos) { _body         ->toIntermediateTag(intGen, tag, pos); }
void TagWhile::toIntermediateTag(IntermediateGen *intGen, std::string tag, int pos) { _body         ->toIntermediateTag(intGen, tag, pos); }
void If::toIntermediateTag      (IntermediateGen *intGen, std::string tag, int pos) { _instrucciones->toIntermediateTag(intGen, tag, pos); }
void IfElse::toIntermediateTag  (IntermediateGen *intGen, std::string tag, int pos) { _brazoTrue    ->toIntermediateTag(intGen, tag, pos); 
                                                                                      _brazoFalse   ->toIntermediateTag(intGen, tag, pos);
                                                                                    }

void While::toIntermediateTagBreak   (IntermediateGen *intGen, std::string tag, int pos) { _body          ->toIntermediateTagBreak(intGen, tag, pos); }
void TagWhile::toIntermediateTagBreak(IntermediateGen *intGen, std::string tag, int pos) { _body          ->toIntermediateTagBreak(intGen, tag, pos); }
void If::toIntermediateTagBreak      (IntermediateGen *intGen, std::string tag, int pos) { _instrucciones ->toIntermediateTagBreak(intGen, tag, pos); }
void IfElse::toIntermediateTagBreak  (IntermediateGen *intGen, std::string tag, int pos) { _brazoTrue     ->toIntermediateTagBreak(intGen, tag, pos); 
                                                                                           _brazoFalse    ->toIntermediateTagBreak(intGen, tag, pos);
                                                                                         }
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
