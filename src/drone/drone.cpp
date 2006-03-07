/* drone.cpp
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

#include <qapplication.h>
#include <qsplashscreen.h>
#include "Timing.h"
#include "DroneCore.h"
#include "GuileBindings.h"
#include "splash.xpm"

#include "MainWindow.h"

#include <iostream>
#include <stdio.h>

#include <qsettings.h>

#include <guile/gh.h>

QSettings globalSettings;

void innerMain(int argc, char** argv)
{
  gh_repl(argc, argv);
}

int main(int argc, char** argv)
{
  gh_enter(argc, argv, GuileBindings::init);

  GuileBindings::release();
  //qtApp.exec();

  //release the core
}

