/**
 *  Nombre del archivo = type.hh
 *
 *  Autores:
 *    Gabriela Limonta
 *    José Luis Jiménez
 *
 *  Descripción:
 *    Definición de la clase Type usada en bear_parser.yy y en la
 *    tabla de simbolos para representar los tipos de las cosas en
 *    el lenguaje Bear.
 *
 *  Última fecha de modificación:
 *    30/03/2014
 */

#ifndef TYPE_HH
#define TYPE_HH
#include <string>
#include <vector>

class Expression;

using namespace std;

class Type
{
  private:
    unsigned int _tam; // Guarda el tamaño en bytes del tipo.
    unsigned int _align;

  public:
    virtual bool isArray()                          { return false;               };
    virtual bool compareStructure(Type* type)       { return compareTypes(type);  };
    virtual bool isHormiguero()                     { return false;               };
    virtual std::string to_string(int nesting) = 0;
    virtual bool isEmpty()                          { return false;               };
    virtual bool isFloat()                          { return false;               };
    virtual bool isBool()                           { return false;               };
    virtual bool isInt()                            { return false;               };
    virtual bool isConstant()                       { return false;               };
    virtual bool isString()                         { return false;               };
    /**
     * Constructor para la clase.
     *
     *  Parámetros:
     *   tam: tamaño del tipo en bytes.
     */
    Type(int tam, unsigned int align);

    /**
     * Retorna una representación en string del tipo
     *
     *  Parámetros:
     *   Ninguno.
     */
    virtual std::string to_string();

    /**
     * Retorna el tamaño en bytes del tipo
     *
     *  Parámetros:
     *   Ninguno.
     */
    virtual unsigned int getSize()            { return _tam;    };
    virtual unsigned int getAlign()           { return _align;  };
    virtual void setSize (unsigned int size)  { _tam = size;    };
    virtual void setAlign(unsigned int align) { _align = align; };

    /**
     * Retorna true si el tipo es simple y
     * false si es uno de los tipos compuestos (grizzli, pardo)
     *
     *  Parámetros:
     *   Ninguno.
     */
    virtual bool isSimple() = 0;

    /**
     * Retorna una representación en string del nombre del tipo.
     *
     *  Parámetros:
     *   Ninguno.
     */
    virtual std::string getName() { return ""; };

    virtual bool compareTypes(Type* t2) { return true; };
};

class PandaType : public Type
{
  public:
    std::string to_string();
    std::string to_string(int nesting);
    bool isSimple();
    bool isBool() { return true; };
    static PandaType* getInstance()
    {
      if(!_instance)
        _instance = new PandaType();

      return _instance;
    }
    bool compareTypes(Type* t2);

  private:
    PandaType();
    static PandaType* _instance;
};

class PolarType : public Type
{
  public:
    virtual std::string to_string();
    std::string to_string(int nesting);
    bool isSimple();
    bool isInt()                { return true;  };
    virtual bool isConstant()   { return false; };
    static PolarType* getInstance()
    {
      if(!_instance)
        _instance = new PolarType();

      return _instance;
    }
    bool compareTypes(Type* t2);

    PolarType();
  private:
    static PolarType* _instance;

};

class NumType : public PolarType
{
  public:
    static NumType* getInstance()
    {
      if(!_instance)
        _instance = new NumType();

      return _instance;
    }
    bool isConstant()          { return true; }
    
    std::string to_string() { return "Num"; }

    NumType();
    static NumType* _instance;
};

class KodiakType : public Type
{
  public:
    std::string to_string();
    std::string to_string(int nesting);
    bool isSimple();
    bool isFloat() { return true; };
    static KodiakType*getInstance()
    {
      if(!_instance)
        _instance = new KodiakType();

      return _instance;
    }
    bool compareTypes(Type* t2);

    KodiakType();
    static KodiakType* _instance;
};

class ConstFloat : public KodiakType
{
  public:
    static ConstFloat* getInstance()
    {
      if(!_instance)
        _instance = new ConstFloat();

      return _instance;
    }
    bool isConstant()          { return true; }

  private:
    ConstFloat();
    static ConstFloat* _instance;
};

class MalayoType : public Type
{
  public:
    std::string to_string();
    std::string to_string(int nesting);
    bool isSimple();
    static MalayoType* getInstance()
    {
      if(!_instance)
        _instance = new MalayoType();

      return _instance;
    }
    bool compareTypes(Type* t2);

  private:
    MalayoType();
    static MalayoType* _instance;
};

