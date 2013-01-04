/* MainWindow.h
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

#ifndef MAINWINDOW_INCLUDED
#define MAINWINDOW_INCLUDED

#include <q3mainwindow.h>
#include <q3toolbar.h>
#include <qtoolbutton.h>
#include <q3popupmenu.h>
#include <qmenubar.h>
#include <qsplitter.h>
//Added by qt3to4:
#include <Q3Frame>
#include <QTimerEvent>
#include <list>

#include "Engine.h"
#include "Project.h"

//#include "GuileBindings.h"

class SchemaGui;
class MetaGearEditor;
class MetaGear;

class MainWindow : public Q3MainWindow
{
  Q_OBJECT

  //friend class GuileBindings;
  
public:

  //! loads the specified schema file
  void load(std::string filename);
  //void play(bool pl);

  MainWindow();
  ~MainWindow();

public slots:
  void slotPlay(bool);
	void slotZoomIn();
	void slotZoomOut();


  void slotMenuNew();
  void slotMenuLoad();
  void slotMenuSave();
  void slotMenuSaveAs();
  void slotMenuQuit();

  void slotMenuPreferences();

  void slotMenuViewMediaPool();
	void slotMenuViewSmallGears();

  void slotMenuItemSelected(int id);

  // place me somewhere else when you have time.
  // Not really the job of the main window
  void initFonts();
protected:
  void timerEvent(QTimerEvent*);

private:

  static const unsigned int MAX_RECENT_SCHEMAS;
  static const std::string SCHEMA_EXTENSION;

  void addToRecentSchema(std::string filename);
  

  Engine* _engine;
  Q3Frame* _frame;
  SchemaGui* _mainSchemaGui;
  
  MetaGearEditor *_metaGearEditor;
  
  Q3ToolBar *_toolBar;
  QToolButton *_playPause;
  QToolButton *_zoomIn;
  QToolButton *_zoomOut;

  
  int _menuSaveItemId;
  int _menuPrefsItemId;
  Q3PopupMenu *_fileMenu;
  Q3PopupMenu *_toolsMenu;
  Q3PopupMenu *_viewMenu;

  Project* _project;
  
  std::string _currentSchemaFilename;  
  QString _lastLoadPath;
  QString _lastSavePath;
  std::list<std::string> _recentSchemas;
  int _menuFirstRecentSchemaId;
	int _menuShowSmallGearsId;
	bool _showSmallGears;

//    PlayThread *_playThread;
};


#endif
