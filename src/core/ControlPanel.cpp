#include "ControlPanel.h"
#include "Control.h"
#include "GearControl.h"

#include "MetaGear.h"

#include <qslider.h>
//Added by qt3to4:
#include <Q3Frame>
#include <Q3GridLayout>
#include <QLabel>

ControlPanel::ControlPanel(QWidget *panelContainerWidget, MetaGear *parentMetagear) :
  QWidget(panelContainerWidget, 0, Qt::WStyle_NormalBorder | Qt::WStyle_Title),
  _parentMetaGear(parentMetagear)
{  
  _mainFrame = new Q3Frame(this);   
  _mainFrame->setMinimumWidth(320);
  _mainFrame->setMinimumHeight(200);  
  _mainFrame->setLineWidth(4);
  _mainFrame->setFrameStyle(QFrame::StyledPanel | Q3Frame::Plain);    
  _mainLayout = new Q3GridLayout(_mainFrame, 4, 4);

  _labelName = new QLabel(parentMetagear->name().c_str(), _mainFrame);
  _mainLayout->addWidget(_labelName, 0, 0);
  //TODO fddsfsdfsd
  
}

ControlPanel::~ControlPanel()
{

}

Control *ControlPanel::addControl(GearControl* gear)
{
  if (!gear)
    return NULL;
  
  Control* control = gear->createControl(this);
  
  _mainLayout->addWidget(control, 0, 1);

  if (!control)
    return NULL;
  
  control->show();

  _controls.push_back(control);

  return control;
}

void ControlPanel::addControlPanel(ControlPanel* controlPanel)
{
  _controlPanels.push_back(controlPanel);
}


