#ifndef _Unzip_Unzip_h_
#define _Unzip_Unzip_h_

#include <src/support/support.h>

#ifndef Far
#  define Far far
#endif

#ifdef WIN32
#  define far
#  define _far
#  define __far
#  define near
#  define _near
#  define __near
#  ifndef FAR
#    define FAR
#  endif
#endif

#define IDM_REPLACE_NO     100
#define IDM_REPLACE_TEXT   101
#define IDM_REPLACE_YES    102
#define IDM_REPLACE_ALL    103
#define IDM_REPLACE_NONE   104
#define IDM_REPLACE_RENAME 105
#define IDM_REPLACE_HELP   106

/* external return codes */
#define PK_OK              0   /* no error */
#define PK_COOL            0   /* no error */
#define PK_WARN            1   /* warning error */
#define PK_ERR             2   /* error in zipfile */
#define PK_BADERR          3   /* severe error in zipfile */
#define PK_MEM             4   /* insufficient memory (during initialization) */
#define PK_MEM2            5   /* insufficient memory (password failure) */
#define PK_MEM3            6   /* insufficient memory (file decompression) */
#define PK_MEM4            7   /* insufficient memory (memory decompression) */
#define PK_MEM5            8   /* insufficient memory (not yet used) */
#define PK_NOZIP           9   /* zipfile not found */
#define PK_PARAM          10   /* bad or illegal parameters specified */
#define PK_FIND           11   /* no files found */
#define PK_DISK           50   /* disk full */
#define PK_EOF            51   /* unexpected EOF */

#define IZ_CTRLC          80   /* user hit ^C to terminate */
#define IZ_UNSUP          81   /* no files found: all unsup. compr/encrypt. */
#define IZ_BADPWD         82   /* no files found: all had bad password */

/* return codes of password fetches (negative = user abort; positive = error) */
#define IZ_PW_ENTERED      0   /* got some password string; use/try it */
#define IZ_PW_CANCEL      -1   /* no password available (for this entry) */
#define IZ_PW_CANCELALL   -2   /* no password, skip any further pwd. request */
#define IZ_PW_ERROR        5   /* = PK_MEM2 : failure (no mem, no tty, ...) */

/* flag values for status callback function */
#define UZ_ST_START_EXTRACT     1       /* no details */
#define UZ_ST_IN_PROGRESS       2       /* no details */
#define UZ_ST_FINISH_MEMBER     3       /* 'details': extracted size */

/* return values of status callback function */
#define UZ_ST_CONTINUE          0
#define UZ_ST_BREAK             1


#ifdef __cplusplus
extern "C" {
#endif

#ifndef DEFINED_ONCE
#define DEFINED_ONCE
typedef int (WINAPI DLLPRNT) (LPSTR, unsigned long);
typedef int (WINAPI DLLPASSWORD) (LPSTR, int, LPCSTR, LPCSTR);
typedef int (WINAPI DLLSERVICE) (LPCSTR, unsigned long);
#endif
typedef void (WINAPI DLLSND) (void);
typedef int (WINAPI DLLREPLACE)(LPSTR);
typedef void (WINAPI DLLMESSAGE)(unsigned long, unsigned long, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, char, LPSTR, LPSTR, unsigned long, char);

typedef struct {
	DLLPRNT *print;
	DLLSND *sound;
	DLLREPLACE *replace;
	DLLPASSWORD *password;
	DLLMESSAGE *SendApplicationMessage;
	DLLSERVICE *ServCallBk;
	unsigned long TotalSizeComp;
	unsigned long TotalSize;
	unsigned long CompFactor;
	unsigned long NumMembers;
	WORD cchComment;
} USERFUNCTIONS, far * LPUSERFUNCTIONS;

typedef struct {
	int ExtractOnlyNewer;
	int SpaceToUnderscore;
	int PromptToOverwrite;
	int fQuiet;
	int ncflag;
	int ntflag;
	int nvflag;
	int nfflag;
	int nzflag;
	int ndflag;
	int noflag;
	int naflag;
	int nZIflag;
	int C_flag;
	int fPrivilege;
	LPSTR lpszZipFN;
	LPSTR lpszExtractDir;
} DCL, far * LPDCL;

#ifdef __cplusplus
}
#endif

typedef int (WINAPI * UNZIP)(int, char **, int, char **, LPDCL, LPUSERFUNCTIONS);

unsigned long doneSize;
unsigned long TotalSize;
bool ProgressEnabled;
Progress UnzipPi;

void WINAPI ReceiveDllMessage(unsigned long ucsize, unsigned long csiz, unsigned cfactor, unsigned mo, unsigned dy, unsigned yr, unsigned hh, unsigned mm, char c, LPSTR filename, LPSTR methbuf, unsigned long crc, char fCrypt);
int WINAPI password(LPSTR p, int n, LPCSTR m, LPCSTR name);
int WINAPI DisplayBuf(LPSTR buf, unsigned long size);
int WINAPI DisplayProgress(LPCSTR buf, unsigned long size);
int WINAPI GetReplaceDlgRetVal(LPSTR filename);
int Extract(const String& ZipFile, const String& OutDir = Null, bool WithProgress = false);

#endif
