#include <Web/Web.h>
#include "version.h"
#include <plugin/bz2/bz2.h>
#ifdef PLATFORM_POSIX
#include <signal.h>
#endif//PLATFORM_POSIX

using namespace Upp;

enum { DEFAULT_PORT = 2346 };

#ifdef _DEBUG
#define DO_SVRLOG 1
#else
#define DO_SVRLOG 0
#endif

#if DO_SVRLOG
#define SVRLOG(x) { StringStream ss; ss << x << '\n'; RLOG(ss.GetResult()); fputs(ss.GetResult(), stderr); fflush(stderr); }
#else
#define SVRLOG(x)
#endif

/*
void DeleteFolderDeep(const char *dir)
{
	FindFile ff(String(dir) + "/*.*");
	while(ff) {
		String name = ff.GetName();
		String p = AppendFileName(dir, name);
		if(ff.IsFile())
			DeleteFile(p);
		else
		if(ff.IsFolder())
			DeleteFolderDeep(p);
		ff.Next();
	}
	DirectoryDelete(dir);
}
*/

static bool quit_signal = false;

static void QuitSignal(int)
{
	puts("Termination signal received, quitting...");
	quit_signal = true;
}

class CommandServer
{
public:
	typedef CommandServer CLASSNAME;
	CommandServer();
	virtual ~CommandServer();

	CommandServer&  Logging(bool l)                { logging = l; return *this; }
	CommandServer&  Timing(bool t)                 { timing = t; return *this; }
	CommandServer&  Filter(const char *ip);

	void            Execute(Socket& socket);
	bool            StartServer(int port, const String& cmdline);
	void            StopServer(int port);
	void            RunServer(int port);
	void            RunClient();
	void            Quit()                         { quit_flag = true; }

protected:
	String          GetConInput(const char *prompt);
	virtual void    ExecTimer();
//	void            RunCommand(String cmd, Socket& socket);
	String          CheckFiles(const char *f);
	String          FileRemove(const char *f);
	String          DeleteFolderDeep(const char *f);
	String          UploadFiles(const char *f);
	String          DownloadFiles(const char *f);
	String          CreateDir(const char *f);
	String          GetFileInfo(const char *f);

protected:
	class Connection
	{
	public:
		Connection(Socket socket) : socket(socket), eof(false) {}
		virtual ~Connection();

		bool              IsEof() const   { return eof; }
		String            GetName() const { return name; }
		virtual bool      Run() = 0;

	protected:
		String            name;
		Socket            socket;
		bool              eof;
	};

	class CommandConnection : public Connection
	{
	public:
		CommandConnection(Socket& socket, const char *command);

		virtual bool      Run();

	private:
		One<SlaveProcess> slave;
	};

	Array<Connection> connections;
	int               timer_chunk;
	bool              logging;
	bool              timing;
	bool              quit_flag;
	Index<dword>      ip_filter;
//	int             thread_count;
};

//////////////////////////////////////////////////////////////////////
// CommandServer::Connection::

CommandServer::Connection::~Connection()
{
	if(socket.IsOpen())
		socket.StopWrite();
}

//////////////////////////////////////////////////////////////////////
// CommandServer::CommandConnection::

CommandServer::CommandConnection::CommandConnection(Socket& _socket, const char *_command)
: Connection(_socket)
{
	_socket.Clear();
	
	try {
		String environment;
		String pathlist;
		if(*_command == ':') {
			const char *b = ++_command;
			while(*_command && *_command++ != '\n')
				;
			environment = ASCII85Decode(String(b, _command));
			for(b = environment; *b; b += strlen(b) + 1)
				if(!MemICmp(b, "PATH=", 5)) {
					pathlist = b + 5;
					break;
				}
		}
		if(*_command == '=')
			_command++;
		String command = _command;
		if(!IsNull(pathlist)) {
			String exec;
			if(*_command == '\"') {
				while(*++_command && (*_command != '\"' || *++_command == '\"'))
					exec.Cat(*_command);
			}
			else
				while(*_command && (byte)*_command > ' ')
					exec.Cat(*_command++);
			command = GetFileOnPath(exec, pathlist, true);
	#ifdef PLATFORM_WIN32
			if(IsNull(command))
				command = GetFileOnPath(exec + ".exe", pathlist, true);
	#endif
			if(IsNull(command))
				command = exec;
			if(command.Find(' ') >= 0)
				command = '\"' + command + '\"';
			command << ' ' << _command;
		}
		LOG("CommandServer::CommandConnection(" << command << ")");
		slave = StartProcess(command, environment);
		LOG("CommandServer::CommandConnection -> OK");
		socket.Write("+\0", 2);
		name << "[command]" << command;
	}
	catch(Exc e) {
		if(socket.IsOpen())
			socket.Write('-' + e + '\0');
		throw;
	}
}

