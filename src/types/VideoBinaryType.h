#ifndef VIDEOBINARYTYPE_INCLUDED
#define VIDEOBINARYTYPE_INCLUDED

#include "ColorSpace.h"
#include "MatrixType.h"

class VideoBinaryType : public MatrixType<bool>
{
public:
  static const bool WHITE;
  static const bool BLACK;

  VideoBinaryType(int width = 0,
                  int height = 0,
                  bool fillValue = VideoBinaryType::WHITE)
    : MatrixType<bool>(width, height, fillValue)
  {
  }

  virtual ~VideoBinaryType() {}

  std::string name() const { return "VideoBinaryType";}
  QColor color() const { return QColor(255,255,255);}
};

const bool VideoBinaryType::WHITE = false;
const bool VideoBinaryType::BLACK = true;

#endif //VIDEOBINARYTYPE_INCLUDED
