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

  void frequencyToTime(float *inputBuffer, float *outputBuffer);
  void timeToFrequency(float *inputBuffer, float *outputBuffer);
  void filter(float *inputBuffer, float *outputBuffer);

  //int getMasterFrequency();
  //float getBand(int lowerFreq, int higherFreq);
  //fftw_real *powerSpectrum() {return _powerSpectrum;};


private:    
/*     rfftw_plan _fftwPlan;                                                     */
/*     rfftw_plan _fftwPlanInvert;                                               */
/*     float *_hammingWindow;                                                 */
/*     int _bufferSize;                                                          */
/*     int _windowSize;                                                          */
/*                                                                               */
/*     fftw_real *_inputComplex;//the complex buffer where our input will be put */
/*     fftw_real *_outputComplex;//the result of fftw                            */
/*     fftw_real *_backComplex;//the complex buffer where our input will be put  */

};


#endif
