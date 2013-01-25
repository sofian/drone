/* 
 * File:   CommandGeneric.h
 * Author: julien
 *
 * Created on January 21, 2013, 7:31 PM
 */

#ifndef COMMANDGENERIC_H
#define	COMMANDGENERIC_H
#include <QUndoCommand>
#include <QString>
#include <QtXml>

class CommandGeneric :public QUndoCommand {
public:
  CommandGeneric();
  void saveSnapshotAfter();
  
  virtual ~CommandGeneric();

  void undo();
  void redo();

private:
  QDomDocument _snapshotBefore;
  QDomDocument _snapshotAfter;
  bool _skipFirstRedo;

};

#endif	/* COMMANDGENERIC_H */

