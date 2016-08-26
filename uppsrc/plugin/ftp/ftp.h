#ifndef _plugin_ftp_ftp_h_
#define _plugin_ftp_ftp_h_

#include <Core/Core.h>

typedef struct NetBuf netbuf;

namespace Upp {

class FtpClient {
public:
	FtpClient();
	~FtpClient();

	bool    Connect(const char *host, const char *user = NULL, const char *password = NULL,
		bool pasv = true, int idletimeout_secs = 30);
	void    Close();
	bool    IsOpen() const;

	bool    Save(const char *path, String data);
	int     SaveCount(const char *path, String data);
	String  Load(const char *path);
	String  List(const char *path);

	String  GetLoadedPart() const { return load_data; }
	int     GetSavePos() const    { return save_pos; }
	int     GetSaveTotal() const  { return save_total; }

	bool    Rename(const char *oldpath, const char *newpath);
	bool    Cd(const char *path);
	bool    MkDir(const char *path);
	bool    RmDir(const char *path);
	bool    Delete(const char *path);
	bool    Exists(const char *path);
	void    RealizePath(const char *path);

	void    SetError(String s) { error = s; }
	String  GetError() const   { return error; }
	void    ClearError()       { error.Clear(); }

	bool    CheckOpen();

public:
	Gate2<int, int>    WhenProgress;

private:
	static int Callback(netbuf *nControl, int xfered, void *arg);

private:
	netbuf *ftpconn;
	String  error;
	String  load_data;
	int     save_pos;
	int     save_total;
};

String FtpClientGet(String url, String *error = NULL);

}

#endif
