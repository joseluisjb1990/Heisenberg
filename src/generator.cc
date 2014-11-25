#ifndef GENERATOR_CC
#define GENERATOR_CC

#include "generator.hh"

using namespace std;

QuadContainer::QuadContainer(Quad* quad, unsigned int numberQuad, bool isLeader)
  : _quad       ( quad       )
  , _numberQuad ( numberQuad )
  , _isLeader   ( isLeader   )
{} 

void QuadContainer::print()
{ 
  std::cout << _numberBlock << " " << _numberQuad << " " << _isLeader << " "; _quad->print(); cout << " / ";
  for(set<string>::iterator it = _quad->_liveVar.begin(); it != _quad->_liveVar.end(); it++)
	cout << (*it) << " ";

}

FlowGraph* IntermediateGen::optimize()
{
  FlowGraph* fw = new FlowGraph(_totalQuadList);
  _totalQuadList = fw->getQuadList();
  return fw;
}

void IntermediateGen::print()
{
  unsigned int i = 0;
  for(std::vector<QuadContainer*>::iterator it = _totalQuadList->begin(); it != _totalQuadList->end(); it++)
  {
	std::cout << i++ << " ";
	(*it)->print();
	cout << endl;
  }

  cout << endl << "-------------------------ALCANCE DE FUNCIONES-----------------------------" << endl;
  
  queue<unsigned int> qalc= _symbolTable->getAlcFunciones();
  int size = qalc.size();
  unsigned int x;

  for(int i = 1;  i <= size; i++)
  {
	x = qalc.front();
	cout << x << " | ";
	qalc.push(x);
  }

  cout << endl << "-------------------------TAMANIO NECESARIO PARA LAS FUNCIONES-----------------------------" << endl;
  
  qalc= _symbolTable->getTamFunciones();
  size = qalc.size();

  for(int i = 1;  i <= size; i++)
  {
	x = qalc.front();
	cout << x << " | ";
	qalc.push(x);
  }
}

IntermediateGen::IntermediateGen(const std::string &file, TablaSimbolos * symbolTable)
  : Generator()
  { 
	_nameTempFile = file + ".s";
	_file.open( _nameTempFile.c_str(), std::ofstream::out); 
	_quadNumber = 0;
	_tempNumber = 0;
	_symbolTable = symbolTable;
	_totalQuadList = new std::vector<QuadContainer*>();
  }

void IntermediateGen::close()
{
  if(_file != NULL) _file.close();
}

bool isTemp(string op)
{
  return boost::regex_match(op.c_str(), boost::regex("_(.*)"));
}

