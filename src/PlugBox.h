#ifndef PLUGBOX_INCLUDED
#define PLUGBOX_INCLUDED

#include <qpainter.h>
#include <string>
#include <vector>
class Engine;
class Plug;
class GearGui;
class ConnectionItem;


class PlugBox
{
public:
    enum eStatus {NORMAL, HILIGHTED};
    
    static const int PLUGBOX_SIZE;
    
    PlugBox(Plug *plug, GearGui *gearItem, Engine *engine);
    virtual ~PlugBox();

    const QColor& PlugBox::color();
    void draw(int x, int y, int gearSizeX, QPainter &painter);
    bool hitted(int x, int y);
    void hilight(bool hiLight);

    bool connect(PlugBox *plugBox, ConnectionItem *connectionItem);
    void assignConnectionOnly(PlugBox *plugBox, ConnectionItem *connectionItem);
    bool canConnectWith(PlugBox *plugBox);    
    void disconnect(ConnectionItem *connectionItem, bool deleteConnectionItem=true);
    void disconnectAll();
    
    Plug *plug(){return _plug;};

    std::string test(){return "test";};
    
    
    int connectionHandleX();
    int connectionHandleY();

    int x(){return _x;};
    int y(){return _y;};
    

private:    
    
    void doHilight();
    void doUnlight();

    Plug *_plug;   
    GearGui *_gearGui;
    Engine *_engine;
    int _x, _y;    
    eStatus _status;
    int _hilightScaling;

    std::vector<ConnectionItem*> _connectionItems;
    
    static const int CONNECTION_HANDLE_OFFSETX;
    static const int CONNECTION_HANDLE_OFFSETY;
    static const int PLUG_NAME_NB_CHARS;
    //colors
    static const QColor SIGNAL_PLUG_COLOR;   
    static const QColor VIDEO_PLUG_COLOR;   
    static const QColor VIDEO_COMPOSE_PLUG_COLOR;   
    static const int MAX_HILIGHTSCALING;

    //fonts
    static const QFont SHORTNAME_FONT;
    static const QFont SHORTNAME_FONT_BOLD;

};

#endif
