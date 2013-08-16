#ifndef __UTIL_H__
#define __UTIL_H__ 1

#ifdef __GNUC__
#	define UNUSED(x) __attribute__((__unused__)) x
#elif defined(_MSC_VER)
#	define UNUSED(x) __pragma(warning(suppress:4100)) x
#else
#	define UNUSED(x) x
#endif

#endif // __UTIL_H__
