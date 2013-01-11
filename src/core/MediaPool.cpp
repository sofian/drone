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
  
	if (_medias.contains(media->uniqueName()))
		return false;

  _medias[media->uniqueName()]=media;

  return true;
}

void MediaPool::deleteMedia(Media* media)
{
  if (!media)
    return;

	_medias.remove(media->uniqueName());
}

void MediaPool::empty()
{
  _medias.clear();
}

Media* MediaPool::getMedia(QString name)
{  
	return _medias.value(name, NULL);
}

QList<Media*> MediaPool::getAllMedia()
{
  return _medias.values();
}


MediaPool *MediaPool::getInstance()
{
  if (!_instance)  
    _instance = new MediaPool();
  
  return _instance;
}


Media* MediaPool::createMediaFromFile(QString)
{
  return NULL;
}

