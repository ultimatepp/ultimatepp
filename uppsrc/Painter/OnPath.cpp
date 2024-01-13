#include "Painter.h"

namespace Upp {

void BufferPainter::BeginOnPathOp(double q, bool abs)
{
	One<SpanSource> none;
	if(onpath.GetCount() == 0)
		RenderPath(ONPATH, none, RGBAZero());
	Begin();
	if(pathlen > 0) {
		if(!abs)
			q *= pathlen;
		Pointf pos(0, 0);
		for(int i = 0; i < onpath.GetCount(); i++) {
			PathLine& l = onpath[i];
			if(l.len > 0 && (l.len > q || q >= 1.0 && i == onpath.GetCount() - 1)) {
				Pointf v = l.p - pos;
				Translate(q / l.len * v + pos);
				Rotate(Bearing(v));
				break;
			}
			q -= l.len;
			pos = l.p;
		}
	}
	attrstack.Top().onpath = true;
	onpathstack.Add() = pick(onpath);
	pathlenstack.Add(pathlen);
	onpath.Clear();
	pathlen = 0;
}

}
