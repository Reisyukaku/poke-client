#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

typedef uint8_t u8;       ///<   8-bit unsigned integer.
typedef uint16_t u16;     ///<  16-bit unsigned integer.
typedef uint32_t u32;     ///<  32-bit unsigned integer.
typedef uint64_t u64;     ///<  64-bit unsigned integer.
typedef __uint128_t u128; ///< 128-bit unsigned integer.

typedef int8_t s8;       ///<   8-bit signed integer.
typedef int16_t s16;     ///<  16-bit signed integer.
typedef int32_t s32;     ///<  32-bit signed integer.
typedef int64_t s64;     ///<  64-bit signed integer.
typedef __int128_t s128; ///< 128-bit unsigned integer.

typedef volatile u8 vu8;     ///<   8-bit volatile unsigned integer.
typedef volatile u16 vu16;   ///<  16-bit volatile unsigned integer.
typedef volatile u32 vu32;   ///<  32-bit volatile unsigned integer.
typedef volatile u64 vu64;   ///<  64-bit volatile unsigned integer.
typedef volatile u128 vu128; ///< 128-bit volatile unsigned integer.

typedef volatile s8 vs8;     ///<   8-bit volatile signed integer.
typedef volatile s16 vs16;   ///<  16-bit volatile signed integer.
typedef volatile s32 vs32;   ///<  32-bit volatile signed integer.
typedef volatile s64 vs64;   ///<  64-bit volatile signed integer.
typedef volatile s128 vs128; ///< 128-bit volatile signed integer.

typedef unsigned char   uchar;
typedef	unsigned short	ushort;
typedef	unsigned int	uint;	
typedef	unsigned long	ulong;

typedef struct {
	float x;
	float y;
} Vec2f;

#define ALIGN_UP(x, a) ((((uintptr_t)x) + (((uintptr_t)a)-1)) & ~(((uintptr_t)a)-1))
#define ALIGN_DOWN(x, a) ((uintptr_t)(x) & ~(((uintptr_t)(a)) - 1))
#define ALIGNED(a)      __attribute__((aligned(a)))
#define ON_INIT         __attribute__((constructor))
#define NOINLINE        __attribute__((noinline))
#define NORETURN        __attribute__((noreturn))
#define UNREACHABLE __builtin_unreachable()
#define PAGE_SIZE (0x1000)
#define ALWAYS_INLINE inline __attribute__((always_inline))
#define BITSIZEOF(x) (sizeof(x) * CHAR_BIT)

#define STRCACHE_N		53
#define STRCACHE_M		2

typedef void (*lua_WarnFunction) (void *ud, const char *msg, int tocont);

typedef union Value {
  void *gc;    /* collectable objects */
  void *p;         /* light userdata */
  void* f; /* light C functions */
  long long i;   /* integer numbers */
  double n;    /* float numbers */
} Value;

typedef struct TValue {
  Value value_; 
  unsigned char tt_;
} TValue;

typedef struct stringtable {
  void **hash;
  int nuse;  /* number of elements */
  int size;
} stringtable;

typedef struct CallInfo {
  void* func;  /* function index in the stack */
  void*	top;  /* top for this function */
  struct CallInfo *previous, *next;  /* dynamic call link */
  union {
    struct {  /* only for Lua functions */
      const unsigned *savedpc;
      volatile int trap;
      int nextraargs;  /* # of extra arguments in vararg functions */
    } l;
    struct {  /* only for C functions */
      void* k;  /* continuation in case of yields */
      ptrdiff_t old_errfunc;
      ptrdiff_t ctx;  /* context info. in case of yields */
    } c;
  } u;
  union {
    int funcidx;  /* called-function index */
    int nyield;  /* number of values yielded */
    struct {  /* info about transferred values (for call/return hooks) */
      unsigned short ftransfer;  /* offset of first value transferred */
      unsigned short ntransfer;  /* number of values transferred */
    } transferinfo;
  } u2;
  short nresults;  /* expected number of results from this function */
  unsigned short callstatus;
} CallInfo;

