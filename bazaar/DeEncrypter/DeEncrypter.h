#ifndef _DeEncrypter_DeEncrypter_h
#define _DeEncrypter_DeEncrypter_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <DeEncrypter/DeEncrypter.lay>
#include <CtrlCore/lay.h>

#include <AESStream/AESStream.h>

#define DEFKEYSIZE 32
#define DEFFILENAME "aes" ASSTRING(DEFKEYSIZE) ".key"
#define BUFFSIZE (512*1024)

class DeEncrypter : public WithDeEncrypterLayout<TopWindow> {
public:
	typedef DeEncrypter CLASSNAME;
	DeEncrypter();
	~DeEncrypter();
	
	void ToInfo(const String & s)
	{
		info.Insert(info.GetLength(), s + "\n");
		info.SetCursor(info.GetLength());	
	}
	
	void Crypt(bool encrypt = true);
	void Generate();
	void OpenKey();
	void Clear();
	void Serialize(Stream& s);
	
	FileSel fsf, fsk;
	FileSel dirs;
	String key;
};

#endif

