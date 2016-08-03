#include <plugin/lz4/lz4.h>

using namespace Upp;

#define LLOG(x)

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool FilesEqual(const char *p1, const char *p2)
{
	FileIn in1(p1);
	FileIn in2(p2);
	
	if(in1.GetSize() != in2.GetSize())
		return false;
	
	while(!in1.IsEof()) {
		if(in1.Get(1024*1024) != in2.Get(1024*1024))
			return false;
	}
	
	return true;
}

void Out(const void *data, int count, Stream *out)
{
	LLOG("Out " << count);
	out->Put(data, count);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	FindFile ff(GetHomeDirFile("testdata") + "/*.*");

	String outdir = GetHomeDirFile("test.output");
	RealizeDirectory(outdir);
	int64 total_sz = 0;
	while(ff) {
		if(ff.IsFile()) {
			String file = ff.GetPath();
			total_sz += ff.GetLength();
		    String lz4 = AppendFileName(outdir, ff.GetName() + ".lz4");
		    String dec = AppendFileName(outdir, ff.GetName());
			
			for(int smll = 0; smll < 2; smll++) {
				for(int concurrent = 0; concurrent < 2; concurrent++) {
					RLOG("***************** " << file << ": " << (ff.GetLength() >> 10) << " KB");
					RLOG("CONCURRENT: " << concurrent);
					RLOG("SMALL: " << smll);
				    {
						RLOG("Compress");
				        FileIn  in(file);
				        FileOut out(lz4);
				        LZ4CompressStream lz4(out);
				        lz4.Co(concurrent);
				        int ch = 0;
				        for(;;) {
				            String data = in.Get(smll ? (++ch & 511) + 1 : 1024*1024);
				            if(IsNull(data))
				                break;
				            lz4.Put(data);
				        }
				    }
		
				    {
				        RLOG("Decompress");
				        FileIn  in(lz4);
				        FileOut out(dec);
				        LZ4DecompressStream lz4(in);
				        lz4.Co(concurrent);
				        int ch = 0;
				        for(;;) {
				            String data = lz4.Get(smll ? (++ch & 511) + 1 : 1024*1024);
				            if(IsNull(data))
				                break;
				            out.Put(data);
				        }
				        ASSERT(lz4.IsEof());
				        ASSERT(!lz4.IsError());
				    }
		
			        ASSERT(FilesEqual(file, dec));
				}
			}
	
		    DeleteFile(lz4);
		    DeleteFile(dec);
		}
		
		ff.Next();
	}

	DeleteFolderDeep(outdir);
	
	ASSERT(total_sz > 1024 * 1024 * 1024);
}
