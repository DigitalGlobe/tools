VERSION 5.00
Begin VB.Form Form1 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   Caption         =   "Form1"
   ClientHeight    =   4995
   ClientLeft      =   1785
   ClientTop       =   1830
   ClientWidth     =   7740
   BeginProperty Font 
      Name            =   "MS Sans Serif"
      Size            =   8.25
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H80000008&
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   4995
   ScaleWidth      =   7740
   Begin VB.CommandButton Hostid 
      Caption         =   "Hostid"
      Height          =   615
      Left            =   1920
      TabIndex        =   8
      Top             =   1560
      Width           =   1335
   End
   Begin VB.CommandButton Exit 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Exit"
      Height          =   615
      Left            =   5040
      TabIndex        =   7
      Top             =   1560
      Width           =   1455
   End
   Begin VB.CommandButton Crypt 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Crypt"
      Height          =   615
      Left            =   240
      TabIndex        =   6
      Top             =   1560
      Width           =   1455
   End
   Begin VB.TextBox Text1 
      Appearance      =   0  'Flat
      Enabled         =   0   'False
      Height          =   2115
      Left            =   240
      MultiLine       =   -1  'True
      TabIndex        =   5
      Text            =   "LMCLIENT.frx":0000
      Top             =   2520
      Width           =   7215
   End
   Begin VB.CommandButton Heartbeat 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Heartbeat"
      Height          =   735
      Left            =   5040
      TabIndex        =   3
      Top             =   600
      Width           =   1455
   End
   Begin VB.CommandButton Checkin 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Checkin"
      Height          =   735
      Left            =   3480
      TabIndex        =   2
      Top             =   600
      Width           =   1335
   End
   Begin VB.CommandButton Checkout 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Checkout"
      Height          =   735
      Index           =   1
      Left            =   1920
      TabIndex        =   1
      Top             =   600
      Width           =   1335
   End
   Begin VB.CommandButton Init 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Init"
      Height          =   735
      Index           =   0
      Left            =   240
      TabIndex        =   0
      Top             =   600
      Width           =   1455
   End
   Begin VB.Label Label1 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "FLEXnet Licensing Visual Basic Demonstration"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000008&
      Height          =   375
      Left            =   1080
      TabIndex        =   4
      Top             =   120
      Width           =   4935
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim lp As Long

Private Sub Checkin_Click()
Dim j As String
' substitute feature for f1
LP_CHECKIN (lp)
Text1.Text = "Checked In a License"
End Sub

Private Sub Checkout_Click(Index As Integer)
Dim s As String
s = Space$(513)
' example uses f1 as feature, version 1.0, 1 user, flag 0
'lp is a Global LONG
i% = LP_CHECKOUT(LM_RESTRICTIVE + LM_RETRY_RESTRICTIVE + LM_MANUAL_HEARTBEAT, "f1", "1.0", 1, ".\license.dat", lp)

If i% = 0 Then
 Text1.Text = "Checked Out a License"
 Else
  i% = LP_ERRSTRING(lp, s, 512)
  s = RTrim$(s)
 Text1.Text = "Error :" + s
End If


End Sub

Private Sub Command1_Click()

End Sub

Private Sub Crypt_Click()
Dim j As String
Dim k As String
Dim l As Long
Dim out_str_len As Integer
Dim err_str_len As Integer
Dim s As String
Dim ReturnStringAddress As Long
Dim Vendor_Key5 As Long

k = Space$(2048) ' this may need to be made larger depending upon size of license file
' Either read file of generate string to pass to lc_cryptstr
j = "FEATURE f1 demo 1.00 permanent 0 0 HOSTID=ANY"
Vendor_Key5 = &HB165DCB
out_str_len = 2047
err_str_len = 0
i% = LC_CRYPTSTR(j, k, out_str_len, vbNullString, err_str_len, Vendor_Key5)

k = RTrim$(k) ' Get rid of extra blanks at end of string

If i% = 0 Then
 Text1.Text = "Encrypted License file"
 MsgBox k, 0, "Lmcrypt"
 Open "license.dat" For Output As 1
 Print #1, k
 Close #1
 Else
 Text1.Text = "Error :" + Format$(i%)
End If
End Sub

Private Sub Exit_Click()
' Should make sure that lc_free_job is called if necessary
End
End Sub

Private Sub Form_Load()
 Text1.Text = "Unitialized"
End Sub

Private Sub Heartbeat_Click()
Dim j As String

Dim reconn As Long
Dim minutes As Long
minutes = 5
reconn = 1
i% = LP_HEARTBEAT(lp, reconn, minutes)
Text1.Text = "status:" + Str$(i%) + " Reconn:" + Str$(reconn) + " Minutes:" + Str$(minutes)

End Sub

Private Sub Hostid_Click()
Dim k As String
Dim HostidType As Long
Dim BufLen As Integer
BufLen = 2048
k = Space$(2048) ' this may need to be made larger depending upon size of license file
' Tupe 11 returns DISK SERIAL NUM
HostidType = 11
i% = LC_HOSTID(HostidType, k, BufLen)
Text1.Text = k

End Sub

Private Sub Init_Click(Index As Integer)
Dim MY_VENDOR_KEY1 As Long, MY_VENDOR_KEY2 As Long, MY_VENDOR_KEY3 As Long, MY_VENDOR_KEY4 As Long
Dim MY_VENDOR_NAME As String

' Copy ENCRYPTION_SEEDS1 and ENCRYPTION_SEEDS1 here.
'    The first value is your encryption seeds and the second value is the value of VENDOR_KEY5

Const MY_XOR1 As Long = &H87654321 Xor &HB165DCB
Const MY_XOR2 As Long = &H12345678 Xor &HB165DCB

' These values are provided to you by GLOBEtrotter Software

MY_VENDOR_KEY1 = &H8362213A
MY_VENDOR_KEY2 = &H932D7424
MY_VENDOR_KEY3 = &HEB9CE723
MY_VENDOR_KEY4 = &H4FE9F50D

MY_VENDOR_NAME = "demo"

' copy values out of lm_code.h
'remember that the encryption seeds need to be changed
'in real life to protect your security


i% = LP_SETUP(MY_VENDOR_NAME, MY_VENDOR_KEY1, MY_VENDOR_KEY2, MY_VENDOR_KEY3, MY_VENDOR_KEY4, MY_XOR1, MY_XOR2)
If i% = 0 Then
 Text1.Text = "Initialized"
 Else
 Text1.Text = "Error :" + Format$(i%)
End If

End Sub

