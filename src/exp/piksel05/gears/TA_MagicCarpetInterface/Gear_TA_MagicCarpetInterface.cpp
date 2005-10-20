/* Gear_TA_MagicCarpetInterface.cpp
 * Copyright (C) 2005 Jean-Sebastien Senecal
 * This file is part of Drone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
//inspired from Using libavformat and libavcodec by Martin Böhme (boehme@inb.uni-luebeckREMOVETHIS.de) 

#include <iostream>
#include "Gear_TA_MagicCarpetInterface.h"
#include "Engine.h"
#include "Math.h"

#include "GearMaker.h"


#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>

void bin_prnt_byte(int x)
{
   int n;
   for(n=0; n<8; n++)
   {
      if((x & 0x80) !=0)
      {
         printf("1");
      }
      else
      {
         printf("0");
      }
      if (n==3)
      {
         printf(" "); /* insert a space between nybbles */
      }
      x = x<<1;
   }
}


extern "C" {           
  Gear* makeGear(Schema *schema, std::string uniqueName)
  {
    return new Gear_TA_MagicCarpetInterface(schema, uniqueName);
  }  
  GearInfo getGearInfo()
  {
    GearInfo gearInfo;
    gearInfo.name = "TA_MagicCarpetInterface";
    gearInfo.classification = GearClassifications::unclassified().instance();
    return gearInfo;
  }
}

Gear_TA_MagicCarpetInterface::Gear_TA_MagicCarpetInterface(Schema *schema, std::string uniqueName) : 
Gear(schema, "TA_MagicCarpetInterface", uniqueName),
_pos(0)
{
  addPlug(_GRID_OUT = new PlugOut<VideoChannelType>(this, "GridOut", false));
  addPlug(_IMG_OUT = new PlugOut<VideoRGBAType>(this, "ImgOut", false));
  socket = new QSocket();

}

Gear_TA_MagicCarpetInterface::~Gear_TA_MagicCarpetInterface()
{
  delete socket;
}

void Gear_TA_MagicCarpetInterface::internalPrePlay()
{
  //  std::cout<<"!!!!!!!!!!trying to connect to tite patente"<<std::endl;
  socket->connectToHost("192.168.255.1",10001);
  QTextStream ts(socket);
  ts<<"hello\n";
	char eat[1024];
	int bufsize  = socket->bytesAvailable();
	if (bufsize)
		socket->readBlock(eat, bufsize);
	_pos=0;
	_bytesNeeded=0;
	_carpetBuffer[0]=1;
}

void Gear_TA_MagicCarpetInterface::runVideo()
{
  VideoChannelType *grid = _GRID_OUT->type();
  VideoRGBAType *img = _IMG_OUT->type();
  grid->resize(8,8);
  img->resize(8,8);

	//std::cout << (int)socket->bytesAvailable() << std::endl;

	if (!_bytesNeeded)
	{
		while(socket->bytesAvailable() && (*_carpetBuffer)!='!')
		{
			socket->readBlock(_carpetBuffer, 1);
		}
		
		if ((*_carpetBuffer)!='!')
			return;		
			
		_bytesNeeded=64;	
	}
		
	int bufsize  = socket->bytesAvailable();
	_pos = 65 - _bytesNeeded;
	
	if (bufsize > _bytesNeeded)
		bufsize=_bytesNeeded;

	if (bufsize)
		socket->readBlock(&_carpetBuffer[_pos], bufsize);
	else
		return;
	
	_bytesNeeded-=bufsize;
	if (_bytesNeeded!=0)
		return;
	else
		_carpetBuffer[0]=1;
		
		
	//for(int y=0;y<8;y++)
	//	{
	//for(int x=0;x<8;x++)
	//	std::cout << _carpetBuffer[1+(y*8) + x];
	
	//std::cout << std::endl;					
	//}	
	
	std::cout << std::endl;					
	std::cout << std::endl;					

  for (int x=0, xInv=grid->width()-1; x<grid->width(); ++x, --xInv)
  {
  	for (int y=0, yInv=grid->height()-1; y<grid->height(); ++y, --yInv)
		{ 
	  	(*grid)(x,y)=(_carpetBuffer[1+(yInv*8)+xInv]=='1')?0:1; // point activated
	  	if((*grid)(x,y))
	    	(*img)(x,y)=RGBA(100,200,100,255);
	  	else
		  	(*img)(x,y)=RGBA(0,0,0,255);
	  
		}
  }
}

void Gear_TA_MagicCarpetInterface::internalPostPlay()
{
  socket->close();
}

