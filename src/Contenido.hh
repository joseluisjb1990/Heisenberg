/**
 *  Nombre del archivo = Contenido.hh
 *
 *  Autores:
 *    Gabriela Limonta
 *    José Luis Jiménez
 *
 *  Descripción:
 *    Declaración de la clase Contenido
 *    usada en TablaSimbolos.hh para manejar
 *    la infomación sobre los símbolos en la
 *    tabla.
 *
 *  Última fecha de modificación:
 *    30/03/2014
 */

#ifndef CONTENIDO_HH
#define CONTENIDO_HH
#include <vector>
class Type;
class Parameter;
/**
 * Macros para establecer la distancia entre las columnas y el separador cuando se imprimen
 * instancias de la clase.
 */

#define BIG_WIDTH     40
#define SMALL_WIDTH   8
#define SEPARADOR     "|"

/**
 * Tipo enumerado para llevar el contro de las categorías a la que
 * puede pertenecer una instancia de la clase contenido.
 */

typedef enum { Var, Const, Proc, Campo, Cueva, Compuesto, Etiqueta} Categorias;

class Contenido
{

  public:

    /**
     * Contructor para la clase Contenido.
     *
     *  Parametros:
     *    tipo:       tipo del símbolo.
     *    categoria:  categoría a la que pertenece el símbolo.
     *    lineaDec:   línea en la que fue declarado definido el símbolo.
     *    columnaDec: columna en la que fue declarado el tipo.
     *    lineaDef:   línea en la que fue definido el símbolo.
     *    columnaDef: columna en la que fue definido el símbolo.
     *    mut:        si el estado de la variable es mutable.
     *
     */
    Contenido ( Type*         tipo
              , Categorias    categoria
              , unsigned int  alcance
              , unsigned int  lineaDec
              , unsigned int  columnaDec
              , unsigned int  lineaDef
              , unsigned int  columnaDef
              , bool          mut
              );
    /**
     * Contructor para la clase Contenido.
     *
     *  Parametros:
     *    tipo:       tipo del símbolo.
     *    categoria:  categoría a la que pertenece el símbolo.
     *    linea:      línea en la que fue declarado definido el símbolo.
     *    columna:    columna en la que fue declarado el tipo.
     *    mut:        si el estado de la variable es mutable.
     *
     */
    Contenido ( Type*         tipo
              , Categorias    categoria
              , unsigned int  alcance
              , unsigned int  linea
              , unsigned int  columna
              , bool          mut
              );

    Contenido ( Type*         tipo
              , Categorias    categoria
              , unsigned int  alcance
              , unsigned int  linea
              , unsigned int  columna
              , bool          mut
              , bool          param
              );
    /**
     *  Retorna un string representando una instancia de la calse Contenido.
     *
     */
    virtual std::string to_string();

    /**
     *  Agrega el tipo a una instancia de la clase contenedor.
     *
     *  Parámetros:
     *    tipo: tipo que se desea agregar.
     */

    void addType( Type* tipo );

    /**
     *  Sirve para agregar la linea y columna de definición a una instancia
     *  de la clase contenido.
     *
     */
    void define ( unsigned int linea
                , unsigned int columna
                );

    /**
     *  Getters para los elementos de la clase.
     *
     */

    bool getDef()                   { return _definido;   };
    Type* getTipo()                 { return _tipo;       };
    bool getMutabilidad()           { return _mutable;    };
    unsigned int getAlcance()       { return _alcance;    };
    unsigned int getLineaDec()      { return _lineaDec;   };
    unsigned int getColumnaDec()    { return _columnaDec; };
    unsigned int getLineaDef()      { return _lineaDef;   };
    unsigned int getColumnaDef()    { return _columnaDef; };
    Categorias getCategoria()       { return _categoria;  };
    unsigned int getOffset()        { return _offset;     };
    void addOffset(unsigned int offset) { _offset = offset;     };
    bool isParameter()              { return _parametro;        };

  private:

    Type*         _tipo;
    Categorias    _categoria;
    unsigned int  _alcance;
    unsigned int  _lineaDec;
    unsigned int  _columnaDec;
    unsigned int  _lineaDef;
    unsigned int  _columnaDef;
    unsigned int  _offset;
    bool          _mutable;
    bool          _definido;
    bool          _parametro = false;

};

/* Clase heredada de Contenido que sirve para manejar los registros del lenguaje */
class Contenedor : public Contenido
{

  public:

    /* Constructores de la clase. */

    Contenedor  ( Type*        tipo
                , Categorias   categoria
                , unsigned int alcance
                , unsigned int linea
                , unsigned int columna
                );

    Contenedor  ( Type*        tipo
                , Categorias   categoria
                , unsigned int alcance
                , unsigned int lineaDec
                , unsigned int columnaDec
                , unsigned int lineaDef
                , unsigned int columnaDef
                , unsigned int alcanceCampos
                );

    /* Retorna una representación en string de una instancia del contenedor. */
    virtual std::string to_string();

    /* Agrega linea, columna, tipo y el alcance los campos a una instancia de la clase */
    void define(unsigned int linea, unsigned int columna, Type* tipo, unsigned int alcanceCampos);

    /* Setter ygetter para la variable _alcanceCampos */
    void set_alcanceCampos(unsigned int alcanceCampos) { _alcanceCampos = alcanceCampos; };
    unsigned int get_alcanceCampos() { return _alcanceCampos; };

private:

  unsigned int _alcanceCampos; /* Sirve para manejar el alcance en que se almacenaron los campos de un registro en la tabla. */
};

/* Clase heredada de Contenido que sirve para manejar las funciones en la tabla de símbolos */
class Funcion : public Contenido
{

  public:

    /* Constructores de la clase */
    Funcion ( Type* tipo
            , unsigned int alcance
            , unsigned int linea
            , unsigned int columna
            , std::vector<Parameter*>* parametros
            );

    Funcion ( Type* tipo
            , unsigned int alcance
            , unsigned int lineaDec
            , unsigned int columnaDec
            , unsigned int lineaDef
            , unsigned int columnaDef
            , std::vector<Parameter*>* parametros
            );

    /* Retorna un string con una representación de la instancia de la clase */
    std::string to_string();

    std::vector<Parameter*> * get_parameters() { return _parametros; };

  private:
    std::vector<Parameter*>* _parametros;
};
#endif
