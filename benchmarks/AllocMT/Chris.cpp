#if 0

#define NDEBUG

/*
Virtually Zero-Overhead Memory Allocator
Copyright (C) 2008  Christopher Michael Thomasson

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
_______________________________________________________________________

// *** Version 0.0.0 (Pre-Alpha ***
_______________________________________________________________________*/
#include <cstddef>
#include <cstdlib>
#include <cassert>
#include <cstdio>
#include <exception>
#include <new>
#include <pthread.h>
#include <Core/Core.h>


#if defined(_MSC_VER)
# pragma warning (disable : 4290)
#endif







/* Global Settings
_____________________________________________________________*/

#ifdef flagCHRIS
#define THREAD_MALLOC_OVERLOAD_NEW_DELETE
#endif


#if ! defined(THREAD_ALLOCATOR_HEAP_SIZE)
# define THREAD_ALLOCATOR_HEAP_SIZE 262144
#endif
#if ! defined(THREAD_ALLOCATOR_DTOR_MARK)
# define THREAD_ALLOCATOR_DTOR_MARK 0x80000000
#endif
#if ! defined(THREAD_ALLOCATOR_PRIME_COUNT)
# define THREAD_ALLOCATOR_PRIME_COUNT 1
#endif
#if ! defined(THREAD_ALLOCATOR_MAX_COUNT)
# define THREAD_ALLOCATOR_MAX_COUNT 2
#endif








/* Helper Macros
_____________________________________________________________*/
#if ! defined(NDEBUG)
# include <cstdio>
# define DBG_PRINTF(mp_exp) std::printf mp_exp
#else
# define DBG_PRINTF(mp_exp)
#endif


#define ALIGN_SIZE(mp_this, mp_type, mp_align) ((mp_type) \
  (((std::ptrdiff_t const)((mp_this))) + \
   ((std::ptrdiff_t const)((mp_align)) - 1) & \
   (-((std::ptrdiff_t const)((mp_align))))) \
)


#define IS_ALIGNED(mp_this, mp_align) ( \
  ! (((std::ptrdiff_t const)((mp_this))) % \
     ((std::ptrdiff_t const)((mp_align)))) \
)






/* Quick and Dirty i686 Atomic Operations
_____________________________________________________________*/
typedef int atomic_word_i686;

typedef char static_assert_i686 [
  (sizeof(atomic_word_i686) == 4) ? 1 : -1
];


__declspec(naked)
atomic_word_i686
atomic_i686_xadd(
 atomic_word_i686 volatile* const _this,
 atomic_word_i686 const addend
) {
  _asm {
    MOV ECX, [ESP + 4]
    MOV EAX, [ESP + 8]
    LOCK XADD [ECX], EAX
    RET
  }
}


__declspec(naked)
atomic_word_i686
atomic_i686_xchg(
 atomic_word_i686 volatile* const _this,
 atomic_word_i686 const value
) {
  _asm {
    MOV ECX, [ESP + 4]
    MOV EAX, [ESP + 8]
    XCHG [ECX], EAX
    RET
  }
}


typedef atomic_word_i686 atomic_word;
#define atomic_xadd atomic_i686_xadd
#define atomic_xchg atomic_i686_xchg








/* General Purpose Intrusive Double Linked-List
_____________________________________________________________*/
template<typename T>
static T dlist_init(T _this) throw() {
  _this->m_head = _this->m_tail = NULL;
  return _this;
}


template<typename T>
static T dlnode_init(T _this) throw() {
  _this->m_next = _this->m_prev = NULL;
  return _this;
}
 

template<typename T, typename N>
static N dlist_pop(T _this, N node) throw() {
  N const next = node->m_next;
  N const prev = node->m_prev;
  if (! next && ! prev) {
    _this->m_head = 0;
    _this->m_tail = 0;
  } else if (next && ! prev) {
    next->m_prev = 0;
    _this->m_head = next;
  } else if (! next && prev) {
    prev->m_next = 0;
    _this->m_tail = prev;
  } else {
    next->m_prev = prev;
    prev->m_next = next;
  }
  return node;
}


