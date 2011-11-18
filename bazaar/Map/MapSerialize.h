#ifndef MAP__MAP_SERIALIZE_H
#define MAP__MAP_SERIALIZE_H

#include <Core/Core.h>
using namespace Upp;

#include <FormEditorProperties/FormEditorProperties.h>

inline int CharFilterMapName(int c)
{
	return c >= 32 && c < 128 && c != '\"' ? c : 0;
}

class MapItemProp : public PropertiesBase<MapItemProp>
{
public:
	MapItemProp() : PropertiesBase<MapItemProp>(this) {}
};

class Room : public MapItemProp
{
	typedef Room CLASSNAME;

public:
	Room() : number(0), discount(0) {}
	virtual ~Room() {}

	PROPS_INIT_PROPERTIES
		PROPS_DEFINE_PROPERTY(SetName, GetName)
			PROPS_GROUP(t_("Room properties"))
			PROPS_NAME(t_("Name"))
			PROPS_HINT(t_("Sets the name of the room"))
			PROPS_TYPE(CEditField(), true)
		PROPS_PROPERTY_END

		PROPS_DEFINE_PROPERTY(SetDesc, GetDesc)
			PROPS_GROUP(t_("Room properties"))
			PROPS_NAME(t_("Description"))
			PROPS_HINT(t_("Sets the description of the room"))
			PROPS_TYPE(CDocEdit(), true)
		PROPS_PROPERTY_END

		PROPS_DEFINE_PROPERTY(SetTags, GetTags)
			PROPS_GROUP(t_("Room properties"))
			PROPS_NAME(t_("Tags"))
			PROPS_HINT(t_("Sets the tags of the room"))
			PROPS_TYPE(CEditField(), true)
		PROPS_PROPERTY_END

		PROPS_DEFINE_PROPERTY(SetNumber, GetNumber)
			PROPS_GROUP(t_("Room properties"))
			PROPS_NAME(t_("Room number"))
			PROPS_HINT(t_("Sets the number of the room"))
			PROPS_TYPE(CEditIntSpin(0, INT_MAX), true)
		PROPS_PROPERTY_END

		PROPS_DEFINE_PROPERTY(SetLogoPath, GetLogoPath)
			PROPS_GROUP(t_("Room properties"))
			PROPS_NAME(t_("Logo path"))
			PROPS_HINT(t_("Sets the path to the room logo"))
			PROPS_TYPE(CEditField(), true)
		PROPS_PROPERTY_END

		PROPS_DEFINE_PROPERTY(SetDiscount, GetDiscount)
			PROPS_GROUP(t_("Discount properties"))
			PROPS_NAME(t_("Discount"))
			PROPS_HINT(t_("Sets the number of the room discounts"))
			PROPS_TYPE(CEditIntSpin(0, INT_MAX), true)
		PROPS_PROPERTY_END
	PROPS_INIT_PROPERTIES_END

	PROPS_METHOD_SET(SetName, void, const char*);
	PROPS_METHOD_GET(GetName, String);

	PROPS_METHOD_SET(SetDesc, void, const char*);
	PROPS_METHOD_GET(GetDesc, String);

	PROPS_METHOD_SET(SetTags, void, const char*);
	PROPS_METHOD_GET(GetTags, String);

	PROPS_METHOD_SET(SetNumber, void, int);
	PROPS_METHOD_GET(GetNumber, int);

	PROPS_METHOD_SET(SetDiscount, void, int);
	PROPS_METHOD_GET(GetDiscount, int);

	PROPS_METHOD_SET(SetLogoPath, void, const char*);
	PROPS_METHOD_GET(GetLogoPath, String);

public:
	void   SetName(const char* s) { name = s; }
	String GetName() const { return name; }

	void   SetDesc(const char* s) { desc = s; }
	String GetDesc() const { return desc; }

	void   SetTags(const char* s) { tags = s; }
	String GetTags() const { return tags; }

	void   SetNumber(int n)  { number = n; }
	int    GetNumber() const { return number; }

	void   SetDiscount(int n)  { discount = n; }
	int    GetDiscount() const { return discount; }

	void   SetLogoPath(const char* s) { logo = s; }
	String GetLogoPath() const { return logo; }

	Room&  AddVertice(const Point& p) { vertices.Add(p); return *this;  }
	Room&  AddVertice(int x, int y)   { vertices.Add(Point(x, y)); return *this; }

