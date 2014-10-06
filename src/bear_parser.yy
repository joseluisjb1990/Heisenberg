/**
 *  Nombre del archivo = bear_parser.yy
 *
 *  Autores:
 *    Gabriela Limonta
 *    José Luis Jiménez
 *
 *  Descripción:
 *  Implementación del parser para el lenguaje Bear utilizando Bison.
 *
 *  Última fecha de modificación:
 *    30/03/2014
 */

%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.2"
%defines
%define parser_class_name {bear_parser}
%define api.token.constructor
%define api.value.type variant
%code requires
{
# include <string>
# include <vector>
# include "node.hh"
# include "expresion.hh"
# include "statement.hh"
# include "definition.hh"
# include "type.hh"
# include "program.hh"

class bear_driver;

typedef struct {
  std::string nombre;
  unsigned int lineaI;
  unsigned int columnaI;
  unsigned int lineaF;
  unsigned int columnaF;
} elementoLista;

}
// The parsing context.
%param { bear_driver& driver }
%locations
%initial-action
{
  // Initialize the initial location.
  @$.begin.filename = @$.end.filename = &driver.file;
};
%define parse.trace
%define parse.error verbose
%code
{

#include "bear_driver.hh"

int ALCANCE_LVALUE;


std::vector<std::string>* extraerIds(std::vector<elementoLista>* ids);

}
%define api.token.prefix {TOK_}
%token
  END  0            "fin del programa"
  MENOS             "-"
  DECREMENTO        "--"
  SUMA              "+"
  INCREMENTO        "++"
  MULTIPLICACION    "*"
  POTENCIA          "**"
  DIVISION          "/"
  MENOR             "<"
  MENORIGUAL        "=<"
  MAYOR             ">"
  MAYORIGUAL        ">="
  IGUALDAD          "=="
  DIFERENCIA        "=/="
  CIERRAPARENTESIS  ")"
  ABREPARENTESIS    "("
  CIERRALLAVE       "}"
  ABRELLAVE         "{"
  CIERRACORCHETE    "]"
  ABRECORCHETE      "["
  DOSPUNTOS         ":"
  INTERROGACION     "?"
  MODULO            "%"
  AND               "&"
  OR                "|"
  NO                "no"
  PUNTOCOMA         ";"
  ASIGNACION        "="
  OSO               "oso"
  FLECHARETORNO     "=>"
  COMA              ","
  PUNTO             "."
  FLECHA            "->"
  TECHO             "^"
;

%token <std::string>                 AKODIAK
%token <std::string>                 AMALAYO
%token <std::string>                 APANDA
%token <std::string>                 APOLAR
%token <std::string>                 BLANCO
%token <std::string>                 CONST
%token <std::string>                 CONSTHORMIGUERO
%token <std::string>                 CONSTKODIAK
%token <std::string>                 CONSTMALAYO
%token <std::string>                 CONSTPOLAR
%token <std::string>                 CUEVA
%token <std::string>                 DE
%token <std::string>                 EN
%token <std::string>                 ENTONCES
%token <std::string>                 ESCRIBIR
%token <std::string>                 EXTINTO
%token <std::string>                 FONDOBLANCO
%token <std::string>                 GRIZZLI
%token <std::string>                 HORMIGUERO
%token <std::string>                 ID
%token <std::string>                 KODIAK
%token <std::string>                 LEER
%token <std::string>                 LON
%token <std::string>                 MALAYO
%token <std::string>                 MIENTRAS
%token <std::string>                 NEGRO
%token <std::string>                 PANDA
%token <std::string>                 PARA
%token <std::string>                 PARDO
%token <std::string>                 POLAR
%token <std::string>                 ROLOEPEA
%token <std::string>                 SI
%token <std::string>                 SINO
%token <std::string>                 VOMITA
%type  <Definition*>                 defcompleja
%type  <Definition*>                 defconstante
%type  <Definition*>                 deffuncion
%type  <Definition*>                 definicionglobal
%type  <Definition*>                 defvariable
%type  <Expression*>                 expresion
%type  <Expression*>                 funcionpredef
%type  <LValueExpr*>                 lvalue
%type  <Parameter*>                  defparametro
%type  <Statement*>                  bloque
%type  <Statement*>                  bloqueespecial
%type  <Statement*>                  instruccion
%type  <Statement*>                  iteracionindeterminada
%type  <Program*>                    programa
%type  <Type*>                       campo
%type  <Type*>                       defcueva
%type  <Type*>                       tipo
%type  <Type*>                       tipocueva
%type  <std::vector<Definition*>*>   definiciones
%type  <std::vector<Definition*>*>   listadefglobales
%type  <std::vector<Expression*>*>   accesocueva
%type  <std::vector<int>*>           cuevas
%type  <std::vector<Expression*>*>   expresiones
%type  <std::vector<Expression*>*>   lvalues
%type  <std::vector<Expression*>*>   maybecueva
%type  <std::vector<int>*>           parametrocueva
%type  <std::vector<Parameter*>*>    defparametros
%type  <std::vector<Statement*>*>    instrucciones
%type  <std::vector<Type*>*>         campos
%type  <std::vector<elementoLista>*> identificadores

%%
%start programa;

