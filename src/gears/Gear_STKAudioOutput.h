#ifndef GEAR_STKAUDIOOUTPUT_INCLUDED
#define GEAR_STKAUDIOOUTPUT_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include <RtAudio.h>


class PlugSignal;

class Gear_STKAudioOutput : public Gear  
{
public:

//CONFIG PARAMETERS
  static const std::string SETTING_FRAMES_PER_BUFFER;
  static const std::string SETTING_NB_BUFFERS;
//

  Gear_STKAudioOutput(Engine *engine, std::string name);
  virtual ~Gear_STKAudioOutput();

  void init();
  void runAudio();    

  PlugIn<SignalType>* AUDIO_IN_LEFT(){return _AUDIO_IN_LEFT;};
  PlugIn<SignalType>* AUDIO_IN_RIGHT(){return _AUDIO_IN_RIGHT;};

  bool ready();

protected:
  void onUpdateSettings();
  void startReadingBuffer();
  void stopReadingBuffer();

  static bool started;


private:

//PLUGS
  PlugIn<SignalType> *_AUDIO_IN_LEFT;
  PlugIn<SignalType> *_AUDIO_IN_RIGHT;
//


/*
    portaudio
*/    

  static const int DEFAULT_FRAMES_PER_BUFFER;
  static const int DEFAULT_NB_BUFFERS;

  RtAudio * _audio;

  void initDevice();    

  int _RingBufferSize;

  SignalType _LBuffer;
  SignalType _RBuffer;

  int _ReadIndex;

  pthread_mutex_t *_Mutex;

  pthread_t _PlaybackThreadHandle;

  static int bufferCallback(char *output_buffer, int frames_per_buffer, void *user_data);


};

#endif

