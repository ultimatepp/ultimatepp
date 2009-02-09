#include "Painter.h"

NAMESPACE_UPP

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

END_UPP_NAMESPACE