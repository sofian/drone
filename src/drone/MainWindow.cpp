/* MainWindow.cpp
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

#include "MainWindow.h"

#include <iostream>
#include <qfiledialog.h>
#include <qapplication.h>
#include <sstream>

#include "Play.xpm"
#include "Pause.xpm"
#include "GearGui.h"
#include "error.h"

#include "SchemaGui.h"
#include "MetaGear.h"
#include "MetaGearEditor.h"

#include "MediaPoolItem.h"
#include "MediaMovie.h"
#include "MediaPoolDialog.h"



//#include "PreferencesDialog.h"

const unsigned int MainWindow::MAX_RECENT_SCHEMAS = 5;
const std::string MainWindow::SCHEMA_EXTENSION = ".drn";

#include <qsettings.h>

extern QSettings globalSettings;


MainWindow::MainWindow() : 
QMainWindow(), 
_engine(NULL), 
_frame(NULL), 
_mainSchemaGui(NULL), 
_menuFirstRecentSchemaId(-1)
{    
  _engine = new Engine(0);    
  _metaGearEditor = new MetaGearEditor(this, _engine->mainMetaGear(), _engine);
  _mainSchemaGui = _metaGearEditor->schemaGui();
  
  _project = new Project(_mainSchemaGui);
  
  setCentralWidget(_metaGearEditor);

  _toolBar = new QToolBar(this);
  addToolBar(_toolBar);        
  _playPause = new QToolButton(_toolBar);    		
  _playPause->setToggleButton(true);
	
	/*
  _zoomIn = new QToolButton(_toolBar);    
	_zoomOut = new QToolButton(_toolBar);    
  QObject::connect(_zoomIn, SIGNAL(toggled(bool)), _metaGearEditor->schemaEditor(), SLOT(slotZoomIn()));
  QObject::connect(_zoomOut, SIGNAL(toggled(bool)), _metaGearEditor->schemaEditor(), SLOT(slotZoomIn()));
	*/

  QIconSet playPauseIcon;
  playPauseIcon.setPixmap(Play_xpm, QIconSet::Automatic, QIconSet::Normal, QIconSet::Off);
  playPauseIcon.setPixmap(Pause_xpm, QIconSet::Automatic, QIconSet::Normal, QIconSet::On);
  _playPause->setIconSet(playPauseIcon);   

  QObject::connect(_playPause, SIGNAL(toggled(bool)), this, SLOT(slotPlay(bool)));
  
  //menu    
  _fileMenu = new QPopupMenu(this);
  _fileMenu->insertItem("New", this, SLOT(slotMenuNew()), CTRL+Key_N);
  _fileMenu->insertItem("Load", this, SLOT(slotMenuLoad()), CTRL+Key_O);
  
  //we need to keep this id to enable/disable the save item
  _menuSaveItemId = _fileMenu->insertItem("Save", this, SLOT(slotMenuSave()), CTRL+Key_S);    
  _fileMenu->setItemEnabled(_menuSaveItemId, false);  
  
  _fileMenu->insertItem("Save as", this, SLOT(slotMenuSaveAs()));    
  _fileMenu->insertSeparator();
  _fileMenu->insertItem("Quit",  this, SLOT(slotMenuQuit()), CTRL+Key_Q);    
  _fileMenu->insertSeparator();
  
  _toolsMenu = new QPopupMenu(this);
  _menuPrefsItemId = _toolsMenu->insertItem("Preferences", this, SLOT(slotMenuPreferences()));
  _toolsMenu->setItemEnabled(_menuPrefsItemId, false);    

  _viewMenu = new QPopupMenu(this);
  _viewMenu->insertItem("Media Pool", this, SLOT(slotMenuViewMediaPool()), CTRL+Key_M);
 
  //for the most recent schema files that will be appended
  QObject::connect(_fileMenu, SIGNAL(activated(int)), this, SLOT(slotMenuItemSelected(int)));

  QMenuBar *menuBar = new QMenuBar(this);
  menuBar->setSeparator(QMenuBar::InWindowsStyle);
  menuBar->insertItem("&File", _fileMenu);
  menuBar->insertItem("&Tools", _toolsMenu);
  menuBar->insertItem("&View", _viewMenu);

  //load settings
  _lastLoadPath = globalSettings.readEntry("/drone/Schema/LastLoadPath");
  _lastSavePath = globalSettings.readEntry("/drone/Schema/LastSavePath");

  //recent schema files
  QStringList recentSchemaKeys = globalSettings.entryList("/drone/Schema/Recent Files");
  globalSettings.beginGroup("/drone/Schema/Recent Files");
  int i=0;
  for (QStringList::iterator it=recentSchemaKeys.begin(); it!=recentSchemaKeys.end(); ++it, ++i)
  {
    QString filename = globalSettings.readEntry(*it);
    _fileMenu->insertItem(filename, i);
    _recentSchemas.push_back(filename);    
  }
  globalSettings.endGroup();
}