template<typename T, typename N>
static N dlist_push_head(T _this, N node) throw() {
  node->m_prev = 0;
  if (! _this->m_head) {
    _this->m_tail = node;
    node->m_next = NULL;
  } else {
    node->m_next = _this->m_head;
    _this->m_head->m_prev = node;
  }
  _this->m_head = node;
  return node;
}








/* General Purpose Thread-Safe Allocator
_____________________________________________________________*/
#if ! defined(NDEBUG)
  static atomic_word g_dbg_thread_malloc_count = 0;
  #define dbg_thread_get_count() (g_dbg_thread_malloc_count)
  #define dbg_thread_adjust_count(mp_value) \
    atomic_xadd(&g_dbg_thread_malloc_count, (mp_value))
#else
  #define dbg_thread_get_count() (0)
  #define dbg_thread_adjust_count(mp_value) (0)
#endif


extern "C" void* thread_malloc(std::size_t const);
extern "C" void thread_free(void const* const);


template<
 std::size_t T_heap_size = THREAD_ALLOCATOR_HEAP_SIZE, 
 atomic_word T_dtor_mark = THREAD_ALLOCATOR_DTOR_MARK
> struct thread_allocator {
  union aligner {
    char m_char;
    short m_short;
    int m_int;
    long m_long;
    double m_double;
    float m_float;
    aligner* m_this;
    void* m_ptr;
    void* (*m_fptr) (void*);
    std::size_t m_size_t;
    std::ptrdiff_t m_ptrdiff_t;
  };


  struct region {
    unsigned char m_heap[T_heap_size];
    region* m_next;
    region* m_prev;
    std::size_t m_offset;
    atomic_word m_lcount;
    atomic_word m_rcount;
    thread_allocator* m_owner;
    void* m_base_mem;

  private:
    void dtor() throw() {
      assert(! m_offset);
      assert(! m_lcount);
      assert(m_rcount & T_dtor_mark);
      DBG_PRINTF(("(%p)-thread_allocator::~thread_allocator()\n", (void*)this));
      std::free(m_base_mem);
      dbg_thread_adjust_count(-1);
    }


  public:
    static region* 
    create(thread_allocator* const owner) throw(std::bad_alloc) {
      void* const ptr = std::malloc(sizeof(region) + T_heap_size - 1);
      if (! ptr) {
        DBG_PRINTF(("(%p)-thread_allocator::region::create() - bad alloc\n", (void*)owner));
        throw std::bad_alloc();
      }
      region* const _this = ALIGN_SIZE(ptr, region*, T_heap_size);
      assert(IS_ALIGNED(_this, T_heap_size));
      assert(IS_ALIGNED(_this->m_heap, sizeof(aligner)));
      _this->m_base_mem = ptr;
      _this->m_offset = 0;
      _this->m_lcount = 1;
      _this->m_rcount = 0;
      _this->m_owner = owner;
      dlnode_init(_this);
      DBG_PRINTF(("(%p/%p)-thread_allocator::region::create() - ctor\n", 
        (void*)_this, (void*)owner));
      dbg_thread_adjust_count(1);
      return _this;
    }


    inline static region* from_ptr(void const* const ptr) throw() {
      if (! IS_ALIGNED(ptr, T_heap_size)) {
        return (region*)(ALIGN_SIZE(ptr, unsigned char const*, T_heap_size) - T_heap_size);
      }
      return (region*)ptr;
    }


    inline bool is_full() const throw() {
      return (! (m_offset < T_dtor_mark));
    }


    inline void reset(std::size_t const offset = 0) throw() {
      assert(m_lcount < 2);
      if (m_offset = offset) {
        m_lcount = 2;
      }
      DBG_PRINTF(("(%p/%p)-thread_allocator::region::reset()\n", 
        (void*)this, (void*)m_owner));
    }


    inline void* allocate_remote(std::size_t const sz) throw() {
      assert(sz <= T_heap_size);
      atomic_word const rcount = atomic_xchg(&m_rcount, 0);
      assert(! (rcount & T_dtor_mark));
      m_lcount -= rcount;
      if (m_lcount == 1) {
        reset(sz);
        DBG_PRINTF(("(%p/%p/%u)-thread_allocator::region::allocate_remote()\n",
          (void*)this, (void*)m_owner, m_offset));
        return m_heap;
      }
      return NULL;
    }


    inline void* allocate_local(std::size_t const sz) throw() {
      assert(sz <= T_heap_size);
      std::size_t const offset = m_offset;
      if (offset + sz <= T_heap_size) {
        m_offset = offset + sz;
        ++m_lcount;
        assert(IS_ALIGNED(m_heap + offset, sizeof(aligner)));
        return m_heap + offset;
      }
      return allocate_remote(sz);
    }


    inline void deallocate_remote() throw() {
      /*
        DBG_PRINTF(("(%p/%u)-thread_allocator::region::deallocate_remote()\n",
          (void*)this, m_offset));
      */
      atomic_word rcount = atomic_xadd(&m_rcount, 1);
      if (rcount & T_dtor_mark) {
        if (atomic_xadd(&m_lcount, -1) == 1) {
          DBG_PRINTF(("(%p/%p/%u)-thread_allocator::region::deallocate_remote() - dtor\n",
            (void*)this, (void*)m_owner, m_offset));
          reset();
          dtor();
        }
      }
    }

    inline bool deallocate_local() throw() {
      if ((--m_lcount) == 1) {
        reset();
        return true;
      }
      return false;
    }


    void shutdown() throw() {
      atomic_word const rcount = atomic_xchg(&m_rcount, T_dtor_mark);
      if (atomic_xadd(&m_lcount, -(rcount + 1)) == rcount + 1) {
          DBG_PRINTF(("(%p/%p/%u)-thread_allocator::region::deallocate_local() - dtor\n",
            (void*)this, (void*)m_owner, m_offset));
          reset();
          dtor();
      }
    };
  };


  region* m_head;
  region* m_tail;
  unsigned int m_count;
  unsigned int m_max_count;
  bool m_startup;


private:
  region* expand() throw(std::bad_alloc) {
    region* const r = dlist_push_head(this, region::create(this));
    ++m_count;
    DBG_PRINTF(("(%p/%p/%d/%d)-thread_allocator::expand()\n",
      (void*)this, (void*)r, m_count, m_max_count));
    return r;
  }


  void shrink(region* const r) throw() {
    --m_count;
    dlist_pop(this, r)->shutdown();
    DBG_PRINTF(("(%p/%p/%d/%d)-thread_allocator::shrink()\n",
      (void*)this, (void*)r, m_count, m_max_count));
  }


  void promote(region* const r) throw() {
    assert(r != m_head);
    dlist_pop(this, r);
    dlist_push_head(this,  r);
    DBG_PRINTF(("(%p/%p/%d/%d)-thread_allocator::promote()\n",
      (void*)this, (void*)r, m_count, m_max_count));
  }


public:
  void startup(
   unsigned int const prime = THREAD_ALLOCATOR_PRIME_COUNT, 
   unsigned int const max_count = THREAD_ALLOCATOR_MAX_COUNT
  ) throw(std::bad_alloc) {
    if (! m_startup) {
      m_count = 0;
      m_max_count = max_count;
      dlist_init(this);
      for (unsigned int i = 0; i < prime && i < m_max_count; ++i) {
        expand();
      }
      DBG_PRINTF(("(%p)-thread_allocator::startup()\n", (void*)this));
      m_startup = true;
    }
  }


  void shutdown() throw() {
    assert(m_startup);
    if (m_startup) {
      region* node = m_head;
      while (node) {
        region* const next = node->m_next;
        node->shutdown();
        --m_count;
        node = next;
      }
      assert(! m_count);
      m_startup = false;
      DBG_PRINTF(("(%p)-thread_allocator::shutdown()\n", (void*)this));
    }
  }

  void* allocate(std::size_t sz) throw(std::bad_alloc) {
//    assert(m_startup);
    startup();
    if (! m_startup) {
      std::printf("STATIC STARTUP ORDER ERROR!!!!!!!!!!\n");
    }
    sz = ALIGN_SIZE(sz, std::size_t, sizeof(aligner));
    if (sz <= T_heap_size) {
      region* node = m_head;
      while (node) {
        void* const ptr = node->allocate_local(sz);
        if (ptr) {
          if (node != m_head) {
            if (! node->is_full()) {
              promote(node);
            }
          }
          return ptr;
        }
        node = node->m_next;
      }
      return expand()->allocate_local(sz);
    }
    return NULL;
  }


  void deallocate(void const* const ptr) throw() {
    assert(m_startup);
    if (ptr) {
      region* const r = region::from_ptr(ptr);
      if (r->m_owner == this) {
        if (r->deallocate_local()) {
          if (m_count > m_max_count) {
            shrink(r);
          }
        }
      } else {
        r->deallocate_remote();
      }
    }
  }
};


