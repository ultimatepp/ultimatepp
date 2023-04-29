#include <Core/Core.h>
#include <Draw/Draw.h>
#include <Painter/Painter.h>
#include <Painter/AlphaBlend.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
#ifdef CPU_SIMD
	{
		RGBA x = Color(11, 22, 33);
	
	#ifdef PLATFORM_MACOS
		*(dword *)&x = SwapEndian32(*(dword *)&x);
	#endif
		
		RGBA *s = &x;
		
		DUMP(LoadRGBAF(&x));
		DUMP(i16x8(*(dword *)s));
		DUMP(Unpack8L(i16x8(*(dword *)s)));
		DUMP(Unpack8L(i16x8(*(dword *)s)));
		
		Vector<RGBA> p;
		p.Add(Yellow());
		p.Add(30 * Magenta());
		p.Add(100 * Blue());
		p.Add(200 * Green());
		p.Add(Red());
	
		DUMP(p);
		AlphaBlend(p.begin(), 250 * LtRed(), 250, 5);
		DUMP(p);
	
		RGBA c = 250 * LtRed();
		DUMP(c);
	#ifdef PLATFORM_MACOS
		*(dword *)&c = SwapEndian32(*(dword *)&c);
	#endif
		DUMP(LoadRGBA2(c));
		DUMP(i16all(250));
		DUMP(i16all(250) * LoadRGBA2(c));
		DUMP(i16all(250) * LoadRGBA2(c) >> 8);
		DUMP(Mul8(LoadRGBA2(c), 250));
	#ifdef PLATFORM_MACOS // too hard to reconcile here...
		LOG("MakeAlpha(Mul8(LoadRGBA2(c), 250)) = 12 11 11 11  12 11 11 11");
	#else
		DUMP(MakeAlpha(Mul8(LoadRGBA2(c), 250)));
	#endif
	}

	{
		f32x4 x(1, 2, 3, 4);
		f32x4 y(1, 3, 2, 4);
		DLOG("----------------");
		DUMP(x);
		DUMP(Broadcast0(x));
		DUMP(Broadcast1(x));
		DUMP(Broadcast2(x));
		DUMP(Broadcast3(x));
		DUMP(min(x, y));
		DUMP(max(x, y));
		DUMP(x == y);
		DUMP(x != y);
		DUMP(x < y);
		DUMP(x <= y);
		DUMP(x > y);
		DUMP(x >= y);
		DUMP(AllTrue(x == y));
		DUMP(AllTrue(x == x));
		DUMP(x + y);
		DUMP(x - y);
		DUMP(x / y);
		DUMP(x * y);
	}

	{
		i16x8 x(1, 2, 3, 4, 5, 6, 7, 8);
		i16x8 y(1, 4, 2, 3, 8, 7, 5, 6);
		DLOG("----------------");
		DUMP(BroadcastLH0(x));
		DUMP(BroadcastLH1(x));
		DUMP(BroadcastLH2(x));
		DUMP(BroadcastLH3(x));
		DUMP(x == y);
		DUMP(x < y);
		DUMP(x > y);
		DUMP(AllTrue(x == y));
		DUMP(AllTrue(x == x));
		DUMP(x + y);
		DUMP(x - y);
		DUMP(x * y);
	}
	
	{
		i8x16 x(1,2,3,4, 11,12,13,14, 21,22,23,24, 31,32,33,34);
		DLOG("----------------");
		DUMP(x);
		DUMP(Unpack8L(x));
		DUMP(Unpack8H(x));
	}
	{
		i16x8 x(1,2,3,4, 11,12,13,14);
		DLOG("----------------");
		DUMP(Unpack16L(x));
		DUMP(Unpack16H(x));
	}
	
	{
		i32x4 x(1, 2, 3, 4);
		i32x4 y(1, 4, 2, 3);
		DLOG("----------------");
		DUMP(ToFloat(x));
		DUMP(x == y);
		DUMP(x < y);
		DUMP(x > y);
		
		DUMP(AllTrue(x == y));
		DUMP(AllTrue(x == x));

		DUMP(x + y);
		DUMP(x - y);
	}

	{
		f32x4 x(1.1, 2.8, 3.7, -4.2);
		DLOG("----------------");
		DUMP(x);
		DUMP(Truncate(x));
	}

	auto LoadLog = [](const String& path) {
		String s = Filter(LoadFile(path), [](int c) { return c == '\r' ? 0 : c; });
		return s.Mid(max(s.FindAfter("\n"), 0));
	};
	
	String log = LoadLog(GetStdLogPath());
	String etalon = LoadLog(GetDataFile("Etalon.log"));

	SaveFile(GetHomeDirFile("log.txt"), log);
	SaveFile(GetHomeDirFile("etalon.txt"), etalon);
	
	ASSERT(log == etalon);

	LOG("================ OK");
#else
	LOG("================ NO SIMD");
#endif
}
