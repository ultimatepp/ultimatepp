// ChineseCodeLib.h: interface for the CChineseCodeLib class.
//
//////////////////////////////////////////////////////////////////////
#include<string>
using namespace std;

/*
function：convert Chinese GB2312 between UTF-8
Author：litz
Email:mycro@163.com
reference：Mr.WuKang's Artical《UTF-8与GB2312之间的互换》
http://www.vckbase.com/document/viewdoc/?id=1397
*/


#if !defined(__CCHINESECODELIB_H_)
#define __CCHINESECODELIB_H_

class CChineseCodeLib
{
public:
	static void UTF_8ToGB2312(CString& pOut,char *pText, int pLen);
	static void GB2312ToUTF_8(CString& pOut,char *pText, int pLen);
	//  Unicode to UTF-8
	static void UnicodeToUTF_8(char* pOut,WCHAR* pText);
	// GB2312 to Unicode
	static void Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer);
	// Unicode  to GB2312
	static void UnicodeToGB2312(char* pOut,unsigned short uData);
	// UTF-8 to Unicode
	static void UTF_8ToUnicode(WCHAR* pOut,char* pText);

	CChineseCodeLib();
	virtual ~CChineseCodeLib();
};

#endif // !defined(__CCHINESECODELIB_H_)
