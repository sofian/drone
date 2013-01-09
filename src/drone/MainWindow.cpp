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
//Added by qt3to4:
#include <QTimerEvent>
#include <QMenuBar>
#include <QMenu>
#include <sstream>

#include "Play.xpm"
#include "Pause.xpm"
#include "GearGui.h"
#include "error.h"

#include "SchemaGui.h"
#include "SchemaEditor.h"
#include "MetaGear.h"
#include "MetaGearEditor.h"

#include "MediaPoolItem.h"
#include "MediaMovie.h"
#include "MediaPoolDialog.h"

#include <qstringlist>
#include <qfontdatabase>
#include <qmessagebox>
#include <qfile>
#include <qtoolbutton>


//#include "PreferencesDialog.h"

const unsigned int MainWindow::MAX_RECENT_SCHEMAS = 5;
const std::string MainWindow::SCHEMA_EXTENSION = ".drn";

#include <qsettings.h>

extern QSettings globalSettings;

MainWindow *MainWindow::instance=0;

MainWindow* MainWindow::getInstance()
{
  if (!instance)
  {
    instance = new MainWindow();
  }
  return instance;
}


MainWindow::MainWindow() : 
QMainWindow(), 
_engine(NULL), 
_frame(NULL), 
_mainSchemaGui(NULL), 
_menuFirstRecentSchemaId(-1),
_menuShowSmallGearsId(false)
{    
  _engine = new Engine(0);    
  _metaGearEditor = new MetaGearEditor(this, _engine->mainMetaGear(), _engine);
  _mainSchemaGui = _metaGearEditor->schemaGui();
  SchemaEditor* schemaEditor = _metaGearEditor->schemaEditor();
  
  _project = new Project(_mainSchemaGui);
  
  setCentralWidget(_metaGearEditor);

  _toolBar = new QToolBar(this);
  addToolBar(_toolBar);


  QIcon playPauseIcon;
  playPauseIcon.setPixmap(Play_xpm, QIcon::Automatic, QIcon::Normal, QIcon::Off);
  playPauseIcon.setPixmap(Pause_xpm, QIcon::Automatic, QIcon::Normal, QIcon::On);
  
//  _tbPlayPause = new QToolButton(_toolBar);    		
  _actPlayPause = new QAction("Play/Pause",this);
  _actPlayPause->setIcon(playPauseIcon);
  _actPlayPause->setCheckable(true);

  //_actPlayPause->setToggleButton(true);
	
  QObject::connect(_actPlayPause, SIGNAL(toggled(bool)), this, SLOT(slotPlay(bool)));
	
	QMenuBar* menuBar = new QMenuBar(0);
  
  //menu    
  _fileMenu = menuBar->addMenu("&File");
  _actNew =  _fileMenu->addAction("New", this, SLOT(slotMenuNew()), QKeySequence::New);
  _actLoad = _fileMenu->addAction("Open", this, SLOT(slotMenuLoad()), QKeySequence::Open);
  _actSave = _fileMenu->addAction("Save", this, SLOT(slotMenuSave()), QKeySequence::Save);    
  _actSave->setDisabled(true);  
  
  _actSaveAs=_fileMenu->addAction("Save as", this, SLOT(slotMenuSaveAs()), QKeySequence::SaveAs);    
  _fileMenu->addSeparator();
  _actQuit = _fileMenu->addAction("Quit",  this, SLOT(slotMenuQuit()), QKeySequence::Quit);    


  
  _editMenu = menuBar->addMenu("&Edit");
	_actSelectAll=_editMenu->addAction("Select all", schemaEditor, SLOT(slotSelectAll()),  QKeySequence::SelectAll);
  _actDeleteSelected=_editMenu->addAction("Delete selected", schemaEditor, SLOT(slotDeleteSelected()), QKeySequence::Delete);
	_actCopy=_editMenu->addAction("Copy", schemaEditor, SLOT(slotGearCopy()), QKeySequence::Copy);
	_actPaste=_editMenu->addAction("Paste", schemaEditor, SLOT(slotGearPaste()), QKeySequence::Paste);
	_actUndo =_editMenu->addAction("Undo", schemaEditor, SLOT(slotUndo()), QKeySequence::Undo);
	_actRedo =_editMenu->addAction("Redo", schemaEditor, SLOT(slotRedo()), QKeySequence::Redo);
 
  _viewMenu = menuBar->addMenu("&View");
	_viewMenu->setCheckable(true);
  
  _actZoomIn = _viewMenu->addAction("Zoom in", schemaEditor, SLOT(slotZoomIn()), Qt::CTRL+Qt::Key_Plus);
  _actZoomOut = _viewMenu->addAction("Zoom out", schemaEditor, SLOT(slotZoomOut()), Qt::CTRL+Qt::Key_Minus);


  _toolsMenu = menuBar->addMenu("&Tools");
  _actPreferences = _toolsMenu->addAction("Preferences", this, SLOT(slotMenuPreferences()));


  // add actions to toolbar
  _toolBar->addAction(_actPlayPause);
  _toolBar->addSeparator();
  _toolBar->addAction(_actZoomOut);
  _toolBar->addAction(_actZoomIn);
  





  //_toolsMenu->setItemEnabled(_menuPrefsItemId, false);    
  
  //_viewMenu->insertItem("Media pool", this, SLOT(slotMenuViewMediaPool()), Qt::CTRL+Qt::Key_M);
  //_menuShowSmallGearsId = _viewMenu->insertItem("Small gears", this, SLOT(slotMenuViewSmallGears()), Qt::CTRL+Qt::Key_I);

  //for the most recent schema files that will be appended
  QObject::connect(_fileMenu, SIGNAL(activated(int)), this, SLOT(slotMenuItemSelected(int)));

  //menuBar->insertItem("&File", _fileMenu);
//  menuBar->insertItem("&Tools", _toolsMenu);
  //menuBar->insertItem("&View", _viewMenu);

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
    //_fileMenu->insertItem(filename, i);
    _recentSchemas.push_back(filename.toStdString());    
  }
  globalSettings.endGroup();
}


