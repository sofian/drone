#include "PlugBox.h"
#include "GearGui.h"
#include "Plug.h"
#include "Engine.h"
#include "ConnectionItem.h"

#include <iostream>
#include <algorithm>
#include <assert.h>

const int PlugBox::PLUGBOX_SIZE = 10;
const int PlugBox::CONNECTION_HANDLE_OFFSETX = PLUGBOX_SIZE - 2;
const int PlugBox::CONNECTION_HANDLE_OFFSETY = PLUGBOX_SIZE / 2;
const QColor PlugBox::SIGNAL_PLUG_COLOR(249,169,7);   
const QColor PlugBox::VIDEO_PLUG_COLOR(31,47,124);   
const QColor PlugBox::VIDEO_COMPOSE_PLUG_COLOR(37,142,234);   
const QFont PlugBox::SHORTNAME_FONT(QFont("system", 8, QFont::Normal));
const QFont PlugBox::SHORTNAME_FONT_BOLD(QFont("system", 8, QFont::Bold));
const int PlugBox::MAX_HILIGHTSCALING = 2;
const int PlugBox::PLUG_NAME_NB_CHARS = 6;

PlugBox::PlugBox(Plug *plug, GearGui* gearGui, Engine *engine) : 
    _plug(plug), 
    _gearGui(gearGui),     
    _engine(engine),
    _x(0), 
    _y(0), 
    _status(NORMAL), 
    _hilightScaling(0)
{
}

const QColor& PlugBox::color()
{
    switch (_plug->Type())
    {
    case SIGNAL:
        return SIGNAL_PLUG_COLOR;
        break;
    case VIDEO:
        return VIDEO_PLUG_COLOR;
        break;
    case VIDEO_COMPOSE:
        return VIDEO_COMPOSE_PLUG_COLOR;
        break;
    default:
        return SIGNAL_PLUG_COLOR;
    }
}

void PlugBox::draw(int x, int y, int gearSizeX, QPainter &painter)
{    
    int halfGearSizeX = gearSizeX / 2;    
    _x = x;
    _y = y;
   
    painter.setPen(Qt::black);
    
    //set color according to type
    switch (_plug->Type())
    {
    case SIGNAL:
        painter.setBrush(SIGNAL_PLUG_COLOR);
        break;
    case VIDEO:
        painter.setBrush(VIDEO_PLUG_COLOR);
        break;
    case VIDEO_COMPOSE:
        painter.setBrush(VIDEO_COMPOSE_PLUG_COLOR);
        break;
    default:
        painter.setBrush(SIGNAL_PLUG_COLOR);
    }
    
    if (_plug->In_Out() == IN)       
        painter.drawRect(_x, _y - _hilightScaling, PLUGBOX_SIZE + _hilightScaling*2, PLUGBOX_SIZE + _hilightScaling*2);    
    else
        painter.drawRect(_x - _hilightScaling*2, _y - _hilightScaling, PLUGBOX_SIZE + _hilightScaling*2, PLUGBOX_SIZE + _hilightScaling*2);    
    
    if (_status==HILIGHTED)       
        painter.setFont(SHORTNAME_FONT_BOLD);
    else
        painter.setFont(SHORTNAME_FONT);
    
    //align text left or right if In or Out
    if (_plug->In_Out() == IN)       
        painter.drawText(_x + PLUGBOX_SIZE + 5, _y - 4, halfGearSizeX, PLUGBOX_SIZE + 8, Qt::AlignLeft | Qt::AlignVCenter, _plug->shortName(PLUG_NAME_NB_CHARS).c_str());
    else
        painter.drawText(_x - halfGearSizeX, _y - 4, halfGearSizeX - 5, PLUGBOX_SIZE + 8, Qt::AlignRight | Qt::AlignVCenter, _plug->shortName(PLUG_NAME_NB_CHARS).c_str());

}

bool PlugBox::hitted(int x, int y)
{
    return ((x > _x) && (x < _x+PLUGBOX_SIZE) && (y > _y) && (y < _y+PLUGBOX_SIZE));
}

void PlugBox::hilight(bool hiLight)
{
    switch (_status)
    {
    case NORMAL:
        if (hiLight)
        {        
            _status = HILIGHTED;
            doHilight();
        }
        break;
    case HILIGHTED:
        if (!hiLight)
        {
            _status = NORMAL;
            doUnlight();
        }
        break;
    }                
}

void PlugBox::doHilight()
{    
    _hilightScaling = MAX_HILIGHTSCALING;
    //_gearGui->reDraw();
}

void PlugBox::doUnlight()
{ 
    _hilightScaling = 0;
    //_gearGui->reDraw();
}

/* bool PlugBox::canStartConnection()      */
/* {                                       */
/*     return _plug->canStartConnection(); */
/* }                                       */

bool PlugBox::canConnectWith(PlugBox *plugBox)
{
    if (plugBox==NULL)
        return false;    

    //we will only perform type checking, because we dont
    //we want the test to refuse multiple connection per inputs.
    //multiple connection per inputs will be handled in
    //connect where we will disconnect before creating the
    //new connection
    return _plug->canConnect(plugBox->plug(), true);        

}

bool PlugBox::connect(PlugBox *plugBox, ConnectionItem *connectionItem)
{
    if (plugBox==NULL)
        return false;
        
    //in the case of an input that is already
    //connected, we first need to disconnect
    if (_plug->In_Out() == IN && _plug->connected())
    {
        //we can only have exactly one connection per Input plug, otherwise something bad is going on
        assert(_connectionItems.size() == 1);
        //destructor of ConnectionItem take care of PlugBox disconnection
        delete _connectionItems[0];
    }
    else if (plugBox->_plug->In_Out() == IN && plugBox->_plug->connected())
    {
        //we can only have exactly one connection per Input plug, otherwise something bad is going on
        assert(plugBox->_connectionItems.size() == 1);
        //destructor of ConnectionItem take care of PlugBox disconnection
        delete plugBox->_connectionItems[0];
    }

    _connectionItems.push_back(connectionItem);
    plugBox->_connectionItems.push_back(connectionItem);

    _engine->scheduleConnection(_plug, plugBox->plug());

    return true;
}

void PlugBox::assignConnectionOnly(PlugBox *plugBox, ConnectionItem *connectionItem)
{
    _connectionItems.push_back(connectionItem);
    plugBox->_connectionItems.push_back(connectionItem);
}

void PlugBox::disconnect(PlugBox *plugBox, ConnectionItem *connectionItem)
{
    if (!_plug->connected())
        return;
    
    _connectionItems.erase(std::remove(_connectionItems.begin(), _connectionItems.end(), connectionItem), _connectionItems.end());
    plugBox->_connectionItems.erase(std::remove(plugBox->_connectionItems.begin(), plugBox->_connectionItems.end(), connectionItem), plugBox->_connectionItems.end());
    
    _engine->scheduleDisconnection(_plug, plugBox->plug());        
}

int PlugBox::connectionHandleX()
{
    if (_plug->In_Out() == IN)          
    {
        return x() + 2;
    }
    else
    {
        return x() + CONNECTION_HANDLE_OFFSETX;
    }
}

int PlugBox::connectionHandleY()
{
    return y() + CONNECTION_HANDLE_OFFSETY;
}


