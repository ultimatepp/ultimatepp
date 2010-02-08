#include "TCoreDb.h"
#pragma hdrstop

NAMESPACE_UPP

static String Format(const Vector<Value>& values)
{
	String s;
	if(!values.IsEmpty()) {
		s << StdFormat(values[0]);
		for(int i = 1; i < values.GetCount(); i++)
			s << ", " << StdFormat(values[i]);
	}
	return s;
}

DataClient::DataClient()
: datafile(0)
, master_index(-1)
, cache_size(DEFAULT_CACHE_SIZE)
{
}

DataClient::~DataClient()
{
}

void DataClient::Serialize(Stream& stream)
{
	NEVER();
}

void DataClient::FlushMaster()
{
	datafile->Set(master_index, SerializeOut(*this), id);
	master_dirty = false;
}

void DataClient::StreamMaster(Stream& stream, DataFile& df, dfid_t _id)
{
	if(!IsOpen() && stream.IsStoring())
		Create(df, _id);
	int master = master_index;
	stream % master;
	if(stream.IsLoading())
		Open(df, master, _id);
}

void DataClient::Open(DataFile& _datafile, int _index, dfid_t _id)
{
	Close();
	datafile = &_datafile;
	id = _id;
	commit_index = master_index = _index;
	master_dirty = false;
	DoOpen();
}

int DataClient::Create(DataFile& _datafile, dfid_t _id)
{
	Close();
	datafile = &_datafile;
	id = _id;
	master_index = datafile->Add(id);
	commit_index = -1;
	master_dirty = true;
	DoCreate();
	return master_index;
}

void DataClient::Close()
{
	DoClose();
	datafile = 0;
	master_index = -1;
}

void DataClient::Unlink()
{
	LOG("DataClient::Unlink(" << master_index << ")");
	datafile->Remove(master_index, id);
	commit_index = -1;
	Close();
}

void DataClient::DoOpen()
{
	SerializeIn(*this, datafile->Get(master_index));
}

void DataClient::DoCreate()
{
	FlushMaster();
}

void DataClient::DoClose()
{
}

int DataClient::GetUsedCacheSize() const
{
	return 0;
}

void DataClient::ShrinkCache()
{
}

void DataClient::Rollback()
{
	DataFile *df = datafile;
	CDB_ASSERT(df);
	Close();
	if(commit_index >= 0)
		Open(*df, commit_index, id);
	else
		Create(*df, id);
}

void DataClient::SetCacheSize(int size)
{
	cache_size = size;
}

void DataClient::GetIndex(int *dest, int count, Stream& src)
{
#ifdef CPU_LITTLE_ENDIAN
	if(sizeof(int) == INDEX_BYTES)
		src.Get(dest, count * INDEX_BYTES);
	else
#endif
		for(int i = 0; i < count; i++)
			*dest++ = src.GetIL();
}

void DataClient::SetIndex(Stream& dest, const int *src, int count)
{
#ifdef CPU_LITTLE_ENDIAN
	if(sizeof(int) == INDEX_BYTES)
		dest.Put(src, count * INDEX_BYTES);
	else
#endif
		for(int i = 0; i < count; i++)
			dest.PutIL(*src++);
}

DataColumn::DataColumn()
: row_count(0)
{
}

DataColumn::~DataColumn()
{
}

DataColumnFix::DataColumnFix(int item_width)
: item_width(item_width)
, per_block(DataFile::BLOCK_BYTES / item_width)
{
	CDB_ASSERT(per_block > 0);
	DoClose(); // tidy up
}

DataColumnFix::~DataColumnFix()
{
	Close();
}

void DataColumnFix::Unlink()
{
	for(int i = 0; i < block_index.GetCount(); i++)
		datafile->Remove(block_index[i], id);
	DataColumn::Unlink();
}

void DataColumnFix::Serialize(Stream& stream)
{
	if(stream.IsLoading()) {
		if(stream.GetLeft() < INDEX_BYTES)
			throw ObjectExc(NFormat("Porušení konzistence databáze (DataColumnFix#%d)", master_index));
		row_count = stream.GetIL();
		block_index.SetCount((int)stream.GetLeft() >> LOG2_INDEX_BYTES);
		GetIndex(block_index, stream);
	}
	else {
		stream.PutIL(row_count);
		SetIndex(stream, block_index);
	}
}

void DataColumnFix::DoClose()
{
	block_index.Clear();
	cache.Clear();
	last_block = last_block1 = last_block2 = last_block3 = 0;
	last_index = last_index1 = last_index2 = last_index3 = -1;
}

void DataColumnFix::Commit()
{
	FlushCache();
	if(master_dirty)
		FlushMaster();
}

int DataColumnFix::GetUsedCacheSize() const
{
	return cache.GetCount() * sizeof(Block);
}

void DataColumnFix::ShrinkCache()
{
	ClearCache();
}

void DataColumnFix::ClearCache()
{
	FlushCache();
	cache.Clear();
	last_block = last_block1 = last_block2 = last_block3 = 0;
	last_index = last_index1 = last_index2 = last_index3 = -1;
}

void DataColumnFix::FlushCache()
{
	RHITCOUNT("DataColumnFix::FlushCache");

	if(row_count == 0)
		return;
	int top = block_index.GetCount() - 1, last = (row_count - top * per_block) * item_width;
	int norm = per_block * item_width;
	for(int i = 0; i < cache.GetCount(); i++)
		if(cache.GetKey(i) <= top && cache[i].dirty) {
			datafile->Set(cache[i].index, String(cache[i].buffer,
				cache.GetKey(i) < top ? norm : last), id);
			cache[i].dirty = false;
		}
}

void DataColumnFix::FetchCache(int block)
{
	RHITCOUNT("DataColumnFix::FetchCache");

	if(block == last_index) return;
	if(block == last_index1) { last_index = last_index1; last_block = last_block1; return; }
	if(block == last_index2) { last_index = last_index2; last_block = last_block2; return; }
	if(block == last_index3) { last_index = last_index3; last_block = last_block3; return; }
	RHITCOUNT("DataColumnFix::FetchCache / seek");
	if((last_index = cache.Find(block)) >= 0) {
		last_block = &cache[last_index];
		last_index = block;
	}
	else {
		RHITCOUNT("DataColumnFix::FetchCache / load");
		if((cache.GetCount() + 1) * sizeof(Block) >= (unsigned)cache_size)
			ClearCache();
		last_index = block;
		last_block = &cache.Add(block);
		last_block->dirty = false;
		String object = datafile->Get(last_block->index = block_index[block]);
		memcpy(last_block->buffer, object, object.GetLength());
	}
	last_index3 = last_index2;
	last_block3 = last_block2;
	last_index2 = last_index1;
	last_block2 = last_block1;
	last_index1 = last_index;
	last_block1 = last_block;

/*
	if(last_index < 0 || block != cache.GetKey(last_index))
		RHITCOUNT("DataColumnFix::FetchCache / seek");
		if((last_index = cache.Find(block)) >= 0)
			last_block = &cache[last_index];
		else { // spoiled everything
			RHITCOUNT("DataColumnFix::FetchCache / load");
			if((cache.GetCount() + 1) * sizeof(Block) >= (unsigned)cache_size)
				FlushCache();
			last_index = cache.GetCount();
			last_block = &cache.Add(block);
			last_block->dirty = false;
			String object = datafile->Get(last_block->index = block_index[block]);
			memcpy(last_block->buffer, object, object.GetLength());
		}
*/
}

const byte *DataColumnFix::GetRaw(int index)
{
//	RTIMING("DataColumnFix::GetRaw");
	RHITCOUNT("DataColumnFix::GetRaw");

	CDB_ASSERT(index >= 0 && index < row_count);
	FetchCache(index / per_block);
	return last_block->buffer + item_width * (index % per_block);
}

void DataColumnFix::SetRaw(int index, const void *value)
{
	CDB_ASSERT(index >= 0);
	CDB_ASSERT(!datafile->IsReadOnly());
	if(index >= row_count)
		SetCount(index + ADD_STEP);
	FetchCache(index / per_block);
	memcpy(last_block->buffer + item_width * (index % per_block), value, item_width);
	master_dirty = last_block->dirty = true;
}

void DataColumnFix::SetCount(int count)
{
	CDB_ASSERT(count >= 0);
	CDB_ASSERT(!datafile->IsReadOnly());
	if(count == row_count)
		return;
	master_dirty = true;
	int blocks = count > 0 ? (count - 1) / per_block + 1 : 0;
	if(blocks < block_index.GetCount()) { // clean up cache
		while(block_index.GetCount() > blocks) {
			int n = block_index.Pop();
			datafile->Remove(n, id);
		}
		for(int i = cache.GetCount(); --i >= 0;)
			if(cache.GetKey(i) >= blocks)
				cache.Remove(i);
	}
	else if(blocks > block_index.GetCount()) { // allocate more blocks
		while(block_index.GetCount() < blocks) {
			int n = datafile->Add(id);
			block_index.Add(n);
			datafile->Set(n, Null, id);
		}
	}
	row_count = count;
}

DataColumnVar::DataColumnVar()
{
	DoClose();
}

DataColumnVar::~DataColumnVar()
{
}

void DataColumnVar::Unlink()
{
	for(int i = 0; i < block_index.GetCount(); i++)
		datafile->Remove(block_index[i], id);
	DataColumn::Unlink();
}

void DataColumnVar::Commit()
{
	FlushCache();
	if(master_dirty)
		FlushMaster();
}

int DataColumnVar::GetUsedCacheSize() const
{
	return current_size;
}

void DataColumnVar::ShrinkCache()
{
	ClearCache();
}

