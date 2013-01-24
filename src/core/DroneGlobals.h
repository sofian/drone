/* 
 * File:   Drone.h
 * Author: julien
 *
 * Created on January 22, 2013, 8:43 AM
 */

#ifndef DRONE_H
#define	DRONE_H
#include <qglobal.h>

namespace Drone
{
    enum LoadingModeFlag {
        UpdateWhenPossible = 0x1,
        DeleteUnvisitedElements = 0x2,
        EmitSignals = 0x4
    };
    Q_DECLARE_FLAGS(LoadingModeFlags, LoadingModeFlag)    

    extern LoadingModeFlags LoadFromFile; 
    extern LoadingModeFlags PasteFromClipboard;
    extern LoadingModeFlags RestoreSnapshot;

}
#endif	/* DRONE_H */

