
#ifndef PLAT_H
#define PLAT_H

#ifdef _WIN32
#define PLAT_GL14
#define PLAT_WIN
#endif

#ifdef _WIN64
#define PLAT_GL14
#define PLAT_WIN
#endif

#if __APPLE__

#include "TargetConditionals.h"
#if TARGET_OS_MAC
#define PLAT_GL14
#define PLAT_MAC
#endif
#if TARGET_OS_IPHONE
#define PLAT_IOS
#define PLAT_IPHONE
#define PLAT_MOBILE
#define PLAT_GLES20
#undef PLAT_GL14
#endif
#if TARGET_OS_IPAD
#define PLAT_IOS
#define PLAT_IPAD
#define PLAT_MOBILE
#define PLAT_GLES20
#undef PLAT_GL14
#endif

#endif	//__APPLE__

#if defined( __GNUC__ )
//#define PLAT_LINUX
#endif
#if defined( __linux__ )
#define PLAT_LINUX
#define PLAT_GL14
#endif
#if defined ( __linux )
#define PLAT_LINUX
#define PLAT_GL14
#endif

#if _WIN32 || _WIN64
#if _WIN64
#define BUILD_64BIT
#define BUILD_SIZEOF_UL	4
#else
#define BUILD_32BIT
#define BUILD_SIZEOF_UL	4
#endif
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
#define BUILD_64BIT
#define BUILD_SIZEOF_UL	4
#else
#define BUILD_32BIT
#define BUILD_SIZEOF_UL	4
#endif
#endif

#ifdef PLAT_MAC

#ifdef __LP64__
#define BUILD_64BIT	//64-bit Intel or PPC
#define BUILD_SIZEOF_UL	4
#else
#define BUILD_32BIT	//32-bit Intel, PPC or ARM
#define BUILD_SIZEOF_UL	4
#endif
#ifdef __x86_64__
#undef BUILD_32_BIT
#define BUILD_64BIT  //64-bit intel
#define BUILD_SIZEOF_UL	8
#endif
#ifdef __i386__
#undef BUILD_64BIT
#define BUILD_32BIT  //32-bit intel
#define BUILD_SIZEOF_UL	8
#endif

#endif	//PLAT_MAC

#endif