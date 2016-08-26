#include "Painter.h"

namespace Upp {

void LinearPathConsumer::End()
{
}

void LinearPathFilter::End()
{
	PutEnd();
}

void Transformer::Move(const Pointf& p)
{
	PutMove(xform.Transform(p));
}

void Transformer::Line(const Pointf& p)
{
	PutLine(xform.Transform(p));
}

}