void DataColumnVar::SetCount(int count)
{
	CHECK_CACHE();

	CDB_ASSERT(count >= 0);
	CDB_ASSERT(!datafile->IsReadOnly());

	if(count == row_count)
		return;
	last_index = -1;
	master_dirty = true;
	if(count == 0) { // clear all
		row_count = 0;
		while(!block_index.IsEmpty())
			datafile->Remove(block_index.Pop(), id);
		index_index.SetCount(1);
		index_index[0] = 0;
		cache.Clear();
		cache_index.Clear();
		return;
	}
	if(count < row_count) { // trim existing blocks
		int pos = FindBlock(count - 1);
		for(int i = cache.GetCount(); --i >= 0;)
			if(cache_index[i] > pos) {
				current_size -= cache[i].total;
				cache_index.Remove(i);
				cache.Remove(i);
			}

		block_index.SetCount(pos + 1);
		index_index.SetCount(pos + 2);
		row_count = index_index.Top();
		if(count < row_count) { // trim last block
			int cp = FetchCache(pos);
			int start = index_index[pos];
			int span = index_index[pos + 1] - start;
			FlushCache(cp, count - start);
		}
	}
	else { // expand index
		int c = block_index.GetCount();
		int span;
		if(c && GetDataLength(c - 1) + SHORT_BYTES
			* (span = index_index[c] - index_index[c - 1]) < INIT_BYTES - SHORT_BYTES) { // add some to last entry
			if(span == 1) { // convert singleton to multiblock
				String data = GetRaw(row_count - 1);
				int to_add = (INIT_BYTES - data.GetLength() - 2 * SHORT_BYTES) / SHORT_BYTES;
				to_add = minmax(to_add, 1, count - row_count);
				Block& block = cache.Add();
				cache_index.Add(c - 1);
				block.total = 3 * SHORT_BYTES + SHORT_BYTES * to_add + data.GetLength();
				CDB_ASSERT(block.total >= 4 * SHORT_BYTES && block.total <= MAX_BYTES);
				StringBuffer blockbuf(block.total);
				memcpy(blockbuf, block.buffer, min(block.buffer.GetLength(), block.total));
				char *p = blockbuf;
				Poke16le(p, data.GetLength() + SHORT_BYTES);
				memcpy(p += SHORT_BYTES, data, data.GetLength());
				Poke16le(p += data.GetLength(), SHORT_BYTES);
				block.offsets = SHORT_BYTES + data.GetLength();
				for(int i = 0; i <= to_add; i++)
					Poke16le(p += SHORT_BYTES, block.offsets);
				block.buffer = blockbuf;
				current_size += block.total;
				row_count += to_add;
				block.raw_dirty = true;
			}
			else { // add to normal block
				Block& block = cache[FetchCache(c - 1)];
				current_size -= block.total;
				int to_add = (INIT_BYTES - block.total) / SHORT_BYTES;
				to_add = minmax(to_add, 1, count - row_count);
				block.total += to_add * SHORT_BYTES;
				int old_total = block.offsets + (span + to_add + 1) * SHORT_BYTES;
				CDB_ASSERT(old_total >= 3 * SHORT_BYTES && old_total <= MAX_BYTES);
				StringBuffer blockbuf(old_total);
				memcpy(blockbuf, block.buffer, min(block.buffer.GetLength(), old_total));
				char *p = ~blockbuf + block.offsets + SHORT_BYTES * span;
				for(int i = 0; i < to_add; i++)
					Poke16le(p += SHORT_BYTES, block.offsets);
				block.buffer = blockbuf;
				block.raw_dirty = true;
				current_size += block.total;
				row_count += to_add;
			}
			index_index.Top() = row_count;
			CHECK_BLOCK(cache[FetchCache(c - 1)], c - 1);
		}
		while(row_count < count) {
			int batch = min(count - row_count, INIT_BYTES / SHORT_BYTES);
			block_index.Add(datafile->Add(id));
			if(batch > 1) {
				Block& block = cache.Add();
				cache_index.Add(block_index.GetCount() - 1);
				block.total = SHORT_BYTES * (batch + 2);
				CDB_ASSERT(block.total >= 3 * SHORT_BYTES && block.total <= MAX_BYTES);
				StringBuffer blockbuf(block.total);
				memcpy(blockbuf, block.buffer, min(block.buffer.GetLength(), block.total));
				char *p = blockbuf;
				Poke16le(p, block.offsets = SHORT_BYTES);
				for(int i = 0; i <= batch; i++)
					Poke16le(p += SHORT_BYTES, SHORT_BYTES);
				block.buffer = blockbuf;
				block.raw_dirty = true;
				current_size += block.total;
			}
			else
				datafile->Set(block_index.Top(), Null, id);
			row_count += batch;
			index_index.Add(row_count);
			if(current_size >= cache_size)
				ClearCache();
			int c = block_index.GetCount() - 1;
#ifdef CDB_DEBUG
			if(index_index[c + 1] - index_index[c] > 1)
				CheckBlock(cache[FetchCache(c)], c);
#endif
		}
	}

	CHECK_CACHE();
}

void DataColumnVar::Serialize(Stream& stream)
{
	if(stream.IsLoading()) {
		if(stream.GetLeft() < INDEX_BYTES)
			throw ObjectExc(NFormat("Porušení konzistence databáze (DataColumnVar#%d)", master_index));
		row_count = stream.GetIL();
		int block_count = (int)stream.GetLeft() / (2 * INDEX_BYTES);
		block_index.SetCount(block_count);
		index_index.SetCount(block_count + 1);
		GetIndex(block_index, stream);
		GetIndex(index_index.Begin(), block_count, stream);
		index_index.Top() = row_count;
	}
	else {
		if(index_index.IsEmpty())
			index_index.Add(0);
		stream.PutIL(row_count);
		SetIndex(stream, block_index);
		SetIndex(stream, index_index.Begin(), block_index.GetCount());
	}
}

void DataColumnVar::DoClose()
{
	index_index.Clear();
	block_index.Clear();
	cache.Clear();
	cache_index.Clear();
	last_index = -1;
	current_size = 0;
}

int DataColumnVar::FindBlock(int index) const
{
	CDB_ASSERT(index >= 0 && index < row_count);
	return BinFindIndex(index_index.Begin() + 1, index_index.End(), index + 1, StdLess<int>());
}

String DataColumnVar::GetRaw(int index)
{
//	RTIMING("DataColumnVar::GetRaw");

	if(current_size >= cache_size)
		ClearCache();
	CDB_ASSERT(index >= 0 && index < row_count);
	AdjustLastIndex(index);
	if(index_index[last_index + 1] - index_index[last_index] == 1) // singleton
		return datafile->Get(block_index[last_index]);
	int pos = cache_index.Find(last_index);
	if(pos < 0)
		pos = FetchCache(last_index);
	const Block& block = cache[pos];
	pos = block.dirty.Find(index);
	if(pos >= 0)
		return block.dirty[pos];
	index -= index_index[last_index];
	const char *p = block.buffer.Begin() + block.offsets + SHORT_BYTES * index;
	int offset = Peek16le(p);
	return String(block.buffer.Begin() + offset, Peek16le(p + SHORT_BYTES) - offset);
}

void DataColumnVar::SetRaw(int index, String value)
{
	CDB_ASSERT(index >= 0);
	CDB_ASSERT(!datafile->IsReadOnly());

	if(index >= row_count)
		SetCount(index + ADD_STEP);
	if(current_size >= cache_size)
		ClearCache();
	AdjustLastIndex(index);
	CDB_ASSERT(index >= index_index[last_index] && index < index_index[last_index + 1]);
	const int *px = &index_index[last_index];
	if(px[1] - px[0] == 1) { // singleton
		if(value.GetLength() < JOIN_BYTES && CanJoin(last_index, value.GetLength())) {
			char buffer[DataFile::BLOCK_BYTES];
			int offset = SHORT_BYTES;
			memcpy(buffer + offset, value, value.GetLength());
			offset += value.GetLength() + GetDataLength(last_index + 1);
			Poke16le(buffer + offset, SHORT_BYTES);
			Poke16le(buffer + offset + SHORT_BYTES, SHORT_BYTES + value.GetLength());
			JoinBlock(last_index, buffer, offset, 1);
			if(current_size >= cache_size)
				ClearCache();
			return;
		}
		else
			datafile->Set(block_index[last_index], value, id);
		return;
	}

	int pos = cache_index.Find(last_index);
	if(pos < 0)
		pos = FetchCache(last_index);

	Block& block = cache[pos];
	CHECK_BLOCK(block, last_index);
	pos = block.dirty.Find(index);
	if(pos >= 0) {
		int delta = value.GetLength() - block.dirty[pos].GetLength();
		current_size += delta;
		block.total += delta;
		CDB_ASSERT(block.total >= 3 * SHORT_BYTES);
		block.dirty[pos] = value;
	}
	else {
		int dx = (index - index_index[last_index]) * SHORT_BYTES + block.offsets;
		int delta = value.GetLength() - (Peek16le(block.buffer.Begin() + dx + SHORT_BYTES)
			- Peek16le(block.buffer.Begin() + dx));
		current_size += delta;
		block.total += delta;
		CDB_ASSERT(block.total >= 3 * SHORT_BYTES);
		block.dirty.Add(index, value);
	}
	if(current_size >= cache_size)
		ClearCache();
}

void DataColumnVar::FlushCache()
{
	for(int i = cache.GetCount(); --i >= 0;)
		if(i < cache.GetCount() && (cache[i].raw_dirty || !cache[i].dirty.IsEmpty()))
			FlushCache(i);
}

void DataColumnVar::FlushCache(int i, int set_span)
{
	CHECK_CACHE();

	int index = cache_index[i];
	Block& block = cache[i];
	CHECK_BLOCK(block, index);
	char buffer[MAX_BYTES];
	int range = index_index[index];
	int span = index_index[index + 1] - index_index[index];
	if(set_span < 0)
		set_span = span;
	CDB_ASSERT(set_span <= span);
	current_size -= block.total;

	if(block.total <= MAX_BYTES && set_span == span) { // keep block together
		int offset = SHORT_BYTES + GetDataLength(index);
		CDB_ASSERT(offset >= SHORT_BYTES && offset <= MAX_BYTES);
		if(CanJoin(index, block.total)) {
			offset += GetDataLength(index + 1);
			Poke16le(buffer + offset, SHORT_BYTES);
			int count = AddBlock(index, buffer, offset, 0);
			JoinBlock(index, buffer, offset, count);
		}
		else {
			Poke16le(buffer, offset);
			Poke16le(buffer + offset, SHORT_BYTES);
			int count = AddBlock(index, buffer, offset, 0);
			CDB_ASSERT(offset + (count + 1) * SHORT_BYTES <= MAX_BYTES);
			datafile->Set(block_index[index], String(buffer, offset + (count + 1) * SHORT_BYTES), id);
			CDB_ASSERT(datafile->GetLength(block_index[index]) >= SHORT_BYTES * (count + 2));
		}
	}
	else { // split block
		Vector<int> order = GetSortOrder(block.dirty.GetKeys(), StdLess<int>());
		Buffer<const char *> data(span);
		Buffer<int>          length(span);
		Buffer<String>       string(span);
 { // generate data offsets & lengths
			int ox = 0, nx = (ox < order.GetCount() ? block.dirty.GetKey(order[ox]) - range : span);
			for(int i = 0; i < set_span; i++)
				if(i == nx) { // dirty block
					String s = block.dirty[order[ox]];
					string[i] = s;
					data[i] = string[i];
					length[i] = s.GetLength();
					nx = (++ox < order.GetCount() ? block.dirty.GetKey(order[ox]) - range : span);
				}
				else { // normal block
					const char *p = block.buffer.Begin() + block.offsets + i * SHORT_BYTES;
					int off = Peek16le(p);
					CDB_ASSERT(off >= SHORT_BYTES && off < block.buffer.GetLength());
					length[i] = Peek16le(p + SHORT_BYTES) - off;
					data[i] = block.buffer.Begin() + off;
				}
		}

		Vector<int> block_insert, index_insert;

		{ // split data into blocks
			int next_block = block_index[index];
			int i = 0;
			while(i < set_span) {
				if(next_block < 0)
					index_insert.Add(i + range);
				int old = i;
				int dtotal = SHORT_BYTES, avail = INIT_BYTES - SHORT_BYTES;
				do {
					avail -= length[i] + SHORT_BYTES;
					dtotal += length[i];
				}
				while((i++ != old || dtotal < SINGLE_BYTES)
					&& i < set_span && avail - length[i] >= SHORT_BYTES);
				int subspan = i - old;
				String s;
				CDB_ASSERT(span > 0);
				if(subspan == 1) { // singleton
					s = string[old];
					if(s.IsEmpty())
						s = String(data[old], length[old]);
				}
				else { // create compound block
					char *p = buffer + dtotal;
					int sum = SHORT_BYTES * (subspan + 1) + dtotal;
					int offset = SHORT_BYTES;
					Poke16le(buffer, dtotal);
					Poke16le(p, SHORT_BYTES);
					while(old < i) {
						int len = length[old];
						memcpy(buffer + offset, data[old], len);
						offset += len;
						Poke16le(p += 2, offset);
						old++;
					}
					s = String(buffer, sum);
				}
				if(next_block < 0) {
					next_block = datafile->Add(id);
					block_insert.Add(next_block);
				}
				datafile->Set(next_block, s, id);
				CDB_ASSERT(subspan == 1 || datafile->GetLength(next_block) >= SHORT_BYTES * subspan);
				next_block = -1;
			}
		}

		int delta = block_insert.GetCount();

		{ // relocate index entries
			for(int i = 0; i < cache.GetCount(); i++)
				if(cache_index[i] > index)
					cache_index.Set(i, cache_index[i] + delta);
		}

		// insert items into master index
		block_index.Insert(index + 1, block_insert);
		index_index.Insert(index + 1, index_insert);
		master_dirty = true;
		last_index = -1;
	}

	if((i = cache_index.Find(index)) >= 0) {
		cache_index.Remove(i);
		cache.Remove(i);
	}

	CHECK_CACHE();
}

