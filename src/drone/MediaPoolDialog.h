#ifndef MEDIAPOOLDIALOG_INCLUDED
#define MEDIAPOOLDIALOG_INCLUDED
                               
#include <qdialog.h>
#include <qlayout.h>
#include <qpushbutton.h>

#include "MediaPoolIconView.h"

class MediaPoolDialog : public QWidget
{
public:  
  MediaPoolDialog(QWidget *parent);
  ~MediaPoolDialog();
  
private:
  QVBoxLayout *_verticalLayout;
  QHBoxLayout *_horizontalLayout;
  MediaPoolIconView *_mediaPoolIconView;  
  QPushButton *_importButton;
  QPushButton *_movieButton;
  QPushButton *_pictureButton;
};

                       
#endif
