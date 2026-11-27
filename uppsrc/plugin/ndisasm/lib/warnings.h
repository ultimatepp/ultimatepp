#ifndef NASM_WARNINGS_H
#define NASM_WARNINGS_H

#ifndef WARN_SHR
# error "warnings.h should only be included from within error.h"
#endif

enum warn_index {
    WARN_IDX_NONE                            =   0, /* not suppressible */
    WARN_IDX_DB_EMPTY                        =   1, /* no operand for data declaration */
    WARN_IDX_DIRECTIVE_GARBAGE_EOL           =   2, /* garbage after directive */
    WARN_IDX_EA_ABSOLUTE                     =   3, /* absolute address cannot be RIP-relative */
    WARN_IDX_EA_DISPSIZE                     =   4, /* displacement size ignored on absolute address */
    WARN_IDX_FLOAT_DENORM                    =   5, /* floating point denormal */
    WARN_IDX_FLOAT_OVERFLOW                  =   6, /* floating point overflow */
    WARN_IDX_FLOAT_TOOLONG                   =   7, /* too many digits in floating-point number */
    WARN_IDX_FLOAT_UNDERFLOW                 =   8, /* floating point underflow */
    WARN_IDX_FORWARD                         =   9, /* forward reference may have unpredictable results */
    WARN_IDX_IMPLICIT_ABS_DEPRECATED         =  10, /* implicit DEFAULT ABS is deprecated */
    WARN_IDX_LABEL_ORPHAN                    =  11, /* labels alone on lines without trailing : */
    WARN_IDX_LABEL_REDEF                     =  12, /* label redefined to an identical value */
    WARN_IDX_LABEL_REDEF_LATE                =  13, /* label (re)defined during code generation */
    WARN_IDX_NUMBER_DEPRECATED_HEX           =  14, /* $ prefix for hexadecimal is deprecated */
    WARN_IDX_NUMBER_OVERFLOW                 =  15, /* numeric constant does not fit */
    WARN_IDX_OBSOLETE_NOP                    =  16, /* instruction obsolete and is a noop on the target CPU */
    WARN_IDX_OBSOLETE_REMOVED                =  17, /* instruction obsolete and removed on the target CPU */
    WARN_IDX_OBSOLETE_VALID                  =  18, /* instruction obsolete but valid on the target CPU */
    WARN_IDX_PHASE                           =  19, /* phase error during stabilization */
    WARN_IDX_PP_ELSE_ELIF                    =  20, /* %elif after %else */
    WARN_IDX_PP_ELSE_ELSE                    =  21, /* %else after %else */
    WARN_IDX_PP_EMPTY_BRACES                 =  22, /* empty %{} construct */
    WARN_IDX_PP_ENVIRONMENT                  =  23, /* nonexistent environment variable */
    WARN_IDX_PP_MACRO_DEF_CASE_SINGLE        =  24, /* single-line macro defined both case sensitive and insensitive */
    WARN_IDX_PP_MACRO_DEF_GREEDY_SINGLE      =  25, /* single-line macro */
    WARN_IDX_PP_MACRO_DEF_PARAM_SINGLE       =  26, /* single-line macro defined with and without parameters */
    WARN_IDX_PP_MACRO_DEFAULTS               =  27, /* macros with more default than optional parameters */
    WARN_IDX_PP_MACRO_PARAMS_LEGACY          =  28, /* improperly calling multi-line macro for legacy support */
    WARN_IDX_PP_MACRO_PARAMS_MULTI           =  29, /* multi-line macro calls with wrong parameter count */
    WARN_IDX_PP_MACRO_PARAMS_SINGLE          =  30, /* single-line macro calls with wrong parameter count */
    WARN_IDX_PP_MACRO_REDEF_MULTI            =  31, /* redefining multi-line macro */
    WARN_IDX_PP_OPEN_BRACES                  =  32, /* unterminated %{...} */
    WARN_IDX_PP_OPEN_BRACKETS                =  33, /* unterminated %[...] */
    WARN_IDX_PP_OPEN_STRING                  =  34, /* unterminated string */
    WARN_IDX_PP_REP_NEGATIVE                 =  35, /* negative %rep count */
    WARN_IDX_PP_RESERVED                     =  36, /* reserved unimplemented preprocessor directive */
    WARN_IDX_PP_SEL_RANGE                    =  37, /* %sel() argument out of range */
    WARN_IDX_PP_TRAILING                     =  38, /* trailing garbage ignored */
    WARN_IDX_PRAGMA_BAD                      =  39, /* malformed %pragma */
    WARN_IDX_PRAGMA_EMPTY                    =  40, /* empty %pragma directive */
    WARN_IDX_PRAGMA_NA                       =  41, /* %pragma not applicable to this compilation */
    WARN_IDX_PRAGMA_UNKNOWN                  =  42, /* unknown %pragma facility or directive */
    WARN_IDX_PREFIX_BADMODE_O64              =  43, /* o64 prefix invalid in 16/32-bit mode */
    WARN_IDX_PREFIX_BND                      =  44, /* invalid BND prefix */
    WARN_IDX_PREFIX_HINT_DROPPED             =  45, /* invalid branch hint prefix dropped */
    WARN_IDX_PREFIX_HLE                      =  46, /* invalid HLE prefix */
    WARN_IDX_PREFIX_INVALID                  =  47, /* invalid prefix for instruction */
    WARN_IDX_PREFIX_LOCK_ERROR               =  48, /* LOCK prefix on unlockable instruction */
    WARN_IDX_PREFIX_LOCK_XCHG                =  49, /* superfluous LOCK prefix on XCHG instruction */
    WARN_IDX_PREFIX_OPSIZE                   =  50, /* invalid operand size prefix */
    WARN_IDX_PREFIX_SEG                      =  51, /* segment prefix ignored in 64-bit mode */
    WARN_IDX_PTR                             =  52, /* non-NASM keyword used in other assemblers */
    WARN_IDX_REGSIZE                         =  53, /* register size specification ignored */
    WARN_IDX_RELOC_ABS_BYTE                  =  54, /* 8-bit absolute section-crossing relocation */
    WARN_IDX_RELOC_ABS_DWORD                 =  55, /* 32-bit absolute section-crossing relocation */
    WARN_IDX_RELOC_ABS_QWORD                 =  56, /* 64-bit absolute section-crossing relocation */
    WARN_IDX_RELOC_ABS_WORD                  =  57, /* 16-bit absolute section-crossing relocation */
    WARN_IDX_RELOC_REL_BYTE                  =  58, /* 8-bit relative section-crossing relocation */
    WARN_IDX_RELOC_REL_DWORD                 =  59, /* 32-bit relative section-crossing relocation */
    WARN_IDX_RELOC_REL_QWORD                 =  60, /* 64-bit relative section-crossing relocation */
    WARN_IDX_RELOC_REL_WORD                  =  61, /* 16-bit relative section-crossing relocation */
    WARN_IDX_SECTION_ALIGNMENT_ROUNDED       =  62, /* section alignment rounded up */
    WARN_IDX_UNKNOWN_WARNING                 =  63, /* unknown warning in -W/-w or warning directive */
    WARN_IDX_USER                            =  64, /* %warning directives */
    WARN_IDX_WARN_STACK_EMPTY                =  65, /* warning stack empty */
    WARN_IDX_ZEROING                         =  66, /* RESx in initialized section becomes zero */
    WARN_IDX_ZEXT_RELOC                      =  67, /* relocation zero-extended to match output format */
    WARN_IDX_OTHER                           =  68, /* any warning not assigned to a specific warning class */
    WARN_IDX_ALL                             =  69  /* all possible warnings */
};

