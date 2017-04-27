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

Public Class Form1
    Inherits System.Windows.Forms.Form

#Region " Windows Form Designer generated code "

    Public Sub New()
        MyBase.New()

        'This call is required by the Windows Form Designer.
        InitializeComponent()

        'Add any initialization after the InitializeComponent() call

    End Sub

    'Form overrides dispose to clean up the component list.
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)

        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents Feature As System.Windows.Forms.TextBox
    Friend WithEvents Status As System.Windows.Forms.TextBox
    Friend WithEvents Checkout As System.Windows.Forms.Button
    Friend WithEvents Checkin As System.Windows.Forms.Button
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents FeatureVersion As System.Windows.Forms.TextBox
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Feature = New System.Windows.Forms.TextBox()
        Me.Status = New System.Windows.Forms.TextBox()
        Me.Checkout = New System.Windows.Forms.Button()
        Me.Checkin = New System.Windows.Forms.Button()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.FeatureVersion = New System.Windows.Forms.TextBox()
        Me.SuspendLayout()
        '
        'Label1
        '
        Me.Label1.Location = New System.Drawing.Point(8, 16)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(80, 23)
        Me.Label1.TabIndex = 0
        Me.Label1.Text = "Feature Name:"
        '
        'Label2
        '
        Me.Label2.Location = New System.Drawing.Point(8, 96)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(40, 23)
        Me.Label2.TabIndex = 1
        Me.Label2.Text = "Status:"
        '
        'Feature
        '
        Me.Feature.Location = New System.Drawing.Point(88, 16)
        Me.Feature.Name = "Feature"
        Me.Feature.Size = New System.Drawing.Size(120, 20)
        Me.Feature.TabIndex = 0
        Me.Feature.Text = ""
        '
        'Status
        '
        Me.Status.Location = New System.Drawing.Point(48, 96)
        Me.Status.Multiline = True
        Me.Status.Name = "Status"
        Me.Status.ReadOnly = True
        Me.Status.Size = New System.Drawing.Size(352, 24)
        Me.Status.TabIndex = 4
        Me.Status.Text = ""
        '
        'Checkout
        '
        Me.Checkout.Location = New System.Drawing.Point(48, 56)
        Me.Checkout.Name = "Checkout"
        Me.Checkout.Size = New System.Drawing.Size(128, 23)
        Me.Checkout.TabIndex = 2
        Me.Checkout.Text = "Check&out Feature"
        '
        'Checkin
        '
        Me.Checkin.Location = New System.Drawing.Point(232, 56)
        Me.Checkin.Name = "Checkin"
        Me.Checkin.Size = New System.Drawing.Size(128, 23)
        Me.Checkin.TabIndex = 3
        Me.Checkin.Text = "Check&in Feature"
        '
        'Label3
        '
        Me.Label3.Location = New System.Drawing.Point(224, 16)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(88, 23)
        Me.Label3.TabIndex = 6
        Me.Label3.Text = "Feature Version:"
        '
        'FeatureVersion
        '
        Me.FeatureVersion.Location = New System.Drawing.Point(312, 16)
        Me.FeatureVersion.Name = "FeatureVersion"
        Me.FeatureVersion.Size = New System.Drawing.Size(88, 20)
        Me.FeatureVersion.TabIndex = 1
        Me.FeatureVersion.Text = "1.00"
        '
        'Form1
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
        Me.ClientSize = New System.Drawing.Size(416, 141)
        Me.Controls.AddRange(New System.Windows.Forms.Control() {Me.FeatureVersion, Me.Label3, Me.Checkin, Me.Checkout, Me.Status, Me.Feature, Me.Label2, Me.Label1})
        Me.Name = "Form1"
        Me.Text = "FLEXnet Licensing using VB .NET"
        Me.ResumeLayout(False)

    End Sub

#End Region

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
    End Sub

    Private Sub Form1_Closed(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Closed

    End Sub

    Private Sub Checkout_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Checkout.Click
        Dim ret As Integer
        Status.Text = ""
        If Len(Feature.Text) = 0 Then
            MsgBox("Feature name must not be blank. Please try again.")
            Exit Sub
        End If
        If Len(FeatureVersion.Text) = 0 Then
            MsgBox("Feature Version name must not be blank. Please try again.")
            Exit Sub
        End If

        Status.Text = "Attempting to checkout feature: " + Feature.Text
        ret = FLEXlm_dll.VB_CHECKOUT(FLEXlm_dll.LM_RESTRICTIVE, Feature.Text, FeatureVersion.Text, ".")
        If ret <> 0 Then
            Status.Text = "Error " + Str(ret) + " attempting to CHECKOUT feature " + Feature.Text
        Else
            Status.Text = "Checkout successful"
        End If
    End Sub

    Private Sub Checkin_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Checkin.Click
        Status.Text = "Attempting to checkin feature: " + Feature.Text
        FLEXlm_dll.VB_CHECKIN()
        Status.Text = "Checkin successful"
    End Sub
End Class