bool CommandServer::CommandConnection::Run()
{
//	SVRLOG("CommandConnection::Run");
	if(eof)
		return true;
	ASSERT(socket.IsOpen());
	bool work = false;
	String inp = socket.ReadUntil('\0', 0, 10000000);
	if(inp.IsVoid())
	{ // broken pipe
		slave -> Kill();
		eof = true;
		return true;
	}
	if(!inp.IsEmpty())
	{
		SVRLOG("-> input data: <" << inp << ">");
		work = true;
		slave -> Write(inp);
	}
	String out = Null;
	eof = !slave -> Read(out);
	if(!out.IsEmpty())
	{
		work = true;
		SVRLOG("-> output data: <" << out << ">");
		socket.Write(out);
	}
	if(eof)
	{
		work = true;
		socket.Write("\0", 1);
		SVRLOG("Process terminated, exit code = " << slave -> GetExitCode());
		String ec = IntStr(slave -> GetExitCode());
		socket.Write(ec, ec.GetLength() + 1);
		socket.StopWrite();
	}
//	SVRLOG("/ CommandConnection::Run -> " << (work ? "working" : "idle"));
	return work;
}

//////////////////////////////////////////////////////////////////////
// CommandServer::

CommandServer::CommandServer()
{
	timer_chunk = 100;
	quit_flag = true;
	Logging(false);
	Timing(false);
}

CommandServer::~CommandServer()
{
}

CommandServer& CommandServer::Filter(const char *ip)
{
	Socket::Init();
	hostent *he = gethostbyname(ip);
	if(he)
		ip_filter.FindAdd(Peek32be((in_addr *)(he -> h_addr_list[0])));
	else
		fputs(NFormat("gethostbyname: adresa '%s' nebyla nalezena ...\n", ip), stderr);
	return *this;
}

String CommandServer::CheckFiles(const char *f)
{
	int line = 0;
	String out = "OK\n";
	while(*f)
	{
		line++;
		const char *b = f;
		while(*f && *f != '\n' && *f != '\t')
			f++;
		String fn = NativePath(String(b, f));
		int t;
		if(*f != '\t' || IsNull(t = ScanInt(f + 1, &f)))
			return NFormat("ERROR(%s): file time expected at line %d", fn, line);

		Time time = Time(2000, 1, 1) + t;
		int l;
		if(*f != '\t' || IsNull(l = ScanInt(f + 1, &f)))
			return NFormat("ERROR(%s): file size expected at line %d", fn, line);

			FindFile ff(fn);
			if(ff && tabs(Time(ff.GetLastWriteTime()) - time) <= 2 && ff.GetLength() == l)
				out << fn << '\n';

		while(*f && *f++ != '\n')
			;
	}
	return out;
}

String CommandServer::FileRemove(const char *f)
{
	String out;
	while(*f)
	{
		const char *b = f;
		while(*f && *f != '\n')
			f++;
		String fn(b, f);
		if(!IsNull(fn) && !FileDelete(NativePath(fn)))
			out << NFormat("%s: error deleting file\n", fn);
		while(*f && *f++ != '\n')
			;
	}
	if(IsNull(out))
		out = "OK";
	return out;
}

String CommandServer::DeleteFolderDeep(const char *f)
{
	while(*f)
	{
		const char *b = f;
		while(*f && *f != '\n')
			f++;
		String fn(b, f);
		if(!IsNull(fn))
			Upp::DeleteFolderDeep(NativePath(fn));
		while(*f && *f++ != '\n')
			;
	}
	return "OK";
}

String CommandServer::GetFileInfo(const char *f)
{
	int td = ScanInt(f, &f) - (int)(GetSysTime() - Time(2000, 1, 1));
	if(*f++ != ':')
		return "ERROR(@): ':' expected after time delta";
	int line = 0;
	String output;
	while(*f)
	{
		line++;
		const char *b = f;
		while(*f && *f != '\n')
			f++;
		String fn(b, f);
		FindFile ff;
		output << fn;
		if(ff.Search(NativePath(fn)))
			output << '\t' << ((int)(Time(ff.GetLastWriteTime()) - Time(2000, 1, 1)) + td)
				<< '\t' << (ff.IsFile() ? (int)ff.GetLength() : -1);
		output << "\n";
		while(*f && *f++ != '\n')
			;
	}
	return output;
}

