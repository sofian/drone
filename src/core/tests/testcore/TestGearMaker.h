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
		QVERIFY(GearMaker::parse(QDir("testgears")));
		
		QVERIFY(GearMaker::findGearInfo("Simple")!=NULL);
	}

	void testParseMultiple()
	{
		QVERIFY(GearMaker::parse(QDir("testgears")));
		QVERIFY(GearMaker::parse(QDir("testgears")));
		
		QVERIFY(GearMaker::findGearInfo("Simple")!=NULL);
	}

	void testParseWrongDir()
	{
		bool result = GearMaker::parse(QDir("patate"));	
		QVERIFY(!result);
	}
	
	void testMakeGear()
	{
	}
};