#define GLOBAL_VP(type, name, param) \
name() \
{ \
	static type x param; \
	return x; \
}

#define GLOBAL_VARP(type, name, param) \
type& GLOBAL_VP(type, name, param)

#define GLOBAL_V(type, name)   GLOBAL_VP(type, name, init_)
#define GLOBAL_VAR(type, name) type& GLOBAL_V(type, name)

#define GLOBAL_VP_INIT(type, name, param) \
name() \
{ \
	static type x param; \
	return x; \
} \
INITBLOCK { name(); }

#define GLOBAL_VARP_INIT(type, name, param) \
type& GLOBAL_VP_INIT(type, name, param)

#define GLOBAL_V_INIT(type, name)   GLOBAL_VP_INIT(type, name, init_)
#define GLOBAL_VAR_INIT(type, name) type& GLOBAL_V_INIT(type, name)


// DEPRECATED! (USE ONCELOCK_)
#define INIT_LOCKV(init, code) \
if(!init) { \
	static StaticCriticalSection cs; \
	CriticalSection::Lock __(cs); \
	if(!init) { \
		init = true; \
		code \
	} \
}

// DEPRECATED! (USE ONCELOCK)
#define INIT_LOCK(code) \
static bool init; \
INIT_LOCKV(init, code)
