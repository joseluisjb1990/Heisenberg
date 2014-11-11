/**
 *  Nombre del archivo = type.cc
 *
 *  Autores:
 *    Gabriela Limonta
 *    José Luis Jiménez
 *
 *  Descripción:
 *    Implementación de las funciones de la clase
 *    Type definida en el archivo
 *    type.hh
 *
 *  Última fecha de modificación:
 *    30/03/2014
 */

#ifndef TYPE_CC
#define TYPE_CC
#include "type.hh"
#include "expresion.hh"

using namespace std;

Type::Type(int tam, unsigned int align)
  : _tam   (tam)
  , _align (align)
  {}

std::string Type::to_string()
{
  return "tipo";
}

std::string Type::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "type";
}

PandaType::PandaType()
  : Type(1, 2)
  {}

PandaType* PandaType::_instance = 0;

bool PandaType::compareTypes (Type* t2)
{
  return t2 == getInstance();
}

std::string PandaType::to_string()
{
  return "panda";
}

std::string PandaType::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "panda";
}

bool PandaType::isSimple()
{
  return true;
}

PolarType::PolarType()
  : Type(4, 4)
  {}

std::string PolarType::to_string()
{
  return "polar";
}

std::string PolarType::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "polar";
}

bool PolarType::isSimple()
{
  return true;
}

PolarType* PolarType::_instance = 0;

bool PolarType::compareTypes (Type* t2)
{
  return t2 == getInstance();
}

NumType::NumType()
  : PolarType()
{}

NumType* NumType::_instance = 0;

KodiakType::KodiakType()
  : Type(8,8)
  {}

std::string KodiakType::to_string()
{
  return "kodiak";
}

std::string KodiakType::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "kodiak";
}

bool KodiakType::isSimple()
{
  return true;
}

KodiakType* KodiakType::_instance = 0;

bool KodiakType::compareTypes (Type* t2)
{
  return t2 == getInstance();
}

ConstFloat::ConstFloat()
  : KodiakType()
{}

ConstFloat* ConstFloat::_instance = 0;

MalayoType::MalayoType()
  : Type(1, 2)
  {}

std::string MalayoType::to_string()
{
  return "malayo";
}

std::string MalayoType::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "malayo";
}

bool MalayoType::isSimple()
{
  return true;
}

MalayoType* MalayoType::_instance = 0;

bool MalayoType::compareTypes (Type* t2)
{
  return t2 == getInstance();
}

ExtintoType::ExtintoType()
  : Type(0, 0)
  {}

std::string ExtintoType::to_string()
{
  return "extinto";
}

std::string ExtintoType::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "extinto";
}

ExtintoType* ExtintoType::_instance = 0;

bool ExtintoType::isSimple()
{
  return true;
}

bool ExtintoType::compareTypes (Type* t2)
{
  return t2 == getInstance();
}

HormigueroType::HormigueroType()
  : Type(1,2)
  {}

std::string HormigueroType::to_string()
{
  return "hormiguero";
}

std::string HormigueroType::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "hormiguero";
}
bool HormigueroType::isSimple()
{
  return true;
}

bool HormigueroType::compareTypes (Type* t2)
{
  return dynamic_cast<HormigueroType*> (t2);
}

bool HormigueroType::isHormiguero() { return true; }

CuevaType::CuevaType(Type* tipo, int longitud)
  : Type(tipo->getSize() * longitud, tipo->getAlign())
  , _tipo     ( tipo     )
  , _longitud ( longitud )
  {}

unsigned int CuevaType::getSize()
{
  return Type::getSize();
}

bool CuevaType::compareTypes (Type* t2)
{
  return this == t2;
}

std::string CuevaType::to_string()
{
  std::string str = _tipo->to_string() + "[" + std::to_string(_longitud) + "]";
  return str;
}

std::string CuevaType::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "cueva [" + std::to_string(_longitud) + "] de " + _tipo->to_string();
}

bool CuevaType::compareStructure(Type* type)
{
  if(type->isArray())
  {
    return _tipo->compareStructure(((CuevaType*)type)->getTipo());
  } else
  {
    return false;
  }
}

int CuevaType::getLongitud()
{
  return _longitud;
};

bool CuevaType::isSimple()
{
  return true;
}

Type* CuevaType::getTipo()
{
  return _tipo;
}

CampoType::CampoType(Type* tipo, std::string nombre)
  : Type(tipo->getSize(),tipo->getAlign())
  , _tipo   ( tipo   )
  , _nombre ( nombre )
  {}

std::string CampoType::to_string()
{
  return _tipo->to_string() + " " + _nombre;
}

std::string CampoType::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + _tipo->to_string() + " " + _nombre;
}

unsigned int CampoType::getSize()
{
  return Type::getSize();
}

bool CampoType::isSimple()
{
  return true;
}

bool CampoType::compareTypes (Type* t2)
{
  return dynamic_cast<CampoType*> (t2);
}

PardoType::PardoType(std::vector<Type*>* campos, std::string nombre, unsigned int tamanio)
  : Type(tamanio,campos->at(0)->getAlign())
  , _campos ( campos )
  , _nombre ( nombre )
  {}

PardoType::PardoType(std::string nombre)
  : Type(0,1)
  , _nombre ( nombre )
  {}

std::string PardoType::to_string()
{
  return "pardo " + _nombre;
}

std::string PardoType::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "pardo" + _nombre;
}
unsigned int PardoType::getSize()
{
  return Type::getSize();
}

bool PardoType::compareTypes (Type* t2)
{
  return dynamic_cast<PardoType*> (t2);
}

bool PardoType::isSimple()
{
  return false;
}

std::string PardoType::getName()
{
  return _nombre;
}

GrizzliType::GrizzliType(std::vector<Type*>* campos, std::string nombre)
  : Type(0,1)
  , _campos ( campos )
  , _nombre ( nombre )
  { Type* maxType = max(); setSize(maxType->getSize()); setAlign(maxType->getAlign()); }

GrizzliType::GrizzliType(std::string nombre)
  : Type(0,1)
  , _nombre ( nombre )
  {}

std::string GrizzliType::to_string()
{
  return "grizzli " + _nombre;
}

std::string GrizzliType::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "grizzli " + _nombre;
}
unsigned int GrizzliType::getSize()
{
  return Type::getSize();
}

Type* GrizzliType::max()
{
  unsigned int max = 0;
  Type* maxType;
  Type* auxType;

  for (unsigned int i=0; i < _campos->size(); ++i) {
    auxType = _campos->at(i);

    if (max < auxType->getSize())
      max     = auxType->getSize();
      maxType = auxType;

  }
  return maxType;
}

bool GrizzliType::isSimple()
{
  return false;
}

std::string GrizzliType::getName()
{
  return _nombre;
}

bool GrizzliType::compareTypes (Type* t2)
{
  return dynamic_cast<GrizzliType*> (t2);
}

ErrorType::ErrorType()
  : Type(0,1)
  {}

std::string ErrorType::to_string()
{
  return "error";
}

std::string ErrorType::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "error";
}

bool ErrorType::isSimple()
{
  return true;
}

ErrorType* ErrorType::_instance = 0;

bool ErrorType::compareTypes (Type* t2)
{
  return false;
}


TagType::TagType()
  : Type(4,4)
  {}

std::string TagType::to_string()
{
  return "etiqueta";
}

std::string TagType::to_string(int nesting)
{
  std::string padding(nesting*2, ' ');
  return padding + "etiqueta";
}
bool TagType::isSimple()
{
  return true;
}

bool TagType::compareTypes (Type* t2)
{
  return dynamic_cast<TagType*> (t2);
}
#endif
