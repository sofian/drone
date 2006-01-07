#ifndef MEDIA_INCLUDED
#define MEDIA_INCLUDED

#include <qpixmap.h>


class Media
{
public:
  
  enum MediaType {AUDIO, MOVIE};
  
  Media(MediaType type) : _type(type){}

  virtual ~Media(){}
  
  virtual QPixmap *thumbNail()=0;
    
  virtual void load(QString filename)=0;
      
  QString displayName(){return _displayName;}
 
  QString uniqueName(){return _uniqueName;}

  QString filename(){return _filename;}

  QString mediaInfo(){return _mediaInfo;}

  MediaType type(){return _type;}
  long lengthSeconds(){return _lengthSeconds;}
    
private:
  
  QString _displayName;
  QString _uniqueName;  
  QString _filename;  
  QString _mediaInfo;
  MediaType _type;
  long  _lengthSeconds;
};

#endif
