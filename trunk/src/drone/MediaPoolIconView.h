#ifndef MEDIAPOOLICONVIEW_INCLUDED
#define MEDIAPOOLICONVIEW_INCLUDED

#include <qiconview.h>

class MediaPoolItem;

class MediaPoolIconView : public QIconView
{
public:  
  
  MediaPoolIconView(QWidget *parent);
  virtual ~MediaPoolIconView();

  void addItem(MediaPoolItem *item);

private:


};

#endif