programa : definiciones "oso" "(" ")" "=>" EXTINTO           { driver.tabla.enter_scope();
                                                               std::vector<Parameter*>* paramVacio = new std::vector<Parameter*>();
                                                               driver.tabla.add_function("oso",ExtintoType::getInstance(),@2.begin.line,@2.begin.column, @2.begin.line, @2.begin.column, paramVacio);
                                                             }
           bloqueespecial                                    { driver.tabla.exit_scope();
                                                               $$ = new Program($1, new DefFunction("oso", new vector<Parameter*>, ExtintoType::getInstance(), $8));
                                                               driver.AST = $$;
                                                             }
         | definiciones "oso" "(" error ")" "=>" EXTINTO     { driver.error(@4, "Main function oso must not recieve parameters.");
                                                               yyerrok;
                                                               driver.tabla.enter_scope();
                                                             }
           bloqueespecial                                    { driver.tabla.exit_scope();
                                                               $$ = new Program($1, new DefFunction("oso", new vector<Parameter*>, ExtintoType::getInstance(), $9));
                                                               driver.AST = $$;
                                                             }
         | definiciones "oso" error "=>" EXTINTO             { driver.error(@3, "Missing \"()\" in main function oso.");
                                                               yyerrok;
                                                               driver.tabla.enter_scope();
                                                             }
           bloqueespecial                                    { driver.tabla.exit_scope();
                                                               $$ = new Program($1, new DefFunction("oso", new vector<Parameter*>, ExtintoType::getInstance(), $7));
                                                               driver.AST = $$;
                                                             }
         | definiciones "oso" "(" ")" "=>" error             { driver.error(@6, "Return type for main function oso must be extinto.");
                                                               yyerrok;
                                                               driver.tabla.enter_scope();
                                                             }
           bloqueespecial                                    { driver.tabla.exit_scope();
                                                               $$ = new Program($1, new DefFunction("oso", new vector<Parameter*>, ExtintoType::getInstance(), $8));
                                                               driver.AST = $$;
                                                             }
         | definiciones "oso" error "=>" error               { driver.error(@3, "Missing \"()\" in main function oso.");
                                                               driver.error(@5, "Return type for main function oso must be extinto.");
                                                               yyerrok;
                                                               driver.tabla.enter_scope();
                                                             }
           bloqueespecial                                    { driver.tabla.exit_scope();
                                                               $$ = new Program($1, new DefFunction("oso", new vector<Parameter*>, ExtintoType::getInstance(), $7));
                                                               driver.AST = $$;
                                                             }
         | definiciones "oso" error "=>" error error         { driver.error(@3, "Missing \"()\" in main function oso.");
                                                               driver.error(@5, "Return type for main function oso must be extinto.");
                                                               yyerrok;
                                                               driver.tabla.enter_scope();
                                                             }
           bloqueespecial                                    { driver.tabla.exit_scope();
                                                               $$ = new Program($1, new DefFunction("oso", new vector<Parameter*>, ExtintoType::getInstance(), $8));
                                                               driver.AST = $$;
                                                             }
         | definiciones "oso" "(" ")" "=>" error error       { driver.error(@6, "Return type for main function oso must be extinto.");
                                                               yyerrok;
                                                               driver.tabla.enter_scope();
                                                             }
           bloqueespecial                                    { driver.tabla.exit_scope();
                                                               $$ = new Program($1, new DefFunction("oso", new vector<Parameter*>, ExtintoType::getInstance(), $9));
                                                               driver.AST = $$;
                                                             }
         | definiciones "oso" "(" error ")" "=>" error       { driver.error(@4, "Main function oso must not recieve parameters.");
                                                               driver.error(@7, "The return type for main function oso must be extinto.");
                                                               yyerrok;
                                                             }
           bloqueespecial                                    { driver.tabla.exit_scope();
                                                               $$ = new Program($1, new DefFunction("oso", new vector<Parameter*>, ExtintoType::getInstance(), $9));
                                                               driver.AST = $$;
                                                             }
         | definiciones "oso" "(" error ")" "=>" error error { driver.error(@4, "Main function oso must not recieve parameters.");
                                                               driver.error(@7, "The return type for main function oso must be extinto.");
                                                               yyerrok;
                                                             }
           bloqueespecial                                    { driver.tabla.exit_scope();
                                                               $$ = new Program($1, new DefFunction("oso", new vector<Parameter*>, ExtintoType::getInstance(), $10));
                                                               driver.AST = $$;
                                                             }
         | definiciones                                      { driver.error("Missing main function oso with no parameters and return type extinto.");
                                                               yyerrok;
                                                               $$ = new Program($1,new EmptyDef());
                                                               driver.AST = $$;
                                                             }
         ;

definiciones:                    { $$ = nullptr; }
            | listadefglobales   { $$ = $1;      }
            ;

listadefglobales: definicionglobal                   { $$ = new std::vector<Definition*>(); $$->push_back($1); }
                | listadefglobales definicionglobal  { $$ = $1; $$->push_back($2);                             }
                ;

definicionglobal: defconstante  { $$ = $1; }
                | defvariable   { $$ = $1; }
                | deffuncion    { $$ = $1; }
                | defcompleja   { $$ = $1; }
                ;

deffuncion: ID "(" defparametros ")" "=>" tipo ";"        { Funcion* f = driver.tabla.get_function($1);
                                                            if (!f) {
                                                              driver.tabla.add_function($1, $6, @1.begin.line, @1.begin.column, $3);
                                                            } else {
                                                              driver.warning(@1, @7, "Function " + $1 + " already declared in " + std::to_string(f->getLineaDec()) + "." + std::to_string(f->getColumnaDec()) + ".");
                                                            }
                                                            $$ = new DecFunction($1, $3, $6);
                                                          }

          | ID "(" defparametros ")" "=>" tipo            { Funcion* f = driver.tabla.get_function($1);
                                                            if(f) {
                                                              if (!(f->getDef())) {
                                                                if (driver.compare_parameters($3, f->get_parameters())) {
                                                                  f->define(@1.begin.line, @1.begin.column);
                                                                  driver.tabla.enter_scope();
                                                                  for(std::vector<Parameter*>::iterator it = $3->begin(); it != $3->end(); ++it) {
                                                                     if(dynamic_cast<CuevaType*> ((*it)->get_tipo())) {
                                                                       driver.tabla.add_symbol((*it)->get_id(), (*it)->get_tipo(),Cueva,@3.begin.line, @3.begin.column, true);
                                                                     }
                                                                     else {
                                                                     driver.tabla.add_symbol((*it)->get_id(), (*it)->get_tipo(),Var,@3.begin.line, @3.begin.column, true);
                                                                     }
                                                                  }
                                                                } else {
                                                                  driver.error(@3, "Parameters in function definition don't match the ones in declaration" + $1 + " ");
                                                                }
                                                              } else {
                                                                driver.error(@1, "Function " + $1 + " already declared in " + std::to_string(f->getLineaDec()) + "." + std::to_string(f->getColumnaDec()) + ".");
                                                              }
                                                            } else {
                                                              driver.tabla.add_function($1,$6,@1.begin.line,@1.begin.column, @1.begin.line, @1.begin.column, $3);
                                                              driver.tabla.enter_scope();
                                                              for(std::vector<Parameter*>::iterator it = $3->begin(); it != $3->end(); ++it) {
                                                                if(dynamic_cast<CuevaType*> ((*it)->get_tipo())) {
                                                                 driver.tabla.add_symbol((*it)->get_id(), (*it)->get_tipo(),Cueva,@3.begin.line, @3.begin.column, true);
                                                                }
                                                                else {
                                                                 driver.tabla.add_symbol((*it)->get_id(), (*it)->get_tipo(),Var,@3.begin.line, @3.begin.column, true);
                                                                }
                                                              }
                                                            }
                                                          }
            bloqueespecial                                {
                                                            driver.tabla.exit_scope(); $$ = new DefFunction($1, $3, $6, $8);
                                                            $$->set_location(@1.begin.line, @1.begin.column, @6.end.line, @6.end.column);
                                                          }
          | ID "(" defparametros ")" "=>" tipo error      { yyerrok; $$ = new EmptyDef(); }
          | ID "(" defparametros ")" error bloqueespecial { yyerrok; $$ = new EmptyDef(); }
          ;

