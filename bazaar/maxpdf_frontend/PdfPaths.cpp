#include "PdfPaths.h"

/////////////////////////////////////////////////////////////////////////////////////
// gets base path of user data
String GetPdfBasePath(void)
{
	return "/home/" + Environment().Get("USER") + "/.MaxPdf/";

} // END GetPdfBasePath()
	
/////////////////////////////////////////////////////////////////////////////////////
// gets data folder path
String GetPdfDataPath(void)
{
	return GetPdfBasePath() + "data/" ;

} // END GetPdfDataPath()

/////////////////////////////////////////////////////////////////////////////////////
// gets config folder path
String GetPdfConfigPath(void)
{
	return GetPdfBasePath() + "config/" ;

} // END GetPdfConfigPath()
