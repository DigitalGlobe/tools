'*****************************************************************************
'
'	    Copyright (c) 1988-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved
'	This software has been provided pursuant to a License Agreement
'	containing restrictions on its use.  This software contains
'	valuable trade secrets and proprietary information of 
'	Macrovision Corporation and is protected by law.  It may 
'	not be copied or distributed in any form or medium, disclosed 
'	to third parties, reverse engineered or used in any manner not 
'	provided for in said License Agreement except with the prior 
'	written authorization from Macrovision Corporation
'
'*****************************************************************************

Public Class FLEXlm_dll

    Public Const LM_RESTRICTIVE As Integer = &H1
    Public Const LM_QUEUE As Integer = &H2
    Public Const LM_FAILSAFE As Integer = &H3
    Public Const LM_LENIENT As Integer = &H4
    Public Const LM_MANUAL_HEARTBEAT As Integer = &H100
    Public Const LM_RETRY_RESTRICTIVE As Integer = &H200
    Public Const LM_CHECK_BADDATE As Integer = &H800

    Public Declare Ansi Function VB_CHECKOUT Lib "lmgr11.dll" _
    Alias "lt_checkout" (Byval policy as Integer, _
                         ByVal fname As String, _
                         ByVal fver As String, _
                         ByVal licpath As String) As Integer

    Public Declare Ansi Function VB_CHECKIN Lib "lmgr11.dll" _
    Alias "lt_checkin" () As Integer

    Public Declare Ansi Function VB_HEARTBEAT Lib "lmgr11.dll" _
    Alias "lt_heartbeat" () As Integer
    Public Declare Ansi Function VB_ERRSTRING Lib "lmgr11.dll" _
    Alias "lt_errstring" () As String
    Public Declare Ansi Function VB_PERROR Lib "lmgr11.dll" _
    Alias "lt_perror" (ByVal errstr as String) As Integer
    Public Declare Ansi Function VB_PWARN Lib "lmgr11.dll" _
    Alias "lt_pwarn" (ByVal errstr as String) As Integer
    Public Declare Ansi Function VB_WARNING Lib "lmgr11.dll" _
    Alias "lt_warning" () As String

End Class
