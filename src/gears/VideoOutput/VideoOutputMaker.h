/* VideoOutputMaker.h
 * Copyright (C) 2004 Mathieu Guindon
 * This file is part of Drone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef VIDEOOUTPUT_MAKER_INCLUDED
#define VIDEOOUTPUT_MAKER_INCLUDED

#include <map>
#include <string>
#include <vector>

class VideoOutput;

class VideoOutputMaker  
{
public:
  VideoOutputMaker(QString type);

  static VideoOutput* makeVideoOutput(QString type);
  static void getAllVideoOutputsType(std::vector<QString> &videoOutputTypes);

private:
  static std::map<QString, VideoOutputMaker*> *_Registry;
  virtual VideoOutput* internalMakeVideoOutput()=0;
};

#define Register_VideoOutput(MMAKERNAME, MCLASSNAME, MTYPE) \
class MMAKERNAME : public VideoOutputMaker \
{ \
public: \
    MMAKERNAME() : VideoOutputMaker(MTYPE){}; \
private: \
    VideoOutput* internalMakeVideoOutput() \
    { \
        return new MCLASSNAME();\
    } \
    static MMAKERNAME _RegisterThis;\
};\
MMAKERNAME MMAKERNAME::_RegisterThis;



#endif 
