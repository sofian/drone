/* Gear_FaceTrack.cpp
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

#include "Gear_FaceTrack.h"
#include "Engine.h"
#include "Utils.h"

#include <iostream>
#include <fstream>
#include <cmath>
//#include <process.h>

#include "GearMaker.h"

Register_Gear(MAKERGear_FaceTrack, Gear_FaceTrack, "FaceTrack")

Gear_FaceTrack::Gear_FaceTrack(Engine *engine, std::string name) : Gear(engine, "FaceTrack", name),
                                                                   m_pCascadeFeatures(0),
                                                                   _threshold(0)
{
  addPlug(_AREA_OUT = new PlugOut<AreaArrayType>(this, "Area"));
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN"));
  addPlug(_THRESHOLD_IN = new PlugIn<ValueType>(this, "Thresh", new ValueType(0, -1, 1)));
}

Gear_FaceTrack::~Gear_FaceTrack()
{
  m_keepLooping = false;
  pthread_attr_destroy(&_detectorAttr);
  pthread_mutex_destroy(&m_inputMutex);
  pthread_mutex_destroy(&m_outputMutex);
  pthread_cond_destroy(&m_inputCond);
  pthread_exit(NULL);
}

void Gear_FaceTrack::init()
{
  m_keepLooping = true;
  
  readFeaturesCascade(FILENAME);

  pthread_mutex_init(&m_inputMutex, NULL ); // init
  pthread_mutex_init(&m_outputMutex, NULL ); // init
  pthread_mutex_lock(&m_inputMutex);
  pthread_cond_init(&m_inputCond, NULL ); // init

  pthread_attr_init(&_detectorAttr);
  pthread_attr_setdetachstate(&_detectorAttr, PTHREAD_CREATE_JOINABLE);
  pthread_create(&_detectorThread, &_detectorAttr, threadStartup, this);
}

bool Gear_FaceTrack::ready()
{
  return(_VIDEO_IN->connected() && _AREA_OUT->connected());
}

void Gear_FaceTrack::runVideo()
{
  pthread_mutex_lock(&m_inputMutex);

  // Wait until the frame is ready
  _image = _VIDEO_IN->type();

  if (_image->isNull())
    return;

  _threshold = _THRESHOLD_IN->type()->value();

  m_picWidth = _image->width();
  m_picHeight = _image->height();

  // Change to grayscale.
  _grayImage.resize(m_picWidth, m_picHeight);
  
  _integralImage.resize(m_picWidth, m_picHeight);
  _integralImageSquare.resize(m_picWidth, m_picHeight);
  _cumulativeRowSum.resize(m_picWidth);
  _cumulativeRowSumSquare.resize(m_picWidth);
  
  m_pIntegralImage = _integralImage.data();
  m_pIntegralImageSquare = _integralImageSquare.data();
  m_pCumulativeRowSum = _cumulativeRowSum.data();
  m_pCumulativeRowSumSquare = _cumulativeRowSumSquare.data();
  
  pthread_mutex_unlock(&m_inputMutex);

  pthread_cond_signal(&m_inputCond);

  // Output the faces.
  pthread_mutex_lock(&m_outputMutex);
  _faces = _AREA_OUT->type();
  _faces->resize(m_drawingFoundFaces.size());
  for (int i=0; i<(int)_faces->size(); ++i)
  {
    Area& area = _faces->operator[](i);
    area.x0 = m_drawingFoundFaces[i].x;
    area.y0 = m_drawingFoundFaces[i].y;
    area.x1 = m_drawingFoundFaces[i].x + m_drawingFoundFaces[i].size;
    area.y1 = m_drawingFoundFaces[i].y + m_drawingFoundFaces[i].size;
  }
  pthread_mutex_unlock(&m_outputMutex);
}

const int     Gear_FaceTrack::m_winSize = 24;
const double  Gear_FaceTrack::m_thresholdList[] = {-0.5,   // features bloc 1
                                                   -0.45,  // features bloc 2 
                                                   -0.35,  // features bloc 3
                                                   -0.1,   // features bloc 4
                                                   -0.05,  // features bloc 5
                                                   -0.05,   // features bloc 6
                                                   0.02,   // features bloc 7
                                                   0.0,   // features bloc 8
                                                   0.0,   // features bloc 9
                                                   0.05,   // features bloc 10
                                                   0.05,   // features bloc 11
                                                   0.06
};

const int     Gear_FaceTrack::m_deltaS = 3; 

//bool    Gear_FaceTrack::m_askForFrame = true;

// std::vector< Gear_FaceTrack::Feature >   Gear_FaceTrack::m_selFeatures;
// std::vector< Gear_FaceTrack::FaceArea >  Gear_FaceTrack::m_foundFaces;
// std::vector< Gear_FaceTrack::FaceArea >  Gear_FaceTrack::m_mergedFaces;
// std::vector< Gear_FaceTrack::FaceArea >  Gear_FaceTrack::m_drawingFoundFaces; // the current faces drawn

//Gear_FaceTrack::Cascade*            Gear_FaceTrack::m_pCascadeFeatures = NULL;

//std::vector< Gear_FaceTrack::PrecompWindows >  Gear_FaceTrack::m_precWinSizes;

// // VERY IMPORTANT!!!
// #if defined(USING_CREATIVE_WEBCAM)
// unsigned int Gear_FaceTrack::m_picWidth = 320;
// unsigned int Gear_FaceTrack::m_picHeight = 240;
// #elif defined(USING_DV)
// unsigned int Gear_FaceTrack::m_picWidth = 360;
// unsigned int Gear_FaceTrack::m_picHeight = 240;
// #endif

// unsigned int Gear_FaceTrack::m_detector_x = 0;
// unsigned int Gear_FaceTrack::m_detector_y = 0;


void *threadStartup(void *obj_)
{
  Gear_FaceTrack *obj = (Gear_FaceTrack *)obj_;
  void *threadResult = obj->loopThread();
  return threadResult;
}


void *Gear_FaceTrack::loopThread()
{
  m_keepLooping = true;

  while (m_keepLooping)
  {
    // Tell the filter that we want a new frame
    //m_askForFrame = true; 

    pthread_cond_wait(&m_inputCond, &m_inputMutex);
    
    m_pCurrentFrame = (unsigned char*) _grayImage.data();
    rgba2grayscale(m_pCurrentFrame, (unsigned char*)_image->data(), _image->size());
    
    computeIntegralImages();
    findFaces();
    
    pthread_mutex_lock(&m_outputMutex);
    mergeFaces();
    m_drawingFoundFaces = m_mergedFaces; //m_foundFaces;
    pthread_mutex_unlock(&m_outputMutex);
  }

  pthread_exit(NULL);
  return NULL;
}


// Loads all the features and put them into the _selFeatures vector
void Gear_FaceTrack::readFeaturesCascade(const std::string& baseName)
{
  Feature tmpFeature;
  std::vector<Feature> tmpBlocFeatures;
  int i;
  int nFeatures;
  char fileName[2000];
  int fileCounter;
  int sizeCounter = 0;

  float resizingFactor = 1.25;

  // Precomputing now all the sizes
  int smallestSize = 3, biggestSize = 8;
  float scale;
  int tmpWindowSize;
  int tmpShifting;
  std::vector< Feature > tmpPrecFeatures;
  std::vector< Feature >::iterator fIt;

  ASSERT_ERROR_MESSAGE(m_pCascadeFeatures, "Cascade should be empty! Restart the application");

  m_pCascadeFeatures = new Cascade[biggestSize - smallestSize + 1];

  scale = pow((double)resizingFactor, smallestSize);
  // Computing the window sizes
  for (i = smallestSize; i <= biggestSize; ++i)
  {
    // First compute the window size
    tmpWindowSize = static_cast<int>((float)m_winSize * scale + 0.499);
    tmpShifting = static_cast<int>((float)m_deltaS * scale + 0.499);
    m_precWinSizes.push_back( PrecompWindows(tmpWindowSize, tmpShifting) );
    scale *= resizingFactor;
  }

  fileCounter = 1;
  while(true)
  {
    sprintf(fileName, "%s_%d.dat", baseName.c_str(), fileCounter++);

    std::ifstream inFile(fileName, std::ios::binary | std::ios::in);
    if (!inFile.is_open())
      break;

    inFile.seekg(0, std::ios::end);
    int tmp = sizeof(Feature);
    nFeatures = inFile.tellg() / sizeof(Feature);
    inFile.seekg(0);

    tmpBlocFeatures.clear();
    for (i = 0; i < nFeatures; ++i)
    {
#if NEW_VERSION
      inFile.read( (char *)&tmpFeature, sizeof(Feature) );
#else
//       char tmpData[150];
//       inFile.read( (char*)tmpData, 150);

      //*((double *)(&tmpData[10]));
      
      char tmpSkipping;
      inFile.read( (char *)&tmpFeature._type, sizeof(FeatureType) );
      inFile.read( (char *)&tmpFeature._cutPoint, sizeof(float) );
      inFile.read( (char *)&tmpFeature._class, sizeof(bool) );
      inFile.read( &tmpSkipping, sizeof(char) );
      inFile.read( &tmpSkipping, sizeof(char) );
      inFile.read( &tmpSkipping, sizeof(char) );
      inFile.read( (char *)&tmpFeature._x, sizeof(int) );
      inFile.read( (char *)&tmpFeature._y, sizeof(int) );
      inFile.read( (char *)&tmpFeature._xSize, sizeof(int) );
      inFile.read( (char *)&tmpFeature._ySize, sizeof(int) );
      inFile.read( &tmpSkipping, sizeof(char) );
      inFile.read( &tmpSkipping, sizeof(char) );
      inFile.read( &tmpSkipping, sizeof(char) );
      inFile.read( &tmpSkipping, sizeof(char) );
      inFile.read( (char *)&tmpFeature._alpha, sizeof(double) );
#endif
      tmpBlocFeatures.push_back(tmpFeature);
    }
    inFile.close();

    scale = pow((double)resizingFactor, smallestSize);
    sizeCounter = 0;

    for (i = smallestSize; i <= biggestSize; ++i, ++sizeCounter)
    {
      // Now the features

      // For each feature found
      for (fIt = tmpBlocFeatures.begin(); fIt != tmpBlocFeatures.end(); ++fIt)
      {
        tmpFeature = *fIt;
        tmpFeature._x = static_cast<int>((float)tmpFeature._x * scale + 0.499);
        tmpFeature._y = static_cast<int>((float)tmpFeature._y * scale + 0.499);
        tmpFeature._xSize = static_cast<int>((float)tmpFeature._xSize * scale + 0.499);
        tmpFeature._ySize = static_cast<int>((float)tmpFeature._ySize * scale + 0.499);

        tmpPrecFeatures.push_back(tmpFeature);
      }

      m_pCascadeFeatures[sizeCounter].push_back(tmpPrecFeatures);
      tmpPrecFeatures.clear();
      scale *= resizingFactor;
    }

  }

}

void Gear_FaceTrack::mergeFaces()
{
  m_mergedFaces.clear();

  if (m_foundFaces.empty())
    return;

  unsigned char* foundFacesGroup = new unsigned char[m_foundFaces.size()];
  unsigned char* foundFacesGroupSize = new unsigned char[m_foundFaces.size()];
//  memset(foundFacesGroupSize, 0, m_foundFaces.size());

  FaceArea* currMergedFace;

  std::vector< FaceArea >::iterator doneIt = m_foundFaces.begin();
  std::vector< FaceArea >::iterator candidateIt;

  //  m_mergedFaces.clear(); // this is a crappy hack to deal with crappy flickering algorithm
  m_mergedFaces.push_back(*doneIt);
  ++doneIt;

  foundFacesGroup[0] = 0;
  foundFacesGroupSize[0] = 1;

  bool foundPrevious;

  int nGroups = 1;
  int foundCounter;
  int candCounter;

  for (foundCounter = 1; doneIt != m_foundFaces.end(); ++doneIt, ++foundCounter)
  {
    candCounter = 0;
    foundPrevious = false;

    for (candidateIt = m_foundFaces.begin(); candidateIt != doneIt; ++candidateIt, ++candCounter)
    {
      if (  (candidateIt->x + candidateIt->size > doneIt->x ) &&
            (candidateIt->x < doneIt->x + doneIt->size)       &&
            (candidateIt->y + candidateIt->size > doneIt->y ) &&
            (candidateIt->y < doneIt->y + doneIt->size) )
      {
        foundFacesGroup[ foundCounter ] = foundFacesGroup[ candCounter ];
        foundFacesGroupSize[ foundFacesGroup[ foundCounter ] ] ++;
        currMergedFace = &m_mergedFaces[ foundFacesGroup[ foundCounter ] ];
        foundPrevious = true;

        currMergedFace->x += candidateIt->x;
        currMergedFace->y += candidateIt->y;
        currMergedFace->size += candidateIt->size;

        break;
      }
    }

    if (!foundPrevious)
    {
      foundFacesGroup[ foundCounter ] = nGroups++;
      foundFacesGroupSize[ foundFacesGroup[ foundCounter ] ] = 1;
      m_mergedFaces.push_back(*doneIt);
      currMergedFace = &m_mergedFaces[ foundFacesGroup[ foundCounter ] ];
    }
  }

  // let's do the real interpolation, dammit!
  for (candidateIt = m_mergedFaces.begin(), candCounter = 0; 
       candidateIt != m_mergedFaces.end(); 
       ++candidateIt,  ++candCounter)
  {
    candidateIt->x /= foundFacesGroupSize[candCounter];
    candidateIt->y /= foundFacesGroupSize[candCounter];
    candidateIt->size /= foundFacesGroupSize[candCounter];

    if (candidateIt->x + candidateIt->size > m_picWidth)
      candidateIt->x = m_picWidth - candidateIt->size - 1;
    if (candidateIt->y + candidateIt->size > m_picHeight)
      candidateIt->y = m_picHeight - candidateIt->size - 1;
  }
 
  delete [] foundFacesGroup;
  delete [] foundFacesGroupSize;
}

void Gear_FaceTrack::findFaces()
{
  unsigned int* pCurrWindow;
  unsigned int* pCurrWindowSquare;

  m_foundFaces.clear();

  std::vector< PrecompWindows > ::iterator vIt;
  Cascade* pCurrPrecFeatures;

  bool isFace;
  int sizesCounter = 0;
  int featuresBloc = 0;

  Cascade::iterator cIt;

  for (vIt = m_precWinSizes.begin(); vIt != m_precWinSizes.end(); ++vIt, ++sizesCounter)
  {
    pCurrPrecFeatures = &m_pCascadeFeatures[sizesCounter];
    //cout << "Window Size: " << windowSize << endl;
    for (m_detector_x = 0; m_detector_x +  vIt->winSize <= m_picWidth; m_detector_x += vIt->shifting)
    {
      for (m_detector_y = 0; m_detector_y + vIt->winSize <= m_picHeight; m_detector_y += vIt->shifting)   
      {
        pCurrWindow = &m_pIntegralImage[m_detector_y * m_picWidth + m_detector_x];
        pCurrWindowSquare = &m_pIntegralImageSquare[m_detector_y * m_picWidth + m_detector_x];

        isFace = true;
        featuresBloc = 0;
        for (cIt = pCurrPrecFeatures->begin(); cIt != pCurrPrecFeatures->end(); ++cIt, ++featuresBloc)
        {
          if ( !detector(pCurrWindow, pCurrWindowSquare, *cIt, vIt->winSize, featuresBloc) )
          {
            isFace = false;
            break;
          }
        }

        if ( isFace )
        {
//           std::cout << "-----> Face found at: (" << x << "-" << (x+windowSize) << ", " 
//                << y << "-" << (y+windowSize) << 
//             "). WindowSize: " << windowSize << std::endl;
          // Yeah! A face found!

          FaceArea aFace;
          aFace.x = m_detector_x;
          aFace.y = m_detector_y;
          aFace.size = vIt->winSize;

          //TRACE("Found at %d,%d (size: %d)\n", aFace.x, aFace.y, aFace.size);

          m_foundFaces.push_back(aFace);
        }
      }
    }
  }
}

inline
bool Gear_FaceTrack::detector(unsigned int*& pCurrWindow, unsigned int*& pCurrWindowSquare,
                        std::vector< Feature >& pPrecFeatures, int& newWinSize, int& blockNumber)
{
  std::vector<Feature>::iterator fIt; // iterator over all the features
  double sumAlphaAndH = 0;
  double sumAlpha = 0;
  int h;      // h is the weak learner
  double f;   // f is the strong learner

  // (newWinSquare - 1) because it starts from 0!

  double sum = (double)( getSumAt(pCurrWindow, newWinSize-1, newWinSize-1) + // 4
                         getSumAt(pCurrWindow, -1, -1) - // 1
                        (getSumAt(pCurrWindow, newWinSize-1, -1) + // 2
                         getSumAt(pCurrWindow, -1, newWinSize-1)) ); // 3

  double sumOfSquares = (double)( getSumAt(pCurrWindowSquare, newWinSize-1, newWinSize-1, m_pIntegralImageSquare) + // 4
                                  getSumAt(pCurrWindowSquare, -1, -1, m_pIntegralImageSquare) - // 1
                                 (getSumAt(pCurrWindowSquare, newWinSize-1, -1, m_pIntegralImageSquare) + // 2
                                  getSumAt(pCurrWindowSquare, -1, newWinSize-1, m_pIntegralImageSquare)) ); // 3

  double numPixels = (double)(newWinSize * newWinSize);

  double stdDev = sqrt( (sumOfSquares / numPixels) -
                        (sum * sum) / (numPixels*numPixels) );
  float normVal = (float)(1.0f / stdDev);

  // For each feature found
  for (fIt = pPrecFeatures.begin(); fIt != pPrecFeatures.end(); ++fIt)
  {
    if ( getFeatureGuess(pCurrWindow, normVal, *fIt) )
    {
      // it is a face for this feature (weak learner)
      h = 1;
    }
    else
    {
      // it is not a face for this feature (weak learner)
      h = -1;
    }

    sumAlphaAndH += fIt->_alpha * h;
    sumAlpha += fIt->_alpha;
  }

  // The formula (normalized) of the final strong learner f(x) is:
  //          \sum (alpha * h)    sumAlphaAndH
  //  f(x) = ----------------- = ---------------
  //           \sum(alpha)          sumAlpha

  f = sumAlphaAndH / sumAlpha;

  if (f > _threshold)// * m_thresholdList[blockNumber])
  {
    //cout << "confidence: " << f << endl;
    return true;
  }
  else
    return false;

}

inline bool
Gear_FaceTrack::getFeatureGuess( unsigned int* intImage, float& normVal, Feature& selectedFeature )
{
  int featureValue;

  switch (selectedFeature._type)
  {
  case FEATURE_V_RECT:
    featureValue = getFeatureVRect(intImage, 
      selectedFeature._x, selectedFeature._y,
      selectedFeature._xSize, selectedFeature._ySize);
    break;

  case FEATURE_H_RECT:
    featureValue = getFeatureHRect(intImage,
      selectedFeature._x, selectedFeature._y,
      selectedFeature._xSize, selectedFeature._ySize);
    break;

  case FEATURE_3_RECT:
    featureValue = getFeature3Rect(intImage,
      selectedFeature._x, selectedFeature._y,
      selectedFeature._xSize, selectedFeature._ySize);

    break;

  case FEATURE_4_RECT:
    featureValue = getFeature4Rect(intImage,
      selectedFeature._x, selectedFeature._y,
      selectedFeature._xSize, selectedFeature._ySize);

    break;

  default:
    return false;
  }

  if ( featureValue * normVal < selectedFeature._cutPoint )
  {
    //if ( selectedFeature._class == realClass )
    //  return true;
    //else
    //  return false;

    return selectedFeature._class;
    //return selectedFeature._class == realClass;
  }
  else
  {
    //if ( !selectedFeature._class == realClass )
    //  return true;
    //else
    //  return false;

    return !selectedFeature._class;
    //return !selectedFeature._class == realClass;
  }

}

//////////////////////////////////////////////////////////////////////////
////////// Features Section //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// VERTICAL FEATURE: 
// (W = White, B = Black)
// For white:
// 1      2   
//  +-----+-----+
//  | [W] |  B  |
// 3+-----4-----+
//        ^-- xHalfPos
// For black:
//       1      2
//  +-----+-----+
//  |  W  | [B] |
//  +----3+-----4
//         ^-- xHalfPos
//            
// The formula is: 4+1 - (2+3)
inline int 
Gear_FaceTrack::getFeatureVRect(unsigned int*& intImage, 
                                  int& x, int& y, 
                                  int& xSize, int& ySize)
{
  int xHalfPos = x + (xSize / 2);
  int yEndPos = y + ySize;

  int whiteSum = getSumAt(intImage, xHalfPos, yEndPos) + // 4
                 getSumAt(intImage, x-1, y-1) - // 1
                (getSumAt(intImage, xHalfPos, y-1) + // 2
                 getSumAt(intImage, x-1, yEndPos) ); // 3

  xHalfPos++;
  int blackSum = getSumAt(intImage, x+xSize, yEndPos) + // 4
                 getSumAt(intImage, xHalfPos-1, y-1) - // 1
                (getSumAt(intImage, x+xSize, y-1) + // 2
                 getSumAt(intImage, xHalfPos-1, yEndPos) ); // 3

  return blackSum - whiteSum;
}

// HORIZONTAL FEATURE
// For white:
//   1        2
//    +-------+
//    |  [W]  |
//   3+-------4
//    |   B   |
//    +-------+
// For black:
//    +-------+
//   1|   W   2
//    +-------+
//    |  [B]  |
//   3+-------4
// The formula is: 4+1 - (2+3)

inline int 
Gear_FaceTrack::getFeatureHRect(unsigned int*& intImage, 
                                  int& x, int& y, 
                                  int& xSize, int& ySize)
{
  int yHalfPos = y + (ySize / 2);
  int xEndPos = x + xSize;

  int whiteSum = getSumAt(intImage, xEndPos, yHalfPos) + // 4
                 getSumAt(intImage, x-1, y-1) - // 1
                (getSumAt(intImage, xEndPos, y-1) + // 2
                 getSumAt(intImage, x-1, yHalfPos) ); // 3

  yHalfPos++;
  int blackSum = getSumAt(intImage, xEndPos, y+ySize) + // 4
                 getSumAt(intImage, x-1, yHalfPos-1) - // 1
                (getSumAt(intImage, xEndPos, yHalfPos-1) + // 2
                 getSumAt(intImage, x-1, y+ySize) ); // 3

  return blackSum - whiteSum;
}

// 3 FEATURE
// For white left:
//   1      2
//    +-----+-----+-----+
//    | [W] |  B  |  W  |
//   3+-----4-----+-----+
// For black:
//         1      2
//    +-----+-----+-----+
//    |  W  | [B] |  W  |
//    +----3+-----4-----+
// For white right:
//               1      2
//    +-----+-----+-----+
//    |  W  |  B  | [W] |
//    +-----+----3+-----4
//
//  The formula is: 4+1 - (2+3)
inline int 
Gear_FaceTrack::getFeature3Rect(unsigned int*& intImage, 
                                  int& x, int& y, 
                                  int& xSize, int& ySize)
{
  // xOneThirdPos correspond to this (xSize = 8):
  //     x     A   B
  //    [0][1][2]|[3][4][5]|[6][7][8]
  // A = before whiteSum
  // B = after whiteSum
  int xOneThirdPos = x + ( xSize / 3);

  // twoThirdXPos correspond to this (xSize = 8):
  //     x               A   B
  //    [0][1][2]|[3][4][5]|[6][7][8]
  // A = before blackSum
  // B = after blackSum
  int xTwoThirdPos = x + ( ((xSize+1) / 3) * 2 - 1);

  // Left White
  int whiteSum = getSumAt(intImage, xOneThirdPos, y+ySize) + // 4
                 getSumAt(intImage, x-1, y-1) - // 1
                (getSumAt(intImage, xOneThirdPos, y-1) + // 2
                 getSumAt(intImage, x-1, y+ySize) ); // 3

  xOneThirdPos++;

  int blackSum = getSumAt(intImage, xTwoThirdPos, y+ySize) + // 4
                 getSumAt(intImage, xOneThirdPos-1, y-1) - // 1
                (getSumAt(intImage, xTwoThirdPos, y-1 ) + // 2
                 getSumAt(intImage, xOneThirdPos-1, y+ySize) ); // 3

  xTwoThirdPos++;

  // Right White
  whiteSum += getSumAt(intImage, x+xSize, y+ySize) + // 4
              getSumAt(intImage, xTwoThirdPos-1, y-1) - // 1
             (getSumAt(intImage, x+xSize, y-1) + // 2
              getSumAt(intImage, xTwoThirdPos-1, y+ySize) ); // 3

  return blackSum - whiteSum;
}

// 4 FEATURE
// For white up-left:
//   1      2
//    +-----+-----+
//    | [W] |  B  |
//   3+-----4-----+
//    |  B  |  W  |
//    +-----+-----+
// For black up-right:
//         1      2
//    +-----+-----+
//    |  W  | [B] |
//    +----3+-----4
//    |  B  |  W  |
//    +-----+-----+
// For black down-left:
//           
//    +-----+-----+
//   1|  W  2  B  |
//    +-----+-----+
//    | [B] |  W  |
//   3+-----4-----+
// For white down-right:
//                 
//    +-----+-----+
//    |  W 1|  B  2
//    +-----+-----+
//    |  B  | [W] |
//    +----3+-----4

//  The formula is: 4+1 - (2+3)
inline int 
Gear_FaceTrack::getFeature4Rect(unsigned int*& intImage, 
                                  int& x, int& y, 
                                  int& xSize, int& ySize)
{ 
  int yHalfPos = y + (ySize / 2);
  int yEndPos = y + ySize;
  int xHalfPos = x + (xSize / 2);
  int xEndPos = x + xSize;

  // up left
  int whiteSum = getSumAt(intImage, xHalfPos, yHalfPos) + // 4
                 getSumAt(intImage, x-1, y-1) - // 1
                (getSumAt(intImage, xHalfPos, y-1) + // 2
#if NEW_VERSION
                 getSumAt(intImage, x-1, yHalfPos) ); // 3
#else
                 getSumAt(intImage, x-1, xHalfPos) ); // 3
#endif

  xHalfPos++;

  // Up right
  int blackSum = getSumAt(intImage, xEndPos, yHalfPos) + // 4
                 getSumAt(intImage, xHalfPos-1, y-1) - // 1
                (getSumAt(intImage, xEndPos, y-1) + // 2
                 getSumAt(intImage, xHalfPos-1, yHalfPos) ); // 3

  xHalfPos--;
  yHalfPos++;

  // Adds down left
  blackSum += getSumAt(intImage, xHalfPos, yEndPos) + // 4
              getSumAt(intImage, x-1, yHalfPos-1) - // 1
             (getSumAt(intImage, xHalfPos, yHalfPos-1) + // 2
              getSumAt(intImage, x-1, yEndPos) ); // 3

  xHalfPos++;

  // Adds down right
  whiteSum += getSumAt(intImage, xEndPos, yEndPos) + // 4
              getSumAt(intImage, xHalfPos-1, yHalfPos-1) - // 1
             (getSumAt(intImage, xEndPos, yHalfPos-1) + // 2
              getSumAt(intImage, xHalfPos-1, yEndPos) ); // 3

  return blackSum - whiteSum;
}

inline unsigned int 
Gear_FaceTrack::getSumAt(unsigned int*& winIntImage, int x, int y)
{
#ifdef DBG
  if (m_picWidth == 0)
  {
    MessageBox(NULL, "ERROR: currPicSize = 0!!", "Error", MB_OK|MB_ICONERROR);
    exit(0);
  }
#endif

  // TAT'S METHOD
  //int diffPos = (int)(winIntImage - integralImage);
  //int globalY = diffPos / picWidth;
  //int globalX = diffPos % picWidth;

  //// Find global coordinates we want to access
  //globalY += y;
  //globalX += x;
  //if (globalX < 0 || globalY < 0)
  //  return 0;
  //else
  //  return integralImage[ picWidth * globalY + globalX ];

  // NORMAN'S METHOD
  if (x < 0 || y < 0)
  {
    //int diffPos = (int)(winIntImage - m_pIntegralImage);
    //int globalY = diffPos / m_picWidth;
    //int globalX = diffPos % m_picWidth;

    if ( (m_detector_x == 0 && x < 0) || (m_detector_y == 0 && y < 0 ))
      return 0;
  }

  return winIntImage[ m_picWidth * y + x ];
}

inline unsigned int 
Gear_FaceTrack::getSumAt(unsigned int*& winIntImage, int x, int y, unsigned int*& picIntImage)
{
#ifdef DBG
  if (m_picWidth == 0)
  {
    MessageBox(NULL, "ERROR: currPicSize = 0!!", "Error", MB_OK|MB_ICONERROR);
    exit(0);
  }
#endif

  // TAT'S METHOD
  //int diffPos = (int)(winIntImage - picIntImage);
  //int globalY = diffPos / picWidth;
  //int globalX = diffPos % picWidth;

  //// Find global coordinates we want to access
  //globalY += y;
  //globalX += x;
  //if (globalX < 0 || globalY < 0)
  //  return 0;
  //else
  //  return picIntImage[ picWidth * globalY + globalX ];

  // NORMAN'S METHOD
  if (x < 0 || y < 0)
  {
    //int diffPos = (int)(winIntImage - picIntImage);
    //int globalY = diffPos / m_picWidth;
    //int globalX = diffPos % m_picWidth;

    if ( (m_detector_x == 0 && x < 0) || (m_detector_y == 0 && y < 0 ))
      return 0;
    //else
    //  return winIntImage[ picWidth * y + x ];
  }
  //else
  return winIntImage[ m_picWidth * y + x ];
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void Gear_FaceTrack::computeIntegralImages()
{
  unsigned int x, y;
  unsigned int prevIntImValue;
  unsigned int prevIntImValueSquare;

  unsigned int* intImageIt;
  unsigned int* squareIntImgIt;
  unsigned char* frameIt = m_pCurrentFrame;

  // For the moment I allocate and deallocate every time..
  //BYTE* bitsHead = new BYTE[bm.bmWidth * bm.bmHeight];
  //picture->GetBitmapBits(bm.bmWidth * bm.bmHeight, bitsHead);
  //BYTE* bits = bitsHead;

  intImageIt = m_pIntegralImage;
  squareIntImgIt = m_pIntegralImageSquare;

  for (x = 0; x < m_picWidth; ++x)
  {
    m_pCumulativeRowSum[x] = 0;
    m_pCumulativeRowSumSquare[x] = 0;
  }

  for (y = 0; y < m_picHeight; ++y)
  {
    prevIntImValue = 0;
    prevIntImValueSquare = 0;

    for (x = 0; x < m_picWidth; ++x)
    {
      // First integral image
      m_pCumulativeRowSum[x] += (unsigned int)(*frameIt);

      *intImageIt = prevIntImValue + m_pCumulativeRowSum[x];
      prevIntImValue = *intImageIt;
      ++intImageIt;

      // Square integral image
      m_pCumulativeRowSumSquare[x] += ((unsigned int)(*frameIt) * (unsigned int)(*frameIt));
      *squareIntImgIt = prevIntImValueSquare + m_pCumulativeRowSumSquare[x];
      prevIntImValueSquare = *squareIntImgIt;

      ++squareIntImgIt;

      // next pixel!
      ++frameIt;
    }
  }

}




