#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String fn = ConfigFile("test");
	{
		for(int i = 0; i < 1000000; i++) {
			RTIMING("OpenClose");
			int fd = open(fn, O_CREAT|O_WRONLY|O_APPEND|O_LARGEFILE, 0777);
			if(fd < 0)
				Panic("Cannot open \'" + fn + "\': " + GetLastErrorMessage());
	//		if(write(fd, (void *)~b.data, ASZ * b.count) != ASZ * b.count)
	//			SnbPanic("Error writing to \'" + fn + "\': " + GetLastErrorMessage());
			if(fsync(fd))
				Panic("fsync error \'" + fn + "\': " + GetLastErrorMessage());
			if(close(fd))
				Panic("Error closing \'" + fn + "\': " + GetLastErrorMessage());
		}
	}
	int l;
	{
		for(int i = 0; i < 1000000; i++) {
			RTIMING("OpenClose + lseek");
			int fd = open(fn, O_CREAT|O_WRONLY|O_APPEND|O_LARGEFILE, 0777);
			lseek(fd, 0, SEEK_CUR);
			if(fd < 0)
				Panic("Cannot open \'" + fn + "\': " + GetLastErrorMessage());
	//		if(write(fd, (void *)~b.data, ASZ * b.count) != ASZ * b.count)
	//			SnbPanic("Error writing to \'" + fn + "\': " + GetLastErrorMessage());
			if(fsync(fd))
				Panic("fsync error \'" + fn + "\': " + GetLastErrorMessage());
			if(close(fd))
				Panic("Error closing \'" + fn + "\': " + GetLastErrorMessage());
		}
	}
}
