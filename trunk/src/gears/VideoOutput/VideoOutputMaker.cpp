/* VideoOutputMaker.cpp
 * Copyright (C) 2004 Mathieu Guindon
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

// VideoOutputMaker.cpp: implementation of the VideoOutputMaker class.
//
//////////////////////////////////////////////////////////////////////

#include "VideoOutputMaker.h"
#include <assert.h>
#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

std::map<std::string, VideoOutputMaker*> *VideoOutputMaker::_Registry=NULL;

VideoOutputMaker::VideoOutputMaker(std::string type)
{
  static bool registry_instanciated=false;

  if (!registry_instanciated)
  {
    _Registry = new std::map<std::string, VideoOutputMaker*>();
    registry_instanciated=true;
  }

  _Registry->insert(std::make_pair(type, this));
}

VideoOutput* VideoOutputMaker::makeVideoOutput(std::string type)
{    
  if (_Registry==NULL)
    return NULL;
  
  VideoOutputMaker *VideoOutputmaker= _Registry->find(type)->second;

  if (VideoOutputmaker == NULL)
    return NULL;


  return VideoOutputmaker->internalMakeVideoOutput();
}

void VideoOutputMaker::getAllVideoOutputsType(std::vector<std::string> &videoOutputsType)
{
  if (_Registry==NULL)
    return;
  
  for (std::map<std::string, VideoOutputMaker*>::iterator it=_Registry->begin(); it != _Registry->end(); ++it)
  {
    videoOutputsType.push_back(it->first);
  }
}
