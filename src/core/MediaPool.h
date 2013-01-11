#ifndef MEDIAPOOL_INCLUDED
#define MEDIAPOOL_INCLUDED
  
#include <QMap>
#include <QString>

class Media;
  
class MediaPool
{
public:

  ~MediaPool();

  bool addMedia(Media* media);
  void deleteMedia(Media* media);
  void empty();

  Media* getMedia(QString name);
  QList<Media*> getAllMedia();

  MediaPool *getInstance();

  static Media* createMediaFromFile(QString filename);

private:
  MediaPool() : _instance(0){}
  
  QMap<QString, Media*> _medias;

  MediaPool *_instance;
};

#endif


