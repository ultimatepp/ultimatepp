// Created by ReC (rev.4) 19 april 2011

//============================================================================
//                   Check template generation settings
//============================================================================

// Name of the template class
#ifndef HC_TEMPLATE_NAME
#error "Please define HC_TEMPLATE_NAME"
#endif

// Caller prototype for non-void functions
#ifndef HC_NORMAL_CALLER
#error "Please define HC_NORMAL_CALLER"
#endif

// Caller prototype for void functions
#ifndef HC_VOID_CALLER
#error "Please define HC_VOID_CALLER"
#endif

// How to extract and convert argument to required type
#ifndef HC_ARGUMENT
#error "Please define HC_ARGUMENT"
#endif

#ifndef HC_BASE_CLASS
#error "Please define HC_BASE_CLASS"
#endif


//============================================================================
//                Some helpers to simplify shitty-coding
//============================================================================

// EXPANDS2(A, B) -> A1 B, A2 B
#define EXPANDS1(Prefix,Suffix) Prefix##1 Suffix
#define EXPANDS2(Prefix,Suffix) EXPANDS1(Prefix,Suffix), Prefix##2 Suffix
#define EXPANDS3(Prefix,Suffix) EXPANDS2(Prefix,Suffix), Prefix##3 Suffix
#define EXPANDS4(Prefix,Suffix) EXPANDS3(Prefix,Suffix), Prefix##4 Suffix
#define EXPANDS5(Prefix,Suffix) EXPANDS4(Prefix,Suffix), Prefix##5 Suffix
#define EXPANDS6(Prefix,Suffix) EXPANDS5(Prefix,Suffix), Prefix##6 Suffix
#define EXPANDS7(Prefix,Suffix) EXPANDS6(Prefix,Suffix), Prefix##7 Suffix
#define EXPANDS8(Prefix,Suffix) EXPANDS7(Prefix,Suffix), Prefix##8 Suffix
#define EXPANDS9(Prefix,Suffix) EXPANDS8(Prefix,Suffix), Prefix##9 Suffix
#define EXPANDS10(Prefix,Suffix) EXPANDS9(Prefix,Suffix), Prefix##10 Suffix

// EXPAND2(A) -> A1, A2
#define EXPAND1(Prefix) Prefix##1
#define EXPAND2(Prefix) EXPAND1(Prefix), Prefix##2
#define EXPAND3(Prefix) EXPAND2(Prefix), Prefix##3
#define EXPAND4(Prefix) EXPAND3(Prefix), Prefix##4
#define EXPAND5(Prefix) EXPAND4(Prefix), Prefix##5
#define EXPAND6(Prefix) EXPAND5(Prefix), Prefix##6
#define EXPAND7(Prefix) EXPAND6(Prefix), Prefix##7
#define EXPAND8(Prefix) EXPAND7(Prefix), Prefix##8
#define EXPAND9(Prefix) EXPAND8(Prefix), Prefix##9
#define EXPAND10(Prefix) EXPAND9(Prefix), Prefix##10

// COPY2(A) -> A, A
#define COPY1(Prefix) Prefix
#define COPY2(Prefix) COPY1(Prefix), Prefix
#define COPY3(Prefix) COPY2(Prefix), Prefix
#define COPY4(Prefix) COPY3(Prefix), Prefix
#define COPY5(Prefix) COPY4(Prefix), Prefix
#define COPY6(Prefix) COPY5(Prefix), Prefix
#define COPY7(Prefix) COPY6(Prefix), Prefix
#define COPY8(Prefix) COPY7(Prefix), Prefix
#define COPY9(Prefix) COPY8(Prefix), Prefix
#define COPY10(Prefix) COPY9(Prefix), Prefix

