#include "Gear_ApplyDisplaceGrid.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_ApplyDisplaceGrid, Gear_ApplyDisplaceGrid, "ApplyDisplaceGrid")

Gear_ApplyDisplaceGrid::Gear_ApplyDisplaceGrid(Engine *engine, std::string name) : Gear(engine, "ApplyDisplaceGrid", name)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoTypeRGBA>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "ImgOUT"));
  addPlug(_GRID = new PlugIn<DisplaceGrid>(this, "Grid"));
}

Gear_ApplyDisplaceGrid::~Gear_ApplyDisplaceGrid()
{
}

void Gear_ApplyDisplaceGrid::init()
{
}

bool Gear_ApplyDisplaceGrid::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_ApplyDisplaceGrid::runVideo()
{
  _image = _VIDEO_IN->type();
  _grid = _GRID->type();
  _outImage = _VIDEO_OUT->type();

  _imsizeY = _image->height();
  _imsizeX = _image->width();

  _gridsizeY = _grid->height();
  _gridsizeX = _grid->width();

  _outImage->resize(_imsizeX, _imsizeY);


  if(_imsizeX != _gridsizeX || _imsizeY != _gridsizeY)
  {
    WARNING("applyDisplaceGrid : gridsize(%i,%i) != imageSize(%i,%i).. resizing not supported yet",_gridsizeX,_gridsizeY,_imsizeX,_imsizeY);
    return;
  }
  
  ////////////////////////////
  
  _gridData = (float*)(_GRID->type()->data());
  _data = (unsigned char*)_image->data();    
  _outData = (unsigned char*)_outImage->data();
  
  for (int y=0;y<_imsizeY;y++)
  {
    for (int x=0;x<_imsizeX;x++)
    {
      _nx = (int)(x + *_gridData++);
      _ny = (int)(y + *_gridData++);
      
      _nx=MIRROR_CLAMP(_nx,0,_imsizeX-1);
      _ny=MIRROR_CLAMP(_ny,0,_imsizeY-1);

      ASSERT_ERROR(_nx>=0 && _nx<=_imsizeX-1);
      ASSERT_ERROR(_ny>=0 && _ny<=_imsizeY-1);      

      _data = (unsigned char*)(_image->row(_ny)+_nx);
  
      //NOTICE("data %i _outData %i _nx %i _ny %i x %i y %i ", _data, _outData, _nx-x, _ny-y, x, y);
      memcpy(_outData,_data,sizeof(RGBA));
      _outData+=4;
    }
  }   
  

}
