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
//	String inpFilename = "C:/xxx/Nos.exe";
//	String inpFilename = "C:/u/aws.data/Viking_Chanarambie_Fenton_With_SurroundingFarms_Backcast_EPEs_14Sep11_v1004_allTI_PCs.blb";

	String outdir = GetHomeDirFile("test.output");
	RealizeDirectory(outdir);
	int64 total_sz = 0;
	while(ff) {
		if(ff.IsFile()) {
			String file = ff.GetPath();
			total_sz += ff.GetLength();
		    String lz4 = AppendFileName(outdir, ff.GetName() + ".lz4");
		    String dec = AppendFileName(outdir, ff.GetName());

			RLOG("***************** " << file << ": " << (ff.GetLength() >> 10) << " KB");
		
		    {
				RLOG("=== U++ compress small blocks ===========================================");
		        { RTIMESTOP("U++ compress small blocks");
		        FileIn  in(file);
		        FileOut out(lz4);
		        Lz4 lz4;
		        lz4.Compress();
		        lz4.WhenOut = callback1(Out, &out);
		        int ch = 0;
		        for(;;) {
		            String data = in.Get((++ch & 511) + 1);
		            if(IsNull(data))
		                break;
		            lz4.Put(~data, data.GetCount());
		        }
		        lz4.End();
		        out.Close();
		        }
		    }

		    {
		        RLOG("=== U++ decompress small blocks =====");
		        { RTIMESTOP("U++ decompress small blocks");
		        FileIn  in(lz4);
		        FileOut out(dec);
		        Lz4 lz4;
		        lz4.Decompress();
		        lz4.WhenOut = callback1(Out, &out);
		        int ch = 0;
		        for(;;) {
		            String data = in.Get((++ch & 511) + 1);
		            if(IsNull(data))
		                break;
		            lz4.Put(~data, data.GetCount());
		        }
		        lz4.End();
		        ASSERT(!lz4.IsError());
		        out.Close();
		        }
		    }
		

	        ASSERT(FilesEqual(file, dec));
		
		    {
		        RLOG("=== U++ compress big blocks ======");
		        { RTIMESTOP("U++");
		        FileIn  in(file);
		        FileOut out(lz4);
		        Lz4 lz4;
		        lz4.Compress();
		        lz4.WhenOut = callback1(Out, &out);
		        int ch = 0;
		        for(;;) {
		            String data = in.Get(150000);
		            if(IsNull(data))
		                break;
		            lz4.Put(~data, data.GetCount());
		        }
		        lz4.End();
		        out.Close();
		        }
		    }
		
		    {
		        RLOG("=== U++ decompress big blocks =====");
		        { RTIMESTOP("U++ decompress big blocks");
		        FileIn  in(lz4);
		        FileOut out(dec);
		        Lz4 lz4;
		        lz4.Decompress();
		        lz4.WhenOut = callback1(Out, &out);
		        int ch = 0;
		        for(;;) {
		            String data = in.Get(167321);
		            if(IsNull(data))
		                break;
		            lz4.Put(~data, data.GetCount());
		        }
		        lz4.End();
		        ASSERT(!lz4.IsError());
		        
		        
		        out.Close();
		        }
		    }

	        ASSERT(FilesEqual(file, dec));

		    DeleteFile(lz4);
		    DeleteFile(dec);
		}
		
		ff.Next();
	}

	DeleteFolderDeep(outdir);
	
	ASSERT(total_sz > 1024 * 1024 * 1024);
}