	Array<Point>& GetVertices() { return vertices; }
	const Array<Point>& GetVertices() const { return vertices; }

public:
	void Xmlize(XmlIO& xml)
	{
		xml ("polygon", "point", vertices)
			("number", number)
			("logo", logo)
			("name", name)
			("desc", desc)
			("tags", tags)
			("discount", discount);
	}

public:
	Room(const Room& other)
	{
		vertices <<= other.vertices;
		number     = other.number;
		logo       = other.logo;
		name       = other.name;
		desc       = other.desc;
		tags       = other.tags;
		discount   = other.discount;
	}

	Room& operator=(const Room& other)
	{
		vertices <<= other.vertices;
		number     = other.number;
		logo       = other.logo;
		name       = other.name;
		desc       = other.desc;
		tags       = other.tags;
		discount   = other.discount;
		return *this;
	}

	void SetRoom(const Room& other)
	{
		*this = other;
	}

protected:
	Array<Point> vertices;
	String logo;
	String name;
	String desc;
	String tags;
	int discount;
	int number;
};

class Layer : public MapItemProp
{
	typedef Layer CLASSNAME;

public:
	virtual ~Layer() {}

	PROPS_INIT_PROPERTIES
		PROPS_DEFINE_PROPERTY(SetName, GetName)
			PROPS_GROUP(t_("Layer properties"))
			PROPS_NAME(t_("Name"))
			PROPS_HINT(t_("Sets the name of the layer"))
			PROPS_TYPE(CEditField(), true)
		PROPS_PROPERTY_END
	PROPS_INIT_PROPERTIES_END

	PROPS_METHOD_SET(SetName, void, const char*);
	PROPS_METHOD_GET(GetName, String);

public:
	void   SetName(const char* s) { name = s; }
	String GetName() const { return name; }

	Array<Room>& GetRooms() { return rooms; }
	const Array<Room>& GetRooms() const { return rooms; }

public:
	void Xmlize(XmlIO& xml)
	{
		xml ("name", name)
			("rooms", "room", rooms);
	}

public:
	Layer() {}
	Layer(const Layer& other)
	{
		rooms <<= other.rooms;
		name    = other.name;
	}

private:
	Array<Room> rooms;
	String name;
};

class Level : public MapItemProp
{
	typedef Level CLASSNAME;

public:
	Level()
		: zoomDX(1.2),
		pageSize(Size(736, 420)),
		cellSize(Size(128, 128))
	{}
	virtual ~Level() {}

	PROPS_INIT_PROPERTIES
		// Level properties
		PROPS_DEFINE_PROPERTY(SetName, GetName)
			PROPS_GROUP(t_("Level properties"))
			PROPS_NAME(t_("Name"))
			PROPS_HINT(t_("Sets the name of the level"))
			PROPS_TYPE(CEditField(), true)
		PROPS_PROPERTY_END

		PROPS_DEFINE_PROPERTY(SetDesc, GetDesc)
			PROPS_GROUP(t_("Level properties"))
			PROPS_NAME(t_("Description"))
			PROPS_HINT(t_("Sets the description of the level"))
			PROPS_TYPE(CDocEdit(), true)
		PROPS_PROPERTY_END

		// BG properties
		PROPS_DEFINE_PROPERTY(SetMapBG, GetMapBG)
			PROPS_GROUP(t_("BG properties"))
			PROPS_DEFAULT("")
			PROPS_NAME(t_("Source map image"))
			PROPS_HINT(t_("Sets the background map of the level"))
			PROPS_TYPE(CDropList(FillMapList(VectorMapEx<String, Value>()
				.Add(t_("None selected"), ""))), true)
		PROPS_PROPERTY_END

		PROPS_DEFINE_PROPERTY(SetPageSize, GetPageSize)
			PROPS_GROUP(t_("BG properties"))
			PROPS_NAME(t_("Start page size"))
			PROPS_HINT(t_("Sets the page size at the start of level calculation"))
			PROPS_TYPE(CEditSize(100, INT_MAX), true)
		PROPS_PROPERTY_END

		PROPS_DEFINE_PROPERTY(SetCellSize, GetCellSize)
			PROPS_GROUP(t_("BG properties"))
			PROPS_NAME(t_("Mipmap size"))
			PROPS_HINT(t_("Sets the mipmap size"))
			PROPS_TYPE(CEditSize(100, INT_MAX), true)
		PROPS_PROPERTY_END

		PROPS_DEFINE_PROPERTY(SetZoomDX, GetZoomDX)
			PROPS_GROUP(t_("BG properties"))
			PROPS_NAME(t_("Zoom step (dx)"))
			PROPS_HINT(t_("Sets the zoom value between two zoom levels"))
			PROPS_TYPE(CEditDouble(1.0, (double)INT_MAX), true)
		PROPS_PROPERTY_END

