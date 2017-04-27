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
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace FLEXlm_C_sharp
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.TextBox Feature;
		private System.Windows.Forms.Button CheckoutBtn;
		private System.Windows.Forms.Button CheckinBtn;
		private System.Windows.Forms.TextBox Status;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.Feature = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.CheckoutBtn = new System.Windows.Forms.Button();
			this.CheckinBtn = new System.Windows.Forms.Button();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.Status = new System.Windows.Forms.TextBox();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// Feature
			// 
			this.Feature.Location = new System.Drawing.Point(88, 24);
			this.Feature.Name = "Feature";
			this.Feature.Size = new System.Drawing.Size(176, 20);
			this.Feature.TabIndex = 0;
			this.Feature.Text = "";
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(8, 24);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(80, 23);
			this.label1.TabIndex = 1;
			this.label1.Text = "Feature Name:";
			// 
			// CheckoutBtn
			// 
			this.CheckoutBtn.Location = new System.Drawing.Point(40, 72);
			this.CheckoutBtn.Name = "CheckoutBtn";
			this.CheckoutBtn.Size = new System.Drawing.Size(104, 23);
			this.CheckoutBtn.TabIndex = 2;
			this.CheckoutBtn.Text = "Checkout Feature";
			this.CheckoutBtn.Click += new System.EventHandler(this.CheckoutBtn_Click);
			// 
			// CheckinBtn
			// 
			this.CheckinBtn.Location = new System.Drawing.Point(160, 72);
			this.CheckinBtn.Name = "CheckinBtn";
			this.CheckinBtn.Size = new System.Drawing.Size(104, 23);
			this.CheckinBtn.TabIndex = 3;
			this.CheckinBtn.Text = "Checkin Feature";
			this.CheckinBtn.Click += new System.EventHandler(this.CheckinBtn_Click);
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.AddRange(new System.Windows.Forms.Control[] {
																					this.Status});
			this.groupBox1.Location = new System.Drawing.Point(8, 120);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(272, 48);
			this.groupBox1.TabIndex = 5;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Status";
			// 
			// Status
			// 
			this.Status.Location = new System.Drawing.Point(16, 16);
			this.Status.Name = "Status";
			this.Status.ReadOnly = true;
			this.Status.Size = new System.Drawing.Size(240, 20);
			this.Status.TabIndex = 0;
			this.Status.Text = "";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(292, 181);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.groupBox1,
																		  this.CheckinBtn,
																		  this.CheckoutBtn,
																		  this.label1,
																		  this.Feature});
			this.Name = "Form1";
			this.Text = "FLEXnet Licensing using C#";
			this.Load += new System.EventHandler(this.Form1_Load);
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}

		private void CheckoutBtn_Click(object sender, System.EventArgs e)
		{
		int iRet;
			iRet = FLEXlm.lt_checkout(FLEXlm.LM_RESTRICTIVE, Feature.Text, "1.00", ".");
			if (iRet != 0)
			{

				Status.Text = "Checkout error. Error #: " + iRet;
			}
			else
				Status.Text = "Checkout Success.";
		}

		private void CheckinBtn_Click(object sender, System.EventArgs e)
		{
			FLEXlm.lt_checkin();		
			Status.Text = "Checkin Success.";
		}

		private void Form1_Load(object sender, System.EventArgs e)
		{
		
		}
	}
}
