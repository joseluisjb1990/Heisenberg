/**
 *  Nombre del archivo = Contenido.cc
 *
 *  Autores:
 *    Gabriela Limonta
 *    José Luis Jiménez
 *
 *  Descripción:
 *    Definición de la clase Contenido
 *    definida en Contenido.hh
 *
 *  Última fecha de modificación:
 *    30/03/2014
 */

#ifndef STATEMENT_CC
#define STATEMENT_CC
# include <iostream>
# include <string>
# include "Contenido.hh"
# include "type.hh"
# include "definition.hh"

using namespace std;

Contenido::Contenido( Type* tipo
                    , Categorias categoria
                    , unsigned int alcance
                    , unsigned int lineaDec
                    , unsigned int columnaDec
                    , unsigned int lineaDef
                    , unsigned int columnaDef
                    , bool mut
                    )
  : _tipo       ( tipo       )
  , _categoria  ( categoria  )
  , _alcance    ( alcance    )
  , _lineaDec   ( lineaDec   )
  , _columnaDec ( columnaDec )
  , _lineaDef   ( lineaDef   )
  , _columnaDef ( columnaDef )
  , _mutable    ( mut        )
  , _definido   ( true       )
  {}

Contenido::Contenido( Type* tipo
                    , Categorias categoria
                    , unsigned int alcance
                    , unsigned int linea
                    , unsigned int columna
                    , bool mut
                    )
  : _tipo       ( tipo      )
  , _categoria  ( categoria )
  , _alcance    ( alcance   )
  , _lineaDec   ( linea     )
  , _columnaDec ( columna   )
  , _lineaDef   ( 0         )
  , _columnaDef ( 0         )
  , _mutable    ( mut       )
  , _definido   ( false     )
  {}

Contenido::Contenido( Type* tipo
                    , Categorias categoria
                    , unsigned int alcance
                    , unsigned int linea
                    , unsigned int columna
                    , bool mut
                    , bool param
                    )
  : _tipo       ( tipo      )
  , _categoria  ( categoria )
  , _alcance    ( alcance   )
  , _lineaDec   ( linea     )
  , _columnaDec ( columna   )
  , _lineaDef   ( 0         )
  , _columnaDef ( 0         )
  , _mutable    ( mut       )
  , _definido   ( false     )
  , _parametro  ( true      )
  {}

void Contenido::addType(Type* tipo) { _tipo = tipo; }

void Contenido::define( unsigned int linea
                      , unsigned int columna
                      )
{
  _definido = true;
  _lineaDef = linea;
  _columnaDef = columna;
}


Contenedor::Contenedor( Type* tipo
                      , Categorias categoria
                      , unsigned int alcance
                      , unsigned int linea
                      , unsigned int columna
                      )
  : Contenido(tipo, categoria, alcance, linea, columna, true)
  , _alcanceCampos ( 0 )
  {}

Contenedor::Contenedor( Type* tipo
                      , Categorias categoria
                      , unsigned int alcance
                      , unsigned int lineaDec
                      , unsigned int columnaDec
                      , unsigned int lineaDef
                      , unsigned int columnaDef
                      , unsigned int alcanceCampos
                      )
  : Contenido(tipo, categoria, alcance, lineaDec, columnaDec, lineaDef, columnaDef, true)
  , _alcanceCampos(alcanceCampos)
  {}

void Contenedor::define( unsigned int linea
                       , unsigned int columna
                       , Type* tipo
                       , unsigned int alcanceCampos
                       )
{
  Contenido::define(linea, columna);
  this->addType(tipo);
  _alcanceCampos = alcanceCampos;
}

Funcion::Funcion ( Type* tipo
                 , unsigned int alcance
                 , unsigned int linea
                 , unsigned int columna
                 , std::vector<Parameter*>* parametros
                 )
  : Contenido(tipo, Proc, alcance, linea, columna, false)
  , _parametros( parametros )
  {}

