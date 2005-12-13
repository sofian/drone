
#ifndef hbrowser_H
#define hbrowser_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QToolButton;
class QTextBrowser;

class HelpBrowser : public QWidget
{
    Q_OBJECT
public:
    HelpBrowser( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~HelpBrowser();

    QToolButton* _backBut;
    QToolButton* _forwardBut;
    QToolButton* _editBut;
    QTextBrowser* _textBrowser;


   void setText(QString txt);

signals:

    void edit();
protected:
    QVBoxLayout* layout4;
    QHBoxLayout* layout3;
    QSpacerItem* spacer1;

};

#endif // FORM1_H
