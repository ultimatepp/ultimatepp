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
		String fileinfo;
		fileinfo << "(" << (i+1) << "/" << fs.GetCount() <<") ";
		String filename = fs.GetFile(i);
		String file = GetFileName(filename);
		String dfilename = dir + DIR_SEP + file + "$";
		
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
		
		pi.Set(0, filesize);

		if(0)
		{
			String msg = "Copying: " + fileinfo + file;
			pi.SetText(msg);
			int time = msecs();
			int size = 0;
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
					int dsize = in.GetPos() - size;
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
				int size = 0;
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
						int dsize = in.GetPos() - size;
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
				int size = 0;
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
						int dsize = in.GetPos() - size;
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

