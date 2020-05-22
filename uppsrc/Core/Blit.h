#ifdef CPU_X86
void huge_memsetd(void *p, dword data, size_t len);
void memsetd_l(dword *t, dword data, size_t len);

inline
void memsetd(void *p, dword data, size_t len)
{
	dword *t = (dword *)p;
	if(len < 4) {
		if(len & 2) {
			t[0] = t[1] = t[len - 1] = data;
			return;
		}
		if(len & 1)
			t[0] = data;
		return;
	}

	if(len >= 16) {
		memsetd_l(t, data, len);
		return;
	}

	__m128i val4 = _mm_set1_epi32(data);
	auto Set4 = [&](size_t at) { _mm_storeu_si128((__m128i *)(t + at), val4); };
	Set4(len - 4); // fill tail
	if(len & 8) {
		Set4(0); Set4(4);
		t += 8;
	}
	if(len & 4)
		Set4(0);
}

void memcpyd_l(dword *t, const dword *s, size_t len);

inline
void memcpyd(dword *t, const dword *s, size_t len)
{
#ifdef CPU_64
	if(len <= 4) {
		if(len) {
			if(len > 1) {
				*(int64 *)t = *(int64 *)s;
				*(int64 *)(t + len - 2) = *(int64 *)(s + len - 2);
				return;
			}
			*t = *s;
		}
		return;
	}
#else
	if(len < 4) {
		if(len) {
			if(len > 1) {
				t[0] = s[0];
				t[1] = s[1];
				t[len - 1] = s[len - 1];
				return;
			}
			*t = *s;
		}
		return;
	}
#endif

	auto Copy4 = [&](size_t at) { _mm_storeu_si128((__m128i *)(t + at), _mm_loadu_si128((__m128i *)(s + at))); };

	Copy4(len - 4); // copy tail
	if(len >= 16) {
		memcpyd_l(t, s, len);
		return;
	}
	if(len & 8) {
		Copy4(0); Copy4(4);
		t += 8;
		s += 8;
	}
	if(len & 4)
		Copy4(0);
}

#else
inline
void memsetd(void *p, dword c, size_t len)
{
	dword *t = (dword *)p;
	while(len >= 16) {
		t[0] = c; t[1] = c; t[2] = c; t[3] = c;
		t[4] = c; t[5] = c; t[6] = c; t[7] = c;
		t[8] = c; t[9] = c; t[10] = c; t[11] = c;
		t[12] = c; t[13] = c; t[14] = c; t[15] = c;
		t += 16;
		len -= 16;
	}
	if(len & 8) {
		t[0] = t[1] = t[2] = t[3] = t[4] = t[5] = t[6] = t[7] = c;
		t += 8;
	}
	if(len & 4) {
		t[0] = t[1] = t[2] = t[3] = c;
		t += 4;
	}
	if(len & 2) {
		t[0] = t[1] = c;
		t += 2;
	}
	if(len & 1)
		t[0] = c;
}

inline
void memcpyd(dword *t, dword *s, size_t len)
{
	if(len >= 16) {
		memcpy(t, s, 4 * len);
		return;
	}
	if(len & 8) {
		t[0] = s[0]; t[1] = s[1]; t[2] = s[2]; t[3] = s[3];
		t[4] = s[4]; t[5] = s[5]; t[6] = s[6]; t[7] = s[7];
		t += 8;
		s += 8;
		len -= 8;
	}
	if(len & 4) {
		t[0] = s[0]; t[1] = s[1]; t[2] = s[2]; t[3] = s[3];
		s += 4;
		t += 4;
	}
	if(len & 2) {
		t[0] = s[0]; t[1] = s[1];
		s += 2;
		t += 2;
	}
	if(len & 1)
		t[0] = s[0];
}
#endif

#ifdef CPU_UNALIGNED

void svo_memset_l(byte *t, dword val4, size_t len);

inline
void svo_memset(void *p, byte val, size_t len)
{
	byte *t = (byte *)p;
	if(len < 2) {
		if(len)
			t[0] = val;
		return;
	}
	dword val4 = 0x1010101 * val;
	if(len <= 4) {
		*(word *)t = *(word *)(t + len - 2) = (word)val4;
		return;
	}
	*(dword *)t = *(dword *)(t + len - 4) = val4; // alignment & fill tail
	if(len > 16) {
		svo_memset_l(t, val4, len);
		return;
	}
	if(len > 8)
		*(dword *)(t + 4) = *(dword *)(t + len - 8) = val4;
}

void svo_memcpy_l(byte *t, byte *s, size_t len);

inline
void svo_memcpy(void *p, const void *q, size_t len)
{
	byte *t = (byte *)p;
	byte *s = (byte *)q;
	if(len < 2) {
		if(len)
			t[0] = s[0];
		return;
	}
	if(len <= 4) {
		*(word *)t = *(word *)s;
		*(word *)(t + len - 2) = *(word *)(s + len - 2);
		return;
	}
	*(dword *)t = *(dword *)s;
	*(dword *)(t + len - 4) = *(dword *)(s + len - 4);
	if(len > 16) {
		svo_memcpy_l(t, s, len);
		return;
	}
	if(len > 8) {
		*(dword *)(t + 4) = *(dword *)(s + 4);
		*(dword *)(t + len - 8) = *(dword *)(s + len - 8);
	}
}

#else

inline
void svo_memset(void *p, byte val, size_t len)
{
	if(len >= 32) {
		memset(p, val, len);
		return;
	}
	byte *t = (byte *)p;
	if(len & 16) {
		t[0] = val; t[1] = val; t[2] = val; t[3] = val;
		t[4] = val; t[5] = val; t[6] = val; t[7] = val;
		t[8] = val; t[9] = val; t[10] = val; t[11] = val;
		t[12] = val; t[13] = val; t[14] = val; t[15] = val;
		t += 16;
	}
	if(len & 8) {
		t[0] = t[1] = t[2] = t[3] = t[4] = t[5] = t[6] = t[7] = val;
		t += 8;
	}
	if(len & 4) {
		t[0] = t[1] = t[2] = t[3] = val;
		t += 4;
	}
	if(len & 2) {
		t[0] = t[1] = val;
		t += 2;
	}
	if(len & 1)
		t[0] = val;
}

inline
void svo_memcpy(void *p, const void *q, size_t len)
{
	byte *t = (byte *)p;
	byte *s = (byte *)q;
	if(len >= 16) {
		memcpy(t, s, len);
		return;
	}
	if(len & 8) {
		t[0] = s[0]; t[1] = s[1]; t[2] = s[2]; t[3] = s[3];
		t[4] = s[4]; t[5] = s[5]; t[6] = s[6]; t[7] = s[7];
		t += 8;
		s += 8;
	}
	if(len & 4) {
		t[0] = s[0]; t[1] = s[1]; t[2] = s[2]; t[3] = s[3];
		t += 4;
		s += 4;
	}
	if(len & 2) {
		t[0] = s[0]; t[1] = s[1];
		t += 2;
		s += 2;
	}
	if(len & 1)
		t[0] = s[0];
}

#endif

template <class T>
void memcpy_t(T *t, const T *s, size_t count)
{
	if((sizeof(T) & 3) == 0)
		memcpyd((dword *)t, (const dword *)s, count * (sizeof(T) >> 2));
	else
		svo_memcpy((void *)t, (void *)s, count * sizeof(T));
}
