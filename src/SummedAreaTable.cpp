#include "SummedAreaTable.h"


SummedAreaTable::SummedAreaTable()
: _acc(0), _accSquares(0), _image(0), _imageData(0), _width(0), _height(0), _size(0)
{
}

SummedAreaTable::~SummedAreaTable()
{
  free(_acc);
  free(_accSquares);
}

void SummedAreaTable::setImage(const MatrixType<RGBA> *image)
{
  _image = image;
  if (_image)
  {
    _imageData = _image->data();
    _width = _image->width();
    _height = _image->height();
    _size = _image->size();
  }
}

void SummedAreaTable::buildTable()
{
  if (!_image)
    return;

  // check if we need to resize
  if ((int)(_image->width() * _image->height()) != _size) // there was a change
  {
    // resize
    _width = _image->width();
    _height = _image->height();
    _size = _image->size();
  }

  // reallocate
  _acc = (RGBAint*)realloc(_acc, _size*sizeof(RGBAint));

  _imageData = _image->data();

  register unsigned char *iterData = (unsigned char*) _imageData;
  register int *iterAcc = (int*) _acc;

  int rowWidth = _width * 4;

  // upper row
  _accR = _accG = _accB = _accA = 0;
  for (int x=0; x<_width; ++x)
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
  for (int y=1; y<_height; ++y)
  {
    // copy upper line
    memcpy(iterAcc, iterAcc - rowWidth, rowWidth * sizeof(int));
    _accR = _accG = _accB = _accA = 0;
    for (int x=0; x<_width; ++x)
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
  if (!_image)
    return;

  // check if we need to resize
  if ((int)(_image->width() * _image->height()) != _size) // there was a change
  {
    // resize
    _width = _image->width();
    _height = _image->height();
    _size = _width * _height;
  }

  // reallocate
  _accSquares = (RGBAint*)realloc(_accSquares, _size*sizeof(RGBAint));

  _imageData = _image->data();

  register unsigned char *iterData = (unsigned char*) _imageData;
  register int *iterAcc = (int*) _acc;

  int rowWidth = _width * 4;

  // upper row
  _accR = _accG = _accB = _accA = 0;
  for (int x=0; x<_width; ++x)
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
  for (int y=1; y<_height; ++y)
  {
    // copy upper line
    memcpy(iterAcc, iterAcc - rowWidth, rowWidth * sizeof(int));
    _accR = _accG = _accB = _accA = 0;
    for (int x=0; x<_width; ++x)
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

