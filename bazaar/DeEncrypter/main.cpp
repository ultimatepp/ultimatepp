#include "DeEncrypter.h"



DeEncrypter::DeEncrypter()
{
	CtrlLayout(*this, "Window title");
	
	String activedir = GetFileFolder(GetExeFilePath());

	fs.ActiveDir(activedir);
	dirs.ActiveDir(activedir);
	
	encrypt << THISBACK1(Crypt, true);
	decrypt << THISBACK1(Crypt, false);
	generate << THISBACK(Generate);
	open << THISBACK(OpenKey);

	//try preload an existing key
	String filename = activedir + DIR_SEP + DEFFILENAME;
	key = LoadFile(filename);
	if(key.IsEmpty())
	{
		ToInfo("Choose or Generate key");
		return;
	}
	
	if(key.GetLength() > DEFKEYSIZE)
	{
		ToInfo("Could not open key " + filename);
		key.Clear();
		keypath.Clear();
	}
	else
	{
		ToInfo("Key opened from file " + filename);
		keypath.SetData(filename);
	}
}

void DeEncrypter::Crypt(bool encrypt)
{
	if(key.IsEmpty())
	{
		ToInfo("No Key selected");
		return;
	}
	
	fs.Multi();
	fs.ClearTypes();
	
	if(!fs.ExecuteOpen(String().Cat() << "Select files to " << ((encrypt)?("ENcrypt"):("DEcrypt")) ))
	{
		ToInfo("Canceled");
		return;
	}
	
	if(!dirs.ExecuteSelectDir(String().Cat() << "Select directory where to put " << ((encrypt)?("ENcrypted"):("DEcrypted")) << " files (same file name used)"))
	{
		ToInfo("Canceled");
		return;
	}
	String dir = dirs.Get();

	Progress pi;
	pi.Create();
	
	for(int i = 0; i < fs.GetCount(); i++)
	{
		String filename = fs.GetFile(i);
		ToInfo("\n\nProcessing: " + filename);

		String sIn;

		sIn = LoadFile(filename);
		if(sIn.IsEmpty())
		{
			ToInfo("!!! ERR: Could not load: " + filename);
			continue;
		}
		else
			ToInfo("File loaded: " + filename);
		
		String sOut;
		if(0)
			sOut = sIn;
		else
		{
			int total = (sIn.GetLength()/BUFFSIZE) + 1;
			pi.SetTotal(total);
			
			if(encrypt)
			{
				pi.SetText("Encrypting: " + filename);
				AESEncoderStream aesEncoder(sIn.GetLength(), key); 
				for(int i = 0; i < sIn.GetLength();)
				{
					aesEncoder << sIn.Mid(i, BUFFSIZE);
					sOut << aesEncoder;
					i+=BUFFSIZE;
					pi.Step();
				}
			}
			else
			{
				pi.SetText("Decrypting: " + filename);
				AESDecoderStream aesDecoder(key); 
				for(int i = 0; i < sIn.GetLength();)
				{
					aesDecoder << sIn.Mid(i, BUFFSIZE);
					sOut << aesDecoder;
					i+=BUFFSIZE;
					pi.Step();
				}
			}
		}

		filename = dir + DIR_SEP + GetFileName(filename);
		if(!SaveFile(filename, sOut))
			ToInfo("!!! ERR: Could not save: " + filename);
		else
			ToInfo(">> saved to " + filename);
	}

	pi.Close();
}

void DeEncrypter::Generate()
{
	fs.Multi(false);
	fs.Types("All Files\t*\nKey Files\t*.key\nDat Files\t*.dat\n");
	fs.Set(DEFFILENAME);
	if(!fs.ExecuteSaveAs("Save new key to"))
		return;
	String filename = fs.Get();
	
	key = AESRandomString(DEFKEYSIZE);

	if(!SaveFile(filename, key))
	{
		ToInfo("Could not save file " + filename);
		key.Clear();
		keypath.Clear();
		return;
	}

	ToInfo("New key in file " + filename);
	keypath.SetData(filename);
}

void DeEncrypter::OpenKey()
{
	fs.Multi(false);
	fs.Types("All Files\t*\nKey Files\t*.key\nDat Files\t*.dat\n");
	fs.Set("aes32");
	if(!fs.ExecuteOpen("Open key from"))
		return;
	String filename = fs.Get();
	
	key = LoadFile(filename);
	
	if(key.IsEmpty() || key.GetLength() > DEFKEYSIZE)
	{
		ToInfo("Could not open key " + filename);
		key.Clear();
		keypath.Clear();
		return;
	}

	ToInfo("Key opened from file " + filename);
	keypath.SetData(filename);
}


GUI_APP_MAIN
{
	DeEncrypter().Run();
}

