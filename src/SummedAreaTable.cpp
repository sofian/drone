#include "SummedAreaTable.h"


const RGBAint SummedAreaTable::ZERO = { 0, 0, 0, 0 };

SummedAreaTable::SummedAreaTable()
  : _acc(0), _accSquares(0), _canvas(0), _canvasData(0), _sizeX(0), _sizeY(0), _size(0)
{
}

SummedAreaTable::~SummedAreaTable()
{
  free(_acc);
  free(_accSquares);
}

void SummedAreaTable::setCanvas(Canvas *canvas)
{
  _canvas = canvas;
  if (_canvas)
  {
    _canvasData = _canvas->_data;
    _sizeX = _canvas->sizeX();
    _sizeY = _canvas->sizeY();
    _size = _sizeX * _sizeY;
  }
}

void SummedAreaTable::buildTable()
{
  if (!_canvas)
    return;

  // check if we need to resize
  if (_canvas->sizeX() * _canvas->sizeY() != _size) // there was a change
  {
    // resize
    _sizeX = _canvas->sizeX();
    _sizeY = _canvas->sizeY();
    _size = _sizeX * _sizeY;
  }

  // reallocate
  _acc = (RGBAint*)realloc(_acc, _size*sizeof(RGBAint));

  _canvasData = _canvas->_data;
  
  register unsigned char *iterData = (unsigned char*) _canvasData;
  register int *iterAcc = (int*) _acc;

  int rowWidth = _sizeX * 4;
  
  // upper row
  _accR = _accG = _accB = _accA = 0;
  for (int x=0; x<_sizeX; ++x)
  { 
    _accR += (int)*(iterData++);
    _accG += (int)*(iterData++);
    _accB += (int)*(iterData++);
    _accA += (int)*(iterData++);
    *(iterAcc++) = _accR;
    *(iterAcc++) = _accG;
    *(iterAcc++) = _accB;
    *(iterAcc++) = _accA;
  }

  // other rows
  for (int y=1; y<_sizeY; ++y)
  {
    // copy upper line
    memcpy(iterAcc, iterAcc - rowWidth, rowWidth * sizeof(int));
    _accR = _accG = _accB = _accA = 0;
    for (int x=0; x<_sizeX; ++x)
    {
      _accR += (int)*(iterData++);
      _accG += (int)*(iterData++);
      _accB += (int)*(iterData++);
      _accA += (int)*(iterData++);
      *(iterAcc++) += _accR;
      *(iterAcc++) += _accG;
      *(iterAcc++) += _accB;
      *(iterAcc++) += _accA;
    }
  }
}

void SummedAreaTable::buildTableOfSquares()
{
  if (!_canvas)
    return;

  // check if we need to resize
  if (_canvas->sizeX() * _canvas->sizeY() != _size) // there was a change
  {
    // resize
    _sizeX = _canvas->sizeX();
    _sizeY = _canvas->sizeY();
    _size = _sizeX * _sizeY;
  }

  // reallocate
  _accSquares = (RGBAint*)realloc(_accSquares, _size*sizeof(RGBAint));

  _canvasData = _canvas->_data;
  
  register unsigned char *iterData = (unsigned char*) _canvasData;
  register int *iterAcc = (int*) _acc;

  int rowWidth = _sizeX * 4;
  
  // upper row
  _accR = _accG = _accB = _accA = 0;
  for (int x=0; x<_sizeX; ++x)
  {
    _accR += SQR( (int)*(iterData++) );
    _accG += SQR( (int)*(iterData++) );
    _accB += SQR( (int)*(iterData++) );
    _accA += SQR( (int)*(iterData++) );
    *(iterAcc++) = _accR;
    *(iterAcc++) = _accG;
    *(iterAcc++) = _accB;
    *(iterAcc++) = _accA;
  }

  // other rows
  for (int y=1; y<_sizeY; ++y)
  {
    // copy upper line
    memcpy(iterAcc, iterAcc - rowWidth, rowWidth * sizeof(int));
    _accR = _accG = _accB = _accA = 0;
    for (int x=0; x<_sizeX; ++x)
    {
      _accR += SQR( (int)*(iterData++) );
      _accG += SQR( (int)*(iterData++) );
      _accB += SQR( (int)*(iterData++) );
      _accA += SQR( (int)*(iterData++) );
      *(iterAcc++) += _accR;
      *(iterAcc++) += _accG;
      *(iterAcc++) += _accB;
      *(iterAcc++) += _accA;
    }
  }
}

