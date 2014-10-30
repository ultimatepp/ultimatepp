// vi: noexpandtab:tabstop=4
/*
	Author: Sergey Sikorskiy (www.sikorskiy.net)
	License: BSD
*/

#include "fb.h"

#ifdef PLATFORM_WIN32
	#define DLLFILENAME "fbclient.dll"
	#define DLLCALL     __stdcall
#else
	#define DLLFILENAME "fbclient.so"
#endif
#define DLIMODULE   FB
#define DLIHEADER   <Firebird/Firebird.dli>
#include <Core/dli_source.h>

// Development-time comments ...


// Conversions ...
// void        ISC_EXPORT isc_decode_date (ISC_QUAD ISC_FAR *,
//                     void ISC_FAR *);
//
// void        ISC_EXPORT isc_decode_sql_date (ISC_DATE ISC_FAR *,
//                     void ISC_FAR *);
//
// void        ISC_EXPORT isc_decode_sql_time (ISC_TIME ISC_FAR *,
//                     void ISC_FAR *);
//
// void        ISC_EXPORT isc_decode_timestamp (ISC_TIMESTAMP ISC_FAR *,
//                     void ISC_FAR *);
//
// void        ISC_EXPORT isc_encode_date (void ISC_FAR *,
//                     ISC_QUAD ISC_FAR *);
//
// void        ISC_EXPORT isc_encode_sql_date (void ISC_FAR *,
//                     ISC_DATE ISC_FAR *);
//
// void        ISC_EXPORT isc_encode_sql_time (void ISC_FAR *,
//                     ISC_TIME ISC_FAR *);
//
// void        ISC_EXPORT isc_encode_timestamp (void ISC_FAR *,
//                     ISC_TIMESTAMP ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_dsql_finish (isc_db_handle ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_dsql_insert (ISC_STATUS ISC_FAR *,
//                        isc_stmt_handle ISC_FAR *,
//                        unsigned short,
//                        XSQLDA ISC_FAR *);
//


// Print ...
// void        ISC_EXPORT isc_print_sqlerror (short,
//                        ISC_STATUS ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_print_status (ISC_STATUS ISC_FAR *);
//



// Not supported ...


// The isc_create_database ( ) method is not currently supported
// from user applications. is for internal use only.
// ISC_STATUS  ISC_EXPORT isc_create_database (ISC_STATUS ISC_FAR *,
//                         short,
//                         char ISC_FAR *,
//                         isc_db_handle ISC_FAR *,
//                         short,
//                         char ISC_FAR *,
//                         short);


// Not documented ...

// void        ISC_EXPORT isc_sql_interprete (short,
//                        char ISC_FAR *,
//                        short);
//
// ISC_LONG    ISC_EXPORT isc_vax_integer (char ISC_FAR *,
//                     short);
//
// ISC_INT64   ISC_EXPORT isc_portable_integer  (unsigned char ISC_FAR *,
//                                               short);
// ISC_STATUS  ISC_EXPORT isc_compile_request (ISC_STATUS ISC_FAR *,
//                         isc_db_handle ISC_FAR *,
//                         isc_req_handle ISC_FAR *,
//                         short,
//                         char ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_compile_request2 (ISC_STATUS ISC_FAR *,
//                          isc_db_handle ISC_FAR *,
//                          isc_req_handle ISC_FAR *,
//                          short,
//                          char ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_ddl (ISC_STATUS ISC_FAR *,
//                     isc_db_handle ISC_FAR *,
//                     isc_tr_handle ISC_FAR *,
//                     short,
//                     char ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_receive (ISC_STATUS ISC_FAR *,
//                     isc_req_handle ISC_FAR *,
//                     short,
//                     short,
//                     void ISC_FAR *,
//                     short);
//
// ISC_STATUS  ISC_EXPORT isc_release_request (ISC_STATUS ISC_FAR *,
//                         isc_req_handle ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_request_info (ISC_STATUS ISC_FAR *,
//                      isc_req_handle ISC_FAR *,
//                      short,
//                      short,
//                      char ISC_FAR *,
//                      short,
//                      char ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_send (ISC_STATUS ISC_FAR *,
//                  isc_req_handle ISC_FAR *,
//                  short,
//                  short,
//                  void ISC_FAR *,
//                  short);
//
// ISC_STATUS  ISC_EXPORT isc_start_and_send (ISC_STATUS ISC_FAR *,
//                        isc_req_handle ISC_FAR *,
//                        isc_tr_handle ISC_FAR *,
//                        short,
//                        short,
//                        void ISC_FAR *,
//                        short);
//
// ISC_STATUS  ISC_EXPORT isc_start_request (ISC_STATUS ISC_FAR *,
//                       isc_req_handle ISC_FAR *,
//                       isc_tr_handle ISC_FAR *,
//                       short);
//
// ISC_STATUS  ISC_EXPORT isc_unwind_request (ISC_STATUS ISC_FAR *,
//                        isc_tr_handle ISC_FAR *,
//                        short);
//
// ISC_STATUS  ISC_EXPORT isc_close (ISC_STATUS ISC_FAR *,
//                   char ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_declare (ISC_STATUS ISC_FAR *,
//                     char ISC_FAR *,
//                     char ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_describe (ISC_STATUS ISC_FAR *,
//                     char ISC_FAR *,
//                     XSQLDA ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_describe_bind (ISC_STATUS ISC_FAR *,
//                       char ISC_FAR *,
//                       XSQLDA ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_open (ISC_STATUS ISC_FAR *,
//                  isc_tr_handle ISC_FAR *,
//                  char ISC_FAR *,
//                  XSQLDA ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_dsql_release (ISC_STATUS ISC_FAR *,
//                      char ISC_FAR *);
//
// ISC_LONG    ISC_EXPORT isc_ftof (char ISC_FAR *,
//                  unsigned short,
//                  char ISC_FAR *,
//                  unsigned short);
//
// ISC_STATUS  ISC_EXPORT isc_print_blr (char ISC_FAR *,
//                       isc_callback,
//                       void ISC_FAR *,
//                       short);
//
// void        ISC_EXPORT isc_set_debug (int);
//
// void        ISC_EXPORT isc_qtoq (ISC_QUAD ISC_FAR *,
//                  ISC_QUAD ISC_FAR *);
//
// void        ISC_EXPORT isc_vtof (char ISC_FAR *,
//                  char ISC_FAR *,
//                  unsigned short);
//
// void        ISC_EXPORT isc_vtov (char ISC_FAR *,
//                  char ISC_FAR *,
//                  short);
//
// ISC_LONG    ISC_EXPORT isc_reset_fpe (unsigned short);








namespace ibpp
{
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	long
	Error::getSQLCode()
	{
		sqlCode_ = dll.isc_sqlcode(status_vector_);
		if (sqlCode_ == 999)
		{
			sqlCode_ = 0;
		}
		return sqlCode_;
	}

	String
	Error::getErrorText()
	{
		// isc_sql_interprete(sqlCode_, msg_, sizeof(msg_));
		// return msg_;

		String errStr;

#if 0
		ISC_STATUS* pVector = status_vector_;
		dll.isc_interprete(msg_, &pVector);
		errStr += msg_;
		while (dll.isc_interprete(msg_, &pVector))
		{
			errStr += "; ";
			errStr += msg_;
		}
#else
		const ISC_STATUS* pVector = status_vector_;
		dll.fb_interpret(msg_, sizeof(msg_), &pVector);
		errStr += msg_;
		while (dll.fb_interpret(msg_, sizeof(msg_), &pVector))
		{
			errStr += "; ";
			errStr += msg_;
		}
#endif

		return errStr;
	}

	void
	Error::check()
	{
		// static char already_exists[] = "already exists";
		// static char duplicate_value[] = "duplicate value";

		if (status_vector_[0] == 1 && status_vector_[1])
		{
			String err_str = getErrorText();

			// if (!info_str.empty())
			// {
			//     err_str += " ";
			//     err_str += info_str;
			// }

			long error_code = getSQLCode();

			throw DbExc(err_str, error_code);
		}
	}

	///////////////////////////////////////////////////////////////////////////
	dpb::dpb() :
			pos_(0)
	{
		memset(buff_, 0, sizeof(buff_));
	}

