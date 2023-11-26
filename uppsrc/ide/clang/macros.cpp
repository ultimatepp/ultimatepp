#include "clang.h"

// we are using libclang.dll for MSVC and therefore need to redefine standard macros
// to mingw clang

//#BLITZ_PROHIBIT

#ifdef PLATFORM_WIN32
const char umacros[] =
R"(#define CHECK_GRID(g) 
#define CHECK_MATRIX(g) 
#define CHECK_NUMBER(g) 
#define _CPPRTTI 1
#define _CPPUNWIND 1
#define _HAS_CHAR16_T_LANGUAGE_SUPPORT 1
#define _INTEGRAL_MAX_BITS 64
#define _MSC_BUILD 1
#define _MSC_EXTENSIONS 1
#define _MSC_FULL_VER 192328106
#define _MSC_VER 1923
#define _MSVC_EXECUTION_CHARACTER_SET 65001
#define _MSVC_LANG 201402L
#define _M_AMD64 100
#define _M_X64 100
#define _NATIVE_NULLPTR_SUPPORTED 1
#define _NATIVE_WCHAR_T_DEFINED 1
#define _RVALUE_REFERENCES_SUPPORTED 1
#define _RVALUE_REFERENCES_V2_SUPPORTED 1
#define _WCHAR_T_DEFINED 1
#define _WIN32 1
#define _WIN64 1
#define __ATOMIC_ACQUIRE 2
#define __ATOMIC_ACQ_REL 4
#define __ATOMIC_CONSUME 1
#define __ATOMIC_RELAXED 0
#define __ATOMIC_RELEASE 3
#define __ATOMIC_SEQ_CST 5
#define __BIGGEST_ALIGNMENT__ 16
#define __BITINT_MAXWIDTH__ 128
#define __BOOL_DEFINED 1
#define __BOOL_WIDTH__ 8
#define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
#define __CHAR16_TYPE__ unsigned short
#define __CHAR32_TYPE__ unsigned int
#define __CHAR_BIT__ 8
#define __CLANG_ATOMIC_BOOL_LOCK_FREE 2
#define __CLANG_ATOMIC_CHAR16_T_LOCK_FREE 2
#define __CLANG_ATOMIC_CHAR32_T_LOCK_FREE 2
#define __CLANG_ATOMIC_CHAR_LOCK_FREE 2
#define __CLANG_ATOMIC_INT_LOCK_FREE 2
#define __CLANG_ATOMIC_LLONG_LOCK_FREE 2
#define __CLANG_ATOMIC_LONG_LOCK_FREE 2
#define __CLANG_ATOMIC_POINTER_LOCK_FREE 2
#define __CLANG_ATOMIC_SHORT_LOCK_FREE 2
#define __CLANG_ATOMIC_WCHAR_T_LOCK_FREE 2
#define __CONSTANT_CFSTRINGS__ 1
#define __DBL_DECIMAL_DIG__ 17
#define __DBL_DENORM_MIN__ 4.9406564584124654e-324
#define __DBL_DIG__ 15
#define __DBL_EPSILON__ 2.2204460492503131e-16
#define __DBL_HAS_DENORM__ 1
#define __DBL_HAS_INFINITY__ 1
#define __DBL_HAS_QUIET_NAN__ 1
#define __DBL_MANT_DIG__ 53
#define __DBL_MAX_10_EXP__ 308
#define __DBL_MAX_EXP__ 1024
#define __DBL_MAX__ 1.7976931348623157e+308
#define __DBL_MIN_10_EXP__ (-307)
#define __DBL_MIN_EXP__ (-1021)
#define __DBL_MIN__ 2.2250738585072014e-308
#define __DECIMAL_DIG__ __LDBL_DECIMAL_DIG__
#define __DEPRECATED 1
#define __FINITE_MATH_ONLY__ 0
#define __FLT_DECIMAL_DIG__ 9
#define __FLT_DENORM_MIN__ 1.40129846e-45F
#define __FLT_DIG__ 6
#define __FLT_EPSILON__ 1.19209290e-7F
#define __FLT_EVAL_METHOD__ 0
#define __FLT_HAS_DENORM__ 1
#define __FLT_HAS_INFINITY__ 1
#define __FLT_HAS_QUIET_NAN__ 1
#define __FLT_MANT_DIG__ 24
#define __FLT_MAX_10_EXP__ 38
#define __FLT_MAX_EXP__ 128
#define __FLT_MAX__ 3.40282347e+38F
#define __FLT_MIN_10_EXP__ (-37)
#define __FLT_MIN_EXP__ (-125)
#define __FLT_MIN__ 1.17549435e-38F
#define __FLT_RADIX__ 2
#define __FXSR__ 1
#define __GCC_ASM_FLAG_OUTPUTS__ 1
#define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 1
#define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 1
#define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4 1
#define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8 1
#define __INT16_C_SUFFIX__ 
#define __INT16_FMTd__ "hd"
#define __INT16_FMTi__ "hi"
#define __INT16_MAX__ 32767
#define __INT16_TYPE__ short
#define __INT32_C_SUFFIX__ 
#define __INT32_FMTd__ "d"
#define __INT32_FMTi__ "i"
#define __INT32_MAX__ 2147483647
#define __INT32_TYPE__ int
#define __INT64_C_SUFFIX__ LL
#define __INT64_FMTd__ "lld"
#define __INT64_FMTi__ "lli"
#define __INT64_MAX__ 9223372036854775807LL
#define __INT64_TYPE__ long long int
#define __INT8_C_SUFFIX__ 
#define __INT8_FMTd__ "hhd"
#define __INT8_FMTi__ "hhi"
#define __INT8_MAX__ 127
#define __INT8_TYPE__ signed char
#define __INTMAX_C_SUFFIX__ LL
#define __INTMAX_FMTd__ "lld"
#define __INTMAX_FMTi__ "lli"
#define __INTMAX_MAX__ 9223372036854775807LL
#define __INTMAX_TYPE__ long long int
#define __INTMAX_WIDTH__ 64
#define __INTPTR_FMTd__ "lld"
#define __INTPTR_FMTi__ "lli"
#define __INTPTR_MAX__ 9223372036854775807LL
#define __INTPTR_TYPE__ long long int
#define __INTPTR_WIDTH__ 64
#define __INT_FAST16_FMTd__ "hd"
#define __INT_FAST16_FMTi__ "hi"
#define __INT_FAST16_MAX__ 32767
#define __INT_FAST16_TYPE__ short
#define __INT_FAST16_WIDTH__ 16
#define __INT_FAST32_FMTd__ "d"
#define __INT_FAST32_FMTi__ "i"
#define __INT_FAST32_MAX__ 2147483647
#define __INT_FAST32_TYPE__ int
#define __INT_FAST32_WIDTH__ 32
#define __INT_FAST64_FMTd__ "lld"
#define __INT_FAST64_FMTi__ "lli"
#define __INT_FAST64_MAX__ 9223372036854775807LL
#define __INT_FAST64_TYPE__ long long int
#define __INT_FAST64_WIDTH__ 64
#define __INT_FAST8_FMTd__ "hhd"
#define __INT_FAST8_FMTi__ "hhi"
#define __INT_FAST8_MAX__ 127
#define __INT_FAST8_TYPE__ signed char
#define __INT_FAST8_WIDTH__ 8
#define __INT_LEAST16_FMTd__ "hd"
#define __INT_LEAST16_FMTi__ "hi"
#define __INT_LEAST16_MAX__ 32767
#define __INT_LEAST16_TYPE__ short
#define __INT_LEAST16_WIDTH__ 16
#define __INT_LEAST32_FMTd__ "d"
#define __INT_LEAST32_FMTi__ "i"
#define __INT_LEAST32_MAX__ 2147483647
#define __INT_LEAST32_TYPE__ int
#define __INT_LEAST32_WIDTH__ 32
#define __INT_LEAST64_FMTd__ "lld"
#define __INT_LEAST64_FMTi__ "lli"
#define __INT_LEAST64_MAX__ 9223372036854775807LL
#define __INT_LEAST64_TYPE__ long long int
#define __INT_LEAST64_WIDTH__ 64
#define __INT_LEAST8_FMTd__ "hhd"
#define __INT_LEAST8_FMTi__ "hhi"
#define __INT_LEAST8_MAX__ 127
#define __INT_LEAST8_TYPE__ signed char
#define __INT_LEAST8_WIDTH__ 8
#define __INT_MAX__ 2147483647
#define __INT_WIDTH__ 32
#define __LDBL_DECIMAL_DIG__ 17
#define __LDBL_DENORM_MIN__ 4.9406564584124654e-324L
#define __LDBL_DIG__ 15
#define __LDBL_EPSILON__ 2.2204460492503131e-16L
#define __LDBL_HAS_DENORM__ 1
#define __LDBL_HAS_INFINITY__ 1
#define __LDBL_HAS_QUIET_NAN__ 1
#define __LDBL_MANT_DIG__ 53
#define __LDBL_MAX_10_EXP__ 308
#define __LDBL_MAX_EXP__ 1024
#define __LDBL_MAX__ 1.7976931348623157e+308L
#define __LDBL_MIN_10_EXP__ (-307)
#define __LDBL_MIN_EXP__ (-1021)
#define __LDBL_MIN__ 2.2250738585072014e-308L
#define __LITTLE_ENDIAN__ 1
#define __LLONG_WIDTH__ 64
#define __LONG_LONG_MAX__ 9223372036854775807LL
#define __LONG_MAX__ 2147483647L
#define __LONG_WIDTH__ 32
#define __MMX__ 1
#define __NO_INLINE__ 1
#define __NO_MATH_INLINES 1
#define __OBJC_BOOL_IS_BOOL 0
#define __OPENCL_MEMORY_SCOPE_ALL_SVM_DEVICES 3
#define __OPENCL_MEMORY_SCOPE_DEVICE 2
#define __OPENCL_MEMORY_SCOPE_SUB_GROUP 4
#define __OPENCL_MEMORY_SCOPE_WORK_GROUP 1
#define __OPENCL_MEMORY_SCOPE_WORK_ITEM 0
#define __ORDER_BIG_ENDIAN__ 4321
#define __ORDER_LITTLE_ENDIAN__ 1234
#define __ORDER_PDP_ENDIAN__ 3412
#define __PIC__ 2
#define __POINTER_WIDTH__ 64
#define __PRAGMA_REDEFINE_EXTNAME 1
#define __PTRDIFF_FMTd__ "lld"
#define __PTRDIFF_FMTi__ "lli"
#define __PTRDIFF_MAX__ 9223372036854775807LL
#define __PTRDIFF_TYPE__ long long int
#define __PTRDIFF_WIDTH__ 64
#define __REGISTER_PREFIX__ 
#define __SCHAR_MAX__ 127
#define __SEG_FS 1
#define __SEG_GS 1
#define __SHRT_MAX__ 32767
#define __SHRT_WIDTH__ 16
#define __SIG_ATOMIC_MAX__ 2147483647
#define __SIG_ATOMIC_WIDTH__ 32
#define __SIZEOF_DOUBLE__ 8
#define __SIZEOF_FLOAT__ 4
#define __SIZEOF_INT128__ 16
#define __SIZEOF_INT__ 4
#define __SIZEOF_LONG_DOUBLE__ 8
#define __SIZEOF_LONG_LONG__ 8
#define __SIZEOF_LONG__ 4
#define __SIZEOF_POINTER__ 8
#define __SIZEOF_PTRDIFF_T__ 8
#define __SIZEOF_SHORT__ 2
#define __SIZEOF_SIZE_T__ 8
#define __SIZEOF_WCHAR_T__ 2
#define __SIZEOF_WINT_T__ 2
#define __SIZE_FMTX__ "llX"
#define __SIZE_FMTo__ "llo"
#define __SIZE_FMTu__ "llu"
#define __SIZE_FMTx__ "llx"
#define __SIZE_MAX__ 18446744073709551615ULL
#define __SIZE_TYPE__ long long unsigned int
#define __SIZE_WIDTH__ 64
#define __SSE2_MATH__ 1
#define __SSE2__ 1
#define __SSE_MATH__ 1
#define __SSE__ 1
#define __STDCPP_DEFAULT_NEW_ALIGNMENT__ 16ULL
#define __STDCPP_THREADS__ 1
#define __STDC_HOSTED__ 1
#define __STDC_NO_THREADS__ 1
#define __STDC_UTF_16__ 1
#define __STDC_UTF_32__ 1
#define __UINT16_C_SUFFIX__ 
#define __UINT16_FMTX__ "hX"
#define __UINT16_FMTo__ "ho"
#define __UINT16_FMTu__ "hu"
#define __UINT16_FMTx__ "hx"
#define __UINT16_MAX__ 65535
#define __UINT16_TYPE__ unsigned short
#define __UINT32_C_SUFFIX__ U
#define __UINT32_FMTX__ "X"
#define __UINT32_FMTo__ "o"
#define __UINT32_FMTu__ "u"
#define __UINT32_FMTx__ "x"
#define __UINT32_MAX__ 4294967295U
#define __UINT32_TYPE__ unsigned int
#define __UINT64_C_SUFFIX__ ULL
#define __UINT64_FMTX__ "llX"
#define __UINT64_FMTo__ "llo"
#define __UINT64_FMTu__ "llu"
#define __UINT64_FMTx__ "llx"
#define __UINT64_MAX__ 18446744073709551615ULL
#define __UINT64_TYPE__ long long unsigned int
#define __UINT8_C_SUFFIX__ 
#define __UINT8_FMTX__ "hhX"
#define __UINT8_FMTo__ "hho"
#define __UINT8_FMTu__ "hhu"
#define __UINT8_FMTx__ "hhx"
#define __UINT8_MAX__ 255
#define __UINT8_TYPE__ unsigned char
#define __UINTMAX_C_SUFFIX__ ULL
#define __UINTMAX_FMTX__ "llX"
#define __UINTMAX_FMTo__ "llo"
#define __UINTMAX_FMTu__ "llu"
#define __UINTMAX_FMTx__ "llx"
#define __UINTMAX_MAX__ 18446744073709551615ULL
#define __UINTMAX_TYPE__ long long unsigned int
#define __UINTMAX_WIDTH__ 64
#define __UINTPTR_FMTX__ "llX"
#define __UINTPTR_FMTo__ "llo"
#define __UINTPTR_FMTu__ "llu"
#define __UINTPTR_FMTx__ "llx"
#define __UINTPTR_MAX__ 18446744073709551615ULL
#define __UINTPTR_TYPE__ long long unsigned int
#define __UINTPTR_WIDTH__ 64
#define __UINT_FAST16_FMTX__ "hX"
#define __UINT_FAST16_FMTo__ "ho"
#define __UINT_FAST16_FMTu__ "hu"
#define __UINT_FAST16_FMTx__ "hx"
#define __UINT_FAST16_MAX__ 65535
#define __UINT_FAST16_TYPE__ unsigned short
#define __UINT_FAST32_FMTX__ "X"
#define __UINT_FAST32_FMTo__ "o"
#define __UINT_FAST32_FMTu__ "u"
#define __UINT_FAST32_FMTx__ "x"
#define __UINT_FAST32_MAX__ 4294967295U
#define __UINT_FAST32_TYPE__ unsigned int
#define __UINT_FAST64_FMTX__ "llX"
#define __UINT_FAST64_FMTo__ "llo"
#define __UINT_FAST64_FMTu__ "llu"
#define __UINT_FAST64_FMTx__ "llx"
#define __UINT_FAST64_MAX__ 18446744073709551615ULL
#define __UINT_FAST64_TYPE__ long long unsigned int
#define __UINT_FAST8_FMTX__ "hhX"
#define __UINT_FAST8_FMTo__ "hho"
#define __UINT_FAST8_FMTu__ "hhu"
#define __UINT_FAST8_FMTx__ "hhx"
#define __UINT_FAST8_MAX__ 255
#define __UINT_FAST8_TYPE__ unsigned char
#define __UINT_LEAST16_FMTX__ "hX"
#define __UINT_LEAST16_FMTo__ "ho"
#define __UINT_LEAST16_FMTu__ "hu"
#define __UINT_LEAST16_FMTx__ "hx"
#define __UINT_LEAST16_MAX__ 65535
#define __UINT_LEAST16_TYPE__ unsigned short
#define __UINT_LEAST32_FMTX__ "X"
#define __UINT_LEAST32_FMTo__ "o"
#define __UINT_LEAST32_FMTu__ "u"
#define __UINT_LEAST32_FMTx__ "x"
#define __UINT_LEAST32_MAX__ 4294967295U
#define __UINT_LEAST32_TYPE__ unsigned int
#define __UINT_LEAST64_FMTX__ "llX"
#define __UINT_LEAST64_FMTo__ "llo"
#define __UINT_LEAST64_FMTu__ "llu"
#define __UINT_LEAST64_FMTx__ "llx"
#define __UINT_LEAST64_MAX__ 18446744073709551615ULL
#define __UINT_LEAST64_TYPE__ long long unsigned int
#define __UINT_LEAST8_FMTX__ "hhX"
#define __UINT_LEAST8_FMTo__ "hho"
#define __UINT_LEAST8_FMTu__ "hhu"
#define __UINT_LEAST8_FMTx__ "hhx"
#define __UINT_LEAST8_MAX__ 255
#define __UINT_LEAST8_TYPE__ unsigned char
#define __USER_LABEL_PREFIX__ 
#define __VERSION__ "Clang 14.0.0"
#define __WCHAR_MAX__ 65535
#define __WCHAR_TYPE__ unsigned short
#define __WCHAR_UNSIGNED__ 1
#define __WCHAR_WIDTH__ 16
#define __WINT_MAX__ 65535
#define __WINT_TYPE__ unsigned short
#define __WINT_UNSIGNED__ 1
#define __WINT_WIDTH__ 16
#define __amd64 1
#define __amd64__ 1
#define __clang__ 1
#define __clang_literal_encoding__ "UTF-8"
#define __clang_major__ 14
#define __clang_minor__ 0
#define __clang_patchlevel__ 0
#define __clang_version__ "14.0.0 "
#define __clang_wide_literal_encoding__ "UTF-16"
#define __code_model_small__ 1
#define __cplusplus 201402L
#define __cpp_aggregate_nsdmi 201304L
#define __cpp_alias_templates 200704L
#define __cpp_attributes 200809L
#define __cpp_binary_literals 201304L
#define __cpp_constexpr 201304L
#define __cpp_constexpr_in_decltype 201711L
#define __cpp_decltype 200707L
#define __cpp_decltype_auto 201304L
#define __cpp_delegating_constructors 200604L
#define __cpp_digit_separators 201309L
#define __cpp_exceptions 199711L
#define __cpp_generic_lambdas 201304L
#define __cpp_impl_destroying_delete 201806L
#define __cpp_inheriting_constructors 201511L
#define __cpp_init_captures 201304L
#define __cpp_initializer_lists 200806L
#define __cpp_lambdas 200907L
#define __cpp_nsdmi 200809L
#define __cpp_range_based_for 200907
#define __cpp_raw_strings 200710L
#define __cpp_ref_qualifiers 200710L
#define __cpp_return_type_deduction 201304L
#define __cpp_rtti 199711L
#define __cpp_rvalue_references 200610L
#define __cpp_static_assert 200410
#define __cpp_threadsafe_static_init 200806L
#define __cpp_unicode_characters 200704L
#define __cpp_unicode_literals 200710L
#define __cpp_user_defined_literals 200809L
#define __cpp_variable_templates 201304L
#define __cpp_variadic_templates 200704L
#define __k8 1
#define __k8__ 1
#define __llvm__ 1
#define __pic__ 2
#define __seg_fs __attribute__((address_space(257)))
#define __seg_gs __attribute__((address_space(256)))
#define __tune_k8__ 1
#define __x86_64 1
#define __x86_64__ 1
)";
#endif

