#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

#ifdef CPU_SIMD

	i8x16 a(-1, 2, -3, 4, -5, 6, -7, 8, -9, 10, -11, 12, -13, 14, -15, 16);
	i16x8 b(-1, 2, -3, 4, -5, 6, -7, 8 );
	i32x4 c(-1, 2, -3, 4);
	f32x4 d(-1.0f, 2.0f, -3.0f, 4.0f);

	// Test 1
	auto mask1 = GetMaski8x16(a);
	DUMPHEX(mask1);
	ASSERT(mask1 == 0b1010101010101010);

	auto mask2 = GetMaski16x8(b);
	DUMPHEX(mask2);
	ASSERT(mask2 == 0b10101010);

	auto mask3 = GetMaski32x4(c);
	DUMPHEX(mask3);
	ASSERT(mask3 == 0b1010);
	
	auto mask4 = GetMaskf32x4(d);
	DUMPHEX(mask4);
	ASSERT(mask4 == 0b1010);
	
	LOG("=================");
	
	// Test 2
	byte p[16] = {0};
	mask1 = GetMaski8x16(p);
	// Case 1: All zeros
	LOG(Format("All zeros:   0x%04x (expected: 0x0000)", mask1));

	// Case 2: All ones
	byte q[16];
	for (int i = 0; i < 16; i++)
		q[i] = 0xFF;
	mask2 = GetMaski8x16(q);
	LOG(Format("All ones:    0x%04x (expected: 0xffff)", mask2));

	// Test case 3: Alternating pattern
	byte r[16];
	for (int i = 0; i < 16; i++)
		r[i] = (i % 2) ? 0x80 : 0x00;
	mask3 = GetMaski8x16(r);
	LOG(Format("Alternating: 0x%04x (expected: 0xaaaa)", mask3));

	// Test case 4: Single bit set
	byte s[16] = {0};
	s[5] = 0x80;  // Set MSB of 6th byte
	mask4 = GetMaski8x16(s);
	LOG(Format("Single bit:  0x%04x (expected: 0x0020)", mask4));

	CheckLogEtalon();
	
#else
	Cout() << "================= NO SIMD\r\n";
#endif	
}
