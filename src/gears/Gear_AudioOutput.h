#ifndef GEAR_AUDIOOUTPUT_INCLUDED
#define GEAR_AUDIOOUTPUT_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include <portaudio.h>



class PlugSignal;

class Gear_AudioOutput : public Gear  
{
public:

//CONFIG PARAMETERS
  static const std::string SETTING_FRAMES_PER_BUFFER;
  static const std::string SETTING_NB_BUFFERS;
//

  Gear_AudioOutput(Engine *engine, std::string name);
  virtual ~Gear_AudioOutput();

  void prePlay();    
  void postPlay();    

  void init();
  void runAudio();    

  PlugIn<SignalType>* AUDIO_IN_LEFT(){return _AUDIO_IN_LEFT;};
  PlugIn<SignalType>* AUDIO_IN_RIGHT(){return _AUDIO_IN_RIGHT;};

  bool ready();

protected:
  void onUpdateSettings();

private:

//PLUGS
  PlugIn<SignalType> *_AUDIO_IN_LEFT;
  PlugIn<SignalType> *_AUDIO_IN_RIGHT;
//



//portaudio

  static const int DEFAULT_FRAMES_PER_BUFFER;
  static const int DEFAULT_NB_BUFFERS;

  PortAudioStream *_stream;

  void initPortAudio();    

  int _ringBufferSize;

  SignalType _lBuffer;
  SignalType _rBuffer;
  int _lBufferIndex;

  int _readIndex;

  static int portAudioCallback(void *, void *output_buffer, unsigned long frames_per_buffer,
                               PaTimestamp, void *user_data);

};

#endif

