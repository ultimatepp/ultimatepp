class RwLock
{
        RwLock ( RwLock const& );
        RwLock const& operator = ( RwLock const& );


    private:
        LONG m_count;
        LONG m_rdwake;
        HANDLE m_wrwset;
        HANDLE m_rdwset;
        CRITICAL_SECTION m_wrlock;


    public:
        RwLock()
                : m_count ( LONG_MAX ),
                m_rdwake ( 0 ),
                m_wrwset ( CreateEvent ( 0, FALSE, FALSE, 0 ) ),
                m_rdwset ( CreateSemaphore ( 0, 0, LONG_MAX, 0 ) )
        {
            InitializeCriticalSection ( &m_wrlock );
        }

        ~RwLock() throw()
        {
            DeleteCriticalSection ( &m_wrlock );
            CloseHandle ( m_rdwset );
            CloseHandle ( m_wrwset );
        }


    public:
        void EnterWrite() {
            EnterCriticalSection ( &m_wrlock );
            LONG count = InterlockedExchangeAdd(&m_count, -LONG_MAX);
            if(count < LONG_MAX)
                if(InterlockedExchangeAdd ( &m_rdwake, LONG_MAX - count ) + LONG_MAX - count )
                    WaitForSingleObject ( m_wrwset, INFINITE );
        }

        void LeaveWrite() {
            LONG count = InterlockedExchangeAdd ( &m_count, LONG_MAX );
            if (count < 0)
                ReleaseSemaphore ( m_rdwset, count * -1, 0 );
            LeaveCriticalSection ( &m_wrlock );
        }


    public:
        void EnterRead() {
            LONG count = InterlockedDecrement ( &m_count );
            if(count < 0)
                WaitForSingleObject ( m_rdwset, INFINITE );
        }

        void LeaveRead() {
            LONG count = InterlockedIncrement ( &m_count );
            if ( count < 1 ) {
                if ( ! InterlockedDecrement ( &m_rdwake ) )
                    SetEvent ( m_wrwset );
            }
        }
};