		PROPS_DEFINE_PROPERTY(SetNull, GetNull)
			PROPS_GROUP(t_("BG properties"))
			PROPS_NAME("")
			PROPS_HINT(t_("Calculates mipmaps for the map background for zoom and translate"))
			PROPS_TYPE(CButton(t_("Calculate"), THISBACK(OnCalculate)), true)
		PROPS_PROPERTY_END
	PROPS_INIT_PROPERTIES_END

	PROPS_METHOD_SET(SetName, void, const char*);
	PROPS_METHOD_GET(GetName, String);

	PROPS_METHOD_SET(SetDesc, void, const char*);
	PROPS_METHOD_GET(GetDesc, String);

	PROPS_METHOD_SET(SetMapBG, void, const char*);
	PROPS_METHOD_GET(GetMapBG, String);

	PROPS_METHOD_SET(SetZoomDX, void, double);
	PROPS_METHOD_GET(GetZoomDX, double);

	PROPS_METHOD_SET(SetPageSize, void, const Size&);
	PROPS_METHOD_GET(GetPageSize, Size);

	PROPS_METHOD_SET(SetCellSize, void, const Size&);
	PROPS_METHOD_GET(GetCellSize, Size);

	PROPS_METHOD_SET(SetNull, void, const char*);
	PROPS_METHOD_GET(GetNull, String);

public:
	void   SetName(const char* s) { name = s; }
	String GetName() const { return name; }

	void   SetDesc(const char* s) { desc = s; }
	String GetDesc() const { return desc; }

	void   SetMapBG(const char* s) { mapPath = s; }
	String GetMapBG() const { return mapPath; }

	void   SetZoomDX(double n) { zoomDX = n; }
	double GetZoomDX() const { return zoomDX; }

	void   SetPageSize(const Size& sz) { pageSize = sz; }
	Size   GetPageSize() { return pageSize; }

	void   SetCellSize(const Size& sz) { cellSize = sz; }
	Size   GetCellSize() { return cellSize; }

	void   SetNull(const char* s) {}
	String GetNull() { return String(); }

	Array<Layer>& GetLayers() { return layers; }
	const Array<Layer>& GetLayers() const { return layers; }

	VectorMapEx<String, Value>& FillMapList(VectorMapEx<String, Value>& mapList)
	{
		WhenMapList(mapList);
		return mapList;
	}

	void OnCalculate() { WhenCalculate(*this); }
	Callback1<Level&> WhenCalculate;
	Callback1<VectorMapEx<String, Value>&> WhenMapList;

public:
	void Xmlize(XmlIO& xml)
	{
		xml ("name", name)
			("desc", desc)
			("zoomDX", zoomDX)
			("mapPath", mapPath)
			("pageSize", pageSize)
			("cellSize", cellSize)
			("layers", "layer", layers);
	}

public:
	Level(const Level& other)
	{
		pageSize = other.pageSize;
		cellSize = other.cellSize;
		zoomDX   = other.zoomDX;
		name     = other.name;
		desc     = other.desc;
		mapPath  = other.mapPath;
		layers <<= other.layers;
	}

private:
	Size pageSize;
	Size cellSize;
	double zoomDX;
	String name;
	String desc;
	String mapPath;
	Array<Layer> layers;
};

class Map : public MapItemProp
{
	typedef Map CLASSNAME;

public:
	virtual ~Map() {}

	PROPS_INIT_PROPERTIES
		PROPS_DEFINE_PROPERTY(SetName, GetName)
			PROPS_GROUP(t_("Map properties"))
			PROPS_NAME(t_("Name"))
			PROPS_HINT(t_("Sets the name of the map"))
			PROPS_TYPE(CEditField(CharFilterMapName), true)
		PROPS_PROPERTY_END
	PROPS_INIT_PROPERTIES_END

	PROPS_METHOD_SET(SetName, void, const char*);
	PROPS_METHOD_GET(GetName, String);

public:
	void   SetName(const char* s) { _name = s; }
	String GetName() const { return _name; }

	Array<Level>& GetLevels() { return _levels; }
	const Array<Level>& GetLevels() const { return _levels; }

public:
	void Xmlize(XmlIO& xml)
	{
		xml ("_name", _name)
			("_levels", "level", _levels);
	}

	void Clear()
	{
		_levels.Clear();
		_name  .Clear();
	}

private:
	Array<Level> _levels;
	String _name;
};

#endif
