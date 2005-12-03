#include "Composite.h"

// /**
//  * gimp_composite_dispatch:
//  * @ctx: The compositing context
//  *
//  * Given a compositing context, perform the compositing function
//  * dictated by the compositing context operation.  There is no return
//  * value, all results are in the compositing context.
//  **/
// void
// gimp_composite_dispatch (GimpCompositeContext *ctx)
// {
//   void (*function) (GimpCompositeContext *);

//   function = gimp_composite_function[ctx->op][ctx->pixelformat_A][ctx->pixelformat_B][ctx->pixelformat_D];

//   if (function)
//     {
//       if (gimp_composite_options.bits & GIMP_COMPOSITE_OPTION_VERBOSE)
//         {
//           printf ("%s %s %s %s = %p\n",
//                   gimp_composite_mode_astext (ctx->op),
//                   gimp_composite_pixelformat_astext (ctx->pixelformat_A),
//                   gimp_composite_pixelformat_astext (ctx->pixelformat_B),
//                   gimp_composite_pixelformat_astext (ctx->pixelformat_D),
//                   function);
//         }

//       (*function) (ctx);
//     }
//   else
//     {
//       printf ("gimp_composite: unsupported operation: %s %s %s %s\n",
//               gimp_composite_mode_astext (ctx->op),
//               gimp_composite_pixelformat_astext (ctx->pixelformat_A),
//               gimp_composite_pixelformat_astext (ctx->pixelformat_B),
//               gimp_composite_pixelformat_astext (ctx->pixelformat_D));
//     }
// }

// /**
//  * gimp_composite_context_print:
//  * @ctx: The context to print
//  *
//  * Print a human readable form of a GimpCompositeContext on stdout.
//  **/
// void
// gimp_composite_context_print (GimpCompositeContext *ctx)
// {
//   printf ("%p: op=%s\n  A=%s(%d):%p\n  B=%s(%d):%p\n  D=%s(%d):%p\n  M=%s(%d):%p\n  n_pixels=%lu\n",
//           ctx,
//           gimp_composite_mode_astext (ctx->op),
//           gimp_composite_pixelformat_astext (ctx->pixelformat_A),
//           ctx->pixelformat_A, ctx->A,
//           gimp_composite_pixelformat_astext (ctx->pixelformat_B),
//           ctx->pixelformat_B, ctx->B,
//           gimp_composite_pixelformat_astext (ctx->pixelformat_D),
//           ctx->pixelformat_D, ctx->D,
//           gimp_composite_pixelformat_astext (ctx->pixelformat_M),
//           ctx->pixelformat_M, ctx->M,
//           ctx->n_pixels);
// }

