#ifndef GEAR_COLORQUANTIZEALPHA_INCLUDED
#define GEAR_COLORQUANTIZEALPHA_INCLUDED

#include "Gear.h"
#include <assert.h>

#define DEFAULT_N_COLORS 16
#define CENTROIDS_THRESHOLD 10

// *** faire quelques tests et si ca marche toujours, enlever...
// *** parfois si on fait une seule iteration ca prend un certain temps... verifier la contrepartie en load average...
#define MAX_ITER 0
//#define DIST(x) (x >= 0 ? x : -x)

// *** a changer (fonction a la place ou direct dans le code)
#define DIST(x) (x*x)
//#define DIST(x) abs(x)

class PlugSignal;

class Gear_ColorQuantizeAlpha : public Gear  
{
public:
    Gear_ColorQuantizeAlpha(Engine *engine, std::string name);
    virtual ~Gear_ColorQuantizeAlpha();

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

    int _nColors;
    RGBAint *_centroids;
#if MAX_ITER
    RGBAint *_prevCentroids;
#endif
  
    //selected centroids map for each pixels
    int *_clusters;
    
    //number of pixels falling in each centroids
    int *_clusterSizes;

    int _iterSizeY;
    int _iterSizeX;
    int _iterSize;

    unsigned char *_imageIn;
    unsigned char *_imageOut;    

};

#endif 