class ExtintoType : public Type
{
  public:
    std::string to_string();
    std::string to_string(int nesting);
    bool isSimple();
    static ExtintoType* getInstance()
    {
      if(!_instance)
        _instance = new ExtintoType();

      return _instance;
    }
    bool compareTypes(Type* t2);

  private:
    ExtintoType();
    static ExtintoType* _instance;
};

class HormigueroType : public Type
{
  public:
    std::string to_string();
    std::string to_string(int nesting);
    bool isSimple();
    bool isString() { return true; };
    HormigueroType();
    bool compareTypes(Type* t2);
    bool isHormiguero();
};

class CuevaType : public Type
{
  private:
    Type* _tipo;     // Tipo de los elementos de la cueva
    int   _longitud; // Lista que contiene las longitudes de cada dimension de la cueva

  public:

    /**
     * Constructor para la clase.
     *
     *  Parámetros:
     *   tipo:       tipo de los elementos de la cueva.
     *   longitudes: lista que contiene las longitudes de cada dimensión de la cueva.
     */
    CuevaType(Type* tipo, int longitud);
    bool isSimple();
    unsigned int getSize();
    std::string to_string();
    std::string to_string(int nesting);
    int getLongitud();
    bool isArray() { return true; };
    bool compareStructure(Type* type);

    /**
     * Retorna tipo de los elementos de la cueva.
     *
     *  Parámetros:
     *   Ninguno.
     */
    Type* getTipo();
    bool compareTypes(Type* t2);
};

class CampoType : public Type
{
  private:
    Type*       _tipo;   // Tipo del campo
    std::string _nombre; // Nombre del campo

  public:

    /**
     * Constructor para la clase
     *
     *  Parámetros:
     *   tipo:   tipo del campo.
     *   nombre: nombre del campo.
     */
    CampoType(Type* tipo, std::string nombre);
    std::string to_string();
    std::string to_string(int nesting);
    unsigned int getSize();
    bool isSimple();
    bool compareTypes(Type* t2);
};

class PardoType : public Type
{
  private:
    std::vector<Type*>* _campos; // Lista de campos del tipo pardo.
    std::string         _nombre; // Nombre del tipo pardo.

  public:

    /**
     * Constructor para la clase
     *
     *  Parámetros:
     *   campos: lista de campos del tipo.
     *   nombre: nombre del tipo pardo.
     */
    PardoType(std::vector<Type*>* campos, std::string nombre,unsigned int tamanio);

    /**
     * Constructor para la clase
     *
     *  Parámetros:
     *   nombre: nombre del tipo pardo.
     */
    PardoType(std::string nombre);
    std::string to_string();
    std::string to_string(int nesting);
    unsigned int getSize();
    bool isSimple();
    bool compareTypes(Type* t2);

    /**
     * Retorna una representación en string del nombre del tipo.
     *
     *  Parámetros:
     *    Ninguno.
     */
    std::string getName();
};

class GrizzliType : public Type
{
  private:
    std::vector<Type*>* _campos; // Lista de campos del tipo grizzli.
    std::string         _nombre; // Nombre del tipo grizzli.
    Type* max();

  public:

    /**
     * Constructor para la clase
     *
     *  Parámetros:
     *   campos: lista de campos del tipo.
     *   nombre: nombre del tipo grizzli.
     */
    GrizzliType(std::vector<Type*>* campos, std::string nombre);

    /**
     * Constructor para la clase
     *
     *  Parámetros:
     *   nombre: nombre del tipo grizzli.
     */
    GrizzliType(std::string nombre);
    std::string to_string();
    std::string to_string(int nesting);
    unsigned int getSize();
    bool isSimple();
    bool compareTypes(Type* t2);

    /**
     * Retorna una representación en string del nombre del tipo.
     *
     *  Parámetros:
     *    Ninguno.
     */
    std::string getName();
};

/**
 * Representa un error de tipos.
 *
 */
class ErrorType : public Type
{
  public:
    std::string to_string();
    std::string to_string(int nesting);
    bool isSimple();
    static ErrorType* getInstance()
    {
      if(!_instance)
        _instance = new ErrorType();

      return _instance;
    }
    bool compareTypes(Type* t2);
  private:
    ErrorType();
    static ErrorType* _instance;
};

/**
 * Representa el tipo de una etiqueta.
 *
 */
class TagType : public Type
{
  public:
    TagType();
    std::string to_string();
    std::string to_string(int nesting);
    bool isSimple();
    bool compareTypes(Type* t2);
};

class EmptyType : public Type
{
  public:
    EmptyType() : Type(0,0) {};
    std::string to_string(int nesting) { return "Empty Type"; }
    bool isSimple() { return true; }
    bool isEmpty()  { return true; }
};
#endif
 