void DataColumnVar::ClearCache()
{
	FlushCache();
	cache.Clear();
	cache_index.Clear();
	current_size = 0;
}

int DataColumnVar::FetchCache(int block)
{
	int i = cache_index.Find(block);
	if(i >= 0)
		return i;
	cache_index.Add(block);
	Block& new_block = cache.Add();
//	datafile->CheckIntegrity();
	new_block.buffer = datafile->Get(block_index[block]);
	new_block.offsets = Peek16le(new_block.buffer);
	CDB_ASSERT(new_block.offsets >= SHORT_BYTES && new_block.offsets < new_block.buffer.GetLength());
	new_block.total = new_block.buffer.GetLength();
	new_block.raw_dirty = false;
	CHECK_BLOCK(new_block, block);
	current_size += new_block.total;
	CHECK_CACHE();
	return cache.GetCount() - 1;
}

bool DataColumnVar::CanJoin(int index, int length) const
{
	CDB_ASSERT(index >= 0);
	if(length > JOIN_BYTES || ++index >= block_index.GetCount())
		return false;
	int pos = cache_index.Find(index);
	int len2 = (pos >= 0 ? cache[pos].total : datafile->GetLength(block_index[index]));
	if(len2 + length >= INIT_BYTES)
		return false;
	if(index_index[index + 1] - index_index[index] == 1 && len2 >= SINGLE_BYTES)
		return false;
	return true;
}

int DataColumnVar::GetDataLength(int index) const
{
	CDB_ASSERT(index >= 0 && index < block_index.GetCount());
	int count = index_index[index + 1] - index_index[index];
	int pos = cache_index.Find(index);
	int len = (pos >= 0 ? cache[pos].total : datafile->GetLength(block_index[index]));
	len -= (count > 1 ? SHORT_BYTES * (count + 2) : 0);
	CDB_ASSERT(len >= 0);
	return len;
}

void DataColumnVar::JoinBlock(int index, char *buffer, int offset, int count)
{
	CDB_ASSERT(index >= 0 && index + 1 < block_index.GetCount());
	Poke16le(buffer, offset);
	count = AddBlock(index + 1, buffer, offset, count);
	datafile->Set(block_index[index], String(buffer, offset + (count + 1) * SHORT_BYTES), id);
	CDB_ASSERT(datafile->GetLength(block_index[index]) >= SHORT_BYTES * (count + 2));

	CHECK_CACHE();

	// update indexes
	block_index.Remove(++index);
	index_index.Remove(index);
	master_dirty = true;
	last_index = -1;
	for(int i = cache_index.GetCount(); --i >= 0;)
		if(cache_index[i] > index)
			cache_index.Set(i, cache_index[i] - 1);
		else if((unsigned)(index - cache_index[i]) <= 1u) { // remove both original block & join block
			cache_index.Remove(i);
			cache.Remove(i);
		}

	CHECK_CACHE();
}

int DataColumnVar::AddBlock(int index, char *buffer, int offset, int count)
{
	CDB_ASSERT(index >= 0 && index < block_index.GetCount());
	CDB_ASSERT(offset >= SHORT_BYTES && offset <= MAX_BYTES);
	CDB_ASSERT(count >= 0 && count < MAX_BYTES);

	int pos_pos = offset + count * SHORT_BYTES;
	int data_offset = Peek16le(buffer + pos_pos);
	int start = index_index[index];
	int span = index_index[index + 1] - start;
	int cpos;
	if(span == 1) { // singleton
		String singleton = datafile->Get(block_index[index]);
		memcpy(buffer + data_offset, singleton, singleton.GetLength());
		CDB_ASSERT(data_offset + singleton.GetLength() == offset);
		Poke16le(buffer + pos_pos + SHORT_BYTES, offset);
	}
	else if((cpos = cache_index.Find(index)) < 0) { // non-cached item
		String item = datafile->Get(block_index[index]);
		int poff = Peek16le(item);
		memcpy(buffer + data_offset, item.Begin() + SHORT_BYTES, poff - SHORT_BYTES);
		const char *src = item.Begin() + poff;
		char *dest = buffer + pos_pos;
		int delta = data_offset - SHORT_BYTES;
		for(int i = 0; i < span; i++)
			Poke16le(dest += SHORT_BYTES, delta + Peek16le(src += SHORT_BYTES));
	}
	else { // cached item
		const Block& block = cache[cpos];
		Vector<int> order = GetSortOrder(block.dirty.GetKeys(), StdLess<int>());
		char *dest = buffer + pos_pos;
		for(int op = 0, x = 0; x < span;) {
			int n = (op >= order.GetCount() ? span : block.dirty.GetKey(order[op]) - start);
			int c = n - x;
			if(c > 0) { // unchanged part
				const char *src = block.buffer.Begin() + block.offsets + SHORT_BYTES * x;
				int start = Peek16le(src);
				CDB_ASSERT(start >= SHORT_BYTES && start < block.buffer.GetLength());
				int part = Peek16le(src + SHORT_BYTES * c) - start;
				int delta = data_offset - start;
				memcpy(buffer + data_offset, block.buffer.Begin() + start, part);
				for(int i = x; i < n; i++)
					Poke16le(dest += SHORT_BYTES, delta + Peek16le(src += SHORT_BYTES));
				data_offset += part;
				x = n;
			}
			while(x < span && op < order.GetCount() && block.dirty.GetKey(order[op]) == x + start) { // add dirty string
				const String& s = block.dirty[order[op++]];
				memcpy(buffer + data_offset, s, s.GetLength());
				Poke16le(dest += SHORT_BYTES, data_offset += s.GetLength());
				x++;
			}
		}
	}
	return count + span;
}

void DataColumnVar::CheckBlock(const Block& blk, int index) const
{
	int blk_span = (blk.buffer.GetLength() - Peek16le(blk.buffer.Begin())) / SHORT_BYTES - 1;
	int ind_span = index_index[index + 1] - index_index[index];
	CDB_ASSERT(blk_span == ind_span);
}

void DataColumnVar::CheckCache() const
{
	for(int i = 0; i < cache.GetCount(); i++)
		CheckBlock(cache[i], cache_index[i]);
}

int DataRowIndex::Block::FindIndex(int offset) const
{
	return BinFindIndex(data, offset, StdLess<int>()) & ~1;
}

DataRowIndex::DataRowIndex()
{
	DoClose();
}

void DataRowIndex::Unlink()
{
	for(int i = 0; i < block_index.GetCount(); i++)
		datafile->Remove(block_index[i], id);
	DataClient::Unlink();
}

void DataRowIndex::Commit()
{
	FlushCache();
	if(master_dirty)
		FlushMaster();
}

int DataRowIndex::GetUsedCacheSize() const
{
	return current_size;
}

void DataRowIndex::ShrinkCache()
{
	ClearCache();
}

int DataRowIndex::Add()
{
	return Add(1)[0];
}

Vector<int> DataRowIndex::Add(int count)
{
	Vector<int> items;
	if(collapsed) {
		items.SetCount(count);
		for(int i = 0; i < count; i++)
			items[i] = exact_count++;
		first_empty = end_index = exact_count;
		master_dirty = true;
		return items;
	}

	for(int bx = first_empty >> LOG2_BLOCK_ITEMS; count > 0 && bx < block_index.GetCount(); bx++) {
		int c = cache.Find(bx);
		if(c < 0 && datafile->IsEmpty(block_index[bx]))
			continue;
		Block& block = cache[c >= 0 ? c : FetchCache(bx)];
		if(block.data.IsEmpty())
			continue;
		block.dirty = true;
		int pos = 0;
		do {
			int first = (pos == 0 ? 0 : block.data[pos - 1]);
			int avail = (pos < block.data.GetCount() ? block.data[pos] : BLOCK_ITEMS) - first;
			int part = min(avail, count);
			if(part == 0)
				continue;
			exact_count += part;
			count -= part;
			if(pos == 0) { // 1st item is a special case
				if(part < avail) {
					block.data.InsertN(0, 2);
					block.data[0] = 0;
					block.data[1] = part;
				}
				else
					block.data[0] = 0;
			}
			else if(part < avail || pos == block.data.GetCount())
				block.data[pos - 1] += part;
			else
				block.data.Remove(pos - 1, 2);
			int at = items.GetCount();
			items.SetCountR(at + part);
			first += bx * BLOCK_ITEMS;
			int *p = &items[at], *e = p + part;
			while(p < e)
				*p++ = first++;
			first_empty = (part < avail ? first : -1);
			if(end_index < first)
				end_index = first;
		}
		while(count > 0 && (pos += 2) <= block.data.GetCount());
		if(block.data.GetCount() == 2 && block.data[0] == 0 && block.data[1] == BLOCK_ITEMS)
			block.data.Clear();
		if(current_size >= cache_size)
			ClearCache();
	}
	while(count >= BLOCK_ITEMS) { // add full blocks
		first_empty = end_index = (block_index.GetCount() + 1) * BLOCK_ITEMS;
		exact_count += BLOCK_ITEMS;
		master_dirty = true;
		int x = datafile->Add(id);
		block_index.Add(x);
		datafile->Set(x, Null, id);
		int pos = items.GetCount();
		items.SetCountR(pos + BLOCK_ITEMS);
		int *p = &items[pos], *e = p + BLOCK_ITEMS;
		int start = (block_index.GetCount() - 1) * BLOCK_ITEMS;
		while(p < e)
			*p++ = start++;
		count -= BLOCK_ITEMS;
	}
	if(count > 0) {
		first_empty = end_index = block_index.GetCount() * BLOCK_ITEMS + count;
		exact_count += count;
		master_dirty = true;
		int x = datafile->Add(id);
		block_index.Add(x);
		StringBuffer content(2 * INDEX_BYTES);
		char *bp = content;
		Poke32le(bp, 0);
		Poke32le(bp + INDEX_BYTES, count);
		datafile->Set(x, content, id);
		int pos = items.GetCount();
		items.SetCountR(pos + count);
		int *p = &items[pos], *e = p + count;
		int start = (block_index.GetCount() - 1) * BLOCK_ITEMS;
		while(p < e)
			*p++ = start++;
	}
	if(first_empty < 0)
		CalcFirstEmpty();
	return items;
}

