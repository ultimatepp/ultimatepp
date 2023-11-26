#include <Draw/Draw.h>
#include <plugin/tif/tif.h>
#include <Geom/Geom.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	TIFRaster tif;
	
	FileIn in(GetDataFile("bogota.tif"));
	
	if(!in || !tif.Open(in)) {
		LOG("Failed to open tiff");
		return;
	}

	int epsg = tif.GetMetaData("epsg");
	LOG("EPSG code: " << epsg);
	
	Value affine = tif.GetMetaData("geo_matrix");
	if(affine.GetCount() != 6) {
		LOG("Invalid transformation matrix");
		return;
	}

	Matrixf transform;
	transform.a.x = affine[0];
	transform.x.x = affine[1];
	transform.y.x = affine[2];
	transform.a.y = affine[3];
	transform.x.y = affine[4];
	transform.y.y = affine[5];

	if(fabs(Determinant(transform)) <= 1e-10) {
		LOG("Invalid transformation matrix");
		return;
	}

	Size isz = tif.GetSize();

	Rectf r = Rectf(Pointf(0, 0) * transform, Pointf(isz.cx, isz.cy) * transform).Normalized();

	LOG("Coordinates: " << r);

	DUMP(tif.GetMetaData("BITSPERSAMPLE"));
	DUMP(tif.GetMetaData("SAMPLESPERPIXEL"));
	DUMP(tif.GetMetaData("PHOTOMETRIC"));
}
