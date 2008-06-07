#include "TCoreDb.h"

NAMESPACE_UPP

static const char tag[] =
"CDF: Cybex Data File version 1.0.\r\n"
"Copyright ©2000 Cybex Development, spol. s r.o.\r\n"
"\x1A";

DataFile::Header::Header()
: version(VERSION)
, object_count(0)
, index_offset(0)
{
}

bool DataFile::Header::Serialize(Stream& stream, bool write)
{
	if(write) {
		stream.SetStoring();
		memset(tag, 0, sizeof(tag));
		CDB_ASSERT(sizeof(UPP::tag) < sizeof(tag));
		strcpy(tag, UPP::tag);
		version = VERSION;
	}
	else
		stream.SetLoading();
	stream.Seek(0);
	stream.SerializeRaw((byte *)tag, sizeof(UPP::tag));
	if(!write && strcmp(tag, UPP::tag))
		return false;
	StreamIL(stream, version);
	if(version < MIN_VERSION || version > MAX_VERSION)
		return false;
	StreamIL(stream, object_count);
	StreamIL(stream, index_offset);
	if(write)
		stream.Put(int(0), int(TOTAL - stream.GetPos()));
	return true;
}

DataFile::Cache::Cache(int size)
: size(size)
, total(0)
{
}

void DataFile::Cache::Clear()
{
	total = 0;
	data.Clear();
	dirty.Clear();
}

void DataFile::Cache::Put(int object, const String& value, bool write)
{
	CDB_ASSERT(dirty.GetCount() <= data.GetCount());
	int i = Find(object);
	total += value.GetLength() - (i >= 0 ? data[i].GetLength() : 0);
	if(total > size) {
		Remove(object);
		WhenFlush();
		Clear();
		total = value.GetLength();
		data.Add(object, value);
	}
	else if(i >= 0)
		data[i] = value;
	else
		data.Add(object, value);
	if(write)
		dirty.FindAdd(object);
	CDB_ASSERT(dirty.GetCount() <= data.GetCount());
}

void DataFile::Cache::Remove(int object)
{
	CDB_ASSERT(dirty.GetCount() <= data.GetCount());
	int i;
	if((i = data.Find(object)) >= 0) {
		total -= data[i].GetLength();
		data.Remove(i);
	}
	if((i = dirty.Find(object)) >= 0)
		dirty.Remove(i);
	CDB_ASSERT(dirty.GetCount() <= data.GetCount());
}

void DataFile::Cache::RemoveAbove(int object)
{
	CDB_ASSERT(dirty.GetCount() <= data.GetCount());
	int i;
	for(i = data.GetCount(); --i >= 0;)
		if(data.GetKey(i) >= object) {
			total -= data[i].GetLength();
			data.Remove(i);
		}
	for(i = dirty.GetCount(); --i >= 0;)
		if(dirty[i] >= object)
			dirty.Remove(i);
	CDB_ASSERT(dirty.GetCount() <= data.GetCount());
}

DataFile::DataFile()
: large_block_size(LARGE_BLOCK_SIZE)
, large_cache(LARGE_CACHE_SIZE)
, small_cache(SMALL_CACHE_SIZE)
, free_offset(Header::TOTAL)
{
	large_cache.WhenFlush = THISBACK(FlushLargeCache);
	small_cache.WhenFlush = THISBACK(FlushSmallCache);
}

DataFile::~DataFile()
{
	Close();
}

bool DataFile::Open(const char *_filename, bool _read_only)
{
	Close();
	filename = GetFullPath(_filename);
	read_only = _read_only;
	if(!stream.Open(filename, read_only ? FileStream::READ : FileStream::READWRITE | FileStream::NOWRITESHARE))
		return false;
	block_level = 0;
	if(!ReadIndex()) {
		Close();
		return false;
	}

//	CHECK_INTEGRITY();

	return true;
}

bool DataFile::Create(const char *_filename)
{
	Close();
	filename = GetFullPath(_filename);
	read_only = false;
	block_level = 0;
	if(!stream.Open(filename, FileStream::CREATE))
		return false;
	header = Header();
	StreamHeader(true);
	return true;
}

bool DataFile::Close()
{
	if(!IsOpen())
		return true;
	bool error = false;
	Clear();
	stream.Close();
	return !error && !stream.IsError();
}

void DataFile::Clear()
{
	small_cache.Clear();
	large_cache.Clear();
	index.Clear();
	write_id.Clear();
	write_index.Clear();
	write_objects.Clear();
	free_objects.Clear();
	free_list.Clear();
	free_list_index.Clear();
}

Time DataFile::GetLastWriteTime() const
{
	ASSERT(IsOpen());
#ifdef PLATFORM_WIN32
	FileTime ft;
	if(!GetFileTime(stream.GetHandle(), 0, 0, &ft))
		return Null;
	return ft;
#endif
#ifdef PLATFORM_POSIX
	struct stat sb;
	if(fstat(stream.GetHandle(), &sb))
		return Null;
	return FileTime(sb.st_mtime);
#endif//PLATFORM_POSIX
}

