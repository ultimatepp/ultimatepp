// vi: noexpandtab:tabstop=4
#ifndef _firebird_fb_h_
#define _firebird_fb_h_

/*
	Author: Sergey Sikorskiy (www.sikorskiy.net)
	License: BSD
*/
 
#include <Sql/Sql.h>
#include "lib/ibase.h"

#ifdef PLATFORM_WIN32
	#define DLLFILENAME "fbclient.dll"
	#define DLLCALL     __stdcall
#else
	#define DLLFILENAME "fbclient.so"
#endif
#define DLIMODULE   FB
#define DLIHEADER   <Firebird/Firebird.dli>
#include <Core/dli_header.h> 

namespace ibpp
{
	using namespace Upp;
	
	class DbExc : public SqlExc
	{
	public:
	#ifndef NOAPPSQL
		DbExc();
	#endif
		DbExc(const SqlSession& session) : SqlExc(session), err_num(0) {}
		DbExc(const Sql& sql) : SqlExc(sql), err_num(0) {}
		DbExc(const String& desc, int en = 0) : SqlExc(desc), err_num(en) {}
		DbExc(const char *desc, int en = 0) : SqlExc(desc), err_num(en) {}
		
	public:
		int GetErrNum() const { return err_num; }
		
	private:
		int err_num;
	};
	
	///////////////////////////////////////////////////////////////////////////
	// To do: parse the status vector by myself.
	class Error
	{
	public:
		Error(T_FB& dll) : dll(dll) {}
		
		ISC_STATUS* getErrorVector()
		{
			return status_vector_;
		}

	public:
		long getSQLCode();
		String getErrorText();
		void check();

		long get_err_code() const
		{
			return status_vector_[1];
		}
		
		void check(ISC_STATUS rc)
		{
			if (rc)
				check();
		}

	protected:
		T_FB&       dll;
	
	private:
		long        sqlCode_;
		ISC_STATUS  status_vector_[20];
		char        msg_[1024];
	};

	class dpb
	{
	public:
		dpb();

	public:
		const char* getBuff() const
		{
			return buff_;
		}
		short getBuffSize() const
		{
			return pos_;
		}

	public:
		void addParam(char value);
		void addCluster(char type, const char* value);
		void addCluster(char type, short value);
		void addCluster(char type, bool value);
		void addCluster(char type, char value);
		void addCluster(char type, unsigned long value);

	private:
		char    buff_[1024];
		short   pos_;
	};

	// ServiceParamBuff (SPB) differs in some ways from the DatabaseParamBuff (DPB) ...
	class spb
	{
	public:
		spb(size_t buff_size = 128);
		~spb();

	public:
		char* getBuff() const
		{
			return buff_ptr_;
		}
		short getBuffSize() const
		{
			return pos_;
		}
		short getBuffMaxSize() const
		{
			return buff_size_;
		}

	public:
		void addParam(char value);
		void addCluster(char type, const char* value);
		void addCluster(char type, unsigned long value);

	public:
		unsigned long get_long()
		{
			return 0;
		}

	private:
		char*           buff_ptr_;
		const size_t    buff_size_;
		unsigned short  pos_;
	};

	class DBParamBuff : public dpb
	{
	public:
		DBParamBuff();
	};

	class DataBase
	{
	public:
		DataBase(T_FB& dll, Error& ib_error);

	public:
		isc_db_handle* getHandleP() const
		{
			return &handle_;
		}
		isc_db_handle& getHandleRef()
		{
			return handle_;
		}

	public:
		void attach(const char* name);
		void dettach();

		void drop();
		
		// This method will not create an attached handle. Use the "attach" method instead of this one.
		void execute_immediate(const String& stmt);
		
		void expand_dpb() const;
		void modify_dpb() const;
		void getInfo() const;

		void retrieveVersion();

	public:
		// User validation

		// String user name, up to 255 characters
		void setUserName(const char* value)
		{
			paramBuff_.addCluster(isc_dpb_user_name, value);
		}
		// String password, up to 255 characters
		void setPassword(const char* value)
		{
			paramBuff_.addCluster(isc_dpb_password, value);
		}
		// String encrypted password, up to 255 characters
		void setEncPassword(const char* value)
		{
			paramBuff_.addCluster(isc_dpb_password_enc, value);
		}
		void setRoleName(const char* value)
		{
			paramBuff_.addCluster(isc_dpb_sql_role_name, value);
		}
		// String system DBA name, up to 255 characters
		void setSysUserName(const char* value)
		{
			paramBuff_.addCluster(isc_dpb_sys_user_name, value);
		}
		void setLicense(const char* value)
		{
			paramBuff_.addCluster(isc_dpb_license, value);
		}
		void setEncryptionKey(const char* value)
		{
			paramBuff_.addCluster(isc_dpb_encrypt_key, value);
		}

