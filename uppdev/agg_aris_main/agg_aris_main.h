#ifndef _agg_main_agg_main_h_
#define _agg_main_agg_main_h_
//this file was added by aris002@yahoo.co.uk for upp package
//I also changed the stucture of agg directories and includes (to have less headaches :)) ...


#include <agg_aris_main/pixel_formats.h> //aris: I moved this file here. might be separate?...
#include <agg_aris_main/agg_config.h>  //check for type defines...
#include <agg_aris_main/agg_basics.h>
#include <agg_aris_main/agg_array.h>

#include <agg_aris_main/agg_rendering_buffer.h>
#include <agg_aris_main/agg_rendering_buffer_dynarow.h>


#include <agg_aris_main/agg_alpha_mask_u8.h>
#include <agg_aris_main/agg_arc.h>
#include <agg_aris_main/agg_arrowhead.h>
#include <agg_aris_main/agg_bezier_arc.h>
#include <agg_aris_main/agg_bitset_iterator.h>
#include <agg_aris_main/agg_bounding_rect.h>
#include <agg_aris_main/agg_bspline.h>
#include <agg_aris_main/agg_blur.h> //new file 19 July 2006

#include <agg_aris_main/agg_clip_liang_barsky.h>
#include <agg_aris_main/agg_color_gray.h>
#include <agg_aris_main/agg_color_rgba.h>
#include <agg_aris_main/agg_conv_adaptor_vcgen.h>
#include <agg_aris_main/agg_conv_adaptor_vpgen.h>
#include <agg_aris_main/agg_conv_bspline.h>
#include <agg_aris_main/agg_conv_clip_polygon.h>
#include <agg_aris_main/agg_conv_clip_polyline.h>
#include <agg_aris_main/agg_conv_close_polygon.h>
#include <agg_aris_main/agg_conv_concat.h>
#include <agg_aris_main/agg_conv_contour.h>
#include <agg_aris_main/agg_conv_curve.h>
#include <agg_aris_main/agg_conv_dash.h>
#include <agg_aris_main/agg_conv_gpc.h>  //gpc- contact the author for comercial use 
#include <agg_aris_main/agg_conv_marker.h>
#include <agg_aris_main/agg_conv_marker_adaptor.h>
#include <agg_aris_main/agg_conv_segmentator.h>
#include <agg_aris_main/agg_conv_shorten_path.h>
#include <agg_aris_main/agg_conv_smooth_poly1.h>
#include <agg_aris_main/agg_conv_stroke.h>
#include <agg_aris_main/agg_conv_transform.h>
#include <agg_aris_main/agg_conv_unclose_polygon.h>
#include <agg_aris_main/agg_curves.h>
#include <agg_aris_main/agg_dda_line.h>
#include <agg_aris_main/agg_ellipse.h>
#include <agg_aris_main/agg_ellipse_bresenham.h>
#include <agg_aris_main/agg_embedded_raster_fonts.h>
#include <agg_aris_main/agg_font_cache_manager.h>
#include <agg_aris_main/agg_gamma_functions.h>
#include <agg_aris_main/agg_gamma_lut.h>
#include <agg_aris_main/agg_glyph_raster_bin.h>
#include <agg_aris_main/agg_gsv_text.h>
#include <agg_aris_main/agg_image_accessors.h>
#include <agg_aris_main/agg_image_filters.h>
#include <agg_aris_main/agg_line_aa_basics.h>
#include <agg_aris_main/agg_math.h>
#include <agg_aris_main/agg_math_stroke.h>
#include <agg_aris_main/agg_path_length.h>
#include <agg_aris_main/agg_path_storage.h>
#include <agg_aris_main/agg_path_storage_integer.h>
#include <agg_aris_main/agg_pattern_filters_rgba.h>
#include <agg_aris_main/agg_pixfmt_amask_adaptor.h>
#include <agg_aris_main/agg_pixfmt_gray.h>
#include <agg_aris_main/agg_pixfmt_rgb.h>
#include <agg_aris_main/agg_pixfmt_rgba.h>
#include <agg_aris_main/agg_pixfmt_rgb_packed.h>
#include <agg_aris_main/agg_pixfmt_transposer.h> //new 19 July 2006
#include <agg_aris_main/agg_rasterizer_cells_aa.h>
#include <agg_aris_main/agg_rasterizer_compound_aa.h>
#include <agg_aris_main/agg_rasterizer_outline.h>
#include <agg_aris_main/agg_rasterizer_outline_aa.h>
#include <agg_aris_main/agg_rasterizer_scanline_aa.h>
#include <agg_aris_main/agg_rasterizer_sl_clip.h>
#include <agg_aris_main/agg_renderer_base.h>
#include <agg_aris_main/agg_renderer_markers.h>
#include <agg_aris_main/agg_renderer_mclip.h>
#include <agg_aris_main/agg_renderer_outline_aa.h>
#include <agg_aris_main/agg_renderer_outline_image.h>
#include <agg_aris_main/agg_renderer_primitives.h>
#include <agg_aris_main/agg_renderer_raster_text.h>
#include <agg_aris_main/agg_renderer_scanline.h>
#include <agg_aris_main/agg_rounded_rect.h>
#include <agg_aris_main/agg_scanline_bin.h>
#include <agg_aris_main/agg_scanline_boolean_algebra.h>
#include <agg_aris_main/agg_scanline_p.h>
#include <agg_aris_main/agg_scanline_storage_aa.h>
#include <agg_aris_main/agg_scanline_storage_bin.h>
#include <agg_aris_main/agg_scanline_u.h>
#include <agg_aris_main/agg_shorten_path.h>
#include <agg_aris_main/agg_simul_eq.h>
#include <agg_aris_main/agg_span_allocator.h>
#include <agg_aris_main/agg_span_converter.h>
#include <agg_aris_main/agg_span_gouraud.h>
#include <agg_aris_main/agg_span_gouraud_gray.h>
#include <agg_aris_main/agg_span_gouraud_rgba.h>
#include <agg_aris_main/agg_span_gradient.h>
#include <agg_aris_main/agg_span_gradient_alpha.h>
#include <agg_aris_main/agg_span_image_filter.h>
#include <agg_aris_main/agg_span_image_filter_gray.h>
#include <agg_aris_main/agg_span_image_filter_rgb.h>
#include <agg_aris_main/agg_span_image_filter_rgba.h>
#include <agg_aris_main/agg_span_interpolator_adaptor.h>
#include <agg_aris_main/agg_span_interpolator_linear.h>
#include <agg_aris_main/agg_span_interpolator_persp.h>
#include <agg_aris_main/agg_span_interpolator_trans.h>
#include <agg_aris_main/agg_span_pattern_gray.h>
#include <agg_aris_main/agg_span_pattern_rgb.h>
#include <agg_aris_main/agg_span_pattern_rgba.h>
#include <agg_aris_main/agg_span_solid.h>
#include <agg_aris_main/agg_span_subdiv_adaptor.h>
#include <agg_aris_main/agg_trans_affine.h>
#include <agg_aris_main/agg_trans_bilinear.h>
#include <agg_aris_main/agg_trans_double_path.h>
//#include <agg_aris_main/agg_trans_lens.h>  //replaced by trans_warp_magnifier???
#include <agg_aris_main/agg_trans_perspective.h>
#include <agg_aris_main/agg_trans_single_path.h>
#include <agg_aris_main/agg_trans_viewport.h>
#include <agg_aris_main/agg_trans_warp_magnifier.h> //substitution for trans lens.h???
#include <agg_aris_main/agg_vcgen_bspline.h>
#include <agg_aris_main/agg_vcgen_contour.h>
#include <agg_aris_main/agg_vcgen_dash.h>
#include <agg_aris_main/agg_vcgen_markers_term.h>
#include <agg_aris_main/agg_vcgen_smooth_poly1.h>
#include <agg_aris_main/agg_vcgen_stroke.h>
#include <agg_aris_main/agg_vcgen_vertex_sequence.h>
#include <agg_aris_main/agg_vertex_sequence.h>
#include <agg_aris_main/agg_vpgen_clip_polygon.h>
#include <agg_aris_main/agg_vpgen_clip_polyline.h>
#include <agg_aris_main/agg_vpgen_segmentator.h>

#include <agg_aris_main/agg_svg_exception.h>
#include <agg_aris_main/agg_svg_path_renderer.h>
#include <agg_aris_main/agg_svg_path_tokenizer.h>
//#include <agg_aris_main/agg_svg_parser.h>



#endif
