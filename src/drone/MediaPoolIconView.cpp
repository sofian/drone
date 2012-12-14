#include "MediaPoolIconView.h"
#include "MediaPoolItem.h"
#include "backgroundMovie.xpm"
//Added by qt3to4:
#include <QPixmap>

 
MediaPoolIconView::MediaPoolIconView(QWidget *parent) : 
  Q3IconView(parent)
{   
  setResizeMode(Q3IconView::Adjust);
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
