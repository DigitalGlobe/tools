Attribute VB_Name = "LM_CLIEN"
'******************************************************************************
'
	    Copyright (c) 1994-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved.
	This software has been provided pursuant to a License Agreement
	containing restrictions on its use.  This software contains
	valuable trade secrets and proprietary information of 
	Macrovision Inc and is protected by law.  It may 
	not be copied or distributed in any form or medium, disclosed 
	to third parties, reverse engineered or used in any manner not 
	provided for in said License Agreement except with the prior 
	written authorization from Macrovision Corporation.
' *****************************************************************************/
' *
' *  Module: lmvbclient.h v1.0.0.0
' *
' *  Description:    FLEXnet Licensing definitions.
' *
' *  Blane Eisenberg
' *  3/18/97
' *
' *  Last changed:  3/18/97
' *
' *
Global Const LM_RETRY_RESTRICTIVE = 512
Global Const LM_MANUAL_HEARTBEAT = 256
Global Const LM_CHECK_BADDATE = 2048

Global Const LM_RESTRICTIVE = 1
Global Const LM_QUEUE = 2
Global Const LM_FAILSAFE = 3
Global Const LM_LENIENT = 4

Global Const LM_CRYPT_ONLY = 1
Global Const LM_CRYPT_FORCE = 2
Global Const LM_CRYPT_IGNORE_FEATNAME_ERRS = 4

Global Const HOSTID_ETHER = 2
Global Const HOSTID_USER = 4
Global Const HOSTID_DISPLAY = 5
Global Const HOSTID_HOSTNAME = 6
Global Const HOSTID_FLEXID1_KEY = 10
Global Const HOSTID_DISK_SERIAL_NUM = 11
Global Const HOSTID_INTERNET = 12
Global Const HOSTID_FLEXID2_KEY = 14

Declare Function LC_CRYPTSTR Lib "LMGR11.DLL" (ByVal ins As Any, ByVal ReturnString As String, return_length As Integer, ByVal b As String, errorlength As Integer, ByVal vendor5 As Long) As Integer
Declare Function LC_HOSTID Lib "LMGR11.DLL" (ByVal ins As Long, ByVal ReturnString As String, retlen As Integer) As Integer
Declare Function LP_CHECKOUT Lib "LMGR11.DLL" (ByVal Policy As Long, ByVal FeatureName As String, ByVal Version As String, ByVal nlic As Long, ByVal LicensePath As String, lpH As Long) As Integer
Declare Function LP_CHECKIN Lib "LMGR11.DLL" (lpH As Long) As Integer
Declare Function LP_ERRSTRING Lib "LMGR11.DLL" (lpH As Long, ByVal ErrString As String, ErrStringLenght As Integer) As Integer
Declare Function LP_HEARTBEAT Lib "LMGR11.DLL" (lpH As Long, reconn As Long, minutes As Long) As Integer
Declare Function LP_SETUP Lib "LMGR11.DLL" (ByVal Name As String, VendorCode1 As Long, VendorCode2 As Long, VendorCode3 As Long, VendorCode4 As Long, XorValue1 As Long, XorValue2 As Long) As Integer