__declspec(thread) thread_allocator<> tls_malloc = { 
  NULL, NULL, 0, 0, false
};


#if defined(THREAD_MALLOC_OVERLOAD_NEW_DELETE)

//# define THREAD_MALLOC_AUTO_STARTUP

# if ! defined(thread_malloc)
#   if defined(THREAD_MALLOC_AUTO_STARTUP)
#     define thread_malloc(mp_sz) ( \
        tls_malloc.startup(), tls_malloc.allocate(sz) \
      )
#   else
#     define thread_malloc(mp_sz) tls_malloc.allocate((mp_sz))
#   endif
# endif
# if ! defined(thread_free)
#   define thread_free(mp_ptr) tls_malloc.deallocate((mp_ptr))
# endif

#if ! defined(NDEBUG)
    void* operator new(std::size_t sz) throw(std::bad_alloc) {
      void* const ptr = thread_malloc(sz);
      dbg_thread_adjust_count(1);
      return ptr;
    }

    void* operator new[](std::size_t sz) throw(std::bad_alloc) {
      void* const ptr = thread_malloc(sz);
      dbg_thread_adjust_count(1);
      return ptr;
    }

    void operator delete(void* ptr) throw() {
      dbg_thread_adjust_count(-1);
      thread_free(ptr);
    }

    void operator delete[](void* ptr) throw() {
      dbg_thread_adjust_count(-1);
      thread_free(ptr);
    }
