#ifndef GEARPROPERTIESDIALOG_INCLUDED
#define GEARPROPERTIESDIALOG_INCLUDED

#include <qdialog.h>
#include <vector>

class QPushButton;
class Gear;
class QVBoxLayout;
class PropertyControl;
class Property;

class GearPropertiesDialog : public QDialog
{
  Q_OBJECT
  public:

  GearPropertiesDialog(QWidget *parent, Gear *gear);
  ~GearPropertiesDialog();

public slots:
  void slotOK();

private:

  void addControl(Property *property);

  Gear* _gear;
  QVBoxLayout* _verticalLayout;
  std::vector<PropertyControl*> _propertyControls;

  QPushButton *_okButton;
  QPushButton *_cancelButton;

};

#endif

