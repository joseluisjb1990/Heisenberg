/**
 *  Nombre del archivo = TablaSimbolos.hh
 *
 *  Autores:
 *    Gabriela Limonta
 *    José Luis Jiménez
 *
 *  Descripción:
 *    Definición de la clase TablaSimbolos
 *    usada en bear_parser.yy para manejar
 *    la tabla de símbolos. La clase
 *    mapea variables del tipo
 *    string a instancias de la
 *    clase Contenido.
 *
 *  Última fecha de modificación:
 *    30/03/2014
 */

#ifndef TABLASIMBOLOS_HH
#define TABLASIMBOLOS_HH
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <stack>
#include <vector>
#include <queue>
#include "Contenido.hh"

class Type;
class Parameter;

using namespace std;

typedef std::multimap<std::string, Contenido*>  Diccionario;
typedef std::vector<unsigned int>               Stack;

class TablaSimbolos
{
private:

    Diccionario   _dicc;      /* Diccionario para llevar la asociación entrevariables e información sobre ésta  */
    Stack         _pila;      /* Pila de enteros para manejar los alcances activos.                             */
    Stack         _offsets;   /* Pila de enteros para manejar los alcances activos.                             */
    unsigned int  _alcance;   /* Variable para llevar los identificadores de los alcances.                      */
    void insert_symbol(std::string nombre, Contenido* cont);
    queue<unsigned int>  _alcanceFunciones;
    queue<unsigned int>  _tamFunciones;

public:

    queue<unsigned int> getAlcFunciones() { return _alcanceFunciones;  }
    queue<unsigned int> getTamFunciones() { return _tamFunciones;      }
    /**
     * Constructor para la clase.
     *
     *  Parámetros:
     *    Ninguno.
     */

    TablaSimbolos();
    void end_parameters();

    /**
     *  Agrega un símbolo nuevo a la tabla de símbolos
     *
     *  Parámetros:
     *    nombre:     nombre del símbolo que se desea agregar la tabla.
     *    tipo:       tipo del símbolo.
     *    categoria:  categoría a la que pertenece el símbolo.
     *    linea:      línea en la que fue declarado el símbolo.
     *    columna:    columna en la que fue declarado el símbolo.
     *    mut:        si el estado de la variable es mutable.
     */

    unsigned int add_symbol(  string nombre
                            , Type* tipo
                            , Categorias categoria
                            , unsigned int linea
                            , unsigned int columna
                            , bool mut
                            );

    unsigned int add_parameter(  string nombre
                                , Type* tipo
                                , Categorias categoria
                                , unsigned int linea
                                , unsigned int columna
                                , bool mut
                                );
    /**
     *  Agrega un símbolo nuevo a la tabla de símbolos.
     *  Inluyendo la línea y columna de definición.
     *
     *  Parámetros:
     *    nombre:     nombre del símbolo que se desea agregar la tabla.
     *    tipo:       tipo del símbolo.
     *    categoria:  categoría a la que pertenece el símbolo.
     *    lineaDec:   línea en la que fue declarado definido el símbolo.
     *    columnaDec: columna en la que fue declarado el tipo.
     *    lineaDef:   línea en la que fue definido el símbolo.
     *    columnaDef: columna en la que fue definido el símbolo.
     *    mut:        si el estado de la variable es mutable.
     */
    unsigned int add_symbol( string nombre
                           , Type* tipo
                           , Categorias categoria
                           , unsigned int lineaDec
                           , unsigned int columnaDec
                           , unsigned int lineaDef
                           , unsigned int columnaDef
                           , bool mut
                           );

    /**
     *  Agrega una declaración de un contenedor a la tabla de símbolos
     *
     *  Parámetros:
     *    nombre:     nombre del contenedor que se desea agregar la tabla.
     *    tipo:       tipo del contenedor.
     *    categoria:  categoría a la que pertenece el contenedor.
     *    linea:      línea en la que fue declarado el contenedor.
     *    columna:    columna en la que fue declarado el contenedor.
     */
    Contenedor*  add_container( std::string nombre
                             , Type* tipo
                             , Categorias categoria
                             , unsigned int linea
                             , unsigned int columna
                             );

    /**
     *  Agrega una definición de un contenedor a la tabla de símbolos
     *
     *  Parámetros:
     *    nombre:         nombre del contenedor que se desea agregar la tabla.
     *    tipo:           tipo del contenedor.
     *    categoria:      categoría a la que pertenece el contenedor.
     *    lineaDec:       línea en la que fue declarado el contenedor.
     *    columnaDec:     columna en la que fue declarado el contenedor.
     *    lineaDef:       línea en la que fue definido el contenedor.
     *    columnaDef:     columna en la que fue definido el contenedor.
     *    alcanceCampos:  Alcance en el que fueron agregados los campos del contenedor.
     *
     */
    Contenedor* add_container( string nombre
                             , Type* tipo
                             , Categorias categoria
                             , unsigned int lineaDec
                             , unsigned int columnaDec
                             , unsigned int lineaDef
                             , unsigned int columnaDef
                             , unsigned int alcanceCampos
                             );

