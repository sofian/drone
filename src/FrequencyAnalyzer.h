#ifndef FREQUENCYANALYZER_INCLUDED
#define FREQUENCYANALYZER_INCLUDED

#include "PlugSignal.h"
#include <fftw3.h>

class FrequencyAnalyser
{
public:

  FrequencyAnalyser(int bufferSize);
  ~FrequencyAnalyser();

  void prepare(int bufferSize);

  void frequencyToTime(Signal_T *inputBuffer, Signal_T *outputBuffer);
  void timeToFrequency(Signal_T *inputBuffer, Signal_T *outputBuffer);
  void filter(Signal_T *inputBuffer, Signal_T *outputBuffer);

  //int getMasterFrequency();
  //float getBand(int lowerFreq, int higherFreq);
  //fftw_real *powerSpectrum() {return _powerSpectrum;};


private:    
/*     rfftw_plan _fftwPlan;                                                     */
/*     rfftw_plan _fftwPlanInvert;                                               */
/*     Signal_T *_hammingWindow;                                                 */
/*     int _bufferSize;                                                          */
/*     int _windowSize;                                                          */
/*                                                                               */
/*     fftw_real *_inputComplex;//the complex buffer where our input will be put */
/*     fftw_real *_outputComplex;//the result of fftw                            */
/*     fftw_real *_backComplex;//the complex buffer where our input will be put  */

};


#endif
