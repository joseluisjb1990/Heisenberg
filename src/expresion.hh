#ifndef EXPRESSION_HH
#define EXPRESSION_HH
#include <string>
#include <vector>
#include <iostream>
#include "type.hh"
#include "node.hh"
#include "quad.hh"
#include "TablaSimbolos.hh"

using namespace std;

class Expression : public Node
{
  public :

    Expression();
    virtual void check() {};
    virtual std::string to_string(int nesting);
    virtual void backpatch(bool con, int jumpDes, IntermediateGen *intGen) {}
    virtual void write(IntermediateGen *intGen) {}
    std::string to_string()                 { return "expression";  };
    void setNoMut()                         { _mutID = false;       }
    void setTam(unsigned int tam)           { _tam = tam;           }
    bool getMut()                           { return _mutID;        }
    unsigned int getTam()                   { return _tam;          }
    virtual std::string getTemp()           { return _temp;         }
    virtual void setTemp(std::string temp)  { _temp = temp;         }
    virtual void toIntermediateGoto(IntermediateGen *intGen) {}
    virtual bool isStruct()                 { return false;         }
    virtual bool isIdExpr()                 { return false;         }
    virtual bool isArray()                  { return false;         }
    virtual std::string getArrayName()      { return "";            }
    virtual Type* getType()                 { return new EmptyType(); }

  private :
    bool         _mutID = true;
    unsigned int _tam   = 0;
    std::string  _temp  = "";
};

class Constant : public Expression
{};

class PolarExpr : public Constant
{
  private :
    std::string valor;

  public :
    PolarExpr(std::string valor);
    std::string to_string(int nesting);
    std::string getValue();
    void check();
    virtual std::string getTemp() { return valor; }
    Type* getType()       { return PolarType::getInstance(); }
};

class KodiakExpr : public Constant
{
  private :
    std::string valor;

  public :
    KodiakExpr(std::string valor);
    std::string to_string(int nesting);
    std::string getValue();
    void check();
    virtual std::string getTemp() { return valor; }
    Type* getType()       { return KodiakType::getInstance(); }
};

class HormigueroExpr : public Constant
{
  private :
    std::string valor;

  public :
    HormigueroExpr(std::string valor);
    std::string to_string(int nesting);
    std::string getValue();
    void check();
    void toIntermediate(IntermediateGen* intGen) { 
                                                   std::string temp = intGen->nextTemp();
                                                   Quad* q = new AssignQuad(valor,temp);
                                                   intGen->gen(q);
                                                   setTemp(temp);
                                                 };
    Type* getType()       { return new HormigueroType(); }
};

class MalayoExpr : public Constant
{
  private :
    std::string valor;

  public :
    MalayoExpr(std::string valor);
    std::string to_string(int nesting);
    std::string getValue();
    void check();
    virtual std::string getTemp() { return valor; }
    Type* getType()       { return MalayoType::getInstance(); }
};

class BoolExpr : public Expression
{
public:
  long _offset;
  unsigned int _trueList  = 0;
  unsigned int _falseList = 0;
  virtual void backpatch(bool con, int jumpDes, IntermediateGen *intGen) { 
                                                                            if      (!con and _falseList  != 0) intGen->patch(_falseList, jumpDes); 
                                                                            else if (con  and _trueList   != 0) intGen->patch(_trueList , jumpDes);
                                                                         }
};

class PandaExpr : public BoolExpr
{
  private :
    std::string valor;

  public :
    PandaExpr(std::string valor);
    std::string to_string(int nesting);
    std::string getValue();
    void check();
    virtual std::string getTemp() { return valor; }
};

class TrueExpr : public PandaExpr
{
public:
  TrueExpr(std::string valor) : PandaExpr (valor) {}
  void toIntermediateGoto(IntermediateGen *intGen);
};

class FalseExpr : public PandaExpr
{
public:
  FalseExpr(std::string valor) : PandaExpr (valor) {}
  void toIntermediateGoto(IntermediateGen *intGen);
};

class Sum : public Expression
{
  private :
    Expression* izq;
    Expression* der;