void DataRowIndex::AddAt(int index)
{
	ASSERT(index >= 0);
	if(!IsEmpty(index))
		return;
	if(collapsed && index == exact_count) {
		first_empty = end_index = ++exact_count;
		master_dirty = true;
		return;
	}
	UnCollapse();
	int bx = index >> LOG2_BLOCK_ITEMS;
	while(block_index.GetCount() < bx) {
		int x = datafile->Add(id);
		block_index.Add(x);
		StringBuffer content(2 * INDEX_BYTES);
		char *bp = content;
		Poke32le(bp, 0);
		Poke32le(bp + INDEX_BYTES, 0);
		datafile->Set(x, content, id);
		master_dirty = true;
	}
	Block& block = cache[FetchCache(bx)];
	current_size -= block.data.GetCount() * sizeof(int);
	int rem = index & (BLOCK_ITEMS - 1);
	int pos = block.FindIndex(rem);
	if(pos < block.data.GetCount() && block.data[pos] < rem)
		pos += 2;
	if(pos > 0 && block.data[pos - 1] == rem) // extend left block
		block.data[pos - 1]++;
	else if(pos < block.data.GetCount() && block.data[pos] == rem) // extend right block
		block.data[pos]--;
	else { // insert new sub-block
		block.data.InsertN(pos, 2);
		block.data[pos] = rem;
		block.data[pos + 1] = rem + 1;
	}
	if(pos > 0 && pos < block.data.GetCount() && block.data[pos - 1] == block.data[pos])
		block.data.Remove(pos, 2); // join neighbouring blocks
	if(block.data.GetCount() == 2 && block.data[0] == 0 && block.data[1] == BLOCK_ITEMS)
		block.data.Clear();
	current_size += block.data.GetCount() * sizeof(int);
	block.dirty = true;
	if(end_index <= index)
		end_index = index + 1;
	if(first_empty == index)
		CalcFirstEmpty();
}

void DataRowIndex::Remove(int index)
{
	Vector<int> x;
	x.SetCount(1);
	x[0] = index;
	Remove(x);
}

void DataRowIndex::Remove(const Vector<int>& index)
{
	if(index.IsEmpty())
		return;
	Vector<int> sorted;
	sorted <<= index;
	Sort(sorted);
	if(collapsed) {
		while(!sorted.IsEmpty() && sorted.Top() == exact_count - 1) {
			sorted.Drop();
			exact_count--;
		}
		first_empty = end_index = exact_count;
		master_dirty = true;
		if(sorted.IsEmpty())
			return;
		UnCollapse();
	}
	for(int i = 0; i < sorted.GetCount();) {
		int f = sorted[i];
		int bx = f >> LOG2_BLOCK_ITEMS;
		int end = (bx + 1) * BLOCK_ITEMS;
		Block& block = cache[FetchCache(bx)];
		CDB_ASSERT((block.data.GetCount() & 1) == 0);
		block.dirty = true;
		current_size -= block.data.GetCount() * sizeof(int);
		if(block.data.IsEmpty())
			block.data << 0 << BLOCK_ITEMS;
		int pos = block.FindIndex(f & (BLOCK_ITEMS - 1));
		do {
			int l = f = sorted[i++];
			while(++l < end && i < sorted.GetCount() && sorted[i] == l)
				i++;
			f &= BLOCK_ITEMS - 1;
			l = ((l - 1) & (BLOCK_ITEMS - 1)) + 1;
			while(pos < block.data.GetCount() && f > block.data[pos + 1])
				pos += 2;
			if(pos == block.data.GetCount()) { // end of block
				if(pos == 0)
					block.data << 0 << 0;
				break;
			}
			while(pos < block.data.GetCount() && l > block.data[pos]) { // process block
				int *p = &block.data[pos];
				bool lm = (f <= p[0]);
				bool rm = (l >= p[1]);
				if(!lm && !rm) { // insert inner block
					exact_count -= l - f;
					block.data.InsertN(pos + 1, 2);
					block.data[pos + 1] = f;
					block.data[pos + 2] = l;
					pos += 2;
				}
				else if(!lm) {
					exact_count -= p[1] - f;
					p[1] = f;
					pos += 2;
				}
				else if(!rm) {
					exact_count -= l - p[0];
					p[0] = l;
				}
				else {
					exact_count -= p[1] - p[0];
					block.data.Remove(pos, 2);
				}
			}
			CDB_ASSERT((block.data.GetCount() & 1) == 0);
		}
		while(i < sorted.GetCount() && sorted[i] < end);
		if(block.data.IsEmpty())
			block.data << 0 << 0;
		current_size += block.data.GetCount() * sizeof(int);
		if(current_size >= cache_size)
			ClearCache();
	}
	if(sorted[0] < first_empty)
		CalcFirstEmpty();
	if(sorted.Top() >= end_index - 1)
		CalcEndIndex();
}

Vector<int> DataRowIndex::GetIndex()
{
	Vector<int> items;
	if(collapsed) {
		items.SetCount(exact_count);
		for(int i = 0; i < exact_count; i++)
			items[i] = i;
		return items;
	}
	int end = (end_index + BLOCK_ITEMS - 1) >> LOG2_BLOCK_ITEMS;
	CDB_ASSERT(block_index.GetCount() >= end);
	for(int i = 0; i < end; i++) {
		int pos = items.GetCount();
		int c = cache.Find(i);
		if(c >= 0 && cache[c].data.IsEmpty() || c < 0 && datafile->IsEmpty(block_index[i])) { // full length block
			items.SetCountR(pos + BLOCK_ITEMS);
			int *p = &items[pos], *e = p + BLOCK_ITEMS;
			int v = i * BLOCK_ITEMS;
			while(p < e)
				*p++ = v++;
		}
		else { // partial block
			const Block& block = cache[FetchCache(i)];
			for(int t = 0; t < block.data.GetCount(); t += 2) {
				int start = block.data[t] + i * BLOCK_ITEMS;
				int count = block.data[t + 1] - block.data[t];
				if(count > 0) {
					items.SetCountR(pos + count);
					int *p = &items[pos], *e = p + count;
					while(p < e)
						*p++ = start++;
					pos += count;
				}
			}
		}
	}
	return items;
}

bool DataRowIndex::IsEmpty(int index)
{
	if(index < 0 || index >= end_index)
		return true;
	if(collapsed)
		return false;
	int bx = index >> LOG2_BLOCK_ITEMS, c = cache.Find(bx);
	if(c < 0 && datafile->IsEmpty(block_index[bx]))
		return false; // full block
	const Block& block = cache[c >= 0 ? c : FetchCache(bx)];
	if(block.data.IsEmpty())
		return false; // full block
	index &= BLOCK_ITEMS - 1;
	int pos = block.FindIndex(index);
	return (pos >= block.data.GetCount() || index < block.data[pos] || index >= block.data[pos + 1]);
}

void DataRowIndex::Serialize(Stream& stream)
{
	int version = 1;
	stream / version;
	if(version < 1 || version > 1) {
		stream.SetError();
		return;
	}
	int c = (collapsed ? exact_count : -1);
	stream % c;
	if(stream.IsLoading()) {
		if(collapsed = (c >= 0)) {
			exact_count = end_index = first_empty = c;
			current_size = 0;
		}
		else {
			block_index.SetCount((int)stream.GetLeft() / INDEX_BYTES);
			DataClient::GetIndex(block_index, stream);
			CalcExactCount();
			CalcEndIndex();
			CalcFirstEmpty();
		}
	}
	else
		SetIndex(stream, block_index);
}

void DataRowIndex::DoClose()
{
	block_index.Clear();
	cache.Clear();
	exact_count = 0;
	end_index = 0;
	first_empty = 0;
	current_size = 0;
	collapsed = true;
}

void DataRowIndex::CalcFirstEmpty()
{
	if(collapsed) {
		first_empty = exact_count;
		return;
	}
	for(int i = 0; i < block_index.GetCount(); i++) {
		int p = cache.Find(i);
		bool emp = (p >= 0 ? cache[p].data.IsEmpty() : datafile->IsEmpty(block_index[i]));
		if(!emp) {
			const Block& block = cache[p >= 0 ? p : FetchCache(i)];
			first_empty = i * BLOCK_ITEMS;
			if(block.data[0] == 0)
				first_empty += block.data[1];
			return;
		}
	}
	first_empty = block_index.GetCount() * BLOCK_ITEMS;
}

void DataRowIndex::CalcExactCount()
{
	if(collapsed)
		return;
	exact_count = 0;
	for(int i = 0; i < block_index.GetCount(); i++) {
		int p = cache.Find(i);
		bool emp = (p >= 0 ? cache[p].data.IsEmpty() : datafile->IsEmpty(block_index[i]));
		if(emp)
			exact_count += BLOCK_ITEMS;
		else {
			const Block& block = cache[p >= 0 ? p : FetchCache(i)];
			for(int t = 0; t < block.data.GetCount(); t += 2)
				exact_count += block.data[t + 1] - block.data[t];
		}
	}
}

void DataRowIndex::CalcEndIndex()
{
	if(collapsed) {
		end_index = exact_count;
		return;
	}
	for(int i = block_index.GetCount(); --i >= 0;) {
		int p = cache.Find(i);
		bool emp = (p >= 0 ? cache[p].data.IsEmpty() : datafile->IsEmpty(block_index[i]));
		if(emp) {
			end_index = (i + 1) * BLOCK_ITEMS;
			return;
		}
		const Block& block = cache[p >= 0 ? p : FetchCache(i)];
		if(block.data[1] != 0) {
			end_index = i * BLOCK_ITEMS + block.data.Top();
			return;
		}
	}
	end_index = 0;
}

void DataRowIndex::FlushCache()
{
	for(int i = 0; i < cache.GetCount(); i++)
		if(cache[i].dirty) {
			Block& block = cache[i];
			CDB_CALL(CheckBlock(block));
			StringStream ss;
			SetIndex(ss, block.data);
			datafile->Set(block_index[cache.GetKey(i)], ss, id);
			block.dirty = false;
		}
}

void DataRowIndex::ClearCache()
{
	FlushCache();
	cache.Clear();
	current_size = 0;
}

int DataRowIndex::FetchCache(int bx)
{
	int p = cache.Find(bx);
	if(p >= 0)
		return p;
	Block& block = cache.Add(bx);
	StringStream ss(datafile->Get(block_index[bx]));
	block.data.SetCount((int)ss.GetSize() >> LOG2_INDEX_BYTES);
	CDB_ASSERT((block.data.GetCount() & 1) == 0);
	DataClient::GetIndex(block.data, ss);
	CDB_CALL(CheckBlock(block));
	current_size += block.data.GetCount() * sizeof(int);
	return cache.GetCount() - 1;
}

void DataRowIndex::UnCollapse()
{
	if(!collapsed)
		return;
	master_dirty = true;
	collapsed = false;
	if(exact_count <= 0)
		return;
	int i = 0;
	while(i < exact_count) {
		int bi = block_index.GetCount();
		int x = datafile->Add(id);
		block_index.Add(x);
		Block& new_block = cache.Add(bi);
		new_block.dirty = true;
		new_block.data.SetCount(2);
		new_block.data[0] = 0;
		new_block.data[1] = min<int>(BLOCK_ITEMS, exact_count - i);
		i += BLOCK_ITEMS;
	}
	master_dirty = true;
}

void DataRowIndex::CheckBlock(const Block& block)
{
	if(block.data.GetCount() != 2 || block.data[0] != 0 || block.data[1] != 0) {
		int last = -1;
		for(int i = 0; i < block.data.GetCount(); i++) {
			if(block.data[i] <= last) {
				NEVER();
			}
			if((last = block.data[i]) > BLOCK_ITEMS) {
				NEVER();
			}
		}
	}
}

