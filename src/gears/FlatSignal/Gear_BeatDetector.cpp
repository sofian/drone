/* Gear_BeatDetector.cpp
 * Copyright (C) 2005 Norman Casagrande, Jean-Sebastien Senecal
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

#include "Gear_BeatDetector.h"
#include "Math.h"
#include "Engine.h"

#include "GearMaker.h"

static const unsigned int BUFFER_SIZE = 5000; // 5 secs

static const unsigned int MIN_RANGE = 100; // equiv to min lag
static const unsigned int MAX_RANGE = 700; // max lag is MAX_RANGE * N_REP
static const unsigned int N_REP = 3;

//static const unsigned int MIN_LAG = 100; // 200 msec
//static const unsigned int MAX_LAG = 3200; // 3 sec
static const float WORKING_FREQ = 1000; // 1KHz
static const float DEFAULT_SMOOTH_DECAY = 0.5;

static const float DEFAULT_ACORR_DECAY = 0.999;
static const unsigned int ACORR_DECAY_STEP = 500;

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_BeatDetector(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "BeatDetector";
  gearInfo.classification = GearClassifications::signal().transform().instance();
  return gearInfo;
}
}

Gear_BeatDetector::Gear_BeatDetector(Schema *schema, std::string uniqueName)
  : Gear(schema, "BeatDetector", uniqueName)
{
  addPlug(_AUDIO_IN = new PlugIn<SignalType>(this, "AudioIn") );
  addPlug(_ACORR_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ACorrOut") );

	_minLag = MIN_RANGE;
	_maxLag = MAX_RANGE * N_REP;
	_acorrSize = _maxLag - _minLag + 1;
  _rangeSize = MAX_RANGE - MIN_RANGE + 1;

	_buffer.resize( BUFFER_SIZE );
	_buffer.zero_fill();
	
	_sampleIt = _buffer.head();
	_sampleLaggedIt = _sampleIt - _minLag;
	
	_autoCorrelation.resize(_acorrSize, 0);
	_pAutoCorrelation = _autoCorrelation.data();
	
	size_t dataSize = _AUDIO_IN->type()->size();
  float sampleRate = (float)Engine::signalInfo().sampleRate();
  float downsampledSize = ((float)dataSize / sampleRate) * WORKING_FREQ;
    
  _downSampledSig.resize( (int)(downsampledSize + 0.5) );
  _pDownSampledSig = _downSampledSig.data();
  
  _smoothSampleAverage = 0.0f;

  _smoothDecay = DEFAULT_SMOOTH_DECAY;
  _acorrDecay = DEFAULT_ACORR_DECAY;
  _acorrDecayCounter = 0;
  _acorrDecayStep = ACORR_DECAY_STEP;
  
  _dbgStep = 0;
  _dbgSize = (unsigned int)(44100.0 / float(Engine::signalInfo().blockSize()) + 0.5) ; // 1 sec..
  
  std::cout << "Acorr Size: " << _acorrSize << std::endl;
}

bool Gear_BeatDetector::ready()
{
  return (_AUDIO_IN->connected() /* && _ACORR_VIDEO_OUT->connected()*/);
}

