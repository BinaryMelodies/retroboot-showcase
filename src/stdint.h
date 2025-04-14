#ifndef _STDINT_H
#define _STDINT_H

typedef unsigned char      uint8_t;
typedef   signed char       int8_t;
typedef unsigned short     uint16_t;
typedef          short      int16_t;

#ifdef __ia16__
typedef unsigned long      uint32_t;
typedef          long       int32_t;
#else
typedef unsigned int       uint32_t;
typedef          int        int32_t;
#endif

#ifndef __amd64__
typedef unsigned long long uint64_t;
typedef          long long  int64_t;
#else
typedef unsigned long      uint64_t;
typedef          long       int64_t;
#endif

#endif // _STDINT_H
