#include "Engine.h"
#include "Gear.h"
#include "Plug.h"
#include "GearMaker.h"
#include <iostream>
#include <unistd.h>
#include <qdom.h>
#include <qfile.h>
#include <qtextstream.h>

void Engine::Connection::save(QDomDocument &doc, QDomElement &parent)
{
    QDomElement connectionElem = doc.createElement("Connection");
    parent.appendChild(connectionElem);
    
    QDomAttr gearA = doc.createAttribute("GearA");
    gearA.setValue(_gearA.c_str());
    connectionElem.setAttributeNode(gearA);
    
    QDomAttr input = doc.createAttribute("Input");
    input.setValue(_input.c_str());
    connectionElem.setAttributeNode(input);

    QDomAttr gearB = doc.createAttribute("GearB");
    gearB.setValue(_gearB.c_str());
    connectionElem.setAttributeNode(gearB);

    QDomAttr output = doc.createAttribute("Output");
    output.setValue(_output.c_str());
    connectionElem.setAttributeNode(output);

}

void Engine::Connection::load(QDomElement &connectionElem)
{
    _gearA = connectionElem.attribute("GearA","").ascii();
    _input = connectionElem.attribute("Input","").ascii();
    _gearB = connectionElem.attribute("GearB","").ascii();
    _output = connectionElem.attribute("Output","").ascii();
}


SignalInfo Engine::_signalInfo;
VideoInfo Engine::_videoInfo;

Engine::Engine(int hwnd) : 
    _hWnd(hwnd),
    _AverageLoad(0.0f),
    _currentTime(0.0),
    _playing(false),
    _graphSynched(false)
    
{

}

Engine::~Engine()
{
    removeAllGears();
}

void Engine::removeAllGears()
{
	for(std::list<Gear*>::iterator it = _Gears.begin(); it != _Gears.end(); ++it)
		delete *it;

    _Gears.clear();
}

void Engine::removeGear(Gear* gear)
{
	delete gear;
	_Gears.remove(gear);
}


Gear* Engine::addGear(std::string geartype, std::string name)
{
	Gear * gear = GearMaker::makeGear(this, geartype, name);
	
    if(gear==NULL)
		std::cout << "Engine addGear: " << geartype << " unknown" << std::endl;
	else         
    {    
        gear->internalInit();
        _Gears.push_back(gear);
    }

    return gear;
}

Gear* Engine::addGear(std::string gearType)
//auto-naming
{
    return addGear(gearType, getNewGearName(gearType));
}


std::string Engine::getNewGearName(std::string prefix)
{
	int i=1;
	std::string tmp;
	bool ok=false;
	char buf[10];
	while(!ok && i<200) 
	{	
		ok=true;
		sprintf(buf,"%i",i);
		tmp=prefix + buf;
		for(std::list<Gear*>::iterator it=_Gears.begin();it!=_Gears.end();++it)
			if((*it)->name() == tmp)
			{
				ok=false;
				break;
			}
		i++;
	}
	return tmp; 
}


Time_T Engine::currentTime()
{
    return _currentTime;
}

void Engine::startPlaying()
{
    _playing=true;
    pthread_create(&_playThreadHandle, NULL, playThread, this);
}

void Engine::stopPlaying()
{                        
	_playing=false;
	
    //destroy thread???
}
                                    
void *Engine::playThread(void *parent)
{
    Engine *engine = (Engine*) parent;

    engine->_currentTime=0;

    float block_starttime, block_endtime, block_targettime;

    float block_time;

    float real_starttime;
    float real_time=0.0f;
    float sleeptime=0.0f;
    long blockIt=1;
    block_targettime = _signalInfo.blockSize() * _signalInfo.timePerSample() * 1000.0f;

    real_starttime = Timing::time();

    float cumul_load = 0.0f;
    engine->_AverageLoad=0.0f;

    for(std::list<Gear*>::iterator it=engine->_Gears.begin();it!=engine->_Gears.end();++it)    
        (*it)->internalPrePlay();

    int currentFrame = 0;

    while (engine->_playing)
    {
        block_starttime = Timing::time();
        
        engine->performScheduledConnectDisconnect();
        
        engine->synchGraph();

        //process audio
        for(std::list<Gear*>::iterator it=engine->_gearsToProcess.begin();it!=engine->_gearsToProcess.end();++it)
            (*it)->runAudio();

        //process video
        if (real_time >= (currentFrame*_videoInfo.timePerFrame()*1000.0f))
        {
            for(std::list<Gear*>::iterator it=engine->_gearsToProcess.begin();it!=engine->_gearsToProcess.end();++it)
            {
                (*it)->runVideo();
            }

            currentFrame++;
        }

        //nsamples-=_signalInfo.blockSize();
        //the currentTime of the engine
        engine->_currentTime += (engine->_signalInfo.blockSize() * engine->_signalInfo.timePerSample());


        block_endtime = Timing::time();
        //the real time that this loop have taken
        real_time = block_endtime - real_starttime;

        //compute AverageLoad
        block_time = block_endtime - block_starttime;
        cumul_load += (block_time /  block_targettime) * 100.0f;
        engine->_AverageLoad =  cumul_load / blockIt;

        //temp
        // if (!(blockIt%100))
//             std::cout << "AverageLoad " << engine->_AverageLoad << "%" << std::endl;

        if (!(blockIt%10))
            std::cout << "AverageLoad " << engine->_AverageLoad << "%" << std::endl;
//

        //to maintain real-time processing we have to sleep
        //for the difference between the time we are supposed to be at (_currentTime, the engine time)
        //and the time we are really at (real_time)
        sleeptime = ( (engine->_currentTime*1000.0f) - real_time);

        if (sleeptime > 0)
            Timing::sleep((int)sleeptime);

        blockIt++;
    }

    for(std::list<Gear*>::iterator it=engine->_Gears.begin();it!=engine->_Gears.end();++it)    
        (*it)->internalPostPlay();

    return NULL;
}

