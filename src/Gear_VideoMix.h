#ifndef GEAR_VIDEOMIX_INCLUDED
#define GEAR_VIDEOMIX_INCLUDED

#include "Gear.h"
#include "Utils.h"

class Gear_VideoMix : public Gear
{
  enum eVideoMixType {
    BLEND,            // 0
    SHADE,
    DARKEN,
    LIGHTEN,
    HUE_ONLY,
    SATURATION_ONLY,  // 5
    VALUE_ONLY,
    COLOR_ONLY,
    MULTIPLY,
    DIVIDE,
    SCREEN,           // 10
    OVERLAY,
    DODGE,
    BURN,
    HARDLIGHT,
    SOFTLIGHT,        // 15
    GRAIN_EXTRACT,
    GRAIN_MERGE,
    ADD,
    SUBTRACT,
    DIFFERENCE,       // 20
    //    DISSOLVE, // XXX trop de paramètres, faire dans une gear differente
    //    REPLACE,  // XXX idem
    N_VIDEOMIX_TYPES };
  static const std::string SETTING_MIX_FUNCTION;

public:

  Gear_VideoMix(Engine *engine, std::string name);
  virtual ~Gear_VideoMix();
    
  void runVideo();

  PlugVideoIn* VIDEO_IN_A(){return _VIDEO_IN_A;}
  PlugVideoIn* VIDEO_IN_B(){return _VIDEO_IN_B;}
  PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;}
  PlugSignalIn* AMOUNT_IN(){return _AMOUNT_IN;};
  PlugSignalIn* MIXFUNC_IN(){return _MIXFUNC_IN;};

  bool ready();

private:

  PlugVideoIn *_VIDEO_IN_A;
  PlugVideoIn *_VIDEO_IN_B;
  PlugVideoOut *_VIDEO_OUT;
  PlugSignalIn *_AMOUNT_IN;
  PlugSignalIn *_MIXFUNC_IN;
  
  //local var
  Video_T *_imageA; 
  Video_T *_imageB; 
  Video_T *_outImage; 
  RGBA *_dataA;
  RGBA *_dataB;
  RGBA *_outData;
  RGBA *_largerImage;
  RGBA *_tallerImage;

  int _iterSizeY;
  int _iterSizeX;    
  int _imageASizeX;
  int _imageASizeY;
  int _imageBSizeX;
  int _imageBSizeY;
  int _imageOutSizeX;
  int _imageOutSizeY;

  eVideoMixType _mixType;
  
  double *_mmxImageInA;
  double *_mmxImageInB;
  double *_mmxImageOut;

  int _remainIterSizeY;
  int _remainIterSizeX;
       

};

#endif
