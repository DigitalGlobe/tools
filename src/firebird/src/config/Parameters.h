
#ifndef STR_PARAMETER
#define STR_PARAMETER(name, value)	static const char* name = #name; static const char* name##Value = value;
#define INT_PARAMETER(name, value)  static const char* name = #name; static int name##Value = value;
#define BOOL_PARAMETER(name, value) static const char* name = #name; static bool name##Value = value;
#endif

	STR_PARAMETER (RootDirectory,		  0)
	INT_PARAMETER (SortMemBlockSize,	  1048576)		// bytes
#ifdef SUPERSERVER
	INT_PARAMETER (SortMemUpperLimit,	  67108864)	// bytes
#elif defined(WIN_NT) // win32 CS
	INT_PARAMETER (SortMemUpperLimit,	  8388608)		// bytes
#else // non-win32 CS
	INT_PARAMETER (SortMemUpperLimit,	  0)			// bytes
#endif
	BOOL_PARAMETER (RemoteFileOpenAbility,  false)
	INT_PARAMETER (GuardianOption,		  1)
	INT_PARAMETER (CpuAffinityMask,		  1)
	BOOL_PARAMETER (OldParameterOrdering,	  false)
	INT_PARAMETER (TcpRemoteBufferSize,	  8192)		// bytes
	BOOL_PARAMETER (TcpNoNagle,			  false)
	INT_PARAMETER (IpcMapSize,			  4096)		// bytes
#ifdef SUPERSERVER
	INT_PARAMETER (DefaultDbCachePages,	  2048)		// pages
#else
	INT_PARAMETER (DefaultDbCachePages,	  75)			// pages
#endif
	INT_PARAMETER (ConnectionTimeout,	  180)			// seconds
	INT_PARAMETER (DummyPacketInterval,	  0)			// seconds
#if defined(WIN_NT) && !defined(SUPERSERVER)
	INT_PARAMETER (LockMemSize,			  1048576)		// bytes
#else
	INT_PARAMETER (LockMemSize,			  262144)		// bytes
#endif
	INT_PARAMETER (LockSemCount,			  32)			// semaphores
	INT_PARAMETER (LockSignal,			  16)			// signal #
	BOOL_PARAMETER (LockGrantOrder,		  true)
	INT_PARAMETER (LockHashSlots,		  101)			// slots
	INT_PARAMETER (LockAcquireSpins,		  0)
	INT_PARAMETER (EventMemSize,			  65536)		// bytes
	INT_PARAMETER (DeadlockTimeout,		  10)			// seconds
	INT_PARAMETER (SolarisStallValue,	  60)			// seconds
	BOOL_PARAMETER (TraceMemoryPools,		  false)		// for internal use only
	INT_PARAMETER (PrioritySwitchDelay,	  100)			// milliseconds
	INT_PARAMETER (DeadThreadsCollection,  50)			// number of PrioritySwitchDelay cycles before dead threads collection
	INT_PARAMETER (PriorityBoost,		  5)			// ratio oh high- to low-priority thread ticks in jrd.cpp
#ifdef FB_SERVICE_NAME
	STR_PARAMETER (RemoteServiceName,	  FB_SERVICE_NAME)
	INT_PARAMETER (RemoteServicePort,	  0)
	STR_PARAMETER (RemotePipeName,		  FB_PIPE_NAME)
	STR_PARAMETER (IpcName,				  FB_IPC_NAME)
#endif
#ifdef WIN_NT
	INT_PARAMETER (MaxUnflushedWrites,	  100)
	INT_PARAMETER (MaxUnflushedWriteTime,  5)
#else
	INT_PARAMETER (MaxUnflushedWrites,	  -1)
	INT_PARAMETER (MaxUnflushedWriteTime,  -1)
#endif
	INT_PARAMETER (ProcessPriorityLevel,	  0)
	BOOL_PARAMETER (CreateInternalWindow,	  true)
	BOOL_PARAMETER (CompleteBooleanEvaluation,  false)
	INT_PARAMETER (RemoteAuxPort,		  0)
	STR_PARAMETER (RemoteBindAddress,	  0)
	STR_PARAMETER (ExternalFileAccess,	  "None")	// location(s) of external files for tables
	STR_PARAMETER (DatabaseAccess,		  "Full")	// location(s) of databases
	STR_PARAMETER (UdfAccess,			  "Restrict UDF")	// location(s) of UDFs
	STR_PARAMETER (TempDirectories,		  0)
	//INT_PARAMETER (TraceDSQL,			  0}			// bitmask
	STR_PARAMETER (LockFileName,		  0)
	STR_PARAMETER (SecurityDatabase,	  "None")
	BOOL_PARAMETER (DatabaseFileShared,  false)
	INT_PARAMETER (TraceFlags,			 0)
	STR_PARAMETER (SecurityManager,		"SecurityDb")
	INT_PARAMETER (CommitInterval,		200)

