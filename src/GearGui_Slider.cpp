#include "GearGui_Slider.h"
#include "Gear_Slider.h"    

#include <qpainter.h>
#include <iostream>
#include <sstream>


const int GearGui_Slider::SLIDER_THICKNESS = 10;
const int GearGui_Slider::SLIDER_SIZEY = 20;
const int GearGui_Slider::SLIDER_SIZEX = 120;
const int GearGui_Slider::SLIDER_LINE_OFFSETY = SLIDER_SIZEY / 2;
const QColor GearGui_Slider::SLIDER_COLOR(249, 169, 7);
const QColor GearGui_Slider::SLIDER_BOX_COLOR(105, 122, 125);

GearGui_Slider::GearGui_Slider(Gear_Slider *gear, QCanvas *canvas) : 
    GearGui(gear, canvas, SLIDER_SIZEX, SLIDER_SIZEY)    
{        
}

void GearGui_Slider::drawShape(QPainter &painter)
{    
    //make the title bar show the value
    std::ostringstream sstream;
    sstream << ((Gear_Slider*)_gear)->getValue();
    setTitle(sstream.str());

    GearGui::drawShape(painter);    

    int sliderStartX, sliderStartY, sizeX, sizeY;
    getDrawableArea(&sliderStartX, &sliderStartY, &sizeX , &sizeY);

    //convert sliderpos to screen value    
    float low = _gear->settings().get(Gear_Slider::SETTING_LOWERBOUND)->valueFloat();
    float hi = _gear->settings().get(Gear_Slider::SETTING_HIGHERBOUND)->valueFloat();    
    float screenRatio = (((Gear_Slider*)_gear)->getValue()-low) / (hi-low);    
    int sliderPos = (int) ((float)(screenRatio * (float)(sizeX-SLIDER_THICKNESS)));

    //box                      
    painter.setPen(Qt::black);
    painter.setBrush(SLIDER_BOX_COLOR);
    painter.drawRect(sliderStartX, sliderStartY, sizeX, sizeY);
    painter.drawRect(sliderStartX, sliderStartY + SLIDER_LINE_OFFSETY, sizeX, 1);

    //3d
    painter.setPen(Qt::white);
    painter.drawRect(sliderStartX, sliderStartY + sizeY - 1, sizeX, 1);
    painter.drawRect(sliderStartX + sizeX - 1, sliderStartY, 1, sizeY);

    //slider
    painter.setPen(Qt::black);
    painter.setBrush(SLIDER_COLOR);
    painter.drawRoundRect(sliderStartX + sliderPos, sliderStartY, SLIDER_THICKNESS, sizeY, 85);

}

void GearGui_Slider::mouseEvent(const QPoint& p, Qt::ButtonState button)
{
    if (button != Qt::LeftButton)
        return;
    
    int sliderStartX, sliderStartY, sizeX, sizeY;
    getDrawableArea(&sliderStartX, &sliderStartY, &sizeX , &sizeY);

    //slider zone ?
    if ((p.y() < sliderStartY)
        || (p.x() > sliderStartX + sizeX - SLIDER_THICKNESS)
        || (p.x() < sliderStartX))
        return;

    moveSlider((int)p.x() - (sliderStartX));
}

void GearGui_Slider::moveSlider(int sliderPos)
{
    //convert screen sliderpos to value
    int sliderStartX, sliderStartY, sizeX, sizeY;
    getDrawableArea(&sliderStartX, &sliderStartY, &sizeX , &sizeY);

    float screenRatio = (float)sliderPos / (float)(sizeX-SLIDER_THICKNESS);
    float low = _gear->settings().get(Gear_Slider::SETTING_LOWERBOUND)->valueFloat();
    float hi = _gear->settings().get(Gear_Slider::SETTING_HIGHERBOUND)->valueFloat();
    float incAmount = (hi - low) * screenRatio;

    float value = low + incAmount;
    //by the constructor we know that it's really a Gear_Slider
    ((Gear_Slider*)_gear)->setValue(value);

    update();
    canvas()->update();
}



