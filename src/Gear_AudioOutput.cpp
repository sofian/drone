#include <iostream>
#include "Gear_AudioOutput.h"
#include "GearMaker.h"
#include "Engine.h"


Register_Gear(MAKERGear_AudioOutput, Gear_AudioOutput, "AudioOutput")

const int Gear_AudioOutput::DEFAULT_FRAMES_PER_BUFFER=512;
const int Gear_AudioOutput::DEFAULT_NB_BUFFERS=16;

const std::string Gear_AudioOutput::SETTING_FRAMES_PER_BUFFER = "FramesPerBuffer";
const std::string Gear_AudioOutput::SETTING_NB_BUFFERS = "NbBuffers";

Gear_AudioOutput::Gear_AudioOutput(Engine *engine, std::string name) : 
    Gear(engine, "AudioOutput", name),     
    _RingBufferSize(512),
    _LBuffer(NULL),
    _RBuffer(NULL),
    _ReadIndex(0)

{
    _AUDIO_IN_LEFT = addPlugSignalIn("Left", 0.0f);    
    _AUDIO_IN_RIGHT = addPlugSignalIn("Right", 0.0f);    

    _settings.add(Property::INT, SETTING_FRAMES_PER_BUFFER, DEFAULT_FRAMES_PER_BUFFER);
    _settings.add(Property::INT, SETTING_NB_BUFFERS, DEFAULT_NB_BUFFERS);    

    _Mutex = new pthread_mutex_t();
    pthread_mutex_init(_Mutex, NULL);


}

Gear_AudioOutput::~Gear_AudioOutput()
{
    
}

bool Gear_AudioOutput::ready()
{
    return (_AUDIO_IN_LEFT->connected() || _AUDIO_IN_RIGHT->connected());
}

void Gear_AudioOutput::init()
{
    std::cout << "Initializing AudioOutput..." << std::endl;

    
    initPortAudio();
    
    //pthread_create(&_PlaybackThreadHandle, NULL, &Gear_AudioOutput::playbackThread, this);

    std::cout << "Initializing AudioOutput...done" << std::endl;
}

/* void *Gear_AudioOutput::playbackThread(void *param)                                                             */
/* {                                                                                                               */
/*     Gear_AudioOutput *parent = (Gear_AudioOutput*) param;                                                       */
/*     int readIndex=0;                                                                                            */
/*                                                                                                                 */
/*     //memset(parent->_LBuffer, 0 , sizeof(float) * parent->_RingBufferSize);                                    */
/*                                                                                                                 */
/*     signed short temp[parent->_RingBufferSize*2];                                                               */
/*     while(1)                                                                                                    */
/*     {                                                                                                           */
/*                                                                                                                 */
/*                                                                                                                 */
/*         //pthread_mutex_lock(parent->_Mutex);                                                                   */
/*         if (snd_pcm_writei(parent->_PCM, &(parent->_LBuffer[readIndex]), Engine::signalInfo().blockSize()) < 0) */
/*         {                                                                                                       */
/*              snd_pcm_prepare(parent->_PCM);                                                                     */
/*              std::cout << "underflow" << std::endl;                                                                       */
/*         }                                                                                                       */
/*                                                                                                                 */
/*         readIndex = (readIndex + Engine::signalInfo().blockSize()*2) % parent->_RingBufferSize;                 */
/*                                                                                                                 */
/*                                                                                                                 */
/*         //pthread_mutex_unlock(parent->_Mutex);                                                                 */
/*                                                                                                                 */
/*     }                                                                                                           */
/*                                                                                                                 */
/*     return 0;                                                                                                   */
/* }                                                                                                               */

void Gear_AudioOutput::onUpdateSettings()
{
    init();
}

void Gear_AudioOutput::runAudio()
{
    Signal_T *left_buffer  = _AUDIO_IN_LEFT->buffer();
    //Signal_T *right_buffer  = _AUDIO_IN_RIGHT->buffer();     
	int signal_blocksize = Engine::signalInfo().blockSize();
    static bool started = false;
    
    

    for (int i=0; i<signal_blocksize; i++)
        _LBuffer[_ReadIndex++] = left_buffer[i];
        
    _ReadIndex %= _RingBufferSize;    
    
    if (!started)
    {
        Pa_StartStream(_Stream);
        started=true;

        
    }
    

}


void Gear_AudioOutput::initPortAudio()
{
    PaError err;

    std::cout << "init PortAudio..." << std::endl;

    int framesPerBuffer = _settings.getInt(SETTING_FRAMES_PER_BUFFER);
    

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

    _RingBufferSize = framesPerBuffer * 16;

    if (_LBuffer!=NULL)
        delete[] _LBuffer;

    _LBuffer = new Signal_T[_RingBufferSize];


    err = Pa_OpenStream(
              &_Stream,
              paNoDevice,/* default input device*/
              0,              /* no input */
              paFloat32,  /* 32 bit floating point input*/
              NULL,
              Pa_GetDefaultOutputDeviceID(),
              1,          /* stereo output*/
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
}

int Gear_AudioOutput::portAudioCallback(void *, void *output_buffer, unsigned long frames_per_buffer,
                                        PaTimestamp, void *user_data)
{
    static Gear_AudioOutput *parent = (Gear_AudioOutput*)user_data;
    static int lindex=0;
    //static int rindex=0;
    Signal_T *lbuffer = parent->_LBuffer;

    int ringBufferSize = parent->_RingBufferSize;

    //Signal_T *rbuffer = parent->_RBuffer;

    float *out = (float*)output_buffer;
    
    for(unsigned int i=0; i<frames_per_buffer; i++ )    
        *out++ = lbuffer[lindex++];        
    
    lindex %= ringBufferSize;
    
    return 0;
}


