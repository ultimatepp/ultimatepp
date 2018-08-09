class SFtp : public Ssh {
public:
  enum Flags {
        READ        = LIBSSH2_FXF_READ,
        WRITE       = LIBSSH2_FXF_WRITE,
        APPEND      = LIBSSH2_FXF_APPEND,
        CREATE      = LIBSSH2_FXF_CREAT,
        TRUNCATE    = LIBSSH2_FXF_TRUNC,
        EXCLUSIVE   = LIBSSH2_FXF_EXCL,
     };

    enum Permissions {
        IRUSR = LIBSSH2_SFTP_S_IRUSR,
        IWUSR = LIBSSH2_SFTP_S_IWUSR,
        IXUSR = LIBSSH2_SFTP_S_IXUSR,
        IRWXU = LIBSSH2_SFTP_S_IRWXU,
        IRGRP = LIBSSH2_SFTP_S_IRGRP,
        IWGRP = LIBSSH2_SFTP_S_IWGRP,
        IXGRP = LIBSSH2_SFTP_S_IXGRP,
        IRWXG = LIBSSH2_SFTP_S_IRWXG,
        IROTH = LIBSSH2_SFTP_S_IROTH,
        IWOTH = LIBSSH2_SFTP_S_IWOTH,
        IXOTH = LIBSSH2_SFTP_S_IXOTH,
        IRWXO = LIBSSH2_SFTP_S_IRWXO,
        IRALL = IRUSR | IRGRP | IROTH,
        IWALL = IWUSR | IWGRP | IWOTH,
        IXALL = IXUSR | IXGRP | IXOTH,
        IRWXA = IRALL | IWALL | IXALL
    };

    class DirEntry : public Moveable<DirEntry> {
        friend class SFtp;
        public:
            String GetName() const                  { return filename; }
            int64  GetUid() const                   { return a->flags & LIBSSH2_SFTP_ATTR_UIDGID ? a->uid : -1; }
            int64  GetGid() const                   { return a->flags & LIBSSH2_SFTP_ATTR_UIDGID ? a->gid : -1; }
            int64  GetSize() const                  { return a->flags & LIBSSH2_SFTP_ATTR_SIZE ? a->filesize : -1; }
            Time   GetLastModified() const          { return a->flags & LIBSSH2_SFTP_ATTR_ACMODTIME ? TimeFromUTC(a->mtime) : Null; }
            Time   GetLastAccessed() const          { return a->flags & LIBSSH2_SFTP_ATTR_ACMODTIME ? TimeFromUTC(a->atime) : Null; }
            SFtpAttrs& GetAttrs()                   { return *a; }

            const SFtpAttrs& operator~() const      { return *a; }
            SFtpAttrs&  operator*()                 { return *a; }

            bool IsFile() const                     { return LIBSSH2_SFTP_S_ISREG(a->permissions); }
            bool IsDirectory() const                { return LIBSSH2_SFTP_S_ISDIR(a->permissions); }
            bool IsSymLink() const                  { return LIBSSH2_SFTP_S_ISLNK(a->permissions); }
            bool IsSpecial() const                  { return LIBSSH2_SFTP_S_ISCHR(a->permissions); }
            bool IsBlock() const                    { return LIBSSH2_SFTP_S_ISBLK(a->permissions); }
            bool IsPipe() const                     { return LIBSSH2_SFTP_S_ISFIFO(a->permissions); }
            bool IsSocket() const                   { return LIBSSH2_SFTP_S_ISSOCK(a->permissions); }
            bool IsReadable() const                 { return CanMode(IRUSR, IRGRP, IROTH); }
            bool IsWriteable() const                { return CanMode(IWUSR, IWGRP, IWOTH); }
            bool IsReadOnly() const                 { return IsReadable() && !IsWriteable(); }
            bool IsExecutable() const               { return !IsDirectory() && CanMode(IXUSR, IXGRP, IXOTH); }

            String ToString() const;
            String ToXml() const;

            DirEntry(const String& path);
            DirEntry(const String& path, const SFtpAttrs& attrs);
            DirEntry()                              { Zero();  }
            DirEntry(const Nuller&)                 { Zero();  }

            DirEntry(DirEntry&& e) = default;
            DirEntry& operator=(DirEntry&& e) = default;

        private:
            bool CanMode(dword u, dword g, dword o) const;
            void Zero();

            bool valid;
            String filename;
            One<SFtpAttrs> a;
     };
    typedef Vector<DirEntry> DirList;

public:
    SFtp&                   Timeout(int ms)                                         { ssh->timeout = ms; return *this; }
    SFtp&                   NonBlocking(bool b = true)                              { return Timeout(b ? 0 : Null) ;}
    SFtp&                   WaitStep(int ms)                                        { ssh->waitstep = clamp(ms, 0, INT_MAX); return *this; }
    SFtp&                   ChunkSize(int sz)                                       { ssh->chunk_size = clamp(sz, 128, INT_MAX); return *this; }

    LIBSSH2_SFTP_HANDLE*    GetHandle() const                                       { return sftp->handle; };
    Value                   GetResult() const                                       { return sftp->value; }

    // File
    SFtpHandle              Open(const String& path, dword flags, long mode);
    SFtpHandle              OpenRead(const String& path)                            { return Open(path, READ, IRALL); }
    SFtpHandle              OpenWrite(const String& path)                           { return Open(path, CREATE | WRITE, IRALL | IWUSR); }
    bool                    Close(SFtpHandle handle);
    bool                    Rename(const String& oldpath, const String& newpath);
    bool                    Delete(const String& path);
    bool                    Sync(SFtpHandle handle);
    SFtp&                   Seek(SFtpHandle handle, int64 position);
    int64                   GetPos(SFtpHandle handle);

    int                     Get(SFtpHandle handle, void *ptr, int size = INT_MAX);
    bool                    Put(SFtpHandle handle, const void *ptr, int size);
    bool                    SaveFile(const char *path, const String& data);
    String                  LoadFile(const char *path);

    // Read/Write
    bool                    Get(SFtpHandle handle, Stream& out);
    bool                    Get(const String& path, Stream& out);
    bool                    Get(const String& path, Stream& out, int64 offset);
    String                  Get(SFtpHandle handle);
    String                  Get(const String& path);
    bool                    Put(SFtpHandle handle, Stream& in);
    bool                    Put(Stream& in, const String& path);
    bool                    Put(Stream& in, const String& path, dword flags, long mode);
    bool                    Put(Stream& in, const String& path, int64 offset);
    bool                    Append(Stream& in, const String& path);
    bool                    Append(Stream& in, const String& path, long mode);
    String                  Peek(const String& path, int64 offset, int64 length);
    bool                    Poke(const String& data, const String& path, int64 offset, int64 length);
    
    // Directory
    SFtpHandle              OpenDir(const String& path);
    bool                    MakeDir(const String& path, long mode);
    bool                    RemoveDir(const String& path);
    bool                    ListDir(SFtpHandle handle, DirList& list);
    bool                    ListDir(const String& path, DirList& list);
    String                  GetWorkDir();

    // Symlink
    bool                    MakeLink(const String& orig, const String& link)        { return SymLink(orig, const_cast<String*>(&link), LIBSSH2_SFTP_SYMLINK); }
    bool                    ReadLink(const String& path, String& target)            { return SymLink(path, &target, LIBSSH2_SFTP_READLINK); }
    bool                    RealizePath(const String& path, String& target)         { return SymLink(path, &target, LIBSSH2_SFTP_REALPATH); }

    // Attributes
    bool                    GetAttrs(SFtpHandle handle, SFtpAttrs& attrs);
    bool                    GetAttrs(const String& path, SFtpAttrs& attrs);
    bool                    SetAttrs(SFtpHandle handle, const SFtpAttrs& attrs);
    bool                    SetAttrs(const String& path, const SFtpAttrs& attrs);
    DirEntry                GetInfo(const String& path);
    bool                    SetInfo(const DirEntry& entry)                          { return SetAttrs(entry.GetName(), ~entry); }
    int64                   GetSize(const String& path)                             { QueryAttr(path, SFTP_ATTR_SIZE); return sftp->value; }
    bool                    SetSize(const String& path, int64 size)                 { return ModifyAttr(path, SFTP_ATTR_SIZE, size); }
    Time                    GetLastModifyTime(const String& path)                   { QueryAttr(path, SFTP_ATTR_LAST_MODIFIED); return sftp->value; }
    bool                    SetLastModifyTime(const String& path, const Time& time) { return ModifyAttr(path, SFTP_ATTR_LAST_MODIFIED, time); }
    Time                    GetLastAccessTime(const String& path)                   { QueryAttr(path, SFTP_ATTR_LAST_ACCESSED); return sftp->value; }
    bool                    SetLastAccessTime(const String& path, const Time& time) { return ModifyAttr(path, SFTP_ATTR_LAST_ACCESSED, time); }

    // Tests
    bool                    FileExists(const String& path)                          { QueryAttr(path, SFTP_ATTR_FILE); return sftp->value; }
    bool                    DirectoryExists(const String& path)                     { QueryAttr(path, SFTP_ATTR_DIRECTORY); return sftp->value; }
    bool                    SymLinkExists(const String& path)                       { QueryAttr(path, SFTP_ATTR_SYMLINK); return sftp->value; }
    bool                    SocketExists(const String& path)                        { QueryAttr(path, SFTP_ATTR_SOCKET); return sftp->value; }
    bool                    PipeExists(const String& path)                          { QueryAttr(path, SFTP_ATTR_PIPE); return sftp->value; }
    bool                    BlockExists(const String& path)                         { QueryAttr(path, SFTP_ATTR_BLOCK); return sftp->value; }
    bool                    SpecialFileExists(const String& path)                   { QueryAttr(path, SFTP_ATTR_SPECIAL); return sftp->value; }

    // (Multithreaded I/O)
    static AsyncWork<String> AsyncGet(SshSession& session, const String& path, Gate<int64, int64, int64> progress = Null);
    static AsyncWork<void>   AsyncGet(SshSession& session, const String& path, Stream& out, Gate<int64, int64, int64> progress = Null);
    static AsyncWork<void>   AsyncPut(SshSession& session, String& in, const String& path, Gate<int64, int64, int64> progress = Null);
    static AsyncWork<void>   AsyncPut(SshSession& session, Stream& in, const String& path, Gate<int64, int64, int64> progress = Null);
    static AsyncWork<void>   AsyncAppend(SshSession& session, String& in, const String& path, Gate<int64, int64, int64> progress = Null);
    static AsyncWork<void>   AsyncAppend(SshSession& session, Stream& in, const String& path, Gate<int64, int64, int64> progress = Null);
    static AsyncWork<void>   AsyncGetToFile(SshSession& session, const String& src, const String& dest, Gate<int64, int64, int64> progress = Null);
    static AsyncWork<void>   AsyncPutFromFile(SshSession& session, const String& src, const String& dest, Gate<int64, int64, int64> progress = Null);
    static AsyncWork<void>   AsyncAppendFromFile(SshSession& session, const String& src, const String& dest, Gate<int64, int64, int64> progress = Null);
    static AsyncWork<void>   AsyncConsumerGet(SshSession& session, const String& path, Event<int64, const void*, int> consumer);

    Event<const void*, int> WhenContent;
    Gate<int64, int64>      WhenProgress;

    SFtp(SshSession& session);
    virtual ~SFtp();

    SFtp(SFtp&&) = default;
    SFtp& operator=(SFtp&&) = default;

private:
    bool                    Init() override;
    void                    Exit() override;
    bool                    Cleanup(Error& e) override;

    SFtpHandle              HANDLE(SFtpHandle h)                                   { return h ? h : sftp->handle; }
    int                     FStat(SFtpHandle handle, SFtpAttrs& a, bool set);
    int                     LStat(const String& path, SFtpAttrs& a, int type);
    bool                    QueryAttr(const String& path, int attr);
    bool                    ModifyAttr(const String& path, int attr, const Value& v);
    bool                    SymLink(const String& path, String* target, int type);
    bool                    DataRead(SFtpHandle handle, int64 size, Event<const void*, int>&& fn, bool str = false);
    bool                    DataWrite(SFtpHandle handle, Stream& out, int64 size);
    static void             StartAsync(int cmd, SshSession& session, const String& path, Stream& io,
                                        Gate<int64, int64, int64> progress, Event<int64, const void*, int> consumer = Null);

    struct SFtpData {
        LIBSSH2_SFTP*       session;
        SFtpHandle          handle;
        DirEntry            finfo;
        Value               value;
        StringStream        stream;
        int64               done;
    };
    One<SFtpData> sftp;

    enum OpCodes {
        SFTP_INIT,
        SFTP_EXIT,
        SFTP_START,
        SFTP_OPEN,
        SFTP_CLOSE,
        SFTP_SYNC,
        SFTP_RENAME,
        SFTP_DELETE,
        SFTP_GET,
        SFTP_PUT,
        SFTP_APPEND,
        SFTP_GET_STAT,
        SFTP_SET_STAT,
        SFTP_OPENDIR,
        SFTP_MAKEDIR,
        SFTP_REMOVEDIR,
        SFTP_LISTDIR,
        SFTP_QUERY,
        SFTP_MODIFY,
        SFTP_SEEK,
        SFTP_TELL,
        SFTP_PEEK,
        SFTP_POKE,
        SFTP_LINK
    };

    enum FileAttributes {
        SFTP_ATTR_FILE,
        SFTP_ATTR_DIRECTORY,
        SFTP_ATTR_SYMLINK,
        SFTP_ATTR_SOCKET,
        SFTP_ATTR_PIPE,
        SFTP_ATTR_BLOCK,
        SFTP_ATTR_SPECIAL,
        SFTP_ATTR_INFO,
        SFTP_ATTR_UID,
        SFTP_ATTR_GID,
        SFTP_ATTR_PERMISSIONS,
        SFTP_ATTR_SIZE,
        SFTP_ATTR_LAST_MODIFIED,
        SFTP_ATTR_LAST_ACCESSED
    };
};

