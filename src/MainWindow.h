#ifndef MAINWINDOW_INCLUDED
#define MAINWINDOW_INCLUDED

#include <qmainwindow.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <list>

#include "Engine.h"

#include "SchemaEditor.h"


class MainWindow : public QMainWindow
{
  Q_OBJECT
  public:
  static const int CANVAS_SIZE_X;
  static const int CANVAS_SIZE_Y;

  //! loads the specified schema file
  void load(std::string filename);
  void play(bool pl);

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

  void slotMenuItemSelected(int id);

private:

  static const unsigned int MainWindow::MAX_RECENT_SCHEMAS;
  static const std::string SCHEMA_EXTENSION;

  void addToRecentSchema(std::string filename);
  

  Engine* _engine;
  QFrame* _frame;
  QCanvas* _schemaCanvas;

  SchemaEditor *_schemaEditor;    

  QToolBar *_toolBar;
  QToolButton *_playPause;
  
  int _menuSaveItemId;
  QPopupMenu *_fileMenu;
  
  std::string _currentSchemaFilename;  
  QString _lastLoadPath;
  QString _lastSavePath;
  std::list<std::string> _recentSchemas;
  int _menuFirstRecentSchemaId;

//    PlayThread *_playThread;
};


#endif
