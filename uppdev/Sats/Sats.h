#ifndef _Sats_Sats_h
#define _Sats_Sats_h

#include <CtrlLib/CtrlLib.h>
#include <plugin/sqlite3/lib/sqlite3.h>
#include <plugin/sqlite3/Sqlite3.h>

using namespace Upp;

#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#define MODEL <Sats/Sats.sch>

#include <Sql/sch_schema.h>
#include <Sql/sch_header.h>
#include <Sql/sch_source.h>

#define LAYOUTFILE <Sats/Sats.lay>
#include <CtrlCore/lay.h>



class Sats : public WithSatsLayout<TopWindow> {
public:
	typedef Sats CLASSNAME;
	void Setup();
};

#endif

