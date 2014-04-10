#include <Core/Core.h>

using namespace Upp;

void PosOverrunTest()
{
	String tmpfile = GetTempFileName("pos");
	FileOut fo;
	if(!fo.Open(tmpfile)) {
		Cout() << "PosOverrunTest: error creating file "  << tmpfile << "\n";
		return;
	}
	for(int i = 0; i < 0x10000; i++)
		fo.PutIW(i);
	int64 size = fo.GetSize();
	fo.Close();
	if(fo.IsError() || size != 0x20000) {
		Cout() << "PosOverrunTest generator error, file " << tmpfile << "\n";
		return;
	}
	FileIn fi;
	fi.SetBufferSize(4096);
	if(!fi.Open(tmpfile)) {
		Cout() << "PosOverrunTest: error reopening temporary file " << tmpfile << "\n";
		return;
	}
	for(int i = 0; i < 4096; i++)
		fi.Get();
	char buffer[32];
	fi.GetAll(buffer, 32);
	bool ok = true;
	for(int i = 0; i < 16; i++) {
		int strmval = Peek16le(buffer + 2 * i);
		int expect = 2048 + i;
		if(strmval != expect) {
			Cout() << "PosOverrunTest: " << FormatIntHex(expect, 4) << " expected, " << FormatIntHex(strmval, 4) << " found\n";
			ok = false;
		}
	}
	if(ok)
		Cout() << "PosOverrunTest: finished without errors\n";
}

enum {
	T_READ = 1,
	T_READGROUP = 2,
	T_WRITE = 4,
	T_WRITEGROUP = 8,
	T_SEEK = 16
};

String Copy(Stream& s, int spos, int ssize, int tpos, dword style) {
	bool lf = dynamic_cast<BlockStream *>(&s);
	if(lf) LOG("<<");
	if(style & T_SEEK)
		s.Seek(spos);
	Buffer<byte> b(ssize);
	int sz = 0;
	if(style & T_READ)
		if(style & T_READGROUP) {
			while(sz < ssize) {
				int c = s.Get();
				if(c < 0)
					break;
				b[sz++] = c;
			}
			ssize = sz;
		}
		else
			ssize = (int)s.Get(b, ssize);
	if(lf) LOG(">>");
	if(style & T_SEEK)
		s.Seek(tpos);
	if(style & T_WRITE)
		if(style & T_WRITEGROUP)
			for(int i = 0; i < ssize; i++)
				s.Put(b[i]);
		else
			s.Put(b, ssize);
	if(style & T_READ)
		return String(b, ssize);
	return Null;
}

String InFile() { return GetDataFile("FileStream.cpp"); }

void StreamTest() {
	FileIn in(InFile());
	FileOut out(ConfigFile("chips1.cpp"));
	for(;;) {
		int c = in.Get();
		if(c < 0)
			break;
		out.Put(c);
	}

	in.Seek(300);
	out.Seek(300);
	for(int i = 0; i < 2000; i++)
		out.Put(in.Get());
	out.SetBufferSize(8);
	out.Seek(0);

	LOG("================================================");
	StringStream ss(LoadFile(InFile()));
	int nn = 0;
	dword style = 0;
	int time0 = msecs();
	while(msecs(time0) < 300000) {
		if(++nn % 1000 == 0)
			Cout() << "ops: " << nn << ", len: " << ss.GetSize()
			       << ", buffersize: " << out.GetBufferSize() << "\n";
		if(ss.GetSize() > 256*1024) {
			int sz = (rand() % 1000) * 100;
			ss.SetSize(sz);
			out.SetSize(sz);
			LOG("Adjusted size: " << ss.GetSize());
		}
		if(ss.GetSize() != out.GetSize()) {
			DUMP(ss.GetSize());
			DUMP(out.GetSize());
			Panic("SIZE MISMATCH!");
			return;
		}
		if((rand() & 255) == 0)
		{
			int64 p = out.GetPos();
			out.Seek(0);
			if(ss.GetResult() != LoadStream(out)) {
				out.Seek(0);
				SaveFile(ConfigFile("f1.txt"), LoadStream(out));
				SaveFile(ConfigFile("f2.txt"), ss.GetResult());
				Panic("CONTENT INEQUAL!");
				return;
			}
			out.Seek(p);
		}
		int spos = ss.GetSize() ? rand() % (int)ss.GetSize() : 0;
		int ssize = rand() % (out.GetBufferSize() * 7);
		int tpos = ss.GetSize() ? MAKELONG(rand(), rand()) % (int)ss.GetSize() : 0;
		if((rand() & 3) == 0)
			tpos = minmax<int>(spos - (rand() & 63) + 32, 0, (int)out.GetSize());
		if((rand() & 3) == 0)
			ssize = rand() % (3 * out.GetBufferSize());
		if((rand() & 4091) == 0) {
			style++;
			Cout() << "MODE: " << style << "\n";
		}
		LOG("---------------------");
		LOG("spos: " << spos << " ssize: " << ssize << " tpos: " << tpos << " style: " << style);
//		DUMP(ss.GetSize());
//		DUMP(out.GetSize());
		String a = Copy(out, spos, ssize, tpos, style);
		String b = Copy(ss, spos, ssize, tpos, style);
//		DUMP(ss.GetSize());
//		DUMP(out.GetSize());
		if((rand() & 1023) == 0)
			out.SetBufferSize((rand() & 127) + 2);
		if(a != b) {
			SaveFile(ConfigFile("f1c.txt"), a);
			SaveFile(ConfigFile("f2c.txt"), b);
			out.Seek(0);
			SaveFile(ConfigFile("f1.txt"), LoadStream(out));
			SaveFile(ConfigFile("f2.txt"), ss.GetResult());
			DUMP(ss.GetSize());
			Panic("INEQUAL READ IN COPY!");
			return;
		}
	}
}

CONSOLE_APP_MAIN
{
//	DataBase::FullTest();
	TimeStop tm;
	PosOverrunTest();
	StreamTest();
	LOG("============= OK " << tm << " s");
}
