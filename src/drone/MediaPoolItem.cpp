#include "MediaPoolItem.h"

#include "Media.h"
  
MediaPoolItem::MediaPoolItem(Q3IconView *parent, Media *media) :
  Q3IconViewItem(parent),
  _media(media)
{
  setPixmap(*(media->thumbNail()));
  setText("the return of the space cowboy");
}

MediaPoolItem::~MediaPoolItem()
{
}

