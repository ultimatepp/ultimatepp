#ifndef _Sats2_Sats2_h
#define _Sats2_Sats2_h

#include <CtrlLib/CtrlLib.h>
#include <Report/Report.h>
#include <plugin/sqlite3/lib/sqlite3.h>
#include <plugin/sqlite3/Sqlite3.h>

using namespace Upp;

#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#define MODEL <Sats2/Sats2.sch>

#include <Sql/sch_schema.h>
#include <Sql/sch_header.h>
#include <Sql/sch_source.h>

#define LAYOUTFILE <Sats2/Sats2.lay>
#include <CtrlCore/lay.h>

class Sats : public WithSatsLayout<TopWindow> {
public:	
	WithISTAddLayout<ParentCtrl> ISTAdd;
	WithOpsAreaUpdLayout<ParentCtrl> OpsAreaUpd;
	WithOpsTotalMisUpdLayout<ParentCtrl>OpsTotMisUpd;
	WithRptMonthlyLayout<ParentCtrl>RptMonthly;

	typedef Sats CLASSNAME;
	Sats();
};

#endif