# else
    void* operator new(std::size_t sz) throw(std::bad_alloc) {
      return thread_malloc(sz);
    }

    void* operator new[](std::size_t sz) throw(std::bad_alloc) {
      return thread_malloc(sz);
    }

    void operator delete(void* ptr) throw() {
      thread_free(ptr);
    }

    void operator delete[](void* ptr) throw() {
      thread_free(ptr);
    }
# endif
#endif











/* Simple PThread Abstraction
_________________________________________________________________*/
class mutex_guard {
  pthread_mutex_t* m_plock;
public:
  struct error {
    struct base : public std::exception {};
    struct lock_failure : public base {};
    struct unlock_failure : public base {};
  };
public:
  mutex_guard(pthread_mutex_t* const plock)
   : m_plock(plock) {
    lock();
  }

  ~mutex_guard() throw() {
    unlock();
  }

  void lock() throw(error::lock_failure) {
    int const status = pthread_mutex_lock(m_plock);
    if (status) {
      throw error::lock_failure();
    }    
  }

  void unlock() throw(error::lock_failure) {
    int const status = pthread_mutex_unlock(m_plock);
    if (status) {
      throw error::unlock_failure();
    }    
  }

  pthread_mutex_t* get_capi() const throw() {
    return m_plock;
  }
};

class cond_guard {
  pthread_cond_t* m_pcond;
public:
  struct error {
    struct base : public std::exception {};
    struct signal_failure : public base {};
    struct wait_failure : public base {};
  };
public:
  cond_guard(pthread_cond_t* const pcond)
   :  m_pcond(pcond) {
  }

  void wait(mutex_guard& mutex) throw(error::wait_failure) {
    int const status = pthread_cond_wait(m_pcond, mutex.get_capi());
    if (status) {
      throw error::wait_failure();
    }
  }

  void signal() throw(error::signal_failure) {
    int const status = pthread_cond_signal(m_pcond);
    if (status) {
      throw error::signal_failure();
    }    
  }

  void broadcast() throw(error::signal_failure) {
    int const status = pthread_cond_broadcast(m_pcond);
    if (status) {
      throw error::signal_failure();
    }    
  }
};



extern "C" void* thread_sys_entry(void*);

class thread_base {
  virtual void on_entry() = 0;
  pthread_t m_tid;

public:
  virtual ~thread_base() = 0;

