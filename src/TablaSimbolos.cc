/**
 *  Nombre del archivo = TablaSimbolos.cc
 *
 *  Autores:
 *    Gabriela Limonta
 *    José Luis Jiménez
 *
 *  Descripción:
 *    Implementación de las funciones de la clase
 *    TablaSimbolos definida en el archivo
 *    TablaSimbolos.hh
 *
 *  Última fecha de modificación:
 *    30/03/2014
 */

#ifndef TABLASIMBOLOS_CC
#define TABLASIMBOLOS_CC
#include "TablaSimbolos.hh"
#include "type.hh"
#include <algorithm>

using namespace std;

Contenedor* TablaSimbolos::add_container( std::string nombre
                                        , Type* tipo
                                        , Categorias categoria
                                        , unsigned int linea
                                        , unsigned int columna
                                        )
{
  Contenedor *cont = new  Contenedor( tipo, categoria, _pila.back(), linea, columna );
  _dicc.insert(std::make_pair( nombre, cont ) );
  return cont;
}

Contenedor* TablaSimbolos::add_container( std::string nombre
                                        , Type* tipo
                                        , Categorias categoria
                                        , unsigned int lineaDec
                                        , unsigned int columnaDec
                                        , unsigned int lineaDef
                                        , unsigned int columnaDef
                                        , unsigned int alcanceCampos
                                        )
{
  Contenedor *cont = new Contenedor( tipo, categoria, _pila.back(), lineaDec, columnaDec, lineaDef, columnaDef, alcanceCampos );
  _dicc.insert(std::make_pair( nombre, cont ) );
  return cont;
}

TablaSimbolos::TablaSimbolos ()
  :_alcance( 0 )
  { _pila.push_back( 0 ); _offsets.push_back( 0 ); }

Contenido* TablaSimbolos::find_symbol(  std::string nombre
                                     ,  Categorias cat
                                     )
{
  Contenido* constante  = nullptr;
  Contenido* mejor      = nullptr;
  Contenido* e          = nullptr;

  for(Diccionario::iterator it = _dicc.lower_bound( nombre ); it != _dicc.upper_bound( nombre ); ++it)
  {
    if ( it->first == nombre and it->second->getCategoria() == cat )
    {
      e = it->second;
      if ( e->getAlcance() == 0 ) { constante = e; }
      else
      {
        for( Stack::reverse_iterator itPila = _pila.rbegin(); itPila != _pila.rend(); ++itPila )
        {
          if( *itPila == e->getAlcance() ) { mejor = e; break; }
          else if ( mejor != nullptr and *itPila == mejor->getAlcance() ) { break; }
        }
      }
    }
  }

  if      (mejor != nullptr)      { return mejor;     }
  else if (constante != nullptr)  { return constante; }
  else                            { return nullptr;   }
}

Contenido* TablaSimbolos::find_scope( std::string nombre
                                    , Categorias cat
                                    , unsigned int alcance
                                    )
{
  for( Diccionario::iterator it = _dicc.lower_bound(nombre); it != _dicc.upper_bound(nombre); ++it )
  {
    if (it->first == nombre and it->second->getCategoria() == cat and it->second->getAlcance() == alcance)
    {
      return it->second;
    }
  }
  return nullptr;
}

Contenido* TablaSimbolos::find_scope( std::string nombre
                                    , unsigned int alcance
                                    )
{
  for(Diccionario::iterator it = _dicc.lower_bound(nombre); it != _dicc.upper_bound(nombre); ++it)
  {
    if (it->first == nombre and it->second->getAlcance() == alcance)
    {
      return it->second;
    }
  }
  return nullptr;
}

bool TablaSimbolos::check_scope( std::string nombre )
{
  Contenido *cont;
  unsigned int topePila = _pila.back();

  for(Diccionario::iterator it = _dicc.lower_bound(nombre); it != _dicc.upper_bound(nombre); ++it)
  {
    cont = it->second;
    if(cont->getAlcance() == topePila) { return true; }
  }
  return false;
}

unsigned int TablaSimbolos::exit_scope()
{
  _pila.pop_back();
  return _pila.back();
}