int DataFile::GetOverhead() const
{
	foff_t size = Header::TOTAL + GetBlockSize(index.GetCount() * OBJECT_BYTES);
	for(int i = 0; i < index.GetCount(); i++)
		if(index[i].length > 0)
			size += GetBlockSize(index[i].length);
	return iscale(max<foff_t>(0, (int)stream.GetSize() - size), 100, max(size, 1));
}

bool DataFile::SaveAs(const char *new_file_name)
{
	Commit(0);
	DataFile nf;
	if(!nf.Create(new_file_name))
		return false;
	nf.SetCount(index.GetCount());
	for(int i = 0; i < index.GetCount(); i++)
		nf.Set(i, Get(i), 0);
	nf.Commit(0);
	if(!nf.Close()) {
		DeleteFile(new_file_name);
		return false;
	}
	Close();
	return Open(new_file_name);
}

bool DataFile::Compact(const char *backup_file_name)
{
	if(read_only) {
		NEVER();
		return false;
	}
	String temp = ForceExt(filename, ".$$$");
	DataFile aux;
	if(!aux.Create(temp))
		return false;
	aux.SetCount(index.GetCount());
	for(int i = 0; i < index.GetCount(); i++)
		aux.Set(i, Get(i), 0);
	aux.Commit(0);
	if(!aux.Close()) {
		DeleteFile(temp);
		return false;
	}
	if(backup_file_name && !FileCopy(temp, backup_file_name))
		return false;
	Close();
	FileDelete(filename);
	return FileMove(temp, filename) && Open(filename);
}

void DataFile::BeginBlock()
{
	block_level++;
}

void DataFile::EndBlock()
{
	ASSERT(block_level > 0);
	if(--block_level <= 0)
		CommitFile();
}

void DataFile::CommitFile()
{
	ASSERT(block_level <= 0);
	int start_msecs = msecs();
	LOG("DataFile::CommitFile");

	if(header.index_offset && header.object_count)
		free_upon_commit.Add(Object(header.index_offset, header.object_count * OBJECT_BYTES));

	LOG("DataFile::CommitFile / FlushIndex: t + " << msecs(start_msecs));
	FlushIndex();
	LOG("DataFile::CommitFile / FileFlush: t + " << msecs(start_msecs));
	FileFlush(stream);
	LOG("DataFile::CommitFile / StreamHeader: t + " << msecs(start_msecs));
	StreamHeader(true);
	LOG("DataFile::CommitFile / FileFlush: t + " << msecs(start_msecs));
	FileFlush(stream);

//	CHECK_INTEGRITY();

	LOG("DataFile::CommitFile / FreeBlock: t + " << msecs(start_msecs));
	while(!free_upon_commit.IsEmpty())
		FreeBlock(free_upon_commit.Pop());

	LOG("//DataFile::CommitFile: t + " << msecs(start_msecs));

//	CHECK_INTEGRITY();
}

void DataFile::Commit(dfid_t id)
{
	CDB_TIMING("DataFile::Commit");
	LOG("DataFile::Commit");
	int start_msecs = msecs();

	CDB_ASSERT(IsOpen());
	if(read_only || id && write_id.Find(id) < 0)
		return;

	LOG("DataFile::Commit / FlushSmallCache: t + " << msecs(start_msecs));
	FlushSmallCacheFor(id);
	LOG("DataFile::Commit / FlushLargeCache: t + " << msecs(start_msecs));
	FlushLargeCacheFor(id);

	LOG("DataFile::Commit / live objects: t + " << msecs(start_msecs));
	if(id == 0) { // commit everything
		for(int i = 0; i < write_index.GetCount(); i++) {
			int x = write_index[i];
			CDB_ASSERT(x >= 0 && x < index.GetCount());
			Object& o = index[x];
			if(o.IsAlive())
				free_upon_commit.Add(o);
			o = write_objects[x];
			CDB_ASSERT(o.length <= 0 || o.offset >= Header::TOTAL);
		}
		write_id.Clear();
		write_index.Clear();
	}
	else {
		Vector<int> to_remove;
		bool to_sort = false;
		for(int i = write_id.Find(id); i >= 0; i = write_id.FindNext(i)) {
			int x = write_index[i];
			CDB_ASSERT(x >= 0 && x < index.GetCount());
			Object& o = index[x];
			if(o.IsAlive())
				free_upon_commit.Add(o);
			o = write_objects[x];
			to_sort = to_sort || (!to_remove.IsEmpty() && i < to_remove.Top());
			to_remove.Add(i);
		}
		if(to_sort)
			Sort(to_remove);
		while(!to_remove.IsEmpty()) {
			int i = to_remove.Pop();
			write_id.Remove(i);
			write_index.Remove(i);
		}
	}

	LOG("DataFile::Commit / CommitFile: t + " << msecs(start_msecs));
	if(block_level <= 0)
		CommitFile();
	LOG("//DataFile::Commit: t + " << msecs(start_msecs));
}

