/* contrast0r.c
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
 * This file is a Frei0r plugin.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "frei0r.h"

#define SIZE_BGRA 4

__inline int MAX(int a, int b)
{
  return (a > b ? a : b);
}

__inline int MIN(int a, int b)
{
  return (a < b ? a : b);
}

__inline void subtract_acc(uint32_t *dst, const uint32_t *src)
{
  int n=SIZE_BGRA;
  while (n--)
    *dst++ -= *src++;
}

__inline void add_acc(uint32_t *dst, const uint32_t *src)
{
  int n=SIZE_BGRA;
  while (n--)
    *dst++ += *src++;
}

__inline void divide(unsigned char *dst, const uint32_t *src, const unsigned int val)
{
  int n=SIZE_BGRA;
  while (n--)
    *dst++ = *src++ / val;
}

typedef struct squareblur_instance
{
  unsigned int width;
  unsigned int height;
  double kernel; /* the kernel size, as a percentage of the biggest of width and height */
  uint32_t *mem; /* memory accumulation matrix of uint32_t (size = acc_width*acc_height*SIZE_BGRA) */
  uint32_t **acc; /* accumulation matrix of pointers to SIZE_BGRA consecutive uint32_t in mem (size = acc_width*acc_height) */
} squareblur_instance_t;

/* Clamps a int32-range int between 0 and 255 inclusive. */
unsigned char CLAMP0255(int32_t a)
{
  return (unsigned char)
    ( (((-a) >> 31) & a)  // 0 if the number was negative
      | (255 - a) >> 31); // -1 if the number was greater than 255
}

/* Updates the summed area table. */
void update_summed_area_table(squareblur_instance_t *inst, const uint32_t *src)
{
  register unsigned char *iter_data;
  register uint32_t *iter_mem;
  register unsigned int i, x, y;

  uint32_t acc_buffer[SIZE_BGRA]; /* accumulation buffer */

  unsigned int row_width;
  unsigned int width, height;
  unsigned int cell_size;
  
  /* Compute basic params. */
  width = inst->width+1;
  height = inst->height+1;
  row_width = SIZE_BGRA * width;
  cell_size = SIZE_BGRA * sizeof(uint32_t);
  
  /* Init iterators. */
  iter_data = (unsigned char*) src;
  iter_mem  = inst->mem;

  /* Process first row (all zeros). */
  memset(iter_mem, 0, row_width * cell_size);
  iter_mem += row_width;
  
  if (height >= 1)
  {
    /* Process second row. */
    memset(acc_buffer, 0, cell_size);
    memset(iter_mem,   0, cell_size); /* first column is void */
    iter_mem += SIZE_BGRA;
    for (x=1; x<width; ++x)
    {
      for (i=0; i<SIZE_BGRA; ++i)
        *iter_mem++ = (acc_buffer[i] += *iter_data++);
    }
    
    /* Process other rows. */
    for (y=2; y<height; ++y)
    {
      /* Copy upper line. */
      memcpy(iter_mem, iter_mem - row_width, row_width * sizeof(uint32_t));
      
      /* Process row. */
      memset(acc_buffer, 0, cell_size);
      memset(iter_mem,   0, cell_size); /* first column is void */
      iter_mem += SIZE_BGRA;
      for (x=1; x<width; ++x)
      {
        for (i=0; i<SIZE_BGRA; ++i)
          *iter_mem++ += (acc_buffer[i] += *iter_data++);
      }
    }
  }
}

int f0r_init()
{
  return 1;
}

void f0r_deinit()
{ /* no initialization required */ }

void f0r_get_plugin_info(f0r_plugin_info_t* inverterInfo)
{
  inverterInfo->name = "Squareblur";
  inverterInfo->author = "Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal (Drone)";
  inverterInfo->plugin_type = F0R_PLUGIN_TYPE_FILTER;
  inverterInfo->color_model = F0R_COLOR_MODEL_BGRA8888;
  inverterInfo->frei0r_version = FREI0R_MAJOR_VERSION;
  inverterInfo->major_version = 0; 
  inverterInfo->minor_version = 1; 
  inverterInfo->num_params =  1; 
  inverterInfo->explanation = "Variable-size square blur";
}

