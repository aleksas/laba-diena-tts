#ifndef THREADING_H_INCLUDED
#define THREADING_H_INCLUDED

#if _WIN32
	#define PS "\\"

	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>

	#define ThreadHandle HANDLE
	#define CallbackReturnType DWORD
	#define CreateNewThread(resultRef, callback, argument) *resultRef = CreateThread(NULL, 0, callback, argument, 0, NULL)
	#define JoinThread(thread, resultRef, joinResRef) \
		*joinResRef = WaitForSingleObject(thread, INFINITE);\
		GetExitCodeThread(thread, resultRef)
	#define CloseThread(handle) CloseHandle(handle)

#elif

	#define PS "/"

	#include <pthread.h>

	#define ThreadHandle pthread_t
	#define CallbackReturnType void *
	#define CreateNewThread(resultRef, callback, argument) pthread_create(resultRef, NULL, TextToSound, pDataArray[k])
	#define JoinThread(thread, resultRef, joinResRef) joinResRef = pthread_join(thread, resultRef)
	#define CloseThread(handle)	

#endif

#endif // THREADING_H_INCLUDED
