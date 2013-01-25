/* Gear_OscInput.h
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

#ifndef GEAR_OSCINPUT_INCLUDED
#define GEAR_OSCINPUT_INCLUDED

#include "Gear.h"
#include "StringType.h"
#include "ValueType.h"
#include "OscMessageType.h"

#include "lo/lo.h"

#include "ThreadUtil.h"

#include <vector>

class Gear_OscInput : public Gear  
{
public:
  Gear_OscInput();
  virtual ~Gear_OscInput();

  void runVideo();

protected:
	void internalPostPlay();
	
private:
	void startOscServer(QString port);
	void stopOscServer();
	
	static void error(int num, const char *m, const char *path);	
	static int configuredOscHandler(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);
		
	PlugIn<StringType> *_PORT;
	
  PlugOut<OscMessageType> *_OSC_OUT;
	
	bool _forceOscServerInit;
	QString _currentPort;
	
	lo_server_thread _loServerThread;
	pthread_mutex_t *_mutex;
	std::vector<OscMessageType> _messages;
};

#endif 
