#include "MapLevel.h"

#define IMAGECLASS MapLevelImg
#define IMAGEFILE <Map/MapLevel.iml>
#include <Draw/iml_source.h>

MapLevel::MapLevel()
	: IMapRender(this), _highQuality(true), _editMode(false), _currPoint(-1),
	 _pagePos(Point(0, 0)), _editModePos(Point(0, 0)), _zoomLevel(-1)
{
	SetTopRender(this);
	BackPaint();
}

void MapLevel::ClearBG()
{
	for (int i = 0; i < _layers.GetCount(); ++i)
		if (_layers[i])
			delete _layers[i];
	_layers.Clear();
}

bool MapLevel::LoadMap(const char* fp)
{
	_zoomLevel = -1;
	if (!AddLayer<MapBG>(0)->Load(fp))
	{
		LOG("MapLevel: Error while creating MapBG-layer!");
		return false;
	}
	if (!LoadFromXMLFile(_zoomLevels,
		AppendFileName( GetFileDirectory(fp), AppendExt(GetFileTitle(fp), ".zoom") )))
	{
		LOG(NFormat("MapLevel: Error while loading zoom-levels from file %s", fp));
		return false;
	}

	if (_zoomLevels.GetCount())
		_zoomLevel = 0;

	Subpixel();
	Zoom(_zoomLevel);
	MoveTo(Point(0, 0));

	return true;
}

void MapLevel::UpdateMap()
{
	if (_layers.GetCount())
		if (_layers[0])
			_layers[0]->PerformState(PERFORM_INIT);
	Refresh();
}