Funcion::Funcion ( Type* tipo
                 , unsigned int alcance
                 , unsigned int lineaDec
                 , unsigned int columnaDec
                 , unsigned int lineaDef
                 , unsigned int columnaDef
                 , std::vector<Parameter*>* parametros
                 )
  : Contenido(tipo, Proc, alcance, lineaDec, columnaDec, lineaDef, columnaDef, false)
  , _parametros( parametros )
  {}

std::string return_category(Categorias cat)
{
  switch( cat )
  {
    case Var         : return std::string("VAR"     ); break;
    case Const       : return std::string("CONST"   ); break;
    case Proc        : return std::string("FUNCION" ); break;
    case Campo       : return std::string("CAMPO"   ); break;
    case Cueva       : return std::string("ARREGLO" ); break;
    case Compuesto   : return std::string("REGISTRO"); break;
    case Etiqueta    : return std::string("ETIQUETA"); break;
    default          : return std::string("NO CAT"  ); break;
  }
}
std::string Contenido::to_string()
{
  std::string tipo        = std::string(_tipo->to_string());
  std::string categoria   = std::to_string(_categoria);
  std::string alcance     = std::to_string(_alcance);
  std::string lineaDec    = std::to_string(_lineaDec);
  std::string columnaDec  = std::to_string(_columnaDec);
  std::string lineaDef    = std::to_string(_lineaDef);
  std::string columnaDef  = std::to_string(_columnaDef);
  std::string mutab       = std::to_string(_mutable);
  std::string def         = std::to_string(_definido);
  std::string offset      = std::to_string(_offset);
  std::string parametro   = std::to_string(_parametro);
  std::string cat         = return_category(_categoria);

  if(_definido)
  {

    return  tipo        + std::string(BIG_WIDTH     - tipo.size()         , ' ') + SEPARADOR
          + alcance     + std::string(SMALL_WIDTH / 2   - alcance.length()    , ' ') + SEPARADOR
          + lineaDec    + std::string(SMALL_WIDTH / 2   - lineaDec.length()   , ' ') + SEPARADOR
          + columnaDec  + std::string(SMALL_WIDTH / 2   - columnaDec.length() , ' ') + SEPARADOR
          + lineaDef    + std::string(SMALL_WIDTH / 2   - lineaDef.length()   , ' ') + SEPARADOR
          + columnaDef  + std::string(SMALL_WIDTH / 2   - columnaDef.length() , ' ') + SEPARADOR
          + mutab       + std::string(SMALL_WIDTH / 2   - mutab.length()      , ' ') + SEPARADOR
          +               std::string(SMALL_WIDTH / 2                         , ' ') + SEPARADOR
          + def         + std::string(SMALL_WIDTH / 2   - def.length()        , ' ') + SEPARADOR
          + cat         + std::string(SMALL_WIDTH   - cat.length()            , ' ') + SEPARADOR
          + offset      + std::string(SMALL_WIDTH / 2   - offset.length()     , ' ') + SEPARADOR
          +               std::string(BIG_WIDTH / 2                           , ' ') + SEPARADOR
          + parametro   + std::string(SMALL_WIDTH / 2   - parametro.length(), ' ')
          ;
  }
  else
  {
    return  tipo        + std::string(BIG_WIDTH     - tipo.size()             , ' ') + SEPARADOR
          + alcance     + std::string(SMALL_WIDTH / 2   - alcance.length()    , ' ') + SEPARADOR
          + lineaDec    + std::string(SMALL_WIDTH / 2   - lineaDec.length()   , ' ') + SEPARADOR
          + columnaDec  + std::string(SMALL_WIDTH / 2   - columnaDec.length() , ' ') + SEPARADOR
          +               std::string(SMALL_WIDTH / 2                         , ' ') + SEPARADOR
          +               std::string(SMALL_WIDTH / 2                         , ' ') + SEPARADOR
          + mutab       + std::string(SMALL_WIDTH / 2   - mutab.length()      , ' ') + SEPARADOR
          +               std::string(SMALL_WIDTH / 2                         , ' ') + SEPARADOR
          + def         + std::string(SMALL_WIDTH / 2   - def.length()        , ' ') + SEPARADOR
          + cat         + std::string(SMALL_WIDTH   - cat.length()            , ' ') + SEPARADOR
          + offset      + std::string(SMALL_WIDTH / 2   - offset.length()     , ' ') + SEPARADOR
          +               std::string(BIG_WIDTH / 2                           , ' ') + SEPARADOR  
          + parametro   + std::string(SMALL_WIDTH / 2   - parametro.length(), ' ')
          ;

  }
}

