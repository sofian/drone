#include "Gear_ColorQuantizeWu.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

Register_Gear(MAKERGear_ColorQuantizeWu, Gear_ColorQuantizeWu, "ColorQuantizeWu")

Gear_ColorQuantizeWu::Gear_ColorQuantizeWu(Engine *engine, std::string name)
  : Gear(engine, "ColorQuantizeWu", name), _nColors(DEFAULT_N_COLORS),
    Ir(0), Ig(0), Ib(0), Qadd(0)
{
  _VIDEO_IN = addPlugVideoIn("ImgIN");
  _VIDEO_OUT = addPlugVideoOut("ImgOUT");
  _AMOUNT_IN = addPlugSignalIn("NColors", DEFAULT_N_COLORS);
}

Gear_ColorQuantizeWu::~Gear_ColorQuantizeWu()
{
  free(Qadd);
  free(Ig);
  free(Ib);
  free(Ir);
}

bool Gear_ColorQuantizeWu::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_ColorQuantizeWu::runVideo()
{
  // initialize
  _nColors = (int)_AMOUNT_IN->buffer()[0];
  
  _image = _VIDEO_IN->canvas();
  _outImage = _VIDEO_OUT->canvas();
  _outImage->allocate(_image->sizeX(), _image->sizeY());

  _data = _image->_data;
  _outData = _outImage->_data;

  size = _image->sizeX() * _image->sizeY();

  struct box	cube[MAXCOLOR];
  unsigned char	*tag;
  unsigned char	lut_r[MAXCOLOR], lut_g[MAXCOLOR], lut_b[MAXCOLOR];
  int		next;
  register long int	i, weight;
  register int	k;
  float		vv[MAXCOLOR], temp;

  Qadd = (unsigned short int *)realloc(Qadd,sizeof(short int)*size);
  
	/* input R,G,B components into Ir, Ig, Ib;
	   set size to width*height */

  Ir = (unsigned char*)realloc(Ir,size*sizeof(unsigned char));
  Ig = (unsigned char*)realloc(Ig,size*sizeof(unsigned char));
  Ib = (unsigned char*)realloc(Ib,size*sizeof(unsigned char));

  _imageIn = (unsigned char*)_data;
  for (i=0; i<size; ++i)
  {
    Ir[i] = *(_imageIn);
    Ig[i] = *(_imageIn+1);
    Ib[i] = *(_imageIn+2);
    _imageIn+=4;
  }

  memset(wt, 0, 33*33*33*sizeof(long int));
  memset(mr, 0, 33*33*33*sizeof(long int));
  memset(mg, 0, 33*33*33*sizeof(long int));
  memset(mb, 0, 33*33*33*sizeof(long int));
  memset(m2, 0, 33*33*33*sizeof(float));
  
	Hist3d((long int*)wt, (long int*)mr, (long int*)mg, (long int*)mb, (float*)m2); //printf("Histogram done\n");

	M3d((long int*)wt, (long int*)mr, (long int*)mg, (long int*)mb, (float*)m2);    //printf("Moments done\n");

	cube[0].r0 = cube[0].g0 = cube[0].b0 = 0;
	cube[0].r1 = cube[0].g1 = cube[0].b1 = 32;
	next = 0;
  for(i=1; i<_nColors; ++i){
    if (Cut(&cube[next], &cube[i])) {
      /* volume test ensures we won't try to cut one-cell box */
      vv[next] = (cube[next].vol>1) ? Var(&cube[next]) : 0.0;
      vv[i] = (cube[i].vol>1) ? Var(&cube[i]) : 0.0;
    } else {
      vv[next] = 0.0;   /* don't try to split this box again */
      i--;              /* didn't create box i */
    }
    next = 0; temp = vv[0];
    for(k=1; k<=i; ++k)
      if (vv[k] > temp) {
        temp = vv[k]; next = k;
      }
    if (temp <= 0.0) {
      _nColors = i+1;
      fprintf(stderr, "Only got %d boxes\n", _nColors);
      break;
    }
	}
  //  printf("Partition done\n");

	/* the space for array m2 can be freed now */

	tag = (unsigned char *)malloc(33*33*33);
	if (tag==NULL) {printf("Not enough space\n"); exit(1);}
	for(k=0; k<_nColors; ++k){
    Mark(&cube[k], k, tag);
    weight = Vol(&cube[k], wt);
    if (weight) {
      lut_r[k] = Vol(&cube[k], mr) / weight;
      lut_g[k] = Vol(&cube[k], mg) / weight;
      lut_b[k] = Vol(&cube[k], mb) / weight;
    }
    else{
      fprintf(stderr, "bogus box %d\n", k);
      lut_r[k] = lut_g[k] = lut_b[k] = 0;		
    }
	}

//   for (k=0; k<_nColors; ++k)
//   {
//     printf("(%d,%d,%d) ",lut_r[k], lut_g[k], lut_b[k]);
//   }
//   printf("\n");
  
  //	for(i=0; i<size; ++i) Qadd[i] = tag[Qadd[i]];
  _imageOut = (unsigned char*)_outData;
  for (i=0; i<size; ++i)
  {
    unsigned short int k = tag[Qadd[i]];
    *_imageOut++ = lut_r[k];
    *_imageOut++ = lut_g[k];
    *_imageOut++ = lut_b[k];
    *_imageOut++ = 0;
  }
  free(tag);

}