void Engine::synchGraph()
{
	if(_graphSynched)
		return;
    
    _gearsToProcess.clear();

	std::vector<Gear*> gears;
	
    for(std::list<Gear*>::iterator it=_Gears.begin();it!=_Gears.end();++it)
    {
        if ((*it)->ready())        
            gears.push_back(*it);
    }
     		
	uint nbox=gears.size();
	
	signed char* depmat=new signed char [nbox*nbox];
	
	// we simply fill depmat
	// depmat[i][j] will == 1 only if
	// gears[i] depends of gears[j]

	bool *done;
	done=new bool[nbox];
	memset(done,0,sizeof(bool)*nbox);
    
	std::vector<Gear*> vec;
	for(uint i=0;i<nbox;i++)
	{
		vec.clear();
		gears[i]->getDependencies(vec);
		
		for(uint j=0;j<nbox;j++)
			depmat[nbox*i+j]=(find(vec.begin(),vec.end(),gears[j]) != vec.end());
			
	}
	
	
	// this algorithm is **sub-sub-optimal**
	// order the box in processing order
	for(uint k=0;k<nbox;k++)
		for(uint i=0;i<nbox;i++)
		{
            bool isReady = !done[i];
			if(isReady)
				for(uint j=0;j<nbox;j++)
					if(depmat[nbox*i+j]!=0)
					{
						isReady=false;
						break;
					}
			//add the box in the queue
			if(isReady)
			{
				_gearsToProcess.push_back(gears[i]);
				done[i]=true;
				for(uint i2=0;i2<nbox;i2++)
					depmat[nbox*i2+i]=0;
			}
		}			

		
	delete []depmat;
	delete []done;
	
	_graphSynched=true;
//	for(it=orderedGears.begin();it!=orderedGears.end();++it)
//		_TRACE("@"<<(*it)->name()<<std::endl);
	//_Gears = orderedGears;
}

void Engine::scheduleConnection(Plug *plugA, Plug *plugB)
{
    _scheduledsConnectDisconnect.push_back( ScheduledConnectDisconnect(plugA, plugB,  ScheduledConnectDisconnect::CONNECT));

    //peform now if not playing
    if (!_playing)
        performScheduledConnectDisconnect();
    
}

void Engine::scheduleDisconnection(Plug *plugA, Plug *plugB)
{
    _scheduledsConnectDisconnect.push_back( ScheduledConnectDisconnect(plugA, plugB,  ScheduledConnectDisconnect::DISCONNECT));

    //peform now if not playing
    if (!_playing)
        performScheduledConnectDisconnect();

}

void Engine::performScheduledConnectDisconnect()
{
    for (std::vector<ScheduledConnectDisconnect>::iterator it=_scheduledsConnectDisconnect.begin(); it != _scheduledsConnectDisconnect.end(); ++it)
    {
        if ((*it).connectDisconnect == ScheduledConnectDisconnect::CONNECT)
        {
            (*it).A->connect((*it).B);
        }
        else
        {
            (*it).A->disconnect((*it).B);
        }
    }

    _scheduledsConnectDisconnect.clear();
}

void Engine::getAllConnections(std::list<Connection*> &connections)
{
	std::list<Plug*> outputs;
    std::list<Plug*> connectedPlugs;
    for(std::list<Gear*>::iterator itGear = _Gears.begin(); itGear != _Gears.end(); ++itGear)
    {        
        (*itGear)->getOutputs(outputs);
        for(std::list<Plug*>::iterator itOutput = outputs.begin(); itOutput != outputs.end(); ++itOutput)
        {                        
            (*itOutput)->connectedPlugs(connectedPlugs);
            
            for(std::list<Plug*>::iterator itConnectedPlug = connectedPlugs.begin(); itConnectedPlug != connectedPlugs.end(); ++itConnectedPlug)
            {                
                connections.push_back( new Connection((*itGear)->name(), (*itOutput)->name(), 
                                                      (*itConnectedPlug)->parent()->name(), 
                                                      (*itConnectedPlug)->name()));
            }            
        }
    }
}

