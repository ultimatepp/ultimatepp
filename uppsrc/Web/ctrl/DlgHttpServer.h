//////////////////////////////////////////////////////////////////////

using namespace Upp;

class DlgServerGui;

class DlgHttpServer
{
public:
	DlgHttpServer();
	~DlgHttpServer();

	void              AddServer(HttpServer& server);
	void              RemoveServer(HttpServer& server);
	int               GetServerCount() const;
	HttpServer&       GetServer(int i) const;

	void              Title(const char *t);
	void              Icon(Image small_icon, Image large_icon);
	void              Icon(Image small_icon) { Icon(small_icon, small_icon); }

	void              Minimize(bool min = true);
	bool              IsMinimized() const;

	void              Position(Point pt);
	Point             GetPosition() const;

	bool              Wait(int msec);
	One<HttpRequest>  GetRequest();

private:
	One<DlgServerGui> gui;
};

//////////////////////////////////////////////////////////////////////
