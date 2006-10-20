#include "Control.h"

#include "ControlPanel.h"

Control::Control(Gear *gear, ControlPanel *parentControlPanel) :
  QWidget(parentControlPanel->mainWidget()),
  _parentControlPanel(parentControlPanel),
  _parentGear(gear)
{
}

Control::~Control()
{
}