String CommandServer::UploadFiles(const char *f)
{
	SVRLOG("CommandServer::UploadFiles");
	int line = 0;
	while(*f)
	{
		++line;
		const char *b = f;
		while(*f && *f != '\t' && *f != '\n')
			f++;
		String fn = NativePath(String(b, f));
		if(IsNull(fn))
			return NFormat("ERROR: file name expected at line %d", line);
		SVRLOG("Updating file '" << fn << "'...");
		int t;
		if(*f != '\t' || IsNull(t = ScanInt(f + 1, &f)))
			return NFormat("ERROR(%s): file time expected at line %d", fn, line);

		Time time = Time(2000, 1, 1) + t;
		int l;
		if(*f != '\t' || IsNull(l = ScanInt(f + 1, &f)))
			return NFormat("ERROR(%s): file size expected at line %d", fn, line);

		if(*f != '\t')
			return NFormat("ERROR(%s): file content expected at line %d", fn, line);
		b = ++f;
		while(*f && *f != '\n')
			f++;
		if(*f != '\n')
			return NFormat("ERROR(%s): unexpected end of file at line %d", fn, line);
		SVRLOG("ASCII85 length = " << (int)(f - b));
		String bz = ASCII85Decode((const byte *)b, f - b);
		SVRLOG("BZ compressed length = " << bz.GetLength());
		String content = BZ2Decompress(bz);
		SVRLOG("Content length = " << content.GetLength());
		if(content.GetLength() != l)
			return NFormat("ERROR(%s): file size (%d) doesn't match decompressed size %d", fn, l, content.GetLength());
		RealizePath(fn);
		if(!SaveFile(fn, content) || !FileSetTime(fn, time))
			return NFormat("ERROR(%s): cannot write file", fn);
		SVRLOG(NFormat("FileSetTime(%s, %`) -> %`", fn, time, FileGetTime(fn)));
		while(*f && *f++ != '\n')
			;
	}
	SVRLOG("// CommandServer::UploadFiles");
	return "OK";
}

String CommandServer::DownloadFiles(const char *f)
{
	int line = 0;
	String output;
	while(*f)
	{
		line++;
		const char *b = f;
		while(*f && *f != '\n')
			f++;
		String fn(b, f);
		FindFile ff;
		output << fn;
		String nfn = NativePath(fn);
		if(ff.Search(nfn))
		{
			String data = LoadFile(nfn);
			output << '\t' << data.GetLength() << '\t' << ASCII85Encode(BZ2Compress(data));
		}
		output << "\n";
		while(*f && *f++ != '\n')
			;
	}
	return output;
}

String CommandServer::CreateDir(const char *f)
{
	RealizeDirectory(f);
	return "OK";
}

/*
void CommandServer::RunCommand(const String& _command, Socket& socket)
{
	String command = _command;
#ifdef PLATFORM_WIN32
	if(*command == '/')
		command = "x:" + command;
#endif
	LOG("CommandServer::RunCommand(" << command << ")");
	One<SlaveProcess> process = StartProcess(command);
	LOG("CommandServer::RunCommand -> OK");
	socket.Send("+\0", 2);
	bool work = false, running;
	do
	{
		if(socket.Peek())
		{
			SVRLOG("-> socket peek");
			String inp = socket.Recv(0 | Socket::MULTIPART);
			SVRLOG("-> input data: <" << inp << ">");
			if(!inp.IsEmpty())
			{
				work = true;
				process -> Write(inp);
			}
		}
		String out = Null;
		running = process -> Read(out);
//		SVRLOG("-> output data: <" << out << ">");
		if(!out.IsEmpty())
		{
			work = true;
			socket.Send(out);
		}
		else
			MtSleep(200);
//		SVRLOG("work = " << work << ", running = " << running);
//		if(!work && running)
	}
	while(running);
	socket.Send("\0", 1);
	LOG("Process terminated, exit code = " << process -> GetExitCode());
	String ec = IntStr(process -> GetExitCode());
	socket.Send(ec, ec.GetLength() + 1);
}
*/

