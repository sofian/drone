#ifndef MEDIAPOOL_INCLUDED
#define MEDIAPOOL_INCLUDED
  
#include <vector>
#include <string>
#include <map>

class Media;
  
class MediaPool
{
public:

  ~MediaPool();

  bool addMedia(Media* media);
  void deleteMedia(Media* media);
  void empty();

  Media* getMedia(std::string name);
  std::vector<Media*> getAllMedia();

  MediaPool *getInstance();

  static Media* createMediaFromFile(std::string filename);

private:
  MediaPool() : _instance(0){}
  
  std::map<std::string, Media*> _medias;

  MediaPool *_instance;
};

#endif


