#include <qpainter.h>



#include "GearGui_TV.h"
#include "Engine.h"




#include <iostream>

const int GearGui_TV::TV_SIZEX = 125;
const int GearGui_TV::TV_SIZEY = 125;
const int GearGui_TV::UPDATE_RATE_MS = 83;
const QColor GearGui_TV::NO_SIGNAL_COLOR(2, 128, 255);

GearGui_TV::GearGui_TV(Gear_TV *gear, QCanvas *canvas) : 
    GearGui(gear, canvas, TV_SIZEX, TV_SIZEY, UPDATE_RATE_MS),
    _currentSizeX(0),
    _currentSizeY(0)
{            
}

void GearGui_TV::drawShape(QPainter &painter)
{
    GearGui::drawShape(painter);

    Video_T *image;
    RGBA *data;

    int x, y, sizeX, sizeY;      
    getDrawableArea(&x, &y, &sizeX, &sizeY);

    //3d
    painter.setPen(Qt::white);
    painter.drawRect(x, y + sizeY + 1, sizeX, 1);
    painter.drawRect(x + sizeX + 1, y, 1, sizeY);

    if (((Gear_TV*)_gear)->VIDEO_IN()->connected())
    {    
        image = ((Gear_TV*)_gear)->VIDEO_IN()->canvas();
        data = image->_data;
    

        if ((image->sizeX() != _currentSizeX) || (image->sizeY() != _currentSizeY))
        {
            _videoFrame.create(image->sizeX(), image->sizeY(), 32);
            _currentSizeX = image->sizeX();
            _currentSizeY = image->sizeY();
        }
            
        unsigned char *bits = _videoFrame.bits();
    
        int iterSizeX = image->sizeX();
        int iterSizeY = image->sizeY();
                   
        for (int y1=0;y1<iterSizeY;++y1)
            for (int x1=0;x1<iterSizeX;++x1)
            {                
                (*(QRgb*)bits) = qRgb(data->r, data->g, data->b); 
                bits+=4;
                data++;
            }

        painter.drawImage(x, y, _videoFrame.scale(sizeX, sizeY));
    }
    else
    {
        painter.setPen(Qt::black);
        painter.setBrush(NO_SIGNAL_COLOR);
        painter.drawRect(x, y, sizeX, sizeY);
    }
    
}


