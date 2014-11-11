#include "bear_driver.hh"
#include "bear_parser.tab.hh"

bear_driver::bear_driver () {}

int bear_driver::parse (const std::string &f)
{
  
  int res;

  file = f;
  scan_begin ();
  yy::bear_parser parser (*this);
  res = parser.parse ();

  if (!untilAST)
  {
    AST->check();
    if (0 == errores and untilTypeCheck) {
      if(AST->get_type() != ErrorType::getInstance()) { cout << AST->to_string(0) << std::endl << tabla << std::endl; }
    }

    if (!untilTypeCheck and errores == 0)
    {
      IntermediateGen *intgen = new IntermediateGen(file, &tabla);
      AST->toIntermediate(intgen);
      intgen->optimize()->print();
      intgen->close();
    }
  }
  scan_end();
  return res;
}

void
bear_driver::error (const yy::location& l, const std::string& m)
{
  ++errores;
  std::cerr << "Error between " << l.begin.line << "." << l.begin.column << " and " << l.end.line << "." << l.end.column << ": " << m << std::endl;
}

void
bear_driver::error (const yy::location& l1, const yy::location& l2, const std::string& m)
{
  ++errores;
  std::cerr << "Error between " << l1.begin.line << "." << l1.begin.column << " and " << l2.end.line << "." << l2.end.column << ": " << m << std::endl;
}

void
bear_driver::error (const unsigned int LI, const unsigned int CI, const unsigned int LF, unsigned int CF, const std::string& m)
{
  ++errores;
  std::cerr << "Error between " << LI << "." << CI << " and " << LF << "." << CF << ": " << m << std::endl;
}

void
bear_driver::error (const std::string& m)
{
  ++errores;
  std::cerr << m << std::endl;
}


void
bear_driver::warning (const yy::location& l, const std::string& m)
{
  std::cout << "Warning: between " << l.begin.line << "." << l.begin.column << " and " << l.end.line << "." << l.end.column << ": " << m << std::endl;
}

void
bear_driver::warning (const yy::location& l1, const yy::location& l2, const std::string& m)
{
  std::cout << "Warning: between " << l1.begin.line << "." << l1.begin.column << " and " << l2.end.line << "." << l2.end.column << ": " << m << std::endl;
}

void
bear_driver::warning (const unsigned int LI, const unsigned int CI, const unsigned int LF, unsigned int CF, const std::string& m)
{
  std::cout << "Warning: between " << LI << "." << CI << " and " << LF << "." << CF << ": " << m << std::endl;
}

void
bear_driver::warning (const std::string& m)
{
  std::cout << "Warning: " << m << std::endl;
}

void bear_driver::agregarConInicializacion(std::vector<elementoLista>* ids, Categorias categoria, Type* tipo, bool mut, std::vector<Expression*>* listExpr)
{
  Type* auxTipo;
  elementoLista e;
  Expression* auxExpr;

  if(tipo->isHormiguero()) { mut = false; }
  for (unsigned int i = 0; i < ids->size(); ++i)
  {
    auxExpr = listExpr->at(i);
    e = ids->at(i);

    if (tabla.check_scope(e.nombre)) {
      Contenido* c = tabla.find_symbol(e.nombre, categoria);
      error(e.lineaI, e.columnaI, e.lineaF, e.columnaF, "Attempt to redefine variable " + e.nombre + " already declared in " + std::to_string(c->getLineaDec()) + "." + std::to_string(c->getColumnaDec()) + ".");
    } else {
      if(tipo->isHormiguero())
      {
        auxTipo = new HormigueroType();
        auxTipo->setSize(auxExpr->getTam());
        tabla.add_symbol(e.nombre, auxTipo, categoria, e.lineaI, e.columnaI, e.lineaF, e.columnaF, mut);
      } else
        tabla.add_symbol(e.nombre, tipo, categoria, e.lineaI, e.columnaI, e.lineaF, e.columnaF, mut);
    }
  }
}

void bear_driver::agregarSinInicializacion(std::vector<elementoLista>* ids, Categorias categoria, Type* tipo)
{
  elementoLista e;
  bool mut;
  if(tipo->isHormiguero()) mut = false; else mut = true;

  for (unsigned int i = 0; i < ids->size(); ++i)
  {
    e = ids->at(i);
    if (tabla.check_scope(e.nombre)) {
      Contenido* c = tabla.find_symbol(e.nombre, categoria);
      error(e.lineaI, e.columnaI, e.lineaF, e.columnaF, "Attempt to redefine variable " + e.nombre + " already declared in " + std::to_string(c->getLineaDec()) + "." + std::to_string(c->getColumnaDec()) + ".");
    } else {
      tabla.add_symbol(e.nombre, tipo, categoria, e.lineaI, e.columnaI, mut);
    }
  }
}

bool bear_driver::compare_parameters(std::vector<Parameter*>* list1, std::vector<Parameter*>* list2)
{
  if(!(list1) or !(list2))           return false;
  if(list1->size() != list2->size()) return false;

  Parameter* p1;
  Parameter* p2;
  bool ok = true;

  for (unsigned int i = 0; i < list1->size(); ++i)
  {
    p1 = list1->at(i);
    p2 = list2->at(i);

    ok = p1->compareParameters(p2) and ok;
  }
  return ok;
}
