#include "MainWindow.h"

#include <iostream>
#include <qfiledialog.h>
#include <qapplication.h>

#include "Play.xpm"
#include "Pause.xpm"
#include "GearGui.h"
#include "error.h"

const int MainWindow::CANVAS_SIZE_X = 2048;
const int MainWindow::CANVAS_SIZE_Y = 2048;

#include <qsettings.h>

extern QSettings globalSettings;


MainWindow::MainWindow() : 
QMainWindow(), 
_engine(NULL), 
_frame(NULL), 
_schemaCanvas(NULL), 
_schemaEditor(NULL),
_currentSchemaFilename("")
{    
  _engine = new Engine(0);

  _schemaCanvas = new QCanvas(CANVAS_SIZE_X, CANVAS_SIZE_Y);
  _schemaCanvas->setBackgroundColor(QColor(107,124,153));

  _schemaEditor = new SchemaEditor(_schemaCanvas, this, _engine);               

  setCentralWidget(_schemaEditor);   

  _toolBar = new QToolBar(this);
  addToolBar(_toolBar);        
  _playPause = new QToolButton(_toolBar);    


  //temp
  _toolBar->addSeparator();
  QToolButton *zoomOut = new QToolButton(_toolBar);    
  zoomOut->setText("-");    
  QToolButton *zoomIn = new QToolButton(_toolBar);    
  zoomIn->setText("+");    
  //

  _playPause->setToggleButton(TRUE);   

  QIconSet playPauseIcon;
  playPauseIcon.setPixmap(Play_xpm, QIconSet::Automatic, QIconSet::Normal, QIconSet::Off);
  playPauseIcon.setPixmap(Pause_xpm, QIconSet::Automatic, QIconSet::Normal, QIconSet::On);
  _playPause->setIconSet(playPauseIcon);   

  QObject::connect(_playPause, SIGNAL(toggled(bool)), this, SLOT(slotPlay(bool)));

  //temp
  QObject::connect(zoomIn, SIGNAL(pressed()), this, SLOT(slotZoomIn()));
  QObject::connect(zoomOut, SIGNAL(pressed()), this, SLOT(slotZoomOut()));
  //

  //menu    
  _fileMenu = new QPopupMenu(this);
  _fileMenu->insertItem("New", this, SLOT(slotMenuNew()));
  _fileMenu->insertItem("Load", this, SLOT(slotMenuLoad()));
  //we need to keep this id to enable/disable the save item
  _menuSaveItemId = _fileMenu->insertItem("Save", this, SLOT(slotMenuSave()));    
  _fileMenu->setItemEnabled(_menuSaveItemId, false);  
  
  _fileMenu->insertItem("Save as", this, SLOT(slotMenuSaveAs()));    
  _fileMenu->insertSeparator();
  _fileMenu->insertItem("Quit",  this, SLOT(slotMenuQuit()));    
  QMenuBar *menuBar = new QMenuBar(this);
  menuBar->setSeparator(QMenuBar::InWindowsStyle);
  menuBar->insertItem("&File", _fileMenu);


  //load settings
  _lastLoadPath = globalSettings.readEntry("/drone/Schema/LastLoadPath");
  _lastSavePath = globalSettings.readEntry("/drone/Schema/LastSavePath");

}

MainWindow::~MainWindow()
{

}

void MainWindow::slotZoomIn()
{
  _schemaEditor->zoomIn();
}

void MainWindow::slotZoomOut()
{
  _schemaEditor->zoomOut();
}


void MainWindow::slotPlay(bool play)
{
  if (play)
  {
    _engine->startPlaying();
  } else
    _engine->stopPlaying();    
}

void MainWindow::slotMenuNew()
{
  _currentSchemaFilename="";
  _schemaEditor->clearSchema();
  _fileMenu->setItemEnabled(_menuSaveItemId, false);
}

void MainWindow::slotMenuLoad()
{  
  QString filename = QFileDialog::getOpenFileName(_lastLoadPath, "*" + Engine::SCHEMA_EXTENSION + ";;" + "*.*", 
                                                  this, "Load", "Load");

  if (!filename.isEmpty())
  {
    _schemaEditor->loadSchema(filename.ascii());
    _currentSchemaFilename=filename.ascii();
    _fileMenu->setItemEnabled(_menuSaveItemId, true);

    //save the last load path
    _lastLoadPath=filename;
    globalSettings.writeEntry("/drone/Schema/LastLoadPath", _lastLoadPath);
    
  }
}

void MainWindow::load(std::string filename)
{
  _schemaEditor->loadSchema(filename);
  _currentSchemaFilename=filename;
  _fileMenu->setItemEnabled(_menuSaveItemId, true);
}

void MainWindow::slotMenuSave()
{
  ASSERT_ERROR(_currentSchemaFilename.size());
  _engine->saveSchema(_currentSchemaFilename);    
}

void MainWindow::slotMenuSaveAs()
{
  QString filename = QFileDialog::getSaveFileName(_lastSavePath, "*" + Engine::SCHEMA_EXTENSION + ";;" + "*.*", 
                                                  this, "Save as", "Save as");
  if (!filename.isEmpty())
  {
    _engine->saveSchema(filename.ascii());
    _currentSchemaFilename=filename.ascii();
    _fileMenu->setItemEnabled(_menuSaveItemId, true);

    //save the last save path
    _lastSavePath=filename;
    globalSettings.writeEntry("/drone/Schema/LastSavePath", _lastSavePath);

  }
}

void MainWindow::slotMenuQuit()
{
  qApp->quit();
}

