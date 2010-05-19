#include "XMLConfig.h"

NAMESPACE_UPP

void XMLConfig::Set(const String& param, const String& value)
{
	Map.GetAdd(param).Data = value;
}

void XMLConfig::Set(const String& param, const char* value)
{
	Map.GetAdd(param).Data = String(value);
}

void XMLConfig::SetBool(const String& param, bool value)
{
	Set(param, value ? "true" : "false");
}

void XMLConfig::SetNumber(const String& param, int value)
{
	Set(param, AsString(value));
}

XMLValue& XMLConfig::GetValue(const String& param)
{
	return Map.Get(param);
}

const XMLValue& XMLConfig::GetValue(const String& param) const
{
	return Map.Get(param);
}

String XMLConfig::Get(const String& param) const
{
	return Map.Find(param) >= 0 ? Map.Get(param).Data : "";
}

String XMLConfig::Get(const String& param, const String& init)
{
	return Map.Find(param) >= 0 ? Map.Get(param).Data : Map.GetAdd(param).Data = init;
}

bool XMLConfig::GetBool(const String& param, bool init)
{
	String r = Get(param, init ? "1" : "0");
	return (r == "true"  || r == "1") ? true : ((r == "false" || r == "0") ? false : init);
}

int XMLConfig::GetNumber(const String& param, int init, int min, int max)
{
	int r = ScanInt( Get(param, AsString(init)) );
	if (r < min) { r = min; Set(param, AsString(init)); }
	if (r > max) { r = max; Set(param, AsString(init)); }
	return r;
}

void XMLConfig::ToggleBool(const String& param)
{
	SetBool(param, !GetBool(param));
}

void XMLConfig::Option(const String& param, const String& type)
{
	XMLValue* p = &Map.GetAdd(param);
	p->IsOption = true;
	p->Type     = type;
}

void XMLConfig::Remove(const String& param)
{
	int i = Map.Find(param);	
	if (i < 0) return;
	Map.Remove(i);
}

void XMLConfig::Clear()
{
	Name.Clear();
	Path.Clear();
	Map.Clear();
}

VectorMap<String, String> XMLConfig::GetValues() const
{
	VectorMap<String, String> result;
	for (int i = 0; i < Map.GetCount(); i++)
		if (!Map[i].IsOption)
			result.Add( Map.GetKey(i), Map[i].Data );
	return result;
}

VectorMap<String, String> XMLConfig::GetOptions() const
{
	VectorMap<String, String> result;
	for (int i = 0; i < Map.GetCount(); i++)
		if (Map[i].IsOption)
			result.Add( Map.GetKey(i), Map[i].Data );
	return result;
}

XMLConfig& XMLConfig::operator=(const XMLConfig& other)
{
	Name  = other.Name;
	Path  = other.Path;
	Map <<= other.Map;
	return *this;
}

void XMLConfig::Xmlize(XmlIO xml)
{
	xml("name", Name);

	XmlIO prop(xml.GetAdd("properties"));
	String param;

	if (xml.IsStoring())
		for(int i = 0; i < Map.GetCount(); i++)
		{
			param = Map.GetKey(i);
			Map[i].Xmlize( prop.Add("property").Attr("name", param) );
		}
	else
		for (int i = 0; i < prop->GetCount(); ++i)
		{
			const XmlNode* n = &prop->Node(i);
			Set(n->Attr("name"), n->Attr("value"));
			if (n->AttrInt("option", 0))
				Option(
					n->Attr("name"),
					n->Attr("type").IsEmpty()
						? "EditField"
						: n->Attr("type")
				);
		}
}

void XMLConfig::DumpXML()
{
	DUMP(Name);
	DUMP(Path);
	for (int i = 0; i < Map.GetCount(); i++)
		LOG(Map.GetKey(i) + " = " + Map[i].Data + (Map[i].IsOption ? " [Option]" : "") );
}

void XMLConfig::Compression(bool flag)
{
	_Compressed = flag;
}

bool XMLConfig::Load(const String& path)
{
	Path = path == "" ? GetFileTitle(GetExeFilePath()) : path;

	if (GetFileName(Path).Find('.') < 0)
		Path += _Compressed ? ".arc" : ".xml";

	if (!FileExists(Path))
	{
		LOG("ERROR: file not exists \"" + Path + "\"");
		return false;
	}

	if (_Compressed)
	{
		String s;
		s = ZDecompress(LoadFile(Path));
		return LoadFromXML(*this, s);
	}

	return LoadFromXMLFile(*this, Path);
}

bool XMLConfig::Save(const String& path)
{
	Path = path == "" ? GetFileTitle(GetExeFilePath()) : path;

	if (GetFileName(Path).Find('.') < 0)
		Path += _Compressed ? ".arc" : ".xml";

	if (_Compressed)
	{
		String s = StoreAsXML(*this, "data");
		return SaveFile(Path, ZCompress(s));
	}

	return StoreAsXMLFile(*this, "data", Path);
}

bool XMLConfig::NameExists(const String& param) const
{
	for (int i = 0; i < Map.GetCount(); ++i)
		if (Map.GetKey(i) == param)
			return true;
	return false;
}

END_UPP_NAMESPACE
