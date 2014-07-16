/**
 *  Nombre del archivo = bear_scanner.ll
 *
 *  Autores:
 *    Gabriela Limonta
 *    José Luis Jiménez
 *
 *  Descripción:
 *  Implementación del lexer para el lenguaje Bear utilizando Flex.
 *
 *  Última fecha de modificación:
 *    30/03/2014
 */

%{ /* -*- C++ -*- */
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include "bear_driver.hh"
#include "bear_parser.tab.hh"

using namespace std;

# undef yywrap
# define yywrap() 1

// The location of the current token.
static yy::location loc;
%}
%option noyywrap nounput batch debug noinput

%x comentario

DIGIT      [0-9]
ID         [a-zA-Z][a-zA-Z0-9\?!_]*
Entero     {DIGIT}+
Decimal    {DIGIT}+\.{DIGIT}*
NotCientif {DIGIT}+(\.{DIGIT})?e-?{DIGIT}+
Caracter   '.'|'\\n'

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}


%%

%{
  // Code run each time yylex is called.
  loc.step ();
%}

"<3".*   { loc.step(); };

"--<3"   { loc.step(); BEGIN(comentario); }

<comentario>{
  "<3->"   { loc.step(); BEGIN(INITIAL);                                                      }
  [^<\n]+  { loc.step();                                                                      }
  "<"      { loc.step();                                                                      }
  \n       { loc.lines(1); loc.step();                                                        }
  <<EOF>>  { driver.error(loc, "Unfinished comment."); return yy::bear_parser::make_END(loc); }
}

{Entero}     { return yy::bear_parser::make_CONSTPOLAR(yytext, loc);  }

{Decimal}    { return yy::bear_parser::make_CONSTKODIAK(yytext, loc); }

{NotCientif} { return yy::bear_parser::make_CONSTKODIAK(yytext, loc); }

{Caracter}   { return yy::bear_parser::make_CONSTMALAYO(yytext, loc); }

\"([^\"\n])*\"  { return yy::bear_parser::make_CONSTHORMIGUERO(yytext, loc);    }
\"([^\"\n])*    { driver.error(loc, "Incomplete hormiguero, missing final \""); }

a_kodiak    { return yy::bear_parser::make_AKODIAK(yytext, loc);     }
a_malayo    { return yy::bear_parser::make_AMALAYO(yytext, loc);     }
a_panda     { return yy::bear_parser::make_APANDA(yytext, loc);      }
a_polar     { return yy::bear_parser::make_APOLAR(yytext, loc);      }
polar       { return yy::bear_parser::make_POLAR(yytext, loc);       }
panda       { return yy::bear_parser::make_PANDA(yytext, loc);       }
malayo      { return yy::bear_parser::make_MALAYO(yytext, loc);      }
hormiguero  { return yy::bear_parser::make_HORMIGUERO(yytext, loc);  }
kodiak      { return yy::bear_parser::make_KODIAK(yytext, loc);      }
blanco      { return yy::bear_parser::make_BLANCO(yytext, loc);      }
const       { return yy::bear_parser::make_CONST(yytext, loc);       }
cueva       { return yy::bear_parser::make_CUEVA(yytext, loc);       }
de          { return yy::bear_parser::make_DE(yytext, loc);          }
en          { return yy::bear_parser::make_EN(yytext, loc);          }
entonces    { return yy::bear_parser::make_ENTONCES(yytext, loc);    }
escribir    { return yy::bear_parser::make_ESCRIBIR(yytext, loc);    }
extinto     { return yy::bear_parser::make_EXTINTO(yytext, loc);     }
fondoBlanco { return yy::bear_parser::make_FONDOBLANCO(yytext, loc); }
grizzli     { return yy::bear_parser::make_GRIZZLI(yytext, loc);     }
leer        { return yy::bear_parser::make_LEER(yytext, loc);        }
lon         { return yy::bear_parser::make_LON(yytext, loc);         }
mientras    { return yy::bear_parser::make_MIENTRAS(yytext, loc);    }
negro       { return yy::bear_parser::make_NEGRO(yytext, loc);       }
no          { return yy::bear_parser::make_NO(loc);                  }
oso         { return yy::bear_parser::make_OSO(loc);                 }
para        { return yy::bear_parser::make_PARA(yytext, loc);        }
pardo       { return yy::bear_parser::make_PARDO(yytext, loc);       }
roloePea    { return yy::bear_parser::make_ROLOEPEA(yytext, loc);    }
si          { return yy::bear_parser::make_SI(yytext, loc);          }
sino        { return yy::bear_parser::make_SINO(yytext, loc);        }
vomita      { return yy::bear_parser::make_VOMITA(yytext, loc);      }

{ID}   { return yy::bear_parser::make_ID(yytext, loc); }

[\n]  { loc.lines(1); loc.step (); }

[ \t]+ { loc.step (); }


"("   { return yy::bear_parser::make_ABREPARENTESIS(loc);   }
")"   { return yy::bear_parser::make_CIERRAPARENTESIS(loc); }
"{"   { return yy::bear_parser::make_ABRELLAVE(loc);        }
"]"   { return yy::bear_parser::make_CIERRACORCHETE(loc);   }
"["   { return yy::bear_parser::make_ABRECORCHETE(loc);     }
"}"   { return yy::bear_parser::make_CIERRALLAVE(loc);      }
":"   { return yy::bear_parser::make_DOSPUNTOS(loc);        }
"?"   { return yy::bear_parser::make_INTERROGACION(loc);    }
"+"   { return yy::bear_parser::make_SUMA(loc);             }
"++"  { return yy::bear_parser::make_INCREMENTO(loc);       }
"-"   { return yy::bear_parser::make_MENOS(loc);            }
"--"  { return yy::bear_parser::make_DECREMENTO(loc);       }
"*"   { return yy::bear_parser::make_MULTIPLICACION(loc);   }
"**"  { return yy::bear_parser::make_POTENCIA(loc);         }
"/"   { return yy::bear_parser::make_DIVISION(loc);         }
"%"   { return yy::bear_parser::make_MODULO(loc);           }
"&"   { return yy::bear_parser::make_AND(loc);              }
"|"   { return yy::bear_parser::make_OR(loc);               }
"<"   { return yy::bear_parser::make_MENOR(loc);            }
">"   { return yy::bear_parser::make_MAYOR(loc);            }
"=<"  { return yy::bear_parser::make_MENORIGUAL(loc);       }
">="  { return yy::bear_parser::make_MAYORIGUAL(loc);       }
"=="  { return yy::bear_parser::make_IGUALDAD(loc);         }
"=/=" { return yy::bear_parser::make_DIFERENCIA(loc);       }
";"   { return yy::bear_parser::make_PUNTOCOMA(loc);        }
","   { return yy::bear_parser::make_COMA(loc);             }
"="   { return yy::bear_parser::make_ASIGNACION(loc);       }
"=>"  { return yy::bear_parser::make_FLECHARETORNO(loc);    }
"->"  { return yy::bear_parser::make_FLECHA(loc);           }
"."   { return yy::bear_parser::make_PUNTO(loc);            }
"^"   { return yy::bear_parser::make_TECHO(loc);            }


<<EOF>>    return yy::bear_parser::make_END(loc);

.  { driver.error(loc, "Illegal character: " + std::string(yytext)); }

%%

void bear_driver::scan_begin ()
{
  yy_flex_debug = false; 
  if (file.empty () || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str (), "r")))
    {
      error ("cannot open " + file + ": " + strerror(errno));
      exit (EXIT_FAILURE);
    }
}



void bear_driver::scan_end ()
{
  fclose (yyin);
}
