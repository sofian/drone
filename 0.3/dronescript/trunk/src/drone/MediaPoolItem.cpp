#include "MediaPoolItem.h"

#include "Media.h"
  
MediaPoolItem::MediaPoolItem(QIconView *parent, Media *media) :
  QIconViewItem(parent),
  _media(media)
{
  setPixmap(*(media->thumbNail()));
  setText("the return of the space cowboy");
}

MediaPoolItem::~MediaPoolItem()
{
}

