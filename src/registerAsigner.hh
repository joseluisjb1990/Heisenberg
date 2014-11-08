#include<vector>
#include<map>
#include<utility>
#include"quad.hh"

using namespace std;

class RegisterAsigner
{
  public:
    RegisterAsigner(int numRegisters);
    pair<int,int> getReg(Quad* quad, set<string> liveVariables);

  private:
    int                         _numRegisters;
    int                         _totalRegisters;
    map<int, vector<string>>    _register;      //Diccionario para mapear los registros con las variables contenidas en el
    map<string, int>            _variables;     //Diccinario para almacenar en que registro esta una variable
    vector<string>              _variableMod;   //Lista para almacenar todas las variables que se usaron como lvalue. 
};