void Engine::saveSchema(std::string filename)
{
    QDomDocument doc("DroneSchema");
    QDomElement rootElem = doc.createElement("Schema");
    doc.appendChild(rootElem);


    //save all gears
    QDomElement gearsElem = doc.createElement("Gears");
    rootElem.appendChild(gearsElem);
        
    for(std::list<Gear*>::iterator it=_Gears.begin();it!=_Gears.end();++it)
    {
        (*it)->internalSave(doc, gearsElem);            
    }


    //save all connections
    QDomElement connectionsElem = doc.createElement("Connections");
    rootElem.appendChild(connectionsElem);
    
    std::list<Connection*> connections;
    getAllConnections(connections);
    
    for(std::list<Connection*>::iterator it = connections.begin(); it != connections.end(); ++it)
    {
        (*it)->save(doc, connectionsElem);
        delete (*it);//free
    }
    
    //save to file
    QFile file(filename.c_str());
    if (file.open(IO_WriteOnly))
    {
        QTextStream stream(&file);
        doc.save(stream,4);
        file.close();
    }

}


void Engine::loadSchema(std::string filename)
{
    QDomDocument doc("DroneSchema");

    QFile file(filename.c_str());

    if (!file.open(IO_ReadOnly))
    {
        std::cout << "Fail to open file " << filename << std::endl;
        return;
    }
     

    QString errMsg;
    int errLine;
    int errColumn;
    if (!doc.setContent(&file, true, &errMsg, &errLine, &errColumn))
    {
        std::cout << "parsing error in " << filename << std::endl;
        std::cout << errMsg.ascii() << std::endl;
        std::cout << "Line: " <<  errLine << std::endl;
        std::cout << "Col: " <<  errColumn << std::endl;
        file.close();
        return;
    }

    file.close();

    //load gears
    QDomNodeList gearsNode = doc.elementsByTagName("Gears");

    //be sure we have exactly one Gears tag
    if (gearsNode.count()==1)
    {
        //iteration on all gears
        QDomNode gearNode = gearsNode.item(0).firstChild();
        Gear *pgear=NULL;
        while (!gearNode.isNull())
        {
            QDomElement gearElem = gearNode.toElement();
            if(!gearElem.isNull())
            {            
                pgear = addGear(gearElem.attribute("Type","").ascii());
                if (pgear!=NULL)                        
                {                                
                    pgear->internalLoad(gearElem);                                
                }

            }
            gearNode = gearNode.nextSibling();
        }                
    }
    else
        std::cout << "Bad DroneSchema : problem with <Gears>" << std::endl;


    //load connections    
    QDomNodeList connectionsNode = doc.elementsByTagName("Connections");

    //be sure we have exactly one Connections tag
    if (connectionsNode.count()==1)
    {
        //iteration on all connections
        QDomNode connectionNode = connectionsNode.item(0).firstChild();
        Connection connection;
        while (!connectionNode.isNull())
        {
            QDomElement connectionElem = connectionNode.toElement();

            if(!connectionElem.isNull())
            {            
                connection.load(connectionElem);            
                connectPlugs(connection);
            }

            connectionNode = connectionNode.nextSibling();
        }               
    }
    else
       std::cout << "Bad DroneSchema : problem with <Connections>" << std::endl;


}

void Engine::getAllGears(std::list<Gear*> &gears)
{
    gears.clear();
    gears.assign(_Gears.begin(), _Gears.end());
}

Gear* Engine::getGear(std::string name) const
{
    for(std::list<Gear*>::const_iterator it = _Gears.begin();it!=_Gears.end();++it)    
    {
        if ((*it)->name() == name)
            return (*it);        
    }

    return NULL;
}

void Engine::connectPlugs(Connection &connection)
{
    Gear *gearA;
    Gear *gearB;
    Plug *input;
    Plug *output;

    if ( (gearA=getGear(connection.gearA())) == NULL)
    {
        std::cout << "connectPlugs fail: " + connection.gearA() + " not found!" << std::endl;
        return;
    }
     
        
    if ( (gearB=getGear(connection.gearB())) == NULL)
    {
        std::cout << "connectPlugs fail: " + connection.gearB() + " not found!" << std::endl;
        return;
    }

    if ( (output=gearA->getOutput(connection.output())) == NULL)
    {
        std::cout << "connectPlugs fail: " + connection.output() + " not found!" << std::endl;
        return;
    }

    if ( (input=gearB->getInput(connection.input())) == NULL)
    {
        std::cout << "connectPlugs fail: " + connection.input() + " not found!" << std::endl;
        return;
    }

    if (!output->connect(input))
    {
        std::cout << "connectPlugs fail!" << std::endl;
    }

}
