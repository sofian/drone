#include "MediaPool.h"
#include "Media.h"
#include "error.h"


MediaPool::~MediaPool()
{
  empty();
}


bool MediaPool::addMedia(Media* media)
{
  if (!media)
    return false;
  
  //verify for unique name
  std::map<std::string, Media*>::iterator it = _medias.find(media->uniqueName());

  if (it != _medias.end())
    return false;

  _medias[media->uniqueName()]=media;

  return true;
}

void MediaPool::deleteMedia(Media* media)
{
  if (!media)
    return;

  std::map<std::string, Media*>::iterator it = _medias.find(media->uniqueName());

  if (it != _medias.end())
  {
    delete media; 
    _medias.erase(it);
  }
  else
    ASSERT_ERROR_MESSAGE(false, "Media not found in the pool, potential memory leak");

}

void MediaPool::empty()
{
  for (std::map<std::string, Media*>::iterator it=_medias.begin();it!=_medias.end();++it)  
    delete it->second;

  _medias.clear();
}

Media* MediaPool::getMedia(std::string name)
{
  std::map<std::string, Media*>::iterator it = _medias.find(name);
  
  if (it != _medias.end())
    return it->second;
  else
    return NULL;
}

std::vector<Media*> MediaPool::getAllMedia()
{
  std::vector<Media*> mediaVec;

  for (std::map<std::string, Media*>::iterator it=_medias.begin();it!=_medias.end();++it)  
    mediaVec.push_back(it->second);
  
  return mediaVec;
}


MediaPool *MediaPool::getInstance()
{
  if (!_instance)  
    _instance = new MediaPool();
  
  return _instance;
}


Media* MediaPool::createMediaFromFile(std::string)
{
  return NULL;
}

