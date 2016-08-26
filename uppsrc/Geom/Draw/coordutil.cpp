#include "GeomDraw.h"

namespace Upp {

static void ExpandSegmentUp(Vector<Point>& src, Vector<Point>& dest, const LinearSegmentTree::Node *node, Point smin, Point dmin)
{
	if(node) {
		ExpandSegmentUp(src, dest, ~node->below, smin, dmin);
		ExpandSegmentUp(src, dest, ~node->above, node->source, node->target);
	}
	else {
		src.Add(smin);
		dest.Add(dmin);
	}
}

static void ExpandSegmentDown(Vector<Point>& src, Vector<Point>& dest, const LinearSegmentTree::Node *node, Point smax, Point dmax)
{
	if(node) {
		ExpandSegmentDown(src, dest, ~node->above, smax, dmax);
		ExpandSegmentDown(src, dest, ~node->below, node->source, node->target);
	}
	else {
		src.Add(smax);
		dest.Add(dmax);
	}
}

static void PaintSplit(ImageBuffer& out, const Image& in,
	const PlanarSegmentTree::Node& planar,
	const LinearSegmentTree::Node *left, const LinearSegmentTree::Node *top,
	const LinearSegmentTree::Node *right, const LinearSegmentTree::Node *bottom,
	const SegmentTreeInfo& info, Color background, int index)
{
	if(planar.split) {
		index <<= 2;
		PaintSplit(out, in, planar.split->topleft,
			left ? ~left->below : NULL, top ? ~top->below : NULL, NULL, NULL,
			info, background, index + 0);
		PaintSplit(out, in, planar.split->topright,
			NULL, top ? ~top->above : NULL, right ? ~right->below : NULL, NULL,
			info, background, index + 1);
		PaintSplit(out, in, planar.split->bottomleft,
			left ? ~left->above : NULL, NULL, NULL, bottom ? ~bottom->below : NULL,
			info, background, index + 2);
		PaintSplit(out, in, planar.split->bottomright,
			NULL, NULL, right ? ~right->above : NULL, bottom ? ~bottom->above : NULL,
			info, background, index + 3);
	}
	else if(!left && !top && !right && !bottom) {
		Sizef d01 = Length(planar.trg_topright - planar.trg_topleft, 1);
		Sizef d12 = Length(planar.trg_bottomright - planar.trg_topright, 1);
		Sizef d23 = Length(planar.trg_bottomleft - planar.trg_bottomright, 1);
		Sizef d30 = Length(planar.trg_topleft - planar.trg_bottomleft, 1);
		Size d0 = Length(d30 - d01, 2), d1 = Length(d01 - d12, 2), d2 = Length(d12 - d23, 2), d3 = Length(d23 - d30, 2);
		Point destpos[4];
		destpos[0] = planar.trg_topleft + d0;
		destpos[1] = planar.trg_topright + d1;
		destpos[2] = planar.trg_bottomright + d2;
		destpos[3] = planar.trg_bottomleft + d3;
		if(!in)
			BilinearSet(out, destpos, background);
		else
			BilinearCopy(out, destpos, in, planar.source, NULL, info.antialias);
	}
	else {
		bool empty = !in;
		Point msrc = planar.source.CenterPoint();
		Point mdest = msrc * info;
		Vector<Point> bsrc;
		Vector<Point> bdest;
		ExpandSegmentUp  (bsrc, bdest, top,    planar.source.TopLeft(),     planar.trg_topleft);
		ExpandSegmentUp  (bsrc, bdest, right,  planar.source.TopRight(),    planar.trg_topright);
		ExpandSegmentDown(bsrc, bdest, bottom, planar.source.BottomRight(), planar.trg_bottomright);
		ExpandSegmentDown(bsrc, bdest, left,   planar.source.BottomLeft(),  planar.trg_bottomleft);
		Point psrc = bsrc.Top();
		Point pdest = bdest.Top();
		for(int i = 0; i < bsrc.GetCount(); i++) {
			Point nsrc = bsrc[i];
			Point ndest = bdest[i];
			Sizef pn = Length(nsrc - psrc, 1);
			Sizef nm = Length(msrc - nsrc, 1);
			Sizef mp = Length(psrc - msrc, 1);
			Size dp = Length(mp - pn, 2), dn = Length(pn - nm, 2), dm = Length(nm - mp, 2);
			if(empty)
				LinearSet(out, mdest + dm, pdest + dp, ndest + dn, background);
			else
				LinearCopy(out, mdest + dm, pdest + dp, ndest + dn, in, msrc + dm, psrc + dp, nsrc + dn, NULL, info.antialias);
			psrc = nsrc;
			pdest = ndest;
		}
	}
}

void AlphaTransformPaint(ImageBuffer& out, const Image& in,
	const PlanarSegmentTree& planar,
	const LinearSegmentTree& left, const LinearSegmentTree& top,
	const LinearSegmentTree& right, const LinearSegmentTree& bottom,
	const SegmentTreeInfo& info, Color background)
{
	PaintSplit(out, in, planar.root, ~left.split, ~top.split, ~right.split, ~bottom.split, info, background, 1);
}

static void PaintTreeSplit(Draw& draw, const PlanarSegmentTree::Node& planar,
	const LinearSegmentTree::Node *left, const LinearSegmentTree::Node *top,
	const LinearSegmentTree::Node *right, const LinearSegmentTree::Node *bottom, int index)
{
	if(planar.split) {
		index <<= 2;
		PaintTreeSplit(draw, planar.split->topleft,     left ? ~left->below : NULL, top ? ~top->below : NULL, NULL, NULL, index + 0);
		PaintTreeSplit(draw, planar.split->topright,    NULL, top ? ~top->above : NULL, right ? ~right->below : NULL, NULL, index + 1);
		PaintTreeSplit(draw, planar.split->bottomleft,  left ? ~left->above : NULL, NULL, NULL, bottom ? ~bottom->below : NULL, index + 2);
		PaintTreeSplit(draw, planar.split->bottomright, NULL, NULL, right ? ~right->above : NULL, bottom ? ~bottom->above : NULL, index + 3);
	}
	else {
		Vector<Point> bsrc;
		Vector<Point> bdest;
		ExpandSegmentUp  (bsrc, bdest, top,    planar.source.TopLeft(),     planar.trg_topleft);
		ExpandSegmentUp  (bsrc, bdest, right,  planar.source.TopRight(),    planar.trg_topright);
		ExpandSegmentDown(bsrc, bdest, bottom, planar.source.BottomRight(), planar.trg_bottomright);
		ExpandSegmentDown(bsrc, bdest, left,   planar.source.BottomLeft(),  planar.trg_bottomleft);
		Point center = (planar.trg_topleft + planar.trg_topright + planar.trg_bottomleft + planar.trg_bottomright) >> 2;
		String fmt = FormatIntHex(index, 4);
		Font font = StdFont();
		Size fmtsize = GetTextSize(fmt, font);
		draw.DrawText(center.x - (fmtsize.cx >> 1), center.y - (fmtsize.cy >> 1), fmt, font, LtGreen());
		Point pdest = bdest[0];
		bdest.Add(pdest);
		draw.DrawPolyline(bdest, 0, LtBlue());
		for(int i = 1; i < bdest.GetCount(); i++)
			draw.DrawRect(bdest[i].x - 1, bdest[i].y - 1, 3, 3, LtGreen());
	}
}

void PaintPlanarTree(Draw& draw, const PlanarSegmentTree& planar,
	const LinearSegmentTree& left, const LinearSegmentTree& top,
	const LinearSegmentTree& right, const LinearSegmentTree& bottom)
{
	PaintTreeSplit(draw, planar.root, ~left.split, ~top.split, ~right.split, ~bottom.split, 1);
}

static void PlotLine(PlotterTool& tool, GisTransform transform, Pointf src1, Pointf src2, int level, double maxdev)
{
	double dev;
	if(--level <= 0 || (dev = transform.SourceDeviation(src1, src2)) <= maxdev) {
		tool.LineTo(transform.Target(src2));
		return;
	}
	Pointf mid = (src1 + src2) / 2.0;
	PlotLine(tool, transform, src1, mid, level, maxdev);
	PlotLine(tool, transform, mid, src2, level, maxdev);
}

/*
void PlotLine(PlotterTool& tool, GisCoords proj, Pointf lonlat1, Pointf lonlat2, int branch)
{
	tool.MoveTo(proj.Project(lonlat1, branch));
	PlotLine(tool, proj, lonlat1, lonlat2, branch, 8,
		(tool.GetPlotter().GetDraw().Dots() ? 5 : 2) / tool.GetPlotter().measure);
}

void PlotLineSafe(PlotterTool& tool, GisCoords proj, Pointf lonlat1, Pointf lonlat2, int branch)
{
	int nseg = minmax<int>((int)fpabsmax(lonlat2 - lonlat1), 1, 360);
	tool.MoveTo(proj.Project(lonlat1, branch));
	for(int i = 1; i <= nseg; i++)
		tool.LineTo(proj.Project(lonlat1 + (lonlat2 - lonlat1) * (i / double(nseg)), branch));
}

void PlotRectSafe(PlotterTool& tool, GisCoords proj, const Rectf& lonlat, int branch)
{
	PlotLineSafe(tool, proj, lonlat.TopLeft(),     lonlat.TopRight(),    branch);
	PlotLineSafe(tool, proj, lonlat.TopRight(),    lonlat.BottomRight(), branch);
	PlotLineSafe(tool, proj, lonlat.BottomRight(), lonlat.BottomLeft(),  branch);
	PlotLineSafe(tool, proj, lonlat.BottomLeft(),  lonlat.TopLeft(),     branch);
}
*/

void PlotLine(PlotterTool& tool, GisTransform transform, Pointf source1, Pointf source2)
{
	if(transform.IsIdentity())
		tool.Line(source1, source2);
	else {
		Pointf target1 = transform.Target(source1), target2 = transform.Target(source2);
		if(!tool.GetPlotter().InLClip(target1) || !tool.GetPlotter().InLClip(target2)) {
			if(!ClipLine(source1, source2, transform.SourceExtent(tool.GetPlotter().logclip)))
				return;
			target1 = transform.Target(source1);
		}
		tool.MoveTo(target1);
		PlotLine(tool, transform, source1, source2, 8,
			(tool.GetPlotter().GetDraw().Dots() ? 5 : 2) / tool.GetPlotter().measure);
	}
}

void PlotRect(PlotterTool& tool, GisTransform transform, const Rectf& src)
{
	PlotLine(tool, transform, src.TopLeft(),     src.TopRight());
	PlotLine(tool, transform, src.TopRight(),    src.BottomRight());
	PlotLine(tool, transform, src.BottomRight(), src.BottomLeft());
	PlotLine(tool, transform, src.BottomLeft(),  src.TopLeft());
}

}
