/* Gear_AmuseOeil.cpp
 * Copyright (C) 2004 Gephex crew
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

#include "Gear_AmuseOeil.h"
#include "Engine.h"
#include "DroneMath.h"
#include "AggWrapper.h"
#include "ColorSpace.h"

#include <iostream>


#include "DroneMath.h"

extern "C" {
Gear* makeGear()
{
  return new Gear_AmuseOeil();
}

}
Gear_AmuseOeil::Gear_AmuseOeil() : 
  Gear("AmuseOeil")
{
  addPlug(_P1_IN = new PlugIn<ValueType>(this, "P1", new ValueType(6, 0, 16)));
  addPlug(_FRIC_IN = new PlugIn<ValueType>(this, "FRIC", new ValueType(.1, 0, 1)));
  addPlug(_ACCEL_IN = new PlugIn<ValueType>(this, "ACCEL", new ValueType(.5, 0, 1)));
  addPlug(_P4_IN = new PlugIn<ValueType>(this, "P4", new ValueType(6, 0, 16)));
  addPlug(_HAVG_IN = new PlugIn<ValueType>(this, "HAVG", new ValueType(0.5, 0, 1)));
  addPlug(_SAVG_IN = new PlugIn<ValueType>(this, "SAVG", new ValueType(0.5, 0, 1)));
  addPlug(_VAVG_IN = new PlugIn<ValueType>(this, "VAVG", new ValueType(0.5, 0, 1)));
  addPlug(_AAVG_IN = new PlugIn<ValueType>(this, "AAVG", new ValueType(0.8, 0, 1)));
  addPlug(_HSPAN_IN = new PlugIn<ValueType>(this, "HSPAN", new ValueType(0.1, 0, 1)));
  addPlug(_SSPAN_IN = new PlugIn<ValueType>(this, "SSPAN", new ValueType(0.1, 0, 1)));
  addPlug(_VSPAN_IN = new PlugIn<ValueType>(this, "VSPAN", new ValueType(0.1, 0, 1)));
  addPlug(_ASPAN_IN = new PlugIn<ValueType>(this, "ASPAN", new ValueType(0.1, 0, 1)));

  addPlug(_SPEED_IN = new PlugIn<ValueType>(this, "SPEED", new ValueType(1, 0, 100)));
  addPlug(_SCALE_IN = new PlugIn<ValueType>(this, "SCALE", new ValueType(1, 0, 10)));

  addPlug(_PERTURB_IN = new PlugIn<ValueType>(this, "PERT", new ValueType(.5, 0, 1)));
  addPlug(_IPERTURB_IN = new PlugIn<ValueType>(this, "IPERT", new ValueType(1, 0, 100)));

  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT"));
  
  _curscale=0;
}

Gear_AmuseOeil::~Gear_AmuseOeil()
{

}

void Gear_AmuseOeil::addStripe()
{

  float _p1 = _P1_IN->type()->value();
  float _p2 = _FRIC_IN->type()->value();
  float _p3 = _ACCEL_IN->type()->value();
  float _p4 = _P4_IN->type()->value();
  float _p5 = _HAVG_IN->type()->value();
  float _p6 = _SAVG_IN->type()->value();
  float _p7 = _VAVG_IN->type()->value();
  float _p8 = _AAVG_IN->type()->value();

  float _p9 = _HSPAN_IN->type()->value();
  float _p10= _SSPAN_IN->type()->value();
  float _p11= _VSPAN_IN->type()->value();
  float _p12= _ASPAN_IN->type()->value();

  stripe_t s;
  s.initial[0]=s.target[0]=s.current[0] = uniform_bounded(0,1);
  s.initial[1]=s.target[1]=s.current[1] = uniform_bounded(0,.3);
  s.initial[2]=s.target[2]=s.current[2] = uniform_bounded(0,.3);
  s.initial[3]=s.target[3]=s.current[3] = uniform_bounded(_p5 - _p9/2, _p5 + _p9/2);
  s.initial[4]=s.target[4]=s.current[4] = uniform_bounded(_p6 - _p10/2, _p6 + _p10/2);
  s.initial[5]=s.target[5]=s.current[5] = uniform_bounded(_p7 - _p11/2, _p7 + _p11/2);
  s.initial[6]=s.target[6]=s.current[6] = uniform_bounded(_p8 - _p12/2, _p8 + _p12/2);
  s.mode = 0;
  s.transisteps = uniform_bounded(300,600);
  s.transicount = 0;

  for(int i=0;i<7;i++)
    s.a[i]=s.v[i]=0;
  _stripes.push_back(s);
}

void Gear_AmuseOeil::updateStripes()
{
  float inputs[20];
  
  inputs[1] = _P1_IN->type()->value();
  inputs[2] = _FRIC_IN->type()->value();
  inputs[3] = _ACCEL_IN->type()->value();
  inputs[4] = _P4_IN->type()->value();
  inputs[5] = _HAVG_IN->type()->value();
  inputs[6] = _SAVG_IN->type()->value();
  inputs[7] = _VAVG_IN->type()->value();
  inputs[8] = _AAVG_IN->type()->value();
  inputs[9] = _HSPAN_IN->type()->value();
  inputs[10] = _SSPAN_IN->type()->value();
  inputs[11] = _VSPAN_IN->type()->value();
  inputs[12] = _ASPAN_IN->type()->value();

  float pert = _PERTURB_IN->type()->value();
  float ipert = _IPERTURB_IN->type()->value();
  
  int avg_param2input[7]={-1,-1,-1,5,6,7,8};
  int span_param2input[7]={-1,-1,-1,9,10,11,12};
  
  float avg,span;
  int i,param;

  while(ipert-- > 0)
  {
    i = rand()%_stripes.size();
    param = rand()%7;
    if(avg_param2input[param]!=-1)
      avg = inputs[avg_param2input[param]];
    else
      avg = _stripes[i].initial[param];
    
    if(span_param2input[param]!=-1)
      span = inputs[span_param2input[param]];
    else
      span = .3;
    _stripes[i].target[param] = (avg + uniform_bounded(-span,span));
  }
  


  for(int i=0;i<_stripes.size();i++)
  {
    for(int param=0;param<7;param++)
    {
      if(rand()%100*_stripes.size()==0)
        _stripes[i].hickup=uniform_bounded(0,.3);
      _stripes[i].hickup/=1.02;
      if(uniform_bounded(0,1) < pert/ (500*_stripes.size()) )
      {
        if(avg_param2input[param]!=-1)
          avg = inputs[avg_param2input[param]];
        else
          avg = _stripes[i].initial[param];

        if(span_param2input[param]!=-1)
          span = inputs[span_param2input[param]];
        else
          span = .3;
        _stripes[i].target[param] = (avg + uniform_bounded(-span,span));
      }
      _stripes[i].v[param] += float(_stripes[i].target[param] - _stripes[i].current[param])/(2000.0*inputs[3]);
      _stripes[i].v[param] /= 1.0+(inputs[2]*.50);
      _stripes[i].last[param] = _stripes[i].current[param];
      _stripes[i].current[param] += _stripes[i].v[param];

    }
    if(_stripes[i].mode==0)
    {
      _stripes[i].transicount++;
      if(_stripes[i].transicount>=_stripes[i].transisteps)
        _stripes[i].mode=1;
    }
    if(_stripes[i].mode==2)
    {
      _stripes[i].transicount--;
      if(_stripes[i].transicount<=0)
        _stripes[i].mode=3;
    }

    //std::cerr<<_stripes[i].mode<<std::endl;
  }

  std::vector<stripe_t> temp;
  for(int i=0;i<_stripes.size();i++)
  {
    if(_stripes[i].mode!=3)
      temp.push_back(_stripes[i]);
  }
  _stripes = temp;

}

int Gear_AmuseOeil::aliveStripes()
{
  int alive=0;
  for(int i=0;i<_stripes.size();i++)
    if(_stripes[i].mode<2)
      alive++;
  //std::cerr<<"alive:"<<alive<<std::endl;

  return alive;
}

void Gear_AmuseOeil::killStripe()
{
//  std::cerr<<"kill!!"<<std::endl;
  for(int i=0;i<_stripes.size();i++)
    if(_stripes[i].mode!=2)
    {
      if(_stripes[i].mode==1)
      {
        _stripes[i].transisteps = uniform_bounded(300,600);
        _stripes[i].transicount = _stripes[i].transisteps;
      }
      _stripes[i].mode=2;
      return;
    }

}

bool Gear_AmuseOeil::ready()
{
  return(_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_AmuseOeil::runVideo()
{
  _image = _VIDEO_IN->type();
  if (_image->isNull())
    return;
  
  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());
  _data = (unsigned int*)_image->data();    
  _outData = (unsigned int*)_outImage->data();

  int _p1 = _P1_IN->type()->value();
  float _p2 = _FRIC_IN->type()->value();
  float _p3 = _ACCEL_IN->type()->value();
  float _p4 = _P4_IN->type()->value();
  float _p5 = _HAVG_IN->type()->value();
  float _p6 = _SAVG_IN->type()->value();
  float _p7 = _VAVG_IN->type()->value();
  float _p8 = _AAVG_IN->type()->value();

  float _p9 = _HSPAN_IN->type()->value();
  float _p10= _SSPAN_IN->type()->value();
  float _p11= _VSPAN_IN->type()->value();
  float _p12= _ASPAN_IN->type()->value();

  int _speed = CLAMP((int)_SPEED_IN->type()->value(),0,999);
  float _scale = _SCALE_IN->type()->value();
  
  while(_stripes.size() < _p1)
    addStripe();

   while(aliveStripes() > _p1)
     killStripe();

  memcpy(&_outData[0], &_data[0], _image->width() * _image->height() * 4);
  
  AggWrapper aggw(_outImage);


  for(;_speed;--_speed)
  {
    _curscale = .95*_curscale + .05*_scale;
    aggw.line(_speed-0.5, 0,_speed-0.5, _image->height(),agg::rgba8(0,0,0,255));    
    updateStripes();
      
    for(int i=0;i<_stripes.size();i++)
    {
      
      int r = CLAMP(_stripes[i].current[3],0.0f,1.0f)*360,g=CLAMP(_stripes[i].current[4],0.0f,1.0f)*255,b=CLAMP(_stripes[i].current[5],0.0f,1.0f)*255,a=CLAMP(_stripes[i].current[6],0.0f,1.0f)*255;
      float y = (_stripes[i].current[0]) * _image->width();
      float y1 = _curscale *CLAMP( _stripes[i].current[1],0.0f,1.0f) * _image->width();    
      float y2 = _curscale *CLAMP( _stripes[i].current[2],0.0f,1.0f) * _image->width();
      
      float ly = CLAMP(_stripes[i].last[0],0.0f,1.0f) * _image->width();
      float ly1 = _curscale *CLAMP( _stripes[i].last[1],0.0f,1.0f) * _image->width();    
      float ly2 = _curscale *CLAMP( _stripes[i].last[2],0.0f,1.0f) * _image->width();
      
      float mul = (_stripes[i].mode==1)?1 : ((float)_stripes[i].transicount / (float)_stripes[i].transisteps);
      
      hsv_to_rgb_int (&r,&g,&b);
//       aggw.quad(_speed, y-y1,
//                 _speed, y+y2,
//                 _speed+1, ly+ly2,
//                 _speed+1, ly-ly1,
//                 agg::rgba8(r,g,b,(float)a*mul));
      aggw.line( y-y1,_image->height()-_speed-0.5,
                 y+y2,_image->height()-_speed-0.5,
                agg::rgba8(r,g,b,(float)a*mul));
      
    }

  }


}


