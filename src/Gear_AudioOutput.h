#ifndef GEAR_AUDIOOUTPUT_INCLUDED
#define GEAR_AUDIOOUTPUT_INCLUDED

#include "Gear.h"
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

    void init();
    void runAudio();    

    PlugSignalIn* AUDIO_IN_LEFT(){return _AUDIO_IN_LEFT;};
    PlugSignalIn* AUDIO_IN_RIGHT(){return _AUDIO_IN_RIGHT;};
    
    bool ready();

protected:
    void onUpdateSettings();

private:

//PLUGS
    PlugSignalIn* _AUDIO_IN_LEFT;
    PlugSignalIn* _AUDIO_IN_RIGHT;
//


/*
    portaudio
*/    

    static const int DEFAULT_FRAMES_PER_BUFFER;
    static const int DEFAULT_NB_BUFFERS;

    PortAudioStream *_Stream;
    
    
    void initPortAudio();    
    
    int _RingBufferSize;

    Signal_T *_LBuffer;
    Signal_T *_RBuffer;

    int _ReadIndex;

    pthread_mutex_t *_Mutex;
    
    pthread_t _PlaybackThreadHandle;

    static int portAudioCallback(void *, void *output_buffer, unsigned long frames_per_buffer,
                                        PaTimestamp, void *user_data);

	
};

#endif

