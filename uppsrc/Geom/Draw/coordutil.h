#ifndef _Geom_Draw_coordutil_h_
#define _Geom_Draw_coordutil_h_

namespace Upp {

void AlphaTransformPaint(ImageBuffer& out, const Image& in,
	const PlanarSegmentTree& planar,
	const LinearSegmentTree& left, const LinearSegmentTree& top,
	const LinearSegmentTree& right, const LinearSegmentTree& bottom,
	const SegmentTreeInfo& info, Color background);

void PaintPlanarTree(Draw& draw, const PlanarSegmentTree& planar,
	const LinearSegmentTree& left, const LinearSegmentTree& top,
	const LinearSegmentTree& right, const LinearSegmentTree& bottom);

void   PlotLine(PlotterTool& tool, GisTransform transform, Pointf source1, Pointf source2);
void   PlotRect(PlotterTool& tool, GisTransform transform, const Rectf& src);

}

#endif
