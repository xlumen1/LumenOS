#ifndef _STDDEF_H
#define _STDDEF_H

// I'll be adding C++ stuff never (≈ 4 months from now probably)

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned long size_t;
#endif

#ifndef _PTRDIFF_T
#define _PTRDIFF_T
typedef long ptrdiff_t;
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#define offsetof(type, member) ((size_t)&(((type *)0)->member))

#endif