// /**
//  * gimp_composite_mode_astext:
//  * @op:
//  *
//  * Given a GimpCompositeOperatin, return a string representation of the name
//  * of that operation.
//  *
//  * Returns: gchar *, the name
//  **/
// const gchar *
// gimp_composite_mode_astext (GimpCompositeOperation op)
// {
//   switch (op)
//     {
//     case GIMP_COMPOSITE_NORMAL:        return ("GIMP_COMPOSITE_NORMAL");
//     case GIMP_COMPOSITE_DISSOLVE:      return ("GIMP_COMPOSITE_DISSOLVE");
//     case GIMP_COMPOSITE_BEHIND:        return ("GIMP_COMPOSITE_BEHIND");
//     case GIMP_COMPOSITE_MULTIPLY:      return ("GIMP_COMPOSITE_MULTIPLY");
//     case GIMP_COMPOSITE_SCREEN:        return ("GIMP_COMPOSITE_SCREEN");
//     case GIMP_COMPOSITE_OVERLAY:       return ("GIMP_COMPOSITE_OVERLAY");
//     case GIMP_COMPOSITE_DIFFERENCE:    return ("GIMP_COMPOSITE_DIFFERENCE");
//     case GIMP_COMPOSITE_ADDITION:      return ("GIMP_COMPOSITE_ADDITION");
//     case GIMP_COMPOSITE_SUBTRACT:      return ("GIMP_COMPOSITE_SUBTRACT");
//     case GIMP_COMPOSITE_DARKEN:        return ("GIMP_COMPOSITE_DARKEN");
//     case GIMP_COMPOSITE_LIGHTEN:       return ("GIMP_COMPOSITE_LIGHTEN");
//     case GIMP_COMPOSITE_HUE:           return ("GIMP_COMPOSITE_HUE");
//     case GIMP_COMPOSITE_SATURATION:    return ("GIMP_COMPOSITE_SATURATION");
//     case GIMP_COMPOSITE_COLOR_ONLY:    return ("GIMP_COMPOSITE_COLOR_ONLY");
//     case GIMP_COMPOSITE_VALUE:         return ("GIMP_COMPOSITE_VALUE");
//     case GIMP_COMPOSITE_DIVIDE:        return ("GIMP_COMPOSITE_DIVIDE");
//     case GIMP_COMPOSITE_DODGE:         return ("GIMP_COMPOSITE_DODGE");
//     case GIMP_COMPOSITE_BURN:          return ("GIMP_COMPOSITE_BURN");
//     case GIMP_COMPOSITE_HARDLIGHT:     return ("GIMP_COMPOSITE_HARDLIGHT");
//     case GIMP_COMPOSITE_SOFTLIGHT:     return ("GIMP_COMPOSITE_SOFTLIGHT");
//     case GIMP_COMPOSITE_GRAIN_EXTRACT: return ("GIMP_COMPOSITE_GRAIN_EXTRACT");
//     case GIMP_COMPOSITE_GRAIN_MERGE:   return ("GIMP_COMPOSITE_GRAIN_MERGE");
//     case GIMP_COMPOSITE_COLOR_ERASE:   return ("GIMP_COMPOSITE_COLOR_ERASE");
//     case GIMP_COMPOSITE_ERASE:         return ("GIMP_COMPOSITE_ERASE");
//     case GIMP_COMPOSITE_REPLACE:       return ("GIMP_COMPOSITE_REPLACE");
//     case GIMP_COMPOSITE_ANTI_ERASE:    return ("GIMP_COMPOSITE_ANTI_ERASE");
//     case GIMP_COMPOSITE_BLEND:         return ("GIMP_COMPOSITE_BLEND");
//     case GIMP_COMPOSITE_SHADE:         return ("GIMP_COMPOSITE_SHADE");
//     case GIMP_COMPOSITE_SWAP:          return ("GIMP_COMPOSITE_SWAP");
//     case GIMP_COMPOSITE_SCALE:         return ("GIMP_COMPOSITE_SCALE");
//     case GIMP_COMPOSITE_CONVERT:       return ("GIMP_COMPOSITE_CONVERT");
//     case GIMP_COMPOSITE_XOR:           return ("GIMP_COMPOSITE_XOR");
//     default:
//       break;
//     }

//   return ("bad mode");
// }

// /**
//  * gimp_composite_pixelformat_astext:
//  * @format:  The format.
//  *
//  * Given a GimpPixelFormat, return a string representation of the name
//  * of that format.
//  *
//  * Returns: gchar *, the name
//  **/
// const gchar *
// gimp_composite_pixelformat_astext (GimpPixelFormat format)
// {
//   switch (format) {
//   case GIMP_PIXELFORMAT_V8:     return ("V8");
//   case GIMP_PIXELFORMAT_VA8:    return ("VA8");
//   case GIMP_PIXELFORMAT_RGB8:   return ("RGB8");
//   case GIMP_PIXELFORMAT_RGBA8:  return ("RGBA8");
// #if GIMP_COMPOSITE_16BIT
//   case GIMP_PIXELFORMAT_V16:    return ("V16");
//   case GIMP_PIXELFORMAT_VA16:   return ("VA16");
//   case GIMP_PIXELFORMAT_RGB16:  return ("RGB16");
//   case GIMP_PIXELFORMAT_RGBA16: return ("RGBA16");
// #endif
// #if GIMP_COMPOSITE_32BIT
//   case GIMP_PIXELFORMAT_V32:    return ("V32");
//   case GIMP_PIXELFORMAT_VA32:   return ("VA32");
//   case GIMP_PIXELFORMAT_RGB32:  return ("RGB32");
//   case GIMP_PIXELFORMAT_RGBA32: return ("RGBA32");
// #endif
//   case GIMP_PIXELFORMAT_ANY:    return ("ANY");
//   default:
//     break;
//   }

