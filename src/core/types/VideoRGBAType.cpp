#include "VideoRGBAType.h"

#include <iostream>
#include "Utils.h"
#include "AlphaComposite.h"


VideoRGBAType::~VideoRGBAType() 
{
}

void VideoRGBAType::toGray()
{
  if (!isGray())
  {
    grayscaleRGBA(data(), data(), size());
    setIsGray(true);
  }
}

void VideoRGBAType::premultiplyAlpha()
{
  if (!isAlphaPremultiplied())
  {
    alpha_premultiply((unsigned char*)data(), size());
    setIsAlphaPremultiplied(true);
  }
}

void VideoRGBAType::demultiplyAlpha()
{
  if (isAlphaPremultiplied())
  {
    alpha_demultiply((unsigned char*)data(), size());
    setIsAlphaPremultiplied(false);
  }
}

void VideoRGBAType::setAlpha(const Array<unsigned char>& mask)
{
  ASSERT_ERROR (size() != mask.size());
  if (isAlphaPremultiplied())
  {
    demultiplyAlpha();
    alpha_set((unsigned char*)data(), mask.data(), size());
    premultiplyAlpha();
  }
  else
    alpha_set((unsigned char*)data(), mask.data(), size());
}

void VideoRGBAType::fillAlpha(unsigned char alpha)
{
  if (isAlphaPremultiplied())
  {
    demultiplyAlpha();
    alpha_fill((unsigned char*)data(), alpha, size());
    premultiplyAlpha();
  }
  else
    alpha_fill((unsigned char*)data(), alpha, size());
}

