#include <CtrlLib/CtrlLib.h>

#include <Ole/Ole.h>
#include <WinCon.h>
#include <WinNetWk.h>
#include <ShlObj.h>		// shell objects like ICDBurn

struct ShellCDBurn : IRef<ICDBurn> {
	ShellCDBurn() {
		LogResult(Create(CLSID_CDBurn, CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER));
	}
};

ICDBurn *CreateICDBurn()
{
	ICDBurn *p;
	return SUCCEEDED(CoCreateInstance(CLSID_CDBurn, NULL,
	                                  CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER, IID_ICDBurn, (PVOID *) &p)) ? p : NULL;
}

bool ShellBurnCD(HWND hwnd = NULL)
{
	ICDBurn *p = CreateICDBurn();
	if(!p)
		return false;
	bool b = SUCCEEDED(p->Burn(hwnd));
	p->Release();
	return b;
}

String GetRecordableDriveLetter()
{
	ICDBurn *p = CreateICDBurn();
	if(!p)
		return Null;
	String ret;
	BOOL b = FALSE;
	WCHAR d[100]={0};
	if(SUCCEEDED(p->HasRecordableDrive(&b)) && b && SUCCEEDED(p->GetRecorderDriveLetter(d, 32)))
		ret = WString(d).ToString();
	p->Release();
	return ret;
}

String GetShellBurnFolderPath() {
	TCHAR path[MAX_PATH]={0};
	return SUCCEEDED(SHGetSpecialFolderPath(NULL, path, CSIDL_CDBURN_AREA, FALSE)) ? WString(path).ToString() : Null;
}

#include "Wininet.h"

int UploadFile(const String& url, const String& filename, const String& data) {
	INTERNET_BUFFERS BufferIn = {0};
	INTERNET_BUFFERS BufferOut = {0};
	BufferIn.dwStructSize = sizeof( INTERNET_BUFFERS );
	BufferOut.dwStructSize = sizeof( INTERNET_BUFFERS );

	HINTERNET hINet = InternetOpen("U++", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(!hINet)
		return GetLastError();

	int error = 0;

	HINTERNET hConnect = InternetConnect(hINet, url, INTERNET_DEFAULT_HTTPS_PORT,
	                                     NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	HINTERNET hRequest = NULL;

	if(hConnect) {
		for(;;) {
			if(!hRequest)
				hRequest = HttpOpenRequest(hConnect, "PUT", filename, NULL, NULL, NULL,
				                           INTERNET_FLAG_NO_CACHE_WRITE|INTERNET_FLAG_SECURE|
				                           INTERNET_FLAG_IGNORE_CERT_CN_INVALID,
				                           0);
// -------------------
			DWORD dwFlags;
			DWORD dwBuffLen;
			InternetQueryOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlags, &dwBuffLen);
			dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
			InternetSetOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, sizeof (dwFlags) );
// -------------------
			if(hRequest && HttpSendRequest(hRequest, NULL, 0, (void*)~data, data.GetLength()))
				break;
			error = GetLastError();
			if(error == ERROR_INTERNET_CLIENT_AUTH_CERT_NEEDED ||
			   error == ERROR_INTERNET_HTTP_TO_HTTPS_ON_REDIR ||
			   error == ERROR_INTERNET_INCORRECT_PASSWORD ||
			   error == ERROR_INTERNET_INVALID_CA ||
			   error == ERROR_INTERNET_POST_IS_NON_SECURE ||
			   error == ERROR_INTERNET_SEC_CERT_CN_INVALID ||
			   error == ERROR_INTERNET_SEC_CERT_DATE_INVALID) {
				if(InternetErrorDlg(::GetDesktopWindow(), hRequest, error,
				   FLAGS_ERROR_UI_FILTER_FOR_ERRORS |
				   FLAGS_ERROR_UI_FLAGS_GENERATE_DATA |
				   FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS,
				   NULL ) == ERROR_CANCELLED) {
		          		break;
				 }
				 else
				     error = 0;
			}
			else
				break;
		}
	}
	else
     	error = GetLastError();

	if(hRequest)
		InternetCloseHandle(hRequest);
	if(hConnect)
		InternetCloseHandle(hConnect);
	InternetCloseHandle(hINet);
	return error;
}