MainWindow::~MainWindow()
{

}

void MainWindow::slotPlay(bool play)
{
  #ifdef SINGLE_THREADED_PLAYBACK    
    static int timerId=0;
  #endif 
  
  if (play)
  {    
    _playPause->setOn(true);   
    #ifndef SINGLE_THREADED_PLAYBACK    
    _engine->startPlaying();
    #else
    _engine->debugStartPlay();  
    timerId=startTimer(10);
    #endif
  } 
  else
  {
    _playPause->setOn(false);
    #ifndef SINGLE_THREADED_PLAYBACK    
    _engine->stopPlaying();    
    #else
     killTimer(timerId);
     _engine->debugStopPlay();  
    #endif
  }
}

void MainWindow::slotMenuNew()
{
  //stop before clearing
  slotPlay(false);

  _project->newProject();
  //_schemaEditor->clearSchema();
  _fileMenu->setItemEnabled(_menuSaveItemId, false);
}

void MainWindow::slotMenuLoad()
{  
  QString filename = QFileDialog::getOpenFileName(_lastLoadPath, "*" + SCHEMA_EXTENSION + ";;" + "*.*", 
                                                  this, "Load", "Load");
  load(filename);
}

void MainWindow::load(std::string filename)
{
  if (filename.empty())
    return;
  
  //stop before loading
  slotPlay(false);

  _project->load(filename);
  _fileMenu->setItemEnabled(_menuSaveItemId, true);
  
  //save the last load path
  _lastLoadPath=_project->projectName();
  globalSettings.writeEntry("/drone/Schema/LastLoadPath", _lastLoadPath);

  //add to recent schema
  addToRecentSchema(filename);

}

void MainWindow::slotMenuSave()
{
  _project->save();    
}

void MainWindow::slotMenuSaveAs()
{
  std::string filename = QFileDialog::getSaveFileName(_project->projectName(), "*" + SCHEMA_EXTENSION + ";;" + "*.*", 
                                                      this, "Save as", "Save as");
  
  if (!filename.empty())
  {
    //set the extension if not already present
    if (filename.find(SCHEMA_EXTENSION.c_str(), filename.size()-SCHEMA_EXTENSION.size())==std::string::npos)
      filename.append(SCHEMA_EXTENSION);  
    
    _project->saveAs(filename);
    
    _fileMenu->setItemEnabled(_menuSaveItemId, true);

    //save the last save path
    _lastSavePath=filename;
    globalSettings.writeEntry("/drone/Schema/LastSavePath", _lastSavePath);

    //add to recent schema
    addToRecentSchema(filename);
  }
}

void MainWindow::slotMenuPreferences()
{
//  PreferencesDialog pd(this, _engine);
//  pd.exec();
  
}

void MainWindow::addToRecentSchema(std::string filename)
{  
  //clean recent menu and settings first
  for (unsigned int j=0; j<_recentSchemas.size();++j)  
  {
    _fileMenu->removeItem(j);  
    std::ostringstream oss;
    oss << "/drone/Schema/Recent Files/" << j;
    globalSettings.removeEntry(oss.str());
  }

  //remove from the list if already in
  _recentSchemas.remove(filename);    

  //add at front
  _recentSchemas.push_front(filename);

  //remove the last one if over MAX_RECENT_SCHEMAS
  if (_recentSchemas.size() > MAX_RECENT_SCHEMAS)
    _recentSchemas.pop_back();
  
  //rebuild the recent schema menu and save to globalSettings
  //item will range from 0 to MAX_RECENT_SCHEMAS  
  int i=0;
  for (std::list<std::string>::iterator it=_recentSchemas.begin(); it!=_recentSchemas.end();++it, ++i)
  {
    _fileMenu->insertItem(*it, i);
    std::ostringstream oss;
    oss << "/drone/Schema/Recent Files/" << i;
    globalSettings.writeEntry(oss.str(), *it);
  }
}

void MainWindow::slotMenuItemSelected(int id)
{
  //item will range from 0 to MAX_RECENT_SCHEMAS
  if (id < 0 || id > (int)MAX_RECENT_SCHEMAS)
    return;
  
  load(_fileMenu->text(id));
}

void MainWindow::slotMenuQuit()
{
  qApp->quit();
}

void MainWindow::slotMenuViewMediaPool()
{
  MediaPoolDialog mediaPoolDialog(this);

  mediaPoolDialog.exec();

}

void MainWindow::timerEvent(QTimerEvent*)
{
  _engine->playThread(_engine);
}
