#include "Debuggers.h"

#ifdef PLATFORM_WIN32

uint32 Pdb::GetRegister32(const Context& ctx, int sym)
{
	switch(sym) {
#define CPU_REG(sym, context_var, kind, name, flags) case sym: return ctx.context32.context_var;
		#include "i386.cpu"
#undef CPU_REG
	}
	return 0;
}

#ifdef CPU_64
uint64 Pdb::GetRegister64(const Context& ctx, int sym)
{
	switch(sym) {
#define CPU_REG(sym, context_var, kind, name, flags) case sym: return ctx.context64.context_var;
		#include "amd64.cpu"
#undef CPU_REG
	}
	return 0;
}
#endif

const VectorMap<int, Pdb::CpuRegister>& Pdb::GetRegisterList()
{
	static VectorMap<int, CpuRegister> r32;
	ONCELOCK {
#define CPU_REG(sym_, context_var, kind_, name_, flags_) { CpuRegister& r = r32.Add(sym_); r.sym = sym_; r.kind = kind_; r.name = name_; r.flags = flags_; }
		#include "i386.cpu"
#undef CPU_REG
	}
#ifdef CPU_64
	static VectorMap<int, CpuRegister> r64;
	ONCELOCK {
#define CPU_REG(sym_, context_var, kind_, name_, flags_) { CpuRegister& r = r64.Add(sym_); r.sym = sym_; r.kind = kind_; r.name = name_; r.flags = flags_; }
		#include "amd64.cpu"
#undef CPU_REG
	}
	return win64 ? r64 : r32;
#else
	return r32;
#endif
}

uint64 Pdb::GetCpuRegister(const Context& ctx, int sym)
{
	int q = GetRegisterList().Find(sym);
	if(q < 0)
		return 0;
	const CpuRegister& r = GetRegisterList()[q];
#ifdef CPU_64
	uint64 val = win64 ? GetRegister64(ctx, sym) : GetRegister32(ctx, sym);
#else
	uint64 val = GetRegister32(ctx, sym);
#endif
	switch(r.kind) {
	case REG_L:
		return LOBYTE(val);
	case REG_H:
		return HIBYTE(val);
	case REG_X:
		return LOWORD(val);
	case REG_E:
		return LODWORD(val);
	}
	return val;
}

#endif
