/* Gear_FaceTrack.h
 * Copyright (C) 2004 Norman Casagrande, Jean-Sebastien Senecal
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

#ifndef GEAR_FACETRACK_INCLUDED
#define GEAR_FACETRACK_INCLUDED


#include "Gear.h"
#include <pthread.h>
#include "VideoRGBAType.h"
#include "AreaArrayType.h"
#include "ValueType.h"
#include <vector>

#define NEW_VERSION 1
#if NEW_VERSION
#define FILENAME "/home/foogy/devel/drone/data/featuresFound/featuresFound"
#else
#define FILENAME "/home/foogy/devel/drone/data/featuresFound/featuresFound"
#endif

class Gear_FaceTrack;

extern "C" void *threadStartup(void *);

class Gear_FaceTrack : public Gear
{
public:

  Gear_FaceTrack(Engine *engine, std::string name);
  virtual ~Gear_FaceTrack();

  void init();
  void runVideo();
  bool ready();

  void   *loopThread();

private:

  struct FaceArea
  {
    unsigned int x, y;
    unsigned int size;
  };

  // Attributes

  bool    _keepLooping;
  unsigned char*   _pCurrentFrame;

  //static const float   _threshold;
  int     _deltaS;

  //  bool    _askForFrame;

  //  pthread_cond_t  _hFrameEvent;
  pthread_mutex_t _inputMutex;
  pthread_cond_t  _inputCond;
  pthread_mutex_t _outputMutex;
//   pthread_mutex_t _exitMutex;
//   pthread_cond_t  _exitCond;
  pthread_t       _detectorThread;
  pthread_attr_t  _detectorAttr;

  std::vector< FaceArea >  _drawingFoundFaces; // the current faces drawn

  enum FeatureType 
  {
    FEATURE_NO_TYPE,
    FEATURE_V_RECT,
    FEATURE_H_RECT,
    FEATURE_3_RECT,
    FEATURE_4_RECT
  };

#if NEW_VERSION
  struct Feature
  {
    Feature()
      : _type(FEATURE_NO_TYPE), _cutPoint(0), _x(0), _y(0), _xSize(0), _ySize(0), _class(false), _alpha(0) {}

    FeatureType _type;
    int         _x, _y;
    int         _xSize, _ySize;
  
    double      _alpha;
    float       _cutPoint;

    bool        _class;
  };
#else
  struct Feature
  {
    Feature()
      : _type(FEATURE_NO_TYPE), _cutPoint(0), _class(false), _x(0), _y(0), _xSize(0), _ySize(0), _alpha(0) {}

      FeatureType _type;
      float       _cutPoint;
      bool        _class;

      int         _x, _y;
      int         _xSize, _ySize;

      double      _alpha;
  };
#endif
  
  struct PrecompWindows
  {
    PrecompWindows(int tWinSize, int tShifting) : winSize(tWinSize), shifting(tShifting) {}
    int winSize;
    int shifting;
  };

  void   readFeaturesCascade(const std::string& featuresFileName);
  void   findFaces();
  inline bool detector(unsigned int*& pCurrWindow, unsigned int*& pSquareCurrWindow, 
                              std::vector< Feature >& precFeatures, int& newWinSize, 
                              int& blockNumber);
  void   computeIntegralImages();
  
  inline bool   getFeatureGuess(unsigned int* intImage, float& normVal, Feature& selectedFeature);
  inline int    getFeatureVRect(unsigned int*& intImage, int& x, int& y, int& xSize, int& ySize);
  inline int    getFeatureHRect(unsigned int*& intImage, int& x, int& y, int& xSize, int& ySize);
  inline int    getFeature3Rect(unsigned int*& intImage, int& x, int& y, int& xSize, int& ySize);
  inline int    getFeature4Rect(unsigned int*& intImage, int& x, int& y, int& xSize, int& ySize);
  inline unsigned int getSumAt(unsigned int*& winIntImage, int x, int y);
  inline unsigned int getSumAt(unsigned int*& winIntImage, int x, int y, unsigned int*& picIntImage);

  inline void   mergeFaces();

  static const int        _winSize;

//  unsigned int*    _integralImage
  unsigned int*    _pIntegralImage;
  unsigned int*    _pIntegralImageSquare;

  unsigned int*    _pCumulativeRowSum;
  unsigned int*    _pCumulativeRowSumSquare;

  unsigned int     _detector_x;
  unsigned int     _detector_y;
  // DO NOT FORGET TO SET THIS!!
  unsigned int     _picWidth;
  unsigned int     _picHeight;

  std::vector< Feature >   _selFeatures;

  std::vector< FaceArea >  _foundFaces; // the faces that are going to be found
  std::vector< FaceArea >  _mergedFaces; // the faces that are going to be found

  // The features resized
  typedef std::vector< std::vector<Feature> > Cascade;

  Cascade*                      _pCascadeFeatures;
  std::vector< PrecompWindows > _precWinSizes;

#if !NEW_VERSION
  static const double                   _thresholdList[];
#endif
  
private:
  PlugOut<AreaArrayType> *_AREA_OUT;
  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugIn<ValueType> *_THRESHOLD_IN;
  //  PlugIn<ValueType> *_SHIFT_IN;
  
  const VideoRGBAType *_image;
  AreaArrayType *_faces;
  
  Array2D<unsigned char> _grayImage;
  Array2D<unsigned int> _integralImage;
  Array2D<unsigned int> _integralImageSquare;
  Array<unsigned int> _cumulativeRowSum;
  Array<unsigned int> _cumulativeRowSumSquare;

  float _threshold;
};

#endif