//   return ("bad format");
// }


/**
 * gimp_composite_init:
 * @be_verbose:    whether to be verbose on stdout
 * @use_cpu_accel: whether to use accelerated routines like MMX
 *
 * Initialise the Gimp Compositing subsystem.  This includes checking
 * for user options and environment, installing the generic set of
 * compositing operation handlers, followed by overloading those which
 * are supported by the current cpu/hardware.
 **/

//void
//gimp_composite_init (gboolean  be_verbose,
//                     gboolean  use_cpu_accel)
//{
//   const gchar *p;

//   if ((p = g_getenv ("GIMP_COMPOSITE")))
//     {
//       gimp_composite_options.bits = strtoul(p, NULL, 16);
//     }

//   if (!use_cpu_accel)
//     gimp_composite_options.bits |=  GIMP_COMPOSITE_OPTION_NOEXTENSIONS;

//   if (be_verbose)
//     g_printerr ("gimp_composite: use=%s, verbose=%s\n",
//                 (gimp_composite_options.bits & GIMP_COMPOSITE_OPTION_USE)     ?
//                 "yes" : "no",
//                 (gimp_composite_options.bits & GIMP_COMPOSITE_OPTION_VERBOSE) ?
//                 "yes" : "no");
//   extern gboolean gimp_composite_generic_install (void);
//   gimp_composite_generic_install ();

  /*
   * Here is where you "glue" in the initialisation of your
   * optimisations.
   *
   * Declare the install() function external, and then call it.  A
   * return value of TRUE from the install function means the
   * installer was successful in instantiating itself.  For example,
   * it succeeded in hooking in the functions with the special
   * optimisation instructions, or hardware, or whatever.
   */
//   if (! (gimp_composite_options.bits & GIMP_COMPOSITE_OPTION_NOEXTENSIONS))
//     {
//       extern gboolean gimp_composite_mmx_install (void);
//       extern gboolean gimp_composite_sse_install (void);
//       extern gboolean gimp_composite_sse2_install (void);
//       extern gboolean gimp_composite_3dnow_install (void);
//       extern gboolean gimp_composite_altivec_install (void);
//       extern gboolean gimp_composite_vis_install (void);

//       gboolean can_use_mmx     = gimp_composite_mmx_install ();
//       gboolean can_use_sse     = gimp_composite_sse_install ();
//       gboolean can_use_sse2    = gimp_composite_sse2_install ();
//       gboolean can_use_3dnow   = gimp_composite_3dnow_install ();
//       gboolean can_use_altivec = gimp_composite_altivec_install ();
//       gboolean can_use_vis     = gimp_composite_vis_install ();

//       if (be_verbose)
//         printf ("Processor instruction sets: "
//                     "%cmmx %csse %csse2 %c3dnow %caltivec %cvis\n",
//                     can_use_mmx     ? '+' : '-',
//                     can_use_sse     ? '+' : '-',
//                     can_use_sse2    ? '+' : '-',
//                     can_use_3dnow   ? '+' : '-',
//                     can_use_altivec ? '+' : '-',
//                     can_use_vis     ? '+' : '-');
//     }
//}
