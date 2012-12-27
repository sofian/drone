//
//  FrameBufferObject.h
//  drone
//
//  Created by Mathieu Guindon on 2012-12-26.
//  Copyright (c) 2012 drone. All rights reserved.
//

#ifndef __drone__FrameBufferObject__
#define __drone__FrameBufferObject__


class FrameBufferObject
{
public:
  FrameBufferObject();
  virtual ~FrameBufferObject();
  
  void create();
  void destroy();
  void attachTexture(unsigned int textureName);
  bool isReady();

  void bind();
  void unBind();
  
private:
  int _frameBuffer;
};

#endif /* defined(__drone__FrameBufferObject__) */
