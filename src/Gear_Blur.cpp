#include "Gear_Blur.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_Blur, Gear_Blur, "Blur")

Gear_Blur::Gear_Blur(Engine *engine, std::string name) : Gear(engine, "Blur", name), _amountMapData(0)
{
    _VIDEO_IN = addPlugVideoIn("ImgIN");
    _AMOUNT_MAP_IN = addPlugVideoIn("Amount Map");
    
    _VIDEO_OUT = addPlugVideoOut("ImgOUT");
    _AMOUNT_IN = addPlugSignalIn("Amount", 4.0f);
    _accumbuf = NULL;
}

Gear_Blur::~Gear_Blur()
{
}
    
bool Gear_Blur::ready()
{
    return (_VIDEO_IN->connected() && _VIDEO_OUT->connected());
}

void Gear_Blur::runVideo()
{
    int  *accumptrpred;
    

    
    _image = _VIDEO_IN->canvas();
    
    _outImage = _VIDEO_OUT->canvas();
    _outImage->allocate(_image->sizeX(), _image->sizeY());
    _sizeY = _image->sizeY();
    _sizeX = _image->sizeX();



    //we set a default value of 255 size of the image if amountmap not connected
    if (!_AMOUNT_MAP_IN->connected())
    {        
        _defaultAmountMapData.allocate(_image->sizeX(), _image->sizeY());
        _defaultAmountMapData.fill(255);
        _amountMapData = (unsigned char*)_defaultAmountMapData._data;
    }
    else
    {
        _amountMapData = (unsigned char*)(_AMOUNT_MAP_IN->canvas()->_data);
    }

    
//    _amountMapData = (unsigned char*)(_AMOUNT_MAP_IN->canvas()->_data);
//    std::cout << "ok" << std::endl;

    //
    if(_accumbuf == NULL) 
        _accumbuf = new int[_image->sizeX() * _image->sizeY()*4];        
        
    memset(_accumbuf, 0, _image->sizeX() * _image->sizeY() * 16);
        
    _data = (unsigned char*)_image->_data;    
    _outData = (unsigned char*)_outImage->_data;
    
/*    
    _blurSize=(int)_AMOUNT_IN->buffer()[0];
    
    _blurSize/=2;
    if (_blurSize<2)
        _blurSize=2;
*/
    /*
    // *** never happens...
    if(_blurSize==0)
    {
        memcpy(_outData, _data, _sizeX*_sizeY*4);
        return;
    }
    */
                
    
    // build accumulation buffer
    
    _tempAccumbuf = _accumbuf;

    for(int y=0;y<_sizeY;y++)
    {           
        _accR=0;
        _accG=0;
        _accB=0;
        _accA=0;

        if(y==0)
            for(int x=0;x<_sizeX;x++)
            {
                _accR+=*(_data);
                _accG+=*(_data+1);
                _accB+=*(_data+2);
                _accA+=*(_data+3);
                
                *(_tempAccumbuf+0) = _accR;
                *(_tempAccumbuf+1) = _accG;
                *(_tempAccumbuf+2) = _accB;
                *(_tempAccumbuf+3) = _accA;
                
                _tempAccumbuf+=4;
            }
        else
        {
            accumptrpred = _tempAccumbuf - _sizeX * 4;
            for(int x=0;x<_sizeX;x++)
            {
                
                _accR+=*(_data);
                _accG+=*(_data+1);
                _accB+=*(_data+2);
                _accA+=*(_data+3);
                
                *(_tempAccumbuf+0) = _accR + *(accumptrpred);
                *(_tempAccumbuf+1) = _accG + *(accumptrpred+1);
                *(_tempAccumbuf+2) = _accB + *(accumptrpred+2);
                *(_tempAccumbuf+3) = _accA + *(accumptrpred+3);
                
                _tempAccumbuf+=4;
                _data+=4;
                accumptrpred+=4;
            }
        }
    }
                  

   


                                       
    ////////////////////////////
    
    _tempAccumbuf = _accumbuf;
            
    for(int y=0;y<_sizeY;y++)
    {   
        
        for(int x=0;x<_sizeX;x++)
        {
            
            //std::cout<<_tempAccumbuf[y * _sizeX * 4 + x*4]<<",";
            

            //std::cout << a-b-c+d << std::endl;
            //memset(0,0,10000);

              
            for(int z=0;z<4;z++)
            {    

                _blurSize=_AMOUNT_IN->buffer()[0] * ((float)*(_amountMapData++)/255.0f);

                
                _blurSize/=2;
                if (_blurSize<2)
                    _blurSize=2;


                _x1 = x - _blurSize-1;
                _x2 = x + _blurSize;
                _y1 = y - _blurSize-1;
                _y2 = y + _blurSize;

                //*** attention : integral image (-1,-1) , (-1,y), (x,-1) doit valoir zero
                if(_x1 < 0)_x1 = 0;
                if(_x2 < 0)_x2 = 0;
                if(_y1 < 0)_y1 = 0;
                if(_y2 < 0)_y2 = 0;

                if(_x1 >= _sizeX)_x1 = _sizeX-1;
                if(_x2 >= _sizeX)_x2 = _sizeX-1;
                if(_y1 >= _sizeY)_y1 = _sizeY-1;
                if(_y2 >= _sizeY)_y2 = _sizeY-1;

                /*
                std::cout << _tempAccumbuf[(y * _sizeX * 4) + (x*4) + z] << " - " <<
             _tempAccumbuf[(y-1) * _sizeX * 4 + (x*4) + z] << " - " <<
             _tempAccumbuf[(y * _sizeX * 4) + (x-1)*4 + z] << " + "<<
             _tempAccumbuf[(y-1) * _sizeX * 4 + (x-1)*4 + z] << " = " << (_tempAccumbuf[(y * _sizeX * 4) + (x*4) + z] - 
                                 _tempAccumbuf[(y-1) * _sizeX * 4 + (x*4) + z] -
                                 _tempAccumbuf[(y * _sizeX * 4) + (x-1)*4 + z] +
                                 _tempAccumbuf[(y-1) * _sizeX * 4 + (x-1)*4 + z]) << std::endl;

                 */  


                *(_outData++) = (_tempAccumbuf[(_y2 * _sizeX * 4) + (_x2*4) + z] - 
                                 _tempAccumbuf[(_y1) * _sizeX * 4 + (_x2*4) + z] -
                                 _tempAccumbuf[(_y2 * _sizeX * 4) + (_x1)*4 + z] +
                                 _tempAccumbuf[(_y1) * _sizeX * 4 + (_x1)*4 + z]) / ((_x2-_x1)*(_y2-_y1));
                                  
                /// ((_x2-_x1) * (_y2-_y1));
                                
                /*
                *(_outData++) = (_tempAccumbuf[(_y2 * _sizeX * 4) + (_x1*4)] -
                               _tempAccumbuf[_y1 * _sizeX * 4 + _x2*4] -
                               _tempAccumbuf[_y2 * _sizeX * 4 + _x1*4] +
                               _tempAccumbuf[_y1 * _sizeX * 4 + _x1*4]) / ((_x2-_x1) * (_y2-_y1));
                */

            }

        }
                                         
    }                                    
    
       
}
