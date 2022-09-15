class SFtp : public Ssh {
public:
    enum Flags {
        READ        = LIBSSH2_FXF_READ,
        WRITE       = LIBSSH2_FXF_WRITE,
        APPEND      = LIBSSH2_FXF_APPEND,
        CREATE      = LIBSSH2_FXF_CREAT,
        TRUNCATE    = LIBSSH2_FXF_TRUNC,
        EXCLUSIVE   = LIBSSH2_FXF_EXCL
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
            int64  GetUid() const                   { return (a->flags & LIBSSH2_SFTP_ATTR_UIDGID) ? a->uid : -1; }
            int64  GetGid() const                   { return (a->flags & LIBSSH2_SFTP_ATTR_UIDGID) ? a->gid : -1; }
            int64  GetSize() const                  { return (a->flags & LIBSSH2_SFTP_ATTR_SIZE) ? a->filesize : -1; }
            Time   GetLastModified() const          { return (a->flags & LIBSSH2_SFTP_ATTR_ACMODTIME) ? TimeFromUTC(a->mtime) : Null; }
            Time   GetLastAccessed() const          { return (a->flags & LIBSSH2_SFTP_ATTR_ACMODTIME) ? TimeFromUTC(a->atime) : Null; }
            dword  GetPermissions() const           { return a->permissions; }
            SFtpAttrs& GetAttrs()                   { return *a; }

            const SFtpAttrs& operator~() const      { return *a; }
            SFtpAttrs&  operator*()                 { return *a; }
            operator bool() const                   { return valid; }

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
            FileSystemInfo::FileInfo ToFileInfo() const;

            void Serialize(Stream& s);
            
            DirEntry(const String& path);
            DirEntry(const String& path, const SFtpAttrs& attrs);
            DirEntry()                              { Zero();  }
            DirEntry(const Nuller&)                 { Zero();  }

            DirEntry(DirEntry&&) = default;
            DirEntry& operator=(DirEntry&&) = default;
            
        private:
            bool CanMode(dword u, dword g, dword o) const;
            void Zero();

            bool valid;
            String filename;
            One<SFtpAttrs> a;
     };
    using DirList = Vector<DirEntry>;

public:
    SFtp&                   Timeout(int ms)                                         { ssh->timeout = ms; return *this; }
    SFtp&                   ChunkSize(int sz)                                       { ssh->chunk_size = clamp(sz, 1, INT_MAX); return *this; }

    // File
    SFtpHandle              Open(const String& path, dword flags, long mode);
    SFtpHandle              OpenRead(const String& path)                            { return Open(path, READ, IRALL); }
    SFtpHandle              OpenWrite(const String& path)                           { return Open(path, CREATE | WRITE, IRALL | IWUSR); }
    void                    Close(SFtpHandle handle);
    bool                    Rename(const String& oldpath, const String& newpath);
    bool                    Delete(const String& path);
    bool                    Sync(SFtpHandle handle);
    SFtp&                   Seek(SFtpHandle handle, int64 position);
    int64                   GetPos(SFtpHandle handle);

    // Read/Write
    int                     Get(SFtpHandle handle, void *ptr, int size = INT_MAX);
    int                     Put(SFtpHandle handle, const void *ptr, int size);
    bool                    SaveFile(const char *path, const String& data);
    String                  LoadFile(const char *path);
    bool                    SaveFile(const char *path, Stream& in);
    bool                    LoadFile(Stream& out, const char *path);
    
    int                     GetDone() const                                         { return done; }
    
    // Directory
    SFtpHandle              OpenDir(const String& path);
    bool                    MakeDir(const String& path, long mode);
    bool                    RemoveDir(const String& path);
    bool                    ListDir(SFtpHandle handle, DirList& list);
    bool                    ListDir(const String& path, DirList& list);
    // Symlink
    bool                    MakeLink(const String& orig, String& target)            { return SymLink(orig, target, LIBSSH2_SFTP_SYMLINK); }
    bool                    ReadLink(const String& path, String& target)            { return SymLink(path, target, LIBSSH2_SFTP_READLINK); }
    bool                    RealizePath(const String& path, String& target)         { return SymLink(path, target, LIBSSH2_SFTP_REALPATH); }

    String                  GetDefaultDir()                                         { String s; return RealizePath(".", s) ? s : String::GetVoid(); }
    
    // Attributes
    bool                    GetAttrs(SFtpHandle handle, SFtpAttrs& attrs);
    bool                    GetAttrs(const String& path, SFtpAttrs& attrs);
    bool                    SetAttrs(SFtpHandle handle, const SFtpAttrs& attrs);
    bool                    SetAttrs(const String& path, const SFtpAttrs& attrs);
    DirEntry                GetInfo(const String& path);
    bool                    SetInfo(const DirEntry& entry)                          { return SetAttrs(entry.GetName(), ~entry); }
    int64                   GetSize(const String& path)                             { return QueryAttr(path, SFTP_ATTR_SIZE); }
    bool                    SetSize(const String& path, int64 size)                 { return ModifyAttr(path, SFTP_ATTR_SIZE, size); }
    Time                    GetLastModifyTime(const String& path)                   { return QueryAttr(path, SFTP_ATTR_LAST_MODIFIED); }
    bool                    SetLastModifyTime(const String& path, const Time& time) { return ModifyAttr(path, SFTP_ATTR_LAST_MODIFIED, time); }
    Time                    GetLastAccessTime(const String& path)                   { return QueryAttr(path, SFTP_ATTR_LAST_ACCESSED); }
    bool                    SetLastAccessTime(const String& path, const Time& time) { return ModifyAttr(path, SFTP_ATTR_LAST_ACCESSED, time); }

    // Tests
    bool                    FileExists(const String& path)                          { return QueryAttr(path, SFTP_ATTR_FILE); }
    bool                    DirectoryExists(const String& path)                     { return QueryAttr(path, SFTP_ATTR_DIRECTORY); }
    bool                    SymLinkExists(const String& path)                       { return QueryAttr(path, SFTP_ATTR_SYMLINK); }
    bool                    SocketExists(const String& path)                        { return QueryAttr(path, SFTP_ATTR_SOCKET); }
    bool                    PipeExists(const String& path)                          { return QueryAttr(path, SFTP_ATTR_PIPE); }
    bool                    BlockExists(const String& path)                         { return QueryAttr(path, SFTP_ATTR_BLOCK); }
    bool                    SpecialFileExists(const String& path)                   { return QueryAttr(path, SFTP_ATTR_SPECIAL); }

    Gate<int64, int64>      WhenProgress;
    
    SFtp(SshSession& session);
    virtual ~SFtp();

    SFtp(SFtp&&) = default;

private:
    bool                    Init() override;
    void                    Exit() override;

    int                     FStat(SFtpHandle handle, SFtpAttrs& a, bool set);
    int                     LStat(const String& path, SFtpAttrs& a, int type);
    Value                   QueryAttr(const String& path, int attr);
    bool                    ModifyAttr(const String& path, int attr, const Value& v);
    bool                    SymLink(const String& path, String& target, int type);
    int                     Read(SFtpHandle handle, void* ptr, int size);
    int                     Write(SFtpHandle handle, const void* ptr, int size);
    bool                    CopyData(Stream& dest, Stream& src, int64 maxsize = INT64_MAX);
  
    One<LIBSSH2_SFTP*>      sftp_session;
    int                     done;

    enum FileAttributes {
        SFTP_ATTR_FILE,
        SFTP_ATTR_DIRECTORY,
        SFTP_ATTR_SYMLINK,
        SFTP_ATTR_SOCKET,
        SFTP_ATTR_PIPE,
        SFTP_ATTR_BLOCK,
        SFTP_ATTR_SPECIAL,
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

class SFtpFileSystemInfo : public FileSystemInfo {
public:
    SFtpFileSystemInfo& Mount(SFtp& sftp)       { browser = &sftp; return *this; }
    int GetStyle() const override               { return STYLE_POSIX; }
    bool CreateFolder(String path, String& error) const override;
    Array<FileSystemInfo::FileInfo> Find(String mask, int max_count = INT_MAX, bool unmounted = false) const override;
    
    SFtpFileSystemInfo()                        { browser = nullptr; }
    SFtpFileSystemInfo(SFtp& sftp)              { Mount(sftp); }
    virtual ~SFtpFileSystemInfo()               {}
    
private:
    SFtp *browser = nullptr;
};