  public :
    Sum(Expression* izq, Expression* der);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);

};

class Substraction : public Expression
{
  private :
    Expression* izq;
    Expression* der;

  public :
    Substraction(Expression* izq, Expression* der);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);

};

class Multiplication : public Expression
{
  private :
    Expression* izq;
    Expression* der;

  public :
    Multiplication(Expression* izq, Expression* der);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);

};

class Division : public Expression
{
  private :
    Expression* izq;
    Expression* der;

  public :
    Division(Expression* izq, Expression* der);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);
};

class Remainder : public Expression
{
  private :
    Expression* izq;
    Expression* der;

  public :
    Remainder(Expression* izq, Expression* der);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);

};

class Power : public Expression
{
  private :
    Expression* izq;
    Expression* der;

  public :
    Power(Expression* izq, Expression* der);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);
};

class Minus : public Expression
{
  private :
    Expression* operando;

  public :
    Minus(Expression* operando);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);

};

class Less : public BoolExpr
{
  private :
    Expression* izq;
    Expression* der;

  public :
    Less(Expression* izq, Expression* der);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);
    void toIntermediateGoto(IntermediateGen *intGen);
};

class LessEqual : public BoolExpr
{
  private :
    Expression* izq;
    Expression* der;

  public :
    LessEqual(Expression* izq, Expression* der);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);
    void toIntermediateGoto(IntermediateGen *intGen);
};

class Greater : public BoolExpr
{
  private :
    Expression* izq;
    Expression* der;

  public :
    Greater(Expression* izq, Expression* der);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);
    void toIntermediateGoto(IntermediateGen *intGen);
};

class GreaterEqual : public BoolExpr
{
  private :
    Expression* izq;
    Expression* der;

  public :
    GreaterEqual(Expression* izq, Expression* der);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);
    void toIntermediateGoto(IntermediateGen *intGen);
};

class Equal : public BoolExpr
{
  private :
    Expression* izq;
    Expression* der;

  public :
    Equal(Expression* izq, Expression* der);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);
    void toIntermediateGoto(IntermediateGen *intGen);
};

class NotEqual : public BoolExpr
{
  private :
    Expression* izq;
    Expression* der;

  public :
    NotEqual(Expression* izq, Expression* der);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);
    void toIntermediateGoto(IntermediateGen *intGen);
};

class And : public BoolExpr
{
  private :
    Expression* izq;
    Expression* der;

  public :
    And(Expression* izq, Expression* der);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);
    void toIntermediateGoto(IntermediateGen *intGen);
    void backpatch(bool con, int jumpDes, IntermediateGen *intGen);
};

class Or : public BoolExpr
{
  private :
    Expression* izq;
    Expression* der;

  public :
    Or(Expression* izq, Expression* der);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);
    void toIntermediateGoto(IntermediateGen *intGen);
    void backpatch(bool con, int jumpDes, IntermediateGen *intGen);
};

class Not : public BoolExpr
{
  private :
    Expression* operando;
    unsigned int _desTrue;
    unsigned int _desFalse;

  public :
    Not(Expression* operando);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);
    void toIntermediateGoto(IntermediateGen *intGen);
    void backpatch(bool con, int jumpDes, IntermediateGen *intGen);
    
};

class SelectorExpr : public BoolExpr
{
  private:
    Expression *_condicion;
    Expression *_brazoTrue;
    Expression *_brazoFalse;

  public:
    SelectorExpr(Expression* condicion, Expression* brazoTrue, Expression* brazoFalse);
    std::string to_string(int nesting);
};

class LValueExpr : public Expression
{
  public:
    LValueExpr(){};
};

class EmptyExpr : public LValueExpr
{
  public:
    EmptyExpr(){};
    void check();
};

class IDExpr : public LValueExpr
{
  private:
    std::string _nombre;
    Contenido * _tableRow;
    unsigned int  _trueList = 0;
    unsigned int  _falseList = 0;

