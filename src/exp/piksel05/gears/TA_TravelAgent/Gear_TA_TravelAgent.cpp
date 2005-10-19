/* Gear_TA_TravelAgent.cpp
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
#include "Gear_TA_TravelAgent.h"
#include "Engine.h"
#include "Math.h"

#include "GearMaker.h"

extern "C" {           
  Gear* makeGear(Schema *schema, std::string uniqueName)
  {
    return new Gear_TA_TravelAgent(schema, uniqueName);
  }  
  GearInfo getGearInfo()
  {
    GearInfo gearInfo;
    gearInfo.name = "TA_TravelAgent";
    gearInfo.classification = GearClassifications::unclassified().instance();
    return gearInfo;
  }
}

const std::string Gear_TA_TravelAgent::SETTING_FILENAME = "Filename";

Gear_TA_TravelAgent::Gear_TA_TravelAgent(Schema *schema, std::string uniqueName) : 
  Gear(schema, "TA_TravelAgent", uniqueName)
{
  //  addPlug(_TA_DATA_IN = new PlugIn<TA_DataType>(this, "DataIn", false));
  addPlug(_GRID_IN = new PlugIn<VideoChannelType>(this, "GridIn", false));
  
  addPlug(_ENERGY_DECAY = new PlugIn<ValueType>(this, "E-Decay", false, new ValueType(0, 0, 1)));
  addPlug(_ENERGY_CONSUMPTION = new PlugIn<ValueType>(this, "E-Consum", false, new ValueType(1, 0, 1)));
  addPlug(_ENERGY_GRID = new PlugIn<ValueType>(this, "E-Grid", false, new ValueType(1, 0, 1)));
  addPlug(_MOVE_ALLOWED = new PlugIn<ValueType>(this, "MoveOK", false, new ValueType(1, 0, 1)));
  addPlug(_NEXT_SCENE = new PlugIn<ValueType>(this, "NextScene", false, new ValueType(1, 0, 1)));

  addPlug(_TA_DATA_OUT = new PlugOut<TA_DataType>(this, "DataOut", false));
  addPlug(_CURRENT_SPOT_OUT = new PlugOut<ValueType>(this, "CurrSpot", false));
  addPlug(_CURRENT_SCENE_OUT = new PlugOut<ValueType>(this, "CurrScene", false));
  
  _settings.add(Property::FILENAME, SETTING_FILENAME)->valueStr("");    
  _TA_DATA_OUT->sleeping(true);

  _currentSpot = -1;
  _currentScene = -1;
}

Gear_TA_TravelAgent::~Gear_TA_TravelAgent()
{
}

void Gear_TA_TravelAgent::onUpdateSettings()
{
  std::cout << "opening file : " << _settings.get(SETTING_FILENAME)->valueStr().c_str() << std::endl;

  TA_DataType *graph = _TA_DATA_OUT->type();
    
  graph->load(_settings.get(SETTING_FILENAME)->valueStr());
  graph->printDebug();
  _currentSpot = graph->begin()->first;
  _currentScene = graph->begin()->second.getCurrentScene();
  _TA_DATA_OUT->sleeping(false);
}

void Gear_TA_TravelAgent::runVideo()
{
  TA_DataType *graph = _TA_DATA_OUT->type();
  VideoChannelType *grid = _GRID_IN->type();
  TA_CityVertex& v = (*graph)[_currentSpot];

  bool nextScene = _NEXT_SCENE->type()->boolValue();
  
  // Dummy agent, just goes from one point to the other, consuming all the energy there.
  if (v.energy <= 0)
    {
      if (_MOVE_ALLOWED->type()->boolValue())
	{
	  // Change next clip of this spot for the next time the agent comes here.
	  v.nextScene();
      
	  // Choose next spot.
	  std::set<int> neighbors = graph->neighbors(_currentSpot);
	  float maxEnergy = -1000000.0f;
	  int next = _currentSpot;
	  for (std::set<int>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
	    {
	      if ((*graph)[*it].energy > maxEnergy)
		{
		  maxEnergy = (*graph)[*it].energy;
		  next = *it;
		}
	    }
	  _currentSpot = next;
	}
      else if (nextScene)
	v.nextScene();
    }
  else
    {
      if (nextScene)
	v.nextScene();
      v.energy -= _ENERGY_CONSUMPTION->type()->value();    
    }

  // Decay energy of every point.
  for (TA_DataType::iterator it = graph->begin(); it != graph->end(); ++it)
    it->second.energy -= _ENERGY_DECAY->type()->value();

  Array<float> energy(graph->size());
  energy.fill(0.0f);
  int total_activated_points = 0;
  for (int x=0; x<grid->width(); ++x)
    {
      for (int y=0; y<grid->height(); ++y)
	{
	  if ((*grid)(x,y)) // point activated
	    {
	      total_activated_points++;
        
	      TA_Point centroid((float)x / (float)grid->width() * graph->xSize + graph->xOrigin,
				(float)y / (float)grid->height() * graph->ySize + graph->yOrigin);
	      float sum = 0;
	      int i = 0;
	      float dist;
	      for (TA_DataType::iterator it = graph->begin(); it != graph->end(); ++it, ++i)
		{
		  dist = distance( centroid, it->second );
		  dist *= dist;
		  //	  dist *= dist;
		  //dist *= dist;
		  energy[i] = 1.0f / dist;
		  sum += energy[i];
		}
	      i = 0;

	      float factor = 1.0f / sum;
	      for (TA_DataType::iterator it = graph->begin(); it != graph->end(); ++it, ++i)
		energy[i] *= factor;
	    }
	}
    }

  // Global energy inputed by grid equals one at all time.
  if (total_activated_points > 0)
    {
      float mult = _ENERGY_GRID->type()->value() / (float)total_activated_points;
      for (int i=0; i<energy.size(); ++i)
	energy[i] *= mult;
    }

  int i=0;
  for (TA_DataType::iterator it = graph->begin(); it != graph->end(); ++it, ++i)
    it->second.energy += energy[i];

  for (TA_DataType::iterator it = graph->begin(); it != graph->end(); ++it)
    it->second.energy = CLAMP(it->second.energy, 0.0f, 45.0f); // lower bound at zero, higher at 45 secs

  //   if (_MOVE_ALLOWED->type()->boolValue())
  //   {
  //     NOTICE("Switch %d ...", _currentSpot);
  //     _currentSpot = (_currentSpot == 1 ? 2 : 1); // swap
  //     NOTICE("...to %d", _currentSpot);
  //   }
  //  graph->printDebug();
  _CURRENT_SPOT_OUT->type()->setValue(_currentSpot);
  _currentScene = (*graph)[_currentSpot].getCurrentScene();
  NOTICE("current scene: %d.", _currentScene);
  _CURRENT_SCENE_OUT->type()->setValue(_currentScene);
}



