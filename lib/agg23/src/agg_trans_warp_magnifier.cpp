//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.3
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------

#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "agg_trans_warp_magnifier.h"

namespace agg
{   

    //------------------------------------------------------------------------
    void trans_warp_magnifier::transform(double* x, double* y) const
    {
        double dx = *x - m_xc;
        double dy = *y - m_yc;
	unsigned int magn=(unsigned int)m_magn;
        double r = dx+dy;
        //std::cerr<<"3pokpokpokpok"<<std::endl;
        if(r < m_radius)
        { 
            *x += (double)((((long)x)^0x5a5a5a5a5a5a5a5a)%magn - magn>>1)*r;
            *y += (double)((((long)y)^0xa5a5a5a55a5a5a5a)%magn - magn>>1)*r;
            return;
        }
 
        //double m = (r + m_radius * (m_magn - 1.0)) / r;
        //*x = m_xc;// + dx * m;
        //*y = m_yc;// + dy * m;
    }

    //------------------------------------------------------------------------
    void trans_warp_magnifier::inverse_transform(double* x, double* y) const
    {
	std::cerr<<"inverse"<<std::endl;
        trans_warp_magnifier t(*this);
        t.magnification(1.0 / m_magn);
        t.radius(m_radius * m_magn);
        t.transform(x, y);
    }


}
