/* 
 * File:   PlugInfo.h
 * Author: julien
 *
 * Created on January 20, 2013, 8:35 PM
 */

#ifndef PLUGINFO_H
#define	PLUGINFO_H

#include <qstring.h>
#include "AbstractPlug.h"
class Gear;
class Schema;

/**
 * Pluginfo provide the meta information associated with each plug of the gear. PlugInfo are saved with GearInfo.
 *
**/ 
class PlugInfo
{
public:
	PlugInfo(){}
  PlugInfo(QString name, eInOut inOut, QString type, QString help="") :
	  _name(name),
		_inOut(inOut),
		_type(type),
		_help(help)
		{}

	QString name() const {return _name;}
	void name(QString v){_name=v;}

	eInOut inOut() const {return _inOut;}
	void inOut(eInOut v){_inOut=v;}

	QString type() const {return _type;}
	void type(QString v){_type=v;}

	QString help() const {return _help;}
	void help(QString v){_help=v;}

	bool save(QDomDocument doc, QDomElement parent)  const;
	bool load(QDomElement elem);
		
private:		
	QString _name;
	eInOut _inOut;
  QString _type;
  QString _help;
};


#endif	/* PLUGINFO_H */

