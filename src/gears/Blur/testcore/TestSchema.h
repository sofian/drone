/*
 *  TestSchema.h
 *  testcore
 *
 *  Created by foogy on 28/12/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <QtTest/QtTest>

class TestSchema : public QObject
{
Q_OBJECT

private slots:
	void testLoad()
	{
			QString str = "Hello";
			QCOMPARE(str.toUpper(), QString("HELLO"));
	}
};