	public:
		// Environmental control

		// Number of database cache buffers to allocate for use with the database;
		void setNumBuffers(short value = 2048)
		{
			paramBuff_.addCluster(isc_dpb_num_buffers, value);
		}
		// Scope of dbkey context. 0 limits scope to the current transaction,
		// 1 extends scope to the database session
		void setDBKeyScope(char value)
		{
			paramBuff_.addCluster(isc_dpb_dbkey_scope, value);
		}
		// 1 - V5.x & V6 compatible; 2 - diagnostic; 3 - V6 only
		void setSQLDialect(char value)
		{
			paramBuff_.addCluster(isc_dpb_sql_dialect, value);
		}
		// 1 - V5.x & V6 compatible; 2 - diagnostic; 3 - V6 only
		void setDBSQLDialect(char value)
		{
			paramBuff_.addCluster(isc_dpb_set_db_sql_dialect, value);
		}

	public:
		// System management

		// Specifies whether database writes are synchronous or asynchronous.
		void setSynchWrites(bool value)
		{
			paramBuff_.addCluster(isc_dpb_force_write, value);
		}
		// Specify whether the database should reserves pace on each page for back versions of records when modifications are made
		void setReserveSpace(bool value = false)
		{
			paramBuff_.addCluster(isc_dpb_no_reserve, !value);
		}

	public:
		// System management
		void setAsDamaged(bool value)
		{
			paramBuff_.addCluster(isc_dpb_damaged, value);
		}
		void setVerify(bool value)
		{
			paramBuff_.addCluster(isc_dpb_verify, value);
		}

	public:
		// Shadow control ( !!! Ignored by DB )
		void setActivateShadow(bool value)
		{
			paramBuff_.addCluster(isc_dpb_activate_shadow, value);
		}
		void setDeleteShadow(bool value)
		{
			paramBuff_.addCluster(isc_dpb_delete_shadow, value);
		}

	public:
		// Replay logging system control
		void setStartLog(bool value)
		{
			paramBuff_.addCluster(isc_dpb_begin_log, value);
		}
		void setEndLog(bool value)
		{
			paramBuff_.addCluster(isc_dpb_quit_log, value);
		}

	public:
		// Character set and message file specification

		// Language-specific message file
		void setMessageFile(const char* value)
		{
			paramBuff_.addCluster(isc_dpb_lc_messages, value);
		}
		// Character set to be utilized
		void setCharSet(const char* value)
		{
			paramBuff_.addCluster(isc_dpb_lc_ctype, value);
		}

	private:
		mutable isc_db_handle	handle_;
		DBParamBuff     		paramBuff_;
		T_FB& dll;
		Error& ib_error;

	public:
		// Because I cannot make a friend function ...
		String     dbVersion_;
		String     odsVersion_;
	};

	class Transaction
	{
	public:
		Transaction(T_FB& dll, Error& ib_error);

	public:
		isc_tr_handle* getHandleP() const
		{
			return &handle_;
		}
		isc_tr_handle& getHandleRef()
		{
			return handle_;
		}

	public:
		// Starts a new transaction against one or more databases.; use a
		// previously declared and populated TPB
		void start(const DataBase& db);
		
		void start_multiple();
		// Commits a transactions changes, and ends the transaction

		void commit();
		
		// Commits a transactions changes, and preserves the transaction
		// context for further transaction processing
		void commit_retaining();

		// Rolls back a transactions changes, and ends the transaction
		void rollback();
		// Rolls back a transactions changes but maintains transaction context

		void rollback_retaining();

		// Performs the first phase of a two-phase commit, prior to calling
		// isc_commit_transaction(); used to coordinate a two-phase commit
		// with some external event
		void prepare();

		// Performs the first phase of a two-phase commit, prior to calling
		// isc_commit_transaction(); used to coordinate a two-phase commit
		// with some external event. This call accepts a message describing the
		// external event
		void prepare2();
		// Returns information about the specified named transaction.
		void getInfo();

