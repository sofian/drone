#include "GearGui.h"
#include "Gear.h"   
#include "Engine.h"
#include "PlugBox.h"

#include <iostream>
#include <sstream>

#include <qpainter.h>
#include <qdom.h>

const int GearGui::CANVAS_RTTI_GEAR = 2000;
const int GearGui::DEFAULT_SIZEX = 120;

const int GearGui::NAME_SIZEY = 20;
const int GearGui::PLUGBOXES_NOMINAL_INTERVAL = 16;
const int GearGui::PLUGBOXES_STARTING_OFFSET = PLUGBOXES_NOMINAL_INTERVAL / 2;
const int GearGui::SHADOW_OFFSET = 4;

const QColor GearGui::BOX_COLOR(207,207,209);
const QColor GearGui::SHADOW_COLOR(87,102,125);
const QColor GearGui::BOXNAME_COLOR(0,31,68);
const QFont GearGui::NAME_FONT("system", 8, QFont::Bold);


GearGui::GearGui(Gear *pgear, QCanvas *canvas, int sizeX, int sizeY, int updateRate) :
    QCanvasRectangle(canvas),
    QObject(),
    _gear(pgear),
    _sizeX(sizeX),
    _sizeY(0),
    _inputsInterval(0),
    _outputsInterval(0),
    _title(pgear->name())
{
    
    if (updateRate>=0)
        startTimer(updateRate);
        
    //create plugboxes
    PlugBox *plugBox;
    std::list<AbstractPlug*> inputs;
    _gear->getInputs(inputs);

    for (std::list<AbstractPlug*>::iterator it = inputs.begin(); it != inputs.end(); ++it)
    {
        plugBox = new PlugBox(*it, this, _gear->engine());
        _plugBoxes.push_back(plugBox);
        _inputPlugBoxes.push_back(plugBox);
    }


    std::list<AbstractPlug*> outputs;
    _gear->getOutputs(outputs);

    for (std::list<AbstractPlug*>::iterator it = outputs.begin(); it != outputs.end(); ++it)
    {
        plugBox = new PlugBox(*it, this, _gear->engine());
        _plugBoxes.push_back(plugBox);
        _outputPlugBoxes.push_back(plugBox);
    }


    //calculate _sizeY for plugboxes fitting
    int maxPlugs = _inputPlugBoxes.size() > _outputPlugBoxes.size() ? _inputPlugBoxes.size() : _outputPlugBoxes.size();

    _sizeY = ((maxPlugs+1) * PLUGBOXES_NOMINAL_INTERVAL) + NAME_SIZEY;
    
    //fit to desired sizeY
    if (_sizeY < sizeY)
        _sizeY = sizeY;
   
    if (_inputPlugBoxes.size())
        _inputsInterval = (_sizeY - NAME_SIZEY) / (_inputPlugBoxes.size()+1);
    else
        _inputsInterval = 0;

    if (_outputPlugBoxes.size())
        _outputsInterval = (_sizeY - NAME_SIZEY) / (_outputPlugBoxes.size()+1);
    else
        _outputsInterval = 0;

    //now set the size for this QCanvasItem
    setSize(_sizeX + SHADOW_OFFSET, _sizeY + SHADOW_OFFSET);
    

}

GearGui::~GearGui()
{
    //delete all the plugBoxes
    //plugboxes take care of deleting connectionItems when deleted
    //Everything is taken care of ! :)
    for (std::vector<PlugBox*>::iterator it = _plugBoxes.begin(); it != _plugBoxes.end(); ++it)
    {
        delete (*it);
    }
    
}

void GearGui::getDrawableArea(int *ox, int *oy, int *sizeX, int *sizeY)
{

    *ox = (int)x() + PlugBox::PLUGBOX_SIZE + 4;
    *oy = (int)y() + GearGui::NAME_SIZEY + 4;
    *sizeX = _sizeX - (PlugBox::PLUGBOX_SIZE+4)*2;
    *sizeY = _sizeY - GearGui::NAME_SIZEY - 10;            
}
                        
bool GearGui::titleBarHitted(const QPoint& p)
{
    return (p.y() < y() + NAME_SIZEY);    
}

