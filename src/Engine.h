#ifndef ENGINE_INCLUDED
#define ENGINE_INCLUDED

#include <list>
#include <vector>
#include <string>
#include <pthread.h>


#include "SignalInfo.h"
#include "VideoInfo.h"

class Gear;
class Plug;
class QDomDocument;
class QDomElement;

#define MAX(a,b) (((a) > (b)) ? a : b)
#define MIN(a,b) (((a) < (b)) ? a : b)

class Engine  
{
public:

    class Connection
    {
    public:
        Connection(){};        
        Connection(std::string gearA, std::string output, std::string gearB, std::string input) :
            _gearA(gearA),
            _input(input),
            _gearB(gearB),
            _output(output)
        {
        }
        
        std::string gearA(){return _gearA;};
        std::string input(){return _input;};
        std::string gearB(){return _gearB;};
        std::string output(){return _output;};

        void save(QDomDocument &doc, QDomElement &parent);
        void load(QDomElement &connectionElem);

    private:
        std::string _gearA;
        std::string _input;
        std::string _gearB;
        std::string _output;

    };

    class ScheduledConnectDisconnect
    {    
    public:
        enum eConnectDisconnect {CONNECT, DISCONNECT};
        
        ScheduledConnectDisconnect(Plug *a, Plug *b, eConnectDisconnect conn) :
            A(a),
            B(b),
            connectDisconnect(conn)
        {
        };
            
        Plug *A;
        Plug *B;
        eConnectDisconnect connectDisconnect;//connection or disconnection
    };


    Engine(int hwnd);
	virtual ~Engine();
       
    void saveSchema(std::string filename);
    void clearSchema();
    void loadSchema(std::string filename);

	Gear* addGear(std::string geartype, std::string name);
    Gear* addGear(std::string gearname);    
	void removeGear(Gear* gear);
    void removeAllGears();

    void getAllGears(std::list<Gear*> &gears);
    Gear* getGear(std::string name) const;

    void getAllConnections(std::list<Connection*> &connections);
 
    void connectPlugs(Connection &connection);


    static const SignalInfo& signalInfo() {return _signalInfo;};
    static const VideoInfo& videoInfo() {return _videoInfo;};
    
	std::string getNewGearName(std::string prefix);

	void startPlaying();
    void stopPlaying();
    

	Time_T currentTime();
	
    int hWnd(){return _hWnd;};
    static void *playThread(void *parent);

    void needSynch() {_graphSynched=false;};
    
    void scheduleConnection(Plug *plugA, Plug *plugB);
    void scheduleDisconnection(Plug *plugA, Plug *plugB);
    void scheduleGearDeletion(Gear *gear);



protected:
	
    void performScheduledConnectDisconnect();
    void performScheduledGearDeletion();
    int _hWnd;
  
    float _AverageLoad;

    static SignalInfo _signalInfo;
    static VideoInfo _videoInfo;

    void synchGraph();
   
	

    Time_T _currentTime;

   
private:
	
    bool _playing;
    bool _graphSynched;
    std::list<Gear*> _Gears;
    
    // is sorted in processing order when _GraphSynched==true
    //only contain gears->ready()    
    std::list<Gear*> _gearsToProcess;

    pthread_t _playThreadHandle;

    std::vector<ScheduledConnectDisconnect> _scheduledsConnectDisconnect;
    std::vector<Gear*> _scheduledsGearDeletion;

};



#endif
