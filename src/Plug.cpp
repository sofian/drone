// Plug.cpp: implementation of the Plug class.
//
//////////////////////////////////////////////////////////////////////
#include <assert.h>
#include <iostream>
#include "Plug.h"

#include "Gear.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Plug::Plug(Gear* parent,  PLUG_TYPE type, IN_OUT in_out, std::string name) : 
    _Parent(parent), 
    _In_Out(in_out), 
    _Type(type),
    _Name(name)    
{
    //une plug a besoin d'un parent
    assert(parent!=NULL);
}         

Plug::~Plug()
{
    disconnectAll();
}


bool Plug::canStartConnection()
{
    //assurer seulement une connection par input
    if (_In_Out == IN)
    {
        if (!_ConnectedPlugs.empty())
            return false;
    }

    return true;
}

bool Plug::canConnect(Plug *plug, bool onlyTypeCheck)
{
    if (plug==NULL)
        return false;

    //s'assurer que plug n'est pas deja connecte a nous
    if (find(_ConnectedPlugs.begin(), _ConnectedPlugs.end(), plug) != _ConnectedPlugs.end())
        return false;
    
    //est-ce que ce sont des plugs de meme type
    if (_Type != plug->Type())
        return false;

    //avons-nous bien une connection d'un in dans un out ou vice-versa
    if (_In_Out == plug->In_Out())
        return false;
    
    //assurer seulement une connection par input
    if (!onlyTypeCheck)
    {    
        if (_In_Out == IN)
        {        
            if (!_ConnectedPlugs.empty())
                return false;            
        }
        else
            if (!plug->_ConnectedPlugs.empty())
                return false;            
    }
    return true;
}

bool Plug::connect(Plug *plug)
//! logique de connection de base
{
        
    if (!canConnect(plug))
        return false;
    
    //ajouter la nouvelle plug a nos connections
    _ConnectedPlugs.push_back(plug);        
    plug->_ConnectedPlugs.push_back(this);    
        
    _Parent->onPlugConnected(this);
    plug->_Parent->onPlugConnected(plug);
    
    //laisser la chance au class derive d'executer leur logique supplementaire
    onConnection(plug);
    plug->onConnection(this);
        
    //tell the gear that a new connection have been created and that sync is needed
    _Parent->needSynch();
    
    return true;    
}

bool Plug::disconnect(Plug *plug)
//! logique de deconnection de base
{
    if (!plug)
        return false;
    
    //on ne peut pas deconnecter une plug qui n'est pas connecte a nous
    std::list<Plug*>::iterator it = find(_ConnectedPlugs.begin(), _ConnectedPlugs.end(), plug);
    if (it == _ConnectedPlugs.end())
        return false;
    
    _Parent->onPlugDisconnected(this);
    plug->_Parent->onPlugDisconnected(plug);
    
    //laisser la chance au class derive d'executer leur logique supplementaire
    onDisconnection(plug);
    plug->onDisconnection(this);

    //remove this plug from our connections
    _ConnectedPlugs.remove(plug);    
    //remove ourself from the other plug connections
    plug->_ConnectedPlugs.remove(this);
        
    //tell the gear that a new connection have been created and that sync is needed
    _Parent->needSynch();

    return true;
}

void Plug::disconnectAll()
{
    while(!_ConnectedPlugs.empty())
    {
        disconnect(_ConnectedPlugs.back());
    }
}

int Plug::connectedPlugs(std::list<Plug*> &connectedplugs) const
{
    connectedplugs.clear();
    connectedplugs.assign(_ConnectedPlugs.begin(), _ConnectedPlugs.end());

    return connectedplugs.size();
}

std::string Plug::fullName() const
{
	return _Parent->name()+"/"+_Name;
}

std::string Plug::shortName(int nbChars) const
{
    std::string abbrev;
        
    int c=0;
    for (std::string::const_iterator it=_Name.begin();it != _Name.end() && c < nbChars; ++it, ++c)
    {
        abbrev+=*it;
    }

       
    return abbrev;
}

