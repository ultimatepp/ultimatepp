#include <Web/Web.h>
#include <plugin/bz2/bz2.h>

#define WEBLINK_VERSION    "1.0.r3"
#define WEBLINK_DATE       Date(2004, 7, 1)
#define WEBLINK_COPYRIGHT  "Copyright © 2004 Tomas Rylek"

struct FilePacket
{
	void Serialize(Stream& stream);

	String filename;
	String filedata;
	Time   filetime;
};

void FilePacket::Serialize(Stream& stream)
{
	int version = StreamHeading(stream, 1, 1, 1, "FilePacket");
	if(version >= 1)
		stream % filename % filedata % filetime;
}

static void SendFilePacket(String host, int port, FilePacket& packet)
{
	Socket socket = ClientSocket(host, port);
	if(IsNull(socket))
		throw Exc(NFormat("cannot open %s:%d: %s", host, port, Socket::GetErrorText()));
	StringStream stream;
	stream % packet;
	String compressed = BZ2Compress(stream.GetResult());
	char temp[12];
	strcpy(temp, "SEND");
	PokeIL(temp + 4, compressed.GetLength());
	int crc = GetCRC(compressed);
	PokeIL(temp + 8, crc);
	if(!socket.WriteWait(temp, 12, 2000))
		throw Exc("timeout writing packet header");
	for(int done = 0; done < compressed.GetLength();) {
		fputs(NFormat("\r%s .. %d B", packet.filename, done), stdout);
		fflush(stdout);
		int bytes = socket.WriteRaw(compressed.GetIter(done), min<int>(compressed.GetLength() - done, 1024));
		if(socket.IsError())
			throw Exc(NFormat("%s:%d: error writing socket: %s", host, port, Socket::GetErrorText()));
		if(bytes > 0)
			done += bytes;
		socket.PeekWrite(1000);
	}
	fputs(NFormat("\rUpload of %s, %d B .. (done)", packet.filename, packet.filedata.GetLength()), stdout);
	fflush(stdout);
	for(;;) {
		String peek = socket.PeekCount(4, 100);
		if(peek.GetLength() >= 4) {
			if(PeekIL(peek) != crc)
				throw Exc(NFormat("%s:%d: bad checksum on file %s", host, port, packet.filename));
			fputs(" .. (OK)\n", stdout);
			fflush(stdout);
			break;
		}
	}
}

static void RecvFilePacket(String host, int port, FilePacket& packet)
{
	Socket socket = ClientSocket(host, port);
	if(IsNull(socket))
		throw Exc(NFormat("cannot open %s:%d: %s", host, port, Socket::GetErrorText()));
	StringStream stream;
	stream % packet;
	String compressed = BZ2Compress(stream.GetResult());
	char temp[12];
	strcpy(temp, "RECV");
	PokeIL(temp + 4, compressed.GetLength());
	int crc = GetCRC(compressed);
	PokeIL(temp + 8, crc);
	if(!socket.WriteWait(temp, 12, 2000) || !socket.WriteWait(compressed, compressed.GetLength(), 10000))
		throw Exc("timeout writing request");
	if(socket.ReadCount(temp, 12, 30000) != 12 || memcmp(temp, "DOWN", 4))
		throw Exc(NFormat("%s:%d: error receiving response header", host, port));
	int downlen = PeekIL(temp + 4);
	compressed = Null;
	while(compressed.GetLength() < downlen) {
		String part = socket.Read(1000, downlen - compressed.GetLength());
		if(!part.IsEmpty())
			compressed.Cat(part);
		fputs(NFormat("\r%d / %d B", compressed.GetLength(), downlen,
			iscale(compressed.GetLength(), 100, max(downlen, 1))), stdout);
		fflush(stdout);
	}
	if(GetCRC(compressed) != PeekIL(temp + 8))
		throw Exc(NFormat("%s:%d: download CRC mismatch", host, port));
	stream.Open(BZ2Decompress(compressed));
	stream % packet;
	if(stream.IsError())
		throw Exc(NFormat("%s:%d: download stream error", host, port));
}

