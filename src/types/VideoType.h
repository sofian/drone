#ifndef VIDEOTYPE_INCLUDED
#define VIDEOTYPE_INCLUDED

#include "ColorSpace.h"
#include "MatrixType.h"

class VideoTypeRGBA : public AbstractType
{
public:
  VideoTypeRGBA()
  {
    addSubType(_image);  
  }

  virtual ~VideoTypeRGBA(){}
  
  std::string name() const { return "VideoTypeRGBA";}
  QColor color() const { return QColor(40,60,114);}

  MatrixType<RGBA>* image() { return &_image;}  

private:
  MatrixType<RGBA> _image;
};

#endif //VIDEOTYPE_INCLUDED
