/* GearMaker.h
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

#ifndef GEAR_MAKER_INCLUDED
#define GEAR_MAKER_INCLUDED

#include <map>
#include <string>
#include <vector>

class Engine;
class Gear;

class GearMaker  
{
public:
  GearMaker(std::string type);

  static Gear* makeGear(Engine *engine, std::string type, std::string name);
  static void getAllGearsName(std::vector<std::string> &gearsName);
private:
  static std::map<std::string, GearMaker*> *_registry;
  virtual Gear* internalMakeGear(Engine *engine, std::string name)=0;
};

#define Register_Gear(MMAKERNAME, MCLASSNAME, MTYPE) \
class MMAKERNAME : public GearMaker \
{ \
public: \
    MMAKERNAME() : GearMaker(MTYPE){}; \
private: \
    Gear* internalMakeGear(Engine *engine, std::string name) \
    { \
        return new MCLASSNAME(engine, name);\
    } \
    static MMAKERNAME _registerThis;\
};\
MMAKERNAME MMAKERNAME::_registerThis;



#endif 