void IntermediateGen::printSpim(TablaSimbolos* tSimbolos)
{
	QuadContainer* qc;
	Quad* q;
	vector<pair<bool, string>> arrPairs;
	
	_file << "      .text\n      .globl main\n      j main "  << std::endl; 

	queue<unsigned int> qAlc = _symbolTable->getAlcFunciones();
	queue<unsigned int> qTam = _symbolTable->getTamFunciones();

	unsigned int alc = 0;
	unsigned int tam = 0;
	
	RegisterAsigner* ra;
	ra = new RegisterAsigner(20);

	unsigned int actualScope = 0;
	
	for(vector<QuadContainer*>::iterator itq = _totalQuadList->begin() + 1; itq != _totalQuadList->end(); itq++)
	{ 
		qc  = *itq;
		q   = qc->_quad; 
		
		qc->print();
		if(actualScope != qc->getNumberBlock())
		{
		  map<string, string> msi = ra->getModVar();
		  storeAll(msi, alc, qAlc.front());
		  actualScope = qc->getNumberBlock();
		  ra = new RegisterAsigner(20);
		}


		if (q->isTag()) 
		{
			alc = qAlc.front(); qAlc.pop();
			tam = qTam.front(); qTam.pop();
			if(qAlc.empty()) qAlc.push(_symbolTable->_alcMayor);

			_file << std::endl << q->toSpim() << std::endl << std::endl;


	    _file <<  "   sub $sp, $sp, 4\n   sw $ra 0($sp)\n";
	    _file <<  "   sub $sp, $sp, 4\n   sw $fp 0($sp)\n";
			_file <<  "   add $fp $sp "   << "4"  <<  endl;
			_file <<  "   sub $sp $sp "   << tam  <<  endl;

			  
			if (qc->isLeader()) 
				_file << std::endl << "bloque" << qc->getNumberBlock() << ":" << std::endl << std::endl;

		} else if (q->isEnd())
    {
			if (qc->isLeader()) 
				_file << std::endl << "bloque" << qc->getNumberBlock() << ":" << std::endl << std::endl;

      if(q->_destiny.empty())
        _file << "   li $v0, 10 \n   syscall";
      else if (q->_destiny != "oso")
      {

        _file << "_epilog" << q->_destiny << ":\n";
			  _file << "   add $sp $sp "   << tam  <<  endl;
	      _file << "   lw $fp 0($sp)\n   add $sp, $sp, 4\n";
	      _file << "   lw $ra 0($sp)\n   add $sp, $sp, 4\n";
	      _file << "   jr $ra";
      }

    }
    else
    {

			if (qc->isLeader()) 
				_file << std::endl << "bloque" << qc->getNumberBlock() << ":" << std::endl << std::endl;

      if(q->useVariables())
			{

			  arrPairs = ra->getReg(q);
			  if(ra->getSpillMode())
			  {
				vector<pair<string, string>> v = ra->getVarToSpill();
				//for(auto& pis: v)
				  //cout << pis.first << ", " << pis.second << endl ;
				
				spillVariables(v, alc, qAlc.front());
				arrPairs = ra->getReg(q);
				ra->print();
			  }

			  // Guardamos los resultados en las variables necesarias

			  susVariables(arrPairs, q, _file, alc, qAlc.front());
			}
			

		  std::string line;
			line = q->toSpim();
		
      cout << endl << "-------------------------------------------------------------------------------------------------" << endl;
		  qc->print();
      cout << endl << "================================================================================================== << endl";
      cout << endl << line << endl;
      cout << endl << "************************************************************************************************** << endl";
		  cout << endl;
		  
			if(qc->isJump() or qc->_quad->isCall())
			{
        //cout << "TENGO QUE HACER STORE PORQUE ESTOY EN UN JUMP O UNA LLAMADA \n";
		    //qc->print();
		    //ra->print();
		    //cout << endl;
			  map<string, string> msi = ra->getModVar();
			  storeAll(msi, alc, qAlc.front());
			  ra = new RegisterAsigner(20);
			}

			if (line != "") _file << "   " << line << std::endl; 
      if (q->isCall())
      {
        if(isTemp(q->_destiny))
          if(_mapOffTemp.find(q->_destiny) == _mapOffTemp.end())
          {
            _mapOffTemp[q->_destiny] = _offsetTemp + tam;  
            _file << "   sub $sp $sp 4\n"; 
            _file << "   sw $10 0($sp)" << endl;
            _offsetTemp += 4;
            _totalTemp++;
          }
      }
		}
	}
}

void IntermediateGen::spillVariables(vector<pair<string, string>> arrPairs, unsigned int alcAct, unsigned int alcTop)
{
  Contenido* c = nullptr;
  for(auto& p: arrPairs)
  {
	for(unsigned int i = alcAct; i <= alcAct; i++)
	{
	  c = _symbolTable->find_scope(p.second, i);
	  if(c) break;
	}

	_file << "   sw " << p.first << " , " << c->getOffset() << "($fp)" << endl; 
  }
}

void IntermediateGen::storeAll(map<string, string> msi, unsigned int alcAct, unsigned int alcTop)
{
	Contenido* c = nullptr;
  for(auto& t: msi)
  {
	  for(unsigned int i = alcAct; i <= alcTop; i++)
	  {
	    if(!isTemp(t.first))
	    {
	  	  c = _symbolTable->find_scope(t.first, i);
	  	  if(c) break;
	    }
	  }

	  if(c) 
    {
      if(!c->getTipo()->isArray())
      {
        if(!c->isParameter())
          _file << "\n   sw " << t.second << " -" << c->getOffset() + 8 << "($fp)" << endl; 
        else
          _file << "\n   sw " << t.second << " "  << c->getOffset() + 4 << "($fp)" << endl; 
      }
    }
  }
}

bool isLiteralInt(string op)
{
  return boost::regex_match(op.c_str(), boost::regex("([0-9])+"));
}