void MainWindow::finalize()
{
  SchemaEditor* schemaEditor = _metaGearEditor->schemaEditor();
  schemaEditor->buildContextMenus();
}
MainWindow::~MainWindow()
{

}

void MainWindow::initFonts()
{
  QStringList list;
  list << "Roboto-Regular.ttf";
  int fontID(-1);
  bool fontWarningShown(false);
  for (QStringList::const_iterator constIterator = list.constBegin(); constIterator != list.constEnd(); ++constIterator)
  {
    QFile res(":/resources/fonts/" + *constIterator);
    if (res.open(QIODevice::ReadOnly) == false)
    {
      if (fontWarningShown == false)
      {
        QMessageBox::warning(0, "Application", (QString) "Impossible d'ouvrir la police " + QChar(0x00AB) + *constIterator + QChar(0x00BB) + ".");
        fontWarningShown = true;
      }
    }
    else
    {
      fontID = QFontDatabase::addApplicationFontFromData(res.readAll());
      if (fontID == -1 && fontWarningShown == false)
      {
        QMessageBox::warning(0, "Application", (QString) "Impossible d'ajouter " + QChar(0x00AB) + *constIterator + QChar(0x00BB) + " à la banque de polices.");
        fontWarningShown = true;
      }
    }
  }
}

void MainWindow::slotPlay(bool play)
{
  #ifdef SINGLE_THREADED_PLAYBACK    
    static int timerId=0;
  #endif 
  
  if (play)
  {    
    _actPlayPause->setChecked(true);   
    #ifndef SINGLE_THREADED_PLAYBACK    
    _engine->startPlaying();
    #else
    _engine->debugStartPlay();  
    timerId=startTimer(10);
    #endif
  } 
  else
  {
    _actPlayPause->setChecked(false);
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
  _actSave->setDisabled(true);
}

void MainWindow::slotMenuLoad()
{  
  QString filename = QFileDialog::getOpenFileName(_lastLoadPath, ("*" + SCHEMA_EXTENSION + ";;" + "*.*").c_str(), 
                                                  this, "Load", "Load");
  load(filename.toStdString());
}

void MainWindow::load(std::string filename)
{
  if (filename.empty())
    return;
  
  //stop before loading
  slotPlay(false);

  _project->load(filename);
  _actSave->setDisabled(false);
  
  //save the last load path
  _lastLoadPath=_project->projectName().c_str();
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
  std::string filename = QFileDialog::getSaveFileName(_project->projectName().c_str(), ("*" + SCHEMA_EXTENSION + ";;" + "*.*").c_str(), 
                                                      this, "Save as", "Save as").toStdString();
  
  if (!filename.empty())
  {
    //set the extension if not already present
    if (filename.find(SCHEMA_EXTENSION.c_str(), filename.size()-SCHEMA_EXTENSION.size())==std::string::npos)
      filename.append(SCHEMA_EXTENSION);  
    
    _project->saveAs(filename);
    
    _actSave->setDisabled(false);

    //save the last save path
    _lastSavePath=filename.c_str();
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
    globalSettings.removeEntry(oss.str().c_str());
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
    _fileMenu->insertItem(it->c_str(), i);
    std::ostringstream oss;
    oss << "/drone/Schema/Recent Files/" << i;
    globalSettings.writeEntry(oss.str().c_str(), it->c_str());
  }
}

void MainWindow::slotMenuItemSelected(int id)
{
  //item will range from 0 to MAX_RECENT_SCHEMAS
  if (id < 0 || id > (int)MAX_RECENT_SCHEMAS)
    return;
  
  load(_fileMenu->text(id).toStdString());
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

void MainWindow::slotMenuViewSmallGears()
{
	_showSmallGears=!_showSmallGears;
	_viewMenu->setItemChecked(_menuShowSmallGearsId, _showSmallGears);
	//	_mainSchemaGui->showSmallGears(_showSmallGears);
}



void MainWindow::timerEvent(QTimerEvent*)
{
  _engine->playThread(_engine);
}