Value DataConvertFilter::Get(int index)
{
	return convert->Format(source->Get(index));
}

void DataConvertFilter::Set(int index, Value value)
{
	source->Set(index, convert->Scan(value));
}

void DataConvertFilter::CheckSet(int index, Value value)
{
	Value v = convert->Scan(value);
	if(v.IsError())
		throw ObjectExc(GetErrorText(v));
}

void DataMapFilter::CheckSet(int index, Value value)
{
	if(map.Find(value) < 0)
		throw ObjectExc(NFormat("<%vt>: hodnota nebyla nalezena v množinì pøípustných hodnot", value));
	source->CheckSet(index, value);
}

void DataMinMaxFilter::CheckSet(int index, Value value)
{
	if(IsNull(value)) {
		if(not_null)
			throw ObjectExc("hodnota nesmí být prázdná");
	}
	else {
		if(!IsNumber(value))
			throw ObjectExc(NFormat("<%vt>: oèekávána èíselná hodnota", value));
		double d = value;
		if(!IsNull(min) && d < min)
			throw ObjectExc(NFormat("<%vt>: hodnota je menší než povolená dolní mez (%n)", value, min));
		if(!IsNull(max) && d > max)
			throw ObjectExc(NFormat("<%vt>: hodnota je vìtší než povolená horní mez (%n)", value, max));
	}
	source->CheckSet(index, value);
}

DataOrderedIndexBase::DataOrderedIndexBase(bool lazy)
: lazy(lazy)
{
	DoClose();
}

void DataOrderedIndexBase::Add(int index, Value value)
{
#ifdef CHECK_INDEX
	CheckIndex();
#endif
	if(!lazy)
		master_dirty = true;
	if(!disabled && !dirty)
		ordered_index.Insert(FindIndex(index, value), index);
	source->Add(index, value);
#ifdef CHECK_INDEX
	CheckIndex();
#endif
}

void DataOrderedIndexBase::Set(int index, Value value)
{
#ifdef CHECK_INDEX
	CheckIndex();
#endif
	if(!lazy)
		master_dirty = true;
	if(!disabled && !dirty) { // locate & move existing item
		int i = FindIndex(index, Get(index));
		ASSERT(i < ordered_index.GetCount() && ordered_index[i] == index);
		int *pi = ordered_index.Begin() + i;
		int *pj = ordered_index.Begin() + FindIndex(index, value);
		if(pi > pj) {
			CopyBackward(pi + 1, pi, pj);
			pj[0] = index;
		}
		else if(pi < pj) {
			Copy(pi, pi + 1, pj);
			pj[-1] = index;
		}
	}
	source->Set(index, value);
#ifdef CHECK_INDEX
	CheckIndex();
#endif
}

void DataOrderedIndexBase::Remove(int index)
{
#ifdef CHECK_INDEX
	CheckIndex();
#endif
	if(!lazy)
		master_dirty = true;
	if(!disabled && !dirty) {
		int i = FindIndex(index, Get(index));
		if(i < ordered_index.GetCount() && ordered_index[i] == index)
			ordered_index.Remove(i);
	}
	source->Remove(index);
#ifdef CHECK_INDEX
	CheckIndex();
#endif
}

void DataOrderedIndexBase::Disable()
{
	if(!datafile->IsReadOnly() && !lazy)
		master_dirty = true;
	ordered_index.Clear();
	disabled = true;
	dirty = true;
}

void DataOrderedIndexBase::Enable()
{
	if(disabled) {
		disabled = false;
		ordered_index.Clear();
		dirty = true;
		RefreshIndex();
	}
}

Vector<int> DataOrderedIndexBase::GetEqual(const Index<Value>& values)
{
	Vector<int> out;
	if(values.GetCount() < 10 * row_index->GetCount()) { // binary search for index values
		RefreshIndex();
		for(int i = 0; i < values.GetCount(); i++) {
			Value v = values[i];
			int l = FindIndex(0, v);
			while(l < ordered_index.GetCount() && source->Get(ordered_index[l]) == v)
				out.Add(ordered_index[l++]);
		}
	}
	else { // full scan & index
		for(int i = 0; i < ordered_index.GetCount(); i++)
			if(values.Find(source->Get(ordered_index[i])) >= 0)
				out.Add(ordered_index[i]);
	}
	return out;
}

Vector<int> DataOrderedIndexBase::GetRange(Value min, Value max)
{
	RefreshIndex();
	int l = IsNull(min) ? 0 : FindIndex(0, min);
	int r = IsNull(max) ? ordered_index.GetCount() : FindIndex(0, max);
	if(r < ordered_index.GetCount() && Get(r) == max)
		r++;
	Vector<int> result;
	if(r > l)
		result.Append(ordered_index, l, r - l);
	return result;
}

void DataOrderedIndexBase::GetMinMax(Ref min, Ref max)
{
	RefreshIndex();
	if(ordered_index.IsEmpty()) {
		min.SetNull();
		max.SetNull();
		return;
	}
	Value v = source->Get(ordered_index[0]);
	if(!IsNull(v))
		min.SetValue(v);
	else { // bisect to find 1st non-null value
		int lo = 1, hi = ordered_index.GetCount() - 1;
		while(lo < hi) {
			int mid = (lo + hi) >> 1;
			v = source->Get(ordered_index[mid]);
			if(IsNull(v))
				lo = mid + 1;
			else
				hi = mid;
		}
		min.SetValue(source->Get(ordered_index[hi]));
	}
	v = source->Get(ordered_index.Top());
	if(!IsNull(v))
		max.SetValue(v);
	else { // bisect
		int lo = 0, hi = ordered_index.GetCount() - 2;
		while(lo < hi) {
			int mid = (lo + hi + 1) >> 1;
			v = source->Get(ordered_index[mid]);
			if(IsNull(v))
				hi = mid - 1;
			else
				lo = mid;
		}
		max.SetValue(source->Get(ordered_index[lo]));
	}
}

int DataOrderedIndexBase::GetFirstEqual(Value value, int exclude_row)
{
	if(disabled)
		return Null;
	RefreshIndex();
	int i = FindIndex(0, value);
	if(i < ordered_index.GetCount() && ordered_index[i] == exclude_row)
		i++;
	return i < ordered_index.GetCount() && Get(ordered_index[i]) == value ? ordered_index[i] : (int)Null;
}

void DataOrderedIndexBase::Serialize(Stream& stream)
{
	if(stream.IsLoading()) {
		disabled = stream.GetIL();
		dirty = true;
		if(!lazy) {
			ordered_index.SetCount((int)stream.GetLeft() >> LOG2_INDEX_BYTES);
			GetIndex(ordered_index, stream);
			dirty = false;
		}
	}
	else {
		stream.PutIL(disabled ? 1 : 0);
		if(!lazy)
			SetIndex(stream, ordered_index);
	}
}

void DataOrderedIndexBase::Commit()
{
#ifdef CHECK_INDEX
	CheckIndex();
#endif
	source->Commit();
	if(master_dirty) {
		FlushMaster();
		LOG(source->Describe() << ".Commit, size = " << datafile->GetLength(GetMasterIndex()));
	}
}

void DataOrderedIndexBase::Rollback()
{
	DataClient::Rollback();
	if(lazy) {
		ordered_index.Clear();
		dirty = true;
	}
}

int DataOrderedIndexBase::GetUsedCacheSize() const
{
	return ordered_index.GetCount() * sizeof(int);
}

void DataOrderedIndexBase::ShrinkCache()
{
	if(lazy) {
		ordered_index.Clear();
		dirty = true;
	}
}

void DataOrderedIndexBase::DoClose()
{
	ordered_index.Clear();
	disabled = false;
	dirty = true;
#ifdef CHECK_INDEX
	row_index = 0;
#endif
}

String DataOrderedIndexBase::Describe() const
{
	return "ordered index->" + source->Describe();
}

Value DataColumnOf<bool>::Get(int index)
{
	int bx = index >> LOG2_BOOL_BLOCK_BITS;
//	if(GetCount() <= bx)
//		return 0;
	return (*(const int *)GetRaw(bx) >> (index & (BOOL_BLOCK_BITS - 1))) & 1;
}

void DataColumnOf<bool>::Set(int index, Value value)
{
	int bx = index >> LOG2_BOOL_BLOCK_BITS;
	if(GetCount() <= bx)
		SetCount(index + 256);
	int i = *(const int *)GetRaw(bx);
	int m = 1 << (index & (BOOL_BLOCK_BITS - 1));
	i = (CalcType<bool>::ValueTo(value) ? i | m : i & ~m);
	SetRaw(bx, &i);
}

void DataColumnOf<bool>::SetCount(int count)
{
	DataColumnFix::SetCount((count + BOOL_BLOCK_BITS - 1) >> LOG2_BOOL_BLOCK_BITS);
}

Value DataColumnOf<Date>::Get(int index)
{
	const byte *p = GetRaw(index);
	if(p[0] == 0 && p[1] == 0)
		return Value();
	Date d;
	d.year = p[0] + (p[1] & 127) * 256 - 16384;
	d.month = (p[1] >> 7) + ((p[2] & 7) << 1) + 1;
	d.day = (p[2] >> 3) + 1;
	return d;
}

void DataColumnOf<Date>::Set(int index, Value value)
{
	byte buffer[3];
	if(IsNull(value))
		buffer[0] = buffer[1] = buffer[2] = 0;
	else {
		Date d = value;
		int y = d.year + 16384;
		buffer[0] = y & 0xFF;
		buffer[1] = ((y >> 8) & 0x7F) | ((d.month - 1) << 7);
		buffer[2] = ((d.month - 1) >> 1) | ((d.day - 1) << 3);
	}
	SetRaw(index, buffer);
}

Value DataColumnOf<Time>::Get(int index)
{
	const byte *p = GetRaw(index);
	if(p[0] == 0 && p[1] == 0)
		return Value();
	Time t;
	t.year = p[0] + (p[1] & 0x3F) * 256 - 8192;
	unsigned r = (p[1] >> 6) | (p[2] << 2) | (p[3] << 10) | (p[4] << 18);
	t.month = r % 12 + 1;
	r /= 12;
	t.day = (r & 0x1F) + 1;
	r >>= 5;
	t.hour = r % 24;
	r /= 24;
	t.minute = r % 60;
	t.second = r / 60;
	return t;
}

void DataColumnOf<Time>::Set(int index, Value value)
{
	byte buffer[5];
	if(IsNull(value))
		buffer[0] = buffer[1] = buffer[2] = buffer[3] = buffer[4] = 0;
	else {
		Time t = value;
		unsigned temp = t.year + 8192;
		buffer[0] = (temp >> 0) & 0xFF;
		buffer[1] = (temp >> 8) & 0x3F;
		temp = ((((t.second * 60 + t.minute) * 24 + t.hour) << 5) + t.day - 1) * 12 + t.month - 1;
		buffer[1] |= (temp << 6) & 0xC0;
		buffer[2] = (temp >> 2) & 0xFF;
		buffer[3] = (temp >> 10) & 0xFF;
		buffer[4] = (temp >> 18) & 0xFF;
	}
	SetRaw(index, buffer);
}