void DataFile::Rollback(dfid_t id)
{
	CHECK_INTEGRITY();

	if(id == 0) { // full rollback
		for(int i = 0; i < write_index.GetCount(); i++) {
			int x = write_index[i];
			FreeBlock(write_objects[x]);
			write_objects[x] = index[x];
		}
		write_id.Clear();
		write_index.Clear();
		small_cache.Clear();
		large_cache.Clear();
	}
	else {
		Vector<int> to_remove;
		bool to_sort = false;
		for(int i = write_id.Find(id); i >= 0; i = write_id.FindNext(i)) {
			int x = write_index[i];
			small_cache.Remove(x);
			large_cache.Remove(x);
			CDB_ASSERT(x >= 0 && x < index.GetCount());
			FreeBlock(write_objects[x]);
			write_objects[x] = index[x];
			to_sort = to_sort || (!to_remove.IsEmpty() && i < to_remove.Top());
			to_remove.Add(i);
		}
		if(to_sort)
			Sort(to_remove);
		while(!to_remove.IsEmpty()) {
			int i = to_remove.Pop();
			write_id.Remove(i);
			write_index.Remove(i);
		}
	}

	CHECK_INTEGRITY();
}

bool DataFile::Import(Stream& stream)
{
	String line;
	for(;;) {
		const char *p = line;
		while(*p && *p != '#' && *p++ != '[')
			;
		if(*p == ']')
			return true; // end of import
		if(*p == '=') { // item count
			int count = stou(p + 1);
			if(count < 0)
				return false;
			SetCount(count);
		}
		else if(*p == '-') { // delete item
			int item = stou(p + 1);
			if(IsValidIndex(item))
				Remove(item, 0);
		}
		else if(IsDigit(*p)) { // item value
			int index = stou(p, &p);
			if(!IsValidIndex(index) || *p++ != ']')
				continue;
			String data;
			while(*p != '[') {
				while(*p && (byte)*p <= ' ')
					p++;
				byte c1, c2;
				while((c1 = ctoi(p[0])) < 16 && (c2 = ctoi(p[1])) < 16) {
					data.Cat(c1 * 16 + c2);
					p += 2;
				}
				if(stream.IsEof())
					break;
				line = stream.GetLine();
				p = line;
			}
			Set(index, data, 0);
			continue;
		}
		if(stream.IsEof())
			return true;
		line = stream.GetLine();
	}
}

void DataFile::Export(Stream& stream)
{
	stream.PutLine("# Cybex DataFile export version 1.0");
	stream.Put("# Export time: ");
	stream.Put(Format(GetSysTime()));
	stream.PutCrLf();
	int c = GetCount();
	stream.Put(Format("[=%d]", c));
	stream.PutCrLf();
	for(int i = 0; i < c; i++)
		if(!IsVoid(i)) {
			String item = Get(i);
			stream.Put(Format("[%d]", i));
			const char *p = item.Begin(), *e = item.End();
			if(p == e)
				stream.PutCrLf();
			else
				do {
					const char *b = p + min<ptrdiff_t>(e - p, 32);
					stream.Put('\t');
					static const char hex[] = "0123456789abcdef";
					while(p < b) {
						stream.Put(hex[(*p >> 4) & 15]);
						stream.Put(hex[*p & 15]);
						p++;
					}
					stream.PutCrLf();
				}
				while(p < e);
		}
	stream.PutLine("[] # end of export");
}

void DataFile::SetCache(int lbs, int scs, int lcs)
{
	FlushLargeCache();
	FlushSmallCache();
	large_block_size = lbs;
	small_cache.size = scs;
	large_cache.size = lcs;
}

void DataFile::SetCount(int count)
{
	int old = GetCount();
	if(count < old) {
		int i, x;
		for(i = count; i < old; i++)
			FreeBlock(index[i]);

		for(i = write_index.GetCount(); --i >= 0;)
			if((x = write_index[i]) >= count) { // remove item
				FreeBlock(write_objects[x]);
				write_id.Remove(i);
				write_index.Remove(i);
			}

	}
	write_objects.SetCountR(count);
	index.SetCountR(count);
}

String DataFile::Get(int object)
{
//	if(object == 313 || object == 80)
//		__asm int 3

	CDB_TIMING("DataFile::Get");

//	CHECK_INTEGRITY();
	int i;
	if((i = small_cache.Find(object)) >= 0)
		return small_cache[i];
	if((i = large_cache.Find(object)) >= 0)
		return large_cache[i];
	if(!IsValidIndex(object)) {
		NEVER();
		return String::GetVoid();
	}
	Object& o = write_objects[object];
	i = o.length;
	if(i < 0)
		return String::GetVoid();
	if(i == 0)
		return Null;
	CDB_ASSERT(o.offset >= Header::TOTAL);
	String s = ReadBlock(o.offset, i);
	Cache& cache = (i >= large_block_size ? large_cache : small_cache);
	if(i <= cache.size)
		cache.Put(object, s, false);
	return s;
}

