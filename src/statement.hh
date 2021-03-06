#ifndef STATEMENT_HH
#define STATEMENT_HH
#include <string>
#include <vector>
#include "expresion.hh"
#include "node.hh"

using namespace std;

class Statement : public Node
{
public :

    Statement(){};
    Statement(Type* type) : Node( type ) {}
    static bool checkFunction;
    static bool checkIter;
    virtual std::string to_string(int nesting) = 0;
    virtual bool isReturn()              { return false;          };
    virtual bool isBody()                { return false;          };
    virtual bool checkReturn(Type* type) { return true;           };
    virtual bool getReturn()             { return false;          };
    virtual void nextInst(int nextInst, IntermediateGen *intGen)  {}
    virtual void nextInstContinue(int nextInst, IntermediateGen *intGen)  {}
    virtual void nextInstBreak(int nextInst, IntermediateGen *intGen)  {}
    virtual void toIntermediateTag(IntermediateGen *intGen, std::string tag, int pos) {}
    virtual void toIntermediateTagBreak(IntermediateGen *intGen, std::string tag, int pos) {}
};

class Assign : public Statement
{
  private:
    std::vector<Expression*>* _ids;
    std::vector<Expression*>* _expr;

  public:
    Assign(std::vector<Expression*>* ids, std::vector<Expression*>* expr);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);

};

class Function : public Statement
{
  public:
    Function(std::string name, std::vector<Type*>* parameterTypes, std::vector<Expression*>* parameters, Type* returnType);
    Function(std::string name, std::vector<Type*>* parameterTypes, std::vector<Expression*>* parameters, Type* returnType, std::vector<bool>* parametros);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);
    std::string getTemp()           { return _temp; }
    void setTemp(std::string temp)  { _temp = temp; }
  
  private:
    std::string               _name;
    std::vector<Type*>*       _parameterTypes;
    std::vector<Expression*>* _parameters;
    Type*                     _return;
    std::string               _temp;
    std::vector<bool>*        _defParametros;
};

class If : public Statement
{
  public:
    If(Expression* condicion, Statement* instrucciones);
    std::string to_string(int nesting);
    void check();
    bool checkReturn(Type* type);
    void toIntermediate(IntermediateGen *intGen);
    void nextInst(int nextInst, IntermediateGen *intGen);
    void nextInstContinue(int nextInst, IntermediateGen *intGen);
    void nextInstBreak(int nextInst, IntermediateGen *intGen);
    void toIntermediateTag(IntermediateGen *intGen, std::string tag, int pos);
    void toIntermediateTagBreak(IntermediateGen *intGen, std::string tag, int pos);

  private:
    Expression* _condicion;
    Statement* _instrucciones;

};

class IfElse : public Statement
{
  public:
    IfElse(Expression* condicion, Statement* brazoTrue, Statement* brazoFalse);
    std::string to_string(int nesting);
    void check();
    bool checkReturn(Type* type);
    void toIntermediate(IntermediateGen *intGen);
    void nextInst(int nextInst, IntermediateGen *intGen);
    void nextInstContinue(int nextInst, IntermediateGen *intGen);
    void nextInstBreak(int nextInst, IntermediateGen *intGen);
    void toIntermediateTag(IntermediateGen *intGen, std::string tag, int pos);
    void toIntermediateTagBreak(IntermediateGen *intGen, std::string tag, int pos);

  private:
    Expression*   _condicion;
    Statement*    _brazoTrue;
    Statement*    _brazoFalse;
    unsigned int  _nextInst;

};

class Write : public Statement
{
  public:
    Write(Expression* expr);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);

  private:
    Expression* _expr;
};

class Read : public Statement
{
  public:
    Read(Expression*);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);

  private:
    Expression* _id;

};

class Body : public Statement
{
  public:
    Body( std::vector<Statement *>* listSta );
    std::string to_string(int nesting);
    void check();
    bool checkReturn(Type* type);
    bool isBody() { return true; }
    void setReturn() { hasReturn = true; }
    bool getReturn() { return hasReturn; }
    void toIntermediate(IntermediateGen *intGen);
    void toIntermediateTag(IntermediateGen *intGen, std::string tag, int pos);
    void toIntermediateTagBreak(IntermediateGen *intGen, std::string tag, int pos);
    void nextInstContinue(int nextInst, IntermediateGen *intGen);
    void nextInstBreak(int nextInst, IntermediateGen *intGen);

  private:
    std::vector<Statement *>  * _listSta;
    bool hasReturn = false;
};