bool isLiteralFloat(string op)
{
  return boost::regex_match(op.c_str(), boost::regex("(([0-9]))+\\.(([0-9])+)"));
}

void IntermediateGen::loadVariable(string var, string reg, unsigned int alcAct, unsigned int alcTop)
{
  Contenido* c;
  if(isLiteralInt(var))
  {
    _file << "   li " << reg << " " << var << endl;
  } else if(isLiteralFloat(var))
  {
    _file << "   li.s " << "$f0" << " " << var << endl;
    _file << "   mfc1 " << reg   << " " << "$f0" << endl;
  }
  else if (isTemp(var))
  {
  	_file << "   lw "   << reg << " -" << 8 + _mapOffTemp[var] << "($fp)" << endl; 
    if(--_totalTemp == 0)
    {
      _file << "    add $sp $sp " << _offsetTemp << endl;
      _offsetTemp = 0;
      _mapOffTemp.clear();
    }
  }
  else
  {
    for(unsigned int i = alcAct; i <= alcTop; i++)
    {
  	  c = _symbolTable->find_scope(var, i);
  	  if(c) break;
    }

    if(!c->isParameter())
    {
      if(c->getTipo()->isArray())
      {
  	    _file << "   move "   << reg << " " << "$fp"  << endl;
  	    _file << "   sub "    << reg << " " << reg    << " "  << c->getOffset() + 8 << endl;
      }
      else
  	    _file << "   lw "   << reg << " -" << c->getOffset() + 8 << "($fp)" << endl; 
    }
    else
    {
  	  _file << "   lw "   << reg << " " << c->getOffset() + 8 << "($fp)" << endl; 
    }
  }
}

void IntermediateGen::susVariables(vector<pair<bool, string>> arrPairs, Quad* q, std::ofstream& file, unsigned int alcAct, unsigned int alcTop)
{
  string    regLeft, regRight, regRes;
  bool      loadLeft, loadRight, loadRes;

  loadLeft    = arrPairs.at(0).first;
  regLeft     = arrPairs.at(0).second;
  loadRight   = arrPairs.at(1).first;
  regRight    = arrPairs.at(1).second;
  regRes      = arrPairs.at(2).second;

  if(loadLeft)
    loadVariable(q->_leftOperand, regLeft, alcAct, alcTop);

  if(loadRight)
    loadVariable(q->_rightOperand, regRight, alcAct, alcTop);

  q->susVarReg(regLeft, regRight, regRes);
}

unsigned int IntermediateGen::gen(Quad* q)
{
  _totalQuadList->push_back(new QuadContainer(q, getQuadNumber(), false));
  return _totalQuadList->size() - 1;
}

void IntermediateGen::patch(unsigned int pos, int jumpDes)
{
  _totalQuadList->at(pos)->_quad->setDestiny(std::to_string(jumpDes)); 
}

void IntermediateGen::gen(std::string op, std::string arg1, std::string arg2, std::string des, std::string com)
{
  std::string r = write(op, arg1, arg2, des);
  _file   << r 
		  << std::string(  40 
						- r.size()
						, ' '
						)
			<< com
			<< std::endl
			;
}

void IntermediateGen::gen(std::string op, std::string arg1, std::string arg2, std::string des)
{
  _file <<  write(op, arg1, arg2, des) << std::endl;
}

std::string IntermediateGen::write(std::string op, std::string arg1, std::string arg2, std::string des)
{
  std::string r = std::to_string(getQuadNumber());
  
  return r 
		+  ":  " 
		+  op  + " " 
		+ arg1 + " " 
		+ arg2 + " " 
		+ des;
}

void IntermediateGen::writeOp(std::string op, std::string des, std::string arg1, std::string arg2)
{
  std::cout << "Fff";
  _file << op <<" " << des << " " << arg1 + " " << arg2 << " "; 
 
}


void IntermediateGen::gen(std::string id)
{
  _file << id << ":  " << std::endl;
}


long IntermediateGen::genEmpty(std::string op)
{
  std::string s = write(op, " "," ","");
  _file << s;
  long r = offset();
  _file << std::string(10, ' ') << std::endl;
  return r;
}


void IntermediateGen::genComment(std::string comment) { _file << comment << std::endl;  }
void IntermediateGen::genSpace()                      { _file << std::endl;             }
#endif