void DataFile::Set(int object, const String& data, dfid_t id)
{
	CDB_TIMING("DataFile::Set");
	ASSERT(!IsReadOnly());

//	CHECK_INTEGRITY();
	LOG("Put(" << object << ", " << data.GetLength() << "); stream size = " << stream.GetSize());
	if(object < 0) {
		NEVER();
		return;
	}
	if(object >= GetCount())
		SetCount(object + 1);
	int w = write_index.Find(object);
	if(w >= 0) { // object has been already written to - check id equality
		if(write_id[w] != id) {
			String s;
			s << "<DataFile::Set(" << object << "), ID = " << id << " / " << write_id[w]
				<< ">: nesouhlas ID pøi zápisu do datového souboru";
			throw ObjectExc(s);
		}
		Object& o = write_objects[object];
		foff_t offset = o.offset;
		o.offset = 0;
		if(offset && o.length > 0)
			FreeBlock(offset, o.length);
	}
	else { // add new update entry to write table
		w = write_index.GetCount();
		write_id.Add(id);
		write_index.Add(object);
		ASSERT(write_id.GetCount() == write_index.GetCount());
	}
	Object& o = write_objects[object];
	o.offset = 0;
	int l = data.GetLength();
	if(l == 0) { // clear item
		small_cache.Remove(object);
		large_cache.Remove(object);
	}
	else if(l >= large_block_size) {
		small_cache.Remove(object);
		if(l <= large_cache.size)
			large_cache.Put(object, data, true);
		else
			o.offset = WriteBlock(data);
	}
	else {
		large_cache.Remove(object);
		if(l <= small_cache.size)
			small_cache.Put(object, data, true);
		else
			o.offset = WriteBlock(data);
	}
	o.length = l;
//	CHECK_INTEGRITY();
}

int DataFile::Add(dfid_t id)
{
	CDB_TIMING("DataFile::Add");

	if(!free_objects.IsEmpty())
		return free_objects.Pop();
	int c = GetCount();
	SetCount(c + 1);
	int w = write_index.Find(c);
	if(w >= 0)
		write_id.Set(w, id);
	return c;
}

void DataFile::Remove(int object, dfid_t id)
{
	CDB_TIMING("DataFile::Remove");

//	CHECK_INTEGRITY();

	if(!IsValidIndex(object)) {
		NEVER();
		return;
	}
	large_cache.Remove(object);
	small_cache.Remove(object);
	int w = write_index.Find(object);
	if(w >= 0) {
		if(write_id[w] != id) {
			String s;
			s << "<DataFile::Remove(" << object << "), ID = " << write_id[w] << " -> " << id
				<< ">: nesouhlas ID pøi mazání objektu";
			throw ObjectExc(s);
		}
		Object& o = write_objects[object];
		FreeBlock(o);
		o.length = -1;
	}
	else {
		w = write_index.GetCount();
		write_id.Add(id);
		write_index.Add(object);
		write_objects[object].length = -1;
	}

//	CHECK_INTEGRITY();
}

bool DataFile::StreamHeader(bool write)
{
	CDB_ASSERT(IsOpen());
	CDB_ASSERT(!write || !read_only);
	return header.Serialize(stream, write);
}

bool DataFile::ReadIndex()
{
	if(!StreamHeader(false))
		return false;
	int index_block_length = header.object_count * OBJECT_BYTES;
	String raw_index = ReadBlock(header.index_offset, index_block_length);
	if(raw_index.GetLength() != index_block_length)
		return false;
	index.SetCount(header.object_count);
	foff_t sum_length = Header::TOTAL + GetBlockSize(index_block_length) - GRANULARITY + 1;
	foff_t file_length = (int)stream.GetSize();
	for(int i = 0; i < index.GetCount(); i++) {
		const char *p = raw_index.Begin() + OBJECT_BYTES * i;
		Object& obj = index[i];
		if((obj.length = Peek32le(p + OFFSET_BYTES)) > 0) {
			obj.offset = Peek32le(p);
			if(obj.offset < Header::TOTAL || obj.offset >= file_length)
				return false; // block damaged
			sum_length += GetBlockSize(obj.length);
		}
	}
	write_objects <<= index;
	if(sum_length > (foff_t)stream.GetSize())
		return false; // file consistence violated
	CreateFreeList();
	return true;
}