void Gear_BeatDetector::runAudio()
{
  float* tmpNewData = _AUDIO_IN->type()->data();
	unsigned int dataSize = _AUDIO_IN->type()->size();

  for (unsigned int i = 0; i < dataSize; ++i)
    tmpNewData[i] = abs(tmpNewData[i]);

  downsample<float>( _pDownSampledSig, _downSampledSig.size(), 
                     tmpNewData, dataSize);
                     
  
	smooth<float>( _pDownSampledSig, _downSampledSig.size(), _smoothDecay,
                _smoothSampleAverage);
  
/*  _downSampledSig.fill(0);
  if (_dbgStep > _dbgSize)
  {
    _downSampledSig[0] = 1;
    _dbgStep = 0;
    std::cout << "Added one.." << std::endl;
  }
  ++_dbgStep;
*/
	
	// add new data to buffer and autocorrelation
	for (size_t i = 0; i < _downSampledSig.size(); ++i)
		addSample( _pDownSampledSig[i] );
	
  float maxVal = 0;
  float tmpSum;
  unsigned int idx = 0;

  for (unsigned int i = 0; i < _rangeSize; ++i)
  {
    tmpSum = 0;
    for (unsigned int j = 1; j < N_REP; j *= 2)
    {
      tmpSum += _pAutoCorrelation[i*j];
    }
    
    if (tmpSum > maxVal)
    {
      maxVal = tmpSum;
      idx = i;   
    }
  }
  
  drawACorr();

  ++_dbgStep;
  if (_dbgStep > _dbgSize)
  {
    for (unsigned int i = 0; i < _acorrSize; ++i)
    {
      if (_pAutoCorrelation[i] > maxVal)
      {
        maxVal = _pAutoCorrelation[i];
        idx = i;
      }
    }
  
	  //float* it = std::max_element(_pAutoCorrelation, _pAutoCorrelation + _acorrSize);
	  //std::cout << "max[" << idx << "] =" << maxVal << std::endl;
    std::cout << "max every " << idx + _minLag << " msecs" << std::endl;
    _dbgStep = 0;
  }
    
  ++_acorrDecayCounter;
  if (_acorrDecayCounter > _acorrDecayStep)
  {
    applyDecay();
    _acorrDecayCounter == 0;
  }
  
}


// performs the autocorrelation with one given sample
// Note: the direction of updating is to the right of the
// circle (rigth == newer data)
//
//        **    \
//     *      *  \
//    *        *  v
//    *        *
//     *      *
//        ** 
inline void Gear_BeatDetector::addSample(float sample)
{
	++_sampleIt;
  ++_sampleLaggedIt;
  *_sampleIt = sample;
	
  CircularArray<float>::iterator sIt = _sampleLaggedIt;
  
	// Autocorrelation of lag is:
	// A(lag) = sum_n x_n * x_(n-lag)
	// Where:
	// x_n = sample
	// x_(n-lag) is sIt which is moving into the past
	  
  // note that maxLag - minLag + 1 = size of pArray!
  for (unsigned int i = 0; i < _acorrSize; ++i)
  {
    _pAutoCorrelation[i] += ( sample * (*sIt) );
    --sIt;
  }
  
}

inline void Gear_BeatDetector::applyDecay()
{
  for (unsigned int i = 0; i < _acorrSize; ++i)
    _pAutoCorrelation[i] *= _acorrDecay;
}

void Gear_BeatDetector::drawACorr()
{
  int div = 4;
  int sizey = 200;
  int smallerSize = _acorrSize/div;
  Array<float> smallerACorr(smallerSize);
  float max;
  unsigned int aIdx = 0;
  for (unsigned int i = 0; i < smallerSize; ++i)
  {
    max = 0;
    for (int j = 0; j < div; ++j)
    {
      if ( _pAutoCorrelation[aIdx] > max)
        max = _pAutoCorrelation[aIdx];
      ++aIdx;
    }
    smallerACorr[i] = max;
  }

	float maxVal = *std::max_element(smallerACorr.begin(), smallerACorr.end());
  float yRatio;
  if (maxVal > 0)
    yRatio = (float)sizey / maxVal;
  else
    yRatio = 0;
 
  //std::cout << "smallerSize: " << smallerSize << " - sizey+1: " << sizey+1 << std::endl;
 
  _pOutImage = _ACORR_VIDEO_OUT->type();
  _pOutImage->resize(smallerSize,sizey+1);

  _pOutImage->fill(WHITE_RGBA);

  for (unsigned int x = 0; x < smallerSize; ++x)
  {
    int yLineSize = (int)(smallerACorr[x]*yRatio+0.5);
    for (unsigned int y = 0; y < yLineSize; ++y)
      _pOutImage->operator()(x, sizey-y)=BLACK_RGBA;
      
    //std::cout << "x: " << x << " - y: " << ySize << std::endl;
  }
}

