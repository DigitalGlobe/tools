VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "FLEXnet Licensing TRL Demo for Visual Basic"
   ClientHeight    =   7440
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6165
   LinkTopic       =   "Form1"
   ScaleHeight     =   7440
   ScaleWidth      =   6165
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame6 
      Caption         =   "Crypt a License"
      Height          =   2775
      Left            =   120
      TabIndex        =   13
      Top             =   3720
      Width           =   5895
      Begin VB.CommandButton CryptBtn 
         Caption         =   "Encrypt License"
         Height          =   495
         Left            =   3360
         TabIndex        =   16
         Top             =   2160
         Width           =   2295
      End
      Begin VB.TextBox LicAfter 
         Height          =   885
         Left            =   240
         MultiLine       =   -1  'True
         ScrollBars      =   1  'Horizontal
         TabIndex        =   15
         Top             =   1080
         Width           =   5415
      End
      Begin VB.TextBox LicBefore 
         Height          =   285
         Left            =   240
         TabIndex        =   14
         Text            =   "Feature f1 demo 1.0 permanent uncounted HOSTID=ANY SIGN=0"
         Top             =   480
         Width           =   5415
      End
      Begin VB.Label Label2 
         Caption         =   "Encrypted Feature line:"
         Height          =   255
         Left            =   240
         TabIndex        =   18
         Top             =   840
         Width           =   1815
      End
      Begin VB.Label Label1 
         Caption         =   "Feature line to encrypt:"
         Height          =   255
         Left            =   240
         TabIndex        =   17
         Top             =   240
         Width           =   1935
      End
   End
   Begin VB.Frame Frame5 
      Caption         =   "HOSTID"
      Height          =   1335
      Left            =   120
      TabIndex        =   8
      Top             =   2280
      Width           =   5895
      Begin VB.TextBox Text2 
         BackColor       =   &H8000000B&
         Enabled         =   0   'False
         Height          =   285
         Left            =   240
         TabIndex        =   11
         Top             =   840
         Width           =   5415
      End
      Begin VB.CommandButton Get_Hostid_btn 
         Caption         =   "Get HOSTID"
         Height          =   375
         Left            =   2400
         TabIndex        =   10
         Top             =   360
         Width           =   3255
      End
      Begin VB.ComboBox hostid_combo 
         Height          =   315
         ItemData        =   "vb_dll_client.frx":0000
         Left            =   240
         List            =   "vb_dll_client.frx":0002
         TabIndex        =   9
         Text            =   "HOSTID_DEFAULT"
         Top             =   360
         Width           =   2055
      End
   End
   Begin VB.Frame Frame3 
      Caption         =   "Feature Version"
      Height          =   735
      Left            =   3840
      TabIndex        =   5
      Top             =   360
      Width           =   2055
      Begin VB.TextBox FeatureVersion 
         Height          =   285
         Left            =   240
         TabIndex        =   6
         Text            =   "1.0"
         Top             =   240
         Width           =   1575
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "Feature Name"
      Height          =   735
      Left            =   240
      TabIndex        =   3
      Top             =   360
      Width           =   3495
      Begin VB.TextBox FeatureName 
         Height          =   285
         Left            =   240
         TabIndex        =   4
         Text            =   "f1"
         Top             =   240
         Width           =   3015
      End
   End
   Begin VB.CommandButton fcheckin 
      Caption         =   "Checkin Feature"
      Height          =   615
      Left            =   3720
      TabIndex        =   1
      Top             =   1320
      Width           =   2175
   End
   Begin VB.CommandButton fcheckout 
      Caption         =   "Checkout Feature"
      Height          =   615
      Left            =   240
      TabIndex        =   0
      Top             =   1320
      Width           =   2415
   End
   Begin VB.Frame Frame4 
      Height          =   2055
      Left            =   120
      TabIndex        =   7
      Top             =   120
      Width           =   5895
   End
   Begin VB.Frame Frame1 
      Caption         =   "Status"
      Height          =   735
      Left            =   120
      TabIndex        =   2
      Top             =   6600
      Width           =   5895
      Begin VB.Label StatusLabel 
         Height          =   375
         Left            =   120
         TabIndex        =   12
         Top             =   240
         Width           =   5535
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub CryptBtn_Click()
Dim OutStr As String
Dim InString As String
Dim iReturn As Integer
Dim ErrString As String
Dim StrLength As Integer

    OutStr = Space$(2048)
    InString = Space$(2048)
    If (LicBefore = "") Then
        MsgBox "License string must not be blank. Please try again."
        Exit Sub
    End If
    InString = LicBefore.Text
    iReturn = lp_cryptstr(InString, OutStr, StrLength, LM_CRYPT_FORCE)
    If (iReturn = 0) Then
        StatusLabel.Caption = "CRYPT STRING successful."
        LicAfter.Text = OutStr
    Else
        ErrString = Space$(2048)
        i% = lp_errstring(ErrString, StrLength)
        StatusLabel.Caption = "CRYPT STRING failed: " + Str(iReturn)
        LicAfter.Text = ""
        MsgBox (ErrString)
    End If
        