	public:
		// InterBase version 3 transaction
		void setVersion3()
		{
			paramBuff_.addParam(isc_tpb_version3);
		}
		// Table-locking transaction model. This mode is serializable.
		void setConsistency()
		{
			paramBuff_.addParam(isc_tpb_consistency);
		}
		// High throughput, high concurrency transaction with repeat able read
		// consistency. This mode takes full advantage of the InterBase
		// multi-generational transaction model [Default].
		void setConcurrency()
		{
			paramBuff_.addParam(isc_tpb_concurrency);
		}
		// Concurrent, shared access of a specified table among all transactions; use
		// in conjunction with isc_tpb_lock_read and isc_tpb_lock_write to
		// establish the lock option [Default].
		void setShared()
		{
			paramBuff_.addParam(isc_tpb_shared);
		}
		// Concurrent, restricted access of a specified table; use in conjunction with
		// isc_tpb_lock_read and isc_tpb_lock_write to establish the lock option.
		void setProtected()
		{
			paramBuff_.addParam(isc_tpb_protected);
		}
		// isc_tpb_wait = Specifies that the transaction is to wait until the conflicting resource is
		// released before retrying an operation [Default].
		//
		// isc_tpb_nowait - Specifies that the transaction is not to wait for the resource to be released,
		// but instead, an update conflict error should be returned immediately
		void setWait(bool wait = true)
		{
			if (wait)
			{
				paramBuff_.addParam(isc_tpb_wait);
			}
			else
			{
				paramBuff_.addParam(isc_tpb_nowait);
			}
		}
		// Read-only access mode that allows a transaction only to select data from
		// tables
		void setReadOnly()
		{
			paramBuff_.addParam(isc_tpb_read);
		}
		// Read-write access mode of that allows a transaction to select, insert,
		// update, and delete table data [Default].
		void setReadWrite()
		{
			paramBuff_.addParam(isc_tpb_write);
		}
		// Read-only access of a specified table. Use in conjunction with
		// isc_tpb_shared, isc_tpb_protected, and isc_tpb_exclusive to establish the
		// lock option.
		void setLockReadOnly()
		{
			paramBuff_.addParam(isc_tpb_lock_read);
		}
		// Read-write access of a specified table. Use in conjunction with
		// isc_tpb_shared, isc_tpb_protected, and isc_tpb_exclusive to establish the
		// lock option [Default].
		void setLockReadWrite()
		{
			paramBuff_.addParam(isc_tpb_lock_write);
		}
		// High throughput, high concurrency transaction that can read changes
		// committed by other concurrent transactions. Transactions in this mode do
		// not provide repeatable read.
		void setReadCommitted()
		{
			paramBuff_.addParam(isc_tpb_read_committed);
		}
		// Enables an isc_tpb_read_committed transaction to read the most recently
		// committed version of a record even if other, uncommitted versions are
		// pending.
		void setRecVersion()
		{
			paramBuff_.addParam(isc_tpb_rec_version);
		}
		// Enables an isc_tpb_read_committed transaction to read only the latest
		// committed version of a record. If an uncommitted version of a record is
		// pending and isc_tpb_wait is also specified, then the transaction waits for
		// the pending record to be committed or rolled back before proceeding.
		// Otherwise, a lock conflict error is reported at once.
		void setNoRecVersion()
		{
			paramBuff_.addParam(isc_tpb_no_rec_version);
		}

	private:
		mutable isc_tr_handle	handle_;
		dpb       				paramBuff_;
		T_FB&                   dll;
		Error&                  ib_error;
	};

	class SQLDataArray
	{
	public:
		SQLDataArray(size_t size = 0);
		SQLDataArray(const SQLDataArray& theArray);
		~SQLDataArray();

	public:
		XSQLDA* getBuffer() const
		{
			return pDA_;
		}
		size_t getSize() const
		{
			return size_;
		}
		int getRequredSize() const
		{
			ASSERT(pDA_);
			return pDA_->sqld;
		}

	public:
		// Pos is ZERO-based ...
		XSQLVAR& operator[](size_t pos)
		{
			XSQLVAR* first_ptr = pDA_->sqlvar;
			return first_ptr[pos];
		}

	public:
		void setSize(size_t size);

	private:
		XSQLDA* pDA_;
		size_t  size_;
	};

	class DynamicSQL
	{
	public:
		DynamicSQL(const DataBase& db, const Transaction& tr, T_FB& dll, Error& ib_error);
		~DynamicSQL();

	public:
		void execute_immediate(const String& stmt);
		void execute_immediate_no_trans(const String& stmt);

		void execute_immediate_data(const String& stmt);

		// define_da is a data array, which is used to describe output arguments.
		void prepare(const String& stmt, const SQLDataArray& define_da);

		void execute();
		
		void execute2();
		
		// Retrieve "in" information.
		void describe_bind(const SQLDataArray& da);

		// Retrieve "out" information.
		void describe_define(const SQLDataArray& da);

