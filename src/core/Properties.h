/* Properties.h
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable
 * This file is part of Drone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef PROPERTIES_INCLUDED
#define PROPERTIES_INCLUDED

#include <QMap>
#include <QString>
#include <QList>
#include "error.h"

class QDomDocument;
class QDomElement;

class Property
{
public:
  enum eType
  {
    STRING, FILENAME, FILENAMES, INT, FLOAT, BOOL
  };

  Property()
  {
    _value.push_back("");
  }

  Property(eType type, QString name) : _type(type), _name(name)
  {
    _value.push_back("");
  }

  void valueStr(QString value);
  void valueStrList(const QList<QString> &value);
  void valueInt(int value);
  void valueFloat(float value);
  void valueBool(bool value);

  QString valueStr() {return _value[0];};
  QList<QString> valueStrList() {return _value;};
  int valueInt();
  float valueFloat();
  bool valueBool();

  eType type(){return _type;};
  QString name(){return _name;};
  void name(QString name){_name = name;};



private:
  eType _type;
  QString _name;
  QList<QString> _value;

};

class Properties
{
public:
  static const QString XML_TAGNAME;

  Property* add(Property::eType type, QString name);

  Property* get(QString name);

  void getAll(QList<Property*> *properties);

  void save(QDomDocument &doc, QDomElement &parent);
  void load(QDomElement &parentElem);

private:
  static const char WHITESPACE_REPLACEMENT;
  QMap<QString, Property*> _properties;


};

#endif
