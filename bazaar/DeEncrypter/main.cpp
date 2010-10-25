#include "DeEncrypter.h"

DeEncrypter::DeEncrypter()
{
	CtrlLayout(*this, "Window title");

	if(!LoadFromFile(*this))
	{
		String activedir = GetFileFolder(GetExeFilePath());
		fsk.ActiveDir(activedir);
		fsf.ActiveDir(activedir);
		dirs.ActiveDir(activedir);
	}

	encrypt <<= THISBACK1(Crypt, true);
	decrypt <<= THISBACK1(Crypt, false);
	generate <<= THISBACK(Generate);
	open <<= THISBACK(OpenKey);
	clear <<= THISBACK(Clear);

	//try preload an existing key
	String filename = fsk.GetActiveDir() + DIR_SEP + DEFFILENAME;
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

DeEncrypter::~DeEncrypter()
{
	StoreToFile(*this);	
}

void DeEncrypter::Serialize(Stream& s)
{
	String a,b,c,d;
	if(s.IsStoring())
	{
		a = fsk.GetActiveDir();	
		b = fsf.GetActiveDir();	
		c = dirs.GetActiveDir();	
	}
	s % a % b % c;
	if(s.IsLoading())
	{
		fsk.ActiveDir(a);
		fsf.ActiveDir(b);
		dirs.ActiveDir(c);
	}
}

void DeEncrypter::Crypt(bool encrypt)
{
	if(key.IsEmpty())
	{
		ToInfo("No Key selected");
		return;
	}
	
	fsf.Multi();
	fsf.ClearTypes();
	
	if(!fsf.ExecuteOpen(String().Cat() << "Select files to " << ((encrypt)?("ENcrypt"):("DEcrypt")) ))
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
	
	for(int i = 0; i < fsf.GetCount(); i++)
	{
		String fileinfo;
		fileinfo << "(" << (i+1) << "/" << fsf.GetCount() <<") ";
		String filename = fsf.GetFile(i);
		String file = GetFileName(filename);
		String dfilename = dir + DIR_SEP + file;
		if(!keepfname)
			dfilename << "$";
		
		ToInfo("\n\nProcessing: " + fileinfo + filename);

		FileIn in(filename);
		if(!in.IsOpen() || !in.IsOK())
		{
			ToInfo("ERR: Could not source open file: " + filename);
			continue;
		}
		else
			ToInfo("File source opened: " + filename);

		FileOut out(dfilename);
		if(!out.IsOpen() || !out.IsOK())
		{
			ToInfo("ERR: Could not open dest file: " + dfilename);
			continue;
		}
		else
			ToInfo("File destination opened: " + dfilename);
		
		String sIn;
		String sOut;
		int64 filesize = in.GetStreamSize();
		
		pi.Set(0, (int)filesize);

		if(0)
		{
			String msg = "Copying: " + fileinfo + file;
			pi.SetText(msg);
			int time = msecs();
			int64 size = 0;
			while(in.GetLeft()>0)
			{
				sIn = in.Get(in.GetBufferSize());

				sOut = sIn; //copy :)

				out.Put(~sOut, sOut.GetCount());
				pi.Step(sIn.GetCount());

				int tt = msecs();
				int dt = tt - time;
				if(dt > 1000)
				{
					time = tt;
					int64 dsize = in.GetPos() - size;
					size = in.GetPos();
					double rate = (double)dsize/(double)dt;
					pi.SetText(msg + " (" + FormatDoubleFix(rate, 1) + " kB/s)");
				}
			}
		}
		else
		{
			if(encrypt)
			{
				String msg = "Encrypting: " + fileinfo + file;
				pi.SetText(msg);
				AESEncoderStream aesEncoder(filesize, key); 
				int time = msecs();
				int64 size = 0;
				while(in.GetLeft()>0)
				{
					sIn = in.Get(in.GetBufferSize());

					aesEncoder << sIn;     //more data can be << too, before retrieving the data
					//sOut << aesEncoder;  //we dont accumulate data in a String,
					sOut = aesEncoder.GetEncryptedData(); //but get what is inside and put it in file, where it is accumulated

					out.Put(~sOut, sOut.GetCount());
					pi.Step(sIn.GetCount());

					int tt = msecs();
					int dt = tt - time;
					if(dt > 1000)
					{
						time = tt;
						int64 dsize = in.GetPos() - size;
						size = in.GetPos();
						double rate = (double)dsize/(double)dt;
						pi.SetText(msg + " (" + FormatDoubleFix(rate, 1) + " kB/s)");
					}
				}
			}
			else
			{
				String msg = "Decrypting: " + fileinfo + file;
				pi.SetText(msg);
				AESDecoderStream aesDecoder(key);
				int time = msecs();
				int64 size = 0;
				while(in.GetLeft()>0)
				{
					sIn = in.Get(in.GetBufferSize());

					aesDecoder << sIn;     //more data can be << too, before retrieving the data
					//sOut << aesDecoder;  //we dont accumulate data in a String,
					sOut = aesDecoder.GetDecryptedData(); //but get what is inside and put it in file, where it is accumulated
					
					out.Put(~sOut, sOut.GetCount());
					pi.Step(sIn.GetCount());

					int tt = msecs();
					int dt = tt - time;
					if(dt > 1000)
					{
						time = tt;
						int64 dsize = in.GetPos() - size;
						size = in.GetPos();
						double rate = (double)dsize/(double)dt;
						pi.SetText(msg + " (" + FormatDoubleFix(rate, 1) + " kB/s)");
					}
				}
			}
		}

		in.Close();
		out.Close();
		ToInfo(">> dest saved to " + dfilename);
	}

	pi.Close();
}

void DeEncrypter::Generate()
{
	fsk.Multi(false);
	fsk.Types("All Files\t*\nKey Files\t*.key\nDat Files\t*.dat\n");
	fsk.Set(DEFFILENAME);
	if(!fsk.ExecuteSaveAs("Save new key to"))
		return;
	String filename = fsk.Get();
	
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
	fsk.Multi(false);
	fsk.Types("All Files\t*\nKey Files\t*.key\nDat Files\t*.dat\n");
	fsk.Set("aes32");
	if(!fsk.ExecuteOpen("Open key from"))
		return;
	String filename = fsk.Get();
	
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

void DeEncrypter::Clear()
{
	info.Clear();	
}

GUI_APP_MAIN
{
	DeEncrypter().Run();
}

