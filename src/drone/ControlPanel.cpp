#include "ControlPanel.h"
#include "Control.h"
#include "GearControl.h"

#include "MetaGear.h"

#include <qslider.h>

ControlPanel::ControlPanel(QWidget *panelContainerWidget, MetaGear *parentMetagear) :
  QWidget(panelContainerWidget, 0, Qt::WStyle_NormalBorder | Qt::WStyle_Title),
  _parentMetaGear(parentMetagear)
{  
  _mainFrame = new QFrame(this);   
  _mainFrame->setMinimumWidth(320);
  _mainFrame->setMinimumHeight(200);  
  _mainFrame->setLineWidth(4);
  _mainFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);    
  _mainLayout = new QGridLayout(_mainFrame, 4, 4);

  _labelName = new QLabel(parentMetagear->name(), _mainFrame);
  _mainLayout->addWidget(_labelName, 0, 0);
  //TODO fddsfsdfsd
  
}

ControlPanel::~ControlPanel()
{

}

Control *ControlPanel::addControl(GearControl* gear)
{
  Q_UNUSED(gear);
//  if (!gear)
    return NULL;
  /*
  Control* control;// = gear->createControl(this);
  
  _mainLayout->addWidget(control, 0, 1);

  if (!control)
    return NULL;
  
  control->show();

  _controls.push_back(control);

  return control;*/
}

void ControlPanel::addControlPanel(ControlPanel* controlPanel)
{
  _controlPanels.push_back(controlPanel);
}


