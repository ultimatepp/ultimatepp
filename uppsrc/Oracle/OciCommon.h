#ifndef __Oracle_OciCommon__
#define __Oracle_OciCommon__

// excerpt from OCI headers (OCI7 & OCI8)
namespace Upp {
struct OCIEnv;
struct OCIServer;
struct OCISession;
struct OCIStmt;
struct OCIBind;
struct OCIDefine;
struct OCISnapshot;
struct OCIParam;

INITIALIZE(Oracle)
}

#define oci_boolean int
typedef void dvoid;
typedef int sword;
typedef int sb4;
typedef UPP::dword uword;
typedef UPP::dword ub4;
typedef short sb2;
typedef UPP::word ub2;
typedef signed char sb1;
typedef UPP::byte ub1;
typedef UPP::byte OraText;
enum
{
	// OCIInitialize modes
	OCI_THREADED   = 0x01,  // the application is in threaded environment
	OCI_OBJECT     = 0x02,  // the application is in object environment
	OCI_EVENTS     = 0x04,  // the application is enabled for events
	OCI_RESERVED1  = 0x08,  // Reserved for internal use
	OCI_SHARED     = 0x10,  // the application is in shared mode
	OCI_RESERVED2  = 0x20,  // Reserved for internal use
		// The following *TWO* are only valid for OCICreateEnvironment call
	OCI_NO_UCB     = 0x40,  // No user callback called during init
	OCI_NO_MUTEX   = 0x80,  // the environment handle will not be
		// NLS
	OCI_NLS_NCHARSET_ID_UT8 = 871,                   /* AL32UTF8 charset id */
	OCI_NLS_NCHARSET_ID_AL32UT8 = 873,               /* AL32UTF8 charset id */
		// protected by a mutex internally
	OCI_SHARED_EXT = 0x100, // Used for shared forms
	OCI_CACHE      = 0x200, // used by iCache

	OCI_DEFAULT      = 0x00,  // the default value for parameters and attributes
	OCI_DATA_AT_EXEC = 0x02,  // data at execute time

	OCI_HTYPE_FIRST                = 1, // start value of handle type
	OCI_HTYPE_ENV                  = 1, // environment handle
	OCI_HTYPE_ERROR                = 2, // error handle
	OCI_HTYPE_SVCCTX               = 3, // service handle
	OCI_HTYPE_STMT                 = 4, // statement handle
	OCI_HTYPE_BIND                 = 5, // bind handle
	OCI_HTYPE_DEFINE               = 6, // define handle
	OCI_HTYPE_DESCRIBE             = 7, // describe handle
	OCI_HTYPE_SERVER               = 8, // server handle
	OCI_HTYPE_SESSION              = 9, // authentication handle
	OCI_HTYPE_TRANS                = 10, // transaction handle
	OCI_HTYPE_COMPLEXOBJECT        = 11, // complex object retrieval handle
	OCI_HTYPE_SECURITY             = 12, // security handle
	OCI_HTYPE_SUBSCRIPTION         = 13, // subscription handle
	OCI_HTYPE_DIRPATH_CTX          = 14, // direct path context
	OCI_HTYPE_DIRPATH_COLUMN_ARRAY = 15, // direct path column array
	OCI_HTYPE_DIRPATH_STREAM       = 16, // direct path stream
	OCI_HTYPE_PROC                 = 17, // process handle
	OCI_HTYPE_LAST                 = 17, // last value of a handle type

