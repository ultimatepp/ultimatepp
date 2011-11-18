#include "MapBG.h"

MapBG::MapBG(IMapRender* render)
	: IMapRender(render), _recalc(true), _currLevel(-1), _virtPage(Size(0, 0)),
		_cachePageSize(CACHE_PAGE_SIZE), _cachePageInitSize(CACHE_PAGE_INIT_SIZE)
{}

void MapBG::Render(Draw* w)
{
	if (_currLevel < 0 || _currLevel >= _levels.GetCount() || !GetTopRender())
		return;

	MipMapLevel& level = _levels[_currLevel];

	Point offset = GetTopRender()->GetPageOffset();
	Rect  page   = GetTopRender()->GetPageRect();
	Point lt     = GetTopRender()->GetPagePos();

	for (int i = 0; i < level.GetMipMaps().GetCount(); ++i)
	{
		Rect itemRect(level.GetMipMaps().GetKey(i), level.GetBlockSize());

		if (page.Intersects(itemRect))
			if (level.GetMipMaps()[i])
			{
				level.GetMipMaps()[i]
					->Render(w,
				         itemRect
							.Offseted(-lt)
							.Offseted(offset));
			}
	}

	IMapRender::Render(w);
}

void MapBG::StatePerformed(dword state, const String& param)
{
	if (_currLevel < 0 || _currLevel >= _levels.GetCount())
		return;

	MipMapLevel& level = _levels[_currLevel];
	Rect pageRect = GetPageRect();

	if (_recalc || !_virtPage.Contains(GetPageRect()))
	{
		_virtPage = pageRect.Inflated(level.GetBlockSize() * _cachePageInitSize);
		_recalc = true;
	}

	Rect currVirtualPage = _virtPage.Inflated(level.GetBlockSize() * _cachePageSize);

	if (!_recalc)
		return;

	for (int i = 0; i < level.GetMipMaps().GetCount(); ++i)
	{
		Point pi = level.GetMipMaps().GetKey(i);
		Rect itemRect(pi, level.GetBlockSize());

		if (!level.GetMipMaps()[i])
			continue;

		if (currVirtualPage.Intersects(itemRect))
		{
			level.GetMipMaps()[i]->Prepare(
				AppendFileName(_mapDir, NFormat("%d-%d-%d.png", _currLevel, pi.x, pi.y)),
				level.GetBlockSize()
			);
		}
		else
			level.GetMipMaps()[i]->Release();
	}

	_recalc = false;
}

bool MapBG::Load(const char* fp)
{
	if (!FileExists(fp))
	{
		LOG(NFormat("MapBG: File not exists, \"%s\"", fp));
		return false;
	}

	if (!LoadFromXMLFile(_levels, fp))
	{
		LOG("MapBG: Error while loading map from XML-file!");
		return false;
	}

	_mapDir = AppendFileName(GetFileDirectory(fp), GetFileTitle(fp));
	_currLevel = _levels.GetCount() > 0 ? 0 : -1;
	if (_currLevel < 0)
	{
		LOG("MapBG: No zoom levels found in map!");
		return false;
	}
	_recalc = true;

	return true;
}

bool MapBG::Zoom(int level)
{
	if (level < 0 || level >= _levels.GetCount())
		return false;
	_currLevel = level;
	_recalc = true;
	return true;
}

Rect MapBG::GetRect()
{
	if (_currLevel < 0 || _currLevel >= _levels.GetCount())
		return Size(0, 0);

	return Rect(Point(0, 0), _levels[_currLevel].GetTotalSize());
}
