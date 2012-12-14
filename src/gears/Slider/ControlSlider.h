#ifndef CONTROLSLIDER_INCLUDED
#define CONTROLSLIDER_INCLUDED

#include "Control.h"

#include <qlayout.h>
#include <qslider.h>
//Added by qt3to4:
#include <Q3VBoxLayout>

class ControlSlider : public Control
{
Q_OBJECT
public:
  ControlSlider(Gear* parentGear, ControlPanel *parentControlPanel);
  ~ControlSlider();

  void onUpdate();

public slots:
  void slotValueChanged(int value);

private:
  Q3VBoxLayout *_mainLayout;
  QSlider *_slider;
};

#endif
