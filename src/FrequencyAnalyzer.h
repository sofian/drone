#ifndef FREQUENCYANALYZER_INCLUDED
#define FREQUENCYANALYZER_INCLUDED

#include "Plug.h"
#include <fftw3.h>

class FrequencyAnalyser
{
public:

  FrequencyAnalyser(int bufferSize);
  ~FrequencyAnalyser();

  void prepare(int bufferSize);

  void frequencyToTime(const float *inputBuffer, float *outputBuffer);
  void timeToFrequency(const float *inputBuffer, float *outputBuffer);
  void filter(const float *inputBuffer, float *outputBuffer);

  int getMasterFrequency();
  float getBand(int lowerFreq, int higherFreq);
  //fftw_real *powerSpectrum() {return _powerSpectrum;};


private:    
    fftw_plan _fftwPlan;
    fftw_plan _fftwPlanInvert;
    float *_hammingWindow;
    int _bufferSize;
    int _windowSize;

    fftw_complex *_inputComplex;//the complex buffer where our input will be put
    fftw_complex *_outputComplex;//the result of fftw
    fftw_complex *_backComplex;//the complex buffer where our input will be put

};


#endif
