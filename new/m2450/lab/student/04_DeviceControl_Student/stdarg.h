typedef char *va_list;

#define __va_promote(type)  (((sizeof(type) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_start(ap, last)      (ap = ((va_list)&(last) + __va_promote(last)))
#define va_arg(ap, type)        ((type *)(ap += sizeof(type) < sizeof(int) ? (abort(), 0) : sizeof(type)))[-1]
#define va_end(ap)
