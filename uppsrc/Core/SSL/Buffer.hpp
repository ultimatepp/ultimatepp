#ifndef _Core_Ssl_SecureBuffer_h_
#define _Core_Ssl_SecureBuffer_h_

#ifdef PLATFORM_POSIX
#include <sys/mman.h>
#endif

#ifndef LLOG
#define LLOG(x) // RLOG(x)

template <class T>
void SecureZero(T* ptr, size_t count)
{
    static_assert(std::is_trivially_copyable_v<T>
               && std::is_trivially_destructible_v<T>,
                  "Upp::SecureZero: T must be trivially copyable & destructible");

    if(!ptr || count == 0)
        return;

    OPENSSL_cleanse(reinterpret_cast<void*>(ptr), count * sizeof(T));
}

template<class T, size_t N>
void SecureZero(T (&obj)[N]) // Safe overload for static arrays.
{
	SecureZero(obj, N);
}

template <class T>
class SecureBuffer : Moveable<SecureBuffer<T>>, NoCopy {
	
    static_assert(std::is_trivially_copyable_v<T>
               && std::is_trivially_destructible_v<T>,
                  "Upp::SecureBuffer: T must be trivially copyable & destructible");

public:
    SecureBuffer()                                       { size = 0; ptr = nullptr; }
    SecureBuffer(size_t size_)                           { New(size_); }
    SecureBuffer(SecureBuffer&& src)                     { Pick(pick(src)); }
    ~SecureBuffer()                                      { Free(); }

    SecureBuffer& operator=(SecureBuffer&& src)          { Pick(pick(src));  return *this; }

    void        Alloc(size_t size)                       { Free(); New(size); }
    void        Clear()                                  { Free(); }
    void        Zero()                                   { SecureZero(ptr, size); };

    size_t      GetSize() const                          { return size; }

    bool        IsEmpty() const                          { return ptr == nullptr; }

    operator T*()                                        { return ptr; }
    operator const T*() const                            { return ptr; }
    T *operator~()                                       { return ptr; }
    const T *operator~() const                           { return ptr; }
    T          *Get()                                    { return ptr; }
    const T    *Get() const                              { return ptr; }
    T          *begin()                                  { return ptr; }
    const T    *begin() const                            { return ptr; }

    T*          Begin()                                  { return ptr;  }
    const T*    Begin() const                            { return ptr;  }
    
    
    T*          end()                                    { return ptr + size; }
    const T*    end() const                              { return ptr + size; }
    T*          End()                                    { return ptr + size; }
    const T*    End() const                              { return ptr + size; }
    

    T& operator[](size_t i)                              { ASSERT(i < size); return ptr[i]; }
    const T& operator[](size_t i) const                  { ASSERT(i < size); return ptr[i]; }


private:
    void New(size_t sz);
    void Free();
    void Pick(SecureBuffer&& src);
    void LockMemory();
    void UnlockMemory();

    T* ptr = nullptr;
    size_t size;
};

template<typename T>
void SecureBuffer<T>::New(size_t sz)
{
    size = 0;
    ptr  = nullptr;

    if(sz > 0) {
        size = sz;
        ptr = (T*) MemoryAlloc(size * sizeof(T));
        LockMemory();
    }
}

template<typename T>
void SecureBuffer<T>::Pick(SecureBuffer&& src)
{
    if(this != &src) {
        Free();
        ptr = src.ptr;
        size = src.size;
        src.ptr = nullptr;
        src.size = 0;
    }
}

template<typename T>
void SecureBuffer<T>::Free()
{
    if(ptr) {
        Zero();
        UnlockMemory();
        MemoryFree(ptr);
        ptr = nullptr;
        size = 0;
    }
}

template<typename T>
void SecureBuffer<T>::LockMemory()
{
    if(!ptr || !size)
        return;
#if defined(PLATFORM_WIN32)
    if(!VirtualLock((LPVOID) ptr, size * sizeof(T))) {
        LLOG("SecureBuffer::LockMemory: VirtualLock failed with error code " << GetLastError());
    }
#elif defined(PLATFORM_POSIX)
    if(mlock((void*) ptr, size * sizeof(T)) != 0) {
        LLOG("SecureBuffer::LockMemory: mlock failed with errno " << errno << " (" << strerror(errno) << ")");
    }
#else
    NEVER();
#endif
}

template<typename T>
void SecureBuffer<T>::UnlockMemory()
{
    if(!ptr || !size)
        return;
#if defined(PLATFORM_WIN32)
    if(!VirtualUnlock((LPVOID) ptr, size * sizeof(T))) {
        LLOG("SecureBuffer::UnlockMemory: VirtualUnlock failed with error code " << GetLastError());
    }
#elif defined(PLATFORM_POSIX)
    if(munlock((void*) ptr, size * sizeof(T)) != 0) {
        LLOG("SecureBuffer::UnlockMemory: munlock failed with errno " << errno << " (" << strerror(errno) << ")");
    }
#else
    NEVER();
#endif
}

#undef LLOG
#endif

#endif
