// vi: noexpandtab:tabstop=4

#ifndef _ADT_ADT_h
#define _ADT_ADT_h

#ifdef flagNONAMESPACE
#define NAMESPACE_ADT
#define END_ADT_NAMESPACE
#define ADT
#else
#define NAMESPACE_ADT     namespace adt {
#define END_ADT_NAMESPACE }
#define ADT               adt
#endif 

NAMESPACE_ADT

#include "List.h"

END_ADT_NAMESPACE


#endif
