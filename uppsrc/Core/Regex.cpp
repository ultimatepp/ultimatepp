#include "Core.h"

namespace Upp {

std::atomic<int64> RegexMatch::serial;

static void* Pcre2Alloc(size_t size, void* memory_data)
{
    return MemoryAlloc(size);
}

static void Pcre2Free(void* ptr, void* memory_data)
{
    MemoryFree(ptr);
}

static pcre2_general_context_8 *s_pcre2_ctx_8 = pcre2_general_context_create_8(
    Pcre2Alloc,
    Pcre2Free,
    nullptr
);

static pcre2_general_context_32 *s_pcre2_ctx_32 = pcre2_general_context_create_32(
    Pcre2Alloc,
    Pcre2Free,
    nullptr
);

EXITBLOCK {
	pcre2_general_context_free_8(s_pcre2_ctx_8);
	pcre2_general_context_free_32(s_pcre2_ctx_32);
}

int RegexMatch::GetCount() const
{
	return count;
}

int RegexMatch::GetOffset(int i) const
{
	ASSERT(i >= 0 && i < GetCount());
	return ovector[2 * i];
}

int RegexMatch::GetLength(int i) const
{
	ASSERT(i >= 0 && i < GetCount());
	return ovector[2 * i + 1] - ovector[2 * i];
}

void RegexMatch::Clear()
{
	if(match_data_8) {
	    pcre2_match_data_free_8(match_data_8);
		match_data_8 = nullptr;
		ovector = nullptr;
	}
	if(match_data_32) {
	    pcre2_match_data_free_32(match_data_32);
		match_data_32 = nullptr;
		ovector = nullptr;
	}
}

RegexMatch::~RegexMatch()
{
	Clear();
}

bool Regex::Match(const char *text, int len, RegexMatch *match) const
{
	pcre2_match_data_8 *m = nullptr;
	if(match) {
		if(match->pattern_serial != pattern_serial) {
			match->Clear();
			match->match_data_8 = pcre2_match_data_create_from_pattern_8(re, s_pcre2_ctx_8);
			match->pattern_serial = pattern_serial;
		}
		m = match->match_data_8;
		match->count = 0;
	}
	else
		m = pcre2_match_data_create_8(1, s_pcre2_ctx_8);

    int rc = pcre2_match_8(
        re,                    // compiled pattern
        (PCRE2_SPTR8)text,     // subject string
        len,                   // length
        0,                     // offset
        options & (PCRE2_ANCHORED|PCRE2_NO_UTF_CHECK|PCRE2_ENDANCHORED),
        m,
        NULL                   // default match context
    );

	if(match) {
		if(rc > 0) {
			match->count = rc;
			match->ovector = pcre2_get_ovector_pointer_8(m);
		}
	}
	else
		pcre2_match_data_free_8(m);
	
	return rc >= 0;
}

String Regex::GetLastErrorText() const
{
    PCRE2_UCHAR8 buffer[256];
    pcre2_get_error_message_8(errorcode, buffer, sizeof(buffer));
    return (char *)buffer;
}

void Regex::Clear()
{
	if(re)
	    pcre2_code_free_8(re);
}

bool Regex::Set(dword opts, const char *pattern, int len)
{
	Clear();

    pattern_serial = ++RegexMatch::serial;
    
    options = opts;

    PCRE2_SIZE erroroffset;
    pcre2_compile_context_8 *cctx = pcre2_compile_context_create_8(s_pcre2_ctx_8);
    re = pcre2_compile_8((PCRE2_SPTR8)pattern, len, options, &errorcode, &erroroffset, cctx);
    pcre2_compile_context_free_8(cctx);

    return re;
}

void WRegex::Clear()
{
	if(re)
	    pcre2_code_free_32(re);
}

bool WRegex::Match(const wchar *text, int len, RegexMatch *match) const
{
	pcre2_match_data_32 *m = nullptr;
	if(match) {
		if(match->pattern_serial != pattern_serial) {
			match->Clear();
			match->match_data_32 = pcre2_match_data_create_from_pattern_32(re, s_pcre2_ctx_32);
			match->pattern_serial = pattern_serial;
		}
		m = match->match_data_32;
		match->count = 0;
	}
	else
		m = pcre2_match_data_create_32(1, s_pcre2_ctx_32);

    int rc = pcre2_match_32(
        re,                    // compiled pattern
        (PCRE2_SPTR32)text,     // subject string
        len,                   // length
        0,                     // offset
        options & (PCRE2_ANCHORED|PCRE2_NO_UTF_CHECK|PCRE2_ENDANCHORED),
        m,
        NULL                   // default match context
    );

	if(match) {
		if(rc > 0) {
			match->count = rc;
			match->ovector = pcre2_get_ovector_pointer_32(m);
		}
	}
	else
		pcre2_match_data_free_32(m);
	
	return rc >= 0;
}

bool WRegex::Set(dword opts, const wchar *pattern, int len)
{
	Clear();

    pattern_serial = ++RegexMatch::serial;
    
    options = opts;

    PCRE2_SIZE erroroffset;
    pcre2_compile_context_32 *cctx = pcre2_compile_context_create_32(s_pcre2_ctx_32);
    re = pcre2_compile_32((PCRE2_SPTR32)pattern, len, options, &errorcode, &erroroffset, cctx);
    pcre2_compile_context_free_32(cctx);

    return re;
}

String WRegex::GetLastErrorText() const
{
    PCRE2_UCHAR8 buffer[256];
    pcre2_get_error_message_8(errorcode, buffer, sizeof(buffer));
    return (char *)buffer;
}

}