class WebUser
{
public:
	WebUser(String name = Null, String pass = Null, String args = Null)
		: name(name), pass(pass), args(args) {}

	void                   Serialize(Stream& stream);

	// low-level
	static String          CheckToken(String token);
	static String          CreateToken(String args);
	static void            DeleteToken(String token);

	// high-level
	static void            SetMap(const Array<WebUser>& map);
	static Array<WebUser>  GetMap();
	static void            SerializeMap(String& s, bool write);
	static void            ClearMap();

	void                   SetUser() const;
	String                 GetArgs() const;

	static String          Login(const HttpQuery& map, String& args,
		const char *isapi_name = NULL, bool *save_cfg = 0);

public:
	String                 name;
	String                 pass;
	String                 args;
};
