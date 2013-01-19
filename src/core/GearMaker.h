/* GearMaker.h
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
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

#include <QMap>

#include <vector>
#include <qdir.h>

#include "Gear.h"
#include "GearInfo.h"

/**
 * note : parseGears have to be called to discover gears in path and populate the 
 * factory. In drone, this task is handled by calling DroneCore::init().
*/
class GearMaker
{
public:									
	static GearMaker* instance();

  void registerStaticGear(QString type, GearInfo::GearCreator gear_creator);

  Gear* makeGear(QString fullName);
	Gear* makeGear(QString type, QString name);

  void getAllGearsInfo(QList<GearInfo*> &gearsInfo);
	GearInfo* findGearInfo(QString fullName);
	GearInfo* findGearInfo(QString type, QString name);

	bool parse(QDir rootDir);
	bool parse();
	QDir defaultGearsDir();

private:
	static QString DRONEGEARS_SUBPATH;
	static QString FREI0RGEARS_SUBPATH;
	static QString CONTROLGEARS_SUBPATH;
	static QString METAGEARS_SUBPATH;
		
	GearMaker();

  template <class T> void parseGears(QDir dir, QString extension);
	void parseDroneGears(QDir rootDir);
  void parseControlGears(QDir rootDir);
  void parseFrei0rGears(QDir rootDir);
  void parseMetaGears(QDir rootDir);
	
  void emptyRegistry();

  QMap<QString, GearInfo*> _registry;
  
};

#endif