unsigned int TablaSimbolos::exit_function_scope()
{
  _alcanceFunciones.push(_pila.back());
  _tamFunciones.push(_offsets.back());
  _pila.pop_back();
  _offsets.pop_back();
  return _pila.back();
}

unsigned int TablaSimbolos::enter_scope()
{
  _pila.push_back(++_alcance);
  return _alcance;
}

unsigned int TablaSimbolos::enter_function_scope()
{
  _pila.push_back(++_alcance);
  _offsets.push_back(0);
  return _alcance;
}

typedef std::pair<std::string, Contenido*> pair_str_cont;
typedef std::vector<pair_str_cont> vector_str_cont;

bool compare_pairs(pair_str_cont pair1, pair_str_cont pair2)
{
  return pair1.second->getAlcance() < pair2.second->getAlcance();
}

void print_header(std::ostream& os)
{
  os  << std::setw(BIG_WIDTH / 3) << "V"      << SEPARADOR  << std::setw(BIG_WIDTH)     << "T[DIM]" << SEPARADOR
      << std::setw(SMALL_WIDTH / 2)   << "A"      << SEPARADOR  << std::setw(SMALL_WIDTH / 2)   << "LDEC"   << SEPARADOR
      << std::setw(SMALL_WIDTH / 2)   << "CDEC"   << SEPARADOR  << std::setw(SMALL_WIDTH / 2)   << "LDEF"   << SEPARADOR
      << std::setw(SMALL_WIDTH / 2)   << "CDEF"   << SEPARADOR  << std::setw(SMALL_WIDTH / 2)   << "M"      << SEPARADOR
      << std::setw(SMALL_WIDTH / 2)   << "AC"     << SEPARADOR  << std::setw(SMALL_WIDTH / 2)   << "D"      << SEPARADOR
      << std::setw(SMALL_WIDTH)       << "C"      << SEPARADOR  << std::setw(SMALL_WIDTH / 2)   << "Off" << SEPARADOR
      << std::setw(BIG_WIDTH / 2)     << "P"
      << std::endl;
}

void print_leyend(std::ostream& os)
{
  os  << std::endl << "LEYENDA" << std::endl
      << "V     : Variable                         T[DIM]  : Tipo y Dimensiones(arreglo)  A     : Alcance de la variable        \n"
      << "LDEC  : Linea de declaración             CDEC    : Columna de declaración       LDEF  : Linea de definición           \n"
      << "CDEF  : Columna de definición            M       : Mutabilidad (1 Si, 0 No)     AC    : Alcance de campos (registros) \n"
      << "D     : Si V esta definida (1 Si, 0 No)  C       : Categoria                    Off   : Offset                        \n"
      << "P     : Parametros (* por referencia) \n";
}

void print_actual_scope(std::ostream& os, int scope)
{
  os << std::endl  << std::setw(50)  << ' ' << std::setw(15) << "ALCANCE " + std::to_string(scope)  << std::endl;
}
std::ostream& operator<<(std::ostream& os, TablaSimbolos &ts)
{
  vector_str_cont pairs_vector;

  for (Diccionario::iterator pos = ts._dicc.begin(); pos != ts._dicc.end(); ++pos)
  {
    pairs_vector.push_back(std::make_pair(std::string(pos->first), pos->second));
  }

  std::sort(pairs_vector.begin(), pairs_vector.end(), compare_pairs);

  os  << std::left  << std::setw(50)  << ' ' << std::setw(20) << "TABLA DE SIMBOLOS"  << std::endl;
  print_actual_scope(os, 0);
  print_header(os);

  unsigned int alcance_ant = 0;

  for (vector_str_cont::iterator pos = pairs_vector.begin(); pos != pairs_vector.end(); ++pos)
  {
    unsigned int alcance_actual = pos->second->getAlcance();

    if(alcance_ant != alcance_actual)
    {
      alcance_ant = alcance_actual;
      print_actual_scope(os, alcance_actual);
      print_header(os);
    }
    os << std::setw(BIG_WIDTH / 3) << pos->first << SEPARADOR << pos->second->to_string() << '\n';
  }

  print_leyend(os);

  return os;
}

