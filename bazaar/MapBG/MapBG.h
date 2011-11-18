#ifndef MAP_BG__MAP_BG_H
#define MAP_BG__MAP_BG_H

#include <MapCommon/MapCommon.h>
#include "MipMapGen.h"
#include "MipMap.h"

#define CACHE_PAGE_INIT_SIZE Size(2, 2)
#define CACHE_PAGE_SIZE      Size(4, 4)

class MapBG : public IMapRender
{
public:
	MapBG(IMapRender* render);
	virtual ~MapBG() {}

	virtual void Render(Draw* w);
	virtual Rect GetRect();

	virtual void StatePerformed(dword state, const String& param = String());
	virtual bool Load(const char* fp);
	virtual bool Zoom(int level);

	virtual Rect GetRenderRect() { return GetRect(); }

	virtual String GetItemType() { return "MapBG"; }

protected:
	Size _cachePageInitSize;
	Size _cachePageSize;
	int  _currLevel;
	bool _recalc;
	Rect _virtPage;
	String _mapDir;

	Vector<MipMapLevel> _levels;
};

#endif
