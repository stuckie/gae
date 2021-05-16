#ifndef _GAE_STRING_H_
#define _GAE_STRING_H_

#if !defined(MSVC)
	#define gae_sprintf(dst, size, format, ...) sprintf((char*)dst, format, __VA_ARGS__);
#else
	#define gae_sprintf(dst, size, format, ...) sprintf_s((char* const)dst, size, format, __VA_ARGS__);
#endif

#endif

