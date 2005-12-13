
#ifndef HELPEDITOR_H
#define HELPEDITOR_H

#include <qvariant.h>
#include <qscrollview.h>
#include <qdialog.h>
#include <qpair.h>
#include "GearInfo.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QTextEdit;
class QComboBox;
class QToolButton;

class HelpEditor : public QWidget
{
    Q_OBJECT

public:
    HelpEditor( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~HelpEditor();

    void loadGearInfo(GearInfo_ *gi);

    QLabel* textLabel3;
    QTextEdit* _introEditor;
    QLabel* textLabel3_2;
    QComboBox* _plugsCombo;
    QTextEdit* _plugEditor;
    QLabel* textLabel4;
    QTextEdit* _descriptionEditor;
    QToolButton* pushButton1;

signals:
    void saved();
protected slots: 
    void save();
    void plugSelected(int);

protected:
    PlugInfo* _currentPlugInfo;
    QMap<int,QPair<QString,PlugInfo*> > _pimap;
    
    GearInfo_* _currentGearInfo;
    QVBoxLayout* layout6;
    QHBoxLayout* layout2;

};

#endif // HELPEDITOR_H
