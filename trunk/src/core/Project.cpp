#include "Project.h"
#include "Engine.h"
#include "error.h"
#include "SchemaGui.h"

#include <iostream>
#include <qdom.h>
#include <qfile.h>
#include <qtextstream.h>

Project::Project(SchemaGui* mainSchemaGui):_mainSchemaGui(mainSchemaGui)
{}

void Project::newProject()
{
  _mainSchemaGui->clear();
  _projectName="";
}

bool Project::save()
{
  ASSERT_ERROR(_projectName.size());
  return saveAs(_projectName);
}

bool Project::saveAs(std::string filename)
{
  QDomDocument doc("DroneProject");
  
  QDomElement projectElem = doc.createElement("Project");
  doc.appendChild(projectElem);

  if(!_mainSchemaGui->save(doc, projectElem))
    return false;

  //save to file  
       
  QFile file(filename.c_str());
  if (file.open(IO_WriteOnly))
  {
    QTextStream stream(&file);
    doc.save(stream,4);
    file.close();
  }
  else
  {
    std::cout << "file io error, cannot save!" << std::endl;
    return false;
  }

  _projectName=filename;
  return true;

}

bool Project::load(std::string filename)
{
  QDomDocument doc("DroneProject");

  QFile file(filename.c_str());

  if (!file.open(IO_ReadOnly))
  {
    std::cout << "Fail to open file " << filename << std::endl;
    return false;
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
    return false;
  }

  file.close();

  
  QDomNode projectNode = doc.firstChild();
  QDomNode schemaNode = projectNode.firstChild();
  
  if (schemaNode.isNull())
  {
    std::cout << "Bad drone project, main schema node isNull" << std::endl;
    return false;
  }
  
  QDomElement schemaElem = schemaNode.toElement();
  
  if (schemaElem.isNull())
  {
    std::cout << "Bad drone project, main schema elem isNull" << std::endl;
    return false;
  }
  
  if(!_mainSchemaGui->load(schemaElem))
    return false;
  
  _projectName=filename;
  return true;
}


