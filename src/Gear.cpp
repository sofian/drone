#include "Gear.h"

#include "GearGui.h"

#include <qdom.h>


#include <iostream>
#include <sstream>

const char Gear::Category::CONTROL[] = "Control";
const char Gear::Category::AUDIO[] = "Audio";
const char Gear::Category::VIDEO[] = "Video";
const char Gear::Category::PARAMETERS_EXTRACTION[] = "parameters extraction";
const char Gear::Category::BASIC[] = "basic processing";
const char Gear::Category::IO[] = "IO";
const char Gear::Category::COLOR[] = "Color";
const char Gear::Category::_DISTORTS[] = "Distorts";
const char Gear::Category::BLUR[] = "Blur";
const char Gear::Category::EDGE[] = "Edge";


Gear::Gear(Engine *engine, std::string type, std::string name) : 
    _engine(engine), 
    _Type(type), 
    _Name(name),
    _gearGui(NULL)
{
}

Gear::~Gear()
{    
    for(std::list<Plug*>::iterator it=_Plugs.begin(); it != _Plugs.end(); ++it)
	   delete (*it);
}


void Gear::internalPrePlay()
{
    prePlay();
}

void Gear::internalPostPlay()
{
    postPlay();
}

void Gear::internalInit()
{
    std::cout << "------------------------------" << std::endl;
    std::cout << "Init : " << _Type << std::endl;
    
    for(std::list<Plug*>::iterator it=_Plugs.begin(); it != _Plugs.end(); ++it)
        (*it)->init();

    _gearGui = createGearGui(NULL);

    //call the virtual method
    init();
}

//////////////////////////////////////////////////////////////////////
//    PLUGS
//////////////////////////////////////////////////////////////////////

void Gear::deletePlug(Plug *plug)
{
    delete plug;
    _Plugs.remove(plug);
}

//todo : unique name for input and for outputs!!!
PlugSignalIn* Gear::addPlugSignalIn(std::string name, Signal_T default_value)
{    
    PlugSignalIn* newplug;
	newplug = new PlugSignalIn(this, name, default_value);
               
    _Plugs.push_back(newplug);
        
    return newplug;    
}

PlugSignalOut* Gear::addPlugSignalOut(std::string name)
{    
    PlugSignalOut* newplug;
	newplug = new PlugSignalOut(this, name);
               
    _Plugs.push_back(newplug);
        
    return newplug;    
}


PlugVideoIn* Gear::addPlugVideoIn(std::string name)
{    
    PlugVideoIn* newplug;
	newplug = new PlugVideoIn(this, name);
               
    _Plugs.push_back(newplug);
        
    return newplug;    
}

PlugVideoOut* Gear::addPlugVideoOut(std::string name)
{    
    PlugVideoOut* newplug;
	newplug = new PlugVideoOut(this, name);
               
    _Plugs.push_back(newplug);
        
    return newplug;    
}

PlugVideoComposeIn* Gear::addPlugVideoComposeIn(std::string name)
{
    PlugVideoComposeIn* newplug;
	newplug = new PlugVideoComposeIn(this, name);
               
    _Plugs.push_back(newplug);
        
    return newplug;    
}

PlugVideoComposeOut* Gear::addPlugVideoComposeOut(std::string name)
{
    PlugVideoComposeOut* newplug;
	newplug = new PlugVideoComposeOut(this, name);
               
    _Plugs.push_back(newplug);
        
    return newplug;    
}


void Gear::getInputs(std::list<Plug*> &inputs) const
{
    inputs.clear();
    for(std::list<Plug*>::const_iterator it=_Plugs.begin(); it != _Plugs.end(); ++it)
    {        
        if ( ((*it)->In_Out() == IN) )
            inputs.push_back(*it);            
    }    
}

void Gear::getOutputs(std::list<Plug*> &outputs) const
{
    outputs.clear();
    for(std::list<Plug*>::const_iterator it=_Plugs.begin(); it != _Plugs.end(); ++it)
    {        
        if ( ((*it)->In_Out() == OUT) )
            outputs.push_back(*it);            
    }
}

void Gear::getDependencies(std::vector<Gear*> &dependencies) const
{
	std::list<Plug*> inputs;
	getInputs(inputs);
	std::list<Plug*> lplug;
	for(std::list<Plug*>::const_iterator it = inputs.begin();it!=inputs.end();++it)
	{
		(*it)->connectedPlugs(lplug);
		if(lplug.size()!=0 && lplug.back()->parent()->ready())
			dependencies.push_back(lplug.back()->parent());
	}
}
	
void Gear::needSynch()
{
    _engine->needSynch();
}

GearGui* Gear::createGearGui(QCanvas *canvas)
{
    return new GearGui(this, canvas);    
}


void Gear::internalSave(QDomDocument &doc, QDomElement &parent)
{               
    QDomElement gearElem = doc.createElement("Gear");
    parent.appendChild(gearElem);
    
    QDomAttr gearType = doc.createAttribute("Type");
    gearType.setValue(_Type.c_str());
    gearElem.setAttributeNode(gearType);
    
    QDomAttr gearName = doc.createAttribute("Name");
    gearName.setValue(_Name.c_str());
    gearElem.setAttributeNode(gearName);

    _gearGui->save(doc, gearElem);
    _settings.save(doc, gearElem);
    
    save(doc, gearElem);

}

void Gear::internalLoad(QDomElement &gearElem)               
{            
    _Name = gearElem.attribute("Name","").ascii();
    
    _settings.load(gearElem);
    updateSettings();

    load(gearElem);

    _gearGui->load(gearElem);
        
}

Plug* Gear::getInput(std::string name) const
{
    std::list<Plug*> inputs;
    getInputs(inputs);
    for(std::list<Plug*>::const_iterator it = inputs.begin();it!=inputs.end();++it)    
    {
        if ((*it)->name() == name)
            return (*it);        
    }

    return NULL;
}

Plug* Gear::getOutput(std::string name) const
{
    std::list<Plug*> outputs;
    getOutputs(outputs);
    for(std::list<Plug*>::const_iterator it = outputs.begin();it!=outputs.end();++it)    
    {
        if ((*it)->name() == name)
            return (*it);        
    }

    return NULL;
}

GearGui* Gear::getGearGui()
{
    return _gearGui;
}

void Gear::updateSettings()
{
    //call the virtual method that can be overriden
    onUpdateSettings();
}