  void run() {
    int const status = 
      pthread_create(&m_tid, NULL, thread_sys_entry, this);
    if (status) {
      throw std::exception();
    }
  }

  void join() {
    int const status = pthread_join(m_tid, NULL);
    if (status) {
      throw std::exception();
    }
  }

  void startup_local() throw(std::bad_alloc) {
    tls_malloc.startup();
  }

  void execute_entry() {
    on_entry();
  }

  void shutdown_local() throw() {
#ifdef flagUSEMALLOC // disable U++ allocator (MemoryAlloc is malloc, new/delete standard library)
    tls_malloc.shutdown();
#else
	UPP::MemoryFreeThread();
#endif
  }
};

class thread_guard {
  thread_base* m_pbase;
public:
  thread_guard(thread_base* const pbase = NULL) throw(std::bad_alloc)
   :m_pbase(pbase) {
    if (m_pbase) {
      m_pbase->startup_local();
    } else {
      tls_malloc.startup();
    }
  }

  ~thread_guard() {
    if (m_pbase) {
      m_pbase->shutdown_local();
    } else {
      tls_malloc.shutdown();
    }
  }

  thread_base* get_base() const throw() {
    return m_pbase;
  }
};

thread_base::~thread_base() throw() {}

extern "C" void* 
thread_sys_entry(
 void* state
) throw() {
  thread_base* const _this = 
    reinterpret_cast<thread_base*>(state);
  _this->startup_local();
  _this->execute_entry();
  _this->shutdown_local();
  return NULL;
}




/* Sample Application
_________________________________________________________________*/
#include <cstdio>
#include <climits>
#include <ctime>
#include <list>
#include <iostream>


template<typename T, template<typename, typename> class C>
class ptr_collection {
  typedef C<T*, std::allocator<T*> > col_type;
  col_type m_col;
  static pthread_mutex_t g_lock;
  static pthread_cond_t g_cond_capi;
  static cond_guard g_cond;

  unsigned int prv_try_flush(unsigned int const max_count) {
    assert(max_count > 0);
    unsigned int count = 0;
    while (! m_col.empty() && count < max_count) {
      T* const node = m_col.front();
      m_col.pop_front();
      delete node;
      ++count;
    }
    return count;
  }

public:
  void push_back(T* const node) {
    {
      mutex_guard lock(&g_lock);
      m_col.push_back(node);
    }
    g_cond.signal();
  }

  unsigned int wait_flush(unsigned int const max_count) {
    unsigned int count;
    mutex_guard lock(&g_lock);
    while (! (count = prv_try_flush(max_count))) {
      g_cond.wait(lock);
    }
    assert(count && count <= max_count);
    return count;
  }
};

template<typename T, template<typename, typename> class C>
pthread_mutex_t
ptr_collection<T, C>::g_lock = PTHREAD_MUTEX_INITIALIZER;

template<typename T, template<typename, typename> class C>
pthread_cond_t
ptr_collection<T, C>::g_cond_capi = PTHREAD_COND_INITIALIZER;

template<typename T, template<typename, typename> class C>
cond_guard
ptr_collection<T, C>::g_cond(&ptr_collection<T, C>::g_cond_capi);






#define THREAD_COUNT 6
#define PRIVATE_COUNT 50000
#define CTOR_COUNT (THREAD_COUNT * 100000)
#define PRODUCER_CTOR_COUNT (CTOR_COUNT / THREAD_COUNT)
#define CONSUMER_DTOR_COUNT (CTOR_COUNT / THREAD_COUNT)
#define DBG_THREAD_PRINTF_VERBOSITY 25000000

struct TestVal {
	int q;
	int c, d, e;

	TestVal(int x) { q = x; }
};

static ptr_collection<TestVal, std::list>* g_int_ptrs = NULL;

class producer_thread : public thread_base {
  void on_entry() {
    DBG_PRINTF(("(%p)-producer_thread::on_entry()\n", (void*)this));
    int i;
    for (i = 1; i < PRODUCER_CTOR_COUNT + 1; ++i) {
      g_int_ptrs->push_back(new TestVal(i));
      if (! (i % DBG_THREAD_PRINTF_VERBOSITY)) {
        std::printf("(%p)-producer_thread - %d - create\n", (void*)this, i);
      }
    }
  }
};


