#ifndef GEAR_INCLUDED
#define GEAR_INCLUDED

#include <string>
#include <list>
#include <vector>

#include "Engine.h"

#include "Properties.h"

#include "Timing.h"
#include "PlugSignalIn.h"
#include "PlugSignalOut.h"
#include "PlugVideoIn.h"
#include "PlugVideoOut.h"
#include "PlugVideoComposeIn.h"
#include "PlugVideoComposeOut.h"


class GearGui;
class QDomDocument;
class QDomElement;
class QCanvas;


class Gear  
{
public:
    class Category
    {
    public:
        static const char CONTROL[];
        static const char AUDIO[];
        static const char VIDEO[];
        static const char PARAMETERS_EXTRACTION[];
        static const char BASIC[];
        static const char IO[];
        static const char COLOR[];
        static const char _DISTORTS[];
        static const char BLUR[];
        static const char EDGE[];
    
        Category &operator<<(std::string s)
        {
            _category.push_back(s);
            return *this;
        }
        
        const std::vector<std::string>& path()
        {
            return _category;
        }

        void clear();
    
    private:
        std::vector<std::string> _category;
    
    };
    

    Gear(Engine* engine, std::string type, std::string name);
	virtual ~Gear();
    
    virtual void init(){};
    virtual void prePlay(){};    
    virtual void postPlay(){};    
    virtual void runAudio(){};//! test pour ready doit etre fait avant
    virtual void runVideo(){};//! test pour ready doit etre fait avant
    GearGui* getGearGui();
   
    void updateSettings();//! user must call this method after settings have been modified
     
    void getInputs(std::list<Plug*> &inputs) const;
    void getOutputs(std::list<Plug*> &outputs) const;

    Plug* getInput(std::string name) const;
    Plug* getOutput(std::string name) const;

	void getDependencies(std::vector<Gear*> & dependencies) const;
    
    const std::string& type() const {return _Type;};
	const std::string& name() const {return _Name;};

    Properties& settings(){return _settings;};
    
    virtual bool ready()=0;
    void needSynch();
    
    virtual void save(QDomDocument &, QDomElement &){};
    virtual void load(QDomElement &){};
    
            
    Engine *engine(){return _engine;};

    const Category &category(){return _category;};
    
    
protected:
    
    virtual GearGui* createGearGui(QCanvas *canvas);//! overload to create your own GearGui
    virtual void onUpdateSettings(){};
    virtual void onPlugConnected(Plug*){};
    virtual void onPlugDisconnected(Plug*){}; 
    friend bool Plug::connect(Plug *plug);
    friend bool Plug::disconnect(Plug *plug);

    PlugSignalIn* addPlugSignalIn(std::string name, Signal_T default_value=0.0f);       
    PlugSignalOut* addPlugSignalOut(std::string name);       
    PlugVideoIn* addPlugVideoIn(std::string name);       
    PlugVideoOut* addPlugVideoOut(std::string name);       
    PlugVideoComposeIn* addPlugVideoComposeIn(std::string name);       
    PlugVideoComposeOut* addPlugVideoComposeOut(std::string name);       
    
    void deletePlug(Plug *plug);
    
    Engine *_engine;

    std::list<Plug*> _Plugs;    

    Properties _settings;
    Category _category;

private:
    std::string _Type;
	std::string _Name;
    
    GearGui *_gearGui;
    
    void internalInit();
    void internalPrePlay();    
    void internalPostPlay();    
    void internalSave(QDomDocument &doc, QDomElement &parent);
    void internalLoad(QDomElement &gearElem);

    friend Gear* Engine::addGear(std::string geartype, std::string name);
    friend void *Engine::playThread(void *parent);
    friend void Engine::loadSchema(std::string filename);
    friend void Engine::saveSchema(std::string filename);
};




#endif