String DataFile::ReadBlock(foff_t offset, int length)
{
	CDB_TIMING("DataFile::ReadBlock");

	if(length <= 0)
		return Null;
	int total = length;
	int reserve = length + OFFSET_BYTES - 1;
	if(length <= BLOCK_BYTES) { // simple storage
		StringBuffer result(length);
		Read(result, offset, length);
		return result;
	}
	int buffer[10];
	buffer[0] = length;
	buffer[1] = (length = ((length - 1) >> LOG2_BLOCK_BYTES) + 1) << LOG2_OFFSET_BYTES;
	int depth = 1;
	while(length > BLOCK_OFFSETS) {
		length = ((length - 1) >> LOG2_BLOCK_OFFSETS) + 1;
		buffer[++depth] = length << LOG2_OFFSET_BYTES;
	}
	if(buffer[depth] > reserve)
		ThrowRead();
	StringBuffer result(reserve);
	char *ptr = result;
	char *off = ptr + reserve - buffer[depth];
	Read(off, offset, buffer[depth]);
	do { // indirection step
		int dl = buffer[--depth];
		const char *os = off;
		if(dl > reserve)
			ThrowRead();
		char *od = off = (depth ? ptr + reserve - dl : ptr);
		while(dl > 0) {
			foff_t foff = Peek32le(os);
			os += OFFSET_BYTES;
			int part = min<int>(dl, BLOCK_BYTES);
			Read(od, foff, part);
			od += part;
			dl -= part;
		}
	}
	while(depth);

	result.SetCount(total);
	return result;
}

void DataFile::EnumBlock(Array<Object>& dest, foff_t offset, int length)
{
	CDB_TIMING("DataFile::EnumBlock");

	if(length <= 0)
		return;

	if(length <= BLOCK_BYTES) {
		dest.Add(Object(offset, (length + GRANULARITY - 1) & -GRANULARITY));
		return;
	}

	struct Layer : public Vector<byte> {
		int          avail;
		int          count;
		int          index;
	};

	Layer layers[10];
	layers[0].count = length;
	layers[1].count = length = ((length - 1) >> LOG2_BLOCK_BYTES) + 1;
	layers[1].index = 0;
	layers[1].SetCount(BLOCK_BYTES);
	int layer = 1;
	while(length > BLOCK_OFFSETS) {
		layer++;
		layers[layer].count = length = ((length - 1) >> LOG2_BLOCK_OFFSETS) + 1;
		layers[layer].index = 0;
		layers[layer].SetCount(BLOCK_BYTES);
	}

	int len = ((layers[layer].avail = layers[layer].count) << LOG2_OFFSET_BYTES);
	Read(layers[layer].Begin(), offset, len);
	dest.Add(Object(offset, (len + GRANULARITY - 1) & -GRANULARITY));
	int current = layer;
	for(;;) {
		while(layers[current].index >= layers[current].avail) // finished layer - step up one layer
			if(++current > layer)
				return; // done
		offset = Peek32le(layers[current].Begin() + OFFSET_BYTES * layers[current].index++);
		if(current == 1) { // final block - add to block list
			length = min<int>(layers[0].count, BLOCK_BYTES);
			layers[0].count -= length;
			dest.Add(Object(offset, (length + GRANULARITY - 1) & -GRANULARITY));
		}
		else { // pointer block - step down one block
			Layer& l = layers[--current];
			l.index = 0;
			l.avail = min<int>(l.count, BLOCK_OFFSETS);
			l.count -= l.avail;
			len = l.avail << LOG2_OFFSET_BYTES;
			Read(l.Begin(), offset, len);
			dest.Add(Object(offset, (len + GRANULARITY - 1) & -GRANULARITY));
		}
	}
}

void DataFile::CheckIntegrity()
{
	CDB_TIMING("DataFile::CheckIntegrity");

	int i;
//*
	int f;
	foff_t start = Header::TOTAL;
	Array<Object> raw;
	EnumBlock(raw, header.index_offset, header.object_count * OBJECT_BYTES);

	ASSERT(index.GetCount() == write_objects.GetCount());
	for(i = 0; i < index.GetCount(); i++) {
		Object& o = index[i];
		Object& w = write_objects[i];
		if(o.IsAlive())
			EnumBlock(raw, o.offset, o.length);
		if(w.IsAlive() && (!o.IsAlive() || o.offset != w.offset || o.length != w.length))
			EnumBlock(raw, w.offset, w.length);
	}

	Sort(raw, &Object::OffsetLess);

	for(i = f = 0; i < raw.GetCount() || f < free_list.GetCount();) {
		if(i >= raw.GetCount() || f < free_list.GetCount() && free_list[f].offset < raw[i].offset) { // free item
			const Object& free = free_list[f++];
			if(free.offset < start) {
				String dump;
				dump << "CheckIntegrity error\n"
					"\n"
					"free_list[" << (f - 1) << "].offset = " << free.offset
					<< ", end of previous = " << start << " (length = " << free.length << ")\n";
				fputs(dump, stderr);
				NEVER();
			}
			start = free.offset + free.length;
		}
		else {
			const Object& o = raw[i++];
			if(o.offset == 0 || o.length <= 0)
				continue;
			if(o.offset < start) { // object crash - find object index
				int i = index.GetCount();
				while(--i >= 0 && index[i].offset != o.offset)
					;
				String dump;
				dump << "CheckIntegrity error\n"
					"\n"
					"object[" << i << "].offset = " << o.offset
					<< ", end of previous = " << start << " (length = " << o.length << ")\n";
				fputs(dump, stderr);
				NEVER();
			}
			start = o.offset + o.length;
		}
	}
//*/
	for(i = 1; i < free_list_index.GetCount(); i++)
		if(!Object::PtrLess(free_list_index[i - 1], free_list_index[i])) {
			String dump;
			dump << "CheckIntegrity error\n"
				"\n"
				"free_list_index[" << (i - 1) << "] > free_list_index[" << i << "]\n";
			fputs(dump, stderr);
			NEVER();
		}
}

