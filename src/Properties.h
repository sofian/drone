#ifndef PROPERTIES_INCLUDED
#define PROPERTIES_INCLUDED

#include <map>
#include <string>
#include <vector>

class QDomDocument;
class QDomElement;

class Property
{
public:
  enum eType
  {
    STRING, FILENAME, INT, FLOAT, BOOL
  };

  Property()
  {
  }

  Property(eType type, std::string name) : _type(type), _name(name)
  {
  }

  void valueStr(std::string value) {_value = value;};
  void valueInt(int value);
  void valueFloat(float value);
  void valueBool(bool value);

  std::string valueStr() {return _value;};
  int valueInt();
  float valueFloat();
  bool valueBool();

  eType type(){return _type;};
  std::string name(){return _name;};
  void name(std::string name){_name = name;};



private:
  eType _type;
  std::string _name;
  std::string _value;

};

class Properties
{
public:

  Property* add(Property::eType type, std::string name);

  Property* get(std::string name);

  void getAll(std::vector<Property*> *properties);

  void save(QDomDocument &doc, QDomElement &parent);
  void load(QDomElement &parentElem);

private:
  static const char WHITESPACE_REPLACEMENT;
  std::map<std::string, Property*> _properties;


};

#endif
