$uvs: PENDING CONFLICT
#include "CtrlLibTest.h"

$uvs: REPOSITORY INSERT
// Run
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
#define IMAGECLASS Img
#define IMAGEFILE <CtrlLibTest/img.iml>
#include <Draw/iml_source.h>

CtrlLibTest::CtrlLibTest()
{
	Sizeable();
	CtrlLayout(*this);
	drop.SetFrame(InsetFrame());
	drop.Add("DropList");
}

$uvs: END REPOSITORY DELETE
GUI_APP_MAIN
{
	CtrlLibTest().Run();
}