defparametros: defparametro                     { $$ = new std::vector<Parameter*> (); $$->push_back($1); }
             | defparametros "," defparametro   { $$ = $1; $$->push_back($3);                             }
             | defparametros error defparametro { $$ = $1; $$->push_back($3); yyerrok;                    }
             |                                  { $$ = new std::vector<Parameter*>();                     }
             ;

defparametro: tipo ID        { $$ = new Parameter($2, $1, false); }
            | "^" tipo ID    { $$ = new Parameter($3, $2, true);  }
            | tipocueva ID   { $$ = new Parameter($2, $1, false); }
            ;

tipocueva: parametrocueva tipo { Type* tipo = $2;
                                 for (unsigned int i= $1->size()-1; i > 0; --i) {
                                   tipo = new CuevaType(tipo, $1->at(i));
                                 }
                                 $$ = new CuevaType(tipo, $1->at(0));
                               }
         ;

parametrocueva: CUEVA "[" "]" DE                             { $$ = new std::vector<int>(); $$->push_back(0);          }
              | CUEVA "[" "]" error                          { $$ = new std::vector<int>(); $$->push_back(0); yyerrok; }
              | parametrocueva CUEVA "[" CONSTPOLAR "]" DE    { $$ = $1; $$->push_back(std::stoi($4));                                               }
              | parametrocueva error "[" CONSTPOLAR "]" DE    { $$ = $1; $$->push_back(std::stoi($4)); yyerrok;                                      }
              | parametrocueva error "[" CONSTPOLAR "]" error { $$ = $1; $$->push_back(std::stoi($4)); yyerrok;                                      }
              | parametrocueva CUEVA "[" CONSTPOLAR "]" error { $$ = $1; $$->push_back(std::stoi($4)); yyerrok;                                      }
              ;

defconstante: CONST tipo identificadores "=" expresiones ";"   { if ($3->size() == $5->size()) {
                                                                   driver.agregarConInicializacion($3, Var, $2, false, $5);
                                                                   std::vector<string>* l = extraerIds($3);
                                                                   $$ = new ConstDef($2, l, $5);
                                                                 } else {
                                                                   driver.error(@1, @5, "The number of l-values and expressions is not the same.");
                                                                   $$ = new EmptyDef();
                                                                 }
                                                               }
            | CONST tipo identificadores error expresiones ";" { yyerrok;
                                                                 $$ = new EmptyDef();
                                                               }
            ;


defvariable: tipo identificadores "=" expresiones ";"   { if ($2->size() == $4->size()) {
                                                            driver.agregarConInicializacion($2, Var, $1, true, $4);
                                                            std::vector<string>* l = extraerIds($2);
                                                            $$ = new DefVar($1, l, $4);
                                                            $$->set_location(@1.begin.line, @1.begin.column, @5.end.line, @5.end.column);
                                                          } else {
                                                            driver.error(@1, @4, "The number of l-values and expressions is not the same.");
                                                            $$ = new EmptyDef();
                                                          }
                                                        }
           | tipo identificadores error expresiones ";" { yyerrok;
                                                          $$ = new EmptyDef();
                                                        }
           | tipo identificadores error ";"             { yyerrok;
                                                          $$ = new EmptyDef();
                                                        }
           | tipo identificadores ";"                   { driver.agregarSinInicializacion($2, Var, $1);
                                                          std::vector<string>* l = extraerIds($2);
                                                          $$ = new DefVarNoInit($1, l);
                                                          $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                        }
           | defcueva ID ";"                            { $$ = new DefArray($1, $2);
                                                          driver.tabla.add_symbol($2, $1, Cueva, @2.begin.line, @2.begin.column, true);
                                                        }
           ;


identificadores: ID                     { $$ = new std::vector<elementoLista>();
                                          elementoLista e;
                                          e.nombre = $1;
                                          e.lineaI = @1.begin.line;
                                          e.columnaI = @1.begin.column;
                                          e.lineaF = @1.end.line;
                                          e.columnaF = @1.end.column;
                                          $$->push_back(e);
                                        }
               | identificadores "," ID { $$ = $1;
                                          elementoLista e;
                                          e.nombre = $3;
                                          e.lineaI = @3.begin.line;
                                          e.columnaI = @3.begin.column;
                                          e.lineaF = @3.end.line;
                                          e.columnaF = @3.end.column;
                                          $$->push_back(e);
                                        }
               ;



defcueva: cuevas tipo { Type* tipo = $2;
                        for (unsigned int i=$1->size()-1; i > 0; --i) {
                          tipo = new CuevaType(tipo, $1->at(i));
                        }
                        $$ = new CuevaType(tipo,$1->at(0));
                      }
        ;

