#include <iostream>
#include "Gear_AudioInput.h"
#include "GearMaker.h"
#include "Engine.h"

Register_Gear(MAKERGear_AudioInput, Gear_AudioInput, "AudioInput")

const int Gear_AudioInput::DEFAULT_FRAMES_PER_BUFFER=512;
const int Gear_AudioInput::DEFAULT_NB_BUFFERS=16;

const std::string Gear_AudioInput::SETTING_FRAMES_PER_BUFFER = "FramesPerBuffer";
const std::string Gear_AudioInput::SETTING_NB_BUFFERS = "NbBuffers";

Gear_AudioInput::Gear_AudioInput(Engine *engine, std::string name) : 
  Gear(engine, "AudioInput", name),
  _stream(0),
  _ringBufferSize(512),
  _lBuffer(),
  _rBuffer(),
  _readIndex(0),
  _mutex(0),
  _playbackThreadHandle()
{
  //  _category << Category::AUDIO << Category::IO;

  addPlug(_AUDIO_OUT_LEFT = new PlugOut<SignalType>(this, "Left"));    
  addPlug(_AUDIO_OUT_RIGHT = new PlugOut<SignalType>(this, "Right"));    

  _settings.add(Property::INT, SETTING_FRAMES_PER_BUFFER)->valueInt(DEFAULT_FRAMES_PER_BUFFER);
  _settings.add(Property::INT, SETTING_NB_BUFFERS)->valueInt(DEFAULT_NB_BUFFERS);    

  _mutex = new pthread_mutex_t();
  pthread_mutex_init(_mutex, NULL);

}

Gear_AudioInput::~Gear_AudioInput()
{

}

bool Gear_AudioInput::ready()
{
  return(_AUDIO_OUT_LEFT->connected() || _AUDIO_OUT_RIGHT->connected());
}

void Gear_AudioInput::init()
{
  std::cout << "Initializing AudioInput..." << std::endl;


  initPortAudio();

  //pthread_create(&_PlaybackThreadHandle, NULL, &Gear_AudioInput::playbackThread, this);

  std::cout << "Initializing AudioInput...done" << std::endl;
}

/* void *Gear_AudioInput::playbackThread(void *param)                                                             */
/* {                                                                                                               */
/*     Gear_AudioInput *parent = (Gear_AudioInput*) param;                                                       */
/*     int readIndex=0;                                                                                            */
/*                                                                                                                 */
/*     //memset(parent->_LBuffer, 0 , sizeof(float) * parent->_ringBufferSize);                                    */
/*                                                                                                                 */
/*     signed short temp[parent->_ringBufferSize*2];                                                               */
/*     while(1)                                                                                                    */
/*     {                                                                                                           */
/*                                                                                                                 */
/*                                                                                                                 */
/*         //pthread_mutex_lock(parent->_mutex);                                                                   */
/*         if (snd_pcm_writei(parent->_PCM, &(parent->_LBuffer[readIndex]), Engine::signalInfo().blockSize()) < 0) */
/*         {                                                                                                       */
/*              snd_pcm_prepare(parent->_PCM);                                                                     */
/*              std::cout << "underflow" << std::endl;                                                                       */
/*         }                                                                                                       */
/*                                                                                                                 */
/*         readIndex = (readIndex + Engine::signalInfo().blockSize()*2) % parent->_ringBufferSize;                 */
/*                                                                                                                 */
/*                                                                                                                 */
/*         //pthread_mutex_unlock(parent->_mutex);                                                                 */
/*                                                                                                                 */
/*     }                                                                                                           */
/*                                                                                                                 */
/*     return 0;                                                                                                   */
/* }                                                                                                               */

void Gear_AudioInput::onUpdateSettings()
{
  init();
}

void Gear_AudioInput::runAudio()
{
  float *left_buffer = _AUDIO_OUT_LEFT->type()->data();
  //SignalType right_buffer  = _AUDIO_OUT_RIGHT->type();
  int signal_blocksize = Engine::signalInfo().blockSize();

  for (int i=0; i<signal_blocksize; i++)
    left_buffer[i] = _lBuffer[_readIndex++];


  _readIndex %= _ringBufferSize;    

}


void Gear_AudioInput::initPortAudio()
{
  PaError err;

  std::cout << "init PortAudio..." << std::endl;

  int framesPerBuffer = _settings.get(SETTING_FRAMES_PER_BUFFER)->valueInt();


  err = Pa_Initialize();

  if (err != paNoError)
    std::cout << Pa_GetErrorText(err) << std::endl;
  else
    std::cout << "init PortAudio...done" << std::endl;

  std::cout << "Opening PortAudio Stream..." << std::endl;
  std::cout << Engine::signalInfo().sampleRate() << "hz " << std::endl;
  std::cout << "Frames per buffer: " << framesPerBuffer << std::endl;

  int nbBuffers = Pa_GetMinNumBuffers(framesPerBuffer, Engine::signalInfo().sampleRate());
  std::cout << "Number of buffers: " << nbBuffers << std::endl;

  _ringBufferSize = framesPerBuffer * 16;

  _lBuffer.resize(_ringBufferSize);

  err = Pa_OpenStream(
                     &_stream,
                     Pa_GetDefaultInputDeviceID(),/* default input device*/
                     1,              /* no input */
                     paFloat32,  /* 32 bit floating point input*/
                     NULL,
                     paNoDevice,
                     0,          /* stereo output*/
                     paFloat32,      /* 32 bit floating point output*/
                     NULL,
                     Engine::signalInfo().sampleRate(),
                     framesPerBuffer,
                     0,              /* number of buffers, if zero then use default minimum*/
                     paClipOff,      /* we won't output out of range samples so don't bother clipping them*/
                     portAudioCallback,
                     this);


  if (err != paNoError)
    std::cout << Pa_GetErrorText(err) << std::endl;
  else
    std::cout << "Opening PortAudio Stream...done" << std::endl;

  std::cout << "Starting PortAudio stream..." << std::endl;


  if (err != paNoError)
    std::cout << Pa_GetErrorText(err) << std::endl;
  else
    std::cout << "Starting PortAudio stream...done" << std::endl;

/*     if (!started) */
/*     {             */
  Pa_StartStream(_stream);
//        started=true;
/*     } */

}

int Gear_AudioInput::portAudioCallback(void *input_buffer, void *, unsigned long frames_per_buffer,
                                       PaTimestamp, void *user_data)
{
  static Gear_AudioInput *parent = (Gear_AudioInput*)user_data;
  static int lindex=0;
  //static int rindex=0;
  SignalType& lbuffer = parent->_lBuffer;

  int ringBufferSize = parent->_ringBufferSize;

  //SignalType *rbuffer = parent->_rBuffer;

  float *in = (float*)input_buffer;

  for (unsigned int i=0; i<frames_per_buffer; i++ )
    lbuffer[lindex++] = *in++;


  lindex %= ringBufferSize;


  return 0;
}


