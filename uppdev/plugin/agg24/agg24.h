#ifndef _ag24upp__agg24upp_h_
#define _ag24upp__agg24upp_h_
//this file is for all-in-one ag24 upp package.
// added by aris002@yahoo.co.uk
//I also changed the stucture of agg directories and includes (to have less headaches :)) ...

#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>  //only for svg

#include "agg_config.h"  //check for type defines...

//namespace agg
//{

#include  "agg_basics.h"
#include  "agg_array.h"
//=========AGG_MATH == AND "Semi-independant"==========
#include  "agg_math.h"
#include  "agg_simul_eq.h"
#include  "agg_bounding_rect.h"
#include  "agg_clip_liang_barsky.h"
#include  "agg_bitset_iterator.h"
#include  "agg_gamma_functions.h"
#include  "agg_gamma_lut.h"
#include  "agg_vertex_sequence.h"
#include  "agg_math_stroke.h"
#include  "agg_shorten_path.h"
#include  "agg_conv_adaptor_vcgen.h"
//======================Trans=============================
#include "agg_trans_affine.h"
#include "agg_trans_perspective.h"  //added *.cpp 

#include  "agg_trans_bilinear.h"
#include  "agg_trans_double_path.h"
#include  "agg_trans_single_path.h"
#include  "agg_trans_viewport.h"
#include  "agg_trans_warp_magnifier.h" //substitution for trans lens.h
//#include  "agg_trans_lens.h"  //replaced by trans_warp_magnifier
#include  "agg_conv_transform.h"
//======================Shapes=============================
#include  "agg_dda_line.h"
#include  "agg_line_aa_basics.h"
#include  "agg_ellipse_bresenham.h"
#include  "agg_ellipse.h"
#include  "agg_bspline.h"
#include  "agg_curves.h"
#include  "agg_arc.h"
#include  "agg_bezier_arc.h"
#include  "agg_rounded_rect.h"
#include  "agg_arrowhead.h"
//======================Image-1=============================
#include  "agg_image_filters.h"
#include  "agg_image_accessors.h"

//======================Strokers=============================
#include  "agg_vcgen_stroke.h"
#include  "agg_conv_stroke.h"


//========Color Bases ================
#include  "agg_color_rgba.h"
#include  "agg_color_gray.h"
//========Color-2 ================
#include  "agg_pattern_filters_rgba.h"

//======================Renderers=============================
#include  "agg_rendering_buffer.h"
#include  "agg_rendering_buffer_dynarow.h"
#include  "agg_renderer_base.h"
#include  "agg_renderer_scanline.h"

#include  "agg_renderer_primitives.h"
#include  "agg_renderer_markers.h"
#include  "agg_renderer_mclip.h"
#include  "agg_renderer_outline_aa.h"
#include  "agg_renderer_outline_image.h"
#include  "agg_renderer_raster_text.h"

//======================line-profile=============================
//agg_line_profile.cpp
//======================pixfmt=============================
#include  "pixel_formats.h" //aris002: I placed this file here, might be separate?...
//======================pixfmt-format-"hidden"=============================
//#include  "agg_pixfmt_rgba.h"
//#include  "agg_pixfmt_gray.h"
//#include  "agg_pixfmt_rgb.h"
//#include  "agg_pixfmt_rgb_packed.h"
//======================pixfmt-2============================
#include  "agg_pixfmt_amask_adaptor.h"
#include  "agg_pixfmt_transposer.h" //new 19 July 2006
#include  "agg_blur.h" //new file 19 July 2006
#include  "agg_alpha_mask_u8.h"

//=====================Scanlines==============================
#include  "agg_scanline_bin.h"
#include  "agg_scanline_storage_aa.h"
#include  "agg_scanline_storage_bin.h"
#include  "agg_scanline_u.h"
#include  "agg_scanline_p.h"
#include  "agg_scanline_boolean_algebra.h"

//=====================Rasterizers==============================
#include  "agg_rasterizer_sl_clip.h"
#include  "agg_rasterizer_cells_aa.h"
#include  "agg_rasterizer_scanline_aa.h"
#include  "agg_rasterizer_compound_aa.h"
#include  "agg_rasterizer_outline.h"
#include  "agg_rasterizer_outline_aa.h"

//=======================Path============================
#include  "agg_path_storage.h"
#include  "agg_path_length.h"
#include  "agg_path_storage_integer.h"
//=======================Vcgen============================
#include  "agg_vcgen_bspline.h"
#include  "agg_vcgen_contour.h"
#include  "agg_vcgen_dash.h"
#include  "agg_vcgen_markers_term.h"
#include  "agg_vcgen_smooth_poly1.h"
#include  "agg_vcgen_vertex_sequence.h"
//#include  "agg_vcgen_stroke.h"  //moved to "Strokers" section
//=======================Vpgen============================
#include  "agg_vpgen_clip_polygon.h"
#include  "agg_vpgen_clip_polyline.h"
#include  "agg_vpgen_segmentator.h"
//=======================Converters============================
//#include  "agg_conv_adaptor_vcgen.h"  //moved to "math" section
#include  "agg_conv_adaptor_vpgen.h"
#include  "agg_conv_bspline.h"
#include  "agg_conv_clip_polygon.h"
#include  "agg_conv_clip_polyline.h"
#include  "agg_conv_close_polygon.h"
#include  "agg_conv_concat.h"
#include  "agg_conv_contour.h"
#include  "agg_conv_curve.h"
#include  "agg_conv_dash.h"
//#include  <plugin/agg24/gpc/agg_conv_gpc.h>  //gpc- contact the author for comercial use!!!
#include  "agg_conv_gpc.h"  //gpc- contact the author for comercial use!!!
#include  "agg_conv_marker.h"
#include  "agg_conv_marker_adaptor.h"
#include  "agg_conv_segmentator.h"
#include  "agg_conv_shorten_path.h"
#include  "agg_conv_smooth_poly1.h"
//#include  "agg_conv_stroke.h"  //moved to "Strokers" section
//#include  "agg_conv_transform.h"  //moved to "Trans" section
#include  "agg_conv_unclose_polygon.h"

//=====================Spans=================================
#include  "agg_span_allocator.h"
#include  "agg_span_converter.h"
#include  "agg_span_solid.h"
#include  "agg_span_gouraud.h"
#include  "agg_span_gouraud_gray.h"
#include  "agg_span_gouraud_rgba.h"
#include  "agg_span_gradient.h"
#include  "agg_span_gradient_alpha.h"
#include  "agg_span_interpolator_adaptor.h"
#include  "agg_span_interpolator_linear.h"
#include  "agg_span_interpolator_persp.h"
#include  "agg_span_interpolator_trans.h"
#include  "agg_span_pattern_gray.h"
#include  "agg_span_pattern_rgb.h"
#include  "agg_span_pattern_rgba.h"
#include  "agg_span_subdiv_adaptor.h"
#include  "agg_span_image_filter.h"
#include  "agg_span_image_filter_gray.h"
#include  "agg_span_image_filter_rgb.h"
#include  "agg_span_image_filter_rgba.h"
//=====================Fonts=================================
#include  "agg_embedded_raster_fonts.h"
#include  "agg_font_cache_manager.h"
#include  "agg_glyph_raster_bin.h"
#include  "agg_gsv_text.h"
//=====================SVG===================================
#include  "agg_svg_exception.h"
#include  "agg_svg_path_tokenizer.h"
#include  "agg_svg_path_renderer.h"

//#include  "agg_svg_parser.h"

//}

#endif