std::string Contenedor::to_string()
{

  std::string tipo;

  if      (dynamic_cast<GrizzliType*>(getTipo())) { tipo = "grizzli"; }
  else if (dynamic_cast<PardoType*>  (getTipo())) { tipo = "pardo";   }

  std::string categoria     = std::to_string(getCategoria()         );
  std::string alcance       = std::to_string(getAlcance()           );
  std::string lineaDec      = std::to_string(getLineaDec()          );
  std::string columnaDec    = std::to_string(getColumnaDec()        );
  std::string lineaDef      = std::to_string(getLineaDef()          );
  std::string columnaDef    = std::to_string(getColumnaDef()        );
  std::string mutab         = std::to_string(getMutabilidad()       );
  std::string alcanceCampos = std::to_string(_alcanceCampos         );
  std::string def           = std::to_string(getDef()               );
  std::string cat           = return_category(getCategoria());

  if(getDef())
  {

    return  tipo          + std::string(BIG_WIDTH     - tipo.size()                 , ' ') + SEPARADOR
          + alcance       + std::string(SMALL_WIDTH / 2   - alcance.length()        , ' ') + SEPARADOR
          + lineaDec      + std::string(SMALL_WIDTH / 2   - lineaDec.length()       , ' ') + SEPARADOR
          + columnaDec    + std::string(SMALL_WIDTH / 2   - columnaDec.length()     , ' ') + SEPARADOR
          + lineaDef      + std::string(SMALL_WIDTH / 2   - lineaDef.length()       , ' ') + SEPARADOR
          + columnaDef    + std::string(SMALL_WIDTH / 2   - columnaDef.length()     , ' ') + SEPARADOR
          + mutab         + std::string(SMALL_WIDTH / 2   - mutab.length()          , ' ') + SEPARADOR
          + alcanceCampos + std::string(SMALL_WIDTH / 2   - alcanceCampos.length()  , ' ') + SEPARADOR
          + def           + std::string(SMALL_WIDTH / 2   - def.length()            , ' ') + SEPARADOR
          + cat           + std::string(SMALL_WIDTH   - cat.length()                , ' ') + SEPARADOR
          +                 std::string(SMALL_WIDTH / 2                             , ' ') + SEPARADOR
          +                 std::string(BIG_WIDTH / 2                               , ' ')
          ;
  }
  else
  {
    return  tipo          + std::string(BIG_WIDTH         - tipo.size()             , ' ') + SEPARADOR
          + alcance       + std::string(SMALL_WIDTH / 2   - alcance.length()        , ' ') + SEPARADOR
          + lineaDec      + std::string(SMALL_WIDTH / 2   - lineaDec.length()       , ' ') + SEPARADOR
          + columnaDec    + std::string(SMALL_WIDTH / 2   - columnaDec.length()     , ' ') + SEPARADOR
          +                 std::string(SMALL_WIDTH / 2                             , ' ') + SEPARADOR
          +                 std::string(SMALL_WIDTH / 2                             , ' ') + SEPARADOR
          + mutab         + std::string(SMALL_WIDTH / 2   - mutab.length()          , ' ') + SEPARADOR
          + alcanceCampos + std::string(SMALL_WIDTH / 2   - alcanceCampos.length()  , ' ') + SEPARADOR
          + def           + std::string(SMALL_WIDTH / 2   - def.length()            , ' ') + SEPARADOR
          + cat           + std::string(SMALL_WIDTH       - cat.length()            , ' ') + SEPARADOR
          +                 std::string(SMALL_WIDTH / 2                             , ' ') + SEPARADOR
          +                 std::string(BIG_WIDTH / 2                               , ' ')
          ;

  }
}

