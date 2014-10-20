#include <plugin/lz4/lz4.h>

using namespace Upp;

#define LLOG(x)

// LZ4 streaming API example : double buffer
// Copyright : Takayuki Matsuoka


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

enum {
//    BLOCK_BYTES = 1024 * 8,
	BLOCK_BYTES = 1024 * 64, 
};


size_t write_int(FILE* fp, int i) {
    return fwrite(&i, sizeof(i), 1, fp);
}

size_t write_bin(FILE* fp, const void* array, size_t arrayBytes) {
    return fwrite(array, 1, arrayBytes, fp);
}

size_t read_int(FILE* fp, int* i) {
    return fread(i, sizeof(*i), 1, fp);
}

size_t read_bin(FILE* fp, void* array, size_t arrayBytes) {
    return fread(array, 1, arrayBytes, fp);
}

void test_compress(FILE* outFp, FILE* inpFp)
{
    LZ4_stream_t lz4Stream_body = { 0 };
    LZ4_stream_t* lz4Stream = &lz4Stream_body;

    char inpBuf[2][BLOCK_BYTES];
    int  inpBufIndex = 0;
    
   	Zero(inpBuf);

    for(;;) {
        char* const inpPtr = inpBuf[inpBufIndex];
        const int inpBytes = (int) read_bin(inpFp, inpPtr, BLOCK_BYTES);
        if(0 == inpBytes) {
            break;
        }

        {
            char cmpBuf[LZ4_COMPRESSBOUND(BLOCK_BYTES)];
            LLOG("To compress " << inpBytes << ' ' << MD5String(inpPtr, inpBytes) << " " << inpBufIndex);
            const int cmpBytes = LZ4_compress_continue(lz4Stream, inpPtr, cmpBuf, inpBytes);
            if(cmpBytes <= 0) {
                break;
            }
			LLOG("Out " << cmpBytes + 4);
            write_int(outFp, cmpBytes);
            write_bin(outFp, cmpBuf, (size_t) cmpBytes);
        }

        inpBufIndex = (inpBufIndex + 1) % 2;
    }

//    write_int(outFp, 0);
}


void test_decompress(FILE* outFp, FILE* inpFp)
{
    LZ4_streamDecode_t lz4StreamDecode_body = { 0 };
    LZ4_streamDecode_t* lz4StreamDecode = &lz4StreamDecode_body;

    char decBuf[2][BLOCK_BYTES];
    int  decBufIndex = 0;

    for(;;) {
        char cmpBuf[LZ4_COMPRESSBOUND(BLOCK_BYTES)];
        int  cmpBytes = 0;

        {
            const size_t readCount0 = read_int(inpFp, &cmpBytes);
            if(readCount0 != 1 || cmpBytes <= 0) {
                break;
            }

            const size_t readCount1 = read_bin(inpFp, cmpBuf, (size_t) cmpBytes);
            if(readCount1 != (size_t) cmpBytes) {
                break;
            }
        }

        {
            char* const decPtr = decBuf[decBufIndex];
            const int decBytes = LZ4_decompress_safe_continue(
                lz4StreamDecode, cmpBuf, decPtr, cmpBytes, BLOCK_BYTES);
            if(decBytes <= 0) {
                break;
            }
            write_bin(outFp, decPtr, (size_t) decBytes);
        }

        decBufIndex = (decBufIndex + 1) % 2;
    }
}

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

extern "C" {
extern int matchOffsetStat[256];
extern int matchLengthStat[256];
extern int matchLengthBig;
};

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
		    String lz4upp = AppendFileName(outdir, ff.GetName() + ".lz4upp");
		    String decupp = AppendFileName(outdir, ff.GetName());

			RLOG("***************** " << file);
		
		    // compress
		    {
		        RTIMESTOP("Compress");
		        FILE* inpFp = fopen(file, "rb");
		        FILE* outFp = fopen(lz4, "wb");
		
		        test_compress(outFp, inpFp);
		        printf("compress : done\n");
		
		        fclose(outFp);
		        fclose(inpFp);
		        RDUMP(GetFileLength(file));
		        RDUMP(GetFileLength(lz4));
		    }
		
		    // decompress
		    {
		        { RTIMESTOP("Decompress");
		        FILE* inpFp = fopen(lz4, "rb");
		        FILE* outFp = fopen(dec, "wb");
		
		        test_decompress(outFp, inpFp);
		        printf("decompress : done\n");
		
		        fclose(outFp);
		        fclose(inpFp);
		        }
		        ASSERT(FilesEqual(file, dec));
		    }

		    {
				RLOG("=== U++ compress small blocks ===========================================");
		        { RTIMESTOP("U++ compress small blocks");
		        FileOut out(lz4upp);
		        FileIn  in(file);
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
		        ASSERT(FilesEqual(lz4upp, lz4));
		    }
		
		    {
		        RLOG("=== U++ compress big blocks ======");
		        { RTIMESTOP("U++");
		        FileOut out(lz4upp);
		        FileIn  in(file);
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
		        ASSERT(FilesEqual(lz4upp, lz4));
		    }
		
		    {
		        RLOG("=== U++ decompress small blocks =====");
		        { RTIMESTOP("U++ decompress small blocks");
		        FileIn  in(lz4upp);
		        FileOut out(decupp);
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
		        ASSERT(FilesEqual(file, decupp));
		    }
		
		    {
		        RLOG("=== U++ decompress big blocks =====");
		        { RTIMESTOP("U++ decompress big blocks");
		        FileIn  in(lz4upp);
		        FileOut out(decupp);
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
		        ASSERT(FilesEqual(file, decupp));
		    }

		    DeleteFile(lz4);
		    DeleteFile(dec);
		    DeleteFile(lz4upp);
		    DeleteFile(decupp);
		}
		
		ff.Next();
	}

	DeleteFolderDeep(outdir);
	
	ASSERT(total_sz > 1024 * 1024 * 1024);

#if 0 
    LOG("--------- MATCH OFFSET HIBYTE -----------");
    int over512 = 0;
    for(int i = 0; i < 256; i++)
        if(matchOffsetStat[i]) {
            LOG(i << " " << matchOffsetStat[i]);
            if(i > 1)
            	over512 += matchOffsetStat[i];
        }
    DDUMP(over512);
    LOG("--------- MATCH LENGTH -----------");
    int  saved = 0;
    int  count = 0;
    for(int i = 0; i < 256; i++)
        if(matchLengthStat[i]) {
            LOG(i + 4 << " " << matchLengthStat[i] << ", bytes " << (i + 4) * matchLengthStat[i]);
            saved += (i + 4) * matchLengthStat[i];
            count += matchLengthStat[i];
        }
    DDUMP(count);
    DDUMP(saved);
    LOG("COST: " << count * 3);
    DDUMP(matchLengthBig);
#endif
}
