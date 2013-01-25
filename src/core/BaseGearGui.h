/* 
 * File:   BaseGearGui.h
 * Author: julien
 *
 * Created on January 19, 2013, 3:54 PM
 */

#ifndef BASEGEARGUI_H
#define	BASEGEARGUI_H
#include <QtXml>

class BaseGearGui {
public:
  BaseGearGui();
  BaseGearGui(const BaseGearGui& orig);
  virtual ~BaseGearGui();
  virtual bool getIsSelected(){return false;}
  
  virtual void save(QDomDocument &doc, QDomElement &gearElem);
  virtual void load(QDomElement &gearElem);

  
private:

};

#endif	/* BASEGEARGUI_H */

