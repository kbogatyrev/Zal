namespace Converter
{
    partial class ConverterForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.buttonCancel = new System.Windows.Forms.Button ();
            this.buttonOK = new System.Windows.Forms.Button ();
            this.progressBar = new System.Windows.Forms.ProgressBar ();
            this.textBox = new System.Windows.Forms.TextBox ();
            this.buttonSourcePath = new System.Windows.Forms.Button ();
            this.buttonDbLocation = new System.Windows.Forms.Button ();
            this.textBoxSourcePath = new System.Windows.Forms.TextBox ();
            this.textBoxDbLocation = new System.Windows.Forms.TextBox ();
            this.textBox3 = new System.Windows.Forms.TextBox ();
            this.textBox4 = new System.Windows.Forms.TextBox ();
            this.textBoxLogPath = new System.Windows.Forms.TextBox ();
            this.buttonLogPath = new System.Windows.Forms.Button ();
            this.textBox6 = new System.Windows.Forms.TextBox ();
            this.numericUpDownStopAfter = new System.Windows.Forms.NumericUpDown ();
            this.textBoxStopAfter = new System.Windows.Forms.TextBox ();
            this.checkBoxTestRun = new System.Windows.Forms.CheckBox ();
            this.panel1 = new System.Windows.Forms.Panel ();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownStopAfter)).BeginInit ();
            this.panel1.SuspendLayout ();
            this.SuspendLayout ();
            // 
            // buttonCancel
            // 
            this.buttonCancel.Location = new System.Drawing.Point (486, 294);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size (75, 23);
            this.buttonCancel.TabIndex = 1;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler (this.buttonCancel_Click);
            // 
            // buttonOK
            // 
            this.buttonOK.Location = new System.Drawing.Point (407, 294);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size (75, 23);
            this.buttonOK.TabIndex = 2;
            this.buttonOK.Text = "OK";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler (this.buttonOK_Click);
            // 
            // progressBar
            // 
            this.progressBar.BackColor = System.Drawing.SystemColors.Control;
            this.progressBar.Location = new System.Drawing.Point (7, 294);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size (394, 23);
            this.progressBar.TabIndex = 3;
            // 
            // textBox
            // 
            this.textBox.Location = new System.Drawing.Point (136, 124);
            this.textBox.Multiline = true;
            this.textBox.Name = "textBox";
            this.textBox.ReadOnly = true;
            this.textBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox.Size = new System.Drawing.Size (423, 162);
            this.textBox.TabIndex = 4;
            this.textBox.Text = "   Conversion not started.";
            // 
            // buttonSourcePath
            // 
            this.buttonSourcePath.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonSourcePath.Location = new System.Drawing.Point (527, 11);
            this.buttonSourcePath.Margin = new System.Windows.Forms.Padding (1, 3, 1, 3);
            this.buttonSourcePath.Name = "buttonSourcePath";
            this.buttonSourcePath.Size = new System.Drawing.Size (34, 26);
            this.buttonSourcePath.TabIndex = 5;
            this.buttonSourcePath.Text = "•••";
            this.buttonSourcePath.UseVisualStyleBackColor = true;
            this.buttonSourcePath.Click += new System.EventHandler (this.buttonSourcePath_Click);
            // 
            // buttonDbLocation
            // 
            this.buttonDbLocation.Location = new System.Drawing.Point (527, 51);
            this.buttonDbLocation.Margin = new System.Windows.Forms.Padding (1, 3, 1, 3);
            this.buttonDbLocation.Name = "buttonDbLocation";
            this.buttonDbLocation.Size = new System.Drawing.Size (34, 26);
            this.buttonDbLocation.TabIndex = 6;
            this.buttonDbLocation.Text = "•••";
            this.buttonDbLocation.UseVisualStyleBackColor = true;
            this.buttonDbLocation.Click += new System.EventHandler (this.buttonDbLocation_Click);
            // 
            // textBoxSourcePath
            // 
            this.textBoxSourcePath.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxSourcePath.Location = new System.Drawing.Point (136, 14);
            this.textBoxSourcePath.Name = "textBoxSourcePath";
            this.textBoxSourcePath.Size = new System.Drawing.Size (384, 22);
            this.textBoxSourcePath.TabIndex = 7;
            // 
            // textBoxDbLocation
            // 
            this.textBoxDbLocation.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F);
            this.textBoxDbLocation.Location = new System.Drawing.Point (136, 53);
            this.textBoxDbLocation.Name = "textBoxDbLocation";
            this.textBoxDbLocation.Size = new System.Drawing.Size (384, 22);
            this.textBoxDbLocation.TabIndex = 8;
            // 
            // textBox3
            // 
            this.textBox3.BackColor = System.Drawing.SystemColors.Control;
            this.textBox3.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox3.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9F);
            this.textBox3.Location = new System.Drawing.Point (12, 19);
            this.textBox3.Multiline = true;
            this.textBox3.Name = "textBox3";
            this.textBox3.ReadOnly = true;
            this.textBox3.Size = new System.Drawing.Size (118, 17);
            this.textBox3.TabIndex = 9;
            this.textBox3.Text = "Source File:";
            // 
            // textBox4
            // 
            this.textBox4.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox4.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9F);
            this.textBox4.Location = new System.Drawing.Point (13, 59);
            this.textBox4.Multiline = true;
            this.textBox4.Name = "textBox4";
            this.textBox4.ReadOnly = true;
            this.textBox4.Size = new System.Drawing.Size (117, 20);
            this.textBox4.TabIndex = 10;
            this.textBox4.Text = "Database Location:";
            // 
            // textBoxLogPath
            // 
            this.textBoxLogPath.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F);
            this.textBoxLogPath.Location = new System.Drawing.Point (136, 90);
            this.textBoxLogPath.Name = "textBoxLogPath";
            this.textBoxLogPath.Size = new System.Drawing.Size (384, 22);
            this.textBoxLogPath.TabIndex = 11;
            // 
            // buttonLogPath
            // 
            this.buttonLogPath.Location = new System.Drawing.Point (527, 89);
            this.buttonLogPath.Name = "buttonLogPath";
            this.buttonLogPath.Size = new System.Drawing.Size (34, 26);
            this.buttonLogPath.TabIndex = 12;
            this.buttonLogPath.Text = "•••";
            this.buttonLogPath.UseVisualStyleBackColor = true;
            this.buttonLogPath.Click += new System.EventHandler (this.buttonLogPath_Click);
            // 
            // textBox6
            // 
            this.textBox6.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox6.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9F);
            this.textBox6.Location = new System.Drawing.Point (13, 92);
            this.textBox6.Multiline = true;
            this.textBox6.Name = "textBox6";
            this.textBox6.ReadOnly = true;
            this.textBox6.Size = new System.Drawing.Size (117, 20);
            this.textBox6.TabIndex = 13;
            this.textBox6.Text = "Log File:";
            // 
            // numericUpDownStopAfter
            // 
            this.numericUpDownStopAfter.Enabled = false;
            this.numericUpDownStopAfter.Increment = new decimal (new int[] {
            100,
            0,
            0,
            0});
            this.numericUpDownStopAfter.Location = new System.Drawing.Point (12, 93);
            this.numericUpDownStopAfter.Maximum = new decimal (new int[] {
            100000,
            0,
            0,
            0});
            this.numericUpDownStopAfter.Name = "numericUpDownStopAfter";
            this.numericUpDownStopAfter.Size = new System.Drawing.Size (93, 20);
            this.numericUpDownStopAfter.TabIndex = 15;
            this.numericUpDownStopAfter.Value = new decimal (new int[] {
            100000,
            0,
            0,
            0});
            this.numericUpDownStopAfter.ValueChanged += new System.EventHandler (this.numericUpDownStopAfter_ValueChanged);
            // 
            // textBoxStopAfter
            // 
            this.textBoxStopAfter.Enabled = false;
            this.textBoxStopAfter.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9F);
            this.textBoxStopAfter.Location = new System.Drawing.Point (12, 62);
            this.textBoxStopAfter.Name = "textBoxStopAfter";
            this.textBoxStopAfter.ReadOnly = true;
            this.textBoxStopAfter.Size = new System.Drawing.Size (93, 21);
            this.textBoxStopAfter.TabIndex = 16;
            this.textBoxStopAfter.Text = "Stop After:";
            // 
            // checkBoxTestRun
            // 
            this.checkBoxTestRun.AutoSize = true;
            this.checkBoxTestRun.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9F);
            this.checkBoxTestRun.Location = new System.Drawing.Point (14, 23);
            this.checkBoxTestRun.Name = "checkBoxTestRun";
            this.checkBoxTestRun.Size = new System.Drawing.Size (75, 19);
            this.checkBoxTestRun.TabIndex = 14;
            this.checkBoxTestRun.Text = "Test Run";
            this.checkBoxTestRun.UseVisualStyleBackColor = true;
            this.checkBoxTestRun.CheckedChanged += new System.EventHandler (this.checkBoxTestRun_CheckedChanged);
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.Control;
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add (this.checkBoxTestRun);
            this.panel1.Controls.Add (this.numericUpDownStopAfter);
            this.panel1.Controls.Add (this.textBoxStopAfter);
            this.panel1.Location = new System.Drawing.Point (7, 124);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size (123, 162);
            this.panel1.TabIndex = 17;
            // 
            // ConverterForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size (571, 324);
            this.Controls.Add (this.panel1);
            this.Controls.Add (this.textBox6);
            this.Controls.Add (this.buttonLogPath);
            this.Controls.Add (this.textBoxLogPath);
            this.Controls.Add (this.textBox4);
            this.Controls.Add (this.textBox3);
            this.Controls.Add (this.textBoxDbLocation);
            this.Controls.Add (this.textBoxSourcePath);
            this.Controls.Add (this.buttonDbLocation);
            this.Controls.Add (this.buttonSourcePath);
            this.Controls.Add (this.textBox);
            this.Controls.Add (this.progressBar);
            this.Controls.Add (this.buttonOK);
            this.Controls.Add (this.buttonCancel);
            this.MaximumSize = new System.Drawing.Size (587, 360);
            this.Name = "ConverterForm";
            this.Text = "GDRL Converter";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.ConverterForm_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownStopAfter)).EndInit ();
            this.panel1.ResumeLayout (false);
            this.panel1.PerformLayout ();
            this.ResumeLayout (false);
            this.PerformLayout ();

        }

        #endregion

        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.Button buttonOK;
        private System.Windows.Forms.ProgressBar progressBar;
        private System.Windows.Forms.TextBox textBox;
        private System.Windows.Forms.Button buttonSourcePath;
        private System.Windows.Forms.Button buttonDbLocation;
        private System.Windows.Forms.TextBox textBoxSourcePath;
        private System.Windows.Forms.TextBox textBoxDbLocation;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.TextBox textBoxLogPath;
        private System.Windows.Forms.Button buttonLogPath;
        private System.Windows.Forms.TextBox textBox6;
        private System.Windows.Forms.NumericUpDown numericUpDownStopAfter;
        private System.Windows.Forms.TextBox textBoxStopAfter;
        private System.Windows.Forms.CheckBox checkBoxTestRun;
        private System.Windows.Forms.Panel panel1;
    }
}

