// Gear_SimpleDelay.cpp: implementation of the Gear_SimpleDelay class.
//
//////////////////////////////////////////////////////////////////////

#include "Gear_SimpleDelay.h"

#include "Engine.h"

#include "GearMaker.h"

Register_Gear(MAKERGear_SimpleDelay, Gear_SimpleDelay, "SimpleDelay")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Gear_SimpleDelay::Gear_SimpleDelay(Engine *engine, std::string name) : Gear(engine, "SimpleDelay", name)
{
	_PARAM_FEEDBACK = addPlugSignalIn("Feedback", 0.3f);
    _PARAM_TIME = addPlugSignalIn("Time", 0.3f);
    _AUDIO_IN = addPlugSignalIn("Input", 0.0f);
	_AUDIO_OUT = addPlugSignalOut("Output");	
	_SmoothTime=false;
}

Gear_SimpleDelay::~Gear_SimpleDelay()
{

}

bool Gear_SimpleDelay::ready()
{
    return (_AUDIO_IN->connected() && _AUDIO_OUT->connected());
}

float _HARDCLIP(float X, float MIN, float MAX) 
{
	if(X<MIN)
			return MIN;
	if(X>MAX)
			return MAX;
	return X;
}

void Gear_SimpleDelay::runAudio()
{    
/*     Signal_T *bufferfeed  = _PARAM_FEEDBACK->buffer();                                                                      */
/*     Signal_T *buffertime  = _PARAM_TIME->buffer();                                                                          */
/*     Signal_T *bufferin = _AUDIO_IN->buffer();                                                                               */
/*     Signal_T *bufferout = _AUDIO_OUT->buffer();                                                                             */
/*                                                                                                                             */
/*     static double t=0;                                                                                                      */
/*     int signal_blocksize = _Engine->audioBlockSize();                                                                       */
/*                                                                                                                             */
/*     if(!_SmoothTime)                                                                                                        */
/*     {                                                                                                                       */
/*         int ndefaults;                                                                                                      */
/*         Signal_T defaultval;                                                                                                */
/*         Signal_T * block1,*block2;                                                                                          */
/*         int block1len,block2len;                                                                                            */
/*                                                                                                                             */
/*         // use first value of buffertime as delay time for whole block processing (could use average)                       */
/*         int past_samples = _HARDCLIP(_PARAM_TIME->secondsToElements(*buffertime), 0, _PARAM_TIME->secondsToElements(2.0f)); */
/*                                                                                                                             */
/*         //_AUDIO_OUT->getPastTimeBlockInfo(-past_samples+1, signal_blocksize, ndefaults, defaultval,                        */
/*         //                              block1,block2,block1len,block2len);                                                 */
/*         int blockpos=0;                                                                                                     */
/*         while(ndefaults--)                                                                                                  */
/*             bufferout[blockpos] = bufferin[blockpos++] + defaultval;                                                        */
/*         while(block1len--)                                                                                                  */
/*             bufferout[blockpos] = bufferin[blockpos++] + *(block1++);                                                       */
/*         while(block2len--)                                                                                                  */
/*             bufferout[blockpos] = bufferin[blockpos++] + *(block2++);                                                       */
/*                                                                                                                             */
/*         if(blockpos!=signal_blocksize)                                                                                      */
/*             _TRACE("Bug here 123123 "<<endl);                                                                               */
/*                                                                                                                             */
/*     }                                                                                                                       */
/*     else                                                                                                                    */
/*     {                                                                                                                       */
/*         for(int i=0; i<signal_blocksize; i++)                                                                               */
/*             bufferout[i] = bufferin[i]                                                                                      */
/*                     + *_AUDIO_OUT->getPastTimeValue(_PARAM_TIME->secondsToElements(buffertime[i]));                         */
/*     }                                                                                                                       */

}
