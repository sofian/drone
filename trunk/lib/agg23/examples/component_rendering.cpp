#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_ellipse.h"
#include "agg_pixfmt_gray.h"
#include "agg_pixfmt_rgb.h"
#include "ctrl/agg_slider_ctrl.h"
#include "platform/agg_platform_support.h"

enum flip_y_e { flip_y = true };


class the_application : public agg::platform_support
{
    agg::slider_ctrl<agg::rgba8> m_alpha;

public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_alpha(5, 5, 320-5, 10+5, !flip_y)
    {
        m_alpha.label("Alpha=%1.0f");
        m_alpha.range(0, 255);
        m_alpha.value(255);
        add_ctrl(m_alpha);
    }

    virtual void on_draw()
    {
        agg::pixfmt_bgr24        pf(rbuf_window());
        agg::pixfmt_gray8_bgr24r pfr(rbuf_window());
        agg::pixfmt_gray8_bgr24g pfg(rbuf_window());
        agg::pixfmt_gray8_bgr24b pfb(rbuf_window());

        typedef agg::renderer_base<agg::pixfmt_bgr24>        rb_type;
        typedef agg::renderer_base<agg::pixfmt_gray8_bgr24r> rbr_type;
        typedef agg::renderer_base<agg::pixfmt_gray8_bgr24g> rbg_type;
        typedef agg::renderer_base<agg::pixfmt_gray8_bgr24b> rbb_type;

        rb_type      rbase(pf);
        rbr_type     rbr(pfr);
        rbg_type     rbg(pfg);
        rbb_type     rbb(pfb);

        agg::renderer_scanline_aa_solid<rb_type>  r(rbase);
        agg::renderer_scanline_aa_solid<rbr_type> rr(rbr);
        agg::renderer_scanline_aa_solid<rbg_type> rg(rbg);
        agg::renderer_scanline_aa_solid<rbb_type> rb(rbb);
        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_p8 sl;

        rbase.clear(agg::rgba(1,1,1));

        agg::ellipse er(width() / 2 - 0.87*50, height() / 2 - 0.5*50, 100, 100, 100);
        rr.color(agg::gray8(0, unsigned(m_alpha.value())));
        ras.add_path(er);
        agg::render_scanlines(ras, sl, rr);
        
        agg::ellipse eg(width() / 2 + 0.87*50, height() / 2 - 0.5*50, 100, 100, 100);
        rg.color(agg::gray8(0, unsigned(m_alpha.value())));
        ras.add_path(eg);
        agg::render_scanlines(ras, sl, rg);

        agg::ellipse eb(width() / 2, height() / 2 + 50, 100, 100, 100);
        rb.color(agg::gray8(0, unsigned(m_alpha.value())));
        ras.add_path(eb);
        agg::render_scanlines(ras, sl, rb);

        agg::render_ctrl(ras, sl, r, m_alpha);
    }

};



int agg_main(int argc, char* argv[])
{
    the_application app(agg::pix_format_bgr24, flip_y);
    app.caption("AGG Example. Component Rendering");

    if(app.init(320, 320, 0))
    {
        return app.run();
    }
    return 1;
}


