#include <Core/Core.h>

String GenString(int len)
{
	String result;
	while(len--)
		result.Cat(255 - rand() % 226);
	return result;
}

void Main()
{
	String master = LoadFile("e:\\file1.txt");
	String data = LoadFile("e:\\file2.txt");
	TimeStop();
//	LOG("Test");
//	RDUMP(TimeStop());
//		RDUMP(DiffPack(master, data).GetLength());
	TimeStop();
	for(int i = 0; i < 10; i++)
	{
		RDUMP(DiffPack(master, data).GetLength());
//		ASSERT(DiffUnpack(master, pack) == data);
	}
	RDUMP(TimeStop());

	String s = GenString(500000);
	String z = String(0, 1000000);
	TimeStop();
	RDUMP(DiffPack(s, s + s).GetLength());
	RDUMP(TimeStop());
	RDUMP(DiffPack(z, z).GetLength());
	RDUMP(TimeStop());
	int q = 0;
	for(;;) {
		if(q++ % 1000)
			printf("%d\r\n", q);
		if(rand() % 5 == 0)
			master = data;
		int n = rand() % 50;
		if(rand() & 1)
			data.Insert(rand() % data.GetLength(), GenString(n));
		else {
			if(n < data.GetLength())
				data.Remove(rand() % (data.GetLength() - n), n);
		}
		String pack = DiffPack(master, data);
		ASSERT(DiffUnpack(master, pack) == data);
	}
}