cuevas: CUEVA        "[" CONSTPOLAR "]" DE { $$ = new std::vector<int>; $$->push_back(std::stoi($3)); }
      | cuevas CUEVA "[" CONSTPOLAR "]" DE { $$ = $1; $$->push_back(std::stoi($4));                   }
      ;


defcompleja: PARDO ID "{"   { driver.tabla.enter_scope(); }
             campos "}"     { int alcanceCampos = driver.tabla.get_actual_scope();
                              unsigned int tamanio = driver.tabla.get_actual_tam();
                              driver.tabla.exit_scope();
                              PardoType* p = new PardoType($5, $2,tamanio);
                              Contenedor* c = driver.tabla.find_container($2);
                              if (c) {
                                if (c->getDef()) {
                                  driver.error(@1, @6, "Attempt to redefine the type " + $2 + " already declared in " + std::to_string(c->getLineaDec()) + "." + std::to_string(c->getColumnaDec()) + ".");
                                } else {
                                  driver.tabla.update_container($2, p, @1.begin.line, @1.begin.column, alcanceCampos);
                                }
                              } else {
                                driver.tabla.add_container($2, p, Compuesto, @1.begin.line, @1.begin.column, @1.begin.line, @1.begin.column, alcanceCampos);
                              }
                              $$ = new DefType($2, p);
                            }

           | GRIZZLI ID "{" { driver.tabla.enter_scope(); }
             campos "}"     { int alcanceCampos = driver.tabla.get_actual_scope();
                              driver.tabla.exit_scope();
                              GrizzliType* g = new GrizzliType($5, $2);
                              Contenedor* c = driver.tabla.find_container($2);
                              if (c) {
                                if (c->getDef()) {
                                  driver.error(@1, @6, "Attempt to redefine the type " + $2 + " already declared in " + std::to_string(c->getLineaDec()) + "." + std::to_string(c->getColumnaDec()) + ".");
                                } else {
                                  driver.tabla.update_container($2, g, @1.begin.line, @1.begin.column, alcanceCampos);
                                }
                              } else {
                                driver.tabla.add_container($2, g, Compuesto, @1.begin.line, @1.begin.column, @1.begin.line, @1.begin.column, alcanceCampos);
                              }
                              $$ = new DefType($2, g);
                            }
           ;

campos: campo ";"          { $$ = new std::vector<Type*>; $$->push_back($1);          }
      | campo error        { $$ = new std::vector<Type*>; $$->push_back($1); yyerrok; }
      | campos campo ";"   { $$ = $1; $$->push_back($2);                              }
      | campos campo error { $$ = $1; $$->push_back($2);                              }
      ;

campo: tipo ID            { $$ = new CampoType($1,$2);
                            driver.tabla.add_symbol($2, $1, Campo, @2.begin.line, @2.begin.column, true);
                           }

      | defcueva ID        { $$ = $1;
                             driver.tabla.add_symbol($2, $1, Cueva, @2.begin.line, @2.begin.column, true);
                           }
      ;


tipo: PANDA       { $$ = PandaType::getInstance();                          }
    | POLAR       { $$ = PolarType::getInstance();                          }
    | KODIAK      { $$ = KodiakType::getInstance();                         }
    | MALAYO      { $$ = MalayoType::getInstance();                         }
    | HORMIGUERO  { $$ = new HormigueroType();                              }
    | EXTINTO     { $$ = ExtintoType::getInstance();                        }
    | ID          { Contenedor* c = driver.tabla.find_container($1);
                    if (!c) {
                      driver.error(@1, "Type " + $1 + " is not defined.");
                      $$ = ErrorType::getInstance();
                    } else {
                      $$ = c->getTipo();
                    }
                  }
    ;

bloqueespecial: "{" instrucciones "}" { $$ = new Body($2);                            }
              | "{" "}"               { $$ = new Body(new std::vector<Statement*>()); }

              ;

bloque: "{"               { driver.tabla.enter_scope(); }
        instrucciones "}" { $$ = new Body($3);
                            driver.tabla.exit_scope();
                          }
      ;

instrucciones: instruccion                { $$ = new std::vector<Statement*>(); $$->push_back($1); }
             | instrucciones instruccion  { $$ = $1; $$->push_back($2);                            }
             ;