class ComplexFor : public Statement
{
  public:
    ComplexFor(std::string id, Expression* begin, Expression* end, Expression* step, Statement* body);
    std::string to_string(int nesting);
    void check();
    bool checkReturn(Type* type);
    void toIntermediate(IntermediateGen *intGen);
    void nextInst(int nextInst, IntermediateGen *intGen);

  private:
    std::string _id;
    Expression* _begin;
    Expression* _end;
    Expression* _step;
    Statement*  _body;
    long        _nextInst;
};

class SimpleFor : public Statement
{
  public:
    SimpleFor(std::string id, Expression* begin, Expression* end, Statement* body);
    std::string to_string(int nesting);
    void check();
    bool checkReturn(Type* type);
    void toIntermediate(IntermediateGen *intGen);
    void nextInst(int nextInst, IntermediateGen *intGen);

  private:
    std::string _id;
    Expression* _begin;
    Expression* _end;
    Statement*  _body;
    long        _nextInst;
};

class IdFor : public Statement
{
  public:
    IdFor(std::string id, std::string iterVar, Statement* body);
    IdFor(std::string id, std::string iterVar, Statement* body, Contenido* c);
    std::string to_string(int nesting);
    void check();
    bool checkReturn(Type* type);
    void toIntermediate(IntermediateGen *intGen);
    void nextInst(int nextInst, IntermediateGen *intGen);

  private:
    std::string _id;
    std::string _iterVar;
    Statement*  _body;
    long        _nextInst;
    Contenido*  _tableRow;
};

class Return : public Statement
{
  public:
    Return();
    std::string to_string(int nesting);
    bool isReturn() { return true; };
    bool checkReturn(Type* type);
    void check();
};

class ReturnExpr : public Statement
{
  public:
    ReturnExpr(Expression* expr);
    std::string to_string(int nesting);
    void check();
    bool checkReturn(Type* type);
    bool isReturn() { return true; };
    void toIntermediate(IntermediateGen *intGen);
    
  private:
    Expression* _expr;

};

class Increase : public Statement
{
  public:
    Increase(std::string id);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);

  private:
    std::string _id;
};

class Decrement : public Statement
{
  public:
    Decrement(std::string id);
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);

  private:
    std::string _id;
};

class Continue : public Statement
{
  public:
    Continue();
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);
    void nextInstContinue(int nextInst, IntermediateGen *intGen);
    long _nextInst;
};

class ContinueID : public Statement
{
  public:
    ContinueID(std::string id);
    std::string to_string(int nesting);
    void check();
    void toIntermediateTag(IntermediateGen *intGen, std::string tag, int pos);
    void toIntermediate(IntermediateGen *intGen);
    long _nextInst;

  private:
    std::string _id;
};

class Break : public Statement
{
  public:
    Break();
    std::string to_string(int nesting);
    void check();
    void toIntermediate(IntermediateGen *intGen);
    void nextInstBreak(int nextInst, IntermediateGen *intGen);
    long _nextInst;
};

class BreakID : public Statement
{
  public:
    BreakID(std::string id);
    std::string to_string(int nesting);
    void check();
    void toIntermediateTagBreak(IntermediateGen *intGen, std::string tag, int pos);
    void toIntermediate(IntermediateGen *intGen);

  private:
    std::string _id;
    long _nextInst;
};

class While : public Statement
{
  public:
    While(Expression* expr, Statement* body);
    std::string to_string(int nesting);
    void check();
    bool checkReturn(Type* type);
    void toIntermediate(IntermediateGen *intGen);
    void nextInst(int nextInst, IntermediateGen *intGen);
    void toIntermediateTag(IntermediateGen *intGen, std::string tag, int pos);
    void toIntermediateTagBreak(IntermediateGen *intGen, std::string tag, int pos);
  private:
    Expression* _expr;
    Statement*  _body;
};

class TagWhile : public Statement
{
  public:
    TagWhile(std::string id, Expression* expr, Statement* body);
    std::string to_string(int nesting);
    void check();
    bool checkReturn(Type* type);
    void toIntermediate(IntermediateGen *intGen);
    void nextInst(int nextInst, IntermediateGen *intGen);
    void toIntermediateTag(IntermediateGen *intGen, std::string tag, int pos);
    void toIntermediateTagBreak(IntermediateGen *intGen, std::string tag, int pos);

  private:
    std::string _id;
    Expression* _expr;
    Statement*  _body;
};

class Empty : public Statement
{
  public:
    Empty();
    Empty( Type* type );
    std::string to_string(int nesting);
    void check();
};

#endif
