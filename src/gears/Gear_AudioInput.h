#ifndef GEAR_AUDIOINPUT_INCLUDED
#define GEAR_AUDIOINPUT_INCLUDED

#include "Gear.h"
#include "SignalType.h"
#include <portaudio.h>

class PlugSignal;

class Gear_AudioInput : public Gear  
{
public:

//CONFIG PARAMETERS
  static const std::string SETTING_FRAMES_PER_BUFFER;
  static const std::string SETTING_NB_BUFFERS;
//

  Gear_AudioInput(Engine *engine, std::string name);
  virtual ~Gear_AudioInput();

  void prePlay();    
  void postPlay();    
  
  void init();
  void runAudio();

  bool ready();

protected:
  void onUpdateSettings();


private:

//PLUGS
  PlugOut<SignalType>* _AUDIO_OUT_LEFT;
  PlugOut<SignalType>* _AUDIO_OUT_RIGHT;
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

  static int portAudioCallback(void *input_buffer, void *, unsigned long frames_per_buffer,
                               PaTimestamp, void *user_data);

};

#endif

