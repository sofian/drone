#ifndef CONTROLPANEL_INCLUDED
#define CONTROLPANEL_INCLUDED

#include <qwidget.h>
#include <qgridlayout>
#include <list>
#include <qframe.h>
#include <qlayout.h>
#include <qlabel.h>


class Control;
class MetaGear;
class GearControl;

class ControlPanel : public QWidget
{
public:
  ControlPanel(QWidget *panelContainerWidget, MetaGear *parentMetagear);
  ~ControlPanel();

  
  Control *addControl(GearControl* gear);
  void addControlPanel(ControlPanel* controlPanel);
  QWidget *mainWidget(){return _mainFrame;}
  
private:
  std::list<Control*> _controls;
  std::list<ControlPanel*> _controlPanels;

  MetaGear *_parentMetaGear;

  QFrame *_mainFrame;  
  QGridLayout *_mainLayout;
  QLabel *_labelName;
};

#endif
