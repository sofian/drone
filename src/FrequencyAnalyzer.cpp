#include "FrequencyAnalyzer.h"
#include "Math.h"
#include <iostream>

FrequencyAnalyser::FrequencyAnalyser(int bufferSize) :
 _hammingWindow(NULL), _bufferSize(bufferSize), _windowSize(2*bufferSize),
 _inputComplex(NULL), _outputComplex(NULL)
{
  prepare(bufferSize);
}

FrequencyAnalyser::~FrequencyAnalyser()
{
}

void FrequencyAnalyser::prepare(int bufferSize)
{    
/*     _bufferSize = bufferSize;                                                                              */
/*     _windowSize = bufferSize*2;                                                                            */
/*                                                                                                            */
/*     _inputComplex = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * _windowSize);                       */
/*                                                                                                            */
/*     _outputComplex =(fftw_complex*) fftw_malloc(sizeof(fftw_complex) * _windowSize);                       */
/*                                                                                                            */
/*     //allocate the plan for fftw                                                                           */
/*     _fftwPlan = fftw_plan_dft_1d(_windowSize, _inputComplex, _outputComplex, FFTW_FORWARD, FFTW_ESTIMATE); */
/*                                                                                                            */
/*     //_fftwPlanInvert = fftw_create_plan(_windowSize, FFTW_BACKWARD, FFTW_ESTIMATE);                       */
/*                                                                                                            */
/*     //allocate and build hamming window                                                                    */
/*     if (_hammingWindow!=NULL)                                                                              */
/*         delete[] _hammingWindow;                                                                           */
/*                                                                                                            */
/*     _hammingWindow = new float[_bufferSize];                                                               */
/*                                                                                                            */
/*     for (int i=0;i<_bufferSize;i++)                                                                        */
/*         _hammingWindow[i] = 0.54f - (0.46 * cos(6.28318f * i/_bufferSize));                                */


/*     //allocate _backComplex                    */
/*     if (_backComplex!=NULL)                    */
/*         delete _backComplex;                   */
/*                                                */
/*     _backComplex = new fftw_real[_windowSize]; */

}

void FrequencyAnalyser::frequencyToTime(const float *inputBuffer, float *outputBuffer)
{
    //multiply with hamming window and assign to _inputComplex;
/*     for (int i=0;i<_bufferSize;i++)                            */
/*         _inputComplex[i] = inputBuffer[i] * _hammingWindow[i]; */
/*                                                                */
/*     //pad the imaginary part with 0                            */
/*     for (int i=_bufferSize;i<_windowSize;i++)                  */
/*         _inputComplex[i]=0.0;                                  */
/*                                                                */
/*     fftw_one(_fftwPlanInvert, _inputComplex, _outputComplex);  */
/*                                                                */
/*     for (int i=1; i<_bufferSize; i++)                          */
/*         outputBuffer[i] = _outputComplex[i];                   */
}

void FrequencyAnalyser::timeToFrequency(const float *inputBuffer, float *outputBuffer)
{                
/*     //multiply with hamming window and assign to _inputComplex;                                                                                      */
/*     for (int i=0;i<_bufferSize;i++)                                                                                                                  */
/*         _inputComplex[i] = inputBuffer[i] * _hammingWindow[i];                                                                                       */
/*                                                                                                                                                      */
/*     //pad the imaginary part with 0                                                                                                                  */
/*     for (int i=_bufferSize;i<_windowSize;i++)                                                                                                        */
/*         _inputComplex[i]=0.0;                                                                                                                        */
/*                                                                                                                                                      */
/*     fftw_execute(_fftwPlan);                                                                                                                         */
/*                                                                                                                                                      */
/*     outputBuffer[0] = _outputComplex[0]*_outputComplex[0];                                                                                           */
/*                                                                                                                                                      */
/*     for (int i=1; i<_bufferSize; i++)                                                                                                                */
/*         outputBuffer[i] = 10 * log10(2 * sqrt((_outputComplex[i]*_outputComplex[i] + _outputComplex[_windowSize-i]*_outputComplex[_windowSize-i]))); */
/*                                                                                                                                                      */
/*     if (_bufferSize%2==0)                                                                                                                            */
/*         outputBuffer[_bufferSize] = _outputComplex[_bufferSize]*_outputComplex[_bufferSize];                                                         */

/*     for (int i=0;i<_bufferSize;i++)                                                    */
/*     {                                                                                  */
/*         std::cout << 44100*i/_windowSize << " --- " << _powerSpectrum[i] << std::endl; */
/*     }                                                                                  */

}

void FrequencyAnalyser::filter(const float *inputBuffer, float *outputBuffer)
{
/*     //multiply with hamming window and assign to _inputComplex;                                              */
/*     for (int i=0;i<_bufferSize;i++)                                                                          */
/*         _inputComplex[i] = inputBuffer[i];// * _hammingWindow[i];                                            */
/*                                                                                                              */
/*     //pad the imaginary part with 0                                                                          */
/*     for (int i=_bufferSize;i<_windowSize;i++)                                                                */
/*         _inputComplex[i]=0.0;                                                                                */
/*                                                                                                              */
/*     fftw_one(_fftwPlan, _inputComplex, _outputComplex);                                                      */
/*                                                                                                              */
/*     fftw_one(_fftwPlanInvert, _outputComplex, _backComplex);                                                 */
/*                                                                                                              */
/*     for (int i=0; i<_bufferSize; i++)                                                                        */
/*         outputBuffer[i] = _backComplex[i] / _windowSize;//signal is multiplied by n by the ifft so we divide */
/*                                                                                                              */
}

int FrequencyAnalyser::getMasterFrequency()
{
/*     fftw_real highest=0.0;                   */
/*     int highestIndex=0;                      */
/*                                              */
/*     for (int i=1;i<_bufferSize-1;i++)        */
/*     {                                        */
/*         if (highest < _powerSpectrum[i])     */
/*         {                                    */
/*             highest = _powerSpectrum[i];     */
/*             highestIndex=i;                  */
/*         }                                    */
/*     }                                        */
/*                                              */
/*     return 22050*highestIndex/(_windowSize); */
}

/**
 * return the average energy [0..1.0f] of bins between the 2 frequencies 
 * 
 * @param lowerFreq
 * @param higherFreq
 * 
 * @return 
 */
float FrequencyAnalyser::getBand(int lowerFreq, int higherFreq)
{
/*     int minFreq = MIN(lowerFreq, higherFreq);        */
/*     int maxFreq = MAX(lowerFreq, higherFreq);        */
/*                                                      */
/*                                                      */
/*     int lowerIndex = _windowSize * minFreq / 22050;  */
/*     int higherIndex = _windowSize * maxFreq / 22050; */
/*                                                      */
/*     if (lowerIndex >= _bufferSize)                   */
/*         lowerIndex = _bufferSize - 1;                */
/*                                                      */
/*     if (higherIndex >= _bufferSize)                  */
/*         higherIndex = _bufferSize - 1;               */
/*                                                      */
/*     float sumOfFreq=0.0f;                            */
/*     for (int i=lowerIndex;i<=higherIndex;i++)        */
/*     {                                                */
/*         sumOfFreq+=_powerSpectrum[i];                */
/*     }                                                */
/*                                                      */
/*     int range = higherIndex - lowerIndex + 1;        */
/*                                                      */
/*     return sumOfFreq / range;                        */

}


