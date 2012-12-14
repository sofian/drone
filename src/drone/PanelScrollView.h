#ifndef PANELSCROLLVIEW_INCLUDED
#define PANELSCROLLVIEW_INCLUDED

#include <q3scrollview.h>
#include <list>
#include <q3vbox.h>

class ControlPanel;
class MetaGear;

class PanelScrollView : public Q3ScrollView
{
public:  
  PanelScrollView(QWidget *parent);
  ~PanelScrollView();

  ControlPanel* addControlPanel(MetaGear *metaGear);
    
private:
  std::list<ControlPanel*> _controlPanels;
  Q3VBox* _mainVBox;
};

#endif