%right ENTONCES SINO;
instruccion: defvariable                                                 { $$ = $1;                      }
           | defconstante                                                { $$ = $1;                      }
           | error ";"                                                   { $$ = new EmptyDef(); yyerrok; }
           | lvalues "=" expresiones ";"                                 {
                                                                           if (!($1->size() == $3->size())) {
                                                                             driver.error(@1, @3, "The number of l-values and expressions is not the same.");
                                                                             $$ = new EmptyDef();
                                                                           } else
                                                                           {
                                                                             $$ = new Assign($1, $3);
                                                                             $$->set_location(@1.begin.line, @1.begin.column, @4.end.line, @4.end.column);
                                                                             for(std::vector<Expression*>::iterator it = $1->begin(); it != $1->end(); ++it)
                                                                             {
                                                                               if(!(*it)->getMut())
                                                                               {
                                                                                 driver.error(@1, @3,"attempt to modify a unmutable variable");
                                                                                 $$ = new EmptyDef();
                                                                               }
                                                                             }
                                                                           }
                                                                         }
           | lvalues error expresiones ";"                               { $$ = new Empty(); yyerrok; }
           | LEER "(" lvalue ")" ";"                                     { $$ = new Read($3);
                                                                           $$->set_location(@1.begin.line, @1.begin.column, @5.end.line, @5.end.column);
                                                                         }
           | ESCRIBIR "(" expresion ")" ";"                              { $$ = new Write($3);
                                                                           $$->set_location(@1.begin.line, @1.begin.column, @5.end.line, @5.end.column);
                                                                         }
           | ID "(" expresiones ")" ";"                                  { Funcion* f = driver.tabla.get_function($1);
                                                                           if (!f) {
                                                                             driver.error(@1,@4,"Function " + $1 + " is not defined.");
                                                                             $$ = new Empty();
                                                                           } else {
                                                                             Type* tipoRetorno = f->getTipo();
                                                                             std::vector<Parameter*>* parametros = f->get_parameters();
                                                                             std::vector<bool>* boolParam = new std::vector<bool> ();
                                                                             std::vector<Type*>* tipos    = new std::vector<Type*>();
                                                                             Parameter* p;
                                                                             for (unsigned int i=0; i < parametros->size(); ++i) {
                                                                               p = parametros->at(i);
                                                                               tipos->push_back(p->get_tipo());
                                                                               if(p->get_ref()) boolParam->push_back(true );
                                                                               else             boolParam->push_back(false);
                                                                             }
                                                                             $$ = new Function($1, tipos, $3, tipoRetorno, boolParam);
                                                                             $$->set_location(@1.begin.line, @1.begin.column, @4.end.line, @4.end.column);
                                                                           }
                                                                         }
           | SI expresion bloque                                         {
                                                                           $$ = new If($2, $3);
                                                                           $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                                         }
           | SI expresion bloque SINO bloque                             { $$ = new IfElse($2, $3, $5);
                                                                           $$->set_location(@1.begin.line, @1.begin.column, @5.end.line, @5.end.column);
                                                                         }

           | PARA ID EN "(" expresion ";" expresion ")"                  { driver.tabla.enter_scope();
                                                                           PolarType* p = PolarType::getInstance();
                                                                           driver.tabla.add_symbol($2, p, Var, @2.begin.line, @2.begin.column, @2.end.line, @2.end.column, false);
                                                                         }
             bloqueespecial                                              {
                                                                           driver.tabla.exit_scope();
                                                                           $$ = new SimpleFor($2, $5, $7, $10);
                                                                           $$->set_location(@1.begin.line, @1.begin.column, @10.end.line, @10.end.column);
                                                                         }
           | PARA ID error "(" expresion ";" expresion ")"               { driver.tabla.enter_scope();
                                                                           yyerrok;
                                                                           PolarType* p = PolarType::getInstance();
                                                                           driver.tabla.add_symbol($2, p, Var, @2.begin.line, @2.begin.column, @2.end.line, @2.end.column, false);
                                                                         }
             bloqueespecial                                              {
                                                                           driver.tabla.exit_scope();
                                                                           $$ = new SimpleFor($2, $5, $7, $10);
                                                                         }
           | PARA ID EN "(" expresion ";" expresion ";" expresion ")"    { driver.tabla.enter_scope();
                                                                           PolarType* p = PolarType::getInstance();
                                                                           driver.tabla.add_symbol($2, p, Var, @2.begin.line, @2.begin.column, @2.end.line, @2.end.column, false);
                                                                         }
             bloqueespecial                                              { driver.tabla.exit_scope();
                                                                           $$ = new ComplexFor($2, $5, $9, $7, $12);
                                                                           $$->set_location(@1.begin.line, @1.begin.column, @12.end.line, @12.end.column);
                                                                         }
           | PARA ID error "(" expresion ";" expresion ";" expresion ")" { driver.tabla.enter_scope();
                                                                           yyerrok;
                                                                           PolarType* p = PolarType::getInstance();
                                                                           driver.tabla.add_symbol($2, p, Var, @2.begin.line, @2.begin.column, @2.end.line, @2.end.column, false);
                                                                         }
             bloqueespecial                                              { driver.tabla.exit_scope();
                                                                           $$ = new ComplexFor($2, $5, $9, $7, $12);
                                                                         }
           | PARA ID EN ID                                               { Contenido* c = driver.tabla.find_symbol($4, Cueva);
                                                                           if (!c) {
                                                                             driver.error(@4,"Cueva " + $4 + " is not declared.");
                                                                           } else {
                                                                             CuevaType* cuevita = (CuevaType*) c->getTipo();
                                                                             Type* tipo = cuevita->getTipo();
                                                                             driver.tabla.enter_scope();
                                                                             if(tipo->isArray()) {
                                                                               driver.tabla.add_symbol($2, tipo, Cueva, @2.begin.line, @2.begin.column, @2.end.line, @2.end.column, false);
                                                                             } else {
                                                                               driver.tabla.add_symbol($2, tipo, Var, @2.begin.line, @2.begin.column, @2.end.line, @2.end.column, false);
                                                                             }
                                                                           }
                                                                         }
            bloqueespecial                                               { driver.tabla.exit_scope();
                                                                           Contenido* c = driver.tabla.find_symbol($4, Cueva);
                                                                           $$ = new IdFor($2, $4, $6, c);
                                                                           $$->set_location(@1.begin.line, @1.begin.column, @6.end.line, @6.end.column);
                                                                         }
           | PARA ID error ID                                            { Contenido* c = driver.tabla.find_symbol($4, Cueva);
                                                                           yyerrok;
                                                                           if (!c) {
                                                                             driver.error(@4,"Cueva " + $4 + " is not declared.");
                                                                           }
                                                                           CuevaType* cuevita = (CuevaType*) c->getTipo();
                                                                           Type* tipo = cuevita->getTipo();
                                                                           driver.tabla.enter_scope();
                                                                           driver.tabla.add_symbol($2, tipo, Var, @2.begin.line, @2.begin.column, @2.end.line, @2.end.column, false);
                                                                         }
            bloqueespecial                                               { driver.tabla.exit_scope();
                                                                           $$ = new IdFor($2, $4, $6);
                                                                         }
           | iteracionindeterminada                                      { $$ = $1; }
           | ID "++" ";"                                                 { Contenido* c = driver.tabla.find_symbol($1,Var);
                                                                           if (c) {
                                                                             if(c->getMutabilidad()) {
                                                                               $$ = new Increase($1);
                                                                               $$->set_type(c->getTipo());
                                                                               $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                                             } else {
                                                                               driver.error(@1, @2, "Attempt to increase variable " + $1 + ", which is not mutable.");
                                                                               $$ = new Empty();
                                                                             }
                                                                           } else {
                                                                             driver.error(@1, @2, "Attempt to increase variable " + $1 + ", which is not declared.");
                                                                             $$ = new Empty();
                                                                           }
                                                                         }
           | ID "--" ";"                                                 { Contenido* c = driver.tabla.find_symbol($1,Var);
                                                                           if (c) {
                                                                             if (c->getMutabilidad()) {
                                                                               $$ = new Decrement($1);
                                                                               $$->set_type(c->getTipo());
                                                                               $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                                             } else {
                                                                               driver.error(@1, @2, "Attempt to decrease variable " + $1 + ", which is not mutable.");
                                                                               $$ = new Empty();
                                                                             }
                                                                           } else {
                                                                             driver.error(@1, @2, "Attempt to decrease variable " + $1 + ", which is not declared.");
                                                                             $$ = new Empty();
                                                                           }
                                                                         }
           | VOMITA expresion  ";"                                       { $$ = new ReturnExpr($2);
                                                                           $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                                         }
           | VOMITA ";"                                                  { $$ = new Return();
                                                                           $$->set_location(@1.begin.line, @1.begin.column, @2.end.line, @2.end.column);
                                                                         }
           | FONDOBLANCO    ";"                                          { $$ = new Continue();
                                                                           $$->set_location(@1.begin.line, @1.begin.column, @2.end.line, @2.end.column);
                                                                         }
           | FONDOBLANCO ID ";"                                          { Contenido* c = driver.tabla.find_symbol($2,Etiqueta);
                                                                           if (c) {
                                                                             $$ = new ContinueID($2);
                                                                             $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                                           } else {
                                                                             driver.error(@1, @2, "Attempt to continue to tag " + $2 + ", which is not declared.");
                                                                             $$ = new Empty();
                                                                           }
                                                                         }
           | ROLOEPEA    ";"                                             { $$ = new Break();
                                                                           $$->set_location(@1.begin.line, @1.begin.column, @2.end.line, @2.end.column);
                                                                         }
           | ROLOEPEA ID ";"                                             { Contenido* c = driver.tabla.find_symbol($2,Etiqueta);
                                                                           if (c) {
                                                                             $$ = new BreakID($2);
                                                                             $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                                           } else {
                                                                             driver.error(@1, @2, "Attempt to break to tag " + $2 + ", which is not declared.");
                                                                             $$ = new Empty();
                                                                           }
                                                                         }
           ;