void f0r_get_param_info(f0r_param_info_t* info, int param_index)
{
  switch(param_index)
  {
  case 0:
    info->name = "Kernel size";
    info->type = F0R_PARAM_DOUBLE;
    info->explanation = "The size of the kernel, as a proportion to its coverage of the image";
    break;
  }
}

f0r_instance_t f0r_construct(unsigned int width, unsigned int height)
{
  squareblur_instance_t* inst = 
    (squareblur_instance_t*)malloc(sizeof(squareblur_instance_t));
  unsigned int i;
  unsigned int acc_width, acc_height = height+1;
  uint32_t*  iter_mem;
  uint32_t** iter_acc;
  /* set params */
  inst->width = width; inst->height = height;
  acc_width = width+1; acc_height = height+1;
  inst->kernel = 0.0;
  /* allocate memory for the summed-area-table */
  inst->mem = (uint32_t*) malloc(acc_width*acc_height*SIZE_BGRA*sizeof(uint32_t));
  inst->acc = (uint32_t**) malloc(acc_width*acc_height*sizeof(uint32_t*));
  /* point at the right place */
  iter_mem = inst->mem;
  iter_acc = inst->acc;
  for (i=0; i<acc_width*acc_height; ++i)
  {
    *iter_acc++ = iter_mem;
    iter_mem += SIZE_BGRA;
  }
  return (f0r_instance_t)inst;
}

void f0r_destruct(f0r_instance_t instance)
{
  squareblur_instance_t* inst = 
    (squareblur_instance_t*)instance;
  free(inst->acc);
  free(inst->mem);
  free(instance);
}

void f0r_set_param_value(f0r_instance_t instance, 
                         f0r_param_t param, int param_index)
{
  assert(instance);
  squareblur_instance_t* inst = (squareblur_instance_t*)instance;

  switch(param_index)
  {
  case 0:
    /* kernel size */
    inst->kernel = *((double*)param);
    break;
  }
}

void f0r_get_param_value(f0r_instance_t instance,
                         f0r_param_t param, int param_index)
{
  assert(instance);
  squareblur_instance_t* inst = (squareblur_instance_t*)instance;
  
  switch(param_index)
  {
  case 0:
    *((double*)param) = inst->kernel;
    break;
  }
}

void f0r_update(f0r_instance_t instance, double time,
                const uint32_t* inframe, uint32_t* outframe)
{
  assert(instance);
  squareblur_instance_t* inst = (squareblur_instance_t*)instance;
  
  unsigned int width = inst->width;
  unsigned int height = inst->height;
  unsigned int acc_width = width+1; /* width of the summed area table */
  
  unsigned int kernel_size = (unsigned int) (inst->kernel * (MAX(width, height) / 2.0));

  unsigned int x, y;
  unsigned int x0, x1, y0, y1;
  unsigned int area;
  
  if (kernel_size <= 0)
  {
    /* No blur, just copy image. */
    memcpy(outframe, inframe, width*height*sizeof(uint32_t));
  }
  else
  {
    assert(inst->acc);
    unsigned char* dst = (unsigned char*)outframe;
    uint32_t** acc = inst->acc;
    uint32_t sum[SIZE_BGRA];
    unsigned int y0_offset, y1_offset;
    
    /* Compute the summed area table. */
    update_summed_area_table(inst, inframe);
    
    /* Loop through the image's pixels. */
    for (y=0;y<height;y++)
    {
      for (x=0;x<width;x++)
      {
        /* The kernel's coordinates. */
        x0 = MAX(x - kernel_size, 0);
        x1 = MIN(x + kernel_size + 1, width);
        y0 = MAX(y - kernel_size, 0);
        y1 = MIN(y + kernel_size + 1, height);
        
        /* Get the sum in the current kernel. */
        area = (x1-x0)*(y1-y0);
        
        y0_offset = y0*acc_width;
        y1_offset = y1*acc_width;
        
        /* it is assumed that (x0,y0) <= (x1,y1) */
        memcpy(sum, acc[y1_offset + x1], SIZE_BGRA*sizeof(uint32_t));
        subtract_acc(sum, acc[y1_offset + x0]);
        subtract_acc(sum, acc[y0_offset + x1]);
        add_acc(sum, acc[y0_offset + x0]);
        
        /* Take the mean and copy it to output. */
        divide(dst, sum, area);
        
        /* Increment iterator. */
        dst += SIZE_BGRA;
      }
    }
  }
}