void DataFile::CheckAvailability(foff_t start, int length)
{
	CDB_TIMING("DataFile::CheckAvailability");

	if(length <= 0)
		return;
	for(int i = 0; i < index.GetCount(); i++) {
		const Object& o = index[i];
		if(o.length > 0 && o.offset) {
			Array<Object> aobj;
			EnumBlock(aobj, o.offset, o.length);
			for(int t = 0; t < aobj.GetCount(); t++)
				VERIFY(aobj[t].offset + aobj[t].length <= start || start + length <= aobj[t].offset);
		}
	}
}

foff_t DataFile::AllocatePart(int block_size)
{
	CDB_TIMING("AllocatePart");
//	CHECK_INTEGRITY();

	block_size = (block_size + GRANULARITY - 1) & -GRANULARITY;
	int pos = BinFindIndex(free_list_index, block_size, &Object::PtrLessLength);
	if(pos < free_list_index.GetCount()) { // found in free list - allocate block
		Object *p = free_list_index[pos];
		foff_t result = p -> offset;
		if(p -> length < block_size + GRANULARITY) { // allocate the whole block
			int ind = BinFindIndex(free_list, free_list_index[pos] -> offset, &Object::LessOffset);
			CDB_ASSERT(ind < free_list.GetCount());
			free_list_index.Remove(pos);
			free_list.Remove(ind);
		}
		else { // modify block & move pointer around in free index
			p -> offset += block_size;
			p -> length -= block_size;
			ReorderFreeItem(pos);
		}
//		LOG("AllocatePart(" << block_size << ") -> free " << result);
		return result;
	}

//	CHECK_INTEGRITY();

	foff_t foff = free_offset;
	free_offset += block_size;

//	LOG("AllocatePart(" << block_size << ") -> more " << foff);
	return foff;
}

void DataFile::FreePart(foff_t offset, int length)
{
	CDB_TIMING("DataFile::FreePart");

	if(length <= 0)
		return;

//	CHECK_INTEGRITY();
	length = (length + GRANULARITY - 1) & -GRANULARITY;

//	LOG("FreePart(" << offset << ", " << length << ")");
	int pos = BinFindIndex(free_list, offset, &Object::LessOffset);
	if(pos > 0) {
		Object& prev = free_list[pos - 1];
		if(prev.offset + prev.length == offset) { // connect to previous block
			int ind = BinFindIndex(free_list_index, &prev, &Object::PtrLess);
			CDB_ASSERT(ind < free_list_index.GetCount());
			int new_length = prev.length + length;
			if(pos < free_list.GetCount() && free_list[pos].offset == prev.offset + new_length) { // connect with the following block as well
				int ind2 = BinFindIndex(free_list_index, &free_list[pos], &Object::PtrLess);
				CDB_ASSERT(ind2 < free_list_index.GetCount() && ind2 != ind);
				prev.length += free_list[pos].length;
				free_list.Remove(pos);
				free_list_index.Remove(ind2);
				if(ind2 < ind)
					ind--;
			}
			else
				prev.length = new_length;
			ReorderFreeItem(ind);
			return;
		}
	}
	if(pos < free_list.GetCount() && free_list[pos].offset == offset + length) { // join with next
		Object& next = free_list[pos];
		int ind = BinFindIndex(free_list_index, &next, &Object::PtrLess);
		CDB_ASSERT(ind < free_list_index.GetCount());
		next.offset = offset;
		next.length += length;
		ReorderFreeItem(ind);
		return;
	}
//	CHECK_INTEGRITY();

	// insert new entry
	Object *p = new Object(offset, length);
	free_list.Insert(pos, p);
	int ind = BinFindIndex(free_list_index, p, &Object::PtrLess);
	free_list_index.Insert(ind, p);

//	CHECK_INTEGRITY();
}

