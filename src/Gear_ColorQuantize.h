#ifndef GEAR_COLORQUANTIZE_INCLUDED
#define GEAR_COLORQUANTIZE_INCLUDED

#include "Gear.h"
#include <assert.h>
#include "Timing.h"

#define DEFAULT_N_COLORS 16

class PlugSignal;

class Gear_ColorQuantize : public Gear  
{
public:
    Gear_ColorQuantize(Engine *engine, std::string name);
    virtual ~Gear_ColorQuantize();

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

    //selected centroids map for each pixels
    int *_clusters;
    
    //number of pixels falling in each centroids
    int *_clusterSizes;

    int _iterSizeY;
    int _iterSizeX;
    int _iterSize;

    int _r;
    int _g;
    int _b;

    unsigned char *_imageIn;
    unsigned char *_imageOut;    

};

#endif 