iteracionindeterminada: ID ":" MIENTRAS "(" expresion ")" { Contenido* c = driver.tabla.find_symbol($1,Etiqueta);
                                                            TagType* t = new TagType();
                                                            if (!c) {
                                                              driver.tabla.add_symbol($1, t, Etiqueta, @1.begin.line, @1.begin.column, @1.end.line, @1.end.column, false);
                                                            } else {
                                                              driver.error(@1, "Tag " + $1 + " is already defined.");
                                                            }
                                                          }
                        bloque                            { $$ = new TagWhile($1, $5, $8);
                                                            $$->set_location(@1.begin.line, @1.begin.column, @8.end.line, @8.end.column);
                                                          }

                      | MIENTRAS "(" expresion ")" bloque { $$ = new While($3, $5);
                                                            $$->set_location(@1.begin.line, @1.begin.column, @5.end.line, @5.end.column);
                                                          }
                      ;

lvalues: lvalue             { $$ = new std::vector<Expression*>(); $$->push_back($1); }
       | lvalues "," lvalue { $$ = $1; $$->push_back($3);                             }
       ;

lvalue: ID maybecueva             {
                                    Contenido* c = driver.tabla.find_symbol($1, Var);
                                    if(c) {
                                      if(nullptr == $2) {
                                        $$ = new IDExpr($1, c);
                                        if(!c->getMutabilidad()) $$->setNoMut();
                                        $$->set_location(@1.begin.line, @1.begin.column, @1.end.line, @1.end.column);
                                        $$->set_type(c->getTipo());
                                        if (!c->getTipo()->isSimple()) {
                                          Contenedor* tipo = driver.tabla.find_container(c->getTipo()->getName());
                                          if ( tipo->getDef() ) {
                                            ALCANCE_LVALUE = tipo->get_alcanceCampos();
                                          } else {
                                            driver.error(@1, "Type " + tipo->getTipo()->getName() + " is never defined.");
                                            $$ = new EmptyExpr();
                                            ALCANCE_LVALUE = -1;
                                          }
                                        } else {
                                          ALCANCE_LVALUE = -1;
                                        }
                                      } else {
                                        driver.error("Variable " + $1 + " is not of type cueva, is of type " + c->getTipo()->to_string());
                                        $$ = new EmptyExpr();
                                      }
                                    } else {
                                      Contenido* c = driver.tabla.find_symbol($1, Cueva);
                                      if(c) {
                                        if(nullptr != $2) {
                                          $$ = new CuevaExpr($1, $2, c);
                                        } else {
                                          $$ = new IDExpr($1, c);
                                        }
                                        $$->set_location(@1.begin.line, @1.begin.column, @2.end.line, @2.end.column);
                                        $$->set_type(c->getTipo());
                                        CuevaType*  cueva = (CuevaType*) c->getTipo();
                                        Type* tipo = cueva->getTipo();
                                        if (!tipo->isSimple()) {
                                          Contenedor* ctipo = driver.tabla.find_container(tipo->getName());
                                          if (ctipo->getDef()){
                                            ALCANCE_LVALUE = ctipo->get_alcanceCampos();
                                          } else {
                                            driver.error(@1, "Type " + ctipo->getTipo()->getName() + " is never defined.");
                                            ALCANCE_LVALUE = -1;
                                            $$ = new EmptyExpr();
                                          }
                                        } else {
                                            ALCANCE_LVALUE = -1;
                                        }
                                      } else {
                                        driver.error(@1, "Variable " + $1 + " is not defined.");
                                        ALCANCE_LVALUE = -1;
                                        $$ = new EmptyExpr();
                                      }

                                    }
                                  }
      | lvalue "->" ID maybecueva { if (-1 != ALCANCE_LVALUE) {
                                    Contenido* c;
                                      if (nullptr == $4) {
                                        c = driver.tabla.find_scope($3, Campo, ALCANCE_LVALUE);
                                        IDExpr* id = new IDExpr($3, c);
                                        id->set_location(@3.begin.line, @3.begin.column, @3.end.line, @3.end.column);
                                        id->set_type(c->getTipo());
                                        $$ = new PardoExpr($1, id, c);
                                        $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                      } else {
                                        c = driver.tabla.find_scope($3, Cueva, ALCANCE_LVALUE);
                                        CuevaExpr* cueva = new CuevaExpr($3, $4, c);
                                        cueva->set_location(@3.begin.line, @3.begin.column, @4.end.line, @4.end.column);
                                        cueva->set_type(c->getTipo());
                                        $$ = new PardoExpr($1, cueva, c);
                                        $$->set_location(@1.begin.line, @1.begin.column, @4.end.line, @4.end.column);
                                      }

                                      if (!c) {
                                        driver.error(@3, "Variable " + $3 + " is not in scope.");
                                        ALCANCE_LVALUE = -1;
                                        $$ = new EmptyExpr();
                                      } else {
                                         if (!c->getTipo()->isSimple()) {
                                           Contenedor* tipo = driver.tabla.find_container(c->getTipo()->getName());
                                           if ( tipo->getDef() ){
                                             ALCANCE_LVALUE = tipo->get_alcanceCampos();
                                           } else {
                                             driver.error(@1, "Type " + tipo->getTipo()->getName() + " is never defined.");
                                             ALCANCE_LVALUE = -1;
                                           }
                                         } else {
                                           if (nullptr == $4) {
                                             ALCANCE_LVALUE = c->getAlcance();
                                           } else {
                                             CuevaType* cueva = (CuevaType*) c->getTipo();
                                             Contenedor* tipo = driver.tabla.find_container(cueva->getTipo()->getName());
                                             if(tipo)
                                             {
                                               ALCANCE_LVALUE = tipo->get_alcanceCampos();
                                             } else {
                                               ALCANCE_LVALUE = -1;
                                             }
                                           }
                                         }
                                      }
                                    } else {
                                      $$ = new EmptyExpr();
                                    }
                                  }
      | lvalue "."  ID maybecueva { if (-1 != ALCANCE_LVALUE) {
                                    Contenido* c;
                                      if (nullptr == $4) {
                                        c = driver.tabla.find_scope($3, Campo, ALCANCE_LVALUE);
                                        IDExpr* id = new IDExpr($3, c);
                                        id->set_location(@3.begin.line, @3.begin.column, @3.end.line, @3.end.column);
                                        id->set_type(c->getTipo());
                                        $$ = new GrizzliExpr($1, id, c);
                                        $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                      } else {
                                        c = driver.tabla.find_scope($3, Cueva, ALCANCE_LVALUE);
                                        CuevaExpr* cueva = new CuevaExpr($3, $4, c);
                                        cueva->set_location(@3.begin.line, @3.begin.column, @4.end.line, @4.end.column);
                                        cueva->set_type(c->getTipo());
                                        $$ = new GrizzliExpr($1, cueva, c);
                                        $$->set_location(@1.begin.line, @1.begin.column, @4.end.line, @4.end.column);
                                      }

                                      if (!c) {
                                        driver.error(@3, "Variable " + $3 + " is not in scope.");
                                        ALCANCE_LVALUE = -1;
                                        $$ = new EmptyExpr();
                                      } else {
                                         if (!c->getTipo()->isSimple()) {
                                           Contenedor* tipo = driver.tabla.find_container(c->getTipo()->getName());
                                           if ( tipo->getDef() ){
                                             ALCANCE_LVALUE = tipo->get_alcanceCampos();
                                           } else {
                                             driver.error(@1, "Type " + tipo->getTipo()->getName() + " is never defined.");
                                             ALCANCE_LVALUE = -1;
                                           }
                                         } else {
                                           if (nullptr == $4) {
                                             ALCANCE_LVALUE = c->getAlcance();
                                           } else {
                                             CuevaType* cueva = (CuevaType*) c->getTipo();
                                             Contenedor* tipo = driver.tabla.find_container(cueva->getTipo()->getName());
                                             if(tipo)
                                             {
                                               ALCANCE_LVALUE = tipo->get_alcanceCampos();
                                             } else {
                                               ALCANCE_LVALUE = -1;
                                             }
                                           }
                                         }
                                      }
                                    } else {
                                      $$ = new EmptyExpr();
                                    }
                                  }
      ;

