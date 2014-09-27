#ifndef EXPRESSION_HH
#define EXPRESSION_HH
#include <string>
#include <vector>
#include <iostream>
#include "type.hh"
#include "node.hh"

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

  private :
    bool         _mutID = true;
    unsigned int _tam   = 0;
    std::string  _temp;
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
    virtual std::string getTemp() { return valor; }

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

};

class BoolExpr : public Expression
{
public:
  long _offset;
  long _trueList  = -1;
  long _falseList = -1;
  virtual void backpatch(bool con, int jumpDes, IntermediateGen *intGen) {}
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
  void backpatch(bool con, int jumpDes, IntermediateGen *intGen);
};

class FalseExpr : public PandaExpr
{
public:
  FalseExpr(std::string valor) : PandaExpr (valor) {}
  void toIntermediateGoto(IntermediateGen *intGen);
  void backpatch(bool con, int jumpDes, IntermediateGen *intGen);
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
    void backpatch(bool con, int jumpDes, IntermediateGen *intGen);
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
    void backpatch(bool con, int jumpDes, IntermediateGen *intGen);
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
    void backpatch(bool con, int jumpDes, IntermediateGen *intGen);
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
    void backpatch(bool con, int jumpDes, IntermediateGen *intGen);
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
    void backpatch(bool con, int jumpDes, IntermediateGen *intGen);

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
    void backpatch(bool con, int jumpDes, IntermediateGen *intGen);
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
    void backpatch(bool con, int jumpDes, IntermediateGen *intGen);;
    
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


  public:
    IDExpr(std::string nombre);
    std::string to_string(int nesting);
    void check();
    std::string getTemp() { return _nombre; }

};

class FunctionExpr : public Expression
{
  public:
    FunctionExpr(std::string name, std::vector<Type*>* parameterTypes, std::vector<Expression*>* parameters, Type* returnType);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);
    
  private:
    std::string               _name;
    std::vector<Type*>*       _parameterTypes;
    std::vector<Expression*>* _parameters;
    Type*                     _return;
};

class AKodiakExpr : public Expression
{
  public:
    AKodiakExpr(Expression* parameter);
    std::string to_string(int nesting);
    void check();

  private:
    Expression* _parameter;
};

class APolarExpr : public Expression
{
  public:
    APolarExpr(Expression* parameter);
    std::string to_string(int nesting);
    void check();

  private:
    Expression* _parameter;
};

class AMalayoExpr : public Expression
{
  public:
    AMalayoExpr(Expression* parameter);
    std::string to_string(int nesting);
    void check();

  private:
    Expression* _parameter;
};

class APandaExpr : public Expression
{
  public:
    APandaExpr(Expression* parameter);
    std::string to_string(int nesting);
    void check();

  private:
    Expression* _parameter;
};

class LonExpr : public Expression
{
  public:
    LonExpr(Expression* parameter);
    std::string to_string(int nesting);
    void check();

  private:
    Expression* _parameter;
};

class CuevaExpr : public LValueExpr
{
  public:
    CuevaExpr(std::string cueva, std::vector<Expression*>* dimensions);
    std::string to_string(int nesting);
    void addDimension(Expression* dimension);
    void check();

  private:
    std::string                     _cueva;
    std::vector<Expression*>* _dimensions;

};

class PardoExpr : public LValueExpr
{
  public:
    PardoExpr(LValueExpr* pardo, IDExpr* campo);
    PardoExpr(LValueExpr* pardo, CuevaExpr* campo);
    std::string to_string(int nesting);
    void check();

  private:
   LValueExpr* _pardo;
   LValueExpr* _campo;

};

class GrizzliExpr : public LValueExpr
{
  public:
    GrizzliExpr(LValueExpr* grizzli, IDExpr* campo);
    GrizzliExpr(LValueExpr* grizzli, CuevaExpr* campo);
    std::string to_string(int nesting);
    void check();

  private:
   LValueExpr* _grizzli;
   LValueExpr* _campo;

};

#endif


