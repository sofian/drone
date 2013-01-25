#ifndef MEDIAMOVIE_INCLUDED
#define MEDIAMOVIE_INCLUDED

#include "Media.h"
#include <qpixmap.h>

class MediaMovie : public Media
{
public:  
  
  MediaMovie();
  virtual ~MediaMovie() {}
  
  
  QPixmap *thumbNail();
  void load(QString filename);

private:
  QPixmap *_thumbnail;
};

#endif
