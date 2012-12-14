#ifndef MEDIAPOOLITEM_INCLUDED
#define MEDIAPOOLITEM_INCLUDED

#include <q3iconview.h>

class Media;

class MediaPoolItem : public Q3IconViewItem
{
public:
  MediaPoolItem(Q3IconView *parent, Media *media);
  ~MediaPoolItem();

private:
  Media *_media;

};

#endif
