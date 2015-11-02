#include "CypherTest.h"

#include <Cypher/Cypher.h>

CypherTest::CypherTest()
{
	CtrlLayout(*this, "Cypher test");
	
	goBtn <<= THISBACK(onGo);
	cancelBtn.Cancel() <<= Breaker(0);
	cypherSwitch <<= THISBACK(onCypherSwitch);
	onCypherSwitch();
	
	keyEdit <<= "AABBCCDDEEFF00112233445566778899";
	
	splitSwitch = 0;
	cypherSwitch = 0;
	streamSwitch = 0;
}

void CypherTest::onCypherSwitch(void)
{
	if(cypherSwitch == 0)
		keyLabel.SetText("Key (16 or 32 bytes long, hex-ascii format - 0011223344) :");
	else
		keyLabel.SetText("Key, usually from 5 to 32 bytes long, hex-ascii format - 0011223344) :");
}

void CypherTest::onGo()
{
	String hexAsciiKey = ToUpper(String(~keyEdit));
	if(hexAsciiKey.GetCount() != 32 && hexAsciiKey.GetCount() != 64 && cypherSwitch == 0)
	{
		PromptOK("Key MUST be 16 or 32 bytes long for snow2 cypher");
		return;
	}

	// convert key in hex bytes
	String key;
	for(int i = 0; i < hexAsciiKey.GetCount()-1;)
	{
		byte b;
		char c = hexAsciiKey[i++];
		if(c >= '0' && c <= '9')
			b = c - '0';
		else if(c >= 'A' && c <= 'F')
			b = c - 'A' + 10;
		else
		{
			PromptOK(String("Invalid hex digit '") + c + "' in key");
			return;
		}
		b <<= 4;
		c = hexAsciiKey[i++];
		if(c >= '0' && c <= '9')
			b += c - '0';
		else if(c >= 'A' && c <= 'F')
			b += c - 'A' + 10;
		else
		{
			PromptOK(String("Invalid hex digit '") + c + "' in key");
			return;
		}
		key.Cat(b);
	}
	
	// check source and dest file names
	if(~sourceEdit == "" || ~destEdit == "")
		return;
	if(!FileExists(~sourceEdit))
	{
		PromptOK(String().Cat() << "File '" << ~sourceEdit << "' doesn't exists");
		return;
	}
	if(FileExists(~destEdit))
		if(!PromptYesNo(String().Cat() << "File '" << ~destEdit << "' exists&[*= OVERWRITE ?]"))
			return;
	
	// load file to encode	
	String s = LoadFile(~sourceEdit);
	
	// encode it choosing requested encoder
	One<Cypher> cypher;
	switch(cypherSwitch)
	{
		// snow2 Cypher
		case 0:
			cypher = new Snow2;
			break;
			
		// RC4 Cypher
		case 1:
			cypher = new RC4;
			break;
		default:
			PromptOK("Unsupported Cypher");
			return;
	}
	String d;
	
	// sets Cypher's key
	cypher->SetKey(key, "12345");
	
	if(streamSwitch == 0)
	{
		// buffer mode
		if(splitSwitch == 0)
		{
			d = (*cypher)(s);
		}
		else
		{
			int len = s.GetCount();
			int randMax = len / 10;
			
			while(len)
			{
				int rnd = min((int)(Random(randMax) + 1), len);
				String ss = s.Left(rnd);
				s = s.Right(len - rnd);
				len = s.GetCount();
				d += (*cypher)(ss);
			}
		}
	}
	else
	{
		// stream mode
		if(splitSwitch == 0)
		{
			(*cypher) << s;
			(*cypher) >> d;
		}
		else
		{
			int len = s.GetCount();
			int randMax = len / 10;
			
			while(len)
			{
				int rnd = min((int)(Random(randMax) + 1), len);
				String ss = s.Left(rnd);
				s = s.Right(len - rnd);
				len = s.GetCount();
				(*cypher) << ss;
			}
			(*cypher) >> d;
		}
	}
	
	// save encoded file
	if(!SaveFile(~destEdit, d))
	{
		PromptOK("Error writing dest file!");
		return;
	}
	PromptOK("Encoding ok!");
		
}

GUI_APP_MAIN
{
	CypherTest().Run();
}

