Attribute VB_Name = "vb_dll_interface"
'/************************************************************************
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
'
' ************************************************************************/
'

' HOSTID values as declared in lmclient.h
'
Global Const HOSTID_DEFAULT = -1
Global Const HOSTID_ETHER = 2        ' Ethernet card address
Global Const HOSTID_DISPLAY = 5      ' Display name
Global Const HOSTID_HOSTNAME = 6     ' Hostname
Global Const HOSTID_HDSERNUM = 11    ' Hard drive serial number
Global Const HOSTID_INTERNET = 12    ' IP address
Global Const HOSTID_FLEXID6 = 23     ' FLEXid 6 dongle
Global Const HOSTID_FLEXID7 = 10     ' FLEXid 7 dongle
Global Const HOSTID_FLEXID8 = 14     ' FLEXid 8 dongle
Global Const HOSTID_FLEXID9 = 15     ' FLEXid 9 dongle

' CRYPTSTR constants in lmclient.h
'
Global Const LM_CRYPT_ONLY = &H1                    ' only return signature for first feature
Global Const LM_CRYPT_FORCE = &H2                   ' recompute signature for every line
Global Const LM_CRYPT_IGNORE_FEATNAME_ERRS = &H4    ' no warnings returned about invalid feature names
Global Const LM_CRYPT_DECIMAL = &H10                ' output license in decimal format

' DLL interface declarations
'
Declare Function lp_setup Lib "vb_dll.dll" (ByVal LicDefault As String) As Integer
Declare Function lp_checkout Lib "vb_dll.dll" (ByVal FeatureName As String, ByVal featurever As String, ByVal liccount As Integer) As Integer
Declare Function lp_checkin Lib "vb_dll.dll" (ByVal FeatureName As String) As Integer
Declare Function lp_shutdown Lib "vb_dll.dll" () As Integer
Declare Function lp_cryptstr Lib "vb_dll.dll" (ByVal InLic As String, ByVal OutLic As String, ByVal BufLen As Integer, ByVal Flag As Integer) As Integer
Declare Function lp_errstring Lib "vb_dll.dll" (ByVal ErrorStr As String, ByVal BufLen As Integer) As Integer
Declare Function lp_heartbeat Lib "vb_dll.dll" (ByVal NumReconnect As Integer, ByVal NumMinute As Integer) As Integer
Declare Function lp_hostid Lib "vb_dll.dll" (ByVal HostType As Integer, ByVal hostidstr As String, ByVal BufLen As Integer) As Integer
