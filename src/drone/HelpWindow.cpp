#include "HelpWindow.h"
#include "HelpEditor.h"
#include "qtextstream.h"
#include "HelpBrowser.h"
#include "GearGui.h"
#include "GearInfo.h"
#include "GearMaker.h"
#include <iostream>
#include <qdom.h>
#include <qfile.h>

HelpWindow::HelpWindow(QWidget *parent) :
    QWidget(parent),_currentGearInfo(0)
{
  QVBoxLayout *vbl= new QVBoxLayout(this);

  _ws = new QWidgetStack(this);
  vbl->addWidget(_ws);

  setCaption("Help Window");
  _verticalLayout = new QVBoxLayout(_ws);
  _browser = new HelpBrowser(_ws);
  _editor = new HelpEditor(_ws);
  _verticalLayout->addWidget(_browser);
  _browserid = _ws->addWidget(_browser);

  _editorid = _ws->addWidget(_editor);
  //loadGear("/home/julien/dev/drone/metagears/video/blur/MetaGear.1.meta");
  _browser->show();
  _editor->show();
  //std::cerr<<_browser->source().latin1()<<std::endl;
  //_ws->raiseWidget(_editorid);

  QObject::connect(_editor,SIGNAL(saved()),this,SLOT(editorSaved()));
  QObject::connect(_browser,SIGNAL(edit()),this,SLOT(editMode()));
}

HelpWindow::~HelpWindow()
{}


void HelpWindow::slotLoadGearInfo(GearInfo_* gi)
{
  ASSERT(gi);
  _currentGearInfo=gi;
  GearMaker::createMissingGearInfoPlugHelp(gi);

  QString helpText;
  helpText+="<h2><font color=\"#220000\">"+gi->_name+"</font></h2>";
  helpText+="<h3>Class: "+gi->_classification.join("/")+"</h3>";
  helpText += "<h3>Intro</h3>"+gi->_intro+"<br>";

  if(gi->_plugs.size())
  {
    helpText+="<h3>Inputs</h3><dl>";
    QMap<QString,PlugInfo>::Iterator it;
    for ( it = gi->_plugs.begin(); it != gi->_plugs.end(); ++it )
      if(it.data().inOut==IN)
      {
        helpText+="<dt>"+it.key()+" ["+it.data().type+"]</dt><dd>"+it.data().help+"</dd>";
      }
    helpText += "</dl><h3>Outputs</h3><dl>";
    for ( it = gi->_plugs.begin(); it != gi->_plugs.end(); ++it )
      if(it.data().inOut==OUT)
      {
        helpText+="<dt>"+it.key()+" ["+it.data().type+"]</dt><dd>"+it.data().help+"</dd>";
      }

    helpText += "</dl><h3>Description</h3>"+gi->_description+"<br>";
    std::cerr<<helpText.latin1()<<std::endl;
    _browser->setText(helpText.latin1());
  }

  // editor window
  _editor->loadGearInfo(gi);


}
void HelpWindow::slotLoadGearInfo(GearGui* ggui)
{
  ASSERT(ggui);
  ASSERT(ggui->gear());
  GearInfo_ *gi = ggui->gear()->getGearInfo_();
  slotLoadGearInfo(gi);
}
/*
 
  while (!plugNode.isNull())
  {
		QDomElement plugElem = plugNode.toElement();
    if (!plugElem.isNull())
    {
			std::string name = plugElem.attribute("Name","").ascii();
			//now find this plug and load is attributes
			for (std::list<AbstractPlug*>::iterator it=_plugs.begin(); it != _plugs.end(); ++it)
			{
				if ((*it)->name() == name)
					(*it)->load(plugElem);
			}
		}
		plugNode = plugNode.nextSibling();
	}
*/


void HelpWindow::editorSaved()
{
  slotLoadGearInfo(_currentGearInfo);
  _ws->raiseWidget(_browserid);
}

void HelpWindow::editMode()
{
  _ws->raiseWidget(_editorid);
}
