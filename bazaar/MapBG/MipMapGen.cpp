#include "MipMapGen.h"
#include "MipMap.h"
#include <Painter/Painter.h>

static StaticMutex LevelsMutex;
static Vector<MipMapLevel> Levels;
static Vector<MapZoomLevel> ZoomLevels;

bool CalcLevel(Image& img, int& step, int blocks, int zoomLevel, const String& dir, Progress& p,
	 const Size& totalSize, int cx, int cy, double zx);
void CalcCount(Image& img, int cx, int cy, int& step);
int  GetScalesTotal(Image& img, const Size& sz, double zoom);
int  GetBlocksTotal(Image& img, const Size& sz, const Size& cellSize, double zoom);
Size GetScaledSize(Image& img, const Size& sz, double zoom, int level);
Image GetScaledMap(Image& img, const Size& sz);
Image GetScaledMap(Image& img, const Size& sz, double zoom, int level);

bool Calculate(int cx, int cy, int px, int py, double zx, const String& name, Image& img)
{
	Mutex::Lock __(LevelsMutex);

	Levels.Clear();
	ZoomLevels.Clear();

	// getting data
	Size sz = Size(cx, cy);
	Size ps = Size(px, py);

	// preparing directory
	String dir = AppendFileName( AppendFileName(GetFileDirectory(GetExeFilePath()), "Mipmaps"),
		name);
	RemoveDirectory(dir);
	RealizeDirectory(dir);

	String xmlFile = AppendFileName(
		AppendFileName(GetFileDirectory(GetExeFilePath()), "Mipmaps"),
			name + ".xml");

	String zoomFile = AppendFileName(
		AppendFileName(GetFileDirectory(GetExeFilePath()), "Mipmaps"),
			name.ToString() + ".zoom");

	FileDelete(xmlFile);
	FileDelete(zoomFile);

	// generating first level
	Levels.Clear();

	int blocks = GetBlocksTotal(img, ps, sz, zx);
	int total  = GetScalesTotal(img, ps, zx);
	int count  = 0;

	Progress p;
	p.Create();

	Size last = Null;
	for (int i = 0; i < total - 1; ++i)
	{
		Image scaledMap = GetScaledMap(img, ps, zx, i + 1);

		if (i == total - 2)
			last = scaledMap.GetSize();
		if (!CalcLevel(scaledMap, count, blocks, i, dir, p, scaledMap.GetSize(), cx, cy,
			(i == total - 1) ? 1.0 : zx))
				break;
	}

	if (!IsNull(last))
		CalcLevel(img, count, blocks, total - 1, dir, p, img.GetSize(), cx, cy,
			(double)img.GetSize().cx / last.cx);
	p.Close();

	if (!StoreAsXMLFile(Levels, "levels", xmlFile))
	{
		Exclamation("Ошибка при сохранении XML-файла карты!");
		return false;
	}

	if (!StoreAsXMLFile(ZoomLevels, "zoomLevels", zoomFile))
	{
		Exclamation("Ошибка при сохранении XML-файла значений zoom!");
		return false;
	}

	return true;
}

bool CalcLevel(Image& img, int& step, int blocks, int zoomLevel, const String& dir, Progress& p,
	const Size& totalSize, int cx, int cy, double zx)
{
	Size sz = Size(cx, cy);

	double ratio = (double)img.GetSize().cx / img.GetSize().cy;
	int  xt = img.GetSize().cx / cx + (img.GetSize().cx % cx ? 1 : 0);
	int  yt = img.GetSize().cy / cy + (img.GetSize().cy % cy ? 1 : 0);

	MipMapLevel level;
	level
		.TotalCX(xt).TotalCY(yt).BlockCX(cx).BlockCY(cy).ZoomDX(zx)
		.TotalSizeCX(totalSize.cx)
		.TotalSizeCY(totalSize.cy);

	ZoomLevels.Add(MapZoomLevel(zx));

	int count = 0;
	for (int i = 0; i < xt; ++i)
		for (int j = 0; j < yt; ++j)
		{
			p.Title(NFormat("Блок %d из %d [%d, %d]", step + 1, blocks, cx, cy));

			ImageBuffer ib(cx, cy);
			BufferPainter sw(ib);
			sw.DrawRect(ib.GetSize(), White());
			sw.DrawImage(ib.GetSize(), img, Rect(Point(i * cx, j * cy), ib.GetSize()));

			PNGEncoder en;
			if (!en.SaveFile(AppendFileName(dir, NFormat("%d-%d-%d.png", zoomLevel,
				i * cx, j * cy)), Rescale(ib, sz, ib.GetSize())))
			{
				Exclamation("Ошибка при сохранении MipMap-изображения!");
				return false;
			}

			level.AddMipMap( Point(i * cx, j * cy), new MipMapItem() );
			p.Set(step++, blocks);
		}

	// xml part
	Levels.Add(level);
	return true;
}

void CalcCount(Image& img, int cx, int cy, int& step)
{
	Size sz = img.GetSize();
	int  xt = sz.cx / cx + (sz.cx % cx ? 1 : 0);
	int  yt = sz.cy / cy + (sz.cy % cy ? 1 : 0);

	for (int i = 0; i < xt; ++i)
		for (int j = 0; j < yt; ++j)
		{
			step++;
		}
}

Image GetScaledMap(Image& img, const Size& sz)
{
	double scale = min((double)sz.cx / img.GetSize().cx, (double)sz.cy / img.GetSize().cy);
	return Rescale(img, img.GetSize() * scale, img.GetSize());
}

Image GetScaledMap(Image& img, const Size& sz, double zoom, int level)
{
	return Rescale(img, GetScaledSize(img, sz, zoom, level), img.GetSize());
}

Size GetScaledSize(Image& img, const Size& sz, double zoom, int level)
{
	if (!level) return sz;

	double scale = min((double)sz.cx / img.GetSize().cx, (double)sz.cy / img.GetSize().cy);
	Size mapSize = img.GetSize() * scale;
	double ratio = (double)img.GetSize().cx / img.GetSize().cy;
	double w     = (double)mapSize.cx;

	for (int i = 1; i < level; ++i)
		w *= zoom;

	if (w > img.GetSize().cx || w / ratio > img.GetSize().cy)
		return img.GetSize();

	return Size(fabs(w), fabs(w / ratio));
}

int GetScalesTotal(Image& img, const Size& sz, double zoom)
{
	int totalCount = 0;
	Size check(0, 0);
	for(;;)
	{
		Size scaledSize = GetScaledSize(img, sz, zoom, totalCount + 1);

		bool found = false;
		if (check == Size(0, 0))
			check = scaledSize;
		else
		{
			if (check == scaledSize)
				found = true;
			else
				check = scaledSize;
		}

		if (found)
			break;

		totalCount++;
	}

	return totalCount;
}

int GetBlocksTotal(Image& img, const Size& sz, const Size& cellSize, double zoom)
{
	int  total = GetScalesTotal(img, sz, zoom);
	int  count = 0;

	for (int i = 0; i < total - 1; ++i)
	{
		Image scaledMap = GetScaledMap(img, sz, zoom, i + 1);
		if (i < total - 1)
			CalcCount(scaledMap, cellSize.cx, cellSize.cy, count);
		if (i == total - 1)
			CalcCount(img, cellSize.cx, cellSize.cy, count);
	}
	return count;
}
