#ifndef GEAR_MAKER_INCLUDED
#define GEAR_MAKER_INCLUDED

#include <map>
#include <string>
#include <vector>

class Engine;
class Gear;

class GearMaker  
{
public:
    GearMaker(std::string type);
	
    static Gear* makeGear(Engine *engine, std::string type, std::string name);
    static void getAllGearsName(std::vector<std::string> &gearsName);
private:
    static std::map<std::string, GearMaker*> *_Registry;
    virtual Gear* internalMakeGear(Engine *engine, std::string name)=0;
};

#define Register_Gear(MMAKERNAME, MCLASSNAME, MTYPE) \
class MMAKERNAME : public GearMaker \
{ \
public: \
    MMAKERNAME() : GearMaker(MTYPE){}; \
private: \
    Gear* internalMakeGear(Engine *engine, std::string name) \
    { \
        return new MCLASSNAME(engine, name);\
    } \
    static MMAKERNAME _RegisterThis;\
};\
MMAKERNAME MMAKERNAME::_RegisterThis;



#endif 
