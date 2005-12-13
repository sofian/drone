#ifndef HelpWindow_INCLUDED
#define HelpWindow_INCLUDED
 
#include <qdialog.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qwidgetstack.h>


class HelpBrowser;
class HelpEditor;
class QWidgetStack;
class GearInfo_;
class GearGui;

class HelpWindow : public QWidget
{
Q_OBJECT
protected slots:
  void editorSaved();
  void editMode();
  void slotLoadGearInfo(GearGui*);
  void slotLoadGearInfo(GearInfo_* gi);


public:  
  HelpWindow(QWidget *parent);
  ~HelpWindow();

private:
  GearInfo_* _currentGearInfo;
  QVBoxLayout *_verticalLayout;
  HelpBrowser * _browser;
  HelpEditor* _editor;
  QWidgetStack * _ws;
  int _browserid,_editorid;
};

#endif
