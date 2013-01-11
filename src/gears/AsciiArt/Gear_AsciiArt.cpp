/* Gear_AsciiArt.cpp
 * Copyright (C) 2004 Mathieu Petit-Clair
 * Some code inspired from other gears by Jean-Sebastien Senecal
 * This file is part of Drone.
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

#include <iostream>

#include "Gear_AsciiArt.h"                       
#include "Engine.h"

#include "GearMaker.h"
#include "DroneMath.h"

aa_context *_aaContext;
aa_hardware_params _aaHDParams;
aa_renderparams *_aaRParams;

extern "C" {
Gear* makeGear(Schema *schema, QString uniqueName)
{
  return new Gear_AsciiArt(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "AsciiArt";
  gearInfo.classification = GearClassifications::video().IO().instance();
  return gearInfo;
}
}

Gear_AsciiArt::Gear_AsciiArt(Schema *schema, QString uniqueName) : 
  Gear(schema, "AsciiArt", uniqueName)
{
  //  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOut"));
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIn"));

  EnumType *renderingFunc = new EnumType(N_RENDERING_TYPES, FAST);
  renderingFunc->setLabel(FAST, "Fast"); 
  renderingFunc->setLabel(GOOD, "Good");
  addPlug(_RENDER_IN =  new PlugIn<EnumType>(this, "Render", renderingFunc));

  //EnumType *ditheringFunc = new EnumType(N_DITHERING_TYPES, NONE);
  EnumType *ditheringFunc = new EnumType(AA_DITHERTYPES, AA_NONE);
  ditheringFunc->setLabel(AA_NONE, "None"); 
  ditheringFunc->setLabel(AA_ERRORDISTRIB, "Error-distrib"); 
  ditheringFunc->setLabel(AA_FLOYD_S, "Floyd-Steinberg"); 
  addPlug(_DITHER_IN =  new PlugIn<EnumType>(this, "Dither", ditheringFunc));

  /* ca ne donne pas de tres bon resultat.. ca depend des fontes.. */
  //   EnumType *charFunc = new EnumType(N_CHARSET_TYPES, BASIC);
  //   charFunc->setLabel(BASIC, "7 Bit"); 
  //   charFunc->setLabel(EXTENDED, "8 Bit"); 
  //   addPlug(_CHAR_IN =  new PlugIn<EnumType>(this, "CharSet", charFunc));
  
  addPlug(_BRIGHTNESS_IN = new PlugIn<ValueType>(this, "Brightness", new ValueType(0, 0, 127)));
  addPlug(_CONTRAST_IN = new PlugIn<ValueType>(this, "Contrast", new ValueType(0, 0, 255)));
  addPlug(_RANDOM_IN = new PlugIn<ValueType>(this, "Noise", new ValueType(0, 0, 255)));

}

Gear_AsciiArt::~Gear_AsciiArt()
{
  if ( _aaContext != NULL ) {
    aa_close(_aaContext);
  }
}

void Gear_AsciiArt::internalInit() 
{
  aa_parseoptions (NULL, NULL, NULL, NULL);
  _aaContext = NULL;

  _renderingSpeed = (eRenderingSpeed)CLAMP((int)_RENDER_IN->type()->value(), (int)FAST, (int)GOOD);
  //_ditheringType = (eDitheringType)CLAMP((int)_DITHER_IN->type()->value(), (int)NONE, (int)FLOYD);
  _ditheringType = (aa_dithering_mode)CLAMP((int)_DITHER_IN->type()->value(), (int)AA_NONE, (int)AA_FLOYD_S);

  _brightness = CLAMP((int)_BRIGHTNESS_IN->type()->value(), 0, 255);
  _contrast = CLAMP((int)_CONTRAST_IN->type()->value(), 0, 127);
  _randomNoise = CLAMP((int)_RANDOM_IN->type()->value(), 0, 255);

}

bool Gear_AsciiArt::ready()
{
  return _VIDEO_IN->connected(); // && _VIDEO_OUT->connected());
}

void Gear_AsciiArt::onUpdateSettings() 
{
}

void Gear_AsciiArt::runVideo()
{  
  _inImage = _VIDEO_IN->type();
  ASSERT_ERROR(_inImage);

  if (_inImage->isNull())
    return;

  _sizeX = _inImage->width();
  _sizeY = _inImage->height();

  _aaHDParams = aa_defparams;
  _aaHDParams.width = _sizeX / 2 + 1;  /* ceil .. */
  _aaHDParams.height = _sizeY / 2 + 1;
  _aaHDParams.font = &aa_font16; /* c'est la seule qui fonctionne, apres quelques tests.. */
  //_aaHDParams.supported = AA_EXTENDED;  /* mauvais resultats : c'est pas beau. */

  if ( _aaContext == NULL ) {
    //_aaContext = aa_autoinit(&_aaHDParams); /* ca, ca sort dans le terminal.. */
    _aaContext = aa_init(&X11_d, &_aaHDParams, NULL);

    if ( _aaContext == NULL ) {
      fprintf(stderr, "Error while initializing aalib\n");
      exit(-1);  /* est-ce trop violent? */
    }
  }

  /* drawing */
  _inData = (unsigned char*)_inImage->data();  
  for (int y=0; y<_sizeY; y++) 
  {
    for (int x=0; x<_sizeX; x++) 
    {
      aa_putpixel(_aaContext, x, y, *_inData++);
      _inData++;
      _inData++;
      _inData++;
    }
  }

  /* rendering */
  _renderingSpeed = (eRenderingSpeed)CLAMP((int)_RENDER_IN->type()->value(), (int)FAST, (int)GOOD);

  switch (_renderingSpeed) 
  {
  case FAST : 
    aa_fastrender(_aaContext, 0, 0, aa_scrwidth(_aaContext), aa_scrheight(_aaContext));
    break;

  case GOOD :
    _aaRParams = aa_getrenderparams();

    //_ditheringType = (eDitheringType)CLAMP((int)_DITHER_IN->type()->value(), (int)NONE, (int)FLOYD);
    _ditheringType = (aa_dithering_mode)CLAMP((int)_DITHER_IN->type()->value(), (int)AA_NONE, (int)AA_FLOYD_S);
    _aaRParams->dither = _ditheringType;

    _contrast = CLAMP((int)_CONTRAST_IN->type()->value(), 0, 127);
    _aaRParams->contrast = _contrast;

    _brightness = CLAMP((int)_BRIGHTNESS_IN->type()->value(), 0, 255);
    _aaRParams->bright = _brightness;

    _randomNoise = CLAMP((int)_RANDOM_IN->type()->value(), 0, 255);
    _aaRParams->randomval = _randomNoise;

    aa_render(_aaContext, _aaRParams, 0, 0, 
              aa_scrwidth(_aaContext), aa_scrheight(_aaContext));    
    break;
  }

  /* flushing to screen */
  aa_flush(_aaContext);
  
}

