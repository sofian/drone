#include "Gear_FFT.h"
#include "Engine.h"

#include "CircularBuffer.h"



#include <math.h>

                                              
const int Gear_FFT::DEFAULT_FFT_SIZE=512;

Register_Gear(MAKERGear_FFT, Gear_FFT, "FFT")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Gear_FFT::Gear_FFT() : 
Gear("FFT")                                                                        
{    
  addPlug(_AUDIO_IN = new PlugIn<SignalType>(this, "In", new SignalType(0.0f)));
  addPlug(_FFT_OUT = new PlugOut<FFTType>(this, "Out", new FFTType(DEFAULT_FFT_SIZE)));  

  _circBuf = new CircularBuffer<Signal_T>(0.0f);
}

Gear_FFT::~Gear_FFT()
{
  fftw_destroy_plan(_fftPlan);
  fftw_free(_fftIn);   
  delete[] _hammingWindow;
}

bool Gear_FFT::ready()
{
  return(_AUDIO_IN->connected());
}                                           

void Gear_FFT::internalInit()
{    
  int fftSize = _FFT_OUT->type()->fftSize();
      
  _fftIn = (double*)fftw_malloc(sizeof(double) * fftSize);
                                              
  _fftPlan = fftw_plan_dft_r2c_1d(fftSize, _fftIn, _FFT_OUT->type()->fftOut(), FFTW_ESTIMATE);

  //allocate and create the hamming window
  _hammingWindow = new double[fftSize];  
  for (int i=0;i<fftSize;i++)           
    _hammingWindow[i] = 0.54f - (0.46 * cos(6.28318f * i/fftSize));
  
}

void Gear_FFT::runVideo()
{  
  int bufferSize = _AUDIO_IN->type()->size();
  int fftSize = _FFT_OUT->type()->fftSize();  

  _circBuf->resize(bufferSize, fftSize / bufferSize);
  _circBuf->append(_AUDIO_IN->type()->data());

  const Signal_T *bufferIn = _circBuf->getBlock(-(fftSize / bufferSize)+1);
  
  //fill the fft input and apply the hamming window at the same time
  for (int i=0;i<fftSize;++i)
    _fftIn[i]=bufferIn[i]*_hammingWindow[i]; 
  
  //execute the fft
  fftw_execute(_fftPlan);

//simple output of the power spectrum for debugging
/*   for (int i=0;i<fftSize/2+1;++i)                                                                                                */
/*   {                                                                                                                                 */
/*     std::cout << 44100*i/bufferSize << " ---:--- " << sqrt(_fftOut[i][0]*_fftOut[i][0] + _fftOut[i][1]*_fftOut[i][1]) << std::endl; */
/*   }                                                                                                                                 */
/*   std::cout << "------------------------------" << std::endl;                                                                       */
}