String InternetErrorMsg(int error)
{
	static struct {
		const char *text;
		int         error;
	}
	err[] = {
		{ "INTERNET_OUT_OF_HANDLES",           (INTERNET_ERROR_BASE + 1) },
		{ "INTERNET_TIMEOUT",                  (INTERNET_ERROR_BASE + 2) },
		{ "INTERNET_EXTENDED_ERROR",           (INTERNET_ERROR_BASE + 3) },
		{ "INTERNET_INTERNAL_ERROR",           (INTERNET_ERROR_BASE + 4) },
		{ "INTERNET_INVALID_URL",              (INTERNET_ERROR_BASE + 5) },
		{ "INTERNET_UNRECOGNIZED_SCHEME",      (INTERNET_ERROR_BASE + 6) },
		{ "INTERNET_NAME_NOT_RESOLVED",        (INTERNET_ERROR_BASE + 7) },
		{ "INTERNET_PROTOCOL_NOT_FOUND",       (INTERNET_ERROR_BASE + 8) },
		{ "INTERNET_INVALID_OPTION",           (INTERNET_ERROR_BASE + 9) },
		{ "INTERNET_BAD_OPTION_LENGTH",        (INTERNET_ERROR_BASE + 10) },
		{ "INTERNET_OPTION_NOT_SETTABLE",      (INTERNET_ERROR_BASE + 11) },
		{ "INTERNET_SHUTDOWN",                 (INTERNET_ERROR_BASE + 12) },
		{ "INTERNET_INCORRECT_USER_NAME",      (INTERNET_ERROR_BASE + 13) },
		{ "INTERNET_INCORRECT_PASSWORD",       (INTERNET_ERROR_BASE + 14) },
		{ "INTERNET_LOGIN_FAILURE",            (INTERNET_ERROR_BASE + 15) },
		{ "INTERNET_INVALID_OPERATION",        (INTERNET_ERROR_BASE + 16) },
		{ "INTERNET_OPERATION_CANCELLED",      (INTERNET_ERROR_BASE + 17) },
		{ "INTERNET_INCORRECT_HANDLE_TYPE",    (INTERNET_ERROR_BASE + 18) },
		{ "INTERNET_INCORRECT_HANDLE_STATE",   (INTERNET_ERROR_BASE + 19) },
		{ "INTERNET_NOT_PROXY_REQUEST",        (INTERNET_ERROR_BASE + 20) },
		{ "INTERNET_REGISTRY_VALUE_NOT_FOUND", (INTERNET_ERROR_BASE + 21) },
		{ "INTERNET_BAD_REGISTRY_PARAMETER",   (INTERNET_ERROR_BASE + 22) },
		{ "INTERNET_NO_DIRECT_ACCESS",         (INTERNET_ERROR_BASE + 23) },
		{ "INTERNET_NO_CONTEXT",               (INTERNET_ERROR_BASE + 24) },
		{ "INTERNET_NO_CALLBACK",              (INTERNET_ERROR_BASE + 25) },
		{ "INTERNET_REQUEST_PENDING",          (INTERNET_ERROR_BASE + 26) },
		{ "INTERNET_INCORRECT_FORMAT",         (INTERNET_ERROR_BASE + 27) },
		{ "INTERNET_ITEM_NOT_FOUND",           (INTERNET_ERROR_BASE + 28) },
		{ "INTERNET_CANNOT_CONNECT",           (INTERNET_ERROR_BASE + 29) },
		{ "INTERNET_CONNECTION_ABORTED",       (INTERNET_ERROR_BASE + 30) },
		{ "INTERNET_CONNECTION_RESET",         (INTERNET_ERROR_BASE + 31) },
		{ "INTERNET_FORCE_RETRY",              (INTERNET_ERROR_BASE + 32) },
		{ "INTERNET_INVALID_PROXY_REQUEST",    (INTERNET_ERROR_BASE + 33) },
		{ "INTERNET_NEED_UI",                  (INTERNET_ERROR_BASE + 34) },
		{ "INTERNET_HANDLE_EXISTS",            (INTERNET_ERROR_BASE + 36) },
		{ "INTERNET_SEC_CERT_DATE_INVALID",    (INTERNET_ERROR_BASE + 37) },
		{ "INTERNET_SEC_CERT_CN_INVALID",      (INTERNET_ERROR_BASE + 38) },
		{ "INTERNET_HTTP_TO_HTTPS_ON_REDIR",   (INTERNET_ERROR_BASE + 39) },
		{ "INTERNET_HTTPS_TO_HTTP_ON_REDIR",   (INTERNET_ERROR_BASE + 40) },
		{ "INTERNET_MIXED_SECURITY",           (INTERNET_ERROR_BASE + 41) },
		{ "INTERNET_CHG_POST_IS_NON_SECURE",   (INTERNET_ERROR_BASE + 42) },
		{ "INTERNET_POST_IS_NON_SECURE",       (INTERNET_ERROR_BASE + 43) },
		{ "INTERNET_CLIENT_AUTH_CERT_NEEDED",  (INTERNET_ERROR_BASE + 44) },
		{ "INTERNET_INVALID_CA",               (INTERNET_ERROR_BASE + 45) },
		{ "INTERNET_CLIENT_AUTH_NOT_SETUP",    (INTERNET_ERROR_BASE + 46) },
		{ "INTERNET_ASYNC_THREAD_FAILED",      (INTERNET_ERROR_BASE + 47) },
		{ "INTERNET_REDIRECT_SCHEME_CHANGE",   (INTERNET_ERROR_BASE + 48) },
		{ "INTERNET_DIALOG_PENDING",           (INTERNET_ERROR_BASE + 49) },
		{ "INTERNET_RETRY_DIALOG",             (INTERNET_ERROR_BASE + 50) },
		{ "INTERNET_HTTPS_HTTP_SUBMIT_REDIR",  (INTERNET_ERROR_BASE + 52) },
		{ "INTERNET_INSERT_CDROM",             (INTERNET_ERROR_BASE + 53) },
		{ "INTERNET_FORTEZZA_LOGIN_NEEDED",    (INTERNET_ERROR_BASE + 54) },
		{ "INTERNET_SEC_CERT_ERRORS",          (INTERNET_ERROR_BASE + 55) },
		{ "INTERNET_SEC_CERT_NO_REV",          (INTERNET_ERROR_BASE + 56) },
		{ "INTERNET_SEC_CERT_REV_FAILED",      (INTERNET_ERROR_BASE + 57) },
		{ "FTP_TRANSFER_IN_PROGRESS",          (INTERNET_ERROR_BASE + 110) },
		{ "FTP_DROPPED",                       (INTERNET_ERROR_BASE + 111) },
		{ "FTP_NO_PASSIVE_MODE",               (INTERNET_ERROR_BASE + 112) },
		{ "GOPHER_PROTOCOL_ERROR",             (INTERNET_ERROR_BASE + 130) },
		{ "GOPHER_NOT_FILE",                   (INTERNET_ERROR_BASE + 131) },
		{ "GOPHER_DATA_ERROR",                 (INTERNET_ERROR_BASE + 132) },
		{ "GOPHER_END_OF_DATA",                (INTERNET_ERROR_BASE + 133) },
		{ "GOPHER_INVALID_LOCATOR",            (INTERNET_ERROR_BASE + 134) },
		{ "GOPHER_INCORRECT_LOCATOR_TYPE",     (INTERNET_ERROR_BASE + 135) },
		{ "GOPHER_NOT_GOPHER_PLUS",            (INTERNET_ERROR_BASE + 136) },
		{ "GOPHER_ATTRIBUTE_NOT_FOUND",        (INTERNET_ERROR_BASE + 137) },
		{ "GOPHER_UNKNOWN_LOCATOR",            (INTERNET_ERROR_BASE + 138) },
		{ "HTTP_HEADER_NOT_FOUND",             (INTERNET_ERROR_BASE + 150) },
		{ "HTTP_DOWNLEVEL_SERVER",             (INTERNET_ERROR_BASE + 151) },
		{ "HTTP_INVALID_SERVER_RESPONSE",      (INTERNET_ERROR_BASE + 152) },
		{ "HTTP_INVALID_HEADER",               (INTERNET_ERROR_BASE + 153) },
		{ "HTTP_INVALID_QUERY_REQUEST",        (INTERNET_ERROR_BASE + 154) },
		{ "HTTP_HEADER_ALREADY_EXISTS",        (INTERNET_ERROR_BASE + 155) },
		{ "HTTP_REDIRECT_FAILED",              (INTERNET_ERROR_BASE + 156) },
		{ "HTTP_NOT_REDIRECTED",               (INTERNET_ERROR_BASE + 160) },
		{ "HTTP_COOKIE_NEEDS_CONFIRMATION",    (INTERNET_ERROR_BASE + 161) },
		{ "HTTP_COOKIE_DECLINED",              (INTERNET_ERROR_BASE + 162) },
		{ "HTTP_REDIRECT_NEEDS_CONFIRMATION",  (INTERNET_ERROR_BASE + 168) },
		{ "INTERNET_SECURITY_CHANNEL_ERROR",   (INTERNET_ERROR_BASE + 157) },
		{ "INTERNET_UNABLE_TO_CACHE_FILE",     (INTERNET_ERROR_BASE + 158) },
		{ "INTERNET_TCPIP_NOT_INSTALLED",      (INTERNET_ERROR_BASE + 159) },
		{ "INTERNET_DISCONNECTED",             (INTERNET_ERROR_BASE + 163) },
		{ "INTERNET_SERVER_UNREACHABLE",       (INTERNET_ERROR_BASE + 164) },
		{ "INTERNET_PROXY_SERVER_UNREACHABLE", (INTERNET_ERROR_BASE + 165) },
		{ "INTERNET_BAD_AUTO_PROXY_SCRIPT",    (INTERNET_ERROR_BASE + 166) },
		{ "INTERNET_UNABLE_TO_DOWNLOAD_SCRIPT", (INTERNET_ERROR_BASE + 167) },
		{ "INTERNET_SEC_INVALID_CERT",         (INTERNET_ERROR_BASE + 169) },
		{ "INTERNET_SEC_CERT_REVOKED",         (INTERNET_ERROR_BASE + 170) },
		{ "INTERNET_FAILED_DUETOSECURITYCHECK",  (INTERNET_ERROR_BASE + 171) },
		{ "INTERNET_NOT_INITIALIZED",          (INTERNET_ERROR_BASE + 172) },
		{ "INTERNET_NEED_MSN_SSPI_PKG",          (INTERNET_ERROR_BASE + 173) },
		{ "INTERNET_LOGIN_FAILURE_DISPLAY_ENTITY_BODY",   (INTERNET_ERROR_BASE + 174) },
	};
	String x = AsString(error);
	for(int i = 0; i < __countof(err); i++)
		if(err[i].error == error)
			return x + ": "+ err[i].text;
	return x;
};

GUI_APP_MAIN
{
	CoInitialize(NULL);
/*	PromptOK(DeQtf(GetShellBurnFolderPath()) + "&" +
	         DeQtf(GetRecordableDriveLetter()));
	SaveFile(AppendFileName(GetShellBurnFolderPath(), "test.txt"), "Hello!");
	ShellBurnCD();
*/

	int err = UploadFile("localhost", "/dotazniky/rsa_c.txt", LoadFile("d:/rsa_c.txt"));
	DUMP(err);
	PromptOK(InternetErrorMsg(err));

	CoUninitialize();
}
