#ifndef GUILE_BINDINGS_INC
#define GUILE_BINDINGS_INC

#include <qapplication.h>
#include "DroneCore.h"
#include "MainWindow.h"
#include "SchemaEditor.h"
#include "MetaGearEditor.h"
#include "MetaGear.h"
#include "Engine.h"
#include "Gear.h"

#include <guile/gh.h>

class MainWindow;

class GuileBindings {
  
 protected:
  static QApplication *_qtApp;
  static MainWindow *_mainWindow;
  static Schema *_mainSchema;
  
 public:
  GuileBindings(int argc, char **argv);
  
  static void init(int argc, char **argv);
  static void release();
  
  static void registerProcedures();

  static SCM startPlaying();
  static SCM stopPlaying();

  static SCM makeGear(SCM s_type);
  static SCM makeGear(SCM s_type, SCM s_properties);
  static SCM gearName(SCM s_gearId);
  static SCM renameGear(SCM s_gearid, SCM s_newname);
  static SCM gearPlugIns(SCM s_gearId);
  static SCM gearPlugOuts(SCM s_gearId);
  static SCM gearSettings(SCM s_gearId);
  static SCM allGears();

  static SCM plugName(SCM s_plugId);
  static SCM connect(SCM s_plugIdA, SCM s_plugIdB);
  //static SCM allConnections();
  
  //  static SCM settingName(SCM s_settingId);
/*   static SCM makeConnection */
/*   static SCM connect(SCM s_name);//, SCM s_posX, SCM s_posY); */
};

#endif
