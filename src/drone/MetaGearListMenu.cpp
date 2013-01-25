#include "MetaGearListMenu.h"
#include "MetaGearMaker.h"
//Added by qt3to4:
#include <Q3PopupMenu>

MetaGearListMenu::MetaGearListMenu(QWidget *parent, MetaGearListMenu* parentMetaGearListMenu) : 
  Q3PopupMenu(parent),
  _parentMetaGearListMenu(parentMetaGearListMenu)
{
  if (parentMetaGearListMenu==NULL)
  {
    _menuItemIndexfileInfoMap = new std::map<int, QFileInfo*>;
  }
  else//share the map if we are a submenu of parentMetaGearListMenu
    _menuItemIndexfileInfoMap = parentMetaGearListMenu->_menuItemIndexfileInfoMap;
  
  //transform the activated(int) signal into a metaGearSelected(QFileInfo*) signal via slotMenuItemSelected
  QObject::connect(this, SIGNAL(activated(int)), this, SLOT(slotMenuItemSelected(int)));   
}

MetaGearListMenu::~MetaGearListMenu()
{
  //delete the map if we are the parent
  if (_parentMetaGearListMenu==NULL)
    delete _menuItemIndexfileInfoMap;  
}

/**
 * create the MetaGearListMenu by consulting the gearsInfos in the MetaGearMaker registry
 */
void MetaGearListMenu::create()
{
/*  std::vector<QFileInfo*> metaGearsPath;

  //get all metaGearsPath from the MetaGearMaker
  metaGearsPath = MetaGearMaker::getAllMetaGearsPath();
 
  for (std::vector<QFileInfo*>::iterator it = metaGearsPath.begin(); it != metaGearsPath.end(); ++it)
  {
     MetaGearListMenu *menu=this;
               
     QStringList subPaths = QStringList::split("/",(*it)->dirPath());
          
     //we remove the root, since this is the metagear path and we dont want to display it
     subPaths.pop_front();

     for (QStringList::Iterator it2=subPaths.begin(); it2!=subPaths.end(); ++it2) 
     {
       menu = menu->addSubMenu(it2->toStdString());       
     }

     //insert the gear name in the correct menu
     int itemIndex = menu->insertItem((*it)->baseName());
     
     //populate the map
     (*_menuItemIndexfileInfoMap)[itemIndex] = (*it);     
  }
 */
}

/**
 * add a submenu to the MetaGearListMenu
 * if this submenu have already been created, return
 * a pointer on it, otherwise create it
 * 
 * @param name   name of the submenu
 * 
 * @return the corresponding subMenu
 */
MetaGearListMenu *MetaGearListMenu::addSubMenu(std::string name)
{
  std::map<std::string, MetaGearListMenu*>::iterator it = _subMenus.find(name);  

  //have we already created this subMenu?
  if (it == _subMenus.end())
  {
    //create it passing us has parent of this new MetaGearListMenu
    MetaGearListMenu *subMenu = new MetaGearListMenu((QWidget*)parent(), this);
    
    //foward of gear selection signal to parent menu
    QObject::connect(subMenu, SIGNAL(metaGearSelected(QFileInfo*)), this, SLOT(slotMetaGearSelected(QFileInfo*)));   
    
    //insert in our internal map
    _subMenus[name] = subMenu;

    //insert in the menu
    insertItem(name.c_str(), subMenu);

    //return the newly created subMenu
    return subMenu;
  }
  else
    //this subMenu have already been created, return it
    return it->second;
}

void MetaGearListMenu::slotMenuItemSelected(int id) 
{ 
  std::map<int, QFileInfo*>::iterator it = _menuItemIndexfileInfoMap->find(id);  
  
  if (it == _menuItemIndexfileInfoMap->end())
  {
    std::cout << "MetaGearListMenu item index mismatch!!!" << std::endl; 
    return;
  }

  emit metaGearSelected(it->second);
}  
