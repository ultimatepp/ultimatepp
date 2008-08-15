#if 0

#include <cstddef>
#include <cstdlib>
#include <cassert>
#include <exception>
#include <new>

#define NDEBUG


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




/* General Purpose Dynamic Region Allocator
_____________________________________________________________*/
template<std::size_t T_depth>
struct region_allocator {
  union aligner {
    char m_char;
    short m_short;
    int m_int;
    long m_long;
    double m_double;
    float m_float;
    aligner* m_this;
    void* m_ptr;
    std::size_t m_size_t;
    std::ptrdiff_t m_ptrdiff_t;
  };

  struct heap {
    unsigned char m_base[T_depth];
    heap* m_next;
    heap* m_prev;
    std::size_t m_offset;
    std::size_t m_depth;

    void ctor() {
      dlnode_init(this);
      m_offset = m_depth = 0;
    }

    void* allocate(std::size_t const sz) throw() {
      std::size_t const offset = m_offset;
      if (offset + sz < T_depth - 1) {
        ++m_depth;
        m_offset += sz;
        return m_base + offset;
      }
      return NULL;
    }

    bool deallocate() throw() {
      if (! (--m_depth)) {
        m_offset = 0;
        return true;
      }
      return false;
    }

    bool is_heap(void const* const ptr) const throw() {
      unsigned char const* const head = 
        reinterpret_cast<unsigned char const*>(m_base);
      unsigned char const* const tail = 
        reinterpret_cast<unsigned char const*>(m_base + T_depth - 1);
      unsigned char const* const buf = 
        reinterpret_cast<unsigned char const*>(ptr);
      return (buf >= head && buf < tail);
    }
  };

  heap* m_head;
  heap* m_tail;
  std::size_t m_depth;
  std::size_t const m_max_depth;

  heap* create_heap() {
    heap* const h = reinterpret_cast<heap*>(std::malloc(sizeof(*h)));
    if (! h) { 
      throw std::bad_alloc();
    }
    h->ctor();
    dlist_push_head(this, h);
    ++m_depth;
    DBG_PRINTF(("(%p/%d/%d) Dynamic Region  Expand\n", 
      (void*)h, m_depth, m_max_depth));
    return h;
  }

  void destroy_heap(heap* const h) throw() {
    dlist_pop(this, h);
    std::free(h);
    --m_depth;
    DBG_PRINTF(("(%p/%d/%d) Dynamic Region Shrink\n", 
      (void*)h, m_depth, m_max_depth));
  }

  heap* find_heap(void const* const ptr) const throw() {
    heap* h = m_head;
    while (h) {
      if (h->is_heap(ptr)) { 
        return h;
      }
      h = h->m_next;
    }
    return NULL;
  }

  region_allocator(
   std::size_t const prime, 
   std::size_t const max_depth
  ) : m_depth(0),
      m_max_depth(max_depth) {
    dlist_init(this);
    for (std::size_t i = 0; i < prime && i < max_depth; ++i) {
      create_heap();
    }
  }

  ~region_allocator() throw() {
    heap* h = m_head;
    while (h) {
      heap* const next = h->m_next;
      destroy_heap(h);
      h = next;
    }
  }

  inline void* allocate(std::size_t sz) throw(std::bad_alloc) {
    sz = ALIGN_SIZE(sz, std::size_t, sizeof(aligner));
    if (sz <= T_depth) {
      heap* h = m_head;
      while (h) {
        void* const ptr = h->allocate(sz);
        if (ptr) { 
          if (h != m_head) {
            dlist_pop(this, h);
            dlist_push_head(this, h);
            DBG_PRINTF(("(%p/%d/%d) Dynamic Region Heap Adjust\n", 
              (void*)h, m_depth, m_max_depth));
          }
          return ptr;
        }
        h = h->m_next;
      }
      void* const ptr = create_heap()->allocate(sz);
      if (ptr) {
        return ptr;
      }
    }
    void* const ptr = std::malloc(sz);
    if(! ptr) {
      throw std::bad_alloc();
    }
    return ptr;
  }

  inline void deallocate(void* const ptr) throw() {
    heap* const h = find_heap(ptr);
    if (h) {
      if (h->deallocate()) {
        if (m_depth > m_max_depth) {
          destroy_heap(h);
        }
      }
    } else {
      std::free(ptr);
    }
  }
};




/* Global Allocator Overloads
_____________________________________________________________*/
#ifdef flagCHRIS
#define TEST_USE_REGION_ALLOCATOR
#endif

#if defined(TEST_USE_REGION_ALLOCATOR)
  // global allocator instance
  static region_allocator<5000000> g_region_malloc(1, 3);

  // global operator new/delete overloads
  void* operator new(std::size_t sz) throw(std::bad_alloc) {
    return g_region_malloc.allocate(sz);
  }

  void* operator new [](std::size_t sz) throw(std::bad_alloc) {
    return g_region_malloc.allocate(sz);
  }

  void operator delete(void* ptr) throw() {
    g_region_malloc.deallocate(ptr);
  }

  void operator delete [](void* ptr) throw() {
    g_region_malloc.deallocate(ptr);
  }  
#endif



/* Simple Test Application
_____________________________________________________________*/
#include <ctime>
#include <cstdio>
#include <iostream>
#include <list>


#define SEQUENCE_DEPTH 999999


void ChrisTest() {
  std::clock_t start = std::clock();

  {
    int i;
    std::list<int> sequence1;
    std::list<int*> sequence2;

    for (i = 0; i < SEQUENCE_DEPTH; ++i) {
      sequence1.push_back(i);
    }

    for (i = 0; i < SEQUENCE_DEPTH; ++i) {
      sequence2.push_back(new int(i));
    }

    while (! sequence1.empty()) {
      sequence1.pop_front();
    }

    while (! sequence2.empty()) {
      delete sequence2.front();
      sequence2.pop_front();
    }
  }

  std::clock_t endt = std::clock();
  std::cout <<"Time: " << 
    double(endt-start)/CLOCKS_PER_SEC * 1000 << " ms\n\n"
    << "Press <ENTER> to exit...\n";
}

#endif