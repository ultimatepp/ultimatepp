#ifndef _Core_Huge_h_
#define _Core_Huge_h_

class Huge {
public:
	enum { CHUNK = 1024 * 1024 };

private:
	struct Block {
		byte data[CHUNK];
	};
	
	Array<Block>  data;
	size_t        size;

public:
	byte  *AddChunk();
	void   Finish(int last_chunk_size);
	
	size_t GetSize() const                              { return size; }
	void   Get(void *t, size_t pos, size_t sz) const;
	void   Get(void *t)                                 { Get(t, 0, GetSize()); }
	String Get() const;
	
	Huge();
};

#endif
