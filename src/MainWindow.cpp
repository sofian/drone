#include "MainWindow.h"

#include <iostream>
#include <qfiledialog.h>
#include <qapplication.h>
#include <sstream>

#include "Play.xpm"
#include "Pause.xpm"
#include "GearGui.h"
#include "error.h"


const int MainWindow::CANVAS_SIZE_X = 2048;
const int MainWindow::CANVAS_SIZE_Y = 2048;
const unsigned int MainWindow::MAX_RECENT_SCHEMAS = 5;

#include <qsettings.h>

extern QSettings globalSettings;


MainWindow::MainWindow() : 
QMainWindow(), 
_engine(NULL), 
_frame(NULL), 
_schemaCanvas(NULL), 
_schemaEditor(NULL),
_currentSchemaFilename(""),
_menuFirstRecentSchemaId(-1)
{    
  _engine = new Engine(0);

  _schemaCanvas = new QCanvas(CANVAS_SIZE_X, CANVAS_SIZE_Y);
  _schemaCanvas->setBackgroundColor(QColor(107,124,153));

  _schemaEditor = new SchemaEditor(_schemaCanvas, this, _engine);               

  setCentralWidget(_schemaEditor);   

  _toolBar = new QToolBar(this);
  addToolBar(_toolBar);        
  _playPause = new QToolButton(_toolBar);    
  _playPause->setToggleButton(true);

  //temp
  _toolBar->addSeparator();
  QToolButton *zoomOut = new QToolButton(_toolBar);    
  zoomOut->setText("-");    
  QToolButton *zoomIn = new QToolButton(_toolBar);    
  zoomIn->setText("+");    
  //

  

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
  _fileMenu->insertSeparator();
  
  //for the most recent schema files that will be appended
  QObject::connect(_fileMenu, SIGNAL(activated(int)), this, SLOT(slotMenuItemSelected(int)));

  QMenuBar *menuBar = new QMenuBar(this);
  menuBar->setSeparator(QMenuBar::InWindowsStyle);
  menuBar->insertItem("&File", _fileMenu);

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

void MainWindow::play(bool pl)
{
  if (pl)
  {
    _playPause->setOn(true);
    _engine->startPlaying();

  } else
  {
    _playPause->setOn(false);
    _engine->stopPlaying();
  }    
}

void MainWindow::slotMenuNew()
{
  //stop before clearing
  play(false);

  _currentSchemaFilename="";
  _schemaEditor->clearSchema();
  _fileMenu->setItemEnabled(_menuSaveItemId, false);
}

void MainWindow::slotMenuLoad()
{  
  QString filename = QFileDialog::getOpenFileName(_lastLoadPath, "*" + Engine::SCHEMA_EXTENSION + ";;" + "*.*", 
                                                  this, "Load", "Load");
  
  load(filename);
}

void MainWindow::load(std::string filename)
{
  if (filename.empty())
    return;
  
  //stop before loading
  play(false);

  _schemaEditor->loadSchema(filename);
  _currentSchemaFilename=filename;
  _fileMenu->setItemEnabled(_menuSaveItemId, true);
  
  //save the last load path
  _lastLoadPath=filename;
  globalSettings.writeEntry("/drone/Schema/LastLoadPath", _lastLoadPath);

  //add to recent schema
  addToRecentSchema(filename);

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

    //add to recent schema
    addToRecentSchema(filename);
  }
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