Value DataColumnOf<Value>::Get(int index)
{
	Value v;
	SerializeIn(v, GetRaw(index));
	return v;
}

void DataColumnOf<Value>::Set(int index, Value value)
{
	SetRaw(index, SerializeOut(value));
}

template <class T>
DataSource *DataTableColumnTypeHelper(One<DataClient>& client, T * = 0)
{
	T *s = new T;
	client = s;
	return s;
}

DataSet& DataSet::Sort()
{
	for(const int *p = set.Begin(), *e = set.End(); ++p < e;)
		if(p[-1] > p[0]) {
			UPP::Sort(set);
			break;
		}
	return *this;
}

DataSet& DataSet::operator &= (const DataSet& s)
{
	ASSERT(table == s.table);
	IntersectSorted(set, s.set);
	return *this;
}

DataSet& DataSet::operator |= (const DataSet& s)
{
	ASSERT(table == s.table);
	UnionSorted(set, s.set);
	return *this;
}

DataSet& DataSet::operator -= (const DataSet& s)
{
	ASSERT(table == s.table);
	RemoveSorted(set, s.set);
	return *this;
}

DataSet operator && (const DataSet& a, const DataSet& b)
{
	if(a.GetCount() < b.GetCount())
		return DataSet(a, 0) &= b;
	else
		return DataSet(b, 0) &= a;
}

DataSet operator || (const DataSet& a, const DataSet& b)
{
	if(a.GetCount() < b.GetCount())
		return DataSet(a, 0) |= b;
	else
		return DataSet(b, 0) |= a;
}

DataSet operator - (const DataSet& a, const DataSet& b)
{
	return DataSet(a, 0) -= b;
}

DataTableColumn::DataTableColumn()
: vtype(VALUE_V)
{
	owner = 0;
	row_index = 0;
	DoClose();
}

String DataTableColumn::GetTableName() const
{
	return owner->GetName();
}

void DataTableColumn::SetStyle(int _vtype, int _flags)
{
	ASSERT(!source);
	vtype = _vtype;
	flags = _flags;
}

void DataTableColumn::ModifyFlags(int _flags)
{
	if(flags != _flags) {
		LOG("DataTableColumn(" << GetName() << ").ModifyFlags");
		int old_flags = flags;
		flags = _flags;
		if((flags ^ old_flags) & INTEGER_INDEX)
			IndexHelper(flags & INTEGER_INDEX, iindex);
		if((flags ^ old_flags) & NUMBER_INDEX)
			IndexHelper(flags & NUMBER_INDEX, nindex);
		if((flags ^ old_flags) & DATETIME_INDEX)
			IndexHelper(flags & DATETIME_INDEX, tindex);
		if((flags ^ old_flags) & STRING_INDEX)
			IndexHelper(flags & STRING_INDEX, sindex);
		RefreshSource();
		LOG("//DataTableColumn(" << GetName() << ").ModifyFlags");
	}
}

DataSet DataTableColumn::GetIndex() const
{
	return owner->GetIndex();
}

void DataTableColumn::Link(DataTable& table)
{
	link = new DataLinkFilter(*this, table);
	RefreshSource();
}

void DataTableColumn::Unlink()
{
	if(iindex) iindex->Unlink();
	if(nindex) nindex->Unlink();
	if(tindex) tindex->Unlink();
	if(sindex) sindex->Unlink();
	client->Unlink();
	DataClient::Unlink();
}

void DataTableColumn::Commit()
{
	ASSERT(source);
//	LOG("DataTableColumn(" << GetName() << ").Commit" << BeginIndent);
	source->Commit();
	if(master_dirty)
		FlushMaster();
//	LOG(EndIndent << "//DataTableColumn(" << GetName() << ").Commit");
}

int DataTableColumn::GetUsedCacheSize() const
{
	int total = source->GetUsedCacheSize();
	if(iindex) total += iindex->GetUsedCacheSize();
	if(nindex) total += nindex->GetUsedCacheSize();
	if(sindex) total += sindex->GetUsedCacheSize();
	if(tindex) total += tindex->GetUsedCacheSize();
	return total;
}

void DataTableColumn::ShrinkCache()
{
	client_source->ShrinkCache();
	if(iindex) iindex->ShrinkCache();
	if(nindex) nindex->ShrinkCache();
	if(sindex) sindex->ShrinkCache();
	if(tindex) tindex->ShrinkCache();
}

void DataTableColumn::Serialize(Stream& stream)
{
	int start_msecs = msecs();
	int version = StreamHeading(stream, 1, 1, 1, "DataTableColumn");
	if(version >= 1) {
		stream % vtype % flags;
		bool new_column = false;
		if(!client) { // create column
			new_column = stream.IsStoring();
			switch(vtype) {
			case BOOL_V:   client_source = DataTableColumnTypeHelper< DataColumnOf<bool>   >(client); break;
			case INT_V:    client_source = DataTableColumnTypeHelper< DataColumnOf<int>    >(client); break;
			case BYTE_V:   client_source = DataTableColumnTypeHelper< DataColumnOf<byte>   >(client); break;
			case SHORT_V:  client_source = DataTableColumnTypeHelper< DataColumnOf<short>  >(client); break;
			case DOUBLE_V: client_source = DataTableColumnTypeHelper< DataColumnOf<double> >(client); break;
			case STRING_V: client_source = DataTableColumnTypeHelper< DataColumnOf<String> >(client); break;
			case DATE_V:   client_source = DataTableColumnTypeHelper< DataColumnOf<Date>   >(client); break;
			case TIME_V:   client_source = DataTableColumnTypeHelper< DataColumnOf<Time>   >(client); break;
			case VALUE_V:  client_source = DataTableColumnTypeHelper< DataColumnOf<Value>  >(client); break;
			default: throw ObjectExc(NFormat("<%d>: chybný datový typ sloupce", vtype));
			}
		}
		client->StreamMaster(stream, *datafile, id);
		ASSERT(client_source);
		LOG("DataTableColumn(" << owner->GetName() << "." << column_name << ").Serialize(" << (stream.IsLoading() ? "in" : "out")
			<< "), master = " << GetMasterIndex() << ", base = t + " << msecs(start_msecs));
		int cflags = 0;
		if(flags & INTEGER_INDEX) {
			if(!iindex) {
				cflags |= INTEGER_INDEX;
				indexes.Add(iindex = new DataOrderedIndex<int>(true));
			}
			iindex->StreamMaster(stream, *datafile, id);
			LOG("DataTableColumn(" << column_name << ").Serialize / integer index = t + " << msecs(start_msecs)
				<< ", master = " << iindex->GetMasterIndex() << ", master size = " << datafile->GetLength(iindex->GetMasterIndex()));
		}
		if(flags & NUMBER_INDEX) {
			if(!nindex) {
				cflags |= NUMBER_INDEX;
				indexes.Add(nindex = new DataOrderedIndex<double>(true));
			}
			nindex->StreamMaster(stream, *datafile, id);
			LOG("DataTableColumn(" << column_name << ").Serialize / number index = t + " << msecs(start_msecs)
				<< ", master = " << nindex->GetMasterIndex() << ", master size = " << datafile->GetLength(nindex->GetMasterIndex()));
		}
		if(flags & DATETIME_INDEX) {
			if(!tindex) {
				cflags |= DATETIME_INDEX;
				indexes.Add(tindex = new DataOrderedIndex<Time>(true));
			}
			tindex->StreamMaster(stream, *datafile, id);
			LOG("DataTableColumn(" << column_name << ").Serialize / datetime index = t + " << msecs(start_msecs)
				<< ", master = " << tindex->GetMasterIndex() << ", master size = " << datafile->GetLength(tindex->GetMasterIndex()));
		}
		if(flags & STRING_INDEX) {
			if(!sindex) {
				cflags |= STRING_INDEX;
				indexes.Add(sindex = new DataOrderedIndex<String>(true));
			}
			sindex->StreamMaster(stream, *datafile, id);
			LOG("DataTableColumn(" << column_name << ").Serialize / string index = t + " << msecs(start_msecs)
				<< ", master = " << sindex->GetMasterIndex() << ", master size = " << datafile->GetLength(sindex->GetMasterIndex()));
		}
		RefreshSource();
		if(new_column) { // initialize new column
			Vector<int> index = row_index->GetIndex();
			for(int i = 0; i < index.GetCount(); i++)
				source->Add(index[i], Value());
		}
		if(cflags) {
			if(cflags & INTEGER_INDEX)  iindex->SetRowIndex(row_index);
			if(cflags & NUMBER_INDEX)   nindex->SetRowIndex(row_index);
			if(cflags & DATETIME_INDEX) tindex->SetRowIndex(row_index);
			if(cflags & STRING_INDEX)   sindex->SetRowIndex(row_index);
		}
	}
	ASSERT(source);
	LOG("//DataTableColumn(" << column_name << ").Serialize = t + " << msecs(start_msecs));
}

void DataTableColumn::RefreshSource()
{
	source = 0;
	DataFilter *last = 0;
	int i;
	for(i = 0; i < filters.GetCount(); i++)
		if(last) {
			last->SetSource(&filters[i]);
			last = &filters[i];
		}
		else
			source = last = &filters[i];
	for(i = 0; i < indexes.GetCount(); i++)
		if(last) {
			last->SetSource(&indexes[i]);
			last = &indexes[i];
		}
		else
			source = last = &indexes[i];
	if(link)
		if(last) {
			last->SetSource(~link);
			last = ~link;
		}
		else
			source = last = ~link;
	if(client_source)
		if(last)
			last->SetSource(client_source);
		else
			source = client_source;
	if(source)
		source->SetRowIndex(row_index);
	ASSERT(source);
}

template <class T>
static Vector<int> DataGetRange(DataSource& source, const T& min, const T& max, const Vector<int>& index)
{
	Vector<int> done;
	if(IsNull(min) && IsNull(max))
		return Vector<int>(index, 0);
	if(IsNull(min)) { // upper bound
		for(int i = 0; i < index.GetCount(); i++) {
			int x = index[i];
			T t = source.Get(x);
			if(!IsNull(t) && t <= max)
				done.Add(x);
		}
	}
	else if(IsNull(max)) { // lower bound
		for(int i = 0; i < index.GetCount(); i++) {
			int x = index[i];
			T t = source.Get(x);
			if(!IsNull(t) && t >= min)
				done.Add(x);
		}
	}
	else { // range
		for(int i = 0; i < index.GetCount(); i++) {
			int x = index[i];
			T t = source.Get(x);
			if(!IsNull(t) && t >= min && t <= max)
				done.Add(x);
		}
	}
	return done;
}

DataSet DataTableColumn::GetRange(Value min, Value max)
{
	bool nnmin = IsNull(min), nnmax = IsNull(max);
	if(nnmin && nnmax)
		return DataSet(owner->GetName());
	if(iindex)
		return DataSet(owner->GetName(), iindex->GetRange(min, max));
	if(nindex)
		return DataSet(owner->GetName(), nindex->GetRange(min, max));
	if(tindex)
		return DataSet(owner->GetName(), tindex->GetRange(min, max));
	if((!nnmin && IsNumber(min)) || (!nnmax && IsNumber(max)))
		return DataSet(owner->GetName(), DataGetRange<double>(*source, min, max, row_index->GetIndex()));
	if((!nnmin && IsDateTime(min)) || (!nnmax && IsDateTime(max)))
		return DataSet(owner->GetName(), DataGetRange<Time>(*source, min, max, row_index->GetIndex()));
	throw Exc(NFormat("%s: nelze vyhledat rozsah %vt .. %vt", GetDotName(), min, max));
}

