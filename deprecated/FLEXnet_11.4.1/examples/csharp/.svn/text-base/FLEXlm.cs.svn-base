//*****************************************************************************
//
//	    Copyright (c) 1988-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved
//	This software has been provided pursuant to a License Agreement
//	containing restrictions on its use.  This software contains
//	valuable trade secrets and proprietary information of 
//	Macrovision Corporation and is protected by law.  It may 
//	not be copied or distributed in any form or medium, disclosed 
//	to third parties, reverse engineered or used in any manner not 
//	provided for in said License Agreement except with the prior 
//	written authorization from Macrovision Corporation
//
//*****************************************************************************

using System;
using System.Runtime.InteropServices;

namespace FLEXlm_C_sharp
{
	/// <summary>
	/// Summary description for FLEXlm.
	/// </summary>
	public class FLEXlm
	{
		public const int LM_RESTRICTIVE        = 0x0001;
		public const int LM_QUEUE              = 0x0002;
		public const int LM_FAILSAFE           = 0x0003;
		public const int LM_LENIENT            = 0x0004;
		public const int LM_MANUAL_HEARTBEAT   = 0x0100;
		public const int LM_RETRY_RESTRICTIVE  = 0x0200;
		public const int LM_CHECK_BADDATE      = 0x0800;

		[DllImport("lmgr11.dll", CharSet=CharSet.Ansi)]
			public static extern int lt_checkout(int iPolicy, String FeatureName, String FeatureVersion, String LicPath);
			[DllImport("lmgr11.dll", CharSet=CharSet.Ansi)]
			public static extern void lt_checkin();
			[DllImport("lmgr11.dll", CharSet=CharSet.Ansi)]
			public static extern int lt_heartbeat();
			[DllImport("lmgr11.dll", CharSet=CharSet.Ansi)]
			public static extern char* lt_errstring();
			[DllImport("lmgr11.dll", CharSet=CharSet.Ansi)]
			public static extern void lt_perror(String szErrStr);
			[DllImport("lmgr11.dll", CharSet=CharSet.Ansi)]
			public static extern void lt_pwarn(String szErrStr);
			[DllImport("lmgr11.dll", CharSet=CharSet.Ansi)]
			public static extern char* lt_warning();

	}
}