	// input data types
	SQLT_CHR = 1, // (ORANET TYPE) character string
	SQLT_NUM = 2, // (ORANET TYPE) oracle numeric
	SQLT_INT = 3, // (ORANET TYPE) integer
	SQLT_FLT = 4, // (ORANET TYPE) Floating point number
	SQLT_STR = 5, // zero terminated string
	SQLT_VNU = 6, // NUM with preceding length byte
	SQLT_PDN = 7, // (ORANET TYPE) Packed Decimal Numeric
	SQLT_LNG = 8, // long
	SQLT_VCS = 9, // Variable character string
	SQLT_NON = 10, // Null/empty PCC Descriptor entry
	SQLT_RID = 11, // rowid
	SQLT_DAT = 12, // date in oracle format
	SQLT_VBI = 15, // binary in VCS format
	SQLT_BIN = 23, // binary data(DTYBIN)
	SQLT_LBI = 24, // long binary
	SQLT_UIN = 68, // unsigned integer
	SQLT_SLS = 91, // Display sign leading separate
	SQLT_LVC = 94, // Longer longs (char)
	SQLT_LVB = 95, // Longer long binary
	SQLT_AFC = 96, // Ansi fixed char
	SQLT_AVC = 97, // Ansi Var char
	SQLT_CUR = 102, // cursor type
	SQLT_RDD = 104, // rowid descriptor
	SQLT_LAB = 105, // label type
	SQLT_OSL = 106, // oslabel type

	SQLT_NTY = 108, // named object type
	SQLT_REF = 110, // ref type
	SQLT_CLOB = 112, // character lob
	SQLT_BLOB = 113, // binary lob
	SQLT_BFILEE = 114, // binary file lob
	SQLT_CFILEE = 115, // character file lob
	SQLT_RSET = 116, // result set type
	SQLT_NCO = 122, // named collection type (varray or nested table)
	SQLT_VST = 155, // OCIString type
	SQLT_ODT = 156, // OCIDate type

	// datetimes and intervals
	SQLT_DATE = 184, // ANSI Date
	SQLT_TIME = 185, // TIME
	SQLT_TIME_TZ = 186, // TIME WITH TIME ZONE
	SQLT_TIMESTAMP = 187, // TIMESTAMP
	SQLT_TIMESTAMP_TZ = 188, // TIMESTAMP WITH TIME ZONE
	SQLT_INTERVAL_YM = 189, // INTERVAL YEAR TO MONTH
	SQLT_INTERVAL_DS = 190, // INTERVAL DAY TO SECOND
	SQLT_TIMESTAMP_LTZ = 232, // TIMESTAMP WITH LOCAL TZ

	// cxcheng: this has been added for backward compatibility -
	// it needs to be here because ocidfn.h can get included ahead of sqldef.h
	SQLT_FILE = SQLT_BFILEE, // binary file lob
	SQLT_CFILE = SQLT_CFILEE,
	SQLT_BFILE = SQLT_BFILEE,

	OCI_DTYPE_FIRST = 50, // start value of descriptor type
	OCI_DTYPE_LOB = 50, // lob locator
	OCI_DTYPE_SNAP = 51, // snapshot descriptor
	OCI_DTYPE_RSET = 52, // result set descriptor
	OCI_DTYPE_PARAM = 53, // a parameter descriptor obtained from ocigparm
	OCI_DTYPE_ROWID = 54, // rowid descriptor
	OCI_DTYPE_COMPLEXOBJECTCOMP = 55,
	 // complex object retrieval descriptor
	OCI_DTYPE_FILE = 56, // File Lob locator
	OCI_DTYPE_AQENQ_OPTIONS = 57, // enqueue options
	OCI_DTYPE_AQDEQ_OPTIONS = 58, // dequeue options
	OCI_DTYPE_AQMSG_PROPERTIES = 59, // message properties
	OCI_DTYPE_AQAGENT = 60, // aq agent
	OCI_DTYPE_LOCATOR = 61, // LOB locator
	OCI_DTYPE_INTERVAL_YM = 62, // Interval year month
	OCI_DTYPE_INTERVAL_DS = 63, // Interval day second
	OCI_DTYPE_AQNFY_DESCRIPTOR = 64, // AQ notify descriptor
	OCI_DTYPE_DATE = 65, // Date
	OCI_DTYPE_TIME = 66, // Time
	OCI_DTYPE_TIME_TZ = 67, // Time with timezone
	OCI_DTYPE_TIMESTAMP = 68, // Timestamp
	OCI_DTYPE_TIMESTAMP_TZ = 69, // Timestamp with timezone
	OCI_DTYPE_TIMESTAMP_LTZ = 70, // Timestamp with local tz
	OCI_DTYPE_UCB = 71, // user callback descdriptor
	OCI_DTYPE_LAST = 71, // last value of a descriptor type

