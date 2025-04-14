#ifndef _STDDEF_H
#define _STDDEF_H

#ifndef __amd64__
typedef unsigned int   size_t;
typedef          int  ssize_t;
#else
typedef unsigned long  size_t;
typedef          long ssize_t;
#endif

#endif // _STDDEF_H
