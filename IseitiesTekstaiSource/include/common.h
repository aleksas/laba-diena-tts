#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#if defined(_MSC_VER)
    //  Microsoft
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)

	#define PS "\\"
#else
	#define PS "/"

	#if defined(__GNUC__)
		//  GCC
		#define EXPORT __attribute__((visibility("default")))
		#define IMPORT
	#else
		//  do nothing and hope for the best?
		#define EXPORT
		#define IMPORT
		#pragma warning Unknown dynamic link import/export semantics.
	#endif
#endif

#ifndef BOOL
    typedef int BOOL;
#endif

#ifndef FALSE
    #define FALSE 0
    #define TRUE !FALSE
#endif

#endif //!COMMON_H_INCLUDED