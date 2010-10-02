#include "CypherBase.h"

NAMESPACE_UPP

////////////////////////////////////////////////////////////////////////////////////////////
//                                      CypherFifo class                                  //
////////////////////////////////////////////////////////////////////////////////////////////

// inserts data
void CypherFifo::Put(byte const *buf, int len)
{
	int l = data.GetCount();
	data.AddN(len);
	byte *b = &data.At(l);
	memcpy(b, buf, len);
}

void CypherFifo::Put(String const &s)
{
	Put((byte const *)~s, s.GetCount());
}

void CypherFifo::Put(byte b)
{
	data.Add(b);
}

// extract data
bool CypherFifo::Get(byte *buf, int len)
{
	if(data.GetCount() < len)
		return false;
	memcpy(buf, &data.At(0), len);
	data.Remove(0, len);
	return true;
}

bool CypherFifo::Get(String &s)
{
	if(!data.GetCount())
		return false;
	StringBuffer sb;
	sb.Cat((const char *)&data.At(0), data.GetCount());
	data.Clear();
	s = sb;
	return true;
}

bool CypherFifo::Get(byte &b)
{
	if(!data.GetCount())
		return false;
	b = data.At(0);
	data.Remove(0);
	return true;
}


CypherBase::CypherBase()
{
	// still no key present
	keyOk = false;
	
	// Cypher in IDLE mode at startup
	mode = IDLE;
	
	// clears FIFO
	FIFO.Clear();
	
}

void CypherBase::Reset()
{
	keyOk = false;
	mode = IDLE;
	FIFO.Clear();
}

END_UPP_NAMESPACE
