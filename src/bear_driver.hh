#ifndef CALCXX_DRIVER_HH
# define CALCXX_DRIVER_HH
# include <string>
# include <map>
# include "bear_parser.tab.hh"
# include "TablaSimbolos.hh"

// Tell Flex the lexer's prototype ...
# define YY_DECL \
  yy::bear_parser::symbol_type yylex (bear_driver& driver)
// ... and declare it for the parser's sake.
YY_DECL;

// Conducting the whole scanning and parsing of Calc++.
class bear_driver
{
public:
  bear_driver();

  TablaSimbolos tabla;
  int errores = 0;
  Program* AST;

  int result;

  // Handling the scanner.
  void scan_begin ();
  void scan_end ();
  bool trace_scanning = false;
  // Run the parser on file F.
  // Return 0 on success.
  int parse (const std::string& f);
  // The name of the file being parsed.
  // Used later to pass the file name to the location tracker.
  std::string file;
  // Whether parser traces should be generated.
  bool trace_parsing = false;

  // Error handling.
  void error (const yy::location& l, const std::string& m);
  void error (const yy::location& l1, const yy::location& l2, const std::string& m);
  void error (const unsigned int LI, const unsigned int CI, const unsigned int LF, unsigned int CF, const std::string& m);
  void error (const std::string& m);
  void warning (const yy::location& l, const std::string& m);
  void warning (const yy::location& l1, const yy::location& l2, const std::string& m);
  void warning (const unsigned int LI, const unsigned int CI, const unsigned int LF, unsigned int CF, const std::string& m);
  void warning (const std::string& m);
  void agregarConInicializacion(std::vector<elementoLista>* ids, Categorias categoria, Type* tipo, bool mut, std::vector<Expression*>* listExpr);
  void agregarSinInicializacion(std::vector<elementoLista>* ids, Categorias categoria, Type* tipo);
  bool compare_parameters(std::vector<Parameter*>* list1, std::vector<Parameter*>* list2);
  void add_function_parameters(std::vector<Parameter*>* listP);
};
#endif // ! CALCXX_DRIVER_HH
