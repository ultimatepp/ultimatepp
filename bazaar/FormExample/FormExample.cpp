#include <Core/Core.h>
using namespace Upp;

#include <Form/Form.hpp>

GUI_APP_MAIN
{
	Form form;
	form.Load(ConfigFile("HelloWorld.form"));
	form.Layout("Default");
	form.Run();
}
