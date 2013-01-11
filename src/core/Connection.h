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

	QString gearA(){return _gearA;};
	QString input(){return _input;};
	QString gearB(){return _gearB;};
	QString output(){return _output;};

	void save(QDomDocument &doc, QDomElement &parent);
	void load(QDomElement &connectionElem);
	void updateWithRenameMapping(std::map<QString,QString> map);

private:
	QString _gearA;
	QString _input;
	QString _gearB;
	QString _output;
};

#endif