#ifndef DISPLACEGRID_INCLUDED
#define DISPLACEGRID_INCLUDED

#include "ColorSpace.h"
#include "MatrixType.h"

struct XYDisp
{
  float xdisp,ydisp;
};

const XYDisp XYDisp_STILL = {0.0f, 0.0f};

class DisplaceGrid : public MatrixType<XYDisp>
{
public:
  DisplaceGrid(int width = 0,
                int height = 0,
                XYDisp fillValue = XYDisp_STILL)
    : MatrixType<XYDisp>(width, height, fillValue)
  {
  }

  virtual ~DisplaceGrid() {}
  
  std::string name() const { return "DisplaceGrid";}
  QColor color() const { return QColor(240,260,14);}
};

#endif //VIDEOTYPE_INCLUDED
