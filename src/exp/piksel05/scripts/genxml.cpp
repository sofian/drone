#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "StringUtils.h"
#include <qstring.h>
#include <stdio.h>
#include <qdir.h>
#include <qtextstream.h>

//int errormsg(char const*,...)
///{}

int main( int argc, char **argv )
{
	
	std::string s;
	std::map<int,std::pair<int,int> > pos;
vector<string> coord;

std::ifstream i("../data/pos.txt");
	if(i.fail())
		printf("could not open pos.txt\n");
        std::ofstream o("../data/data.xml");
	o<<"<data x0=\"60.396\" y=\"5.32\" width=\"0.003\" height=\"0.002\">\n";

	getline(i, s);

	QString ts;
	coord = split(s," ");
	int yoff = tolong(coord[1]);
	int xoff = tolong(coord[0]);
	printf("using offset %i,%i\n",xoff,yoff);
	while (getline(i, s)) {
      // s contains the input line, without final newline char.
	coord = split(s," ");
	//if(coord.size()!=3)
	{
	  //printf("erreur : %i for line %i ",coord.size(),coord[0].c_str());
		//exit(1);
	}

	ts="<spot id=\""+QString((coord[0]))+"\">\n";
	ts+="  <position x=\""+QString(tostring(tolong(coord[1])+xoff))+"\" y=\""+QString(tostring(-tolong(coord[2])+yoff))+"\" z=\"0\"/>\n";

	QDir d;
	d.setPath("../data/clips");
        d.setFilter( QDir::Files | QDir::Hidden | QDir::NoSymLinks );
        d.setSorting( QDir::Size | QDir::Reversed );

	char pouet[10];
	sprintf(pouet,"%02i",tolong(coord[0]));
	d.setNameFilter("shot"+string(pouet)+"*");
        const QFileInfoList *list = d.entryInfoList();
        QFileInfoListIterator it( *list );
        QFileInfo *fi;
	int clips=0;
	while ( (fi = it.current()) != 0 ) {
            ts+= "  <moviefile>"+QString(fi->fileName().latin1())+"</moviefile>\n";
            ++it;
	    ++clips;
        }
	for(int i=3;i<coord.size();i++)
	  ts+= "  <connection from=\""+QString((coord[0]))+"\" to=\""+QString((coord[i]))+"\" />\n";
	
	ts+="</spot>\n\n";
	if(clips>0)
	  o<<ts.latin1();
   }
o<<"</data>\n";
o.close();
	
        return 0;
}
