#include "MediaPoolIconView.h"
#include "MediaPoolItem.h"
#include "backgroundMovie.xpm"

 
MediaPoolIconView::MediaPoolIconView(QWidget *parent) : 
  QIconView(parent)
{   
  setResizeMode(QIconView::Adjust);
  setMinimumWidth(460);
  setMinimumHeight(370);
  setSpacing(15);
  viewport()->setPaletteBackgroundPixmap(QPixmap(backgroundMovie_xpm));
}

MediaPoolIconView::~MediaPoolIconView()
{
}

void MediaPoolIconView::addItem(MediaPoolItem *item)
{
  insertItem(item);
}