String RedefineMacros()
{
#ifdef PLATFORM_WIN32
	INTERLOCKED {
		static String cmdline;
		static int cpp_version = 0;

		if(cpp_version != LibClangCppVersion) {
			cpp_version = LibClangCppVersion;
			VectorMap<String, String> um, dm;
			auto Parse = [&](const char *s) {
				VectorMap<String, String> r;
				StringStream ss(s);
				while(!ss.IsEof()) {
					String line = ss.GetLine();
					if(line.TrimStart("#define ") && line.Find('\"') < 0) {
						int q = line.Find(' ');
						r.GetAdd(TrimBoth(line.Mid(0, q))) = TrimBoth(line.Mid(q + 1));
					}
				}
				return r;
			};
			
			um = Parse(umacros);
			
			String dummy = ConfigFile("dummy.cpp");
			Upp::SaveFile(dummy, String());
			String dmacros = Sys(GetExeDirFile("bin/clang/bin/c++") +
			                     " -v -std=c++" + AsString(cpp_version) + " -xc++ -dM -E " + dummy);

			dm = Parse(dmacros);
			
			for(const auto& h : ~um) {
				int q = dm.Find(h.key);
				if(q < 0)
					cmdline << " -U" << h.key;
				else
				if(dm[q] != h.value)
					cmdline << " -U" << h.key << " -D" << h.key << "=" << dm[q];
			}
			for(const auto& h : ~dm)
				if(um.Find(h.key) < 0)
					cmdline << " -D" << h.key << "=" << h.value;
		}
	
		cmdline << " -Dlint";
	
		return cmdline;
	}
#endif	
	return Null;
}