maybecueva:             { $$ = nullptr; }
          | accesocueva { $$ = $1;      }
          ;

accesocueva:  "[" expresion "]"            { $$ = new std::vector<Expression*>(); $$->push_back($2); }
           | accesocueva "[" expresion "]" { $$ = $1; $$->push_back($3);                             }
           ;


expresiones: expresion                   { $$ = new std::vector<Expression*>(); $$->push_back($1); }
           | expresiones "," expresion   { $$ = $1; $$->push_back($3);                             }
           | expresiones error expresion { $$ = $1; $$->push_back($3); yyerrok;                    }
           ;

%nonassoc ":" "?";
%left "==" "=/=";
%nonassoc "<" "=<" ">" ">=";
%left "+" "-";
%left "*" "/" "%";
%left "&";
%left "|";
%nonassoc "no";
%nonassoc UNARIO;
%right "**";

expresion: CONSTPOLAR                            { $$ = new PolarExpr($1);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @1.end.line, @1.end.column);
                                                 }
         | CONSTKODIAK                           { $$ = new KodiakExpr($1);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @1.end.line, @1.end.column);
                                                 }
         | CONSTHORMIGUERO                       { $$ = new HormigueroExpr($1);
                                                   $$->setTam($1.size() - 2);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @1.end.line, @1.end.column);
                                                 }
         | CONSTMALAYO                           { $$ = new MalayoExpr($1);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @1.end.line, @1.end.column);
                                                 }
         | lvalue                                { $$ = $1;                     }
         | ID "(" expresiones ")"                { Funcion* f = driver.tabla.get_function($1);
                                                   if (!f) {
                                                     driver.error(@1,@4,"Function " + $1 + " is not defined.");
                                                     $$ = new EmptyExpr();
                                                   } else {
                                                     Type* tipoRetorno = f->getTipo();
                                                     std::vector<Parameter*>* parametros = f->get_parameters();
                                                     std::vector<Type*>* tipos = new std::vector<Type*>();
                                                     for (unsigned int i=0; i < parametros->size(); ++i) {
                                                       tipos->push_back(parametros->at(i)->get_tipo());
                                                     }
                                                     Parameter* p;
                                                     Expression* e;
                                                     bool error = false;
                                                     for (unsigned int i=0; i < $3->size(); ++i) {
                                                       e = $3->at(i);
                                                       p = parametros->at(i);
                                                       if(p->get_ref()) if(!e->isIdExpr())
                                                       {
                                                          driver.error(@1, @4, "Cannot call a function with a expression as a reference");
                                                          $$ = new EmptyExpr();
                                                          error = true;
                                                          break;
                                                       }
                                                     }
                                                     if(!error) { 
                                                        $$ = new FunctionExpr($1, tipos, $3, tipoRetorno, parametros);
                                                        $$->set_location(@1.begin.line, @1.begin.column, @4.end.line, @4.end.column);
                                                     }
                                                   }
                                                 }
         | funcionpredef                         { $$ = $1; }
         | BLANCO                                { $$ = new FalseExpr($1);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @1.end.line, @1.end.column);
                                                 }
         | NEGRO                                 { $$ = new TrueExpr($1);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @1.end.line, @1.end.column);
                                                 }
         | expresion "<"   expresion             { $$ = new Less($1, $3);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                 }
         | expresion "=<"  expresion             { $$ = new LessEqual($1, $3);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                 }
         | expresion ">"   expresion             { $$ = new Greater($1, $3);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                 }
         | expresion ">="  expresion             { $$ = new GreaterEqual($1, $3);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                 }
         | expresion "=="  expresion             { $$ = new Equal($1, $3);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                 }
         | expresion "=/=" expresion             { $$ = new NotEqual($1, $3);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                 }
         | expresion "|"   expresion             { $$ = new Or($1, $3);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                 }
         | expresion "&"   expresion             { $$ = new And($1, $3);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                 }
         | "no" expresion                        { $$ = new Not($2);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @2.end.line, @2.end.column);
                                                 }
         | expresion "+"  expresion              { $$ = new Sum($1, $3);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                 }
         | expresion "-"  expresion              { $$ = new Substraction($1, $3);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                 }
         | expresion "**" expresion              { $$ = new Power($1, $3);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                 }
         | expresion "*"  expresion              { $$ = new Multiplication($1, $3);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                 }
         | expresion "/"  expresion              { $$ = new Division($1, $3);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                 }
         | expresion "%"  expresion              { $$ = new Remainder($1, $3);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @3.end.line, @3.end.column);
                                                 }
         | "-" expresion %prec UNARIO            { $$ = new Minus($2    );
                                                   $$->set_location(@1.begin.line, @1.begin.column, @2.end.line, @2.end.column);
                                                 }
         | "(" expresion ")"                     { $$ = $2; }
         | expresion "?" expresion ":" expresion { $$ = new SelectorExpr($1, $3, $5);
                                                   $$->set_location(@1.begin.line, @1.begin.column, @5.end.line, @5.end.column);
                                                 }
         ;




