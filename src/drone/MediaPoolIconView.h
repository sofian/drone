#ifndef MEDIAPOOLICONVIEW_INCLUDED
#define MEDIAPOOLICONVIEW_INCLUDED

#include <q3iconview.h>

class MediaPoolItem;

class MediaPoolIconView : public Q3IconView
{
public:  
  
  MediaPoolIconView(QWidget *parent);
  virtual ~MediaPoolIconView();

  void addItem(MediaPoolItem *item);

private:


};

#endif
