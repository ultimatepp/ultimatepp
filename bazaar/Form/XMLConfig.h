#ifndef XML_CONFIG_H
#define XML_CONFIG_H

#include <Core/Core.h>

NAMESPACE_UPP

struct XMLValue : Moveable<XMLValue>
{
	XMLValue(const String& data = "", bool option = false)
		: Data(data), Type("FieldEdit"), IsOption(option)
	{}

	void Xmlize(XmlIO xml)
	{
		xml.Attr("value", Data);

		if (xml.IsStoring())
		{
			if (IsOption)
			{
				xml.Attr("option", IsOption);
				xml.Attr("type", Type);
			}
		}
		else
		{
			xml.Attr("option", IsOption);
			xml.Attr("type", Type);
		}
	}

	String Data;
	String Type;
	bool IsOption;
};

class XMLConfig : Moveable<XMLConfig>
{
public:
	 XMLConfig(const String& name = "", bool cmp = false) : Name(name), _Compressed(cmp) {}
	~XMLConfig() { Clear(); }

	virtual void Xmlize(XmlIO xml);

	bool Load(const String& path = "");
	bool Save(const String& path = "");

	void Set(const String& param, const String& value);
	void Set(const String& param, const char* value);
	void SetBool(const String& param, bool value);
	void SetNumber(const String& param, int value);

	void ToggleBool(const String& param);

	String Get(const String& param) const;
	String Get(const String& param, const String& init);
	bool   GetBool(const String& param, bool init = false);
	int    GetNumber(const String& param, int init = 0, int min = INT_MIN, int max = INT_MAX);

	XMLValue& GetValue(const String& param);
	const XMLValue& GetValue(const String& param) const;

	void Option(const String& param, const String& type = "EditField");
	void Remove(const String& param);
	void DumpXML();
	void Clear();

	bool NameExists(const String& param) const;

	int GetCount() const { return Map.GetCount(); }

	XMLConfig& operator=(const XMLConfig& other);

	VectorMap<String, String> GetValues() const;
	VectorMap<String, String> GetOptions() const;

	void Compression(bool flag = true);
	bool IsCompressed() const { return _Compressed; }

	String Name;
	String Path;
	VectorMap<String, XMLValue> Map;
	Callback2<int, int> WhenProgressBar;

private:
	bool _Compressed;
};

END_UPP_NAMESPACE

#endif // .. XML_CONFIG_H
