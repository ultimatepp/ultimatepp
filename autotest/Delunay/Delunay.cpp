#include <Core/Core.h>
#include <Geom/Geom.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Array<Pointf> poly;
	poly << Pointf(18.951605,	0.8) << Pointf(19.714577,	0.6168874) << Pointf(20.14996, 0) << Pointf(19.714577, -0.6168874) << Pointf(18.951605, -0.8) << Pointf(17.753117, 0) << Pointf(18.188618, -0.6) << Pointf(18.188618, 0.6);
		
	Delaunay del;
	del.Build(poly);
}
