#ifndef INCLUDED_FLIPPO_H
#define INCLUDED_FLIPPO_H

#include "frei0r.h"
#include <stdint.h>

#define GRID_SIZE_LOG 3
#define GRID_SIZE (1<<GRID_SIZE_LOG)

typedef struct flippo_instance
{
  unsigned int width, height;
  char flippox,flippoy;
} flippo_instance_t;

#endif