int DataFile::WriteBlock(const String& data)
{
	CDB_TIMING("DataFile::WriteBlock");

	int length = data.GetLength();
	if(length == 0)
		return 0;
	if(length <= BLOCK_BYTES) { // direct write
//		CHECK_INTEGRITY();
		foff_t offset = AllocatePart(length);
		CHECK_AVAILABILITY(offset, length);
		Write(data, offset, length);
		return offset;
	}
	Vector< Vector<byte> > offset_map;
	const char *p = data;
	while(length > 0) {
		int part = min<int>(length, BLOCK_BYTES);
		foff_t poff = AllocatePart(part);
		CHECK_AVAILABILITY(poff, part);
		Write(p, poff, part);
		p += part;
		length -= part;
		int pix = 0;
		for(;;) {
			if(pix >= offset_map.GetCount())
				offset_map.Add().Reserve(BLOCK_BYTES);
			Vector<byte>& v = offset_map[pix];
			int q = v.GetCount();
			v.InsertN(q, OFFSET_BYTES);
			Poke32le(&v[q], poff);
			if(v.GetCount() < BLOCK_BYTES)
				break;
			poff = AllocatePart(BLOCK_BYTES);
			CHECK_AVAILABILITY(poff, BLOCK_BYTES);
			Write(v.Begin(), poff, BLOCK_BYTES);
			v.Clear();
			pix++;
		}
	}
	int t = 0;
	while(t < offset_map.GetCount() && offset_map[t].IsEmpty())
		t++;
	if(t >= offset_map.GetCount())
		ThrowWrite(); // internal error
	int poff;
	for(;;) {
		Vector<byte>& v = offset_map[t];
		poff = AllocatePart(v.GetCount());
		CHECK_AVAILABILITY(poff, v.GetCount());
		Write(v.Begin(), poff, v.GetCount());
		if(++t >= offset_map.GetCount())
			return poff;
		int q = offset_map[t].GetCount();
		offset_map[t].InsertN(q, OFFSET_BYTES);
		Poke32le(&offset_map[t][q], poff);
	}
}

void DataFile::FlushSmallCacheFor(dfid_t id)
{
	CDB_TIMING("DataFile::FlushSmallCache");

//	CHECK_INTEGRITY();
	CDB_ASSERT(small_cache.dirty.GetCount() <= small_cache.data.GetCount());
	dfid_t mask = dfid_t(id ? -1 : 0);
	for(int i = small_cache.dirty.GetCount(); --i >= 0;) {
		int b = small_cache.dirty[i], t, w;
		if(IsValidIndex(b) && (t = small_cache.Find(b)) >= 0
			&& (w = write_index.Find(b)) >= 0) {
			if((write_id[w] & mask) == id) {
				CDB_ASSERT(write_objects[b].length == small_cache[t].GetLength());
				write_objects[b].offset = WriteBlock(small_cache[t]);
				small_cache.dirty.Remove(i);
			}
		}
		else
			NEVER();
	}
//	CHECK_INTEGRITY();
}

void DataFile::FlushLargeCacheFor(dfid_t id)
{
	CDB_TIMING("DataFile::FlushLargeCache");

//	CHECK_INTEGRITY();
	CDB_ASSERT(large_cache.dirty.GetCount() <= large_cache.data.GetCount());
	dfid_t mask = dfid_t(id ? -1 : 0);
	for(int i = large_cache.dirty.GetCount(); --i >= 0;) {
		int b = large_cache.dirty[i], t, w;
		if(IsValidIndex(b) && (t = large_cache.Find(b)) >= 0
			&& (w = write_index.Find(b)) >= 0) {
			if((write_id[w] & mask) == id) {
				CDB_ASSERT(write_objects[b].length == large_cache[t].GetLength());
				write_objects[b].offset = WriteBlock(large_cache[t]);
				large_cache.dirty.Remove(i);
			}
		}
		else
			NEVER();
	}
	CHECK_INTEGRITY();
}

int DataFile::GetBlockSize(int length)
{
	int sum = (length + GRANULARITY - 1) & -GRANULARITY;
	if(length > BLOCK_BYTES) {
		length = ((length - 1) >> LOG2_BLOCK_BYTES) + 1;
		sum += length << LOG2_OFFSET_BYTES;
		while(length > BLOCK_OFFSETS) {
			length = ((length - 1) >> LOG2_BLOCK_OFFSETS) + 1;
			sum += length << LOG2_OFFSET_BYTES;
		}
	}
	return sum;
}

void DataFile::Read(void *dest, int offset, int length)
{
	CDB_TIMING("DataFile::Read");

	int fs = (int)stream.GetSize();
	if(offset < 0 || length < 0 || length > fs || offset + length > fs)
		ThrowRead();
	if(length <= 0)
		return;
	stream.Seek(offset);
	if(!stream.GetAll(dest, length) || stream.IsError())
		ThrowRead();
}

void DataFile::Write(const void *src, int offset, int length)
{
	CDB_TIMING("DataFile::Write");

	int fs = (int)stream.GetSize();
	if(offset < 0 || length < 0)
		ThrowWrite();
	if(length == 0)
		return;
	if(offset > fs) {
		stream.Seek(fs);
		stream.Put(0, offset - fs);
	}
	else
		stream.Seek(offset);
	stream.Put(src, length);
	if(stream.IsError())
		ThrowWrite();
}

