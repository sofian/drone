#ifndef GEAR_SLIDER_INCLUDED
#define GEAR_SLIDER_INCLUDED

#include "Gear.h"


class Gear_Slider : public Gear  
{
public:
	static const Signal_T DEFAULT_VALUE;

    static const std::string SETTING_LOWERBOUND;
    static const std::string SETTING_HIGHERBOUND;

    Gear_Slider(Engine *engine, std::string name);
	virtual ~Gear_Slider();

    void save(QDomDocument &doc, QDomElement &gearElem);
    void load(QDomElement &gearElem);
    
    void runAudio();

    PlugSignalOut* VALUE_OUT(){return _VALUE_OUT;};

    void setValue(Signal_T value);
    Signal_T getValue(){return _VALUE_OUT->buffer()[0];};
    

    bool ready();

protected:
    GearGui* createGearGui(QCanvas *canvas);//we want a slider so we overload
    void onUpdateSettings();

private:
    PlugSignalOut* _VALUE_OUT;	
};

#endif 
