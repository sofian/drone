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
    : MatrixType<RGBA>(width, height, fillValue)
  {
  }

  virtual ~VideoTypeRGBA() {}
  
  std::string name() const { return "VideoTypeRGBA";}
  QColor color() const { return QColor(40,60,114);}
};

#endif //VIDEOTYPE_INCLUDED
