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

#include <qmainwindow.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qsplitter.h>
#include <list>

#include "Engine.h"
#include "Project.h"

class SchemaGui;
class MetaGearEditor;
class MetaGear;

class MainWindow : public QMainWindow
{
  Q_OBJECT
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

  void slotMenuItemSelected(int id);

protected:
  void timerEvent(QTimerEvent*);

private:

  static const unsigned int MainWindow::MAX_RECENT_SCHEMAS;
  static const std::string SCHEMA_EXTENSION;

  void addToRecentSchema(std::string filename);
  

  Engine* _engine;
  QFrame* _frame;
  SchemaGui* _mainSchemaGui;
  
  MetaGearEditor *_metaGearEditor;
  
  QToolBar *_toolBar;
  QToolButton *_playPause;
  
  int _menuSaveItemId;
  int _menuPrefsItemId;
  QPopupMenu *_fileMenu;
  QPopupMenu *_toolsMenu;
  QPopupMenu *_viewMenu;

  Project* _project;
  
  std::string _currentSchemaFilename;  
  QString _lastLoadPath;
  QString _lastSavePath;
  std::list<std::string> _recentSchemas;
  int _menuFirstRecentSchemaId;

//    PlayThread *_playThread;
};


#endif
