#ifndef _OPENTHREAD_EXPORTS_H_
#define _OPENTHREAD_EXPORTS_H_


#ifndef WIN32
	#define OPENTHREAD_EXPORT_DIRECTIVE
#else
	#ifdef OPENTHREADS_EXPORTS
		#define OPENTHREAD_EXPORT_DIRECTIVE __declspec(dllexport)
	#else
		#define OPENTHREAD_EXPORT_DIRECTIVE __declspec(dllimport)

		#ifdef _MSC_VER
		#ifdef _DEBUG
			#pragma comment(lib ,"OpenThreadsWin32d")
		#else 
			#pragma comment(lib, "OpenThreadsWin32")
		#endif
		#endif
	#endif
#endif

#endif


