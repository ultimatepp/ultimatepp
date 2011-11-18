#ifndef MAP_BG__MAP_COMMON_H
#define MAP_BG__MAP_COMMON_H

#include "FileUtils.h"

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

class IMipMapItem : public Moveable<IMipMapItem>, public Pte<IMipMapItem>
{
public:
	virtual ~IMipMapItem() {}

	virtual void Prepare(const char* fp, const Size& sz) = 0;
	virtual void Release() = 0;

	virtual void Render(Draw* w, const Rect& r) = 0;
	virtual void Xmlize(XmlIO xml) {}
};

class MipMapItem : public IMipMapItem
{
public:
	MipMapItem() : _ready(false) {}
	virtual ~MipMapItem() {}

	virtual void Prepare(const char* fp, const Size& sz);
	virtual void Release();

	virtual void Render(Draw* w, const Rect& r);

private:
	bool _ready;
	Image _mipMap;
};

class MipMapLevel : public Moveable<MipMapLevel>
{
public:
	MipMapLevel() {}
	MipMapLevel(const MipMapLevel& other)
	{
		_blockX     = other._blockX;
		_blockY     = other._blockY;
		_totalX     = other._totalX;
		_totalY     = other._totalY;
		_zoomDX     = other._zoomDX;
		_totalSizeX = other._totalSizeX;
		_totalSizeY = other._totalSizeY;
		_mipMaps  <<= other._mipMaps;
	}

	virtual ~MipMapLevel() { Clear(); }
	void Clear()
	{
		for (int i = 0; i < _mipMaps.GetCount(); ++i)
			if (_mipMaps[i])
				delete _mipMaps[i];
		_mipMaps.Clear();
	}

	void Release()
	{
		for (int i = 0; i < _mipMaps.GetCount(); ++i)
			if (_mipMaps[i])
				_mipMaps[i]->Release();
	}

	MipMapLevel& BlockCX    (int i) { Mutex::Lock _(_mutex); _blockX     = i; return *this; }
	MipMapLevel& BlockCY    (int i) { Mutex::Lock _(_mutex); _blockY     = i; return *this; }
	MipMapLevel& TotalCX    (int i) { Mutex::Lock _(_mutex); _totalX     = i; return *this; }
	MipMapLevel& TotalCY    (int i) { Mutex::Lock _(_mutex); _totalY     = i; return *this; }
	MipMapLevel& TotalSizeCX(int i) { Mutex::Lock _(_mutex); _totalSizeX = i; return *this; }
	MipMapLevel& TotalSizeCY(int i) { Mutex::Lock _(_mutex); _totalSizeY = i; return *this; }
	MipMapLevel& ZoomDX( double dx) { Mutex::Lock _(_mutex); _zoomDX    = dx; return *this; }

	int GetBlockCX()     { Mutex::Lock _(_mutex); return _blockX; }
	int GetBlockCY()     { Mutex::Lock _(_mutex); return _blockY; }
	int GetTotalCX()     { Mutex::Lock _(_mutex); return _totalX; }
	int GetTotalCY()     { Mutex::Lock _(_mutex); return _totalY; }
	int GetTotalSizeCX() { Mutex::Lock _(_mutex); return _totalSizeX; }
	int GetTotalSizeCY() { Mutex::Lock _(_mutex); return _totalSizeY; }
	double GetZoomDX()   { Mutex::Lock _(_mutex); return _zoomDX; }

	Size GetBlockSize()      { Mutex::Lock _(_mutex); return Size(_blockX, _blockY); }
	Size GetTotalSize()      { Mutex::Lock _(_mutex); return Size(_totalSizeX, _totalSizeY); }
	Size GetTotalBlockSize() { Mutex::Lock _(_mutex); return Size(_totalX, _totalY); }

	MipMapLevel& AddMipMap(const Point& p, Ptr<IMipMapItem> item)
	{
		Mutex::Lock _(_mutex);
		_mipMaps.Add(p, item);
		return *this;
	}

	const VectorMap<Point, Ptr<IMipMapItem> >& GetMipMaps() const { return _mipMaps; }
	      VectorMap<Point, Ptr<IMipMapItem> >& GetMipMaps()       { return _mipMaps; }

	void Xmlize(XmlIO xml)
	{
		Mutex::Lock _(_mutex);

		xml ("blockX",     _blockX)
			("blockY",     _blockY)
			("totalX",     _totalX)
			("totalY",     _totalY)
			("zoomDX",     _zoomDX)
			("totalSizeX", _totalSizeX)
			("totalSizeY", _totalSizeY);

		if (xml.IsStoring())
		{
			Vector<Point> keys = _mipMaps.GetKeys();
			xml("mipmaps", keys);
		}
		else
		{
			Vector<Point> keys;
			xml("mipmaps", keys);

			Clear();
			for (int i = 0; i < keys.GetCount(); ++i)
				AddMipMap(keys[i], new MipMapItem());
		}
	}

private:
	Mutex _mutex;
	int _blockX;
	int _blockY;
	int _totalX;
	int _totalY;
	int _totalSizeX;
	int _totalSizeY;
	double _zoomDX;
	VectorMap<Point, Ptr<IMipMapItem> > _mipMaps;
};

class MapZoomLevel : public Moveable<MapZoomLevel>
{
public:
	MapZoomLevel() : _zoomDX(1.0) {}
	MapZoomLevel(double zx) : _zoomDX(zx) {}
	MapZoomLevel(const MapZoomLevel& other) { _zoomDX = other._zoomDX; }
	virtual ~MapZoomLevel() {}

	MapZoomLevel& Zoom( double dx) { Mutex::Lock _(_mutex); _zoomDX = dx; return *this; }
	double GetZoom() { Mutex::Lock _(_mutex); return _zoomDX; }

	void Xmlize(XmlIO xml)
	{
		Mutex::Lock _(_mutex);
		xml("zoomDX", _zoomDX);
	}

private:
	Mutex _mutex;
	double _zoomDX;
};

#endif
