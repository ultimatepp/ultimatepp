#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	try {
		String d = LoadFile(GetDataFile("XmlBug.xml"));

		XmlParser p(d);

		while(!p.IsTag()) p.Skip();

		p.PassTag("Settings");
		
		String Host, CustomersUrl, CustomersSince, ProductsUrl, ProductsSince, OrdersUrl, OrdersSince;
		int    Port, CycleTime;

		while(!p.IsEof())
		{
			if(p.Tag("WebServer"))
			{
				Host = p["Host"];
				Port = atoi(p["Port"]);
			}
			else if(p.Tag("CustomerList"))
			{
				CustomersUrl = p["Url"];
				DUMP(CustomersUrl);
				CustomersSince = p["Since"];
			}
			else if(p.Tag("ProductList"))
			{
				ProductsUrl = p["Url"];
				ProductsSince = p["Since"];
			}
			else if(p.Tag("OrderList"))
			{
				OrdersUrl = p["Url"];
				OrdersSince = p["Since"];
			}
			else if(p.Tag("Cycle"))
			{
				CycleTime = atoi(p["Time"]);
				if (CycleTime <= 0)
				{
					CycleTime = 10;
				}
			}
			else
			{
				p.Skip();
			}
		}
	}
	catch(XmlError)
	{
		LOG("Error");
	}
}