End Sub

Private Sub fcheckin_Click()
    If Len(FeatureName.Text) = 0 Then
        MsgBox "Feature name must not be blank. Please try again."
        Exit Sub
    End If
    lp_checkin (FeatureName.Text)
    StatusLabel.Caption = "Checkin License Successful."
End Sub

Private Sub fcheckout_Click()
Dim iReturn As Integer
Dim ErrString As String
Dim StrLength As Integer

    If Len(FeatureName.Text) = 0 Then
        MsgBox "Feature name must not be blank. Please try again."
        Exit Sub
    End If
     If Len(FeatureVersion.Text) = 0 Then
        MsgBox "Feature version must not be blank. Please try again."
        Exit Sub
    End If
   
    iReturn = lp_checkout(FeatureName.Text, FeatureVersion.Text, 1)
    If iReturn = 0 Then
        StatusLabel.Caption = "Checkout License Successful."
    Else
        ErrString = Space$(2048)
        i% = lp_errstring(ErrString, StrLength)
        StatusLabel.Caption = "Checkout failed: " + Str(iReturn)
        ErrString = Space$(2048)
        i% = lp_errstring(ErrString, StrLength)
        MsgBox (ErrString)
    End If

End Sub

Private Sub Form_Initialize()
Dim ErrString As String
Dim StrLength As Integer
    
    i% = lp_setup(".")
    If (i% = 0) Then
        StatusLabel.Caption = "Initialize successful."
      Else
        StatusLabel.Caption = "Initialize failed. " + Str(i%)
        MsgBox (ErrString)
    End If
    ' Initialize hostid combobox
    hostid_combo.AddItem ("HOSTID_DEFAULT")
    hostid_combo.AddItem ("HOSTID_ETHER")
    hostid_combo.AddItem ("HOSTID_DISPLAY")
    hostid_combo.AddItem ("HOSTID_HOSTNAME")
    hostid_combo.AddItem ("HOSTID_HDSERNUM")
    hostid_combo.AddItem ("HOSTID_INTERNET")
    hostid_combo.AddItem ("HOSTID_FLEXID6")
    hostid_combo.AddItem ("HOSTID_FLEXID7")
    hostid_combo.AddItem ("HOSTID_FLEXID8")
    hostid_combo.AddItem ("HOSTID_FLEXID9")
    
End Sub


Private Sub Form_Terminate()
    lp_shutdown
End Sub

Private Sub Get_Hostid_btn_Click()
Dim hostidstr As String
Dim IDLen As Integer
Dim iReturn As Integer
Dim ErrString As String
Dim StrLength As Integer

    hostidstr = Space$(2048)
    IDLen = 2048
    If (hostid_combo.Text = "HOSTID_DEFAULT") Then
        iType = HOSTID_DEFAULT
    End If
    If (hostid_combo.Text = "HOSTID_ETHER") Then
        iType = HOSTID_ETHER
    End If
    If (hostid_combo.Text = "HOSTID_DISPLAY") Then
        iType = HOSTID_DISPLAY
    End If
    If (hostid_combo.Text = "HOSTID_HOSTNAME") Then
        iType = HOSTID_HOSTNAME
    End If
    If (hostid_combo.Text = "HOSTID_HDSERNUM") Then
        iType = HOSTID_HDSERNUM
    End If
    If (hostid_combo.Text = "HOSTID_INTERNET") Then
        iType = HOSTID_INTERNET
    End If
    If (hostid_combo.Text = "HOSTID_FLEXID6") Then
        iType = HOSTID_FLEXID6
    End If
    If (hostid_combo.Text = "HOSTID_FLEXID7") Then
        iType = HOSTID_FLEXID7
    End If
    If (hostid_combo.Text = "HOSTID_FLEXID8") Then
        iType = HOSTID_FLEXID8
    End If
    If (hostid_combo.Text = "HOSTID_FLEXID9") Then
        iType = HOSTID_FLEXID9
    End If

    iReturn = lp_hostid(iType, hostidstr, IDLen)
    Text2.Text = hostidstr
    
    If (iReturn = 0) Then
        StatusLabel.Caption = "HOSTID Successful."
    Else
        StatusLabel.Caption = "HOSTID Failed: " + Str(iReturn)
        ErrString = Space$(2048)
        i% = lp_errstring(ErrString, StrLength)
        MsgBox (ErrString)

    End If
    
End Sub

