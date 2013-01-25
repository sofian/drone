/* 
 * File:   CommandMoveItems.h
 * Author: julien
 *
 * Created on January 9, 2013, 2:25 PM
 */

#ifndef COMMANDMOVEITEMS_H
#define	COMMANDMOVEITEMS_H

#include <QUndoCommand>
#include <QList>
#include <QString>
#include <QPointF>

class CommandMoveItems: public QUndoCommand {
public:
  CommandMoveItems(QList<QString> &itemList,QPointF delta);
  virtual ~CommandMoveItems();
  
  void undo();
  void redo();

protected:
  QList<QString> _itemList;
  QPointF _delta;
  bool _skipFirstRedo;

};

#endif	/* COMMANDMOVEITEMS_H */

