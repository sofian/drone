#ifndef VIDEOTYPE_INCLUDED
#define VIDEOTYPE_INCLUDED

#include "ColorSpace.h"
#include "MatrixType.h"

class VideoTypeRGBA : public MatrixType<RGBA>
{
public:
  VideoTypeRGBA(int width = 0,
                int height = 0,
                RGBA fillValue = BLACK_RGBA)
    : MatrixType<RGBA>(width, height, fillValue),
      _isGray(false)
  {
  }

  virtual ~VideoTypeRGBA() {}

  // returns an antialiased pixel. Warning: No bound checking is done!
//  inline void getAAPixel(float x, float y ,RGBA * pix) const;
  std::string name() const { return "VideoTypeRGBA";}
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
