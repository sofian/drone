#ifndef MAINWINDOW_INCLUDED
#define MAINWINDOW_INCLUDED

#include <qmainwindow.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include "Engine.h"

#include "SchemaEditor.h"


class MainWindow : public QMainWindow
{
  Q_OBJECT
  public:
  static const int CANVAS_SIZE_X;
  static const int CANVAS_SIZE_Y;

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

private:


  Engine* _engine;
  QFrame* _frame;
  QCanvas* _schemaCanvas;

  SchemaEditor *_schemaEditor;    

  QToolBar *_toolBar;
  QToolButton *_playPause;
  std::string _currentSchemaFilename;

//    PlayThread *_playThread;
};


#endif
