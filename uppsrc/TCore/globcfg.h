NAMESPACE_UPP

class Configuration;
class ConfigItem;

class ConfigHeading
{
public:
	ConfigHeading();
	ConfigHeading(const char *key, int version, int minor, int major);

	void   Serialize(Stream& stream);
	bool   Matches(const ConfigHeading& heading) const;

public:
	String key;
	int    version;
	int    minor;
	int    major;
};

class ConfigItem : protected ConfigHeading
{
	friend class Configuration;
	friend class ConfigStream;

public:
	ConfigItem();
	ConfigItem(const char *key, int version, int minor, int major,
		Configuration* cfg = NULL); // NULL = AppCfg()

	bool   operator = (const ConfigItem& item);

	void   Prepare(bool storing);
	void   Clear();

	void   Serialize(Stream& stream);

	int    GetVersion() const { return current_version; }

	bool   operator <  (int version) const { return current_version <  version; }
	bool   operator <= (int version) const { return current_version <= version; }
	bool   operator == (int version) const { return current_version == version; }
	bool   operator != (int version) const { return current_version != version; }
	bool   operator >= (int version) const { return current_version >= version; }
	bool   operator >  (int version) const { return current_version >  version; }

protected:
	int    save_version;
	int    current_version;
	String data;
};

#define CONFIG_ITEM(var, key, version, minor, major) \
	GLOBAL_VARP(ConfigItem, var, (key, version, minor, major)) \
	INITBLOCK { var(); }

class ConfigStream : public StringStream
{
public:
	ConfigStream(ConfigItem& item, bool storing);
	~ConfigStream();

protected:
	ConfigItem& item;
};

template <class T>
bool ConfigureItem(T& object, ConfigItem& item, bool storing)
{
	ConfigStream stream(item, storing);
	if(stream.IsLoading() && stream.IsEof())
	 	return true; // empty configuration is OK
	stream % object;
	return !stream.IsError();
}

template <class T>
inline bool ReadConfigItem(T& object, ConfigItem& item)
{ return ConfigureItem(object, item, false); }

template <class T>
inline bool WriteConfigItem(const T& object, ConfigItem& item)
{ return ConfigureItem(const_cast<T&>(object), item, true); }

#define ReadConfigSelf()  ReadConfigItem(*this, config())
#define WriteConfigSelf() WriteConfigItem(*this, config())

class Configuration : protected ConfigHeading
{
public:
	Configuration(const char *key, int version, int minor, int major);

	void                  Add(ConfigItem& item);
	void                  Serialize(Stream& stream);

	static Configuration& Main();

	bool                  IsItemError() const { return item_error; }

protected:
	VectorMap<String, ConfigItem*> items;
	bool                  item_error;
};

END_UPP_NAMESPACE