void CommandServer::Execute(Socket& socket)
{
	String cmd = socket.ReadUntil('\0', Null, 10000000);
	if(logging)
		puts(cmd);
	SVRLOG(cmd);
	String out;
	const char *data = cmd;
	switch(*data++)
	{
	case '?':
		out = "+" TSERV_VERSION "\x1A";
		break;

	case '.':
		Quit();
		out = ".QUITTING\x1A";
		break;

	case ':':
	case '=':
		connections.Add(new CommandConnection(socket, cmd));
		return;

	case '-':
		out = FileRemove(data);
		break;

	case '~':
		out = DeleteFolderDeep(data);
		break;

	case '<':
		out = CheckFiles(data);
		break;

	case '>':
		out = UploadFiles(data);
		break;

	case '^':
		out = DownloadFiles(data);
		break;

	case '*':
		out = CreateDir(data);
		break;

	case '@':
		out = GetFileInfo(data);
		break;

	default:
		throw Exc(NFormat("Invalid command: '%s'", cmd));
	}
	socket.Write(out);
	socket.Write("\0", 1);
	socket.Close();
}

void CommandServer::ExecTimer()
{
}

bool CommandServer::StartServer(int port, const String& cmdline)
{
#ifdef PLATFORM_WIN32
	STARTUPINFO sinfo;
	GetStartupInfo(&sinfo);
	PROCESS_INFORMATION pinfo;
	Buffer<char> cmd(cmdline.GetLength() + 1);
	memcpy(cmd, cmdline, cmdline.GetLength() + 1);
	if(!CreateProcess(NULL, cmd, NULL, NULL, false,
		CREATE_NO_WINDOW, NULL, NULL, &sinfo, &pinfo))
		throw Exc(NFormat("Cannot create process '%s':\n%s", cmdline, GetErrorMessage(GetLastError())));
	// detach from process
	CloseHandle(pinfo.hThread);
	CloseHandle(pinfo.hProcess);
#else
	pid_t pid = fork();
	if(pid == -1)
		throw Exc(NFormat("Error executing fork(), error code: %d", errno));
//	printf("fork -> %d\n", pid);
	if(!pid)
	{ // new process, return true to fall through to server loop
		Logging(false);
		Timing(false);
		return true;
	}
#endif
	Socket::Init();
	String sockerr;
	String self = Socket::GetHostName();
	Socket socket;
	if(!ClientSocket(socket, self, port))
		throw Exc(NFormat("Error opening server %s:%d: %s", self, port, Socket::GetErrorText()));
	socket.Write("?\n");
	String res = socket.ReadUntil('\x1A');
	if(*res == '\x08')
		throw Exc(res.Mid(1));
	if(res != TSERV_VERSION)
		throw Exc("Cannot run server process.");
	return false;
}

void CommandServer::StopServer(int port)
{
	Socket::Init();
	Socket socket;
	if(!ClientSocket(socket, Socket::GetHostName(), port))
		throw Exc(NFormat("Error opening server at port %d", port));
	socket.Write(".\n");
	socket.ReadUntil('\x1A');
}

void CommandServer::RunServer(int port)
{
//	thread_count = 0;

	puts(NFormat("Running server at port %d", port));
	if(logging)
		puts(NFormat("Creating server socket at port %d ...", port));

	String error;
	Socket server;
	if(!ServerSocket(server, port, true, 100)) {
		puts(NFormat("Error creating server socket at port %d: %s", port, Socket::GetErrorText()));
		return;
	}
	server.NoLinger();

	if(logging) {
		if(ip_filter.IsEmpty())
			puts("IP Filtering: off (accepting requests from all IP addresses)");
		else
		{
			fputs("IP filter:", stdout);
			for(int i = 0; i < ip_filter.GetCount(); i++)
				fputs(' ' + FormatIP(ip_filter[i]), stdout);
			putc('\n', stdout);
		}
		puts("Running accept loop ...");
	}

	dword ipaddr;
	dword ticks = GetTickCount() + 1000;
	for(quit_flag = false; !quit_flag && !quit_signal;)
	{
		bool work = false;
		Socket connection;
		if(server.Accept(connection, &ipaddr, true, timer_chunk))
		{
			work = true;
			if(logging)
			{
				fputc('\r', stderr);
				fputs(Format(GetSysTime()), stderr);
				fflush(stderr);
			}
			ExecTimer();
			if(!connection.IsOpen())
				continue;

			try
			{
				if(!ip_filter.IsEmpty() && ip_filter.Find(ipaddr) < 0)
					throw Exc(NFormat("Invalid request source IP address: %s", FormatIP(ipaddr)));
				Execute(connection);
			}
			catch(Exc e)
			{
				if(connection.IsOpen())
					connection.Write('-' + e + '\0');
				if(logging)
					puts(NFormat("ERROR: %s", e));
			}
		}
		if(!connections.IsEmpty())
		{
			for(int i = connections.GetCount(); --i >= 0;)
			{
				Connection& conn = connections[i];
				bool err = false;
				try
				{
					if(conn.Run())
						work = true;
				}
				catch(Exc e)
				{
					puts(NFormat("%s: %s", conn.GetName(), e));
					err = true;
				}
				if(err || conn.IsEof())
				{
					connections.Remove(i);
					continue;
				}
			}
		}
		if(!work)
			Sleep(timer_chunk);
		dword new_ticks = GetTickCount();
		if(new_ticks >= ticks)
		{
			ticks = new_ticks + 1000;
			fputs(String().Cat() << "\r" << Format(GetSysTime()), stderr);
			fflush(stderr);
		}
	}
	if(logging)
		fputs("\nServer is shutting down ...                \n", stderr);
}

