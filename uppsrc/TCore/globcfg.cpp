#include "TCore.h"

NAMESPACE_UPP

ConfigHeading::ConfigHeading()
{
	version = 0;
	minor   = 0;
	major   = -1;
}

ConfigHeading::ConfigHeading(const char *key, int version, int minor, int major)
{
	this->key     = key;
	this->version = version;
	this->minor   = minor;
	this->major   = major;
}

void ConfigHeading::Serialize(Stream& stream)
{
	if(!stream.IsLoading() || !stream.IsEof())
		stream % key / version;
}

bool ConfigHeading::Matches(const ConfigHeading& heading) const
{
	return key == heading.key && version >= heading.minor && version <= heading.major;
}

ConfigItem::ConfigItem()
: current_version(0)
, save_version(0)
{
}

ConfigItem::ConfigItem(const char *key, int version, int minor, int major,
					   Configuration* configuration)
: ConfigHeading(key, 0, minor, major)
, current_version(0)
, save_version(version)
{
	(configuration ? configuration : &Configuration::Main())->Add(*this);
}

void ConfigItem::Serialize(Stream& stream)
{
	ConfigHeading::Serialize(stream);
	stream % data;
}

void ConfigItem::Prepare(bool storing)
{
	current_version = (storing ? save_version : version);
}

void ConfigItem::Clear()
{
	version = 0;
	data.Clear();
}

bool ConfigItem::operator = (const ConfigItem& item)
{
	if(!item.Matches(*this))
		return false;
	data = item.data;
	version = item.version;
	return true;
}

ConfigStream::ConfigStream(ConfigItem& item, bool storing)
: item(item)
{
	item.Prepare(storing);
	if(!storing)
		Open(item.data);
}

ConfigStream::~ConfigStream()
{
	if(IsStoring())
	{
		item.version = item.save_version;
		item.data = GetResult();
	}
}

Configuration::Configuration(const char *key, int version, int minor, int major)
: ConfigHeading(key, version, minor, major)
{
}

void Configuration::Add(ConfigItem& item)
{
	ASSERT(items.Find(item.key) < 0); // duplicate configuration items
	items.Add(item.key, &item);
}

void Configuration::Serialize(Stream& stream)
{
	item_error = false;

	if(stream.IsLoading()) { // clear configuration items
		for(ConfigItem **p = items.Begin(), **e = items.End(); p < e; p++)
			(*p)->Clear();
		if(stream.IsEof())
			return; // empty configuration file
	}

	ConfigHeading temp = *this;
	stream % temp;
	if(!temp.Matches(*this)) { // whole configuration is invalid
		stream.SetError();
		return;
	}

	int item_count = items.GetCount();
	stream % item_count;

	for(int i = 0; i < item_count; i++)
		if(stream.IsLoading()) {
			ConfigItem item;
			stream % item;
			int index = items.Find(item.key);
			if(index >= 0 && !(*items[index] = item))
				item_error = true;
		}
		else
			stream % *items[i];
}

Configuration& Configuration::Main()
{
	static Configuration main_configuration("Application configuration", 1, 1, 1);
	return main_configuration;
}

END_UPP_NAMESPACE
