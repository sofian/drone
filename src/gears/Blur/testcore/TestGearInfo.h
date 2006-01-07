/*
 *  TestGearInfo.h
 *  testcore
 *
 *  Created by foogy on 01/01/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include <QtTest/QtTest>

#include "GearInfo.h"
#include "Gear.h"

class TestGearInfo : public QObject
{
Q_OBJECT

private slots:

	void testLoad_MetaDoesntExist()
	{
		//prepare
		//remove any existing meta info file
		QFile metaFile("testgears/drone/Simple.xml");
		metaFile.remove();

		QFileInfo simpleFileInfo("testgears/drone/libSimple.dylib.1.0.0");
		GearInfoDrone simpleGearInfo(simpleFileInfo);
		
		//test
		QVERIFY(simpleGearInfo.load());		
		//verify
		QVERIFY(metaFile.exists());
	}

	void testLoad_MetaWrong()
	{
		//prepare
		//remove any existing meta info file
		//and create an empty one
		QFile metaFile("testgears/drone/Simple.xml");
		metaFile.remove();
		metaFile.open(QIODevice::WriteOnly);
		metaFile.close();

		QFileInfo simpleFileInfo("testgears/drone/libSimple.dylib.1.0.0");
		GearInfoDrone simpleGearInfo(simpleFileInfo);
		
		//test
		QVERIFY(simpleGearInfo.load());		
		//verify
		QVERIFY(metaFile.exists());
	}
	
	void testLoad()
	{
		//prepare
		//create it
		QFileInfo simpleFileInfo("testgears/drone/libSimple.dylib.1.0.0");
		GearInfoDrone simpleGearInfoForCreation(simpleFileInfo);
		simpleGearInfoForCreation.load();

		//test & verify
		GearInfoDrone simpleGearInfo(simpleFileInfo);
		QVERIFY(simpleGearInfo.load());		
	}
	
	void testCreateGearInstance()
	{
		//prepare
		QFileInfo simpleFileInfo("testgears/drone/libSimple.dylib.1.0.0");
		GearInfoDrone simpleGearInfo(simpleFileInfo);
		QVERIFY(simpleGearInfo.load());		
	
		//test
		Gear* gear = simpleGearInfo.createGearInstance(NULL, "Simple");
		//verify
		QVERIFY(gear!=NULL);
		delete gear;
	}
	
	void testPlugsInfo_DoesntExists()
	{
		//prepare
		//remove any existing meta info file
		QFile metaFile("testgears/drone/Simple.xml");
		metaFile.remove();
		//load
		QFileInfo simpleFileInfo("testgears/drone/libSimple.dylib.1.0.0");
		GearInfoDrone simpleGearInfo(simpleFileInfo);
		QVERIFY(simpleGearInfo.load());		

		//test
		QMap<QString, PlugInfo> plugsInfo = simpleGearInfo.getAllPlugsInfo();
		QVERIFY(plugsInfo.contains("ImgIN"));
		QVERIFY(plugsInfo.contains("ImgOUT"));
		QVERIFY(plugsInfo.contains("Amount"));
		
	}

	void testPlugsInfo_Exists()
	{		
		//prepare
		//create it
		QFileInfo simpleFileInfo("testgears/drone/libSimple.dylib.1.0.0");
		GearInfoDrone simpleGearInfoForCreation(simpleFileInfo);
		simpleGearInfoForCreation.load();

		//test
		GearInfoDrone simpleGearInfo(simpleFileInfo);
		QVERIFY(simpleGearInfo.load());		

		QMap<QString, PlugInfo> plugsInfo = simpleGearInfo.getAllPlugsInfo();
		QVERIFY(plugsInfo.contains("ImgIN"));
		QVERIFY(plugsInfo.contains("ImgOUT"));
		QVERIFY(plugsInfo.contains("Amount"));
	}

	void testPlugsInfo_Sync()
	{		
		//prepare
		//create it
		QFileInfo simpleFileInfo("testgears/drone/libSimple.dylib.1.0.0");
		GearInfoDrone simpleGearInfoForCreation(simpleFileInfo);
		simpleGearInfoForCreation.load();
		PlugInfo pi = simpleGearInfoForCreation.getPlugInfo("ImgIN");
		pi.type("patate");
		pi.inOut(OUT);
		simpleGearInfoForCreation.setPlugInfo(pi);
		simpleGearInfoForCreation.save();
		
		//test
		GearInfoDrone simpleGearInfo(simpleFileInfo);
		QVERIFY(simpleGearInfo.load());		
		PlugInfo expectedPi;
		expectedPi = simpleGearInfo.getPlugInfo("ImgIN");
		QVERIFY(expectedPi.type() == "VideoRGBAType");
		QVERIFY(expectedPi.inOut() == IN);
	}

	void testPlugsInfo_Help()
	{		
		//prepare
		//create it
		QFileInfo simpleFileInfo("testgears/drone/libSimple.dylib.1.0.0");
		GearInfoDrone simpleGearInfoForCreation(simpleFileInfo);
		simpleGearInfoForCreation.load();
		PlugInfo pi = simpleGearInfoForCreation.getPlugInfo("ImgIN");
		pi.help("hello!");
		simpleGearInfoForCreation.setPlugInfo(pi);
		simpleGearInfoForCreation.save();
		
		//test
		GearInfoDrone simpleGearInfo(simpleFileInfo);
		QVERIFY(simpleGearInfo.load());		
		PlugInfo expectedPi;
		expectedPi = simpleGearInfo.getPlugInfo("ImgIN");
		QVERIFY(expectedPi.help() == "hello!");
	}

};