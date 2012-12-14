/* Gear_VideoTexture.cpp
 * Copyright (C) 2004 Jean-Sebastien Senecal
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

#include <iostream>
#include <algorithm>
#include "Gear_VideoTexture.h"
#include "Engine.h"
#include "DroneMath.h"
#include "log_add.h"

#include "GearMaker.h"

//#include "Timer.h"

Register_Gear(MAKERGear_VideoTexture, Gear_VideoTexture, "VideoTexture");

const std::string Gear_VideoTexture::SETTING_FILENAME = "Filename";
const std::string Gear_VideoTexture::SETTING_NFRAMES = "NFrames";
const std::string Gear_VideoTexture::SETTING_TIMEWINDOWLENGTH = "Time window length";

Gear_VideoTexture::Gear_VideoTexture(Schema *schema, std::string uniqueName) : Gear(schema, "VideoTexture", uniqueName),
                                                                         _file(NULL),
                                                                         _sizeX(0),
                                                                         _sizeY(0)
{    
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOut"));
  //addPlug(_SIGMA_IN = new PlugIn<ValueType>(this, "Sigma", new ValueType());

  _settings.add(Property::FILENAME, SETTING_FILENAME)->valueStr("");
  _settings.add(Property::FILENAME, SETTING_NFRAMES)->valueInt(1);
  _settings.add(Property::FILENAME, SETTING_TIMEWINDOWLENGTH)->valueInt(2);

  _mutex = new pthread_mutex_t();
  pthread_mutex_init(_mutex, NULL);

  //todo
  memset(_frame, 0, 1024*sizeof(RGBA*));
}

Gear_VideoTexture::~Gear_VideoTexture()
{
  if (_file!=NULL)
    mpeg3_close(_file);

  pthread_mutex_destroy(_mutex);

  for (int i=0;i<1024;i++)
    free(_frame[i]);
}

bool Gear_VideoTexture::ready()
{
  return(_VIDEO_OUT->connected());
}

void Gear_VideoTexture::onUpdateSettings()
{
//   Timer timer;
//   timer.reset();

  char tempstr[1024];

  // XXX todo : parameters
  double power = 1.0;
  int nEpochs = 3;
  double alpha = 0.999;

  pthread_mutex_lock(_mutex);

  // Initialize (open) the movie.
  strcpy(tempstr,_settings.get(SETTING_FILENAME)->valueStr().c_str());
  
  std::cout << "opening movie : " << tempstr << std::endl;

  if (_file!=NULL)
    mpeg3_close(_file);
  
  _file = mpeg3_open(tempstr);

  if (_file==NULL)
  {
    std::cout << "error opening movie : " << tempstr << std::endl;
    pthread_mutex_unlock(_mutex);
    return;
  }
  _sizeX = mpeg3_video_width(_file, 0);
  _sizeY = mpeg3_video_height(_file, 0);

  //_nFrames = (int)CLAMP((long int)_settings.get(SETTING_NFRAMES)->valueInt(), 1l, mpeg3_video_frames(_file, 0));
  _nFrames = (int)_settings.get(SETTING_NFRAMES)->valueInt();
  ASSERT_ERROR(_nFrames >= 1);
  
  std::cout << "movie size X : " << _sizeX << std::endl;
  std::cout << "movie size Y : " << _sizeY << std::endl;

  std::cout << "numframes : " << _nFrames << " / " << mpeg3_video_frames(_file, 0) << std::endl;
  std::cout << "movie samplerate : " << mpeg3_sample_rate(_file,0) << std::endl;

  for (int i=0;i<_sizeY-1;i++)
    _frame[i] = (RGBA*) realloc(_frame[i], _sizeX * sizeof(RGBA));

  //from the doc :
  //You must allocate 4 extra bytes in the last output_row. This is scratch area for the MMX routines.
  _frame[_sizeY-1] = (RGBA*) realloc(_frame[_sizeY-1], (_sizeX * sizeof(RGBA)) + 4);

//   std::cout << "Time to initialize things: " << timer.getTime()  << std::endl;
//   timer.reset();

  // Fill sequences and distance matrix.
  _distances.resize(_nFrames, _nFrames);
  _sequences.resize(_nFrames);
  _size = _sizeX * _sizeY;
  NOTICE("Filling distance and sequence matrix.");
  for (int i=0; i<_nFrames; ++i)
  {
    Array2D<RGBA>& currImage = _sequences[i];
    currImage.resize(_sizeX, _sizeY);

    // Read current image.
    mpeg3_read_frame(_file, (unsigned char**)_frame, 0, 0, _sizeX, _sizeY, _sizeX, _sizeY, MPEG3_RGBA8888, 0);

    // Add image to sequences.
    for(int y=0;y<_sizeY;y++)
      memcpy(currImage.row(y), _frame[y], sizeof(RGBA) * _sizeX);
    
    // Update distance matrix (this is the bottleneck of the whole algorithm).
    _distances(i,i) = 0.0;
    for (int j=0; j<i; ++j)
      _distances(i,j) = _distances(j,i) = L2((unsigned char*)currImage.data(), (unsigned char*)_sequences[j].data(), (size_t)_size*SIZE_RGBA);
  }

//   std::cout << "Time to fill sequence and distance: " << timer.getTime() << std::endl;
//   timer.reset();

#if DEBUG_NOTICE
  std::cout << "Distances: " << std::endl;
  for (int i=0; i<_nFrames; ++i)
  {
    for (int j=0; j<_nFrames; ++j)
      std::cout << _distances(j,i) << " ";
    std::cout << std::endl;
  }
  std::cout << std::endl;
#endif

  // Add temporal coherence to distance matrix by smoothing the distance with linear interpolation.
  NOTICE("Computing smoothed distances.");
  int timeWindowLength = CLAMP(_settings.get(SETTING_TIMEWINDOWLENGTH)->valueInt(), 0, 2);
  _smoothedDistances.resize(_nFrames, _nFrames);
  switch (timeWindowLength)
  {
  case 0:
    memcpy(_smoothedDistances.data(), _distances.data(), _distances.size());
    break;
  case 1:
    for (int i=0; i<_nFrames; ++i)
    {
      for (int j=0; j<_nFrames; ++j)
      {
        int iPrev = MAX(i-1,0);
        int jPrev = MAX(j-1,0);
        _smoothedDistances(j,i) =
          0.5 * _distances(jPrev,iPrev) +
          0.5 * _distances(j,i);
      }
    }
    break;
  case 2:
    for (int i=0; i<_nFrames; ++i)
    {
      for (int j=0; j<_nFrames; ++j)
      {
        int iPrev = MAX(i-1,0);
        int jPrev = MAX(j-1,0);
        int iNext = MIN(i+1,_nFrames-1);
        int jNext = MIN(j+1,_nFrames-1);
        int iPrev2 = MAX(i-2,0);
        int jPrev2 = MAX(j-2,0);
        _smoothedDistances(j,i) =
          0.125 * _distances(jPrev2,iPrev2) +
          0.375  * _distances(jPrev,iPrev) +
          0.375  * _distances(j,i) +
          0.125 * _distances(jNext,iNext);
      }
    }
    break;
  default:;
    error("Wrong time window length specified, please check");
  }

//   std::cout <<  "Time to compute smoothed distances: " << timer.getTime() << std::endl;
//   timer.reset();

#if DEBUG_NOTICE
  std::cout << "Smoothed distances: " << std::endl;
  for (int i=0; i<_nFrames; ++i)
  {
    for (int j=0; j<_nFrames; ++j)
      std::cout << _smoothedDistances(i,j) << " ";
    std::cout << std::endl;
  }
  std::cout << std::endl;
#endif
  
  // Using Q-learning, recompute the matrix of distances.
  NOTICE("Computing final set of distances using Q-learning.");
  _minDistances.resize(_nFrames);

  // Initialize distances.
  for (int i=0; i<_nFrames; ++i)
    for (int j=0; j<=i; ++j)
      _distances(i,j) = _distances(j,i) = _smoothedDistances(i,j) = _smoothedDistances(j,i) = pow(_smoothedDistances(i,j), power);

  // Q-learning.
  for (int t=0; t<nEpochs; ++t)
  {
#if DEBUG_NOTICE
    std::cout << "Q-learn distances step " << t << " : " << std::endl;
    for (int i=0; i<_nFrames; ++i)
    {
      for (int j=0; j<_nFrames; ++j)
        std::cout << _distances(j,i) << " ";
      std::cout << std::endl;
    }
    std::cout << std::endl;
#endif

    // Init min distances.
    for (int j=0; j<_nFrames; ++j)
      _minDistances[j] = min(_distances.row(j), (size_t)_nFrames);
    
    for (int i=_nFrames-1; i>=0; --i)
      for (int j=0; j<_nFrames; ++j)
      {
        // Update distances.
        _distances(j,i) = _smoothedDistances(j,i) + alpha * _minDistances[j];
        // Update min distances.
        _minDistances[j] = min(_distances.row(j), (size_t)_nFrames);
      }
  }

  // std::cout <<  "Time to compute Q-learned distances: " <<  timer.getTime() << std::endl;
//   timer.reset();

  // Calculate the mean (smoothed) distance.
  double meanDistance = sum(_distances.data(), _distances.size()) / (double)_distances.size();

  // Compute cumulative probabilities.
  _logCumProbs.resize(_nFrames, _nFrames);
  for (int i=0; i<_nFrames; ++i)
  {
    // Compute logCumProbs.
    _logCumProbs(0,i) = LOG_ZERO;
    for (int j=1; j<_nFrames; ++j)
      _logCumProbs(j,i) = logAdd(_logCumProbs(j-1,i), -_distances(j,i) / meanDistance);
    
    // Normalize to make a true probability.
    double norm = _logCumProbs(_nFrames-1,i);
    for (int j=0; j<_nFrames; ++j)
      _logCumProbs(j,i) -= norm;
  }

//   std::cout << "Time to compute probabilities: "<< timer.getTime() << std::endl;

#if DEBUG_NOTICE
  std::cout << "Distances: " << std::endl;
  for (int i=0; i<_nFrames; ++i)
  {
    for (int j=0; j<_nFrames; ++j)
      std::cout << _distances(j,i) << " ";
    std::cout << std::endl;
  }
  std::cout << std::endl;

  std::cout << "Probabilities: " << std::endl;
  for (int i=0; i<_nFrames; ++i)
  {
    for (int j=0; j<_nFrames; ++j)
      std::cout << exp(_logCumProbs(j,i)) << " ";
    std::cout << std::endl;
  }
  std::cout << std::endl;
#endif
  _currentFrame = 0;
  
  pthread_mutex_unlock(_mutex);
}

void Gear_VideoTexture::runVideo()
{
  _VIDEO_OUT->type()->resize(_sizeX, _sizeY);

  double rnd = log(double(rand()) / (double)(RAND_MAX)); // sample from uniform (0,1)
  double *rowBegin = _logCumProbs.row(_currentFrame);

#if DEBUG_NOTICE
//   std::cout << "Probs: " << std::endl;
//   for (int j=0; j<_nFrames; ++j)
//     std::cout << exp(_logCumProbs(j,_currentFrame)) << " ";
//   std::cout << std::endl;
#endif

  double *frame = std::lower_bound(rowBegin, rowBegin + _nFrames, rnd);
  _currentFrame = MIN((int)(frame - rowBegin), _nFrames-1);

  ASSERT_ERROR(_currentFrame < _nFrames);
//   NOTICE("Random number: %f, Chosen frame : %d", rnd, _currentFrame);

  memcpy(_VIDEO_OUT->type()->data(), _sequences[_currentFrame].data(), _size * sizeof(RGBA));
}

double Gear_VideoTexture::L2(const unsigned char *src1, const unsigned char *src2, size_t n)
{
  double sum = 0;
  while (--n)
    sum += SQR( (int)(*src1++ - *src2++) );
  return (double)sqrt(sum);
}
