#ifndef PANELSCROLLVIEW_INCLUDED
#define PANELSCROLLVIEW_INCLUDED

#include <qscrollview.h>
#include <list>
#include <qvbox.h>

class ControlPanel;
class MetaGear;

class PanelScrollView : public QScrollView
{
public:  
  PanelScrollView(QWidget *parent);
  ~PanelScrollView();

  ControlPanel* addControlPanel(MetaGear *metaGear);
    
private:
  std::list<ControlPanel*> _controlPanels;
  QVBox* _mainVBox;
};

#endif
