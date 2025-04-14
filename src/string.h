#ifndef _STRING_H
#define _STRING_H

#include "stddef.h"
#if __ia16__ || __i386__ || __amd64__
# include "i86.h"
#endif

static inline size_t strlen(const char * s)
{
	size_t len;
	for(len = 0; s[len] != '\0'; len++)
		;
	return len;
}

static inline void * memset(void * s, int c, size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		((char *)s)[i] = c;
	}
	return s;
}

static inline void * memcpy(void * dest, const void * src, size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		((char *)dest)[i] = ((const char *)src)[i];
	}
	return dest;
}

static inline void * memmove(void * dest, const void * src, size_t n)
{
	if(dest < src && src < dest + n)
	{
		for(size_t i = n; i > 0; i--)
		{
			((char *)dest)[i - 1] = ((const char *)src)[i - 1];
		}
		return dest;
	}
	else
	{
		return memcpy(dest, src, n);
	}
}

static inline int memcmp(const void * s1, const void * s2, size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		unsigned char c1 = ((unsigned char *)s1)[i];
		unsigned char c2 = ((unsigned char *)s2)[i];
		if(c1 != c2)
			return c1 - c2;
	}
	return 0;
}

#ifdef __ia16__
static inline size_t _fstrlen(const char far * s)
{
	size_t len;
	for(len = 0; s[len] != '\0'; len++)
		;
	return len;
}

static inline void far * _fmemset(void far * s, int c, size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		((char far *)s)[i] = c;
	}
	return s;
}

static inline void far * _fmemcpy(void far * dest, const void far * src, size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		((char far *)dest)[i] = ((const char far *)src)[i];
	}
	return dest;
}

static inline void far * _fmemmove(void far * dest, const void far * src, size_t n)
{
	if(dest < src && src < dest + n)
	{
		for(size_t i = n; i > 0; i--)
		{
			((char far *)dest)[i - 1] = ((const char far *)src)[i - 1];
		}
		return dest;
	}
	else
	{
		return _fmemcpy(dest, src, n);
	}
}

static inline int _fmemcmp(const void far * s1, const void far * s2, size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		unsigned char c1 = ((unsigned char far *)s1)[i];
		unsigned char c2 = ((unsigned char far *)s2)[i];
		if(c1 != c2)
			return c1 - c2;
	}
	return 0;
}
#endif

#endif // _STRING_H
