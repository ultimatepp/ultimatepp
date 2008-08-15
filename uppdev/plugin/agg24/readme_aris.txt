this is for all-in-one ag24 upp package by aris002@yahoo.co.uk
I also changed the stucture of agg directories and includes (to have less headaches :)) ...

!!! You don't need to use makefiles with upp!!!

Install like normal upp package
Any questions - http://www.ultimatepp.org/forum/

15/07/2006 Added upp Vector support to agg::path_storage
2007 - need to check and test

Important from antigrain web:
  It can seem it's always better to use the packed version, but in practice it's not so. 
The scanline_p works faster when rendering large solid polygons, 
that is when the area of the polygon is much larger than the perimeter 
in the meaning of the number of pixels. 
  But when rendering text it's definitely better to use the scanline_u 
because of much less number of processed spans.
The difference is about three times
and the number of spans is also important for the overall performance.
  Besides, in most of the span generators, such as gradients, 
Gouraud shader, or image transformers,
the number of spans becomes even more critical, and so,
the scanline_p is not used there.

==
Aris: scanline_p8 p-olygons large.
==
my svg xml parsing is an old version and also maybe it should be a separate upp package

== from agg mailing list: ===
The scanline rasterizer doesn't know anything about colors, etc. So that, you
can only render one style at a time. But you can store as many paths in the
path storage as you need and then navigate to them by path_id. I did that in
the svg viewer. You store an array of structures like this:

struct attributes_type
{
agg::rgba8 fill;
agg::rgba8 stroke;
float stroke_width;
. . .
unsigned path_id;
};

Then you collect these entries in a separate container, say, std::vector like
this:

m_path.remove_all();
m_attributes.clear();
for_each_shape
{
attributes_type attr;
attr.fill = . . .
. . .
attr.path_id = m_path.start_new_path();
m_attributes.push_back(attr);
m_path.move_to(. . .);
m_path.line_to(. . .);
. . .
}

Then you can draw it all with a single loop:
for(i = 0; i < m_attributes.size(); i++)
{
if(m_attributes[i].fill_required)
{
m_rasterizer.add_path(m_fill_pipeline, m_attributes[i].path_id);
m_renderer.color(m_attributes[i].fill);
agg::render_scalines(. . .);
}

if(m_attributes[i].stroke_required)
{
m_stroker.width(. . .);
m_rasterizer.add_path(m_stroke_pipeline, m_attributes[i].path_id);
m_renderer.color(m_attributes[i].fill);
}
}

You can see how exactly it's done in the svg viewer. See svg_path_renderer.h,
function render().

McSeem
also from agg web:========================
Include Files

    * Removed Files
          o include/agg_render_scanlines.h
          o include/agg_span_generator.h
          o include/agg_span_image_resample.h
          o include/agg_span_image_resample_gray.h
          o include/agg_span_image_resample_rgb.h
          o include/agg_span_image_resample_rgba.h
          o include/agg_span_pattern.h
          o include/agg_span_pattern_filter_gray.h
          o include/agg_span_pattern_filter_rgb.h
          o include/agg_span_pattern_filter_rgba.h
          o include/agg_span_pattern_resample_gray.h
          o include/agg_span_pattern_resample_rgb.h
          o include/agg_span_pattern_resample_rgba.h

    * New Files
          o include/agg_image_accessors.h
          o include/agg_path_length.h
          o include/agg_rasterizer_cells_aa.h
          o include/agg_rasterizer_compound_aa.h
          o include/agg_rasterizer_sl_clip.h
          o include/agg_span_pattern_gray.h


Source Files

    * Removed Files
          o src/agg_path_storage.cpp
          o src/agg_rasterizer_scanline_aa.cpp