#define ARG1(Prefix) HC_ARGUMENT(1, P##1)
#define ARG2(Prefix) ARG1(Prefix), HC_ARGUMENT(2, Prefix##2)
#define ARG3(Prefix) ARG2(Prefix), HC_ARGUMENT(3, Prefix##3)
#define ARG4(Prefix) ARG3(Prefix), HC_ARGUMENT(4, Prefix##4)
#define ARG5(Prefix) ARG4(Prefix), HC_ARGUMENT(5, Prefix##5)
#define ARG6(Prefix) ARG5(Prefix), HC_ARGUMENT(6, Prefix##6)
#define ARG7(Prefix) ARG6(Prefix), HC_ARGUMENT(7, Prefix##7)
#define ARG8(Prefix) ARG7(Prefix), HC_ARGUMENT(8, Prefix##8)
#define ARG9(Prefix) ARG8(Prefix), HC_ARGUMENT(9, Prefix##9)
#define ARG10(Prefix) ARG9(Prefix), HC_ARGUMENT(10, Prefix##10)

//============================================================================
//                        And now bunch of templates ...
//============================================================================

// Templates for functions with return value
template <class C, class R, EXPANDS10(typename P, =void)>
struct HC_TEMPLATE_NAME HC_BASE_CLASS {
    HC_NORMAL_CALLER(C, R, (EXPAND10(P)), (ARG10(P)))
};

template <class C, class R, EXPAND9(typename P)>
struct HC_TEMPLATE_NAME <C, R, EXPAND9(P), COPY1(void)> HC_BASE_CLASS {
    HC_NORMAL_CALLER(C, R, (EXPAND9(P)), (ARG9(P)))
};

template <class C, class R, EXPAND8(typename P)>
struct HC_TEMPLATE_NAME <C, R, EXPAND8(P), COPY2(void)> HC_BASE_CLASS {
    HC_NORMAL_CALLER(C, R, (EXPAND8(P)), (ARG8(P)))
};

template <class C, class R, EXPAND7(typename P)>
struct HC_TEMPLATE_NAME <C, R, EXPAND7(P), COPY3(void)> HC_BASE_CLASS {
    HC_NORMAL_CALLER(C, R, (EXPAND7(P)), (ARG7(P)))
};

template <class C, class R, EXPAND6(typename P)>
struct HC_TEMPLATE_NAME <C, R, EXPAND6(P), COPY4(void)> HC_BASE_CLASS {
    HC_NORMAL_CALLER(C, R, (EXPAND6(P)), (ARG6(P)))
};

template <class C, class R, EXPAND5(typename P)>
struct HC_TEMPLATE_NAME <C, R, EXPAND5(P), COPY5(void)> HC_BASE_CLASS {
    HC_NORMAL_CALLER(C, R, (EXPAND5(P)), (ARG5(P)))
};

template <class C, class R, EXPAND4(typename P)>
struct HC_TEMPLATE_NAME <C, R, EXPAND4(P), COPY6(void)> HC_BASE_CLASS {
    HC_NORMAL_CALLER(C, R, (EXPAND4(P)), (ARG4(P)))
};

template <class C, class R, EXPAND3(typename P)>
struct HC_TEMPLATE_NAME <C, R, EXPAND3(P), COPY7(void)> HC_BASE_CLASS {
    HC_NORMAL_CALLER(C, R, (EXPAND3(P)), (ARG3(P)))
};

template <class C, class R, EXPAND2(typename P)>
struct HC_TEMPLATE_NAME <C, R, EXPAND2(P), COPY8(void)> HC_BASE_CLASS {
    HC_NORMAL_CALLER(C, R, (EXPAND2(P)), (ARG2(P)))
};

template <class C, class R, EXPAND1(typename P)>
struct HC_TEMPLATE_NAME <C, R, EXPAND1(P), COPY9(void)> HC_BASE_CLASS {
    HC_NORMAL_CALLER(C, R, (EXPAND1(P)), (ARG1(P)))
};

template <class C, class R>
struct HC_TEMPLATE_NAME <C, R, COPY10(void)> HC_BASE_CLASS {
    HC_NORMAL_CALLER(C, R, (), ())
};

