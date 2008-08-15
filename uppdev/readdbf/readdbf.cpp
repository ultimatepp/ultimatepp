#include <Core/Core.h>
#include <TCore/dbf.h>

using namespace Upp;

CONSOLE_APP_MAIN
{   int i;
    int NFields;
	DbfStream dbf;

	if(!dbf.Open(GetDataFile("club.dbf"), false) ) {
		return;
	}

//	int NFields = dbf.GetFieldCount();
//    DUMP(NFields);
/*	while(dbf.Fetch()) {
		String text;
	//	for (i=0; i<3; i++) {
			text << dbf["CODE"] << dbf["CLUB"];
	//	}
		text << "\n";
		Cout() << text;
	}
  */
    dbf.Close();
}
