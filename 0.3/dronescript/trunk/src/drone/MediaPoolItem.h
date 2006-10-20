#ifndef MEDIAPOOLITEM_INCLUDED
#define MEDIAPOOLITEM_INCLUDED

#include <qiconview.h>

class Media;

class MediaPoolItem : public QIconViewItem
{
public:
  MediaPoolItem(QIconView *parent, Media *media);
  ~MediaPoolItem();

private:
  Media *_media;

};

#endif
