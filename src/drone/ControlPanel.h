#ifndef CONTROLPANEL_INCLUDED
#define CONTROLPANEL_INCLUDED

#include <QWidget>
#include <QGridLayout>
#include <QFrame>
#include <QLayout>
#include <QLabel>

#include <list>

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
