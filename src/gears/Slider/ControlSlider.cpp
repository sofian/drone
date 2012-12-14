#include "ControlSlider.h"
#include "Gear_Slider.h"
//Added by qt3to4:
#include <Q3VBoxLayout>

ControlSlider::ControlSlider(Gear* parentGear, ControlPanel *parentControlPanel) :
  Control(parentGear, parentControlPanel)
{
  _mainLayout = new Q3VBoxLayout(this, 1);
  _slider = new QSlider(Qt::Horizontal, this);
  QObject::connect(_slider, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));  
  _mainLayout->addWidget(_slider);
  onUpdate();
  
}

ControlSlider::~ControlSlider()
{

}

void ControlSlider::onUpdate()
{
  float low = _parentGear->settings().get(Gear_Slider::SETTING_LOWERBOUND)->valueFloat();
  float hi = _parentGear->settings().get(Gear_Slider::SETTING_HIGHERBOUND)->valueFloat();

  //todo : correct conversion by multiplying
  _slider->setRange((int)low, (int)hi);

  float value = ((Gear_Slider*)_parentGear)->getValue();

  //todo : correct conversion by multiplying
  _slider->setValue((int)value);
}

void ControlSlider::slotValueChanged(int value)
{
  ((Gear_Slider*)_parentGear)->setValue(value);  
}

