#ifndef PLUG_INCLUDED
#define PLUG_INCLUDED

#include <string>
#include <list>

enum IN_OUT {IN, OUT};
enum PLUG_TYPE {SIGNAL, VIDEO, VIDEO_COMPOSE};

class Gear;

class Plug  
{    
public:
    Plug(Gear* parent, PLUG_TYPE type, IN_OUT in_out, std::string name);
	virtual ~Plug();
    
    virtual void init()=0;
    bool canStartConnection();
    bool canConnect(Plug *plug, bool onlyTypeCheck=false);
    bool connect(Plug *plug);
    bool disconnect(Plug *plug);
    void disconnectAll();

    bool connected() const { return !_ConnectedPlugs.empty();};

    virtual void onConnection(Plug*){};//!overloader pour ajouter fonctionnalites APRES une bonne connection
    virtual void onDisconnection(Plug*){};//!overloader pour ajouter fonctionnalites AVANT deconnection

    PLUG_TYPE Type() const {return _Type;};
    IN_OUT In_Out() const {return _In_Out;};
    
    int connectedPlugs(std::list<Plug*> &connectedplugs) const;
	Plug* firstConnectedPlug(){return _ConnectedPlugs.front();} 
    int nbConnectedPlugs() const {return _ConnectedPlugs.size();};
    Gear* parent() const {return _Parent;};

	std::string fullName() const;
    std::string shortName(int nbChars) const;
    std::string name() const {return _Name;};
        
protected:
                          
    std::list<Plug*> _ConnectedPlugs;    

private:
    Gear *_Parent;
    IN_OUT _In_Out;
    PLUG_TYPE _Type;
    std::string _Name;
    
};



#endif 
