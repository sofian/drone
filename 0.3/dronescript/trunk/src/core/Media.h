#ifndef MEDIA_INCLUDED
#define MEDIA_INCLUDED

#include <qpixmap.h>
#include <string>

class Media
{
public:
  
  enum MediaType {AUDIO, MOVIE};
  
  Media(MediaType type) : _type(type){}

  virtual ~Media(){}
  
  virtual QPixmap *thumbNail()=0;
    
  virtual void load(std::string filename)=0;
      
  std::string displayName(){return _displayName;}
 
  std::string uniqueName(){return _uniqueName;}

  std::string filename(){return _filename;}

  std::string mediaInfo(){return _mediaInfo;}

  MediaType type(){return _type;}
  long lengthSeconds(){return _lengthSeconds;}
    
private:
  
  std::string _displayName;
  std::string _uniqueName;  
  std::string _filename;  
  std::string _mediaInfo;
  MediaType _type;
  long  _lengthSeconds;
};

#endif
