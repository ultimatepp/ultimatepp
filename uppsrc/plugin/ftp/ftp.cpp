#include "ftp.h"
#include "lib/ftplib.h"

namespace Upp {

#define LLOGBLOCK(x)  // LOGBLOCK(x)
#define LLOG(x)       // DLOG(x)

static bool ftpinit = false;

FtpClient::FtpClient()
{
	ftpconn = NULL;
	if(!ftpinit) {
		INTERLOCKED {
			if(!ftpinit) {
				FtpInit();
				ftpinit = true;
			}
		}
	}
}

FtpClient::~FtpClient()
{
	Close();
}

bool FtpClient::IsOpen() const
{
	return ftpconn;
}

bool FtpClient::Connect(const char *host, const char *user, const char *password, bool pasv, int idletimeout_secs)
{
	LOGBLOCK("FtpClient::Connect");
	Close();
	char perror[512];
	LLOG("FtpConnect(" << host << ")");
	if(WhenProgress(0,0)) {
		error = "connect aborted";
		return false;
	}
	if(!FtpConnect(host, &ftpconn, perror, &FtpClient::Callback, this, 200, idletimeout_secs)) {
		error = perror;
		return false;
	}
	LLOG("FtpLogin(" << user << ", " << password << ")");
	if(!FtpLogin(user, password, ftpconn)) {
		error = FtpError(ftpconn);
		Close();
		return false;
	}
	LLOG("FtpOptions(pasv = " << pasv << ")");
	if(!FtpOptions(FTPLIB_CONNMODE, pasv ? FTPLIB_PASSIVE : FTPLIB_PORT, ftpconn)) {
		error = FtpError(ftpconn);
		Close();
		return false;
	}
	return true;
}

int FtpClient::Callback(netbuf *nControl, int xfered, void *arg)
{
	FtpClient *ftp = (FtpClient *)arg;
	return !ftp->WhenProgress(xfered, ftp->save_total);
}

void FtpClient::Close()
{
	if(ftpconn) {
		FtpQuit(ftpconn);
		ftpconn = NULL;
	}
}

bool FtpClient::CheckOpen()
{
	if(!IsOpen()) {
		error = t_("not connected");
		return false;
	}
	return true;
}

String FtpClient::Load(const char *path)
{
	LLOGBLOCK("FtpClient::Load");
	if(!CheckOpen())
		return String::GetVoid();
	netbuf *ftpdata;
	LLOG("FtpAccess(" << path << ")");
	if(WhenProgress(0,0)) {
		error = t_("aborted");
		return String::GetVoid();
	}
	if(!FtpAccess(path, FTPLIB_FILE_READ, FTPLIB_IMAGE, ftpconn, &ftpdata)) {
		error = FtpError(ftpconn);
		return String::GetVoid();
	}
	load_data = Null;
	while(!WhenProgress(load_data.GetLength(), 0)) {
		byte buffer[1024];
		int ndata = FtpRead(buffer, sizeof(buffer), ftpdata);
		LLOG("FtpRead -> " << ndata);
		if(ndata < 0) {
			error = FtpError(ftpdata);
			FtpClose(ftpdata);
			return String::GetVoid();
		}
		if(ndata == 0) {
			load_data.Shrink();
			error = FtpError(ftpdata);
			break;
		}
		load_data.Cat(buffer, ndata);
#ifdef SLOWTRANSFER
		int end = GetTickCount() + SLOWTRANSFER;
		for(int d; (d = end - GetTickCount()) > 0; Sleep(d))
			;
#endif
	}
	FtpClose(ftpdata);
	return load_data;
}

bool FtpClient::Save(const char *path, String data)
{
	return SaveCount(path, data) == data.GetLength();
}

int FtpClient::SaveCount(const char *path, String data)
{
	LLOGBLOCK("FtpClient::Save");
	netbuf *ftpdata;
	save_pos = 0;
	save_total = data.GetLength();
	if(!CheckOpen())
		return 0;
	LLOG("FtpAccess(" << path << ")");
	if(WhenProgress(0,data.GetLength()))
		return 0;
	if(!FtpAccess(path, FTPLIB_FILE_WRITE, FTPLIB_IMAGE, ftpconn, &ftpdata)) {
		error = FtpError(ftpconn);
		return 0;
	}
	while(save_pos < data.GetLength()) {
		if(WhenProgress(save_pos, data.GetLength())) {
			error = NFormat(t_("write aborted after %d bytes(s)"), save_pos);
			FtpClose(ftpdata);
			return save_pos;
		}
		int chunk = min(data.GetLength() - save_pos, 1024);
		int ndata = FtpWrite((void *)data.GetIter(save_pos), chunk, ftpdata);
		LLOG("FtpWrite(" << chunk << ") -> " << ndata);
		if(ndata <= 0 || ndata > chunk) {
			error = FtpError(ftpdata);
			FtpClose(ftpdata);
			return save_pos;
		}
		save_pos += ndata;
#ifdef SLOWTRANSFER
		int end = GetTickCount() + SLOWTRANSFER;
		for(int d; (d = end - GetTickCount()) > 0; Sleep(d))
			;
#endif
	}
	WhenProgress(save_pos, data.GetLength());
	LLOG("FtpClose");
	FtpClose(ftpdata);
	return save_pos;
}

bool  FtpClient::Exists(const char *path) {
	LLOGBLOCK("FtpClient::Exists");
	if(!CheckOpen())
		return false;
	netbuf *data;
	LLOG("FtpAccess(" << path << ")");
	if(!FtpAccess(path, FTPLIB_FILE_READ, FTPLIB_IMAGE, ftpconn, &data)) {
		error = FtpError(ftpconn);
		return false;
	}
	FtpClose(data);
	return true;
}

bool FtpClient::Rename(const char *oldpath, const char *newpath) {
	LLOGBLOCK("FtpClient::Rename");
	LLOG("FtpRename(oldname = " << oldpath << ", newname = " << newpath << ")");
	return CheckOpen() && !!FtpRename(oldpath, newpath, ftpconn);
}

bool FtpClient::Cd(const char *path) {
	LLOGBLOCK("FtpClient::Cd");
	LLOG("FtpChdir(" << path << ")");
	return CheckOpen() && !!FtpChdir(path, ftpconn);
}

bool FtpClient::MkDir(const char *path) {
	LLOGBLOCK("FtpClient::MkDir");
	LLOG("FtpMkdir(" << path << ")");
	return CheckOpen() && !!FtpMkdir(path, ftpconn);
}

bool FtpClient::RmDir(const char *path) {
	LLOGBLOCK("FtpClient::RmDir");
	LLOG("FtpRmdir(" << path << ")");
	return CheckOpen() && !!FtpRmdir(path, ftpconn);
}

bool FtpClient::Delete(const char *path) {
	LLOGBLOCK("FtpClient::Delete");
	LLOG("FtpDelete(" << path << ")");
	return CheckOpen() && !!FtpDelete(path, ftpconn);
}

String FtpClient::List(const char *path)
{
	LLOGBLOCK("FtpClient::List");
	load_data = Null;
	if(!CheckOpen())
		return String::GetVoid();
	netbuf *ftpdata;
	LLOG("FtpAccess(" << path << ")");
	if(WhenProgress(0,0)) {
		error = t_("aborted");
		return String::GetVoid();
	}
	if(!FtpAccess(path, FTPLIB_DIR, FTPLIB_ASCII, ftpconn, &ftpdata)) {
		error = FtpError(ftpconn);
		return String::GetVoid();
	}
	while(!WhenProgress(load_data.GetLength(),0)) {
		byte buffer[1024];
		int ndata = FtpRead(buffer, sizeof(buffer), ftpdata);
		LLOG("FtpRead -> " << ndata);
		if(ndata < 0) {
			error = FtpError(ftpdata);
			FtpClose(ftpdata);
			return String::GetVoid();
		}
		if(ndata == 0) {
			load_data.Shrink();
			break;
		}
		load_data.Cat(buffer, ndata);
	#ifdef SLOWTRANSFER
		int end = GetTickCount() + SLOWTRANSFER;
		for(int d; (d = end - GetTickCount()) > 0; Sleep(d))
			;
	#endif
	}
	FtpClose(ftpdata);
	return load_data;
}

void FtpClient::RealizePath(const char *path)
{
	LLOGBLOCK("FtpClient::RealizePath");
	const char *s = path;
	if(*s == '\0') return;
	for(;;) {
		s = strchr(s + 1, '/');
		if(!s) break;
		MkDir(String(path, s));
	}
}

String FtpClientGet(String url, String *error)
{
	const char *u = url, *t = u;
	while(*t && *t != '?')
		if(*t++ == '/') {
			if(*t == '/')
				u = ++t;
			break;
		}
	t = u;
	while(*u && *u != '@' && *u != '/')
		u++;
	String host = String(t, u);
	String user, pwd;
	if(*u == '@') {
		t = ++u;
		while(*u && *u != '/' && *u != ':')
			u++;
		user = String(t, u);
		if(*u == ':') {
			t = ++u;
			while(*u && *u != '/')
				u++;
			pwd = String(t, u);
		}
	}
	FtpClient ftp;
	if(!ftp.Connect(host, user, pwd)) {
		if(error)
			*error = ftp.GetError();
		return String::GetVoid();
	}
	String data = ftp.Load(u);
	if(data.IsVoid()) {
		if(error)
			*error = ftp.GetError();
	}
	return data;
}

}
