#ifndef SCHEMAEDITOR_INCLUDED
#define SCHEMAEDITOR_INCLUDED

#include <qcanvas.h>
#include <qpopupmenu.h>
#include <string>
#include <vector>

class GearGui;
class ConnectionItem;
class Engine;

class SchemaEditor : public QCanvasView
{
Q_OBJECT

public:
        

    enum eStatus {IDLE, MOVING_GEAR, CONNECTING};
    
    SchemaEditor(QCanvas *canvas, QWidget *parent, Engine *engine);
    ~SchemaEditor();

    
    void addGear(std::string type, std::string name, int x, int y);
    void addGear(std::string type, int x, int y);
    void removeGear(GearGui* gearGui);
    void recreateSchemaFromEngine();
    void zoomIn();
    void zoomOut();
    void clearSchema();
    void loadSchema(std::string filename);
    //void saveSchema(std::string filename);
    //void loadSchema(std::string filename);

public slots:
    void slotMenuItemSelected(int id);
    void slotGearProperties();
    void slotGearDelete();

protected:
        
    void contentsMousePressEvent(QMouseEvent *mouseEvent);
    void contentsMouseMoveEvent(QMouseEvent *mouseEvent);       
    void contentsMouseReleaseEvent(QMouseEvent *mouseEvent);
    void contentsWheelEvent(QWheelEvent *wheelEvent);    
    void contentsMouseDoubleClickEvent(QMouseEvent *mouseEvent);
    void contextMenuEvent(QContextMenuEvent *contextMenuEvent);
    void moveGearBy(GearGui *gearItem, int x, int y);
    GearGui* testForGearCollision(const QPoint &p);
    ConnectionItem* testForConnectionCollision(const QPoint &p);
    void unHilightAllConnections();

    QPopupMenu* createGearsMenu();

private:
    static const std::string NAME;
    static const double ZOOM_FACTOR;

    Engine *_engine;
    eStatus _state;
    GearGui *_movingGear;
    QPoint _movingGearStartPos;
    double _zoom;

    ConnectionItem* _activeConnection;

    //popupmenus
    std::vector<std::string> _allGearsName;
    QPopupMenu *_allGearsMenu; 
    QPoint _allGearsMenuPos;
    QPopupMenu *_gearMenu;
    GearGui *_contextGear;//when the context menu of a gear is pop, this is the gear that make the menu pop
};

#endif