static void RunServer(int port)
{
	Socket socket = ServerSocket(port);
	if(IsNull(socket))
		throw Exc(NFormat("cannot open server socket %d: %s", port, Socket::GetErrorText()));
	for(;;)
		try {
			fputs(NFormat("\rweblink server (version %s) running at port %d (%`)", WEBLINK_VERSION, port, GetSysTime()), stdout);
			fflush(stdout);
			Socket new_socket = socket.Accept(NULL, true, 1000);
			if(!IsNull(new_socket)) {
				char temp[12];
				if(new_socket.ReadCount(temp, 12, 30000) < 12)
					throw Exc("error reading request header");
				String compressed;
				int clen = PeekIL(temp + 4);
				while(compressed.GetLength() < clen) {
					fputs(NFormat("\r%d / %d B", compressed.GetLength(), clen), stdout);
					fflush(stdout);
					compressed.Cat(new_socket.Read(clen - compressed.GetLength(), 1000));
				}
				if(GetCRC(compressed) != PeekIL(temp + 8))
					throw Exc("invalid request CRC");
				StringStream stream(BZ2Decompress(compressed));
				FilePacket packet;
				stream % packet;
				if(stream.IsError())
					throw Exc("request header error");
				if(!memcmp(temp, "SEND", 4)) {
					if(!SaveFile(packet.filename, packet.filedata) || !FileSetTime(packet.filename, packet.filetime))
						throw Exc(NFormat("error saving file %s (%d B)", packet.filename, packet.filedata.GetLength()));
					fputs(NFormat("\nUpload: %s, %d B\n", packet.filename, packet.filedata.GetLength()), stdout);
					fflush(stdout);
					if(!new_socket.WriteWait(temp + 8, 4, 10000))
						throw Exc("handshake error");
				}
				else if(!memcmp(temp, "RECV", 4)) {
					packet.filedata = LoadFile(packet.filename);
					packet.filetime = FileGetTime(packet.filename);
					if(packet.filedata.IsVoid())
						throw Exc(NFormat("error reading file '%s'", packet.filename));
					stream.Create();
					stream % packet;
					compressed = BZ2Compress(stream.GetResult());
					strcpy(temp, "DOWN");
					PokeIL(temp + 4, compressed.GetLength());
					PokeIL(temp + 8, GetCRC(compressed));
					if(!new_socket.WriteWait(temp, 12, 30000))
						throw Exc("error writing response header");
					for(int done = 0; done < compressed.GetLength();) {
						fputs(NFormat("\r%s .. %d / %d B", packet.filename, done, compressed.GetLength(),
							iscale(done, 100, (compressed.GetLength(), 1))), stdout);
						fflush(stdout);
						int part = new_socket.WriteRaw(compressed.GetIter(done), min<int>(compressed.GetLength() - done, 1024));
						if(part > 0)
							done += part;
					}
					puts(NFormat("Download: %s, %d B\n", packet.filename, packet.filedata.GetLength()));
					fflush(stdout);
				}
				else
					throw Exc("request header error");
			}

		}
		catch(Exc e) {
			puts(e);
		}
}

static void TryMain()
{
	String server;
	int port = 4444;

	const Vector<String>& cmd = CommandLine();
	if(!cmd.IsEmpty() && cmd[0] == "-v") {
		AppVersion(WEBLINK_VERSION);
		return;
	}
	for(int i = 0; i < cmd.GetCount(); i++) {
		const char *arg = cmd[i];
		if(*arg == '-' || *arg == '+') {
			bool upload = (*arg++ == '+');
			if(!*arg) {
				if(!*arg && ++i >= cmd.GetCount())
					throw Exc("+/- missing file name");
				arg = cmd[i];
			}
			String file = arg;
			if(IsNull(server))
				throw Exc("+/-: server name not set");
			FilePacket packet;
			if(upload) {
				packet.filename = GetFileName(file);
				packet.filedata = LoadFile(file);
				packet.filetime = FileGetTime(file);
				if(packet.filedata.IsVoid())
					throw Exc(NFormat("error reading file '%s'", file));
				SendFilePacket(server, port, packet);
			}
			else {
				packet.filename = GetFileName(file);
				RecvFilePacket(server, port, packet);
				if(!SaveFile(file, packet.filedata) || !FileSetTime(file, packet.filetime))
					throw Exc(NFormat("error writing file '%s'", file));
			}
		}
		else if(IsDigit(*arg)) {
			port = ScanInt(arg + 1);
			if(port < 1 || port > 65535)
				throw Exc(NFormat("invalid port number: %d", port));
		}
		else
			server = arg;
	}

	if(IsNull(server))
		RunServer(port);
}

CONSOLE_APP_MAIN
{
	try {
		TryMain();
	}
	catch(Exc e) {
		puts(e);
		SetExitCode(1);
	}
}
