#include <iostream>
#include "Gear_STKAudioOutput.h"
#include "GearMaker.h"
#include "Engine.h"

Register_Gear(MAKERGear_STKAudioOutput, Gear_STKAudioOutput, "STKAudioOutput")

const int Gear_STKAudioOutput::DEFAULT_FRAMES_PER_BUFFER=128;
const int Gear_STKAudioOutput::DEFAULT_NB_BUFFERS=16;

const std::string Gear_STKAudioOutput::SETTING_FRAMES_PER_BUFFER = "FramesPerBuffer";
const std::string Gear_STKAudioOutput::SETTING_NB_BUFFERS = "NbBuffers";

bool Gear_STKAudioOutput::started = false;

Gear_STKAudioOutput::Gear_STKAudioOutput(Engine *engine, std::string name) : 
Gear(engine, "STKAudioOutput", name),     
_RingBufferSize(512),
_LBuffer(),
_RBuffer(),
_ReadIndex(0)

{
  addPlug(_AUDIO_IN_LEFT = new PlugIn<SignalType>(this, "Left", new SignalType(0.0f)));    
  addPlug(_AUDIO_IN_RIGHT = new PlugIn<SignalType>(this, "Right", new SignalType(0.0f)));    

  _settings.add(Property::INT, SETTING_FRAMES_PER_BUFFER)->valueInt(DEFAULT_FRAMES_PER_BUFFER);
  _settings.add(Property::INT, SETTING_NB_BUFFERS)->valueInt(DEFAULT_NB_BUFFERS);    

  _Mutex = new pthread_mutex_t();
  pthread_mutex_init(_Mutex, NULL);


}

Gear_STKAudioOutput::~Gear_STKAudioOutput()
{
  void stopReadingBuffer();

}

bool Gear_STKAudioOutput::ready()
{
  return(_AUDIO_IN_LEFT->connected() || _AUDIO_IN_RIGHT->connected());
}

void Gear_STKAudioOutput::init()
{
  std::cout << "Initializing STKAudioOutput..." << std::endl;


  initDevice();

  //pthread_create(&_PlaybackThreadHandle, NULL, &Gear_STKAudioOutput::playbackThread, this);

  std::cout << "Initializing STKAudioOutput...done" << std::endl;
}

void Gear_STKAudioOutput::onUpdateSettings()
{
  init();
}

void Gear_STKAudioOutput::runAudio()
{
  std::cerr<<"!!!!!!!!! ";
  float *left_buffer = _AUDIO_IN_LEFT->type()->data();
  int signal_blocksize = Engine::signalInfo().blockSize();
  
  for (int i=0; i<signal_blocksize; i++)
    _LBuffer[_ReadIndex++] = 0;//i%23;//sin(i/200.0f);
  std::cerr<<"signal_blocksize!0 ";
  _ReadIndex %= _RingBufferSize;    
  
  startReadingBuffer();
}


void Gear_STKAudioOutput::initDevice()
{
  int channels = 2;
  int sampleRate = 44100;
  int framesPerBuffer = _settings.get(SETTING_FRAMES_PER_BUFFER)->valueInt();
  int nBuffers = _settings.get(SETTING_NB_BUFFERS)->valueInt();
  
  int device = 1;        // 0 indicates the default or first available device

  std::cout << "init RtAudio..." << std::endl;
  
  try {
    _audio = new RtAudio(device, channels, 0, 0, RTAUDIO_FLOAT32,
                        sampleRate, &framesPerBuffer, nBuffers);
  }
  
  
  catch (RtError &error) {
    error.printMessage();
    exit(EXIT_FAILURE);
  }
  std::cout << "init RtAudio...done" << std::endl;

  _RingBufferSize = framesPerBuffer * 16;

  _LBuffer.resize(_RingBufferSize);
  _LBuffer.resize(0);
  std::cerr<<"framesPerBuffer: "<<framesPerBuffer<<"\n";
  
  startReadingBuffer();
}

void Gear_STKAudioOutput::startReadingBuffer()
{
  if(!started)
  {
    try {
      // Set the stream callback function
      _audio->setStreamCallback(&bufferCallback, (void *)this);
      
      // Start the stream
      _audio->startStream();
    }
    catch (RtError &error) {
      error.printMessage();
      exit(EXIT_FAILURE);
    }
    
    std::cout << "\nPlaying ... \n";
    started=true;
  }
}


void Gear_STKAudioOutput::stopReadingBuffer()
{
  if(started)
  {
    try {
      // Stop and close the stream
      _audio->stopStream();
      _audio->closeStream();
    }
    catch (RtError &error) {
      error.printMessage();
    }
    started=false;
  }
}


//   std::cout << "init RtAudio...done" << std::endl;

//   std::cout << "Opening RtAudio Stream..." << std::endl;
//   std::cout << Engine::signalInfo().sampleRate() << "hz " << std::endl;
//   std::cout << "Frames per buffer: " << framesPerBuffer << std::endl;

//   int nbBuffers = Pa_GetMinNumBuffers(framesPerBuffer, Engine::signalInfo().sampleRate());
//   std::cout << "Number of buffers: " << nbBuffers << std::endl;

//   _RingBufferSize = framesPerBuffer * 16;

//   _LBuffer.resize(_RingBufferSize);

//   std::cout << "Opening RtAudio Stream...done" << std::endl;

//   std::cout << "Starting RtAudio stream..." << std::endl;


//   if (err != paNoError)
//     std::cout << Pa_GetErrorText(err) << std::endl;
//   else
//     std::cout << "Starting RtAudio stream...done" << std::endl;
// }

int Gear_STKAudioOutput::bufferCallback(char *output_buffer, int frames_per_buffer, void *user_data)
{
  static Gear_STKAudioOutput *parent = (Gear_STKAudioOutput*)user_data;
  static int lindex=0;
  //static int rindex=0;
  SignalType& lbuffer = parent->_LBuffer;

  int ringBufferSize = parent->_RingBufferSize;

  //Signal_T *rbuffer = parent->_RBuffer;

  float *out = (float*)output_buffer;

  for (int i=0; i<frames_per_buffer; i++ )
  {
    *out++ = lbuffer[lindex++];        
    std::cerr<<lbuffer[lindex-1]<< " ";        
  }
  lindex %= ringBufferSize;

  return 0;
}
