#include "MainWindow.h"

#include <iostream>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qfiledialog.h>
#include <qapplication.h>

#include "Play.xpm"
#include "Pause.xpm"
#include "GearGui.h"

const int MainWindow::CANVAS_SIZE_X = 2048;
const int MainWindow::CANVAS_SIZE_Y = 2048;

MainWindow::MainWindow() : 
    QMainWindow(), 
    _engine(NULL), 
    _frame(NULL), 
    _schemaCanvas(NULL), 
    _schemaEditor(NULL)    
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
    QPopupMenu *fileMenu = new QPopupMenu(this);
    fileMenu->insertItem("New", this, SLOT(slotMenuNew()));
    fileMenu->insertItem("Load", this, SLOT(slotMenuLoad()));
    fileMenu->insertItem("Save", this, SLOT(slotMenuSave()));    
    fileMenu->insertItem("Save as", this, SLOT(slotMenuSaveAs()));    
    fileMenu->insertSeparator();
    fileMenu->insertItem("Quit",  this, SLOT(slotMenuQuit()));    
    QMenuBar *menuBar = new QMenuBar(this);
    menuBar->setSeparator(QMenuBar::InWindowsStyle);
    menuBar->insertItem("&File", fileMenu);


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
    }        
    else
        _engine->stopPlaying();    
}

void MainWindow::slotMenuNew()
{
    _currentSchemaFilename="";
}

void MainWindow::slotMenuLoad()
{
    QString filename = QFileDialog::getOpenFileName(QString::null, "*.*", this, "Load", "Load");
    if (!filename.isEmpty())
    {    
        _engine->loadSchema(filename.ascii());
        _schemaEditor->recreateSchemaFromEngine();
        _currentSchemaFilename=filename.ascii();
    }
}

void MainWindow::slotMenuSave()
{
    _engine->saveSchema(_currentSchemaFilename);    
}

void MainWindow::slotMenuSaveAs()
{
    QString filename = QFileDialog::getSaveFileName(QString::null, "*.*", this, "Save as", "Save as");
    if (!filename.isEmpty())
    {
        _engine->saveSchema(filename.ascii());
        _currentSchemaFilename=filename.ascii();
    }    
}

void MainWindow::slotMenuQuit()
{
    qApp->quit();
}