		void setCursorName(const char* name);

		bool fetch();

		// Like isc_info_sql_stmt_select
		int getStmtType();

		void close_cursor();

	public:
		void setINSize(size_t size)
		{
			inDataArray_.setSize(size);
		}
		XSQLVAR* getINVar(size_t pos) const
		{
			ASSERT(pos < inDataArray_.getSize());
			return &inDataArray_.getBuffer()->sqlvar[pos];
		}
		void setOUTSize(size_t size)
		{
			outDataArray_.setSize(size);
		}
		XSQLVAR* getOUTVar(size_t pos) const
		{
			ASSERT(pos < outDataArray_.getSize());
			return &outDataArray_.getBuffer()->sqlvar[pos];
		}

		SQLDataArray& GetIn()
		{
			return inDataArray_;
		}
		SQLDataArray& GetOut()
		{
			return outDataArray_;
		}

	private:
		// Constructor-destructor
		void allocate_stmt();
		void allocate_stmt2();

		void drop_statement();

	private:
		unsigned short  dialect_;

		isc_db_handle*  pDBHandle_;
		isc_tr_handle*  pTRHandle_;

		T_FB&           dll;
		Error&          ib_error;

		isc_stmt_handle handle_;        // It is a pointer, actualy.

		SQLDataArray    inDataArray_;
		SQLDataArray    outDataArray_;

		bool            all_data_fetched_; // A workaround for problems that were introduced in v1.5.2.
	};

	class EmbededSQL
	{
	public:
		EmbededSQL();
		
	private:
	};

	class SegmentBlob;
	class BlobDescr
	{
	public:
	    BlobDescr(T_FB& dll, Error& ib_error) : dll(dll), ib_error(ib_error) {}
	    
		// Determines the subtype, character set, and segment size of a SegmentBlob, given a table name
		// and SegmentBlob column name.
		void get(
			SegmentBlob& blob,
			const String& tbl_name,
			const String& clm_name
		);
		// Loads a data structure with default information about a SegmentBlob, including its subtype,
		// character set, and segment size.
		void getDefault();
		// Sets the subtype and character set for a SegmentBlob.
		void set();

	protected:
	private:
		ISC_BLOB_DESC blobDescr_;
		T_FB&         dll;
		Error&        ib_error;
	};

	class SegmentBlob
	{
		friend class BlobDescr;

	public:
		class info
		{
			friend class SegmentBlob;

		public:
			info();

		public:
			long getLenght() const
			{
				return lenght_;
			}
			long getMaxSegments() const
			{
				return maxSegments_;
			}
			long getNumSegments() const
			{
				return numSegments_;
			}

		private:
			long    lenght_;
			long    maxSegments_;
			long    numSegments_;
			bool    type_segmented;
		};

	public:
		SegmentBlob(const DataBase& db, const Transaction& tr, T_FB& dll, Error& ib_error);
		~SegmentBlob();

	public:
		void open();
		void create();
		void close();

		bool getSegment(
			unsigned short& actualSegLen,
			unsigned short maxSegLen,
			char* pSegment
		);
		// void addSegment(const String& data);
		void addSegment(const char* pSegment, unsigned short seg_length);

		void put_segment(const char* data, size_t data_len);

		void cancel();
		
		void retrieve_info();
		
		const info& getInfo() const { return info_; }

	public:
		const GDS_QUAD& getID() const
		{
			return id_;
		}
		GDS_QUAD& getID()
		{
			return id_;
		}

	private:
		// Not documented ...
		void open_internal();
		void open2_internal();

		void setup_open();
		void setup_close();

		// Not documented ...
		void create_internal();

		// Creates and opens the SegmentBlob for write access, and optionally specifies the filters to be used
		// to translate the SegmentBlob from one subtype to another.
		void create2_internal();

	private:
		isc_db_handle*  pDBHandle_;
		isc_tr_handle*  pTRHandle_;
		isc_blob_handle handle_;        // It is a pointer, actualy.

		T_FB&           dll;
		Error&          ib_error;

		GDS_QUAD        id_;    // SegmentBlob ID put into out_sqlda by isc_dsql_fetch()
		int             stat_;
		bool            isOpen_;
		GDS_QUAD        lastID_;

		// Info ...
		bool            hasInfo_;
		info            info_;
	};

	class Array
	{
	public:
		Array(const DataBase& db, const Transaction& tr);

	public:
		const ISC_QUAD& getID() const
		{
			return id_;
		}
		ISC_QUAD& getID()
		{
			return id_;
		}

