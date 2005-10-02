/*
 *  PlugListView.h
 *  drone
 *
 *  Created by foogy on 20/09/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <qlistview.h>


class Gear;
class GearGui;

class PlugListView : public QListView
{
	Q_OBJECT
public:
	PlugListView(QWidget *parent);
  
	void refresh(Gear *gear);    

public slots:
	void slotGearSelected(GearGui *gearGui);
		
	
private:
	Gear *_gear;
	
};