funcionpredef: APANDA  "(" expresion ")" { $$ = new APandaExpr($3);
                                           $$->set_location(@1.begin.line, @1.begin.column, @4.end.line, @4.end.column);
                                         }
             | APANDA  "(" error     ")" { $$ = new EmptyExpr();
                                           $$->set_location(@1.begin.line, @1.begin.column, @4.end.line, @4.end.column);
                                         }
             | AKODIAK "(" expresion ")" { $$ = new AKodiakExpr($3);
                                           $$->set_location(@1.begin.line, @1.begin.column, @4.end.line, @4.end.column);
                                         }
             | AKODIAK "(" error     ")" { $$ = new EmptyExpr();
                                           $$->set_location(@1.begin.line, @1.begin.column, @4.end.line, @4.end.column);
                                         }
             | AMALAYO "(" expresion ")" { $$ = new AMalayoExpr($3);
                                           $$->set_location(@1.begin.line, @1.begin.column, @4.end.line, @4.end.column);
                                         }
             | AMALAYO "(" error     ")" { $$ = new EmptyExpr();
                                           $$->set_location(@1.begin.line, @1.begin.column, @4.end.line, @4.end.column);
                                         }
             | APOLAR  "(" expresion ")" { $$ = new APolarExpr($3);
                                           $$->set_location(@1.begin.line, @1.begin.column, @4.end.line, @4.end.column);
                                         }
             | APOLAR  "(" error     ")" { $$ = new EmptyExpr();
                                           $$->set_location(@1.begin.line, @1.begin.column, @4.end.line, @4.end.column);
                                         }
             | LON     "(" expresion ")" { $$ = new LonExpr($3);
                                           $$->set_location(@1.begin.line, @1.begin.column, @4.end.line, @4.end.column);
                                         }
             | LON     "(" error     ")" { $$ = new EmptyExpr();
                                           $$->set_location(@1.begin.line, @1.begin.column, @4.end.line, @4.end.column);
                                         }
             ;

%%

void yy::bear_parser::error ( const location_type& l,
                              const std::string& m  )
{
  driver.error (l, m);
}

std::vector<std::string>* extraerIds(std::vector<elementoLista>* ids)
{
  std::vector<std::string>* l = new std::vector<std::string>();
  for(unsigned int i = 0; i < ids->size(); ++i)
  {
    l->push_back(ids->at(i).nombre);
  }
  return l;

}
