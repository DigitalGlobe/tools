VERSION 5.00
Begin VB.Form Crypt_demo 
   Caption         =   "FLEXnet Licensing - Crypt_demo"
   ClientHeight    =   5205
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7350
   LinkTopic       =   "Form1"
   ScaleHeight     =   5205
   ScaleWidth      =   7350
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox OutputWindow 
      Height          =   855
      Left            =   360
      MultiLine       =   -1  'True
      TabIndex        =   8
      Top             =   3960
      Width           =   6615
   End
   Begin VB.Frame Frame1 
      Height          =   2055
      Left            =   360
      TabIndex        =   1
      Top             =   1440
      Width           =   6615
      Begin VB.CommandButton GenKeyBtn 
         Caption         =   "Generate License..."
         Height          =   855
         Left            =   5160
         TabIndex        =   10
         Top             =   720
         Width           =   1215
      End
      Begin VB.OptionButton BothKeys 
         Caption         =   "Use Standard License Key and SIGN="
         Height          =   375
         Left            =   1680
         TabIndex        =   6
         Top             =   1440
         Width           =   3135
      End
      Begin VB.OptionButton SignOnly 
         Caption         =   "Use SIGN= Only"
         Height          =   375
         Left            =   1680
         TabIndex        =   5
         Top             =   1080
         Width           =   3135
      End
      Begin VB.OptionButton LicenseKeyOnly 
         Caption         =   "Use Standard  License Key Only"
         Height          =   375
         Left            =   1680
         TabIndex        =   4
         Top             =   720
         Value           =   -1  'True
         Width           =   3135
      End
      Begin VB.TextBox FeatureName 
         Height          =   285
         Left            =   1680
         TabIndex        =   3
         Top             =   360
         Width           =   2775
      End
      Begin VB.Label Label3 
         Height          =   375
         Left            =   360
         TabIndex        =   7
         Top             =   1200
         Width           =   1095
      End
      Begin VB.Label Label2 
         Caption         =   "Feature Name:"
         Height          =   255
         Left            =   240
         TabIndex        =   2
         Top             =   360
         Width           =   1215
      End
   End
   Begin VB.Label Label5 
      Caption         =   "This demonstration program is intended to generate licenses that are uncounted. "
      Height          =   375
      Left            =   360
      TabIndex        =   11
      Top             =   960
      Width           =   6495
   End
   Begin VB.Label Label4 
      Caption         =   "Encrypted License Output"
      Height          =   255
      Left            =   360
      TabIndex        =   9
      Top             =   3600
      Width           =   2055
   End
   Begin VB.Label Label1 
      Caption         =   "FLEXnet Licensing Visual Basic Demonstration: LC_CRYPTSTR()"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   360
      TabIndex        =   0
      Top             =   360
      Width           =   6735
   End
End
Attribute VB_Name = "Crypt_demo"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub Form_Load()
Dim ErrStr As String

i% = LP_SETUP(MY_VENDOR_NAME, MY_VENDOR_KEY1, MY_VENDOR_KEY2, MY_VENDOR_KEY3, MY_VENDOR_KEY4, MY_XOR1, MY_XOR2)
If i% <> 0 Then
 ErrStr = "Error :" + Format$(i%)
 MsgBox ErrStr, 0, "Crypt_demo"
End If
End Sub

Private Sub GenKeyBtn_Click()
Dim FeatureStr As String
Dim FeatureIn As String
Dim FeatureOut As String
Dim l As Long
Dim out_str_len As Integer
Dim err_str_len As Integer
Dim s As String
Dim ReturnStringAddress As Long

    FeatureStr = FeatureName.Text
    If FeatureStr = "" Then
        ErrStr = "Error : Feature cannot be blank. Please try again."
        MsgBox ErrStr, 0, "Crypt_demo"
        Exit Sub
    End If
    
    If LicenseKeyOnly.Value = True Then
        FeatureIn = "FEATURE " + FeatureStr + " " + MY_VENDOR_NAME + " 1.00 permanent 0 0 HOSTID=ANY"
    End If
    If SignOnly.Value = True Then
        FeatureIn = "FEATURE " + FeatureStr + " " + MY_VENDOR_NAME + " 1.00 permanent uncounted HOSTID=ANY SIGN=0"
    End If
    If BothKeys.Value = True Then
        FeatureIn = "FEATURE " + FeatureStr + " " + MY_VENDOR_NAME + " 1.00 permanent 0 0 HOSTID=ANY SIGN=0"
    End If
    
    ' this may need to be made larger depending upon size of license file
    FeatureOut = Space$(2048)
    
    out_str_len = 2047
    err_str_len = 0
    i% = LC_CRYPTSTR(FeatureIn, FeatureOut, out_str_len, vbNullString, err_str_len, MY_VENDOR_KEY5)
    
    ' Get rid of extra blanks at end of string
    FeatureOut = RTrim$(FeatureOut)
    FeatureOut = Replace(FeatureOut, Chr(10), "", 1, -1, vbBinaryCompare)
    If i% = 0 Then
        OutputWindow.Text = FeatureOut
    Else
        OutputWindow.Text = "Error :" + Format$(i%)
    End If

End Sub
