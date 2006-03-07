#include "GuileBindings.h"


QApplication *GuileBindings::_qtApp = 0;
MainWindow *GuileBindings::_mainWindow = 0;
Schema *GuileBindings::_mainSchema = 0;

void GuileBindings::init(int argc, char **argv)
{
  _qtApp = new QApplication(argc, argv);
  QApplication::setGlobalMouseTracking(TRUE);
  
  //init the drone core
  DroneCore::init();
	
  _mainWindow = new MainWindow();
  _mainWindow->adjustSize();
  _qtApp->setMainWidget(_mainWindow);

  _mainWindow->show();

  _mainSchema = _mainWindow->_metaGearEditor->schemaGui()->getSchema();
  registerProcedures();

  gh_repl(argc, argv);

}

void GuileBindings::release()
{
  DroneCore::release();

  delete _qtApp;
  delete _mainWindow;
}

void GuileBindings::registerProcedures()
{
  gh_new_procedure0_0("start-playing",   startPlaying);
  gh_new_procedure0_0("stop-playing",    stopPlaying);
  gh_new_procedure0_1("make-gear",       makeGear);
  gh_new_procedure0_2("make-gear",       makeGear);
  gh_new_procedure0_1("gear-name",       gearName);
  gh_new_procedure0_0("all-gears",       allGears);
  gh_new_procedure0_1("gear-plug-ins",   gearPlugIns);
  gh_new_procedure0_1("gear-plug-outs",  gearPlugOuts);
  gh_new_procedure0_1("plug-name",       plugName);
  gh_new_procedure0_2("connect",         connect);
}

SCM GuileBindings::startPlaying()
{
  _mainWindow->slotPlay(true);
  _qtApp->exec();
  return SCM_UNSPECIFIED;
}

SCM GuileBindings::stopPlaying()
{
  _mainWindow->slotPlay(false);
  return SCM_UNSPECIFIED;
}

SCM GuileBindings::makeGear(SCM s_type)
{
  SCM_ASSERT(SCM_STRINGP(s_type), s_type, SCM_ARG1, "make-gear");
	char *type=0;
	size_t size=0;
	type=gh_scm2newstr(s_type,&size);	
//   SCM_ASSERT(SCM_NUMBERP(s_posX), s_posX, SCM_ARG2, "add-gear");
//   int posX = SCM_INUM(s_posX);
//   int posY = SCM_INUM(s_posY);
//   SCM_ASSERT(SCM_NUMBERP(s_posY), s_posY, SCM_ARG3, "add-gear");
  Gear *newGear = _mainSchema->addGear(std::string(type));
  return SCM_MAKINUM((int)newGear);
}

SCM GuileBindings::makeGear(SCM s_type, SCM s_properties)
{
  SCM_ASSERT(SCM_STRINGP(s_type), s_type, SCM_ARG1, "make-gear");
  SCM_ASSERT(scm_list_p(s_properties), s_properties, SCM_ARG2, "make-gear");
	char *type=0;
	size_t size=0;
	type=gh_scm2newstr(s_type,&size);	
  Gear *newGear = _mainSchema->addGear(std::string(type));
  std::vector<Property*> prop;
  newGear->settings().getAll(&prop);
  
  /* Check that it is a list */ 
  if (SCM_NFALSEP(scm_list_p(s_properties)))
  {
    /* Find its length */
    size_t list_length = scm_num2ulong(scm_length(s_properties), 0, "make-gear");
	
    int i = 0;
    /* Iterate through the list */ 
    while (SCM_FALSEP (scm_null_p (s_properties)))
    {
      
	    /* Get the head of the list */
      SCM head = SCM_CAR(s_properties);
      prop[i++]->valueStr(gh_scm2newstr(head, &size));
      
	    /* Discard the head of the list */
	    s_properties = SCM_CDR(s_properties);
    }
  }
  
  return SCM_MAKINUM((int)newGear);
}

SCM GuileBindings::gearName(SCM s_gearId)
{
  SCM_ASSERT(SCM_NUMBERP(s_gearId), s_gearId, SCM_ARG1, "gear-name");
  Gear *gear = (Gear*) SCM_INUM(s_gearId);
  return scm_makfrom0str(gear->name().c_str());
}

SCM GuileBindings::renameGear(SCM s_gearid, SCM s_newname)
{
  
}

SCM GuileBindings::allGears()
{
  std::list<Gear*> allGears = _mainSchema->getGears();
  SCM scmAllGears = SCM_EOL;
  for (std::list<Gear*>::iterator it=allGears.begin(); it!=allGears.end(); ++it)
    scmAllGears = scm_append( scm_list_2( scmAllGears, scm_list_1(SCM_MAKINUM((int)*it)) ) );
  return scmAllGears;
}

SCM GuileBindings::plugName(SCM s_plugId)
{
  SCM_ASSERT(SCM_NUMBERP(s_plugId), s_plugId, SCM_ARG1, "plug-name");
  AbstractPlug *plug = (AbstractPlug*) SCM_INUM(s_plugId);
  return scm_makfrom0str(plug->name().c_str());
}

SCM GuileBindings::gearPlugIns(SCM s_gearId)
{
  SCM_ASSERT(SCM_NUMBERP(s_gearId), s_gearId, SCM_ARG1, "gear-plug-ins");
  Gear *gear = (Gear*) SCM_INUM(s_gearId);
  std::list<AbstractPlug*> inputs;
  gear->getInputs(inputs);
  SCM scmInputs = SCM_EOL;
  for (std::list<AbstractPlug*>::iterator it=inputs.begin(); it!=inputs.end(); ++it)
    scmInputs = scm_append( scm_list_2( scmInputs, scm_list_1(SCM_MAKINUM((int)*it)) ) );
  return scmInputs;
}

SCM GuileBindings::gearPlugOuts(SCM s_gearId)
{
  SCM_ASSERT(SCM_NUMBERP(s_gearId), s_gearId, SCM_ARG1, "gear-plug-outs");
  Gear *gear = (Gear*) SCM_INUM(s_gearId);
  std::list<AbstractPlug*> outputs;
  gear->getOutputs(outputs);
  SCM scmOutputs = SCM_EOL;
  for (std::list<AbstractPlug*>::iterator it=outputs.begin(); it!=outputs.end(); ++it)
    scmOutputs = scm_append( scm_list_2( scmOutputs, scm_list_1(SCM_MAKINUM((int)*it)) ) );
  return scmOutputs;
}

SCM GuileBindings::connect(SCM s_plugIdA, SCM s_plugIdB)
{
  SCM_ASSERT(SCM_NUMBERP(s_plugIdA), s_plugIdA, SCM_ARG1, "connect");
  SCM_ASSERT(SCM_NUMBERP(s_plugIdB), s_plugIdB, SCM_ARG2, "connect");
  AbstractPlug *plugA = (AbstractPlug*) SCM_INUM(s_plugIdA);
  AbstractPlug *plugB = (AbstractPlug*) SCM_INUM(s_plugIdB);
  _mainSchema->connect(plugA, plugB);
  return SCM_UNSPECIFIED;
}
