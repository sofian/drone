#ifndef CONTROLPANEL_INCLUDED
#define CONTROLPANEL_INCLUDED

#include <qwidget.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <list>
#include <q3frame.h>
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

  Q3Frame *_mainFrame;  
  Q3GridLayout *_mainLayout;
  QLabel *_labelName;
};

#endif
