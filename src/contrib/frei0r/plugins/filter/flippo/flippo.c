/* flippo.c */

/*
 * 07/11/2004 c.e. prelz
 *
 * My first frei0r effect - simple flipping
 */

#include "flippo.h"
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>

int f0r_init()
{
  return 1;
}

void f0r_deinit()
{
}

void f0r_get_plugin_info(f0r_plugin_info_t* flippoInfo)
{
  flippoInfo->name = "Flippo";
  flippoInfo->author = "Carlo Emilio";
  flippoInfo->plugin_type = F0R_PLUGIN_TYPE_FILTER;
  flippoInfo->color_model = F0R_COLOR_MODEL_BGRA8888;
  flippoInfo->frei0r_version = FREI0R_MAJOR_VERSION;
  flippoInfo->major_version = 0;
  flippoInfo->minor_version = 1;
  flippoInfo->num_params =  2;
  flippoInfo->explanation = "Flipping in x and y axis";
}

void f0r_get_param_info(f0r_param_info_t* info, int param_index)
{
  switch(param_index)
  {
  case 0:
    info->name="X axis";
    info->type=F0R_PARAM_BOOL;
    info->explanation="Flipping on the horizontal axis";
    break;
  case 1:
    info->name="Y axis";
    info->type=F0R_PARAM_BOOL;
    info->explanation = "Flipping on the vertical axis";
    break;
  }
}

f0r_instance_t f0r_construct(unsigned int width,unsigned int height)
{
  flippo_instance_t *inst=
    (flippo_instance_t*)malloc(sizeof(flippo_instance_t));
  
  inst->width=width;
  inst->height=height;

  return (f0r_instance_t)inst;
}

void f0r_destruct(f0r_instance_t instance)
{
  flippo_instance_t* inst = (flippo_instance_t*)instance;

  free(inst);
}

void f0r_set_param_value(f0r_instance_t instance,
			 f0r_param_t param,int param_index)
{
  assert(instance);
  flippo_instance_t *inst=(flippo_instance_t*)instance;
  
  switch(param_index)
  {
  case 0:
    inst->flippox=(*((double*)param))>=0.5 ? 1 : 0;
    break;
  case 1:
    inst->flippoy=(*((double*)param))>=0.5 ? 1 : 0;
    break;
  }
}

void f0r_get_param_value(f0r_instance_t instance,
			 f0r_param_t param,int param_index)
{
  assert(instance);
  
  flippo_instance_t *inst=(flippo_instance_t*)instance;
  
  switch(param_index)
  {
  case 0:
    *((double*)param)=(inst->flippox>0) ? 1.0 : 0.0;
    break;
  case 1:
    *((double*)param)=(inst->flippoy>0) ? 1.0 : 0.0;
    break;
  }
}

void f0r_update(f0r_instance_t instance,double time,
		const uint32_t *inframe, uint32_t *outframe)
{
  assert(instance);
  
  flippo_instance_t* inst=(flippo_instance_t*)instance;
  unsigned int w=inst->width;
  unsigned int h=inst->height;
  unsigned int x,y;
  
  for(y=0;y<=h;y++)
    for(x=0;x<=w;x++)
      outframe[y*w+x]=inframe[(inst->flippox ? h-y-1 : y )*w+
			      (inst->flippoy ? w-x-1 : x)];
}

