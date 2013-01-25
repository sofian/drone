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

#include <QMainWindow>
#include <QToolBar>
#include <QToolButton>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>
//Added by qt3to4:
#include <QFrame>
#include <QTimerEvent>
#include <QUndoStack>

#include <list>

#include "Engine.h"
#include "Project.h"

//#include "GuileBindings.h"

class SchemaGui;
class MetaGearEditor;
class MetaGear;

class MainWindow : public QMainWindow
{
  Q_OBJECT

  //friend class GuileBindings;
  
public:

  //! loads the specified schema file
  void load(QString filename);
  //void play(bool pl);
  static MainWindow* getInstance();

  MainWindow();
  ~MainWindow();
  void finalize();
  
  void pushUndoCommand(QUndoCommand* c);
  SchemaGui* getSchemaGui() const {return _mainSchemaGui;}
  Project* getProject(){return _project;}
  
public slots:
  void slotPlay(bool);
  void slotItemsMoved(QList<QString> &itemNames, QPointF dist);


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

  static MainWindow* instance;
  void timerEvent(QTimerEvent*);

private:

  static const int MAX_RECENT_SCHEMAS;
  static const QString SCHEMA_EXTENSION;

  void addToRecentSchema(QString filename);
  

  Engine* _engine;
  QFrame* _frame;
  SchemaGui* _mainSchemaGui;
  
  MetaGearEditor *_metaGearEditor;
  
  QToolBar *_toolBar;
  
  // toolbar buttons
  //QToolButton* _tbPlayPause;
  
  
  
  // ugly but we'll fix that when isolating CORE from GUI
public :
  // QActions
  QAction *_actPlayPause;
  QAction *_actZoomIn;
  QAction *_actZoomOut;
  QAction *_actNew;
  QAction *_actLoad;
  QAction *_actSave;
  QAction *_actSaveAs;
  QAction *_actQuit;
  QAction *_actPreferences; 
  
  QAction *_actSelectAll;
  QAction *_actDeleteSelected;
  QAction *_actCopy;
  QAction *_actPaste;
  QAction *_actUndo;
  QAction *_actRedo;
  
  
protected:
  
  
  QMenu *_fileMenu;
  QMenu *_editMenu;
  QMenu *_toolsMenu;
  QMenu *_viewMenu;

  QUndoStack *_undoStack;
  
  
  
  
  Project* _project;
  
  std::string _currentSchemaFilename;  
  QString _lastLoadPath;
  QString _lastSavePath;
  QList<QString> _recentSchemas;
  int _menuFirstRecentSchemaId;
	int _menuShowSmallGearsId;
	bool _showSmallGears;

//    PlayThread *_playThread;
};


#endif