std::string Funcion::to_string()
{
  std::string tipo          = std::string   (getTipo()->to_string() );
  std::string categoria     = std::to_string(getCategoria()         );
  std::string alcance       = std::to_string(getAlcance()           );
  std::string lineaDec      = std::to_string(getLineaDec()          );
  std::string columnaDec    = std::to_string(getColumnaDec()        );
  std::string lineaDef      = std::to_string(getLineaDef()          );
  std::string columnaDef    = std::to_string(getColumnaDef()        );
  std::string mutab         = std::to_string(getMutabilidad()       );
  std::string def           = std::to_string(getDef()               );
  std::string cat           = return_category(getCategoria());
  std::string param = "";
  std::string auxParam = "";

  for(std::vector<Parameter*>::iterator it = _parametros->begin(); it != _parametros->end(); ++it)
  {
    param += (*it)->to_string(0) + ", ";
  }

  if(getDef())
  {
    return  tipo          + std::string(BIG_WIDTH     - tipo.size()             , ' ') + SEPARADOR
          + alcance       + std::string(SMALL_WIDTH / 2   - alcance.length()        , ' ') + SEPARADOR
          + lineaDec      + std::string(SMALL_WIDTH / 2   - lineaDec.length()       , ' ') + SEPARADOR
          + columnaDec    + std::string(SMALL_WIDTH / 2   - columnaDec.length()     , ' ') + SEPARADOR
          + lineaDef      + std::string(SMALL_WIDTH / 2   - lineaDef.length()       , ' ') + SEPARADOR
          + columnaDef    + std::string(SMALL_WIDTH / 2   - columnaDef.length()     , ' ') + SEPARADOR
          + mutab         + std::string(SMALL_WIDTH / 2   - mutab.length()          , ' ') + SEPARADOR
          +                 std::string(SMALL_WIDTH / 2                             , ' ') + SEPARADOR
          + def           + std::string(SMALL_WIDTH / 2   - def.length()            , ' ') + SEPARADOR
          + cat           + std::string(SMALL_WIDTH   - cat.length()            , ' ') + SEPARADOR
          +                 std::string(SMALL_WIDTH / 2                             , ' ') + SEPARADOR
          + param         + std::string(BIG_WIDTH /2                            , ' ')
          ;
  }
  else
  {
    return  tipo          + std::string(BIG_WIDTH     - tipo.size()             , ' ') + SEPARADOR
          + alcance       + std::string(SMALL_WIDTH / 2   - alcance.length()        , ' ') + SEPARADOR
          + lineaDec      + std::string(SMALL_WIDTH / 2   - lineaDec.length()       , ' ') + SEPARADOR
          + columnaDec    + std::string(SMALL_WIDTH / 2   - columnaDec.length()     , ' ') + SEPARADOR
          +                 std::string(SMALL_WIDTH / 2                             , ' ') + SEPARADOR
          +                 std::string(SMALL_WIDTH / 2                             , ' ') + SEPARADOR
          + mutab         + std::string(SMALL_WIDTH / 2   - mutab.length()          , ' ') + SEPARADOR
          +                 std::string(SMALL_WIDTH / 2                             , ' ') + SEPARADOR
          + def           + std::string(SMALL_WIDTH / 2   - def.length()            , ' ') + SEPARADOR
          + cat           + std::string(SMALL_WIDTH   - cat.length()            , ' ') + SEPARADOR
          +                 std::string(SMALL_WIDTH / 2                             , ' ') + SEPARADOR
          + param         + std::string(BIG_WIDTH /2                            , ' ')
          ;

  }
}

#endif