class consumer_thread : public thread_base {
  void on_entry() {
    DBG_PRINTF(("(%p)-consumer_thread::on_entry()\n", (void*)this));
    int i = 0;
    while ((i += g_int_ptrs->wait_flush(CONSUMER_DTOR_COUNT - i)) != CONSUMER_DTOR_COUNT) {
      if (! (i % DBG_THREAD_PRINTF_VERBOSITY)) {
        std::printf("(%p)-consumer_thread - %d - delete\n", (void*)this, i);
      }
    }
    std::printf("(%p)-consumer_thread - %d - delete\n", (void*)this, i);
    assert(i == CONSUMER_DTOR_COUNT);
  }
};


class private_thread : public thread_base {
  void on_entry() {
    DBG_PRINTF(("(%p)-private_thread::on_entry()\n", (void*)this));
    for (int runs = 1; runs < 10; ++runs) { 
      std::list<TestVal*> int_ptrs_local;
      for (int i = 1; i < (runs * PRIVATE_COUNT) + 1; ++i) {
        int_ptrs_local.push_back(new TestVal(i));
        if (! (i % DBG_THREAD_PRINTF_VERBOSITY)) {
          std::printf("(%p)-private_thread - %d\n", (void*)this, i);
        }
      }

      while (! int_ptrs_local.empty()) {
        TestVal* const number = int_ptrs_local.front();
        if (! (number->q % DBG_THREAD_PRINTF_VERBOSITY)) {
          std::printf("(%p)-main - %d delete\n", (void*)this, *number);
        }
        delete number;
        int_ptrs_local.pop_front();
      }
    }
  }
};

#define TEST_RUNS 20

void ChrisTest() {
  std::clock_t total_time = 0;
  for(int runs = 1; runs < TEST_RUNS + 1; ++runs) {
    std::printf("(%d) - test running\n", runs);
    thread_guard this_thread;
    std::clock_t const startt = std::clock();
    {
      producer_thread producer[THREAD_COUNT];
      consumer_thread consumer[THREAD_COUNT];
      private_thread privatet;

      ptr_collection<TestVal, std::list> int_ptrs_global;
      g_int_ptrs = &int_ptrs_global;

      int i;
      for (i = 0; i < THREAD_COUNT; ++i) {
        consumer[i].run();
      }
      for (i = 0; i < THREAD_COUNT; ++i) {
        producer[i].run();
      }
      privatet.run();

      std::list<TestVal*> int_ptrs_local;
      for (i = 1; i < runs * PRIVATE_COUNT + 1; ++i) {
        int_ptrs_local.push_back(new TestVal(i));
        if (! (i % DBG_THREAD_PRINTF_VERBOSITY)) {
          std::printf("(%p)-main - %d\n", (void*)&this_thread, i);
        }
      }

      while (! int_ptrs_local.empty()) {
        TestVal* const number = int_ptrs_local.front();
        if (! (number->q % DBG_THREAD_PRINTF_VERBOSITY)) {
          std::printf("(%p)-main - %d delete\n", (void*)&this_thread, *number);
        }
        delete number;
        int_ptrs_local.pop_front();
      }

      if (! (runs % 2)) {
        for (i = 0; i < THREAD_COUNT; ++i) {
          consumer[i].join();
        }

        privatet.join();
        for (i = 0; i < THREAD_COUNT; ++i) {
          producer[i].join();
        }
      } else {
        for (i = 0; i < THREAD_COUNT; ++i) {
          producer[i].join();
        }

        for (i = 0; i < THREAD_COUNT; ++i) {
          consumer[i].join();
        }

        privatet.join();
      }
    }

    std::clock_t const endt = std::clock();
    total_time += endt - startt;

    std::printf("(%d) - test finished\n", runs);
  }

  std::cout << "\n\ntest time: " << 
    double(total_time)/CLOCKS_PER_SEC * 1000 << " ms\n--------------------\n";

  if (dbg_thread_get_count()) {
    std::printf("\n\nMEMORY LEAK: %d BLOCKS\n\n", dbg_thread_get_count());
  }

  DBG_PRINTF(("\n\n\n__________________________\npress <ENTER> to exit...\n"));
  std::getchar();
}

#endif