void GearGui::drawPlugBoxes(QPainter &painter)
{
    int startX = (int)x();
    int startY = (int)y() + NAME_SIZEY;
    int by=0;
    int bx=0;

    if (_inputPlugBoxes.size())
    {
        by=startY + _inputsInterval - PLUGBOXES_STARTING_OFFSET;
        for (std::vector<PlugBox*>::iterator it = _inputPlugBoxes.begin(); it != _inputPlugBoxes.end(); ++it)
        {
            (*it)->draw(startX, by, _sizeX, painter);
            by+=_inputsInterval;
        }
    }

    if (_outputPlugBoxes.size())
    {
        by=startY + _outputsInterval - PLUGBOXES_STARTING_OFFSET;
        bx=startX + _sizeX - PlugBox::PLUGBOX_SIZE;
        for (std::vector<PlugBox*>::iterator it = _outputPlugBoxes.begin(); it != _outputPlugBoxes.end(); ++it)
        {
            (*it)->draw(bx, by, _sizeX, painter);
            by+=_outputsInterval;
        }
    }

}

void GearGui::drawShape(QPainter &painter)
{
    int startX = (int)x();
    int startY = (int)y();
    
   
    //shadow
    painter.setPen(Qt::NoPen);
    painter.setBrush(SHADOW_COLOR);
    //painter.setBrush(Qt::Dense4Pattern);
    painter.drawRoundRect(startX + SHADOW_OFFSET, startY + SHADOW_OFFSET, _sizeX, _sizeY);

    //box
    painter.setPen(Qt::black);
    painter.setBrush(BOX_COLOR);
    painter.drawRoundRect(startX, startY, _sizeX, _sizeY);

    //name
    painter.setPen(Qt::black);
    painter.setBrush(BOXNAME_COLOR);
    painter.drawRect(startX, startY, _sizeX, NAME_SIZEY);
    painter.setFont(NAME_FONT);
    painter.setPen(Qt::white);
    painter.drawText(startX, startY, _sizeX, NAME_SIZEY, Qt::AlignHCenter | Qt::AlignVCenter, _title.c_str());

    //plugs
    drawPlugBoxes(painter);

}

PlugBox* GearGui::plugHitted(const QPoint &p)
{
    for (std::vector<PlugBox*>::iterator it = _plugBoxes.begin(); it != _plugBoxes.end(); ++it)
    {
        if ((*it)->hitted(p.x(), p.y()))
            return (*it);
    }

    return NULL;
}

void GearGui::performPlugHighligthing(const QPoint &p, PlugBox *onlyCompatibleWith)
{
    PlugBox *plugbox;


    for (std::vector<PlugBox*>::iterator it = _plugBoxes.begin(); it != _plugBoxes.end(); ++it)
    {
        plugbox = *it;

        if (plugbox->hitted(p.x(), p.y()))
        {
            if (onlyCompatibleWith!=NULL) //perform compatibility test?
            {
                if (plugbox->canConnectWith(onlyCompatibleWith))
                    plugbox->hilight(true);
                else
                    plugbox->hilight(false);
            }
            else
                plugbox->hilight(true);

        }
        else
            plugbox->hilight(false);
    }

    //we need to repaint ourself
    reDraw();
}

void GearGui::save(QDomDocument &doc, QDomElement &gearElem)                        
{                                                                                                                       
    QDomAttr gearPosX = doc.createAttribute("PosX");
    std::ostringstream strX;
    strX << x();
    gearPosX.setValue(strX.str().c_str());
    gearElem.setAttributeNode(gearPosX);

    QDomAttr gearPosY = doc.createAttribute("PosY");
    std::ostringstream strY;
    strY << y();
    gearPosY.setValue(strY.str().c_str());
    gearElem.setAttributeNode(gearPosY);

}                                                                                                                       

void GearGui::load(QDomElement &gearElem)                        
{
    setX(gearElem.attribute("PosX","").toInt());
    setY(gearElem.attribute("PosY","").toInt());
}


PlugBox* GearGui::getInputPlugBox(std::string name) const
{
    for(std::vector<PlugBox*>::const_iterator it = _inputPlugBoxes.begin();it!=_inputPlugBoxes.end();++it)    
    {
        if ((*it)->plug()->name() == name)
            return (*it);        
    }

    return NULL;
}

PlugBox* GearGui::getOutputPlugBox(std::string name) const
{
    for(std::vector<PlugBox*>::const_iterator it = _outputPlugBoxes.begin();it!=_outputPlugBoxes.end();++it)    
    {
        if ((*it)->plug()->name() == name)
            return (*it);        
    }

    return NULL;
}

int GearGui::rtti() const
{
    return CANVAS_RTTI_GEAR;
}

void GearGui::timerEvent(QTimerEvent*)
{
    //cannot update if not on a canvas
    if (canvas()==NULL)
        return;
    
    reDraw();
}

void GearGui::reDraw()
{
    if (canvas()!=NULL)
    {        
        update();    
        canvas()->update();    
    }
}
