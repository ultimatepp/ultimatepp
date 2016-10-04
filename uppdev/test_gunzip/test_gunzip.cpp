#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	FileOut out("u:/gztest/output.bin");
	FileIn in;

	if (!in.Open("u:/gztest/MedicalEnRu_abrv.idx.gz"))
		return;
	
	Buffer<char> index_data;
	MemStream index_stream;
	const int idx_file_size = 48;

	// Preallocate memory and create a memory stream ...
	index_data.Alloc(idx_file_size);
	index_stream.Create(~index_data, idx_file_size);

	GZDecompress(index_stream, in, in.GetLeft());
	index_stream.Seek(0);
			
	CopyStream(out, index_stream);
}
