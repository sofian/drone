#ifndef PALETTERGBATYPE_INCLUDED
#define PALETTERGBATYPE_INCLUDED

#include "ColorSpace.h"
#include "VectorType.h"
#include "Engine.h"

class PaletteRGBAType : public VectorType<RGBA>
{
public:
  // XXX normalement il devrait peut-être pas y avoir deux fois la même couleur...
  // XXX fonction de mapping ?
  PaletteRGBAType(int size = 0, RGBA fillValue = BLACK_RGBA)
    : VectorType<RGBA>(size, fillValue)
  {}

  virtual ~PaletteRGBAType(){}

  std::string name() const { return "PaletteRGBAType";}
  QColor color() const { return QColor(50,124,50);}
};

#endif 
