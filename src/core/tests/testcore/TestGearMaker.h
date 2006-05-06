/*
 *  TestGearMaker.h
 *  testcore
 *
 *  Created by foogy on 28/12/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */
#include <QtTest/QtTest>

#include "GearMaker.h"

class TestGearMaker : public QObject
{
Q_OBJECT

private slots:

	
	void testParse()
	{
		QVERIFY(GearMaker::instance()->parse(QDir("testgears")));
		
		QVERIFY(GearMaker::instance()->findGearInfo("Drone:Simple")!=NULL);
		QVERIFY(GearMaker::instance()->findGearInfo("Frei0r:flippo")!=NULL);				
		//other find
		QVERIFY(GearMaker::instance()->findGearInfo("Drone", "Simple")!=NULL);
		QVERIFY(GearMaker::instance()->findGearInfo("Frei0r","flippo")!=NULL);				
	}

	void testParseMultiple()
	{
		QVERIFY(GearMaker::instance()->parse(QDir("testgears")));
		QVERIFY(GearMaker::instance()->parse(QDir("testgears")));
		
		QVERIFY(GearMaker::instance()->findGearInfo("Drone:Simple")!=NULL);
		QVERIFY(GearMaker::instance()->findGearInfo("Frei0r:flippo")!=NULL);		
		//TODO: pas en double?
	}

	void testParseWrongDir()
	{
		bool result = GearMaker::instance()->parse(QDir("patate"));	
		QVERIFY(!result);
	}
	
	void testMakeGearDrone()
	{
		QVERIFY(GearMaker::instance()->parse(QDir("testgears")));
		QVERIFY(GearMaker::instance()->makeGear(NULL, 
	}
};