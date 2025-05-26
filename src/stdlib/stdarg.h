#ifndef STDARG_H
#define STDARG_H

typedef char* va_list;

#define VA_ALIGN(type) (((sizeof(type) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

#define va_start(ap, last_arg) (ap = (va_list)&last_arg + VA_ALIGN(last_arg))
#define va_arg(ap, type) (*(type *)((ap += VA_ALIGN(type)) - VA_ALIGN(type)))
#define va_end(ap) (ap = (va_list)0)

#endif