/*
static void PutConsole(const char *p)
{
	enum { ROW_BATCH = 20 };
	int rows = ROW_BATCH;
	int col = 0;
	const char *b = p;

	while(*p)
	{
		char c = *p;
		putchar(c);
		if(*p++ == '\n' || ++col >= 70)
		{
			col = 0;
			if(*p && --rows <= 0)
			{
				rows = ROW_BATCH;
				fputs("\n==== Pokracovat pomoci Enter ====\n", stdout);
				fflush(stdout);
				bool quit = false;
				for(int c; (c = getchar()) >= 0 && c != '\n'; quit |= (c == 'Q' || c == 'q'))
					;
				if(quit)
					break;
			}
		}
	}
	if(p > b && p[-1] != '\n')
		putchar('\n');
}
*/

String CommandServer::GetConInput(const char *prompt)
{
	fputc('\n', stdout);
	fputs(prompt, stdout);
	int i = getchar();
	if(i == EOF)
		return String::GetVoid();
	String s;
	while(i != '\n')
	{
		s.Cat(i);
		if((i = getchar()) == EOF)
			break;
	}
	return s;
}

void CommandServer::RunClient()
{
	Socket::Init();
	quit_flag = false;
	for(String in; !quit_flag && !(in = GetConInput("run> ")).IsVoid();)
	{
		if(!CompareNoCase(in, "."))
			break;

		try
		{
			One<SlaveProcess> sp = StartProcess(in);
			for(String s; sp -> Read(s); fputs(s, stdout))
			{
//				puts("Flushing application...");
				Sleep(500);
			}
			printf("\nTerminated, exit code = %d\n", sp -> GetExitCode());
		}
		catch(Exc e)
		{
			puts("Error: " + e);
		}
	}
}

void TryMain()
{
	bool start = false;
	bool stop = false;
	int port = -1;

	CommandServer server;
	Vector<String> cmdline;
	cmdline <<= CommandLine();

	for(int c = 0; c < cmdline.GetCount(); c++)
	{
		const char *arg = cmdline[c];
		if(*arg == '-')
			switch(arg++, *arg++)
			{
			case 'r': start = true; break;
			case 's': stop = true; break;
			default:  throw Exc(NFormat("invalid option: '%s'", cmdline[c]));
			}
		else if(IsDigit(*arg))
		{ // port number
			port = atoi(arg);
		}
	}

	bool noport = (port < 0);
	if(noport)
		port = DEFAULT_PORT;

	if(stop)
	{
		server.StopServer(port);
		return;
	}

	if(noport)
	{ // run client against host
		server.RunClient();
		return;
	}

	if(start)
	{
		String cmd_line;
		cmd_line << port;
		if(!server.StartServer(port, cmd_line))
			return;
	}

	server.RunServer(port);
}

CONSOLE_APP_MAIN
{
#ifdef PLATFORM_POSIX
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, &QuitSignal);
	signal(SIGTERM, &QuitSignal);
#endif//PLATFORM_POSIX
	puts(String().Cat() << "TSERV " TSERV_VERSION ", release date: " << TSERV_DATE << "\n"
	TSERV_COPYRIGHT);

/*
	Vector<String> test;
	for(int i = 0; i < 1000; i++)
		test.Add(GetRandomIdent(10000));
	test.Clear();

	puts("Done !");
	SetExitCode(111);
	return;
*/
	int retcode = 1;
	try
	{
//		SetLanguage(LNG_ENGLISH);
		TryMain();
	}
	catch(Exc e)
	{
		puts("Error: " + e);
		SetExitCode(1);
	}
}
