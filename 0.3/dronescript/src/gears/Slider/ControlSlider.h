#ifndef CONTROLSLIDER_INCLUDED
#define CONTROLSLIDER_INCLUDED

#include "Control.h"

#include <qlayout.h>
#include <qslider.h>

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
  QVBoxLayout *_mainLayout;
  QSlider *_slider;
};

#endif