class SFtpStream : public BlockStream {
protected:
	virtual  void  SetStreamSize(int64 size);
	virtual  dword Read(int64 at, void *ptr, dword size);
	virtual  void  Write(int64 at, const void *data, dword size);

public:
	virtual  void  Close();
	virtual  bool  IsOpen() const;

protected:
	SFtp       *sftp;
	SFtpHandle  handle;

	void      SetPos(int64 pos);
	void      Init(int64 size);

public:
	operator  bool() const                 { return IsOpen(); }

	bool       Open(SFtp& sftp, const char *filename, dword mode, int acm = 0644);
	SFtpStream(SFtp& sftp, const char *filename, dword mode, int acm = 0644);
	SFtpStream();
	~SFtpStream();
	SFtpHandle GetHandle() const            { return handle; }
};

class SFtpFileOut : public SFtpStream {
public:
	bool Open(SFtp& sftp, const char *fn, int acm = 0644) { return SFtpStream::Open(sftp, fn, CREATE|NOWRITESHARE, acm); }

	SFtpFileOut(SFtp& sftp, const char *fn)    { Open(sftp, fn); }
	SFtpFileOut()                              {}
};

class SFtpFileAppend : public SFtpStream {
public:
	bool Open(SFtp& sftp, const char *fn)      { return SFtpStream::Open(sftp, fn, APPEND|NOWRITESHARE); }

	SFtpFileAppend(SFtp& sftp, const char *fn) { Open(sftp, fn); }
	SFtpFileAppend()                           {}
};

class SFtpFileIn : public SFtpStream {
public:
	bool Open(SFtp& sftp, const char *fn)      { return SFtpStream::Open(sftp, fn, READ); }

	SFtpFileIn(SFtp& sftp, const char *fn)     { Open(sftp, fn); }
	SFtpFileIn()                               {}
};
