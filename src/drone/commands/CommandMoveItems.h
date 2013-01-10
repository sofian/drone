/* 
 * File:   CommandMoveItems.h
 * Author: julien
 *
 * Created on January 9, 2013, 2:25 PM
 */

#ifndef COMMANDMOVEITEMS_H
#define	COMMANDMOVEITEMS_H

#include <qundocommand>

class CommandMoveItems: public QUndoCommand {
public:
  CommandMoveItems();
  CommandMoveItems(const CommandMoveItems& orig);
  virtual ~CommandMoveItems();
private:

};

#endif	/* COMMANDMOVEITEMS_H */

