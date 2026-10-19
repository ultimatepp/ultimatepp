#include "lzma.h"

#include <lzma.h>

/*
	HAVE_LZMAEXT: Use the LZMA1EXT API (added in liblzma 5.4.0).
	
	This provides:
	- LZMA_FILTER_LZMA1EXT filter ID
	- lzma_options_lzma::ext_size_low, ext_size_high, ext_flags
	- LZMA_LZMA1EXT_ALLOW_EOPM constant
	- lzma_filters_free() function
	
	Older liblzma (like Rocky 9's 5.2.5) falls back to LZMA1 +
	lzma_alone_decoder(). The only behavioral difference: the
	fallback encoder always writes the end-of-payload marker,
	which is still valid .lzma format.
*/
#if defined(LZMA_VERSION_MAJOR) && defined(LZMA_VERSION_MINOR) && \
    (LZMA_VERSION_MAJOR > 5 || \
     (LZMA_VERSION_MAJOR == 5 && LZMA_VERSION_MINOR >= 4))
	#define HAVE_LZMAEXT 1
#else
	#define HAVE_LZMAEXT 0
#endif

namespace Upp {

static constexpr size_t LZMA_PROPS_SIZE = 5;
static constexpr size_t LZMA_HEADER_SIZE = LZMA_PROPS_SIZE + 8;
static constexpr size_t LZMA_IO_BUFFER_SIZE = 1 << 16;


static bool Write(Stream& out, const byte *data, size_t size, int64& written)
{
	if(size == 0)
		return true;

	out.Put64(data, size);

	if(out.IsError())
		return false;

	written += (int64)size;
	return true;
}


static bool ReportProgress(Gate2<int64, int64>& progress,
                           uint64 done, uint64 total)
{
	return progress && progress((int64)done, (int64)total);
}


#if HAVE_LZMAEXT

static void SetExtSize(lzma_options_lzma& options, uint64 size)
{
	options.ext_size_low  = (uint32_t)size;
	options.ext_size_high = (uint32_t)(size >> 32);
}

#endif


static bool Encode(Stream& out, Stream& in, int64 fileSize,
                   Gate2<int64, int64> progress, int level,
                   int64& written)
{
	if(fileSize < 0 || level < 0 || level > 9)
		return false;

	lzma_options_lzma options;

	if(lzma_lzma_preset(&options, (uint32_t)level))
		return false;

#if HAVE_LZMAEXT
	/*
		LZMA1EXT is important here.

		With ext_flags == 0 the encoder does NOT write the
		LZMA end-of-payload marker. The real uncompressed size
		is stored in our 13-byte .lzma header instead.

		This matches the old U++ LZMA SDK output semantics.
	*/
	options.ext_flags = 0;
	SetExtSize(options, (uint64)fileSize);

	lzma_filter filters[] = {
		{ LZMA_FILTER_LZMA1EXT, &options },
		{ LZMA_VLI_UNKNOWN, nullptr }
	};
#else
	/*
		liblzma < 5.4: no LZMA1EXT. The raw LZMA1 encoder
		ALWAYS writes the end-of-payload marker on LZMA_FINISH;
		there is no API to suppress it. And .lzma decoders of
		this era (e.g. lzma_alone_decoder on 5.2.x) reject a
		stream that has BOTH a known uncompressed size in the
		header AND an end-of-payload marker.

		So we cannot honestly declare the size here. Instead we
		write the "streamed" .lzma variant: size unknown
		(0xFFFFFFFFFFFFFFFF), terminated by the EOPM. This is
		exactly what "xz -F lzma" produces for piped input and
		it is decodable by every liblzma generation, including
		5.2.x and our own decoder on all platforms.
	*/
	lzma_filter filters[] = {
		{ LZMA_FILTER_LZMA1, &options },
		{ LZMA_VLI_UNKNOWN, nullptr }
	};
#endif

	/*
		The legacy .lzma properties field is exactly 5 bytes.
	*/
	uint32_t props_size = 0;

	if(lzma_properties_size(&props_size, &filters[0]) != LZMA_OK ||
	   props_size != LZMA_PROPS_SIZE)
		return false;

	byte header[LZMA_HEADER_SIZE];

	if(lzma_properties_encode(&filters[0], header) != LZMA_OK)
		return false;

	/*
		Legacy U++ format:

		    5 bytes  LZMA properties
		    8 bytes  uint64 uncompressed size, little endian

		On liblzma < 5.4 the size is declared unknown because
		the EOPM is unavoidable in the stream (see above).
	*/
	uint64 size = (uint64)fileSize;

#if !HAVE_LZMAEXT
	size = UINT64_MAX;
#endif

	for(int i = 0; i < 8; i++)
		header[LZMA_PROPS_SIZE + i] = (byte)(size >> (8 * i));

	if(!Write(out, header, sizeof(header), written))
		return false;

	lzma_stream strm = LZMA_STREAM_INIT;

	/*
		IMPORTANT: lzma_raw_encoder() expects a terminated
		filter array.
	*/
	lzma_ret ret = lzma_raw_encoder(&strm, filters);

	if(ret != LZMA_OK)
		return false;

	byte inbuf[LZMA_IO_BUFFER_SIZE];
	byte outbuf[LZMA_IO_BUFFER_SIZE];

	bool eof = false;
	bool ok = true;

	strm.next_in = nullptr;
	strm.avail_in = 0;

	for(;;) {
		if(strm.avail_in == 0 && !eof) {
			int64 n = in.Get64(inbuf, sizeof(inbuf));

			if(n < 0 || in.IsError()) {
				ok = false;
				break;
			}

			if(n == 0) {
				eof = true;
			}
			else {
				strm.next_in = inbuf;
				strm.avail_in = (size_t)n;
			}
		}

		strm.next_out = outbuf;
		strm.avail_out = sizeof(outbuf);

		ret = lzma_code(&strm, eof ? LZMA_FINISH : LZMA_RUN);

		size_t produced = sizeof(outbuf) - strm.avail_out;

		if(produced && !Write(out, outbuf, produced, written)) {
			ok = false;
			break;
		}

		if(ReportProgress(progress, strm.total_in, (uint64)fileSize)) {
			ok = false;
			break;
		}

		if(ret == LZMA_STREAM_END)
			break;

		if(ret != LZMA_OK) {
			ok = false;
			break;
		}
	}

	lzma_end(&strm);

	return ok && !in.IsError() && !out.IsError();
}

static bool Decode(Stream& out, Stream& in,
                   Gate2<int64, int64> progress,
                   int64& written)
{
	byte header[LZMA_HEADER_SIZE];

	if(in.Get(header, sizeof(header)) != sizeof(header) || in.IsError())
		return false;

	uint64 unpackSize = 0;

	for(int i = 0; i < 8; i++)
		unpackSize |=
			(uint64)header[LZMA_PROPS_SIZE + i] << (8 * i);

#if HAVE_LZMAEXT
	/*
		IMPORTANT:

		lzma_properties_decode() allocates the options structure.
		Do not point filter.options at a stack object.
	*/
	lzma_filter filters[] = {
		{ LZMA_FILTER_LZMA1EXT, nullptr },
		{ LZMA_VLI_UNKNOWN, nullptr }
	};

	lzma_ret ret = lzma_properties_decode(
		&filters[0],
		nullptr,
		header,
		LZMA_PROPS_SIZE
	);

	if(ret != LZMA_OK)
		return false;

	lzma_options_lzma *options =
		(lzma_options_lzma *)filters[0].options;

	/*
		The header contains the exact uncompressed size.

		Allow an EOPM as well, so the new decoder accepts both:

		    old U++ format:
		        known size + no EOPM

		    ordinary LZMA-Alone:
		        known size + EOPM

		    streamed LZMA-Alone (written by the encoder above
		    on liblzma < 5.4):
		        unknown size + EOPM
	*/
	SetExtSize(*options, unpackSize);
	options->ext_flags = LZMA_LZMA1EXT_ALLOW_EOPM;

	/*
		IMPORTANT: pass the complete terminated filter array.
	*/
	lzma_stream strm = LZMA_STREAM_INIT;

	ret = lzma_raw_decoder(&strm, filters);

	if(ret != LZMA_OK) {
		lzma_filters_free(filters, nullptr);
		return false;
	}
#else
	/*
		liblzma < 5.4: no LZMA1EXT. lzma_alone_decoder()
		consumes the complete 13-byte .lzma header itself
		(properties + uncompressed size). Both variants decode
		natively here:

		    known size (old U++ format, no EOPM):
		        the decoder stops exactly at the size

		    unknown size (streamed, EOPM present):
		        the decoder stops at the EOPM
	*/
	lzma_stream strm = LZMA_STREAM_INIT;

	lzma_ret ret = lzma_alone_decoder(&strm, UINT64_MAX);

	if(ret != LZMA_OK)
		return false;

	byte sink[64];

	strm.next_in  = header;
	strm.avail_in = sizeof(header);

	while(strm.avail_in > 0) {
		strm.next_out  = sink;
		strm.avail_out = sizeof(sink);

		ret = lzma_code(&strm, LZMA_RUN);

		if(ret != LZMA_OK) {
			lzma_end(&strm);
			return false;
		}
	}
#endif

	byte inbuf[LZMA_IO_BUFFER_SIZE];
	byte outbuf[LZMA_IO_BUFFER_SIZE];

	bool eof = false;
	bool ok = true;

	strm.next_in = nullptr;
	strm.avail_in = 0;

	for(;;) {
		if(strm.avail_in == 0 && !eof) {
			int64 n = in.Get64(inbuf, sizeof(inbuf));

			if(n < 0 || in.IsError()) {
				ok = false;
				break;
			}

			if(n == 0) {
				eof = true;
			}
			else {
				strm.next_in = inbuf;
				strm.avail_in = (size_t)n;
			}
		}

		strm.next_out = outbuf;
		strm.avail_out = sizeof(outbuf);

		ret = lzma_code(&strm, eof ? LZMA_FINISH : LZMA_RUN);

		size_t produced = sizeof(outbuf) - strm.avail_out;

		if(produced && !Write(out, outbuf, produced, written)) {
			ok = false;
			break;
		}

		if(ReportProgress(progress, strm.total_out, unpackSize)) {
			ok = false;
			break;
		}

		if(ret == LZMA_STREAM_END)
			break;

		if(ret != LZMA_OK) {
			ok = false;
			break;
		}
	}

	lzma_end(&strm);
#if HAVE_LZMAEXT
	lzma_filters_free(filters, nullptr);
#endif

	/*
		The size in the .lzma header is authoritative.
		A declared size of unknown (UINT64_MAX, the streamed
		variant written by the encoder on liblzma < 5.4)
		cannot be checked this way - the EOPM already
		terminated the stream successfully.
	*/
	if(ok && unpackSize != UINT64_MAX && (uint64)written != unpackSize)
		ok = false;

	return ok && !in.IsError() && !out.IsError();
}

int64 LZMACompress(Stream& out, Stream& in, int64 size,
                   Gate2<int64, int64> progress, int lvl)
{
	int64 written = 0;

	if(!Encode(out, in, size, progress, lvl, written))
		return -1;

	return written;
}


int64 LZMACompress(Stream& out, Stream& in,
                   Gate2<int64, int64> progress, int lvl)
{
	return LZMACompress(out, in, in.GetLeft(), progress, lvl);
}


String LZMACompress(const void *data, int64 len,
                    Gate2<int64, int64> progress, int lvl)
{
	StringStream out;
	MemReadStream in(data, len);

	if(LZMACompress(out, in, len, progress, lvl) >= 0)
		return out;

	return String::GetVoid();
}


String LZMACompress(const String& s,
                    Gate2<int64, int64> progress, int lvl)
{
	return LZMACompress(~s, s.GetLength(), progress, lvl);
}


int64 LZMADecompress(Stream& out, Stream& in,
                     Gate2<int64, int64> progress)
{
	int64 written = 0;

	if(!Decode(out, in, progress, written))
		return -1;

	return written;
}


String LZMADecompress(const void *data, int64 len,
                      Gate2<int64, int64> progress)
{
	StringStream out;
	MemReadStream in(data, len);

	if(LZMADecompress(out, in, progress) >= 0)
		return out;

	return String::GetVoid();
}


String LZMADecompress(const String& s,
                      Gate2<int64, int64> progress)
{
	return LZMADecompress(~s, s.GetLength(), progress);
}


bool LZMACompressFile(const char *dstfile, const char *srcfile,
                      Gate2<int64, int64> progress, int lvl)
{
	FileIn in(srcfile);
	if(!in)
		return false;

	FileOut out(dstfile);
	if(!out)
		return false;

	if(LZMACompress(out, in, (int64)in.GetLeft(), progress, lvl) < 0)
		return false;

	out.Close();
	return !out.IsError();
}


bool LZMACompressFile(const char *srcfile,
                      Gate2<int64, int64> progress, int lvl)
{
	String dstfile = String(srcfile) + ".lzma";
	return LZMACompressFile(dstfile, srcfile, progress, lvl);
}


bool LZMADecompressFile(const char *dstfile, const char *srcfile,
                        Gate2<int64, int64> progress)
{
	FileIn in(srcfile);
	if(!in)
		return false;

	FileOut out(dstfile);
	if(!out)
		return false;

	if(LZMADecompress(out, in, progress) < 0)
		return false;

	out.Close();
	return !out.IsError();
}


bool LZMADecompressFile(const char *srcfile,
                        Gate2<int64, int64> progress)
{
	String dstfile = srcfile;

	if(dstfile.EndsWith(".lzma"))
		dstfile.Trim(dstfile.GetLength() - 5);
	else
		return false;

	return LZMADecompressFile(dstfile, srcfile, progress);
}

}