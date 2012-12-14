#ifndef MEDIAPOOLDIALOG_INCLUDED
#define MEDIAPOOLDIALOG_INCLUDED
                               
#include <qdialog.h>
#include <qlayout.h>
#include <qpushbutton.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3HBoxLayout>

#include "MediaPoolIconView.h"

class MediaPoolDialog : public QDialog
{
public:  
  MediaPoolDialog(QWidget *parent);
  ~MediaPoolDialog();
  
private:
  Q3VBoxLayout *_verticalLayout;
  Q3HBoxLayout *_horizontalLayout;
  MediaPoolIconView *_mediaPoolIconView;  
  QPushButton *_importButton;
  QPushButton *_movieButton;
  QPushButton *_pictureButton;
};

                       
#endif
