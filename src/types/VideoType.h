/* VideoRGBAType.h
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
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

#ifndef VIDEORGBATYPE_INCLUDED
#define VIDEORGBATYPE_INCLUDED

#include "ColorSpace.h"
#include "MatrixType.h"

class VideoRGBAType : public MatrixType<RGBA>
{
public:
  VideoRGBAType(int width = 0,
                int height = 0,
                RGBA fillValue = BLACK_RGBA)
    : MatrixType<RGBA>(width, height, fillValue),
      _isGray(false)
  {
  }

  virtual ~VideoRGBAType() {}

  // returns an antialiased pixel. Warning: No bound checking is done!
//  inline void getAAPixel(float x, float y ,RGBA * pix) const;
  std::string name() const { return "VideoRGBAType";}
  QColor color() const { return QColor(40,60,114);}

  bool isGray() const { return _isGray; }
  void setIsGray(bool isGray) { _isGray = isGray; }
  
protected:
  // GetAAPixel related functions ////////////////////
  // fractionnary part of coords for getAAPixel
  mutable float _fracx,_fracy,_fp1,_fp2,_fp3,_fp4;
  mutable int _intx,_inty;
  // pixels #1 and #3 for getAAPixel
  mutable RGBA* _p1,_p2,_p3,_p4;
  bool _isGray;
  ////////////////////////////////////////////////
};

#endif //VIDEOTYPE_INCLUDED