	OCI_V7_SYNTAX  = 2, // V815 language - for backwards compatibility
	OCI_V8_SYNTAX  = 3, // V815 language - for backwards compatibility
	OCI_NTV_SYNTAX = 1, // Use what so ever is the native lang of server

	OCI_STMT_SELECT  = 1, // select statement
	OCI_STMT_UPDATE  = 2, // update statement
	OCI_STMT_DELETE  = 3, // delete statement
	OCI_STMT_INSERT  = 4, // Insert Statement
	OCI_STMT_CREATE  = 5, // create statement
	OCI_STMT_DROP    = 6, // drop statement
	OCI_STMT_ALTER   = 7, // alter statement
	OCI_STMT_BEGIN   = 8, // begin ... (pl/sql statement)
	OCI_STMT_DECLARE = 9, // declare ... (pl/sql statement)

	OCI_BATCH_MODE             = 0x01, // batch the oci statement for execution
	OCI_EXACT_FETCH            = 0x02, // fetch the exact rows specified
	OCI_KEEP_FETCH_STATE       = 0x04, // unused
	OCI_SCROLLABLE_CURSOR      = 0x08, // cursor scrollable
	OCI_DESCRIBE_ONLY          = 0x10, // only describe the statement
	OCI_COMMIT_ON_SUCCESS      = 0x20, // commit, if successful execution
	OCI_NON_BLOCKING           = 0x40, // non-blocking
	OCI_BATCH_ERRORS           = 0x80, // batch errors in array dmls
	OCI_PARSE_ONLY             = 0x100, // only parse the statement
	OCI_EXACT_FETCH_RESERVED_1 = 0x200, // reserved for internal use
	OCI_SHOW_DML_WARNINGS      = 0x400,

	OCI_SUCCESS              = 0,      // maps to SQL_SUCCESS of SAG CLI
	OCI_SUCCESS_WITH_INFO    = 1,      // maps to SQL_SUCCESS_WITH_INFO
	OCI_RESERVED_FOR_INT_USE = 200,    // reserved for internal use
	OCI_NO_DATA              = 100,    // maps to SQL_NO_DATA
	OCI_ERROR                = -1,     // maps to SQL_ERROR
	OCI_INVALID_HANDLE       = -2,     // maps to SQL_INVALID_HANDLE
	OCI_NEED_DATA            = 99,     // maps to SQL_NEED_DATA
	OCI_STILL_EXECUTING      = -3123,  // OCI would block error
	OCI_CONTINUE             = -24200, // Continue with the body of the OCI function

	SQLCS_IMPLICIT = 1, // for CHAR, VARCHAR2, CLOB w/o a specified set
	SQLCS_NCHAR    = 2, // for NCHAR, NCHAR VARYING, NCLOB
	SQLCS_EXPLICIT = 3, // for CHAR, etc, with "CHARACTER SET ..." syntax
	SQLCS_FLEXIBLE = 4, // for PL/SQL "flexible" parameters
	SQLCS_LIT_NULL = 5, // for typecheck of NULL and empty_clob() lits

	OCI_FETCH_NEXT       = 0x02, // next row
	OCI_FETCH_FIRST      = 0x04, // first row of the result set
	OCI_FETCH_LAST       = 0x08, // the last row of the result set
	OCI_FETCH_PRIOR      = 0x10, // the previous row relative to current
	OCI_FETCH_ABSOLUTE   = 0x20, // absolute offset from first
	OCI_FETCH_RELATIVE   = 0x40, // offset relative to current
	OCI_FETCH_RESERVED_1 = 0x80, // reserved for internal use

