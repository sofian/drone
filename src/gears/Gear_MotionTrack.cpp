/* Gear_MotionTrack.cpp
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable
 * This file is part of Drone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "Gear_MotionTrack.h"
#include "Engine.h"

#include <iostream>




Register_Gear(MAKERGear_MotionTrack, Gear_MotionTrack, "MotionTrack")

Gear_MotionTrack::Gear_MotionTrack()
  : Gear("MotionTrack"),
    _image_rgba(0), _image_rgb(0), _sizeX(0), _sizeY(0), _firstRun(true)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN"));
  addPlug(_THRESHOLD = new PlugIn<ValueType>(this, "Thresh", new ValueType(0,0,255)));
  addPlug(_HISTOGRAM_BINS = new PlugIn<ValueType>(this, "HistBins", new ValueType(20,10,120)));
  addPlug(_MIN_S = new PlugIn<ValueType>(this, "MinS", new ValueType(20,0,255)));
  addPlug(_MAX_S = new PlugIn<ValueType>(this, "MaxS", new ValueType(255,0,255)));
  addPlug(_MIN_V = new PlugIn<ValueType>(this, "MinV", new ValueType(40,0,255)));
  addPlug(_MAX_V = new PlugIn<ValueType>(this, "MaxV", new ValueType(255,0,255)));
  
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT"));
  _tracker = new CvCamShiftTracker();
  ASSERT_ERROR(_tracker);
}

Gear_MotionTrack::~Gear_MotionTrack()
{
  ASSERT_ERROR(_tracker);
  delete _tracker;
  if (_image_rgba)
    cvReleaseImageHeader(&_image_rgba);
  if (_image_rgb)
    cvReleaseImage(&_image_rgb);
}

void Gear_MotionTrack::internalInit()
{
  //  _firstRun = true;
  ASSERT_ERROR(_tracker);
}

bool Gear_MotionTrack::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_MotionTrack::runVideo()
{
  ASSERT_ERROR(_tracker);
  
  // Init.
  _image = _VIDEO_IN->type();
  if (_image->isNull())
    return;

  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());
  
  int thresh = (int) CLAMP((int)_THRESHOLD->type()->value(), 0, 255);
  int nBins  = (int) CLAMP((int)_HISTOGRAM_BINS->type()->value(), 10, 120);
  int nSmin  = (int) CLAMP((int)_MIN_S->type()->value(), 0, 255);
  int nSmax  = (int) CLAMP((int)_MAX_S->type()->value(), nSmin, 255);
  int nVmin  = (int) CLAMP((int)_MIN_V->type()->value(), 0, 255);
  int nVmax  = (int) CLAMP((int)_MAX_V->type()->value(), nVmin, 255);
  
  CvRect window = _tracker->get_window();
  
  // Check if we need to reset images.
  if (_sizeX != (int)_image->width() || _sizeY != (int)_image->height() || _firstRun)
  {
    _sizeX = _image->width();
    _sizeY = _image->height();
    
    NOTICE("Creating CvImage");
    // Create OpenCV images.
    CvSize size;
    size.width = _image->width();
    size.height = _image->height();
    
    // Release old images.
    if (_image_rgba)
      cvReleaseImageHeader(&_image_rgba);
    if (_image_rgb)
      cvReleaseImage(&_image_rgb);
    
    // Reallocate.
    _image_rgba = cvCreateImageHeader(size,IPL_DEPTH_8U,4);
    _image_rgb  = cvCreateImage(size,IPL_DEPTH_8U,3);
    
    ASSERT_ERROR(_image_rgba);
    ASSERT_ERROR(_image_rgb);
    
    _image_rgb->origin = IPL_ORIGIN_BL;

    // Set default window of the tracker.
    // XXX centrer SVP
    window.x = cvRound(_image->width()*0.4f);
    window.y = cvRound(_image->height()*0.3f);
    window.width = cvRound(_image->width()*0.2f);
    window.height = cvRound(_image->height()*0.3f);
  }

  // Check if we need to reset histogram.
  int testNBins;
  _tracker->get_hist_dims(&testNBins);
  if (nBins != testNBins ||
      nSmin != _tracker->get_min_ch_val(1) || nSmax != _tracker->get_max_ch_val(1) ||
      nVmin != _tracker->get_min_ch_val(2) || nVmax != _tracker->get_max_ch_val(2) ||
      _firstRun)
  {
    NOTICE("Reset histogram");
    
    // Set default parameter values of the tracker.
    _tracker->set_hist_dims(1, &nBins);
    _tracker->set_hist_bin_range(0,0,180);
    _tracker->set_min_ch_val(1,nSmin);
    _tracker->set_max_ch_val(1,nSmax);
    _tracker->set_min_ch_val(2,nVmin);
    _tracker->set_max_ch_val(2,nVmax);
        
    // Reset histogram.
    _tracker->reset_histogram();
  }

  // Update current image.
  _image_rgba->imageData = (char*)_image->data();

  // Convert to RGB space.
  cvCvtColor(_image_rgba, _image_rgb, CV_RGBA2RGB);
  
  // Compute histogram.
  _tracker->update_histogram( _image_rgb );

  _tracker->set_window(window);

  // Set threshold.
  _tracker->set_threshold(thresh);  
  
  NOTICE("Track object");
  // Track object.
  _tracker->track_object( _image_rgb );

  // Draw image.
  memcpy(_outImage->data(), _image->data(), _image->size()*sizeof(RGBA));

  Rasterer rast;
  rast.setImage(_outImage);
  rast.setColor(WHITE_RGBA);

  CvPoint2D32f point = _tracker->get_center();

  // Compute window.
  if ( window.x < 0 ) window.x = 0;
  if ( window.x > (int)(_image->width() - window.width - 1) )
    window.x = MAX(0, (int)(_image->width() - window.width - 1));

  if ( window.y < 0 ) window.y = 0;
  if ( window.y > (int)(_image->height() - window.height - 1) )
    window.y = MAX(0, (int)(_image->height() - window.height - 1));

  if ( window.width > (int)(_image->width() - window.x) )
    window.width = MIN((int)_image->width(), (int)(_image->width() - window.x));

  if ( window.height > (int)(_image->height() - window.y) )
    window.height = MIN((int)_image->height(), (int)(_image->height() - window.y));
    
  rast.rect(window.x, window.y, window.x + window.width - 1, window.y + window.height - 1, false);

  _firstRun = false;
//   RGBA green(0,255,0,0);
//   rast.setColor(green);
//   rast.fill((int)point.x+1, (int)point.y);
//   rast.fill((int)point.x-1, (int)point.y);
//   rast.fill((int)point.x, (int)point.y+1);
//   rast.fill((int)point.x, (int)point.y-1);
//   rast.fill((int)point.x, (int)point.y);

}