	void
	dpb::addParam(char value)
	{
		ASSERT(sizeof(pos_) < sizeof(buff_));
		buff_[pos_++] = value;
	}

	void
	dpb::addCluster(char type, const char* value)
	{
		short len = value ? (short)strlen(value) : 0;

		ASSERT(sizeof(pos_ + 2 + len - 1) < sizeof(buff_));
		buff_[pos_++] = type;
		buff_[pos_++] = char(len);
//         strncpy(&buff_[pos_], value, len);
		memcpy(&buff_[pos_], value, len);
		pos_ += len;
	}

	void
	dpb::addCluster(char type, short value)
	{
		ASSERT(sizeof(pos_ + 4 - 1) < sizeof(buff_));
		buff_[pos_++] = type;
		buff_[pos_++] = char(2);
		*(short*)&buff_[pos_] = value;
		pos_ += short(2);
	}

	void
	dpb::addCluster(char type, bool value)
	{
		ASSERT(sizeof(pos_ + 3 - 1) < sizeof(buff_));
		buff_[pos_++] = type;
		buff_[pos_++] = char(1);
		buff_[pos_++] = char(value ? 1 : 0);
	}

	void
	dpb::addCluster(char type, char value)
	{
		ASSERT(sizeof(pos_ + 3 - 1) < sizeof(buff_));
		buff_[pos_++] = type;
		buff_[pos_++] = char(1);
		buff_[pos_++] = value;
	}

	void
	dpb::addCluster(char type, unsigned long value)
	{
		ASSERT(pos_ + 1 + sizeof(value) - 1 < sizeof(buff_));
		buff_[pos_++] = type;
		*(unsigned long*)&buff_[pos_] = value;
		pos_ += short(sizeof(value));
	}

	///////////////////////////////////////////////////////////////////////////
	DBParamBuff::DBParamBuff()
	{
		addParam(isc_dpb_version1);
	}

	///////////////////////////////////////////////////////////////////////////
	DataBase::DataBase(T_FB& dll, Error& ib_error)
	: handle_(NULL)
	, dll(dll)
	, ib_error(ib_error)
	{
	}

	void
	DataBase::attach(const char* name)
	{
		handle_ = NULL;
		ib_error.check(dll.isc_attach_database(
				  ib_error.getErrorVector(),
				  strlen(name),
				  const_cast<char*>(name),
				  &handle_,
				  paramBuff_.getBuffSize(),
				  const_cast<char*>(paramBuff_.getBuff())
			  ));
	}

	void
	DataBase::dettach()
	{
		if (handle_ != 0)
		{
			ib_error.check(dll.isc_detach_database(
					  ib_error.getErrorVector(),
					  //const_cast<void**>(&handle_)
					  &handle_
				  ));
		}
	}

	void
	DataBase::drop()
	{
		ib_error.check(dll.isc_drop_database(
				  ib_error.getErrorVector(),
				  //const_cast<void**>(&handle_)
				  &handle_
			  ));
		handle_ = NULL;
	}

	void
	DataBase::execute_immediate(const String& stmt)
	{
		ASSERT(handle_ == 0);
		isc_tr_handle tr_handle = NULL;

		ib_error.check(dll.isc_dsql_execute_immediate(
				  ib_error.getErrorVector(),
				  &handle_,
				  &tr_handle,
				  stmt.GetCount(),
				  stmt,
				  3, // dialect_,
				  NULL // inDataArray_.getBuffer()
			  ));
			  
		ib_error.check(dll.isc_detach_database(
				  ib_error.getErrorVector(),
				  //static_cast<void**>(&handle_)
				  &handle_
			  ));
	}

	void
	DataBase::expand_dpb() const
	{
		// isc_expand_dpb() allocates a new block for the DPB. to avoid memory leaks, call
		// isc_free() to release that space.

		// void        ISC_EXPORT_VARARG isc_expand_dpb (char ISC_FAR * ISC_FAR *,
//                           short ISC_FAR *,
//                           ...);
//

// void        ISC_EXPORT isc_print_sqlerror (short,
	}

	void
	DataBase::modify_dpb() const
	{
// int        ISC_EXPORT isc_modify_dpb (char ISC_FAR * ISC_FAR *,
//                      short ISC_FAR *, unsigned short,
//                      char ISC_FAR *, short );
//
	}

	void
	DataBase::getInfo() const
	{
// ISC_STATUS  ISC_EXPORT isc_database_info (ISC_STATUS ISC_FAR *,
//                       isc_db_handle ISC_FAR *,
//                       short,
//                       char ISC_FAR *,
//                       short,
//                       char ISC_FAR *);
	}

	void version_callback(void* user_arg, char* data)
	{
		ASSERT(user_arg);
		if (user_arg)
		{
			DataBase* pDataBase = (DataBase*)user_arg;

			if (pDataBase->dbVersion_.IsEmpty())
			{
				pDataBase->dbVersion_ = data;
			}
			else
			{
				pDataBase->odsVersion_ = data;
			}
		}
	}

	void
	DataBase::retrieveVersion()
	{
		dbVersion_.Clear();
		odsVersion_.Clear();

#if FB_API_VER < 20
		typedef isc_callback cb_type;
#else
		typedef ISC_VERSION_CALLBACK cb_type;
#endif

		ib_error.check(dll.isc_version(
				  &handle_,
				  (cb_type)version_callback,
				  this
			  ));
	}

	///////////////////////////////////////////////////////////////////////////
	Transaction::Transaction(T_FB& dll, Error& ib_error)
	: handle_(0L)
	, dll(dll)
	, ib_error(ib_error)
	{
	}

	void
	Transaction::start(const DataBase& db)
	{
		handle_ = NULL;

		// Note If a TPB is not created for a transaction, a NULL pointer must be passed to
		// isc_start_transaction () in its place. A default set of attributes is automatically assigned to
		// such transactions.

		ib_error.check(dll.isc_start_transaction(
				  ib_error.getErrorVector(),
				  &handle_,
				  1, // Number of database handles passed in this call
				  db.getHandleP(),
				  paramBuff_.getBuffSize(),
				  const_cast<char*>(paramBuff_.getBuff())
			  ));
	}

	void
	Transaction::start_multiple()
	{
// ISC_STATUS  ISC_EXPORT isc_start_multiple (ISC_STATUS ISC_FAR *,
//                        isc_tr_handle ISC_FAR *,
//                        short,
//                        void ISC_FAR *);
//
	}

	void
	Transaction::commit()
	{
		ib_error.check(dll.isc_commit_transaction(
				  ib_error.getErrorVector(),
				  &handle_
			  ));
	}

	void
	Transaction::commit_retaining()
	{
		ib_error.check(dll.isc_commit_retaining(
				  ib_error.getErrorVector(),
				  &handle_
			  ));
	}

	void
	Transaction::rollback()
	{
		ib_error.check(dll.isc_rollback_transaction(
				  ib_error.getErrorVector(),
				  &handle_
			  ));
	}

	void
	Transaction::rollback_retaining()
	{
		// isc_rollback_retaining () should be used with caution because the error
		// that caused the rollback may be in the transactions context. In this case, until the context
		// is released the error will continue.
		ib_error.check(dll.isc_rollback_retaining(
				  ib_error.getErrorVector(),
				  &handle_
			  ));
	}

	void
	Transaction::prepare()
	{
		ib_error.check(dll.isc_prepare_transaction(
				  ib_error.getErrorVector(),
				  &handle_
			  ));
	}

	void
	Transaction::prepare2()
	{
// ISC_STATUS  ISC_EXPORT isc_prepare_transaction2 (ISC_STATUS ISC_FAR *,
//                          isc_tr_handle ISC_FAR *,
//                          short,
//                          char ISC_FAR *);
//
	}

