#include "GearListMenu.h"
#include "GearMaker.h"
#include "GearClassification.h"
//Added by qt3to4:
#include <qmenu>


GearListMenu::GearListMenu(QWidget *parent) : 
  QMenu(parent)
{
  //transform the activated(int) signal into a gearSelected(QString) signal via slotMenuItemSelected
  QObject::connect(this, SIGNAL(activated(int)), this, SLOT(slotMenuItemSelected(int)));   
}

/**
 * create the GearListMenu by consulting the gearsInfos in the GearMaker registry
 */
void GearListMenu::create()
{
  std::vector<const GearInfo*> gearsInfo;

  //get all gearsInfo from the GearMaker
  GearMaker::getAllGearsInfo(gearsInfo);
 
  for (std::vector<const GearInfo*>::iterator it = gearsInfo.begin(); it != gearsInfo.end(); ++it)
  {
     GearListMenu *menu=this;
     
     if ((*it)->classification)
     {     
       std::vector<std::string> path = (*it)->classification->path();
                                 
       //create subMenu path for this gear
       for (std::vector<std::string>::iterator it2 = path.begin(); it2!=path.end();++it2)       
         menu = menu->addSubMenu((*it2));
       
     }

     //insert the gear name in the correct menu
     menu->addAction(new QAction((*it)->name.c_str(),this));
  }
}

/**
 * add a submenu to the GearListMenu
 * if this submenu have already been created, return
 * a pointer on it, otherwise create it
 * 
 * @param name   name of the submenu
 * 
 * @return the corresponding subMenu
 */
GearListMenu *GearListMenu::addSubMenu(std::string name)
{
  std::map<std::string, GearListMenu*>::iterator it = _subMenus.find(name);  

  //have we already created this subMenu?
  if (it == _subMenus.end())
  {
    //create it
    GearListMenu *subMenu = new GearListMenu(this);
    
    //foward of gear selection signal to parent menu
    QObject::connect(subMenu, SIGNAL(gearSelected(QString)), this, SLOT(slotGearSelected(QString)));   
    
    //insert in our internal map
    _subMenus[name] = subMenu;

    //insert in the menu
    QAction* subMenuAction = addMenu(subMenu);
    subMenuAction->setText(name.c_str());
            
    //return the newly created subMenu
    return subMenu;
  }
  else
    //this subMenu have already been created, return it
    return it->second;
}