DataSet DataTableColumn::GetLike(WString mask)
{
	Vector<int> done = row_index->GetIndex();
	WildcardCompare wcc(mask);
	int *d = done.Begin();
	for(const int *s = done.Begin(), *e = done.End(); s < e; s++) {
		WString value = Get(*s);
		if(!IsNull(value) && wcc.Matches(value))
			*d++ = *s;
	}
	done.SetCount(d - done.Begin());
	return DataSet(owner->GetName(), done);
}

DataSet DataTableColumn::GetEqual(Value value)
{
	Index<Value> values;
	values.Reserve(1);
	values.Add(value);
	return GetEqual(values);
}

DataSet DataTableColumn::GetEqual(const Vector<int>& set)
{
	Index<Value> values;
	values.Reserve(set.GetCount());
	Append(values, set);
	return GetEqual(values);
}

DataSet DataTableColumn::GetEqual(const Index<Value>& set)
{
	if(iindex) return DataSet(owner->GetName(), iindex->GetEqual(set));
	if(nindex) return DataSet(owner->GetName(), nindex->GetEqual(set));
	if(sindex) return DataSet(owner->GetName(), sindex->GetEqual(set));
	if(tindex) return DataSet(owner->GetName(), tindex->GetEqual(set));
	Vector<int> done;
	Vector<int> index = row_index->GetIndex();
	for(int i = 0; i < index.GetCount(); i++)
		if(set.Find(source->Get(index[i])) >= 0)
			done.Add(index[i]);
	return DataSet(owner->GetName(), done);
}

int DataTableColumn::GetFirstEqual(Value value, int exclude_row)
{
	if(iindex && iindex->IsEnabled())
		return iindex->GetFirstEqual(value, exclude_row);
	if(nindex && nindex->IsEnabled())
		return nindex->GetFirstEqual(value, exclude_row);
	Vector<int> index = row_index->GetIndex();
	for(int i = 0; i < index.GetCount(); i++)
		if(source->Get(index[i]) == value && index[i] != exclude_row)
			return index[i];
	return Null;
}

void DataTableColumn::GetMinMax(double& min, double& max)
{
	if(iindex)
		iindex->GetMinMax(min, max);
	else if(nindex)
		nindex->GetMinMax(min, max);
	else {
		Vector<int> set = row_index->GetIndex();
		min = max = Null;
		for(int i = 0; i < set.GetCount(); i++) {
			double d = source->Get(set[i]);
			if(!IsNull(d)) {
				if(IsNull(min))
					min = max = d;
				else {
					if(d < min) min = d;
					if(d > max) max = d;
				}
			}
		}
	}
}

void DataTableColumn::GetMinMax(Time& min, Time& max)
{
	if(tindex)
		tindex->GetMinMax(min, max);
	else {
		Vector<int> set = row_index->GetIndex();
		min = max = Null;
		for(int i = 0; i < set.GetCount(); i++) {
			Time t = source->Get(set[i]);
			if(!IsNull(t)) {
				if(IsNull(min))
					min = max = t;
				else {
					if(t < min) min = t;
					if(t > max) max = t;
				}
			}
		}
	}
}

DataSet DataTableColumn::operator != (Value v)
{
	DataSet full = GetIndex();
	full -= GetEqual(v);
	return full;
}

DataSet DataTableColumn::operator != (const Index<Value>& v)
{
	DataSet full = GetIndex();
	full -= GetEqual(v);
	return full;
}

DataSet DataTableColumn::operator != (const Vector<int>& v)
{
	DataSet full = GetIndex();
	full -= GetEqual(v);
	return full;
}

void DataTableColumn::Set(int index, Value value)
{
	ASSERT(source);
	try {
		source->CheckSet(index, value);
		source->Set(index, value);
	}
	catch(ObjectExc e) {
		throw ObjectExc(NFormat("<%s#%d>: %s", owner->GetName(), index, e));
	}
}

Value DataTableColumn::Get(int index)
{
//	RTIMING("DataTableColumn::Get");

	ASSERT(source);
	return source -> Get(index);
}

Value DataTableColumn::operator [] (int index)
{
//	RTIMING("DataTableColumn::operator []");

	return Get(index);
}

Vector<Value> DataTableColumn::Get(const Vector<int>& rowset)
{
	Vector<Value> out;
	out.SetCount(rowset.GetCount());
	for(int i = 0; i < rowset.GetCount(); i++)
		out[i] = Get(rowset[i]);
	return out;
}

Vector<int> DataTableColumn::GetInt(const Vector<int>& rowset)
{
	Vector<int> out;
	out.SetCount(rowset.GetCount());
	for(int i = 0; i < rowset.GetCount(); i++)
		out[i] = Get(rowset[i]);
	return out;
}

void DataTableColumn::DoClose()
{
	source = 0;
	client = 0;
	iindex = 0;
	nindex = 0;
	tindex = 0;
	sindex = 0;
//	hindex = 0;
//	pindex = 0;
	client_source = 0;
	filters.Clear();
	indexes.Clear();
	client.Clear();
}

void DataTableColumn::Describe(Stream& stream, int flags)
{
	stream << column_name << ": " << source->Describe() << "\n";
}

String DataTableColumn::GetDotName() const
{
	String s;
	if(owner)
		s << owner->GetName() << '.';
	s << GetName();
	return s;
}

DataTable::DataTable()
{
}

DataTable::~DataTable()
{
	while(!links.IsEmpty())
		links.Pop()->Kill();
}

DataTableColumn& DataTable::Column(String name)
{
	int f = columns.Find(name);
	if(f >= 0)
		return columns[f];
	throw Exc(NFormat("<%s>: sloupec %s nebyl nalezen.", table_name, name));
}

Vector<Value> DataTable::EmptyRow() const
{
	Vector<Value> values;
	values.SetCount(columns.GetCount());
	return values;
}

void DataTable::Serialize(Stream& stream)
{
	int version = StreamHeading(stream, 2, 1, 2, "DataTable");
	if(version >= 1) {
		row_index.StreamMaster(stream, *datafile, id);
		int count = columns.GetCount();
		stream / count;
		for(int i = 0; i < count; i++) {
			String name;
			if(stream.IsStoring())
				name = columns.GetKey(i);
			stream % name;
			if(stream.IsLoading())
				columns.Add(name).SetTable(this).SetName(name).SetRowIndex(&row_index);
			columns[i].StreamMaster(stream, *datafile, id);
			ASSERT(columns[i].source);
		}
		if(version >= 2)
			stream % user_info;
		else if(stream.IsLoading())
			user_info = Null;
	}
}

void DataTable::Unlink()
{
	for(int i = 0; i < columns.GetCount(); i++)
		columns[i].Unlink();
	DataClient::Unlink();
}

void DataTable::Commit()
{
	for(int i = 0; i < columns.GetCount(); i++)
		columns[i].Commit();
	row_index.Commit();
	if(master_dirty)
		FlushMaster();
	datafile->Commit(id);
}

void DataTable::Rollback()
{
	datafile->Rollback(id);
	DataClient::Rollback();
	row_index.Rollback();
	for(int i = 0; i < columns.GetCount(); i++)
		columns[i].Rollback();
}

int DataTable::GetUsedCacheSize() const
{
	int total = row_index.GetUsedCacheSize();
	for(int i = 0; i < columns.GetCount(); i++)
		total += columns[i].GetUsedCacheSize();
	return total;
}

void DataTable::ShrinkCache()
{
	row_index.ShrinkCache();
	for(int i = 0; i < columns.GetCount(); i++)
		columns[i].ShrinkCache();
}

int DataTable::AddColumn(String name, int vtype, int flags)
{
	if(FindColumn(name) >= 0)
		throw ObjectExc(NFormat("<%s>: duplikátní název sloupce v tabulce <%s>", name, table_name));
	master_dirty = true;
	DataTableColumn& col = columns.Add(name);
	col.SetTable(this);
	col.SetName(name);
	col.SetStyle(vtype, flags);
	col.row_index = &row_index;
	col.Create(*datafile, id);
	return columns.GetCount() - 1;
}

void DataTable::RemoveColumn(String name)
{
	int i = FindColumn(name);
	if(i < 0)
		throw ObjectExc(NFormat("<%s>: sloupec nebyl nalezen v tabulce <%s>", name, table_name));
	master_dirty = true;
	columns[i].Unlink();
	columns.Remove(i);
}

void DataTable::AddLink(DataLinkFilter& link)
{
	if(FindIndex(links, &link) < 0)
		links.Add(&link);
}

void DataTable::RemoveLink(DataLinkFilter& link)
{
	int p = FindIndex(links, &link);
	if(p >= 0)
		links.Remove(p);
}

void DataTable::DoClose()
{
	row_index.Close();
	columns.Clear();
}

void DataTable::Describe(Stream& stream, int flags)
{
	if(flags & EXPORT_FORMAT) {
		Export(stream);
		return;
	}

	stream << "tabulka '" << table_name << "' (" << row_index.GetCount() << " øádkù)\n"
		"{\n";
//	stream.Begin();
	int i;
	for(i = 0; i < columns.GetCount(); i++)
		columns[i].Describe(stream, flags);
	if(flags & DUMP_ROWS) {
		stream << "\nRow dump:\n";
		DataSet index = GetIndex();
		for(i = 0; i < index.GetCount(); i++)
			stream << Format(Get(index[i])) << "\n";
	}
//	stream.End();
	stream << "}\n";
}

void DataTable::Export(Stream& stream)
{
	DataSet index = GetIndex();
	for(int i = 0; i < index.GetCount(); i++) {
		int x = index[i];
		stream << '=' << x << "\n";
		for(int c = 0; c < GetColumns(); c++) {
			String row;
			row << GetName(c) << ": ";
			Value value = (*this)(c, x);
			if(IsNull(value))
				row << "NULL";
			else if(IsNumber(value))
				row << DblStr(value);
			else if(IsDateTime(value))
				row << '@' << StdFormat(value);
			else {
				if(!IsString(value))
					value = SerializeOut(value);
				String s = value;
				row << '\"';
				for(const char *p = s, *e = s.End(); p < e; p++) {
					if(row.GetLength() > 70) {
						row.Cat("\"&");
						stream.PutLine(row);
						row = "    &\"";
					}
					if((byte)*p >= ' ' && *p != '\x7F' && *p != '\xFF' && *p != '\\' && *p != '\"')
						row.Cat(*p);
					else {
						row.Cat('\\');
						switch(*p) {
						case '\\': row << '\\'; break;
						case '\"': row << '\"'; break;
						case '\a': row << 'a'; break;
						case '\b': row << 'b'; break;
						case '\f': row << 'f'; break;
						case '\t': row << 't'; break;
						case '\v': row << 'v'; break;
						case '\r': row << 'r'; break;
						case '\n': row << 'n'; break;
						default:
							row.Cat('x');
							static char hex[] = "0123456789abcdef";
							row.Cat(hex[(*p >> 4) & 15]);
							row.Cat(hex[(*p >> 0) & 15]);
							break;
						}
					}
				}
				row << '\"';
			}
			stream.PutLine(row);
		}
	}
}