enum warn_const {
    WARN_NONE                                =   0 << WARN_SHR,
    WARN_DB_EMPTY                            =   1 << WARN_SHR,
    WARN_DIRECTIVE_GARBAGE_EOL               =   2 << WARN_SHR,
    WARN_EA_ABSOLUTE                         =   3 << WARN_SHR,
    WARN_EA_DISPSIZE                         =   4 << WARN_SHR,
    WARN_FLOAT_DENORM                        =   5 << WARN_SHR,
    WARN_FLOAT_OVERFLOW                      =   6 << WARN_SHR,
    WARN_FLOAT_TOOLONG                       =   7 << WARN_SHR,
    WARN_FLOAT_UNDERFLOW                     =   8 << WARN_SHR,
    WARN_FORWARD                             =   9 << WARN_SHR,
    WARN_IMPLICIT_ABS_DEPRECATED             =  10 << WARN_SHR,
    WARN_LABEL_ORPHAN                        =  11 << WARN_SHR,
    WARN_LABEL_REDEF                         =  12 << WARN_SHR,
    WARN_LABEL_REDEF_LATE                    =  13 << WARN_SHR,
    WARN_NUMBER_DEPRECATED_HEX               =  14 << WARN_SHR,
    WARN_NUMBER_OVERFLOW                     =  15 << WARN_SHR,
    WARN_OBSOLETE_NOP                        =  16 << WARN_SHR,
    WARN_OBSOLETE_REMOVED                    =  17 << WARN_SHR,
    WARN_OBSOLETE_VALID                      =  18 << WARN_SHR,
    WARN_PHASE                               =  19 << WARN_SHR,
    WARN_PP_ELSE_ELIF                        =  20 << WARN_SHR,
    WARN_PP_ELSE_ELSE                        =  21 << WARN_SHR,
    WARN_PP_EMPTY_BRACES                     =  22 << WARN_SHR,
    WARN_PP_ENVIRONMENT                      =  23 << WARN_SHR,
    WARN_PP_MACRO_DEF_CASE_SINGLE            =  24 << WARN_SHR,
    WARN_PP_MACRO_DEF_GREEDY_SINGLE          =  25 << WARN_SHR,
    WARN_PP_MACRO_DEF_PARAM_SINGLE           =  26 << WARN_SHR,
    WARN_PP_MACRO_DEFAULTS                   =  27 << WARN_SHR,
    WARN_PP_MACRO_PARAMS_LEGACY              =  28 << WARN_SHR,
    WARN_PP_MACRO_PARAMS_MULTI               =  29 << WARN_SHR,
    WARN_PP_MACRO_PARAMS_SINGLE              =  30 << WARN_SHR,
    WARN_PP_MACRO_REDEF_MULTI                =  31 << WARN_SHR,
    WARN_PP_OPEN_BRACES                      =  32 << WARN_SHR,
    WARN_PP_OPEN_BRACKETS                    =  33 << WARN_SHR,
    WARN_PP_OPEN_STRING                      =  34 << WARN_SHR,
    WARN_PP_REP_NEGATIVE                     =  35 << WARN_SHR,
    WARN_PP_RESERVED                         =  36 << WARN_SHR,
    WARN_PP_SEL_RANGE                        =  37 << WARN_SHR,
    WARN_PP_TRAILING                         =  38 << WARN_SHR,
    WARN_PRAGMA_BAD                          =  39 << WARN_SHR,
    WARN_PRAGMA_EMPTY                        =  40 << WARN_SHR,
    WARN_PRAGMA_NA                           =  41 << WARN_SHR,
    WARN_PRAGMA_UNKNOWN                      =  42 << WARN_SHR,
    WARN_PREFIX_BADMODE_O64                  =  43 << WARN_SHR,
    WARN_PREFIX_BND                          =  44 << WARN_SHR,
    WARN_PREFIX_HINT_DROPPED                 =  45 << WARN_SHR,
    WARN_PREFIX_HLE                          =  46 << WARN_SHR,
    WARN_PREFIX_INVALID                      =  47 << WARN_SHR,
    WARN_PREFIX_LOCK_ERROR                   =  48 << WARN_SHR,
    WARN_PREFIX_LOCK_XCHG                    =  49 << WARN_SHR,
    WARN_PREFIX_OPSIZE                       =  50 << WARN_SHR,
    WARN_PREFIX_SEG                          =  51 << WARN_SHR,
    WARN_PTR                                 =  52 << WARN_SHR,
    WARN_REGSIZE                             =  53 << WARN_SHR,
    WARN_RELOC_ABS_BYTE                      =  54 << WARN_SHR,
    WARN_RELOC_ABS_DWORD                     =  55 << WARN_SHR,
    WARN_RELOC_ABS_QWORD                     =  56 << WARN_SHR,
    WARN_RELOC_ABS_WORD                      =  57 << WARN_SHR,
    WARN_RELOC_REL_BYTE                      =  58 << WARN_SHR,
    WARN_RELOC_REL_DWORD                     =  59 << WARN_SHR,
    WARN_RELOC_REL_QWORD                     =  60 << WARN_SHR,
    WARN_RELOC_REL_WORD                      =  61 << WARN_SHR,
    WARN_SECTION_ALIGNMENT_ROUNDED           =  62 << WARN_SHR,
    WARN_UNKNOWN_WARNING                     =  63 << WARN_SHR,
    WARN_USER                                =  64 << WARN_SHR,
    WARN_WARN_STACK_EMPTY                    =  65 << WARN_SHR,
    WARN_ZEROING                             =  66 << WARN_SHR,
    WARN_ZEXT_RELOC                          =  67 << WARN_SHR,
    WARN_OTHER                               =  68 << WARN_SHR
};

struct warning_alias {
    const char *name;
    enum warn_index warning;
};

#define NUM_WARNINGS      69
#define NUM_WARNING_ALIAS 85
extern const char * const warning_name[70];
extern const char * const warning_help[70];
extern const struct warning_alias warning_alias[NUM_WARNING_ALIAS];

#endif /* NASM_WARNINGS_H */
extern uint8_t warning_state[NUM_WARNINGS];
extern const uint8_t warning_default[NUM_WARNINGS];
