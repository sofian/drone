/* 
 * File:   BaseGearGui.cpp
 * Author: julien
 * 
 * Created on January 19, 2013, 3:54 PM
 */

#include "BaseGearGui.h"

BaseGearGui::BaseGearGui()
{
}

BaseGearGui::BaseGearGui(const BaseGearGui& orig)
{
  Q_UNUSED(orig);
}

BaseGearGui::~BaseGearGui()
{
}

void BaseGearGui::save(QDomDocument &doc, QDomElement &gearElem)
{
  Q_UNUSED(doc);
  Q_UNUSED(gearElem);
};

void BaseGearGui::load(QDomElement &gearElem)
{
  Q_UNUSED(gearElem);

};
