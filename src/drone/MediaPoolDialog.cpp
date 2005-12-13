#include "MediaPoolDialog.h"
#include "MediaPoolItem.h"
#include "MediaMovie.h"


MediaPoolDialog::MediaPoolDialog(QWidget *parent) :
  QWidget(parent)
{
  setCaption("Media Pool");
  _verticalLayout = new QVBoxLayout(this, 3);
  _horizontalLayout = new QHBoxLayout(_verticalLayout);
  
  _mediaPoolIconView = new MediaPoolIconView(this);
  _importButton = new QPushButton("Import", this);
  _movieButton = new QPushButton("Movie", this);
  _movieButton->setToggleButton(true);
  _pictureButton = new QPushButton("Picture", this);
  _pictureButton->setToggleButton(true);

  _horizontalLayout->addWidget(_movieButton);
  _horizontalLayout->addWidget(_pictureButton);

  //_verticalLayout->addLayout(_horizontalLayout); Mathieu : qt aime pas ce pcq horizlayout a deja le verticallayout comme parent
  _verticalLayout->addWidget(_mediaPoolIconView);    
  _verticalLayout->addWidget(_importButton);

  MediaMovie *testMediaMovie = new MediaMovie();
  
  for (int i=0; i < 16; i++)
  {
     MediaPoolItem *testItem = new MediaPoolItem(_mediaPoolIconView, testMediaMovie);
     _mediaPoolIconView->addItem(testItem);
  }
}

MediaPoolDialog::~MediaPoolDialog()
{
}
  
