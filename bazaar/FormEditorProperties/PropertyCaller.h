#ifndef FORM_EDITOR_PROPERTIES__PROPERTY_CALLER_H
#define FORM_EDITOR_PROPERTIES__PROPERTY_CALLER_H

#include "Common.h"

#define HC_TEMPLATE_NAME PropertyCaller

// Prototype for non-void functions
#define HC_NORMAL_CALLER(Class, Return, ArgList, Args) \
static void call(Class &object, Return (Class::*method)ArgList, SProperty& c) { \
	PropReturn<Return>((object.*method)Args, c); \
} \
static void call(const Class &object, Return (Class::*method)ArgList const, SProperty& c) { \
	PropReturn<Return>((object.*method)Args, c); \
}

// Prototype for void functions
#define HC_VOID_CALLER(Class, ArgList, Args) \
static void call(Class &object, void (Class::*method)ArgList, SProperty& c) { \
	(object.*method)Args; \
} \
static void call(const Class &object, void (Class::*method)ArgList const, SProperty& c) { \
	(object.*method)Args; \
}

// Argument conversion
#define HC_ARGUMENT(Pos, Type) (Type)ValueConvert(c.GetArg(Pos-1))

// No base class
#define HC_BASE_CLASS

// Generate now!
#include <FormEditorCommon/hypercall.h>

#endif