String DataTable::Import(Stream& stream, bool row_indices)
{
	int rx = -1;
	int line = 0;
	try {
		while(!stream.IsEof()) {
			String s = stream.GetLine();
			line++;
			const char *p = s;
			while(*p && (byte)*p <= ' ')
				p++;
			if(*p == '=') // new row index
				rx = stou(p + 1);
			else if(IsAlpha(*p) || *p == '_') {
				const char *b = p;
				while(*p && *p != ':')
					p++;
				if(*p != ':')
					throw Exc("oèekáván znak ':'");
				const char *e = p++;
				while(e > b && (byte)e[-1] <= ' ')
					e--;
				String col(b, e);
				int i = FindColumn(col);
				if(i < 0)
					throw Exc(NFormat("tabulka neobsahuje sloupec '%s'", col));
				while(*p && (byte)*p <= ' ')
					p++;
				Value value;
				if(p[0] == 'N' && p[1] == 'U' && p[2] == 'L' && p[3] == 'L')
					;
				else if(IsDigit(*p) || *p == '+' || *p == '-') { // number
					double d = ScanDouble(p, &p);
					if(IsNull(d))
						throw Exc(NFormat("oèekáváno reálné èíslo, nalezeno: %s", StringSample(p, 20)));
					value = d;
				}
				else if(*p == '@') {
					value = StdConvertTime().Scan(++p);
					if(value.IsError())
						throw Exc(NFormat("oèekáváno datum, nalezeno: %s", StringSample(p, 20)));
				}
				else if(*p == '\"') {
					String text;
					for(;;) {
						while(*++p != '\"')
							if(*p == '\\')
								switch(*++p) {
								case 0: throw Exc("uvozující znak escape-sekvence (\\) na konci øádku");
								case '\\':
								case '\"': text.Cat(*p); break;
								case 'a':  text.Cat('\a'); break;
								case 'b':  text.Cat('\b'); break;
								case 'f':  text.Cat('\f'); break;
								case 't':  text.Cat('\t'); break;
								case 'v':  text.Cat('\v'); break;
								case 'r':  text.Cat('\r'); break;
								case 'n':  text.Cat('\n'); break;
								case 'x':
									if(IsXDigit(p[1])) {
										int code = ctoi(*++p);
										if(IsXDigit(p[1]))
											code = code * 16 + ctoi(*++p);
										text.Cat(code);
									}
									else
										throw Exc(NFormat("chybná escape-sekvence %s", StringSample(p, 20)));
									break;
								}
							else if(*p)
								text.Cat(*p);
							else
								throw Exc("neukonèená øetìzcová konstanta");
						if(p[1] != '&')
							break;
						s = stream.GetLine();
						line++;
						p = s;
						while(*p && (byte)*p <= ' ')
							p++;
						if(*p++ != '&' || *p != '\"')
							throw Exc(NFormat("oèekáváno napojení øetìzce (&\"), nalezeno: %s", StringSample(p, 20)));
					}
					if(columns[i].vtype == STRING_V)
						value = text;
					else if(!SerializeIn(value, text))
						throw Exc("chybná binární hodnota");
				}
				else
					throw Exc(NFormat("chybný typ hodnoty: %s", StringSample(p, 20)));
				if(rx < 0 || IsEmpty(rx)) {
					rx = Add(rx);
					columns[i].source->CheckAdd(rx, value);
					columns[i].source->Add(rx, value);
				}
				else {
					columns[i].source->CheckSet(rx, value);
					columns[i].source->Set(rx, value);
				}
			}
			else if(*p && *p != '#')
				throw Exc(NFormat("chybný øádek: %s", StringSample(p, 20)));
		}
		Commit();
		return Null;
	}
	catch(Exc e) {
		Rollback();
		return NFormat("Øádek %d: %s", line, e);
	}
}

int DataTable::Add(int index)
{
	Vector<Value> row;
	row.SetCount(columns.GetCount());
	return Add(row, index);
}

int DataTable::Add(const Vector<Value>& row, int index)
{
	ASSERT(row_index.GetEndIndex() >= 0);
	if(index >= 0 && !IsEmpty(index)) {
		Set(index, row);
		return index;
	}
	if(index < 0)
		index = row_index.Add();
	else
		row_index.AddAt(index);
	ASSERT(row_index.GetEndIndex() > index);
	int i;
	for(i = 0; i < columns.GetCount(); i++)
		try {
			columns[i].source->CheckAdd(index, row[i]);
		}
		catch(ObjectExc e) {
			row_index.Remove(index);
			throw ObjectExc(NFormat("<%s#%d.%s>: %s", table_name, index, columns.GetKey(i), e));
		}

	for(i = 0; i < columns.GetCount(); i++)
		try {
			columns[i].source->Add(index, row[i]);
		}
		catch(ObjectExc e) {
			while(--i >= 0)
				columns[i].source->Remove(index);
			row_index.Remove(index);
			throw ObjectExc(NFormat("<%s#%d.%s>: %s", table_name, index, columns.GetKey(i), e));
		}

	return index;
}

void DataTable::Set(int index, const Vector<Value>& row)
{
	if(row.GetCount() != columns.GetCount())
		throw ObjectExc(NFormat("<%s#%d>: chybný poèet sloupcù", table_name, index));
	int i;
	for(i = 0; i < columns.GetCount(); i++)
		try {
			columns[i].source->CheckSet(index, row[i]);
		}
		catch(ObjectExc e) {
			throw ObjectExc(NFormat("<%s#%d.%s>: %s", table_name, index, columns.GetKey(i), e));
		}

	for(i = 0; i < columns.GetCount(); i++)
		try {
			columns[i].source->Set(index, row[i]);
		}
		catch(ObjectExc e) {
			throw ObjectExc(NFormat("<%s#%d.%s>: %s", table_name, index, columns.GetKey(i), e));
		}
}

void DataTable::Remove(int index)
{
	Remove(DataSet(GetName(), index));
}

void DataTable::Remove(const DataSet& index)
{
	int i;
	for(i = 0; i < index.GetCount(); i++)
		if(row_index.IsEmpty(index[i]))
			throw ObjectExc(NFormat("<%s#%d>: øádek je již smazán", table_name, index[i]));

	for(i = 0; i < links.GetCount(); i++) {
		DataLinkFilter *link = links[i];
		for(int r = 0; r < index.GetCount(); r++)
			if(link->Exists(index[r]))
				throw ObjectExc(NFormat("<%s#%d>: na øádek existuje odkaz z <%s>", table_name, index[r], link->GetColumnName()));
	}

	row_index.Remove(index.set);

	for(i = 0; i < columns.GetCount(); i++) {
		DataSource& source = ~columns[i];
		for(int r = 0; r < index.GetCount(); r++)
			try {
				source.Remove(index[r]);
			}
			catch(ObjectExc e) {
				throw ObjectExc(NFormat("<%s#%d.%s>: %s", table_name, index[r], columns.GetKey(i), e));
			}
	}
}

Vector<Value> DataTable::Get(int index)
{
	Vector<Value> values;
	values.SetCount(columns.GetCount());
	for(int i = 0; i < columns.GetCount(); i++)
		try {
			values[i] = columns[i].source->Get(index);
		}
		catch(ObjectExc e) {
			throw ObjectExc(NFormat("<%s#%d.%s>: %s", table_name, index, columns.GetKey(i), e));
		}
	return values;
}

Vector<Value> DataTable::GetAlways(int index)
{
	if(IsEmpty(index))
		return EmptyRow();
	return Get(index);
}

DataLinkFilter::DataLinkFilter(DataTableColumn& column, DataTable& table)
: column(&column)
, table(&table)
{
	table.AddLink(*this);
}

DataLinkFilter::~DataLinkFilter()
{
	if(table && column)
		table->RemoveLink(*this);
}

void DataLinkFilter::CheckSet(int index, Value value)
{
	if(IsNull(value))
		return;
	if(!CalcType<int>::IsType(value))
		throw ObjectExc(NFormat("<%vt>: oèekáváno celé èíslo (odkaz do tabulky %s)", value, table->GetName()));
	if(!IsNull(value) && table->IsEmpty(value))
		throw ObjectExc(NFormat("<%vt>: neplatný odkaz do tabulky %s", value, table->GetName()));
}

String DataLinkFilter::Describe() const
{
	String s;
	s << "link(";
	if(table)
		s << table->GetName();
	s << ")->" << source->Describe();
	return s;
}

DataBase::DataBase()
{
	block_level = 0;
}

void DataBase::Commit()
{
	datafile->BeginBlock();
	for(int i = 0; i < tables.GetCount(); i++)
		tables[i].Commit();
	if(master_dirty)
		FlushMaster();
	datafile->Commit(0);
	datafile->EndBlock();
}

void DataBase::Rollback()
{
	datafile->Rollback(0);
	for(int i = 0; i < tables.GetCount(); i++)
		tables[i].Rollback();
	DataClient::Rollback();
}

int DataBase::GetUsedCacheSize() const
{
	int total = 0;
	for(int i = 0; i < tables.GetCount(); i++)
		total += tables[i].GetUsedCacheSize();
	return total;
}

void DataBase::ShrinkCache()
{
	for(int i = 0; i < tables.GetCount(); i++)
		tables[i].ShrinkCache();
}

void DataBase::DoClose()
{
	tables.Clear();
}

void DataBase::Serialize(Stream& stream)
{
	ASSERT(datafile);
	int version = StreamHeading(stream, 1, 1, 1, "DataBase");
	if(version >= 1) {
		int count = tables.GetCount();
		stream / count;
		for(int i = 0; i < count; i++) {
			String name;
			int master;
			if(stream.IsStoring()) {
				name = tables.GetKey(i);
				master = tables[i].GetMasterIndex();
			}
			stream % name % master;
			if(stream.IsLoading()) {
				DataTable& table = tables.Add(name);
				table.SetName(name);
				table.Open(*datafile, master, (dfid_t)&table);
			}
		}
	}
}

DataTable& DataBase::Get(String name)
{
	int f = tables.Find(name);
	if(f >= 0)
		return tables[f];
	throw Exc(NFormat("<%s>: tabulka nebyla nalezena.", name));
}

DataTable& DataBase::Add(String name)
{
	ASSERT(datafile);
	if(tables.Find(name) >= 0)
		throw ObjectExc(NFormat("<%s>: tabulka již v databázi je", name));
	master_dirty = true;
	DataTable& table = tables.Add(name);
	table.SetName(name);
	table.Create(*datafile, (dfid_t)&table);
	return table;
}

DataTable& DataBase::GetAdd(String name)
{
	int i = tables.Find(name);
	if(i >= 0)
		return tables[i];
	return Add(name);
}

void DataBase::Remove(int i)
{
	master_dirty = true;
	ASSERT(i >= 0 && i < tables.GetCount());
	tables[i].Unlink();
	tables.Remove(i);
}

void DataBase::Remove(String name)
{
	int i = tables.Find(name);
	if(i < 0)
		throw ObjectExc(NFormat("<%s>: tabulka nebyla nalezena", name));
	Remove(i);
}

void DataBase::Describe(Stream& stream, int flags)
{
	stream << "Ultimate DataBase\n"
		"{\n";
//	stream.Begin();
	for(int i = 0; i < tables.GetCount(); i++)
		tables[i].Describe(stream, flags);
//	stream.End();
	stream << "}\n";
}

bool DataBase::TryRemove(String n)
{
	int i = Find(n);
	if(i < 0)
		return false;
	Remove(i);
	return true;
}

END_UPP_NAMESPACE
