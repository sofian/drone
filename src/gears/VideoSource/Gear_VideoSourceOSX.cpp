/* Gear_VideoSource.cpp
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable
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

#include <iostream>
#include "Gear_VideoSourceOSX.h"
#include "Engine.h"

#include "GearMaker.h"

extern "C" {           
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_VideoSource(schema, uniqueName);
}  
GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "VideoSource";
  gearInfo.classification = GearClassifications::video().IO().instance();
  return gearInfo;
}
}

const std::string Gear_VideoSource::SETTING_FILENAME = "Filename";

Gear_VideoSource::Gear_VideoSource(Schema *schema, std::string uniqueName) : 
  Gear(schema, "VideoSource", uniqueName),
 _sizeX(0),
 _sizeY(0),
 _movie(NULL),
 _baseAddr(NULL),
 _offscreen(NULL),
 _pixMap(NULL),
 _currentTime(0L),
 _movieDuration(0L)
{    
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOut"));
  addPlug(_AUDIO_OUT = new PlugOut<SignalType>(this, "AudioOut"));
  
  addPlug(_RESET_IN = new PlugIn<ValueType>(this, "Reset", new ValueType(0, 0, 1)));
  
  EnumType *playbackMode = new EnumType(N_PLAYBACK_MODE, LOOP);
  playbackMode->setLabel(NORMAL,"Normal");
  playbackMode->setLabel(LOOP,"Loop");
  addPlug(_MODE_IN = new PlugIn<EnumType>(this, "Mode", playbackMode));
  
  _settings.add(Property::FILENAME, SETTING_FILENAME)->valueStr("");    

	initQuickTime();
}

Gear_VideoSource::~Gear_VideoSource()
{
	freeQuickTime();
	freeMovie();
}

bool Gear_VideoSource::initQuickTime()
{
	//setup quicktime environment
	long qtVersion = 0L;
	std::cout << "initializing QuickTime" << std::endl;
	
	if(Gestalt (gestaltQuickTime, &qtVersion) != noErr)
	{
		std::cout << "The QuickTime extension is not present in this system" << std::endl;
		return false;
	}
	
	if( (qtVersion >> 16 ) < 0x400 ) 
	{
		std::cout << "QT 4.0 or higher needed" << std::endl;
		return false;
	}

	if(EnterMovies() != noErr) 
	{
		std::cout << "QuickTime EnterMovies failed" << std::endl;
		return false;
	}

	GetGWorld(&_origPort, &_origDevice);
	
	return true;
}

bool Gear_VideoSource::ready()
{
  return(_VIDEO_OUT->connected() || _AUDIO_OUT->connected());
}

void Gear_VideoSource::freeQuickTime()
{
	//set back GWorld to original value
	SetGWorld(_origPort, _origDevice); 

	//free GWorld
	if (_offscreen!=NULL)
	{
		DisposeGWorld(_offscreen);
		_offscreen=NULL;
	}
	
	_baseAddr=NULL;
	
}

void Gear_VideoSource::onUpdateSettings()
{
	freeQuickTime();
	
	if (!openMovie())
		return;

	Rect movieRect = {0, 0, 0, 0};
		
  GetMovieBox(_movie, &movieRect);
  OffsetRect(&movieRect,  -movieRect.left,  -movieRect.top);

	_sizeX = movieRect.right;
	_sizeY = movieRect.bottom;
	
  std::cout << "movie size X : " << _sizeX << std::endl;
  std::cout << "movie size Y : " << _sizeY << std::endl;	
	
  SetMovieBox(_movie, &movieRect); 
	
	if (NewGWorld(&_offscreen, k32ARGBPixelFormat, &movieRect, NULL, NULL, 0) != noErr)
	{
		std::cout << "fail to create gworld!" << std::endl;
		_offscreen=NULL;
		return;
	}
	
	_pixMap = GetGWorldPixMap(_offscreen);  
	SetMovieGWorld(_movie, _offscreen, GetGWorldDevice(_offscreen));

	if(!LockPixels(_pixMap))
	{
		std::cout << "fail to lock pixmap" << std::endl;
		return;
	}
	
	_baseAddr = (unsigned char *) GetPixBaseAddr(_pixMap);	
	_rowStride = (unsigned long) GetPixRowBytes (_pixMap);

	_currentTime=0;
	_movieDuration = GetMovieDuration(_movie);
	return;
}

void Gear_VideoSource::freeMovie()
{
	if (_movie!=NULL)
	{
		DisposeMovie(_movie);
		_movie=NULL;
	}
}

bool Gear_VideoSource::openMovie()
{
	FSSpec fsspecMovie;
	short resFile=0;
	short resID=0;
	Str255 movieName;
	Boolean wasChanged;
	
	std::cout << "opening movie : " << _settings.get(SETTING_FILENAME)->valueStr() << std::endl;
	
	freeMovie();
	
	//convert standard string path to fsspec
	if (pathToFSS(_settings.get(SETTING_FILENAME)->valueStr(), &fsspecMovie) != noErr)
	{
		std::cout << "fail to make fsspec for path : " << _settings.get(SETTING_FILENAME)->valueStr() << std::endl;
		return false;
	}
	
	//open the movie file
	if (OpenMovieFile(&fsspecMovie, &resFile, fsRdPerm) != noErr)
	{
		std::cout << "fail to open movie file!" << std::endl;
		return false;
	}

	if (NewMovieFromFile(&_movie, resFile, &resID, movieName, newMovieActive, &wasChanged) != noErr)
	{
		std::cout << "fail to load movie from file!" << std::endl;
		_movie=NULL;
		CloseMovieFile (resFile);
		return false;
	}	
	
	CloseMovieFile (resFile);
	
	return true;
}

void Gear_VideoSource::runVideo()
{
	if (_movie==NULL || _baseAddr==NULL)
		return;

	OSType mediaType = VideoMediaType;

	
	VideoRGBAType *_imageOut = _VIDEO_OUT->type();
	_imageOut->resize(_sizeX, _sizeY);
		
	UpdateMovie(_movie); 
	MoviesTask(_movie, 0);

  unsigned char *tempimg = (unsigned char*)_imageOut->data();
  unsigned char *row; 
  for(int y=0; y<_sizeY; y++)
	{
	  row = _baseAddr + y*_rowStride;
		for(int x=0; x<_sizeX; x++)
	  {
			row++;
			*(tempimg++) = *(row++);
			*(tempimg++) = *(row++);
			*(tempimg++) = *(row++);
			tempimg++;
	  }
	}
		
	//handle looping and reset
  if ((int)_RESET_IN->type()->value() == 1 ||
      ((ePlaybackMode)_MODE_IN->type()->value() == LOOP && _currentTime == -1))
		_currentTime=0L;
	else	
		//move to next frame
		GetMovieNextInterestingTime(_movie, nextTimeMediaSample, 1, &mediaType, _currentTime, 0, &_currentTime, NULL);
	
	SetMovieTimeValue(_movie, _currentTime);
}

void Gear_VideoSource::runAudio()
{
}

int Gear_VideoSource::pathToFSS(const std::string path, FSSpec *fss)
{
	FSRef fsref;
	
	FSPathMakeRef((const UInt8*)path.c_str(), &fsref, 0); 
	
	return FSGetCatalogInfo(&fsref, kFSCatInfoNone, NULL, NULL, fss, NULL);
}

void Gear_VideoSource::prePlay()
{
	std::cout << " !!!!" << std::endl;
}    

void Gear_VideoSource::postPlay()
{
}    