	protected:
		// isc_array_gen_sdl
		// isc_array_get_slice
		// isc_array_lookup_bounds
		// isc_array_lookup_desc
		// isc_array_set_desc
		// isc_array_put_slice
	private:
		isc_db_handle*  pDBHandle_;
		isc_tr_handle*  pTRHandle_;

		ISC_QUAD		id_;
		ISC_ARRAY_DESC	desc_;
	};

	class Event
	{
	public:
		Event();

	protected:
		// isc_event_block
		// isc_event_counts
		// isc_que_events
	private:
	};

    enum network_protocol_t {
		np_embedded,
		np_local,
		np_TCP_IP,
		np_NetBEUI,
		np_IPX_SPX
		};

	class Service
	{
	public:
		Service(T_FB& dll, Error& ib_error) : dll(dll), ib_error(ib_error) {}
		
		void attach(
			const char* host = NULL,
			const char* user = NULL,
			const char* pswd = NULL,
			network_protocol_t protocol = np_local
		);

		void detach();

		void executeCommand(const spb& pb);

		void query(const spb& request, const spb& result);

		T_FB& get_dll() { return dll; }
		Error& get_ib_error() { return ib_error; }

	private:
		isc_svc_handle      handle_;
		T_FB& dll;
		Error& ib_error;
	};

	class user_info : Moveable<user_info>
	{
	public:
		user_info();
		user_info(const user_info& info);

	public:
		unsigned long user_id;
		unsigned long group_id;
		String user_name;
		String first_name;
		String middle_name;
		String last_name;

	private:
		void copy(const user_info& info);
	};

	class server_info
	{
	public:
		server_info();
		server_info(const server_info& info);

	public:
		unsigned long svc_version;
		unsigned long num_licensed_users;
		String server_version;
		String implementation_str;
		String env_ib_str;
		String env_lock_str;
		String env_msg_str;
		String user_db_path_str;

	private:
		void copy(const server_info& info);
	};

	namespace svc_cmd
	{
		class base
		{
		public:
			base(Service& svc);

		protected:
			unsigned long read_long(char*& p) const;
			unsigned short read_short(char*& p) const;
			String read_string(char*& p) const;

		protected:
			Service*    svc_ptr_;
			spb         paramBuff_;
		};

		class backup : base
		{
		public:
			backup(Service& svc);
		};

		class restore : base
		{
		public:
			restore(Service& svc);
		};

		class set_properties : base
		{
		public:
			set_properties(Service& svc);
		};

		class repair : base
		{
		public:
			repair(Service& svc);
		};

		class get_db_stats : base
		{
		public:
			get_db_stats(Service& svc);
		};

		class get_server_info : base
		{
		public:
			get_server_info(Service& svc);

		public:
			void execute();
			const server_info& get_info() const
			{
				return info_;
			}

		public:
			server_info info_;
		};

		class get_log : base
		{
		public:
			get_log(Service& svc);
		};

		class get_user_info : base
		{
		public:
			get_user_info(Service& svc, const String& user_name);

		public:
			void execute();
			const user_info& get_info() const
			{
				return info_;
			}

		public:
			user_info info_;
			const String user_name_;
		};

		class get_users : base
		{
		public:
			get_users(Service& svc);

		public:
			void execute();
			const Vector<user_info>& get_info() const
			{
				return user_info_list_;
			}

		public:
			Vector<user_info> user_info_list_;
		};

		class add_user : base
		{
		public:
			add_user(Service& svc);

		public:
			void execute();

		public:
			void set_username(const char* name);
			void set_password(const char* pswd);
			void set_firstname(const char* value);
			void set_middlename(const char* value);
			void set_lastname(const char* value);
			void set_userid(unsigned long value);
			void set_groupid(unsigned long value);
			void set_groupname(const char* value);
			void set_rolename(const char* value);
		};

		class delete_user : base
		{
		public:
			delete_user(Service& svc);

		public:
			void execute();

		public:
			void set_username(const char* name);
			void set_rolename(const char* value);
		};

		class update_user : base
		{
		public:
			update_user(Service& svc);

		public:
			void execute();

		public:
			void set_username(const char* name);
			void set_password(const char* pswd);
			void set_firstname(const char* value);
			void set_middlename(const char* value);
			void set_lastname(const char* value);
			void set_userid(unsigned long value);
			void set_groupid(unsigned long value);
			void set_groupname(const char* value);
			void set_rolename(const char* value);
		};

		class add_license : base
		{
		public:
			add_license(Service& svc);
		};

		class remove_license : base
		{
		public:
			remove_license(Service& svc);
		};
	}

	class Install
	{
	};

}

#endif