// Templates for functions without return value
template <class C, EXPAND10(typename P)>
struct HC_TEMPLATE_NAME <C, void, EXPAND10(P)> HC_BASE_CLASS {
    HC_VOID_CALLER(C, (EXPAND10(P)), (ARG10(P)))
};

template <class C, EXPAND9(typename P)>
struct HC_TEMPLATE_NAME <C, void, EXPAND9(P), COPY1(void)> HC_BASE_CLASS {
    HC_VOID_CALLER(C, (EXPAND9(P)), (ARG9(P)))
};

template <class C, EXPAND8(typename P)>
struct HC_TEMPLATE_NAME <C, void, EXPAND8(P), COPY2(void)> HC_BASE_CLASS {
    HC_VOID_CALLER(C, (EXPAND8(P)), (ARG8(P)))
};

template <class C, EXPAND7(typename P)>
struct HC_TEMPLATE_NAME <C, void, EXPAND7(P), COPY3(void)> HC_BASE_CLASS {
    HC_VOID_CALLER(C, (EXPAND7(P)), (ARG7(P)))
};

template <class C, EXPAND6(typename P)>
struct HC_TEMPLATE_NAME <C, void, EXPAND6(P), COPY4(void)> HC_BASE_CLASS {
    HC_VOID_CALLER(C, (EXPAND6(P)), (ARG6(P)))
};

template <class C, EXPAND5(typename P)>
struct HC_TEMPLATE_NAME <C, void, EXPAND5(P), COPY5(void)> HC_BASE_CLASS {
    HC_VOID_CALLER(C, (EXPAND5(P)), (ARG5(P)))
};

template <class C, EXPAND4(typename P)>
struct HC_TEMPLATE_NAME <C, void, EXPAND4(P), COPY6(void)> HC_BASE_CLASS {
    HC_VOID_CALLER(C, (EXPAND4(P)), (ARG4(P)))
};

template <class C, EXPAND3(typename P)>
struct HC_TEMPLATE_NAME <C, void, EXPAND3(P), COPY7(void)> HC_BASE_CLASS {
    HC_VOID_CALLER(C, (EXPAND3(P)), (ARG3(P)))
};

template <class C, EXPAND2(typename P)>
struct HC_TEMPLATE_NAME <C, void, EXPAND2(P), COPY8(void)> HC_BASE_CLASS {
    HC_VOID_CALLER(C, (EXPAND2(P)), (ARG2(P)))
};

template <class C, EXPAND1(typename P)>
struct HC_TEMPLATE_NAME <C, void, EXPAND1(P), COPY9(void)> HC_BASE_CLASS {
    HC_VOID_CALLER(C, (EXPAND1(P)), (ARG1(P)))
};

template <class C>
struct HC_TEMPLATE_NAME <C, void, COPY10(void)> HC_BASE_CLASS {
    HC_VOID_CALLER(C, (), ())
};

//============================================================================
//                           Undefine all macroshit
//============================================================================
#undef EXPAND1
#undef EXPAND2
#undef EXPAND3
#undef EXPAND4
#undef EXPAND5
#undef EXPAND6
#undef EXPAND7
#undef EXPAND8
#undef EXPAND9
#undef EXPAND10

#undef EXPANDS1
#undef EXPANDS2
#undef EXPANDS3
#undef EXPANDS4
#undef EXPANDS5
#undef EXPANDS6
#undef EXPANDS7
#undef EXPANDS8
#undef EXPANDS9
#undef EXPANDS10

#undef COPY1
#undef COPY2
#undef COPY3
#undef COPY4
#undef COPY5
#undef COPY6
#undef COPY7
#undef COPY8
#undef COPY9
#undef COPY10

#undef ARG1
#undef ARG2
#undef ARG3
#undef ARG4
#undef ARG5
#undef ARG6
#undef ARG7
#undef ARG8
#undef ARG9
#undef ARG10

#undef HC_TEMPLATE_NAME
#undef HC_NORMAL_CALLER
#undef HC_VOID_CALLER
#undef HC_ARGUMENT
#undef HC_BASE_CLASS
