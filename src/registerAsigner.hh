#ifndef REGISTERASIGNER_HH
#define REGISTERASIGNER_HH
#include<vector>
#include<map>
#include<boost/regex.hpp>
#include<utility>
#include"quad.hh"

using namespace std;

class RegisterAsigner
{
  public:
    RegisterAsigner(int numRegisters);
    vector<pair<bool,string>> getReg(Quad* quad);
    void                        print();
    bool getSpillMode()                     { return !_varToSpill.empty();   }
    vector<pair<int, string>> getVarToSpill();
    bool isLiteral(string op);

  private:
    void                        spill(Quad* q, vector<string> vs);
    pair<bool, int>             getRegister(string op);
    int                         findRegister();
    bool                        inRegister(string op);
    vector<pair<bool,string>>   insertAll(vector<string> vs);
    int                         cantInRegister(vector<string> vs);

    void                        cleanRegisters();
    int                         _totalRegisters;
    int                         _occupiedReg;
    map<int, vector<string>>    _register;      //Diccionario para mapear los registros con las variables contenidas en el
    map<string, int>            _variables;     //Diccinario para almacenar en que registro esta una variable
    vector<string>              _variableMod;   //Lista para almacenar todas las variables que se usaron como lvalue. 
    map<int,string>             _mipsRegisters;
    vector<pair<int, string>>   _varToSpill;
};
#endif