typedef struct global_State {
  void* frealloc;  /* function to reallocate memory */
  void *ud;         /* auxiliary data to 'frealloc' */
  ptrdiff_t totalbytes;  /* number of bytes currently allocated - GCdebt */
  ptrdiff_t GCdebt;  /* bytes allocated not yet compensated by the collector */
  size_t GCestimate;  /* an estimate of the non-garbage memory in use */
  size_t lastatomic;  /* see function 'genstep' in file 'lgc.c' */
  stringtable strt;  /* hash table for strings */
  TValue l_registry;
  TValue nilvalue;  /* a nil value */
  unsigned int seed;  /* randomized seed for hashes */
  unsigned char currentwhite;
  unsigned char gcstate;  /* state of garbage collector */
  unsigned char gckind;  /* kind of GC running */
  unsigned char genminormul;  /* control for minor generational collections */
  unsigned char genmajormul;  /* control for major generational collections */
  unsigned char gcrunning;  /* true if GC is running */
  unsigned char gcemergency;  /* true if this is an emergency collection */
  unsigned char gcpause;  /* size of pause between successive GCs */
  unsigned char gcstepmul;  /* GC "speed" */
  unsigned char gcstepsize;  /* (log2 of) GC granularity */
  void *allgc;  /* list of all collectable objects */
  void **sweepgc;  /* current position of sweep in list */
  void *finobj;  /* list of collectable objects with finalizers */
  void *gray;  /* list of gray objects */
  void *grayagain;  /* list of objects to be traversed atomically */
  void *weak;  /* list of tables with weak values */
  void *ephemeron;  /* list of ephemeron tables (weak keys) */
  void *allweak;  /* list of all-weak tables */
  void *tobefnz;  /* list of userdata to be GC */
  void *fixedgc;  /* list of objects not to be collected */
  /* fields for generational collector */
  void *survival;  /* start of objects that survived one GC cycle */
  void *old1;  /* start of old1 objects */
  void *reallyold;  /* objects more than one cycle old ("really old") */
  void *firstold1;  /* first OLD1 object in the list (if any) */
  void *finobjsur;  /* list of survival objects with finalizers */
  void *finobjold1;  /* list of old1 objects with finalizers */
  void *finobjrold;  /* list of really old objects with finalizers */
  struct lua_State *twups;  /* list of threads with open upvalues */
  void* panic;  /* to be called in unprotected errors */
  struct lua_State *mainthread;
  void *memerrmsg;  /* message for memory-allocation errors */
  void *tmname[25];  /* array with tag-method names */
  struct Table *mt[9];  /* metatables for basic types */
  void *strcache[STRCACHE_N][STRCACHE_M];  /* cache for strings in API */
  lua_WarnFunction warnf;  /* warning function */
  void *ud_warn;         /* auxiliary data to 'warnf' */
} global_State;

struct lua_State {
  char header[10];
  unsigned char status;
  unsigned char allowhook;
  unsigned short nci;  /* number of items in 'ci' list */
  void* top;  /* first free slot in the stack */
  global_State *l_G;
  CallInfo *ci;  /* call info for current function */
  void* stack_last;  /* end of stack (last element + 1) */
  void* stack;  /* stack base */
  void *openupval;  /* list of open upvalues in this stack */
  void *gclist;
  void *twups;  /* list of threads with open upvalues */
  void *errorJmp;  /* current error recover point */
  CallInfo base_ci;  /* CallInfo for first level (C calling Lua) */
  volatile void* hook;
  ptrdiff_t errfunc;  /* current error handling function (stack index) */
  unsigned nCcalls;  /* number of nested (non-yieldable | C)  calls */
  int oldpc;  /* last pc traced */
  int basehookcount;
  int hookcount;
  volatile int hookmask;
};

static const char luaTypes[][11] = {
    "nil",
    "boolean",
    "userdata",
    "number",
    "string",
    "table",
    "function",
    "userdata",
    "thread",
    "upvalue",
    "proto",
};