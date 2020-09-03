#include <plugin/zstd/zstd.h>

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
	
	RLOG("Test files directory: " << GetHomeDirFile("testdata"));

	FindFile ff(GetHomeDirFile("testdata") + "/*.*");

	String outdir = GetHomeDirFile("test.output");
	RealizeDirectory(outdir);
	int64 total_sz = 0;
	while(ff) {
		if(ff.IsFile()) {
			String file = ff.GetPath();
			total_sz += ff.GetLength();
		    String zstd = AppendFileName(outdir, ff.GetName() + ".zstd");
		    String dec = AppendFileName(outdir, ff.GetName());
			
			for(int smll = 0; smll < 2; smll++) {
				for(int concurrent = 0; concurrent < 2; concurrent++) {
					RLOG("***************** " << file << ": " << (ff.GetLength() >> 10) << " KB");
					RLOG("CONCURRENT: " << concurrent);
					RLOG("SMALL: " << smll);
				    {
						RLOG("Compress");
				        FileIn  in(file);
				        FileOut out(zstd);
				        ZstdCompressStream zstd(out);
				        zstd.Co(concurrent);
				        int ch = 0;
				        for(;;) {
				            String data = in.Get(smll ? (++ch & 511) + 1 : 1024*1024);
				            if(IsNull(data))
				                break;
				            zstd.Put(data);
				        }
				    }
				    
				    RLOG("LEN: " << (GetFileLength(zstd) >> 10) << " KB");
		
				    {
				        RLOG("Decompress");
				        FileIn  in(zstd);
				        FileOut out(dec);
				        ZstdDecompressStream zstd(in);
				        zstd.Co(concurrent);
				        int ch = 0;
				        for(;;) {
				            String data = zstd.Get(smll ? (++ch & 511) + 1 : 1024*1024);
				            if(IsNull(data))
				                break;
				            out.Put(data);
				        }
				        ASSERT(zstd.IsEof());
				        ASSERT(!zstd.IsError());
				    }
		
			        ASSERT(FilesEqual(file, dec));
				}
			}
	
		    DeleteFile(zstd);
		    DeleteFile(dec);
		}
		
		ff.Next();
	}
	
	RLOG("========================== OK");

	DeleteFolderDeep(outdir);
	
	ASSERT(total_sz > 1024 * 1024 * 1024);
}
