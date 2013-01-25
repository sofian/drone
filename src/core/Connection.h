#ifndef CONNECTION_INCLUDED
#define CONNECTION_INCLUDED

/*
 *  Connection.h
 *  droneCore
 *
 *  Created by foogy on 20/02/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */
#include <QtXml>
#include <QDataStream>

class Gear;

class Connection
{
public:
	Connection(){};        
	Connection(QString gearA, QString output, QString gearB, QString input) :
		_gearA(gearA),
		_input(input),
		_gearB(gearB),
		_output(output)
	{
	}

	Connection(QString connStr)
	{
    QStringList ends(connStr.split(","));
    QStringList endA(ends[0].split(':'));
    QStringList endB(ends[1].split(':'));
    _gearA=endA[0];
    _gearB=endB[0];
    _output=endA[1];
    _input=endB[1];
	}

	QString gearA() const {return _gearA;};
	QString input() const {return _input;};
	QString gearB() const {return _gearB;};
	QString output() const {return _output;};

	void save(QDomDocument &doc, QDomElement &parent);
	void load(QDomElement &connectionElem);
	void updateWithRenameMapping(QMap<QString,QString> map);
  QString toString() const { return QString(gearA()+":"+output()+","+gearB()+":"+input()); }

private:
	QString _gearA;
	QString _input;
	QString _gearB;
	QString _output;
};

QDebug	operator<< ( QDebug out, const Connection & conn );  


#endif