  public:
    IDExpr(std::string nombre);
    IDExpr(std::string nombre, Contenido* c);
    std::string to_string(int nesting);
    void check();
    std::string getTemp() { return _nombre; }
    void setTemp(std::string temp) { _nombre = temp; }
    void toIntermediateGoto(IntermediateGen *intGen);
    bool isIdExpr()       { return true;    }
    void backpatch(bool con, int jumpDes, IntermediateGen *intGen) { 
                                                                      if      (!con and _falseList  != 0) intGen->patch(_falseList, jumpDes); 
                                                                      else if (con  and _trueList   != 0) intGen->patch(_trueList , jumpDes);
                                                                   }
    Type* getType();
};

class FunctionExpr : public Expression
{
  public:
    FunctionExpr(   std::string name
                  , std::vector<Type*>* parameterTypes
                  , std::vector<Expression*>* parameters
                  , Type* returnType
                  );
    FunctionExpr( std::string name
                , std::vector<Type*>* parameterTypes
                , std::vector<Expression*>* parameters
                , Type* returnType
                , std::vector<Parameter*>* defParametros
                );
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);
    void toIntermediateAux(IntermediateGen *intGen);
    void toIntermediateGoto(IntermediateGen *intGen);
    void backpatch(bool con, int jumpDes, IntermediateGen *intGen);
    unsigned int _trueList  = 0;
    unsigned int _falseList = 0;
    
  private:
    std::string               _name;
    std::vector<Type*>*       _parameterTypes;
    std::vector<Expression*>* _parameters;
    Type*                     _return;
    std::vector<Parameter*>*  _defParametros;
};

class AKodiakExpr : public Expression
{
  public:
    AKodiakExpr(Expression* parameter);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);

  private:
    Expression* _parameter;
};

class APolarExpr : public Expression
{
  public:
    APolarExpr(Expression* parameter);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);

  private:
    Expression* _parameter;
};

class AMalayoExpr : public Expression
{
  public:
    AMalayoExpr(Expression* parameter);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);

  private:
    Expression* _parameter;
};

class APandaExpr : public Expression
{
  public:
    APandaExpr(Expression* parameter);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);

  private:
    Expression* _parameter;
};

class LonExpr : public Expression
{
  public:
    LonExpr(Expression* parameter);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);
    
  private:
    Expression* _parameter;
};

class CuevaExpr : public LValueExpr
{
  public:
    CuevaExpr(std::string cueva, std::vector<Expression*>* dimensions);
    CuevaExpr(std::string cueva, std::vector<Expression*>* dimensions, Contenido *tableRow);
    std::string to_string(int nesting);
    void addDimension(Expression* dimension);
    void check();
    void toIntermediate(IntermediateGen *intGen);
    Contenido* getTableRow()        { return _tableRow; }
    std::string getArrayName()      { return _cueva;    }
    bool isArray()                  { return true;      }
    
  private:
    std::string               _cueva;
    std::vector<Expression*>* _dimensions;
    Contenido *_tableRow;
    CuevaType *_cuevaType;
};

class PardoExpr : public LValueExpr
{
  public:
    PardoExpr(LValueExpr* pardo, IDExpr* campo);
    PardoExpr(LValueExpr* pardo, CuevaExpr* campo);
    PardoExpr(LValueExpr* pardo, IDExpr* campo, Contenido* c);
    PardoExpr(LValueExpr* pardo, CuevaExpr* campo, Contenido* c);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen* intGen);
    bool isStruct() { return true; }

  private:
    LValueExpr* _pardo;
    LValueExpr* _campo;
    Contenido *_tableRow;
};

class GrizzliExpr : public LValueExpr
{
  public:
    GrizzliExpr(LValueExpr* grizzli, IDExpr* campo);
    GrizzliExpr(LValueExpr* grizzli, CuevaExpr* campo);
    GrizzliExpr(LValueExpr* grizzli, IDExpr* campo, Contenido* c);
    GrizzliExpr(LValueExpr* grizzli, CuevaExpr* campo, Contenido* c);
    std::string to_string(int nesting);
    void toIntermediate(IntermediateGen *intGen);
    void check();

  private:
   LValueExpr*  _grizzli;
   LValueExpr*  _campo;
   Contenido*   _tableRow;
};

#endif
