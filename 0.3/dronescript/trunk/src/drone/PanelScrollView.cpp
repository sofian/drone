#include "PanelScrollView.h"
#include "ControlPanel.h"

#include "MetaGear.h"
  
#include <iostream>
#include <qlabel.h>


PanelScrollView::PanelScrollView(QWidget *parent) :
  QScrollView(parent)
{  
  setMinimumWidth(350);  
  //resizeContents(400, 400);  
  
  _mainVBox = new QVBox(viewport());
  _mainVBox->setMinimumWidth(350);   
  _mainVBox->setMinimumHeight(800);
  addChild(_mainVBox);                
}

PanelScrollView::~PanelScrollView()
{
}

ControlPanel *PanelScrollView::addControlPanel(MetaGear *metaGear)
{
  ControlPanel *controlPanel = new ControlPanel(_mainVBox, metaGear);
  metaGear->associateControlPanel(controlPanel);
  
  controlPanel->show();
    
  return controlPanel;
}