    /**
     * Busca un contenedor dentro de la tabla de símbolos.
     *
     *  Parámetros:
     *    nombre: Nombre de la variable contenedor que se desea buscar.
     */
    Contenedor* find_container (string nombre);

    /**
     *  Actualiza un contenedor con su definición.
     *
     *  Parámetros:
     *    nombre:     nombre de la variable contenedor que se desea definir.
     *    tipo:       Información del tipo del contenedor(incluye información sobre los campos)
     *    lineaDef:   linea en la que fue definido el contenedor.
     *    columnaDef: columna en la que fue definido el contenedor.
     *    tipo:   Información del tipo del contenedor(incluye nombre y campos-)
     */
    Contenedor* update_container( string nombre
                                , Type* tipo
                                , unsigned int lineaDef
                                , unsigned int columnaDef
                                , unsigned int alcanceCampos
                         );

    /**
     *  Agrega información sobre la declaracipon de una función a la tabla de símbolos.
     *
     *  Parámetros:
     *    nombre:     nombre de la función.
     *    tipo:       tipo de retorno de la función.
     *    linea:      linea en la que fue declarada la función.
     *    columna:    columna en la que fue declarada la variable.
     *    parametros: vector que incluye información de los parámetros de la función.
     */
    unsigned int add_function( string nombre
                             , Type* tipo
                             , unsigned int linea
                             , unsigned int columna
                             , vector<Parameter*>* parametros
                             );

    /**
     *  Agrega información sobre la definición de una función a la tabla de símbolos.
     *
     *  Parámetros:
     *    nombre:     nombre de la función.
     *    tipo:       tipo de retorno de la función.
     *    lineaDec:   linea en la que fue declarada la función.
     *    columnaDec: columna en la que fue declarada la variable.
     *    lineaDef:   linea en la que fue declarada la función.
     *    columnaDef: columna en la que fue declarada la variable.
     *    parametros: vector que incluye información de los parámetros de la función.
     */
    unsigned int add_function( string nombre
                             , Type* tipo
                             , unsigned int lineaDec
                             , unsigned int columnaDec
                             , unsigned int lineaDef
                             , unsigned int columnaDef
                             , vector<Parameter*>* parametros
                             );

    /**
     * Función para manejar un stream con la información de la tabla.
     *
     *  Parámetros:
     *    os: El stream que se desea modificar.
     *    ts: La tabla de símbolos.
     */
    friend std::ostream& operator<<( std::ostream& os
                                   , TablaSimbolos &ts
                                   );

    /**
     * Función para agregar un alcance nuevo a la pila de alcances.
     *
     */
    unsigned int enter_scope();
    unsigned int enter_function_scope();

    /**
     * Función para sacar el alcance actual del tope de la pila.
     *
     */
    unsigned int exit_scope();
    unsigned int exit_function_scope();

    /**
     *  Hace una búsqueda solo en el alcance que está en el tope de la pila.
     *
     *  Parámetros:
     *    nombre: nombre de la variable que se desea buscar.
     */
    bool check_scope(string nombre);

    /**
     * Retorna el alcance que está en el tope de la pila.
     *
     */
    unsigned int get_actual_scope();
    unsigned int get_actual_tam();

    /**
     *  Busca una variable función en la tabla de símbolos.
     *
     *  Parámetros:
     *    nombre: Nombre de la función que se desea buscar.
     */
    Funcion* get_function(std::string nombre);

    /**
     *  Busca una variable dentro de la tabla de símbolos.
     *
     *  Parámetros:
     *    nombre: nombre del símbolo que se desea buscar.
     *    cat:    Categoria del simbolo.
     */
    Contenido* find_symbol( std::string nombre
                            , Categorias cat
                            );

    /**
     * Realiza una búsqueda por alcance.
     *
     * Parametros:
     *    nombre:   nombre del símbolo que se desea buscar.
     *    cat:      categoría del símbolo.
     *    alcance:  alcance en el que se desea buscar el símbolo.
     */
     Contenido* find_scope( std::string nombre
                          , Categorias cat
                          , unsigned int alcance
                          );

     Contenido* find_scope( std::string nombre
                          , unsigned int alcance
                          );
};
#endif
