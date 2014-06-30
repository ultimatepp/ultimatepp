struct KeyInfo {
	const char *name;
	dword key[4];
};

NTL_MOVEABLE(KeyInfo)

void RegisterKeyBinding(const char *group, const char *id, KeyInfo& (*info)());

KeyInfo& AK_NULL();

void   EditKeys();

String StoreKeys();
void   RestoreKeys(const String& data);

dword ParseKeyDesc(CParser& p);

String GetDesc(const KeyInfo& f, bool parenthesis = true);
