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
    enum eType {STRING, FILENAME, INT, FLOAT};
    
    Property()
    {
    }

    Property(eType type, std::string name) : _type(type), _name(name)
    {
    }
    
    std::string value() {return _value;};
    void value(std::string value) {_value = value;};
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

    void add(Property::eType type, std::string name, std::string default_value);
    void add(Property::eType type, std::string name, int default_value);
    void add(Property::eType type, std::string name, float default_value);

    void set(std::string name, std::string value);
    void set(std::string name, int value);
    void set(std::string name, float value);
    
    std::string get(std::string name);
    int getInt(std::string name);
    float getFloat(std::string name);

    void getAll(std::vector<Property*> *properties);

    void save(QDomDocument &doc, QDomElement &parent);
    void load(QDomElement &parentElem);

private:
    static const char WHITESPACE_REPLACEMENT;
    std::map<std::string, Property*> _properties;

    
};

#endif