	OCI_ONE_PIECE    = 0, // one piece
	OCI_FIRST_PIECE  = 1, // the first piece
	OCI_NEXT_PIECE   = 2, // the next of many pieces
	OCI_LAST_PIECE   = 3, // the last piece

	OCI_ATTR_DATA_SIZE     = 1, // maximum size of the data
	OCI_ATTR_DATA_TYPE     = 2, // the SQL type of the column/argument
	OCI_ATTR_NAME          = 4, // the name of the column/argument
	OCI_ATTR_PRECISION     = 5, // precision if number type
	OCI_ATTR_SCALE         = 6, // scale if number type
	OCI_ATTR_ROW_COUNT     = 9, // the rows processed so far
	OCI_ATTR_PREFETCH_ROWS = 11, // sets the number of rows to prefetch
	OCI_ATTR_STMT_TYPE     = 24, // statement type
	OCI_ATTR_PARAM_COUNT   = 18, // number of column in the select list

	OCI_ATTR_ENV           = 5,  // the environment handle
	OCI_ATTR_SERVER        = 6,  // the server handle
	OCI_ATTR_SESSION       = 7,  // the user session handle

	OCI_ATTR_USERNAME      = 22, // username attribute
	OCI_ATTR_PASSWORD      = 23, // password attribute

	OCI_CRED_RDBMS         = 1,  // database username/password

	OCI_LM_DEF = 0, // default login
	OCI_LM_NBL = 1, // non-blocking logon

	OCI_STRING_MAXLEN = 4000, // default maximum length of a vstring
};

#if defined(SS_64BIT_SERVER) || defined(__64BIT__)
#define CDA_SIZE 88
#else
# define CDA_SIZE 64
#endif

struct cda_head {
    sb2          v2_rc;
    ub2          ft;
    ub4          rpc;
    ub2          peo;
    ub1          fc;
    ub1          rcs1;
    ub2          rc;
    ub1          wrn;
    ub1          rcs2;
    sword        rcs3;
    struct {
        struct {
           ub4    rcs4;
           ub2    rcs5;
           ub1    rcs6;
        } rd;
        ub4    rcs7;
        ub2    rcs8;
    } rid;
    sword        ose;
    ub1		 chk;
    dvoid        *rcsp;
};

struct cda_def {
    sb2          v2_rc;                                    /* V2 return code */
    ub2          ft;                                    /* SQL function type */
    ub4          rpc;                                /* rows processed count */
    ub2          peo;                                  /* parse error offset */
    ub1          fc;                                    /* OCI function code */
    ub1          rcs1;                                        /* filler area */
    ub2          rc;                                       /* V7 return code */
    ub1          wrn;                                       /* warning flags */
    ub1          rcs2;                                           /* reserved */
    sword        rcs3;                                           /* reserved */
    struct {                                              /* rowid structure */
        struct {
           ub4    rcs4;
           ub2    rcs5;
           ub1    rcs6;
        } rd;
        ub4    rcs7;
        ub2    rcs8;
    } rid;
    sword        ose;                                 /* OSD dependent error */
    ub1		 chk;
    dvoid        *rcsp;                          /* pointer to reserved area */
    ub1          rcs9[CDA_SIZE - sizeof (struct cda_head)];        /* filler */
};

typedef sb4 (*OCICallbackInBind)(dvoid *ictxp, UPP::OCIBind *bindp, ub4 iter,
	ub4 index, dvoid **bufpp, ub4 *alenp,
	ub1 *piecep, dvoid **indp);

typedef sb4 (*OCICallbackOutBind)(dvoid *octxp, UPP::OCIBind *bindp, ub4 iter,
	ub4 index, dvoid **bufpp, ub4 **alenp,
	ub1 *piecep, dvoid **indp,
	ub2 **rcodep);

#endif//__Oracle_OciCommon__