	void
	Transaction::getInfo()
	{
// ISC_STATUS  ISC_EXPORT isc_transaction_info (ISC_STATUS ISC_FAR *,
//                          isc_tr_handle ISC_FAR *,
//                          short,
//                          char ISC_FAR *,
//                          short,
//                          char ISC_FAR *);
	}

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_transact_request (ISC_STATUS ISC_FAR *,
//                          isc_db_handle ISC_FAR *,
//                          isc_tr_handle ISC_FAR *,
//                          unsigned short,
//                          char ISC_FAR *,
//                          unsigned short,
//                          char ISC_FAR *,
//                          unsigned short,
//                          char ISC_FAR *);

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_reconnect_transaction (ISC_STATUS ISC_FAR *,
//                           isc_db_handle ISC_FAR *,
//                           isc_tr_handle ISC_FAR *,
//                           short,
//                           char ISC_FAR *);
//

	///////////////////////////////////////////////////////////////////////////
	SQLDataArray::SQLDataArray(size_t size) :
			pDA_(NULL),
			size_(0)
	{
		setSize(size);
	}

	SQLDataArray::SQLDataArray(const SQLDataArray& /*theArray*/)
	{
		ASSERT(false);
	}

	SQLDataArray::~SQLDataArray()
	{
		try
		{
			setSize(0);
		}
		catch (...)
		{
			ASSERT(false);
		}
	}

	void
	SQLDataArray::setSize(size_t size)
	{
		if (size != size_)
		{
			if (pDA_)
			{
				free(pDA_);
				pDA_ = NULL;
			}
			size_ = size;
			if (size_ != 0)
			{
				size_t len = XSQLDA_LENGTH(size_);
				pDA_ = (XSQLDA *)malloc(len);
				memset(pDA_, 0, len);
				// Initialize it ...
				pDA_->version = SQLDA_VERSION1;
				pDA_->sqln = size_;
				pDA_->sqld = size_; // actual number of fields ...
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////
	DynamicSQL::DynamicSQL(const DataBase& db, const Transaction& tr, T_FB& dll, Error& ib_error)
	: dialect_(3)
	, pDBHandle_(db.getHandleP())
	, pTRHandle_(tr.getHandleP())
	, dll(dll)
	, ib_error(ib_error)
	, handle_(NULL)
	, all_data_fetched_(false)
	{
		allocate_stmt2();
	}
	
	DynamicSQL::~DynamicSQL()
	{
		try
		{
			drop_statement();
		}
		catch (...)
		{
			ASSERT(false);
		}
	}

	void
	DynamicSQL::execute_immediate(const String& stmt)
	{
		ib_error.check(dll.isc_dsql_execute_immediate(
				  ib_error.getErrorVector(),
				  pDBHandle_,
				  pTRHandle_,
				  stmt.GetCount(),
				  stmt,
				  dialect_,
				  inDataArray_.getBuffer()
			  ));
		// There is no data to fetch ...
		// all_data_fetched_ = false;
	}

	void
	DynamicSQL::execute_immediate_data(const String& stmt)
	{
		ib_error.check(dll.isc_dsql_exec_immed2(
				  ib_error.getErrorVector(),
				  pDBHandle_,
				  pTRHandle_,
				  stmt.GetCount(),
				  stmt,
				  dialect_,
				  inDataArray_.getBuffer(),
				  outDataArray_.getBuffer()
			  ));
		all_data_fetched_ = false;
	}

	void
	DynamicSQL::execute_immediate_no_trans(const String& stmt)
	{
		ASSERT(*pDBHandle_ == 0);
		ASSERT(*pTRHandle_ == 0);

		ib_error.check(dll.isc_dsql_execute_immediate(
				  ib_error.getErrorVector(),
				  pDBHandle_,
				  pTRHandle_,
				  stmt.GetCount(),
				  stmt,
				  dialect_,
				  inDataArray_.getBuffer()
			  ));
		// There is no data to fetch ...
		// all_data_fetched_ = false;
	}

	void
	DynamicSQL::prepare(const String& stmt, const SQLDataArray& define_da)
	{
		ib_error.check(dll.isc_dsql_prepare(
				  ib_error.getErrorVector(),
				  pTRHandle_,
				  &handle_,
				  stmt.GetCount(),
				  stmt,
				  dialect_,
				  define_da.getBuffer()
			  ));
	}

	void
	DynamicSQL::execute()
	{
		ib_error.check(dll.isc_dsql_execute(
				  ib_error.getErrorVector(),
				  pTRHandle_,
				  &handle_,
				  // Indicates the version of the extended SQL descriptor area (XSQLDA)
				  // ???? ??????? ???? 3-?, ?? SQL_DIALECT_V6, ???? ?????? - SQL_DIALECT_V5
				  SQL_DIALECT_V6,
				  inDataArray_.getBuffer()
			  ));
		all_data_fetched_ = false;
	}

	void
	DynamicSQL::execute2()
	{
		ib_error.check(dll.isc_dsql_execute2(
				  ib_error.getErrorVector(),
				  pTRHandle_,
				  &handle_,
				  // Indicates the version of the extended SQL descriptor area (XSQLDA)
				  // ???? ??????? ???? 3-?, ?? SQL_DIALECT_V6, ???? ?????? - SQL_DIALECT_V5
				  SQL_DIALECT_V6,
				  inDataArray_.getBuffer(),
				  outDataArray_.getBuffer()
			  ));
		// all_data_fetched_ = false;
	}

	void
	DynamicSQL::describe_bind(const SQLDataArray& da)
	{
		ib_error.check(dll.isc_dsql_describe_bind(
				  ib_error.getErrorVector(),
				  &handle_,
				  // Indicates the version of the extended SQL descriptor area (XSQLDA)
				  // ???? ??????? ???? 3-?, ?? SQL_DIALECT_V6, ???? ?????? - SQL_DIALECT_V5
				  SQL_DIALECT_V6,
				  da.getBuffer()
			  ));
	}

	void
	DynamicSQL::describe_define(const SQLDataArray& da)
	{
		ib_error.check(dll.isc_dsql_describe(
				  ib_error.getErrorVector(),
				  &handle_,
				  // Indicates the version of the extended SQL descriptor area (XSQLDA)
				  // ???? ??????? ???? 3-?, ?? SQL_DIALECT_V6, ???? ?????? - SQL_DIALECT_V5
				  SQL_DIALECT_V6,
				  da.getBuffer()
			  ));
	}

	void
	DynamicSQL::setCursorName(const char* name)
	{
		ib_error.check(dll.isc_dsql_set_cursor_name(
				  ib_error.getErrorVector(),
				  &handle_,
				  const_cast<char*>(name),
				  0 // Reserved for future use; set to 0
			  ));
	}

	bool
	DynamicSQL::fetch()
	{
		if (!all_data_fetched_)
		{
			int fetch_stat = dll.isc_dsql_fetch(
								 ib_error.getErrorVector(),
								 &handle_,
								 // Indicates the version of the extended SQL descriptor area (XSQLDA)
								 // ???? ??????? ???? 3-?, ?? SQL_DIALECT_V6, ???? ?????? - SQL_DIALECT_V5
								 SQL_DIALECT_V6,
								 outDataArray_.getBuffer()
							 );

			switch (fetch_stat)
			{
			case 0:
				return true;
			case 100:
				// No more data.
				all_data_fetched_ = true;
				return false;
			default:
				ib_error.check();
				break;
			}
		}

		return false;
	}

	int
	DynamicSQL::getStmtType()
	{
		// Get Statement type in our case ...
		char type_item[] = {isc_info_sql_stmt_type};
		char res_buffer[8];
		int  st_type = 0;

		ib_error.check(dll.isc_dsql_sql_info(
				  ib_error.getErrorVector(),
				  &handle_,
				  sizeof(type_item),
				  type_item,
				  sizeof(res_buffer),
				  res_buffer
			  ));

		if (res_buffer[0] == isc_info_sql_stmt_type)
		{
			short length;
			length = static_cast<short>(dll.isc_portable_integer(reinterpret_cast<unsigned char*>(res_buffer + 1), 2));
			st_type = static_cast<int>(dll.isc_portable_integer(reinterpret_cast<unsigned char*>(res_buffer + 3), length));
		}
		
		return st_type;
	}

	void
	DynamicSQL::allocate_stmt()
	{
		// Note The isc_dsql_allocate_statement2() function is similar to the
		// isc_dsql_alloc_statement() function except that statement handles allocated using
		// isc_dsql_allocate_statement2() are automatically reset to
		// NULL when the database under which they are allocated is detached.

		if (handle_ == 0)
		{
			ib_error.check(dll.isc_dsql_allocate_statement(
					  ib_error.getErrorVector(),
					  pDBHandle_,
					  &handle_
				  ));
		}
		else
		{
			ASSERT(handle_ == 0);
		}
	}

	void
	DynamicSQL::allocate_stmt2()
	{
		// Note The isc_dsql_allocate_statement2() function is similar to the
		// isc_dsql_alloc_statement() function except that statement handles allocated using
		// isc_dsql_allocate_statement2() are automatically reset to
		// NULL when the database under which they are allocated is detached.

		if (handle_ == 0)
		{
			ib_error.check(dll.isc_dsql_alloc_statement2(
					  ib_error.getErrorVector(),
					  pDBHandle_,
					  &handle_
				  ));
		}
		else
		{
			ASSERT(handle_ == 0);
		}
	}

	void
	DynamicSQL::close_cursor()
	{
		ib_error.check(dll.isc_dsql_free_statement(
				  ib_error.getErrorVector(),
				  &handle_,
				  DSQL_close
			  ));
		// !!! Do not set handle_ = NULL here !!!;
	}

	void
	DynamicSQL::drop_statement()
	{
		ib_error.check(dll.isc_dsql_free_statement(
				  ib_error.getErrorVector(),
				  &handle_,
				  DSQL_drop
			  ));
		handle_ = NULL;
	}

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_execute (ISC_STATUS ISC_FAR *,
//                     isc_tr_handle ISC_FAR *,
//                     char ISC_FAR *,
//                     XSQLDA ISC_FAR *);
//

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_execute_immediate (ISC_STATUS ISC_FAR *,
//                           isc_db_handle ISC_FAR *,
//                           isc_tr_handle ISC_FAR *,
//                           short ISC_FAR *,
//                           char ISC_FAR *);
//

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_fetch (ISC_STATUS ISC_FAR *,
//                   char ISC_FAR *,
//                   XSQLDA ISC_FAR *);
//

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_prepare (ISC_STATUS ISC_FAR *,
//                     isc_db_handle ISC_FAR *,
//                     isc_tr_handle ISC_FAR *,
//                     char ISC_FAR *,
//                     short ISC_FAR *,
//                     char ISC_FAR *,
//                     XSQLDA ISC_FAR *);

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_dsql_execute_m (ISC_STATUS ISC_FAR *,
//                        isc_tr_handle ISC_FAR *,
//                        isc_stmt_handle ISC_FAR *,
//                        unsigned short,
//                        char ISC_FAR *,
//                        unsigned short,
//                        unsigned short,
//                        char ISC_FAR *);
//

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_dsql_execute2_m (ISC_STATUS ISC_FAR *,
//                        isc_tr_handle ISC_FAR *,
//                        isc_stmt_handle ISC_FAR *,
//                        unsigned short,
//                        char ISC_FAR *,
//                        unsigned short,
//                        unsigned short,
//                        char ISC_FAR *,
//                        unsigned short,
//                        char ISC_FAR *,
//                        unsigned short,
//                        unsigned short,
//                        char ISC_FAR *);


// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_dsql_execute_immediate_m (ISC_STATUS ISC_FAR *,
//                              isc_db_handle ISC_FAR *,
//                              isc_tr_handle ISC_FAR *,
//                              unsigned short,
//                              char ISC_FAR *,
//                              unsigned short,
//                              unsigned short,
//                              char ISC_FAR *,
//                              unsigned short,
//                              unsigned short,
//                              char ISC_FAR *);

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_dsql_exec_immed3_m (ISC_STATUS ISC_FAR *,
//                            isc_db_handle ISC_FAR *,
//                            isc_tr_handle ISC_FAR *,
//                            unsigned short,
//                            char ISC_FAR *,
//                            unsigned short,
//                            unsigned short,
//                            char ISC_FAR *,
//                            unsigned short,
//                            unsigned short,
//                            char ISC_FAR *,
//                            unsigned short,
//                            char ISC_FAR *,
//                            unsigned short,
//                            unsigned short,
//                            char ISC_FAR *);

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_dsql_fetch_m (ISC_STATUS ISC_FAR *,
//                      isc_stmt_handle ISC_FAR *,
//                      unsigned short,
//                      char ISC_FAR *,
//                      unsigned short,
//                      unsigned short,
//                      char ISC_FAR *);

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_dsql_insert_m (ISC_STATUS ISC_FAR *,
//                       isc_stmt_handle ISC_FAR *,
//                       unsigned short,
//                       char ISC_FAR *,
//                       unsigned short,
//                       unsigned short,
//                       char ISC_FAR *);

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_dsql_prepare_m (ISC_STATUS ISC_FAR *,
//                        isc_tr_handle ISC_FAR *,
//                        isc_stmt_handle ISC_FAR *,
//                        unsigned short,
//                        char ISC_FAR *,
//                        unsigned short,
//                        unsigned short,
//                        char ISC_FAR *,
//                        unsigned short,
//                        char ISC_FAR *);
//

	///////////////////////////////////////////////////////////////////////////
	EmbededSQL::EmbededSQL()
	{
	}

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_embed_dsql_close (ISC_STATUS ISC_FAR *,
//                          char ISC_FAR *);

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_embed_dsql_declare (ISC_STATUS ISC_FAR *,
//                           char ISC_FAR *,
//                           char ISC_FAR *);

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_embed_dsql_describe (ISC_STATUS ISC_FAR *,
//                         char ISC_FAR *,
//                         unsigned short,
//                         XSQLDA ISC_FAR *);

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_embed_dsql_describe_bind (ISC_STATUS ISC_FAR *,
//                              char ISC_FAR *,
//                              unsigned short,
//                              XSQLDA ISC_FAR *);

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_embed_dsql_execute (ISC_STATUS ISC_FAR *,
//                            isc_tr_handle ISC_FAR *,
//                            char ISC_FAR *,
//                            unsigned short,
//                            XSQLDA ISC_FAR *);

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_embed_dsql_execute2 (ISC_STATUS ISC_FAR *,
//                         isc_tr_handle ISC_FAR *,
//                         char ISC_FAR *,
//                         unsigned short,
//                         XSQLDA ISC_FAR *,
//                         XSQLDA ISC_FAR *);

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_embed_dsql_execute_immed (ISC_STATUS ISC_FAR *,
//                              isc_db_handle ISC_FAR *,
//                              isc_tr_handle ISC_FAR *,
//                              unsigned short,
//                              char ISC_FAR *,
//                              unsigned short,
//                              XSQLDA ISC_FAR *);

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_embed_dsql_fetch (ISC_STATUS ISC_FAR *,
//                          char ISC_FAR *,
//                          unsigned short,
//                          XSQLDA ISC_FAR *);

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_embed_dsql_open (ISC_STATUS ISC_FAR *,
//                         isc_tr_handle ISC_FAR *,
//                         char ISC_FAR *,
//                         unsigned short,
//                         XSQLDA ISC_FAR *);

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_embed_dsql_open2 (ISC_STATUS ISC_FAR *,
//                          isc_tr_handle ISC_FAR *,
//                          char ISC_FAR *,
//                          unsigned short,
//                          XSQLDA ISC_FAR *,
//                          XSQLDA ISC_FAR *);

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_embed_dsql_insert (ISC_STATUS ISC_FAR *,
//                           char ISC_FAR *,
//                           unsigned short,
//                           XSQLDA ISC_FAR *);

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_embed_dsql_prepare (ISC_STATUS ISC_FAR *,
//                            isc_db_handle ISC_FAR *,
//                            isc_tr_handle ISC_FAR *,
//                            char ISC_FAR *,
//                            unsigned short,
//                            char ISC_FAR *,
//                            unsigned short,
//                            XSQLDA ISC_FAR *);

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_embed_dsql_release (ISC_STATUS ISC_FAR *,
//                            char ISC_FAR *);

	///////////////////////////////////////////////////////////////////////////
	void
	BlobDescr::get(
		SegmentBlob& blob,
		const String& tbl_name,
		const String& clm_name
	)
	{
		char clm_global_name[256];

		ib_error.check(dll.isc_blob_lookup_desc(
				  ib_error.getErrorVector(),
				  blob.pDBHandle_,
				  blob.pTRHandle_,
				  (unsigned char ISC_FAR *)~tbl_name,
				  (unsigned char ISC_FAR *)~clm_name,
				  &blobDescr_,
				  (unsigned char ISC_FAR *)clm_global_name
			  ));
	}

	void
	BlobDescr::getDefault()
	{
		// void       ISC_EXPORT isc_blob_default_desc (ISC_BLOB_DESC ISC_FAR *,
		//                                         unsigned char ISC_FAR *,
		//                                         unsigned char ISC_FAR *);
		//
	}

	void
	BlobDescr::set()
	{
		// ISC_STATUS ISC_EXPORT isc_blob_set_desc (ISC_STATUS ISC_FAR *,
		//                      unsigned char ISC_FAR *,
		//                      unsigned char ISC_FAR *,
		//                      short,
		//                      short,
		//                      short,
		//                      ISC_BLOB_DESC ISC_FAR *);
		//
	}

	// ISC_STATUS ISC_EXPORT isc_blob_gen_bpb (ISC_STATUS ISC_FAR *,
	//                     ISC_BLOB_DESC ISC_FAR *,
	//                     ISC_BLOB_DESC ISC_FAR *,
	//                     unsigned short,
	//                     unsigned char ISC_FAR *,
	//                     unsigned short ISC_FAR *);
	//

	///////////////////////////////////////////////////////////////////////////
	SegmentBlob::info::info() :
			lenght_(0),
			maxSegments_(0),
			numSegments_(0),
			type_segmented(false)
	{
	}

	SegmentBlob::SegmentBlob(const DataBase& db, const Transaction& tr, T_FB& dll, Error& ib_error) :
		pDBHandle_(db.getHandleP()),
		pTRHandle_(tr.getHandleP()),
		dll(dll),
		ib_error(ib_error),
		isOpen_(false),
		hasInfo_(false)
	{
		handle_ = NULL;

		id_.gds_quad_low = 0;
		id_.gds_quad_high = 0;

		lastID_.gds_quad_low = 0;
		lastID_.gds_quad_high = 0;
	}

	SegmentBlob::~SegmentBlob()
	{
		// This is not a right place to close BLOB ...
		// close();
	}

	void 
	SegmentBlob::open_internal()
	{
		handle_ = NULL;

		ib_error.check(dll.isc_open_blob(
				  ib_error.getErrorVector(),
				  pDBHandle_,
				  pTRHandle_,
				  &handle_,   /* set by this function to refer to the SegmentBlob */
				  &id_       /* SegmentBlob ID put into out_sqlda by isc_dsql_fetch() */
			  ));
	}

	void
	SegmentBlob::open2_internal()
	{
		handle_ = NULL;

		ib_error.check(dll.isc_open_blob2(
				  ib_error.getErrorVector(),
				  pDBHandle_,
				  pTRHandle_,
				  &handle_,   /* set by this function to refer to the SegmentBlob */
				  &id_,       /* SegmentBlob ID put into out_sqlda by isc_dsql_fetch() */
				  0,          /* BPB length = 0; no filter will be used */
				  NULL        /* NULL BPB, since no filter will be used */
			  ));
	}

	void
	SegmentBlob::setup_open()
	{
		isOpen_ = true;
		lastID_ = id_;
		hasInfo_ = false;
	}

	void
	SegmentBlob::setup_close()
	{
		isOpen_ = false;
		lastID_.gds_quad_low = 0;
		lastID_.gds_quad_high = 0;
		hasInfo_ = false;
	}

	void
	SegmentBlob::open()
	{
		if (!isOpen_)
		{
			open2_internal();
			setup_open();
		}
		else if ((lastID_.gds_quad_high != id_.gds_quad_high) || (lastID_.gds_quad_low != id_.gds_quad_low))
		{
			close();
			open2_internal();
			setup_open();
		}
	}

	void
	SegmentBlob::create_internal()
	{
		handle_ = NULL;
		id_.gds_quad_low = 0;
		id_.gds_quad_high = 0;

		ib_error.check(dll.isc_create_blob(
				  ib_error.getErrorVector(),
				  pDBHandle_,
				  pTRHandle_,
				  &handle_,   /* set by this function to refer to the SegmentBlob */
				  &id_        /* SegmentBlob ID put into out_sqlda by isc_dsql_fetch() */
			  ));
	}

	void
	SegmentBlob::create2_internal()
	{
		handle_ = NULL;
		id_.gds_quad_low = 0;
		id_.gds_quad_high = 0;

		ib_error.check(dll.isc_create_blob2(
				  ib_error.getErrorVector(),
				  pDBHandle_,
				  pTRHandle_,
				  &handle_,   /* set by this function to refer to the SegmentBlob */
				  &id_,       /* SegmentBlob ID put into out_sqlda by isc_dsql_fetch() */
				  0,          /* BPB length = 0; no filter will be used */
				  NULL        /* NULL BPB, since no filter will be used */
			  ));
	}

	void
	SegmentBlob::create()
	{
		close();
		create2_internal();
		setup_open();
	}

	void
	SegmentBlob::close()
	{
		if (isOpen_)
		{
			ASSERT(handle_);
			ib_error.check(dll.isc_close_blob(
					  ib_error.getErrorVector(),
					  &handle_
				  ));
			handle_ = NULL;
			setup_close();
		}
	}

	bool
	SegmentBlob::getSegment(
		unsigned short& actualSegLen,
		unsigned short maxSegLen,
		char* pSegment
	)
	{
		bool result = false;

		if (!isOpen_)
			open();
		
		ASSERT(handle_);
		stat_ = dll.isc_get_segment(
					ib_error.getErrorVector(),
					&handle_,
					&actualSegLen,
					maxSegLen,
					pSegment
				);
		if (stat_ == 0 || (stat_ != 0 && ib_error.get_err_code() == isc_segment))
			result = true;
		else if (stat_ == isc_segstr_eof)
			result = false;
		else
			// Exception.
			ib_error.check();

		return result;
	}

	void
	// SegmentBlob::addSegment(const String& data)
	SegmentBlob::addSegment(const char* pSegment, unsigned short seg_length)
	{
		if (!isOpen_)
			open();

		ASSERT(handle_);
		ib_error.check(dll.isc_put_segment(
				  ib_error.getErrorVector(),
				  &handle_,
				  seg_length,
				  const_cast<char*>(pSegment)
			  ));
	}

	void
	SegmentBlob::put_segment(const char* data_ptr, size_t data_len)
	{
		long cur_str_pos = 0;
		unsigned short cur_segment_size = 0;
		long size_left = data_len;
		const unsigned short db_segment_size = 0xFFFF;

		// !!! Segment size is limited to "unsigned short" ...
		while (size_left > 0)
		{
			cur_segment_size = (size_left > db_segment_size ? db_segment_size : static_cast<unsigned short>(size_left));

			addSegment(data_ptr + cur_str_pos, cur_segment_size);
			
			size_left -= cur_segment_size;
			cur_str_pos += cur_segment_size;
		}
	}

	void
	SegmentBlob::cancel()
	{
		ASSERT(handle_);
		ib_error.check(dll.isc_cancel_blob(
				  ib_error.getErrorVector(),
				  &handle_
			  ));
		setup_close();
	}

	void
	SegmentBlob::retrieve_info()
	{
		if (!hasInfo_)
		{
			char res_buffer[40];
			char blob_items[] =
			{
				isc_info_blob_total_length,
				isc_info_blob_max_segment,
				isc_info_blob_num_segments,
				isc_info_blob_type
			};

			if (!isOpen_)
				open();

			ASSERT(handle_);
			ib_error.check(dll.isc_blob_info(
					  ib_error.getErrorVector(),
					  &handle_,
					  sizeof(blob_items),/* Length of item-list buffer. */
					  blob_items, /* Item-list buffer. */
					  sizeof(res_buffer),/* Length of result buffer. */
					  res_buffer /* Result buffer */
				  ));

			// Extract values returned in the result buffer
			for (char* p = res_buffer; *p != isc_info_end ;)
			{

				char item = *p++;

				short length = (short)dll.isc_vax_integer(p, 2);

				p += 2;

				switch (item)
				{
				case isc_info_blob_total_length:
					info_.lenght_ = dll.isc_vax_integer(p, length);
					break;
				case isc_info_blob_max_segment:
					info_.maxSegments_ = dll.isc_vax_integer(p, length);
					break;
				case isc_info_blob_num_segments:
					info_.numSegments_ = dll.isc_vax_integer(p, length);
					break;
				case isc_info_blob_type:
					info_.type_segmented = (dll.isc_vax_integer(p, length) == 0);
					break;
				case isc_info_truncated:
					// handle error
					throw DbExc("BLOB information was truncated.");
					break;
				case isc_info_error:
					// handle error
					throw DbExc("Requested BLOB information is unavailable.");
					break;
				default:
					break;
				}

				p += length;
			}
			
			hasInfo_ = true;
		}
	}

// Stream Blobs ...
// http://www.ibphoenix.com/a4556.htm

// Not documented ...
// ISC_STATUS  ISC_EXPORT isc_seek_blob (ISC_STATUS ISC_FAR *,
//                       isc_blob_handle ISC_FAR *,
//                       short,
//                       ISC_LONG,
//                       ISC_LONG ISC_FAR *);
//




// Not documented ...
// BSTREAM     ISC_FAR * ISC_EXPORT BLOB_open (isc_blob_handle,
//                         char ISC_FAR *,
//                         int);
//

// Not documented ...
// int         ISC_EXPORT BLOB_put (char,
//                  BSTREAM ISC_FAR *);
//

// Not documented ...
// int         ISC_EXPORT BLOB_close (BSTREAM ISC_FAR *);
//

// Not documented ...
// int         ISC_EXPORT BLOB_get (BSTREAM ISC_FAR *);
//

// Not documented ...
// int         ISC_EXPORT BLOB_display (ISC_QUAD ISC_FAR *,
//                      isc_db_handle,
//                      isc_tr_handle,
//                      char ISC_FAR *);
//

// Not documented ...
// int         ISC_EXPORT BLOB_dump (ISC_QUAD ISC_FAR *,
//                   isc_db_handle,
//                   isc_tr_handle,
//                   char ISC_FAR *);
//

// Not documented ...
// int         ISC_EXPORT BLOB_edit (ISC_QUAD ISC_FAR *,
//                   isc_db_handle,
//                   isc_tr_handle,
//                   char ISC_FAR *);
//

// Not documented ...
// int         ISC_EXPORT BLOB_load (ISC_QUAD ISC_FAR *,
//                   isc_db_handle,
//                   isc_tr_handle,
//                   char ISC_FAR *);
//

// Not documented ...
// int         ISC_EXPORT BLOB_text_dump (ISC_QUAD ISC_FAR *,
//                   isc_db_handle,
//                   isc_tr_handle,
//                   char ISC_FAR *);
//

// Not documented ...
// int         ISC_EXPORT BLOB_text_load (ISC_QUAD ISC_FAR *,
//                   isc_db_handle,
//                   isc_tr_handle,
//                   char ISC_FAR *);
//

// Not documented ...
// BSTREAM     ISC_FAR * ISC_EXPORT Bopen (ISC_QUAD ISC_FAR *,
//                         isc_db_handle,
//                         isc_tr_handle,
//                         char ISC_FAR *);
//

// Not documented ...
// BSTREAM     ISC_FAR * ISC_EXPORT Bopen2 (ISC_QUAD ISC_FAR *,
//                      isc_db_handle,
//                      isc_tr_handle,
//                      char ISC_FAR *,
//                      unsigned short);



	///////////////////////////////////////////////////////////////////////////
	spb::spb(size_t buff_size) :
			buff_size_(buff_size),
			pos_(0)
	{
		buff_ptr_ = new char[buff_size_];
		memset(buff_ptr_, 0, buff_size_);
	}

	spb::~spb()
	{
		delete [] buff_ptr_;
	}

	void
	spb::addParam(char value)
	{
		ASSERT(pos_ < buff_size_);
		buff_ptr_[pos_++] = value;
	}

	void
	spb::addCluster(char type, const char* value)
	{
		unsigned short len = (unsigned short)strlen(value);

		ASSERT(static_cast<size_t>(pos_ + 1 + 2 + len - 1) < buff_size_);
		buff_ptr_[pos_++] = type;
		char* p = &buff_ptr_[pos_];
		ADD_SPB_LENGTH(p, len); // It is okay to have a warning "conversion from 'unsigned short' to 'char'" here.
		pos_ += sizeof(len);
		memcpy(&buff_ptr_[pos_], value, len);
		pos_ += len;
	}

	void
	spb::addCluster(char type, unsigned long value)
	{
		ASSERT(pos_ + 1 + sizeof(value) - 1 < buff_size_);
		buff_ptr_[pos_++] = type;
		// *(short*)&buff_ptr_[pos_] = value;
		char* p = &buff_ptr_[pos_];
		ADD_SPB_NUMERIC(p, value);
		pos_ += short(sizeof(value));
	}

	///////////////////////////////////////////////////////////////////////////
	void
	Service::attach(
		const char* host,
		const char* user,
		const char* pswd,
		network_protocol_t protocol
	)
	{
		String service_host;

		service_host = host;
		if (!service_host.IsEmpty())
		{
			switch (protocol)
			{
			case np_TCP_IP:
				service_host += ":service_mgr";
				break;
			case np_NetBEUI:
				service_host = "\\\\" + service_host;
				service_host += "\\service_mgr";
				break;
			case np_IPX_SPX:
				service_host += "@service_mgr";
				break;
			case np_local:
				service_host = "service_mgr";
				break;
			}
		}
		else
		{
			service_host = "service_mgr";
		}

		dpb paramBuff;
		paramBuff.addParam(isc_spb_version);
		paramBuff.addParam(isc_spb_current_version);
		paramBuff.addCluster(isc_spb_user_name, user);
		paramBuff.addCluster(isc_spb_password, pswd);

		handle_ = 0L;
		ib_error.check(dll.isc_service_attach(
				  ib_error.getErrorVector(),
				  service_host.GetCount(),
				  service_host,
				  &handle_,
				  paramBuff.getBuffSize(),
				  const_cast<char*>(paramBuff.getBuff())
			  ));
	}

	void
	Service::detach()
	{
		ib_error.check(dll.isc_service_detach(
				  ib_error.getErrorVector(),
				  &handle_
			  ));
	}

	void
	Service::executeCommand(const spb& pb)
	{
		ib_error.check(dll.isc_service_start(
				  ib_error.getErrorVector(),
				  &handle_,
				  NULL,
				  pb.getBuffSize(),
				  const_cast<char*>(pb.getBuff())
			  ));
	}

	void
	Service::query(const spb& request, const spb& result)
	{
		spb flags_buff;

		flags_buff.addParam(isc_spb_version);
		flags_buff.addParam(isc_spb_current_version);
		flags_buff.addCluster(isc_info_svc_timeout, 60);

		ib_error.check(dll.isc_service_query(
				  ib_error.getErrorVector(),
				  &handle_,
				  NULL,
				  flags_buff.getBuffSize(), flags_buff.getBuff(),
				  request.getBuffSize(), request.getBuff(),
				  result.getBuffMaxSize(), result.getBuff()
			  ));

//         char spb_buffer[6], *spb = spb_buffer;
//         char request_buffer[] = {
//             isc_info_svc_version,
//             isc_info_svc_server_version,
//             isc_info_svc_implementation,
//             isc_info_svc_get_env,
//             isc_info_svc_get_env_lock,
//             isc_info_svc_get_env_msg,
//             isc_info_svc_get_licensed_users,
//             isc_info_svc_user_dbpath
//             };
//         char result_buffer[1024], *p = result_buffer;
//         memset(result_buffer, 0, sizeof(result_buffer));
//
//         // *spb++ = isc_info_svc_timeout;
//         // ADD_SPB_NUMERIC(spb, 60); /* 1 minute timeout */
//
//         if (isc_service_query (
//             ib_error.getErrorVector(),
//             &handle_,
//             NULL,
//             spb - spb_buffer, spb_buffer,
//             // sizeof(request_buffer), request_buffer,
//             request.getBuffSize(), request.getBuff(),
//             // sizeof(result_buffer), result_buffer
//             result.getBuffMaxSize(), result.getBuff()
//             )
//         )
//         {
//             isc_print_status(ib_error.getErrorVector());
//             isc_service_detach(ib_error.getErrorVector(), &handle_);
//             return;
//         }
	}

	///////////////////////////////////////////////////////////////////////////
	user_info::user_info() :
			user_id(0),
			group_id(0)
	{
	}

	user_info::user_info(const user_info& info)
	{
		copy(info);
	}

	void
	user_info::copy(const user_info& info)
	{
		user_id = info.user_id;
		group_id = info.group_id;
		user_name = info.user_name;
		first_name = info.first_name;
		middle_name = info.middle_name;
		last_name = info.last_name;
	}

	///////////////////////////////////////////////////////////////////////////
	server_info::server_info() :
			svc_version(0),
			num_licensed_users(0)
	{
	}

	server_info::server_info(const server_info& info)
	{
		copy(info);
	}

	void
	server_info::copy(const server_info& info)
	{
		svc_version = info.svc_version;
		num_licensed_users = info.num_licensed_users;
		server_version = info.server_version;
		implementation_str = info.implementation_str;
		env_ib_str = info.env_ib_str;
		env_lock_str = info.env_lock_str;
		env_msg_str = info.env_msg_str;
		user_db_path_str = info.user_db_path_str;
	}

	///////////////////////////////////////////////////////////////////////////
	namespace svc_cmd
	{
		///////////////////////////////////////////////////////////////////////////
		base::base(Service& svc) :
				svc_ptr_(&svc)
		{
		}

		unsigned long
		base::read_long(char*& p) const
		{
			unsigned long result;

			// p += sizeof(unsigned short);
			result = (unsigned long)svc_ptr_->get_dll().isc_portable_integer((unsigned char*)p, sizeof(result));
			p += sizeof(unsigned long);
			return result;
		}

		unsigned short
		base::read_short(char*& p) const
		{
			unsigned short result;

			// p += sizeof(unsigned short);
			result = (unsigned short)svc_ptr_->get_dll().isc_portable_integer((unsigned char*)p, sizeof(result));
			p += sizeof(unsigned short);
			return result;
		}

		String
		base::read_string(char*& p) const
		{
			String result;
			size_t str_length;

			str_length = (unsigned short)svc_ptr_->get_dll().isc_portable_integer((unsigned char*)p, sizeof(unsigned short));
			p += sizeof(unsigned short);
			result = String(p, str_length);
			p += str_length;
			return result;
		}

		///////////////////////////////////////////////////////////////////////////
		backup::backup(Service& svc) :
				base(svc)
		{
		}

		///////////////////////////////////////////////////////////////////////////
		restore::restore(Service& svc) :
				base(svc)
		{
		}

		///////////////////////////////////////////////////////////////////////////
		set_properties::set_properties(Service& svc) :
				base(svc)
		{
		}

		///////////////////////////////////////////////////////////////////////////
		repair::repair(Service& svc) :
				base(svc)
		{
		}

		///////////////////////////////////////////////////////////////////////////
		get_db_stats::get_db_stats(Service& svc) :
				base(svc)
		{
		}

		///////////////////////////////////////////////////////////////////////////
		get_server_info::get_server_info(Service& svc) :
				base(svc)
		{
		}

		void
		get_server_info::execute()
		{
			spb request;
			spb result(1024);

			// Prepare request ...
			request.addParam(isc_info_svc_version);
			request.addParam(isc_info_svc_server_version);
			request.addParam(isc_info_svc_implementation);
			request.addParam(isc_info_svc_get_env);
			request.addParam(isc_info_svc_get_env_lock);
			request.addParam(isc_info_svc_get_env_msg);
			request.addParam(isc_info_svc_get_licensed_users);
			request.addParam(isc_info_svc_user_dbpath);

			// Execute ...
			svc_ptr_->query(request, result);

			// Parse results ...
			char* p = const_cast<char*>(result.getBuff());

			do
			{
				switch (*p++)
				{
				case isc_info_truncated:
					throw DbExc("Server information was truncated.");
					break;
				case isc_info_svc_version:
					info_.svc_version = read_long(p);
					break;
				case isc_info_svc_get_licensed_users:
					info_.num_licensed_users = read_long(p);
					break;
				case isc_info_svc_server_version:
					info_.server_version = read_string(p);
					break;
				case isc_info_svc_implementation:
					info_.implementation_str = read_string(p);
					break;
				case isc_info_svc_get_env:
					info_.env_ib_str = read_string(p);
					break;
				case isc_info_svc_get_env_lock:
					info_.env_lock_str = read_string(p);
					break;
				case isc_info_svc_get_env_msg:
					info_.env_msg_str = read_string(p);
					break;
				case isc_info_svc_user_dbpath:
					info_.user_db_path_str = read_string(p);
					break;
				}
			}
			while (*p);
		}

		///////////////////////////////////////////////////////////////////////////
		get_log::get_log(Service& svc) :
				base(svc)
		{
		}

		///////////////////////////////////////////////////////////////////////////
		get_user_info::get_user_info(Service& svc, const String& user_name) :
				base(svc),
				user_name_(user_name)
		{
			paramBuff_.addParam(isc_action_svc_display_user);
			paramBuff_.addCluster(isc_spb_sec_username, user_name);
		}

		void
		get_user_info::execute()
		{
			spb request;
			spb result(1024);

			unsigned short loop;

			// Executea task ...
			svc_ptr_->executeCommand(paramBuff_);

			// Prepare request ...
			request.addParam(isc_info_svc_get_users);

			// Execute ...
			svc_ptr_->query(request, result);

			// Parse results ...
			char* p = const_cast<char*>(result.getBuff());

			do
			{
				switch (*p++)
				{
				case isc_info_truncated:
					throw DbExc("User information was truncated.");
					break;
				case isc_info_svc_get_users:
					loop = read_short(p);
					while (*p != isc_info_end)
					{
						switch (*p++)
						{
						case isc_spb_sec_username:
							info_.user_name = read_string(p);
							loop -= (info_.user_name.GetCount() + sizeof(unsigned short) + 1);
							break;
						case isc_spb_sec_firstname:
							info_.first_name = read_string(p);
							loop -= (info_.first_name.GetCount() + sizeof(unsigned short) + 1);
							break;
						case isc_spb_sec_middlename:
							info_.middle_name = read_string(p);
							loop -= (info_.middle_name.GetCount() + sizeof(unsigned short) + 1);
							break;
						case isc_spb_sec_lastname:
							info_.last_name = read_string(p);
							loop -= (info_.last_name.GetCount() + sizeof(unsigned short) + 1);
							break;
						case isc_spb_sec_groupid:
							info_.group_id = read_long(p);
							loop -= (sizeof(unsigned long) + 1);
							break;
						case isc_spb_sec_userid:
							info_.user_id = read_long(p);
							loop -= (sizeof(unsigned long) + 1);
							break;
						}
					}
					break;
				}
			}
			while (*p);
		}

		///////////////////////////////////////////////////////////////////////////
		get_users::get_users(Service& svc) :
				base(svc)
		{
			paramBuff_.addParam(isc_action_svc_display_user);
		}

		void
		get_users::execute()
		{
			spb request;
			spb result(1024);

			unsigned short loop;

			// Executea task ...
			svc_ptr_->executeCommand(paramBuff_);

			// Prepare request ...
			request.addParam(isc_info_svc_get_users);

			// Execute ...
			svc_ptr_->query(request, result);

			// Parse results ...
			char* p = const_cast<char*>(result.getBuff());
			user_info info;

			do
			{
				switch (*p++)
				{
				case isc_info_truncated:
					throw DbExc("User list information was truncated.");
					break;
				case isc_info_svc_get_users:
					loop = read_short(p);
					while (*p != isc_info_end)
					{
						switch (*p++)
						{
						case isc_spb_sec_username:
							if (!info.user_name.IsEmpty())
							{
								user_info_list_.push_back(info);
								info = user_info();
							}
							info.user_name = read_string(p);
							loop -= (info.user_name.GetCount() + sizeof(unsigned short) + 1);
							break;
						case isc_spb_sec_firstname:
							info.first_name = read_string(p);
							loop -= (info.first_name.GetCount() + sizeof(unsigned short) + 1);
							break;
						case isc_spb_sec_middlename:
							info.middle_name = read_string(p);
							loop -= (info.middle_name.GetCount() + sizeof(unsigned short) + 1);
							break;
						case isc_spb_sec_lastname:
							info.last_name = read_string(p);
							loop -= (info.last_name.GetCount() + sizeof(unsigned short) + 1);
							break;
						case isc_spb_sec_groupid:
							info.group_id = read_long(p);
							loop -= (sizeof(unsigned long) + 1);
							break;
						case isc_spb_sec_userid:
							info.user_id = read_long(p);
							loop -= (sizeof(unsigned long) + 1);
							break;
						}
					}
					break;
				}
			}
			while (*p);

			if (!info.user_name.IsEmpty())
			{
				user_info_list_.push_back(info);
				info = user_info();
			}
		}

		///////////////////////////////////////////////////////////////////////////
		add_user::add_user(Service& svc) :
				base(svc)
		{
			paramBuff_.addParam(isc_action_svc_add_user);
		}

		void
		add_user::execute()
		{
			svc_ptr_->executeCommand(paramBuff_);
		}

		void
		add_user::set_username(const char* name)
		{
			paramBuff_.addCluster(isc_spb_sec_username, name);
		}

		void
		add_user::set_password(const char* pswd)
		{
			paramBuff_.addCluster(isc_spb_sec_password, pswd);
		}

		void
		add_user::set_firstname(const char* value)
		{
			paramBuff_.addCluster(isc_spb_sec_firstname, value);
		}

		void
		add_user::set_middlename(const char* value)
		{
			paramBuff_.addCluster(isc_spb_sec_middlename, value);
		}

		void
		add_user::set_lastname(const char* value)
		{
			paramBuff_.addCluster(isc_spb_sec_lastname, value);
		}

		void
		add_user::set_userid(unsigned long value)
		{
			paramBuff_.addCluster(isc_spb_sec_userid, value);
		}

		void
		add_user::set_groupid(unsigned long value)
		{
			paramBuff_.addCluster(isc_spb_sec_groupid, value);
		}

		void
		add_user::set_groupname(const char* value)
		{
			paramBuff_.addCluster(isc_spb_sec_groupname, value);
		}

		void
		add_user::set_rolename(const char* value)
		{
			paramBuff_.addCluster(isc_spb_sql_role_name, value);
		}

		///////////////////////////////////////////////////////////////////////////
		delete_user::delete_user(Service& svc) :
				base(svc)
		{
			paramBuff_.addParam(isc_action_svc_delete_user);
		}

		void
		delete_user::execute()
		{
			svc_ptr_->executeCommand(paramBuff_);
		}

		void
		delete_user::set_username(const char* name)
		{
			paramBuff_.addCluster(isc_spb_sec_username, name);
		}

		void
		delete_user::set_rolename(const char* value)
		{
			paramBuff_.addCluster(isc_spb_sql_role_name, value);
		}

		///////////////////////////////////////////////////////////////////////////
		update_user::update_user(Service& svc) :
				base(svc)
		{
			paramBuff_.addParam(isc_action_svc_modify_user);
		}

		void
		update_user::execute()
		{
			svc_ptr_->executeCommand(paramBuff_);
		}

		void
		update_user::set_username(const char* name)
		{
			paramBuff_.addCluster(isc_spb_sec_username, name);
		}

		void
		update_user::set_password(const char* pswd)
		{
			paramBuff_.addCluster(isc_spb_sec_password, pswd);
		}

		void
		update_user::set_firstname(const char* value)
		{
			paramBuff_.addCluster(isc_spb_sec_firstname, value);
		}

		void
		update_user::set_middlename(const char* value)
		{
			paramBuff_.addCluster(isc_spb_sec_middlename, value);
		}

		void
		update_user::set_lastname(const char* value)
		{
			paramBuff_.addCluster(isc_spb_sec_lastname, value);
		}

		void
		update_user::set_userid(unsigned long value)
		{
			paramBuff_.addCluster(isc_spb_sec_userid, value);
		}

		void
		update_user::set_groupid(unsigned long value)
		{
			paramBuff_.addCluster(isc_spb_sec_groupid, value);
		}

		void
		update_user::set_groupname(const char* value)
		{
			paramBuff_.addCluster(isc_spb_sec_groupname, value);
		}

		void
		update_user::set_rolename(const char* value)
		{
			paramBuff_.addCluster(isc_spb_sql_role_name, value);
		}

		///////////////////////////////////////////////////////////////////////////
		add_license::add_license(Service& svc) :
				base(svc)
		{
		}

		///////////////////////////////////////////////////////////////////////////
		remove_license::remove_license(Service& svc) :
				base(svc)
		{
		}

	}

	///////////////////////////////////////////////////////////////////////////
	Array::Array(const DataBase& db, const Transaction& tr) 
	: pDBHandle_(db.getHandleP())
	, pTRHandle_(tr.getHandleP())
	{
		id_.gds_quad_low = 0;
		id_.gds_quad_high = 0;
	}
	
// ISC_STATUS  ISC_EXPORT isc_array_gen_sdl (ISC_STATUS ISC_FAR *,
//                       ISC_ARRAY_DESC ISC_FAR *,
//                       short ISC_FAR *,
//                       char ISC_FAR *,
//                       short ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_array_get_slice (ISC_STATUS ISC_FAR *,
//                         isc_db_handle ISC_FAR *,
//                         isc_tr_handle ISC_FAR *,
//                         ISC_QUAD ISC_FAR *,
//                         ISC_ARRAY_DESC ISC_FAR *,
//                         void ISC_FAR *,
//                         ISC_LONG ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_array_lookup_bounds (ISC_STATUS ISC_FAR *,
//                         isc_db_handle ISC_FAR *,
//                         isc_tr_handle ISC_FAR *,
//                         char ISC_FAR *,
//                         char ISC_FAR *,
//                         ISC_ARRAY_DESC ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_array_lookup_desc (ISC_STATUS ISC_FAR *,
//                           isc_db_handle ISC_FAR *,
//                           isc_tr_handle ISC_FAR *,
//                           char ISC_FAR *,
//                           char ISC_FAR *,
//                           ISC_ARRAY_DESC ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_array_set_desc (ISC_STATUS ISC_FAR *,
//                        char ISC_FAR *,
//                        char ISC_FAR *,
//                        short ISC_FAR *,
//                        short ISC_FAR *,
//                        short ISC_FAR *,
//                        ISC_ARRAY_DESC ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_array_put_slice (ISC_STATUS ISC_FAR *,
//                         isc_db_handle ISC_FAR *,
//                         isc_tr_handle ISC_FAR *,
//                         ISC_QUAD ISC_FAR *,
//                         ISC_ARRAY_DESC ISC_FAR *,
//                         void ISC_FAR *,
//                         ISC_LONG ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_get_slice (ISC_STATUS ISC_FAR *,
//                       isc_db_handle ISC_FAR *,
//                       isc_tr_handle ISC_FAR *,
//                       ISC_QUAD ISC_FAR *,
//                       short,
//                       char ISC_FAR *,
//                       short,
//                       ISC_LONG ISC_FAR *,
//                       ISC_LONG,
//                       void ISC_FAR *,
//                       ISC_LONG ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_put_slice (ISC_STATUS ISC_FAR *,
//                       isc_db_handle ISC_FAR *,
//                       isc_tr_handle ISC_FAR *,
//                       ISC_QUAD ISC_FAR *,
//                       short,
//                       char ISC_FAR *,
//                       short,
//                       ISC_LONG ISC_FAR *,
//                       ISC_LONG,
//                       void ISC_FAR *);
//

	///////////////////////////////////////////////////////////////////////////
	Event::Event()
	{
	}

// ISC_STATUS  ISC_EXPORT isc_cancel_events (ISC_STATUS ISC_FAR *,
//                       isc_db_handle ISC_FAR *,
//                       ISC_LONG ISC_FAR *);
//
// ISC_LONG    ISC_EXPORT_VARARG isc_event_block (char ISC_FAR * ISC_FAR *,
//                            char ISC_FAR * ISC_FAR *,
//                            unsigned short, ...);
//
// void        ISC_EXPORT isc_event_counts (unsigned ISC_LONG ISC_FAR *,
//                      short,
//                      char ISC_FAR *,
//                      char ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_que_events (ISC_STATUS ISC_FAR *,
//                        isc_db_handle ISC_FAR *,
//                        ISC_LONG ISC_FAR *,
//                        short,
//                        char ISC_FAR *,
//                        isc_callback,
//                        void ISC_FAR *);
//
// ISC_STATUS  ISC_EXPORT isc_wait_for_event (ISC_STATUS ISC_FAR *,
//                        isc_db_handle ISC_FAR *,
//                        short,
//                        char ISC_FAR *,
//                        char ISC_FAR *);

}
