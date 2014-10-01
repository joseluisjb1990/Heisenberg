#ifndef DEFINITION_CC
#define DEFINITION_CC
#include "definition.hh"

using namespace std;

DefWithInit::DefWithInit(Type* tipo, std::vector<std::string>* ids, std::vector<Expression*>* expr)
  : Definition()
  , _tipo ( tipo )
  , _ids  ( ids  )
  , _expr ( expr )
{}

std::string DefWithInit::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  std::string str = padding + "Definición con Inicialización\n" + padding + "Tipo:\n" + padding + padding + _tipo->to_string() + "\n" + padding + "Variables:\n";
  for (unsigned int i=0; i < _ids->size(); ++i) {
    str += padding + "Nombre:\n" + padding + padding + _ids->at(i) + "\n" + padding + "Valor:\n" + _expr->at(i)->to_string(nesting+1) + "\n";
  }
  return str;
}

void DefWithInit::check()
{
  bool errortype = false;
  for(std::vector<Expression*>::iterator it = _expr->begin(); it != _expr->end(); ++it)
  {
    Expression* exp = *it;
    exp->check();
    if(!_tipo->compareTypes(exp->get_type()))
    {
      errortype = true;
      if (exp->get_type() != ErrorType::getInstance()) {
        error("expression " + exp->to_string(0) + " is not of type " + _tipo->to_string());
      }
    }
  }

  if(errortype) set_type(ErrorType::getInstance());
  else          set_type(_tipo);
}

ConstDef::ConstDef(Type* tipo, std::vector<std::string>* ids, std::vector<Expression*>* expr)
  : DefWithInit(tipo, ids, expr)
  {}

DefVar::DefVar(Type* tipo, std::vector<std::string>* ids, std::vector<Expression*>* expr)
  : DefWithInit(tipo, ids, expr)
  {}

DefVarNoInit::DefVarNoInit(Type* tipo, std::vector<std::string>* ids)
  : _tipo( tipo )
  , _ids( ids )
  {}

std::string DefVarNoInit::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  std::string str = padding + "Definición sin inicializar\n" + padding + "Tipo:\n" + padding + padding + _tipo->to_string() + "\n" + padding + "Variables:\n";
  for (unsigned int i=0; i < _ids->size(); ++i) {
    str += padding + "Nombre:\n" + padding + padding + _ids->at(i) + "\n";
  }
  return str;
}

void DefVarNoInit::check() { set_type(_tipo); }

DefType::DefType(std::string nombre, Type* tipo)
  : _nombre ( nombre )
  , _tipo   ( tipo   )
  {}

std::string DefType::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Definición de tipo\n" + padding + "Nombre:\n" + padding + padding + _nombre + "\n" + padding + "Tipo:\n" + padding + padding + _tipo->to_string() + "\n";
}

void DefType::check() { set_type(_tipo); }

DefArray::DefArray(Type* tipo, std::string id)
  : _tipo( tipo )
  , _id( id )
  {}

std::string DefArray::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "Definición de cueva:\n" + padding + "Nombre:\n" + padding + padding + _id + "\n" + padding + "Tipo : " + padding + padding + _tipo->to_string() + "\n";
}

void DefArray::check() { set_type(_tipo); }

DecFunction::DecFunction ( std::string         name
                         , std::vector<Parameter*>* parametros
                         , Type*               tipoRetorno
                         )
  : Definition  ()
  , _name       ( name        )
  , _parametros ( parametros  )
  , _tipoRetorno( tipoRetorno )
  {}

std::string DecFunction::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  std::string str =  padding + "Declaración de función\n" + padding + "Nombre:\n" + padding + "  " + _name + "\n" + padding + "Parametros:\n";
  for (unsigned int i=0; i<_parametros->size(); ++i)
    str += _parametros->at(i)->to_string(nesting+1) + '\n';

  str += padding + "Retorna:\n" + _tipoRetorno->to_string(nesting + 1) + '\n';
  return str;
}

void DecFunction::check()
{
  bool ok = true;
  for(std::vector<Parameter*>::iterator it = _parametros->begin(); it != _parametros->end(); ++it)
  {
    (*it)->check();
    if((*it)->get_type() == ErrorType::getInstance()) ok = false;
  }

  if(ok) set_type(_tipoRetorno);
  else   set_type(ErrorType::getInstance());
}

Parameter::Parameter ( std::string id
                     , Type* tipo
                     , bool ref
                     )
  : Definition()
  , _id       ( id    )
  , _tipo     ( tipo  )
  , _ref      ( ref   )
  {}

std::string Parameter::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  std::string str;
  if (_ref) str = padding + "*"; else str = padding;
  str += _id + " : " + _tipo->to_string();
  return str;
}

std::string Parameter::get_id()   { return _id;      }
Type*       Parameter::get_tipo() { return _tipo;    }
bool        Parameter::get_ref()  { return _ref;     }
void        Parameter::check()    { set_type(_tipo); }

bool        Parameter::compareParameters(Parameter* p2)
{
  if (!_tipo->compareStructure(p2->get_tipo()) or !(_id == p2->get_id()))
  {
    error("parameter " + to_string(0) + " in function definition don't match parameter " + p2->to_string(0) + " in function declaration");
    return false;
  }
  return true;
}

bool Parameter::compareTypes(Type* tipo)
{
  if(tipo->isArray() and _tipo->isArray())
  {
    return _tipo->compareStructure(tipo);
  } else
  {
    return _tipo->compareTypes(tipo);
  }
}

DefFunction::DefFunction ( std::string               id
                         , std::vector<Parameter*>*  parameters
                         , Type*                     type
                         , Statement*                statements
                         )
  : _id         ( id )
  , _parameters ( parameters )
  , _type       ( type )
  , _statements ( statements )
  {}

std::string DefFunction::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  std::string str = padding + "Definición de funcion\n" + padding + "Nombre:\n" + padding + "  " + _id + "\n" + padding + "Parametros:\n";
  for (unsigned int i=0; i< _parameters->size(); ++i)
    str += _parameters->at(i)->to_string(nesting+1) + '\n';

  str += padding + "Retorna:\n" + _type->to_string(nesting + 1)  + '\n';
  str += padding + "Instrucción:\n" + _statements->to_string(nesting+1) + '\n';
  return str;
}

void DefFunction::check()
{
  bool ok = true;

  for(std::vector<Parameter*>::iterator it = _parameters->begin(); it != _parameters->end(); it++)
  {
    (*it)->check();
    if((*it)->get_type() == ErrorType::getInstance()) ok = false;
  }

  _statements->check();
  _statements->checkReturn(_type);

  if(!_statements->getReturn())
    if(ExtintoType::getInstance() != _type)
      warning("function does not contain any explicit return statement");

  if(_statements->get_type() == ErrorType::getInstance() or !ok)
    set_type(ErrorType::getInstance());
  else
    set_type(_type);
}

void DefFunction::toIntermediate(IntermediateGen *intGen)
{

  intGen->gen(_id);  
  _statements->toIntermediate(intGen);
}

#endif