void TablaSimbolos::insert_symbol(std::string nombre, Contenido* cont)
{
  Type* tipo = cont->getTipo();
  unsigned int top   = _offsets.back(); _offsets.pop_back();
  unsigned int align = tipo->getAlign();
  unsigned int tam   = tipo->getSize();

  if((top % align) != 0)
  {
    top = top + (align - top % align);
    cont->addOffset(top);
  }
  else cont->addOffset(top);

  top += tam;
  _offsets.push_back(top);

  _dicc.insert(std::make_pair( nombre, cont ) );
}

unsigned int TablaSimbolos::add_symbol( std::string nombre
                                      , Type* tipo
                                      , Categorias categoria
                                      , unsigned int linea
                                      , unsigned int columna
                                      , bool mut
                                      )
{
  Contenido *cont = new Contenido( tipo, categoria, _pila.back(), linea, columna, mut );
  insert_symbol(nombre, cont);
  return _alcance;
}

unsigned int TablaSimbolos::add_symbol( std::string nombre
                                      , Type* tipo
                                      , Categorias categoria
                                      , unsigned int lineaDec
                                      , unsigned int columnaDec
                                      , unsigned int lineaDef
                                      , unsigned int columnaDef
                                      , bool mut
                                      )
{
  Contenido *cont = new Contenido( tipo, categoria, _pila.back(), lineaDec, columnaDec, lineaDef, columnaDef, mut );
  insert_symbol(nombre, cont);
  return _alcance;
}

unsigned int TablaSimbolos::add_function  (
                                            std::string nombre
                                          , Type* tipo
                                          , unsigned int linea
                                          , unsigned int columna
                                          , vector<Parameter*>* parametros
                                          )
{
  Contenido *cont = new Funcion(tipo, _pila.back(), linea, columna, parametros);
  _dicc.insert(std::make_pair(nombre,cont));
  return _alcance;
}

unsigned int TablaSimbolos::add_function  (
                                            std::string nombre
                                          , Type* tipo
                                          , unsigned int lineaDec
                                          , unsigned int columnaDec
                                          , unsigned int lineaDef
                                          , unsigned int columnaDef
                                          , vector<Parameter*>* parametros
                                          )
{
  Contenido *cont = new Funcion( tipo, _pila.back(), lineaDec, columnaDec, lineaDef, columnaDef, parametros );
  _dicc.insert(std::make_pair( nombre, cont ) );
  return _alcance;
}

unsigned int TablaSimbolos::get_actual_scope() { return _pila.back();    }
unsigned int TablaSimbolos::get_actual_tam() { return _offsets.back(); }

Funcion* TablaSimbolos::get_function(std::string nombre)
{
  for(Diccionario::iterator it = _dicc.lower_bound(nombre); it != _dicc.upper_bound(nombre); ++it)
  {
    if (it->first == nombre)
    {
      if(it->second->getCategoria() == Proc)
      {
        Funcion* f = dynamic_cast <Funcion *>(it->second);
        return f;
      }
    }
  }
  return nullptr;
}

Contenedor* TablaSimbolos::update_container(  string nombre
                                           , Type* tipo
                                           , unsigned int lineaDef
                                           , unsigned int columnaDef
                                           , unsigned int alcanceCampos
                                           )
{
  for(Diccionario::iterator it = _dicc.lower_bound(nombre); it != _dicc.upper_bound(nombre); ++it)
  {
    if (it->first == nombre)
    {
      if(it->second->getCategoria() == Compuesto)
      {
        Contenedor* c = dynamic_cast <Contenedor *>(it->second);
        c->define(lineaDef, columnaDef, tipo, alcanceCampos);
        return c;
      }
    }
  }
  return nullptr;
}

Contenedor* TablaSimbolos::find_container (string nombre)
{
  for(Diccionario::iterator it = _dicc.lower_bound(nombre); it != _dicc.upper_bound(nombre); ++it)
  {
    if (it->first == nombre)
    {
      if(it->second->getCategoria() == Compuesto)
      {
        Contenedor* c = dynamic_cast <Contenedor *>(it->second);
        return c;
      }
    }
  }
  return nullptr;
}
#endif