void DataFile::ThrowRead() const
{
	throw ObjectExc("Chyba pøi ètení souboru '" + filename + "'.");
}

void DataFile::ThrowWrite() const
{
	throw ObjectExc("Chyba pøi zápisu do souboru '" + filename + "'.");
}

void DataFile::CreateFreeList()
{
	free_list.Clear();
	free_list_index.Clear();
	free_offset = Header::TOTAL;
	Array<Object> data_blocks;
	EnumBlock(data_blocks, header.index_offset, header.object_count << LOG2_OBJECT_BYTES);
	int i;
	for(i = 0; i < index.GetCount(); i++)
		if(write_index.Find(i) < 0 && index[i].IsAlive())
			EnumBlock(data_blocks, index[i].offset, index[i].length);

	for(i = 0; i < write_objects.GetCount(); i++)
		if(write_objects[i].IsAlive())
			EnumBlock(data_blocks, write_objects[i].offset, write_objects[i].length);

	Sort(data_blocks, &Object::OffsetLess);
	if(data_blocks.IsEmpty())
		return; // no data
	for(i = 0; i < data_blocks.GetCount(); i++) {
		const Object& o = data_blocks[i];
		if(o.offset >= free_offset + GRANULARITY)
			free_list.Add(new Object(free_offset, (o.offset - free_offset) & -GRANULARITY));
		free_offset = o.offset + ((o.length + GRANULARITY - 1) & -GRANULARITY);
	}
	free_list_index.SetCount(free_list.GetCount());
	for(i = 0; i < free_list.GetCount(); i++)
		free_list_index[i] = &free_list[i];
	Sort(free_list_index, &Object::PtrLess);
//	CHECK_INTEGRITY();
}

void DataFile::FreeBlock(foff_t offset, int length)
{
	CDB_TIMING("DataFile::FreeBlock");

	if(offset == 0 || length <= 0)
		return;
//	CHECK_INTEGRITY();
	Array<Object> parts;
	EnumBlock(parts, offset, length);
	for(int i = 0; i < parts.GetCount(); i++)
		FreePart(parts[i].offset, parts[i].length);
//	CHECK_INTEGRITY();
}

void DataFile::ReorderFreeItem(int pos)
{
	CDB_TIMING("DataFile::ReorderFreeItem");

	CDB_ASSERT(pos >= 0 && pos < free_list_index.GetCount());
	Object *p = free_list_index[pos];
	bool lok = (pos == 0 || Object::PtrLess(free_list_index[pos - 1], p));
	bool rok = (pos == free_list_index.GetCount() - 1 || Object::PtrLess(p, free_list_index[pos + 1]));
	if(!rok) { // move item up
		Object **op = free_list_index.Begin() + pos + 1, **oe = free_list_index.End();
		int n = BinFindIndex(op, oe, p, &Object::PtrLess);
		Copy(op - 1, op, op + n);
		op[n - 1] = p;
	}
	else if(!lok) { // move item down
		Object **op = free_list_index.Begin() + pos, **ob = free_list_index.Begin();
		int n = BinFindIndex(ob, op, p, &Object::PtrLess);
		CopyBackward(op + 1, op, ob + n);
		ob[n] = p;
	}
//	CHECK_INTEGRITY();
}

void DataFile::FlushIndex()
{
	CDB_TIMING("DataFile::FlushIndex");
//	CHECK_INTEGRITY();

	StringStream out;
	for(int i = 0; i < index.GetCount(); i++) {
		const Object& o = index[i];
		CDB_ASSERT(o.length <= 0 || o.offset >= Header::TOTAL);
		out.PutIL(o.offset);
		out.PutIL(o.length);
	}
	header.index_offset = WriteBlock(out.GetResult());
	header.object_count = index.GetCount();

//	CHECK_INTEGRITY();
}

Vector<int> DataFile::FindObjects(foff_t start, int length)
{
	Vector<int> list;
	for(int i = 0; i < index.GetCount(); i++) {
		const Object& o = index[i];
		if(!o.IsAlive())
			continue;
		Array<Object> blocks;
		EnumBlock(blocks, o.offset, o.length);
		for(int t = 0; t < blocks.GetCount(); t++)
			if(blocks[t].offset + blocks[t].length > start
				&& blocks[t].offset < start + length) {
				list.Add(i);
				break;
			}
	}
	return list;
}

void DataFile::DumpObjects(const Vector<int>& list, bool dump_data)
{
	for(int i = 0; i < list.GetCount(); i++) {
		int o = list[i];
		if(!IsValidIndex(o))
			RLOG("#" << o << " - invalid");
		else {
			const Object& obj = index[o];
			RLOG("#" << o << " - " << Format("[0x%08x +0x%08x)", obj.offset, obj.length));
			if(dump_data && obj.IsAlive()) {
				String data = Get(o);
				RLOGHEXDUMP(data, data.GetLength());
			}
		}
	}
}

END_UPP_NAMESPACE
