#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// These implementations were taken from Torch 3 (http://www.torch.ch).

char xxpetit_message_pour_melanie[10000];

void errormsg(const char* msg, ...)
{
  va_list args;
  va_start(args,msg);
  vsprintf(xxpetit_message_pour_melanie, msg, args);
  printf("\n$ Error: %s\n\n", xxpetit_message_pour_melanie);
  fflush(stdout);
  va_end(args);
  exit(-1);
}

void warningmsg(const char* msg, ...)
{
  va_list args;
  va_start(args,msg);
  vsprintf(xxpetit_message_pour_melanie, msg, args);
  printf("! Warning: %s\n", xxpetit_message_pour_melanie);
  fflush(stdout);
  va_end(args);
}

void noticemsg(const char* msg, ...)
{
  va_list args;
  va_start(args,msg);
  vsprintf(xxpetit_message_pour_melanie, msg, args);
  printf("! Notice: %s\n",  xxpetit_message_pour_melanie);
  fflush(stdout);
  va_end(args);
}

void message(const char* msg, ...)
{
  va_list args;
  va_start(args,msg);
  vsprintf(xxpetit_message_pour_melanie, msg, args);
  printf("# %s\n", xxpetit_message_pour_melanie);
  fflush(stdout);
  va_end(args);
}

void print(const char* msg, ...)
{
  va_list args;
  va_start(args,msg);
  vsprintf(xxpetit_message_pour_melanie, msg, args);
  printf("%s", xxpetit_message_pour_melanie);
  fflush(stdout);
  va_end(args);
}
