#include "Project.h"
#include "Engine.h"
#include "error.h"
#include "Schema.h"

#include <iostream>
#include <QtXml>
#include <qfile.h>
#include <qtextstream.h>

Project::Project(Schema* mainSchema):
	_mainSchema(mainSchema)
{}

void Project::newProject()
{
  _mainSchema->clear();
  _projectName="";
  //todo:
}

bool Project::save()
{
  ASSERT_ERROR(_projectName.size());
  return saveAs(_projectName);
}

bool Project::saveAs(QString filename)
{
  QDomDocument doc("DroneProject");
  
  QDomElement projectElem = doc.createElement("Project");
  doc.appendChild(projectElem);

  if(!_mainSchema->save(doc, projectElem))
    return false;

  //save to file  
       
  QFile file(filename);
  if (file.open(QIODevice::WriteOnly))
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

bool Project::load(QString filename)
{
  QDomDocument doc("DroneProject");

  QFile file(filename);

  if (!file.open(QIODevice::ReadOnly))
  {
    qCritical() << "Fail to open file " << filename;
    return false;
  }

  QString errMsg;
  int errLine;
  int errColumn;
  if (!doc.setContent(&file, true, &errMsg, &errLine, &errColumn))
  {
    qCritical() << "parsing error in " << filename;
    qCritical() << errMsg;
    qCritical() << "Line: " <<  errLine;
    qCritical() << "Col: " <<  errColumn;
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
  
  if(!_mainSchema->load(schemaElem))
    return false;
  
  _projectName=filename;
  return true;
}


