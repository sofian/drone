#ifndef GEAR_COLORQUANTIZER_INCLUDED
#define GEAR_COLORQUANTIZER_INCLUDED

#include "Gear.h"
#include <assert.h>

#define DEFAULT_N_COLORS 16
#define CENTROIDS_THRESHOLD 10
#define MAX_ITER 10
//#define DIST(x) (x >= 0 ? x : -x)
#define DIST(x) (x*x)


class PlugSignal;

class Gear_ColorQuantizer : public Gear  
{
public:
  Gear_ColorQuantizer(Engine *engine, std::string name);
  virtual ~Gear_ColorQuantizer();

  void runVideo();

  PlugVideoIn* VIDEO_IN(){return _VIDEO_IN;};
  PlugVideoOut* VIDEO_OUT(){return _VIDEO_OUT;};

  bool ready();

private:

  PlugVideoIn *_VIDEO_IN;
  PlugVideoOut *_VIDEO_OUT;

  //local var
  Video_T *_image; 
  Video_T *_outImage; 
  RGBA *_data;
  RGBA *_outData;

  unsigned short _nColors;
  RGBAint *_centroids;
  RGBAint *_prevCentroids;

  //selected centroids map for each pixels
  unsigned short *_mapping;

  //number of pixels falling in each centroids
  unsigned short *_nInMapping;
  unsigned short *_prevNInMapping;

  float *_logNInMapping;

  bool _initColors;

  int _iterSizeY;
  int _iterSizeX;

  int *_mmxImageIn;
  int *_mmxImageOut;
  unsigned long long int _mmxColor;

  short _r;
  short _g;
  short _b;
  short _a;

  unsigned char *_imageIn;
  unsigned char *_imageOut;    

};

#endif 
