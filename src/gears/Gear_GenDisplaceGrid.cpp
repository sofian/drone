#include "Gear_GenDisplaceGrid.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_GenDisplaceGrid, Gear_GenDisplaceGrid, "GenDisplaceGrid")

Gear_GenDisplaceGrid::Gear_GenDisplaceGrid(Engine *engine, std::string name) : Gear(engine, "GenDisplaceGrid", name)
{
  addPlug(_GRID_OUT = new PlugOut<DisplaceGrid>(this, "GRID"));
  addPlug(_TYPE = new PlugIn<ValueType>(this, "TYPE", new ValueType(0,0,1)));
  addPlug(_PARAM1 = new PlugIn<ValueType>(this, "PARAM1", new ValueType(3, 0, 50)));
  addPlug(_PARAM2 = new PlugIn<ValueType>(this, "PARAM2", new ValueType(0, -M_PI, M_PI)));
  addPlug(_PARAM3 = new PlugIn<ValueType>(this, "PARAM3", new ValueType(2*M_PI, 0, 2*M_PI)));
  addPlug(_XSIZE = new PlugIn<ValueType>(this, "XSIZE", new ValueType(1,1,300)));
  addPlug(_YSIZE = new PlugIn<ValueType>(this, "YSIZE", new ValueType(1,1,300)));

  // hum.. ugly
  _lastparam1=_param1+1;
  _lastparam2=_param2+1;
  _lastparam3=_param3+1;
}

Gear_GenDisplaceGrid::~Gear_GenDisplaceGrid()
{
}

void Gear_GenDisplaceGrid::init()
{
}

bool Gear_GenDisplaceGrid::ready()
{
  return(_GRID_OUT->connected() );
}

void Gear_GenDisplaceGrid::runVideo()
{
  _type = _TYPE->type()->intValue();
  _xsize = _XSIZE->type()->intValue();
  _ysize = _YSIZE->type()->intValue();

  _param1 = _PARAM1->type()->value();
  _param2 = _PARAM2->type()->value();
  _param3 = _PARAM3->type()->value();
  
  // we should have a generic machanism for this..
  if(_param1==_lastparam1 && _param2==_lastparam2 && _param3==_lastparam3)
    return;
  _lastparam1=_param1;
  _lastparam2=_param2;
  _lastparam3=_param3;
  
  _halfdiag = sqrt(_xsize*_xsize+_ysize*_ysize); 
  
  _gridOut = _GRID_OUT->type();
  _gridOut->resize(_xsize,_ysize);

  _gridData = _gridOut->data();
  
  float fx,fy,theta,rho;

  fy = - _ysize/2;
  
  switch(_type)
  {
  case 0:
    for (int y=0 ; y<_ysize ; y++)
    {
      fx = - _xsize/2;
      
      for (int x=0 ; x<_xsize ; x++)
      {
        rho = fastPolarDist(fx,fy);
        theta = fastPolarAngle(fx,fy) + _param2;
        
        rho *= _param1;
        
        rho=MIRROR_CLAMP(rho,0.0f,_param3);
        
//rho /= -_halfdiag;
        //NOTICE("gd %i rho %f theta %f x %i y %i xsize %i ysize %i",_gridData, rho,theta, x,y,_xsize,_ysize);
        
        _gridData->xdisp = cos(theta)*rho;//fastPolarX(rho,theta);
        _gridData->ydisp = sin(theta)*rho;//fastPolarY(rho,theta);
        _gridData++;
        fx++;
      }
      fy++;
    }   

  break;

  case 1:

    for (float y=0 ; y<_ysize ; y++)
    {
      for (float x=0 ; x<_xsize ; x++)
      {
        _gridData->xdisp = cos(x/_xsize*_param1)*_param2;//fastPolarX(rho,theta);
        _gridData->ydisp = sin(y/_ysize*_param1)*_param2;//fastPolarY(rho,theta);
        _gridData++;
        
      }
    }   
  break;
  }
}
