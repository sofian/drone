#ifndef VIDEOCHANNELTYPE_INCLUDED
#define VIDEOCHANNELTYPE_INCLUDED

#include "ColorSpace.h"
#include "MatrixType.h"

class VideoChannelType : public MatrixType<unsigned char>
{
public:
  VideoChannelType(int width = 0,
                   int height = 0,
                   unsigned char fillValue = 0)
    : MatrixType<unsigned char>(width, height, fillValue)
  {
  }

  virtual ~VideoChannelType() {}

  std::string name() const { return "VideoChannelType";}
  QColor color() const { return QColor(63,63,63);}
};

#endif //VIDEOCHANNELTYPE_INCLUDED
