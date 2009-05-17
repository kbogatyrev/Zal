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
            this.tabControl = new System.Windows.Forms.TabControl ();
            this.tabPage1 = new System.Windows.Forms.TabPage ();
            this.textBox2 = new System.Windows.Forms.TextBox ();
            this.buttonLogPath = new System.Windows.Forms.Button ();
            this.textBoxLogPath = new System.Windows.Forms.TextBox ();
            this.textBox7 = new System.Windows.Forms.TextBox ();
            this.textBox8 = new System.Windows.Forms.TextBox ();
            this.textBoxDbLocation = new System.Windows.Forms.TextBox ();
            this.textBoxSourcePath = new System.Windows.Forms.TextBox ();
            this.buttonDbLocation = new System.Windows.Forms.Button ();
            this.buttonSourcePath = new System.Windows.Forms.Button ();
            this.textBox = new System.Windows.Forms.TextBox ();
            this.panel2 = new System.Windows.Forms.Panel ();
            this.checkBoxTestRun = new System.Windows.Forms.CheckBox ();
            this.numericUpDownStopAfter = new System.Windows.Forms.NumericUpDown ();
            this.textBoxStopAfter = new System.Windows.Forms.TextBox ();
            this.tabPage2 = new System.Windows.Forms.TabPage ();
            this.buttonPreprocessLog = new System.Windows.Forms.Button ();
            this.textBox3 = new System.Windows.Forms.TextBox ();
            this.textBoxPreprocessLog = new System.Windows.Forms.TextBox ();
            this.buttonPreprocessErrors = new System.Windows.Forms.Button ();
            this.textBox1 = new System.Windows.Forms.TextBox ();
            this.textBoxPreprocessErrors = new System.Windows.Forms.TextBox ();
            this.textBox4 = new System.Windows.Forms.TextBox ();
            this.textBox5 = new System.Windows.Forms.TextBox ();
            this.textBoxPreprocessOutput = new System.Windows.Forms.TextBox ();
            this.textBoxPreprocessSource = new System.Windows.Forms.TextBox ();
            this.buttonPreprocessOutput = new System.Windows.Forms.Button ();
            this.buttonPreprocessSource = new System.Windows.Forms.Button ();
            this.textBoxPreprocess = new System.Windows.Forms.TextBox ();
            this.progressBar = new System.Windows.Forms.ProgressBar ();
            this.buttonOK = new System.Windows.Forms.Button ();
            this.buttonCancel = new System.Windows.Forms.Button ();
            this.tabControl.SuspendLayout ();
            this.tabPage1.SuspendLayout ();
            this.panel2.SuspendLayout ();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownStopAfter)).BeginInit ();
            this.tabPage2.SuspendLayout ();
            this.SuspendLayout ();
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add (this.tabPage1);
            this.tabControl.Controls.Add (this.tabPage2);
            this.tabControl.Location = new System.Drawing.Point (0, 0);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size (584, 322);
            this.tabControl.TabIndex = 18;
            this.tabControl.SelectedIndexChanged += new System.EventHandler (this.tabControl_SelectedIndexChanged);
            // 
            // tabPage1
            // 
            this.tabPage1.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage1.Controls.Add (this.textBox2);
            this.tabPage1.Controls.Add (this.buttonLogPath);
            this.tabPage1.Controls.Add (this.textBoxLogPath);
            this.tabPage1.Controls.Add (this.textBox7);
            this.tabPage1.Controls.Add (this.textBox8);
            this.tabPage1.Controls.Add (this.textBoxDbLocation);
            this.tabPage1.Controls.Add (this.textBoxSourcePath);
            this.tabPage1.Controls.Add (this.buttonDbLocation);
            this.tabPage1.Controls.Add (this.buttonSourcePath);
            this.tabPage1.Controls.Add (this.textBox);
            this.tabPage1.Controls.Add (this.panel2);
            this.tabPage1.Location = new System.Drawing.Point (4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding (3);
            this.tabPage1.Size = new System.Drawing.Size (576, 296);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Conversion";
            // 
            // textBox2
            // 
            this.textBox2.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox2.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9F);
            this.textBox2.Location = new System.Drawing.Point (16, 94);
            this.textBox2.Multiline = true;
            this.textBox2.Name = "textBox2";
            this.textBox2.ReadOnly = true;
            this.textBox2.Size = new System.Drawing.Size (117, 20);
            this.textBox2.TabIndex = 28;
            this.textBox2.Text = "Log File:";
            // 
            // buttonLogPath
            // 
            this.buttonLogPath.Location = new System.Drawing.Point (530, 91);
            this.buttonLogPath.Name = "buttonLogPath";
            this.buttonLogPath.Size = new System.Drawing.Size (34, 26);
            this.buttonLogPath.TabIndex = 27;
            this.buttonLogPath.Text = "•••";
            this.buttonLogPath.UseVisualStyleBackColor = true;
            this.buttonLogPath.Click += new System.EventHandler (this.buttonLogPath_Click);
            // 
            // textBoxLogPath
            // 
            this.textBoxLogPath.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F);
            this.textBoxLogPath.Location = new System.Drawing.Point (139, 92);
            this.textBoxLogPath.Name = "textBoxLogPath";
            this.textBoxLogPath.Size = new System.Drawing.Size (384, 22);
            this.textBoxLogPath.TabIndex = 26;
            this.textBoxLogPath.TextChanged += new System.EventHandler (this.textBoxLogPath_TextChanged);
            // 
            // textBox7
            // 
            this.textBox7.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox7.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9F);
            this.textBox7.Location = new System.Drawing.Point (16, 61);
            this.textBox7.Multiline = true;
            this.textBox7.Name = "textBox7";
            this.textBox7.ReadOnly = true;
            this.textBox7.Size = new System.Drawing.Size (117, 20);
            this.textBox7.TabIndex = 25;
            this.textBox7.Text = "Database Location:";
            // 
            // textBox8
            // 
            this.textBox8.BackColor = System.Drawing.SystemColors.Control;
            this.textBox8.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox8.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9F);
            this.textBox8.Location = new System.Drawing.Point (15, 21);
            this.textBox8.Multiline = true;
            this.textBox8.Name = "textBox8";
            this.textBox8.ReadOnly = true;
            this.textBox8.Size = new System.Drawing.Size (118, 17);
            this.textBox8.TabIndex = 24;
            this.textBox8.Text = "Source File:";
            // 
            // textBoxDbLocation
            // 
            this.textBoxDbLocation.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F);
            this.textBoxDbLocation.Location = new System.Drawing.Point (139, 55);
            this.textBoxDbLocation.Name = "textBoxDbLocation";
            this.textBoxDbLocation.Size = new System.Drawing.Size (384, 22);
            this.textBoxDbLocation.TabIndex = 23;
            this.textBoxDbLocation.TextChanged += new System.EventHandler (this.textBoxDbLocation_TextChanged);
            // 
            // textBoxSourcePath
            // 
            this.textBoxSourcePath.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxSourcePath.Location = new System.Drawing.Point (139, 16);
            this.textBoxSourcePath.Name = "textBoxSourcePath";
            this.textBoxSourcePath.Size = new System.Drawing.Size (384, 22);
            this.textBoxSourcePath.TabIndex = 22;
            this.textBoxSourcePath.TextChanged += new System.EventHandler (this.textBoxSourcePath_TextChanged);
            // 
            // buttonDbLocation
            // 
            this.buttonDbLocation.Location = new System.Drawing.Point (530, 53);
            this.buttonDbLocation.Margin = new System.Windows.Forms.Padding (1, 3, 1, 3);
            this.buttonDbLocation.Name = "buttonDbLocation";
            this.buttonDbLocation.Size = new System.Drawing.Size (34, 26);
            this.buttonDbLocation.TabIndex = 21;
            this.buttonDbLocation.Text = "•••";
            this.buttonDbLocation.UseVisualStyleBackColor = true;
            this.buttonDbLocation.Click += new System.EventHandler (this.buttonDbLocation_Click);
            // 
            // buttonSourcePath
            // 
            this.buttonSourcePath.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonSourcePath.Location = new System.Drawing.Point (530, 13);
            this.buttonSourcePath.Margin = new System.Windows.Forms.Padding (1, 3, 1, 3);
            this.buttonSourcePath.Name = "buttonSourcePath";
            this.buttonSourcePath.Size = new System.Drawing.Size (34, 26);
            this.buttonSourcePath.TabIndex = 20;
            this.buttonSourcePath.Text = "•••";
            this.buttonSourcePath.UseVisualStyleBackColor = true;
            this.buttonSourcePath.Click += new System.EventHandler (this.buttonSourcePath_Click);
            // 
            // textBox
            // 
            this.textBox.Location = new System.Drawing.Point (139, 127);
            this.textBox.Multiline = true;
            this.textBox.Name = "textBox";
            this.textBox.ReadOnly = true;
            this.textBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox.Size = new System.Drawing.Size (423, 162);
            this.textBox.TabIndex = 19;
            this.textBox.Text = "   Conversion not started.";
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.SystemColors.Control;
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel2.Controls.Add (this.checkBoxTestRun);
            this.panel2.Controls.Add (this.numericUpDownStopAfter);
            this.panel2.Controls.Add (this.textBoxStopAfter);
            this.panel2.Location = new System.Drawing.Point (6, 131);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size (123, 162);
            this.panel2.TabIndex = 18;
            // 
            // checkBoxTestRun
            // 
            this.checkBoxTestRun.AutoSize = true;
            this.checkBoxTestRun.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9F);
            this.checkBoxTestRun.Location = new System.Drawing.Point (14, 21);
            this.checkBoxTestRun.Name = "checkBoxTestRun";
            this.checkBoxTestRun.Size = new System.Drawing.Size (75, 19);
            this.checkBoxTestRun.TabIndex = 14;
            this.checkBoxTestRun.Text = "Test Run";
            this.checkBoxTestRun.UseVisualStyleBackColor = true;
            this.checkBoxTestRun.Click += new System.EventHandler (this.checkBoxTestRun_CheckedChanged);
            // 
            // numericUpDownStopAfter
            // 
            this.numericUpDownStopAfter.Enabled = false;
            this.numericUpDownStopAfter.Increment = new decimal (new int[] {
            100,
            0,
            0,
            0});
            this.numericUpDownStopAfter.Location = new System.Drawing.Point (12, 91);
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
            this.numericUpDownStopAfter.Click += new System.EventHandler (this.numericUpDownStopAfter_ValueChanged);
            // 
            // textBoxStopAfter
            // 
            this.textBoxStopAfter.Enabled = false;
            this.textBoxStopAfter.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9F);
            this.textBoxStopAfter.Location = new System.Drawing.Point (12, 60);
            this.textBoxStopAfter.Name = "textBoxStopAfter";
            this.textBoxStopAfter.ReadOnly = true;
            this.textBoxStopAfter.Size = new System.Drawing.Size (93, 21);
            this.textBoxStopAfter.TabIndex = 16;
            this.textBoxStopAfter.Text = "Stop After:";
            // 
            // tabPage2
            // 
            this.tabPage2.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage2.Controls.Add (this.buttonPreprocessLog);
            this.tabPage2.Controls.Add (this.textBox3);
            this.tabPage2.Controls.Add (this.textBoxPreprocessLog);
            this.tabPage2.Controls.Add (this.buttonPreprocessErrors);
            this.tabPage2.Controls.Add (this.textBox1);
            this.tabPage2.Controls.Add (this.textBoxPreprocessErrors);
            this.tabPage2.Controls.Add (this.textBox4);
            this.tabPage2.Controls.Add (this.textBox5);
            this.tabPage2.Controls.Add (this.textBoxPreprocessOutput);
            this.tabPage2.Controls.Add (this.textBoxPreprocessSource);
            this.tabPage2.Controls.Add (this.buttonPreprocessOutput);
            this.tabPage2.Controls.Add (this.buttonPreprocessSource);
            this.tabPage2.Controls.Add (this.textBoxPreprocess);
            this.tabPage2.Location = new System.Drawing.Point (4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding (3);
            this.tabPage2.Size = new System.Drawing.Size (576, 296);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Pre-Processing";
            // 
            // buttonPreprocessLog
            // 
            this.buttonPreprocessLog.Location = new System.Drawing.Point (536, 110);
            this.buttonPreprocessLog.Margin = new System.Windows.Forms.Padding (1, 3, 1, 3);
            this.buttonPreprocessLog.Name = "buttonPreprocessLog";
            this.buttonPreprocessLog.Size = new System.Drawing.Size (34, 26);
            this.buttonPreprocessLog.TabIndex = 47;
            this.buttonPreprocessLog.Text = "•••";
            this.buttonPreprocessLog.UseVisualStyleBackColor = true;
            this.buttonPreprocessLog.Click += new System.EventHandler (this.buttonLogPath_Click);
            // 
            // textBox3
            // 
            this.textBox3.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox3.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9F);
            this.textBox3.Location = new System.Drawing.Point (8, 115);
            this.textBox3.Multiline = true;
            this.textBox3.Name = "textBox3";
            this.textBox3.ReadOnly = true;
            this.textBox3.Size = new System.Drawing.Size (69, 20);
            this.textBox3.TabIndex = 46;
            this.textBox3.Text = "Log File:";
            // 
            // textBoxPreprocessLog
            // 
            this.textBoxPreprocessLog.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F);
            this.textBoxPreprocessLog.Location = new System.Drawing.Point (81, 112);
            this.textBoxPreprocessLog.Name = "textBoxPreprocessLog";
            this.textBoxPreprocessLog.Size = new System.Drawing.Size (448, 22);
            this.textBoxPreprocessLog.TabIndex = 45;
            this.textBoxPreprocessLog.TextChanged += new System.EventHandler (this.textBoxPreprocessLog_TextChanged);
            // 
            // buttonPreprocessErrors
            // 
            this.buttonPreprocessErrors.Location = new System.Drawing.Point (536, 74);
            this.buttonPreprocessErrors.Margin = new System.Windows.Forms.Padding (1, 3, 1, 3);
            this.buttonPreprocessErrors.Name = "buttonPreprocessErrors";
            this.buttonPreprocessErrors.Size = new System.Drawing.Size (34, 26);
            this.buttonPreprocessErrors.TabIndex = 44;
            this.buttonPreprocessErrors.Text = "•••";
            this.buttonPreprocessErrors.UseVisualStyleBackColor = true;
            this.buttonPreprocessErrors.Click += new System.EventHandler (this.buttonPreprocessErrors_Click);
            // 
            // textBox1
            // 
            this.textBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox1.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9F);
            this.textBox1.Location = new System.Drawing.Point (8, 79);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size (69, 20);
            this.textBox1.TabIndex = 40;
            this.textBox1.Text = "Error File:";
            // 
            // textBoxPreprocessErrors
            // 
            this.textBoxPreprocessErrors.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F);
            this.textBoxPreprocessErrors.Location = new System.Drawing.Point (81, 78);
            this.textBoxPreprocessErrors.Name = "textBoxPreprocessErrors";
            this.textBoxPreprocessErrors.Size = new System.Drawing.Size (448, 22);
            this.textBoxPreprocessErrors.TabIndex = 39;
            this.textBoxPreprocessErrors.TextChanged += new System.EventHandler (this.textBoxPreprocessErrors_TextChanged);
            // 
            // textBox4
            // 
            this.textBox4.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox4.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9F);
            this.textBox4.Location = new System.Drawing.Point (8, 47);
            this.textBox4.Multiline = true;
            this.textBox4.Name = "textBox4";
            this.textBox4.ReadOnly = true;
            this.textBox4.Size = new System.Drawing.Size (69, 20);
            this.textBox4.TabIndex = 38;
            this.textBox4.Text = "Output File:";
            // 
            // textBox5
            // 
            this.textBox5.BackColor = System.Drawing.SystemColors.Control;
            this.textBox5.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox5.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9F);
            this.textBox5.Location = new System.Drawing.Point (8, 15);
            this.textBox5.Multiline = true;
            this.textBox5.Name = "textBox5";
            this.textBox5.ReadOnly = true;
            this.textBox5.Size = new System.Drawing.Size (69, 17);
            this.textBox5.TabIndex = 37;
            this.textBox5.Text = "Source File:";
            // 
            // textBoxPreprocessOutput
            // 
            this.textBoxPreprocessOutput.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F);
            this.textBoxPreprocessOutput.Location = new System.Drawing.Point (81, 44);
            this.textBoxPreprocessOutput.Name = "textBoxPreprocessOutput";
            this.textBoxPreprocessOutput.Size = new System.Drawing.Size (448, 22);
            this.textBoxPreprocessOutput.TabIndex = 36;
            this.textBoxPreprocessOutput.TextChanged += new System.EventHandler (this.textBoxPreprocessOutput_TextChanged);
            // 
            // textBoxPreprocessSource
            // 
            this.textBoxPreprocessSource.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxPreprocessSource.Location = new System.Drawing.Point (81, 10);
            this.textBoxPreprocessSource.Name = "textBoxPreprocessSource";
            this.textBoxPreprocessSource.Size = new System.Drawing.Size (448, 22);
            this.textBoxPreprocessSource.TabIndex = 35;
            this.textBoxPreprocessSource.TextChanged += new System.EventHandler (this.textBoxPreprocessSource_TextChanged);
            // 
            // buttonPreprocessOutput
            // 
            this.buttonPreprocessOutput.Location = new System.Drawing.Point (536, 42);
            this.buttonPreprocessOutput.Margin = new System.Windows.Forms.Padding (1, 3, 1, 3);
            this.buttonPreprocessOutput.Name = "buttonPreprocessOutput";
            this.buttonPreprocessOutput.Size = new System.Drawing.Size (34, 26);
            this.buttonPreprocessOutput.TabIndex = 34;
            this.buttonPreprocessOutput.Text = "•••";
            this.buttonPreprocessOutput.UseVisualStyleBackColor = true;
            this.buttonPreprocessOutput.Click += new System.EventHandler (this.buttonPreprocessOut_Click);
            // 
            // buttonPreprocessSource
            // 
            this.buttonPreprocessSource.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonPreprocessSource.Location = new System.Drawing.Point (536, 8);
            this.buttonPreprocessSource.Margin = new System.Windows.Forms.Padding (1, 3, 1, 3);
            this.buttonPreprocessSource.Name = "buttonPreprocessSource";
            this.buttonPreprocessSource.Size = new System.Drawing.Size (34, 26);
            this.buttonPreprocessSource.TabIndex = 33;
            this.buttonPreprocessSource.Text = "•••";
            this.buttonPreprocessSource.UseVisualStyleBackColor = true;
            this.buttonPreprocessSource.Click += new System.EventHandler (this.buttonSourcePath_Click);
            // 
            // textBoxPreprocess
            // 
            this.textBoxPreprocess.Location = new System.Drawing.Point (8, 147);
            this.textBoxPreprocess.Multiline = true;
            this.textBoxPreprocess.Name = "textBoxPreprocess";
            this.textBoxPreprocess.ReadOnly = true;
            this.textBoxPreprocess.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxPreprocess.Size = new System.Drawing.Size (562, 143);
            this.textBoxPreprocess.TabIndex = 32;
            this.textBoxPreprocess.Text = "   Pre-processing not started.";
            // 
            // progressBar
            // 
            this.progressBar.BackColor = System.Drawing.SystemColors.Control;
            this.progressBar.Location = new System.Drawing.Point (4, 326);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size (394, 23);
            this.progressBar.TabIndex = 31;
            // 
            // buttonOK
            // 
            this.buttonOK.Location = new System.Drawing.Point (405, 326);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size (75, 23);
            this.buttonOK.TabIndex = 30;
            this.buttonOK.Text = "OK";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler (this.buttonOK_Click);
            // 
            // buttonCancel
            // 
            this.buttonCancel.Location = new System.Drawing.Point (485, 326);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size (75, 23);
            this.buttonCancel.TabIndex = 29;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler (this.buttonCancel_Click);
            // 
            // ConverterForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size (584, 354);
            this.Controls.Add (this.buttonCancel);
            this.Controls.Add (this.buttonOK);
            this.Controls.Add (this.progressBar);
            this.Controls.Add (this.tabControl);
            this.MaximumSize = new System.Drawing.Size (600, 390);
            this.Name = "ConverterForm";
            this.Text = "GDRL Converter";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.ConverterForm_FormClosing);
            this.tabControl.ResumeLayout (false);
            this.tabPage1.ResumeLayout (false);
            this.tabPage1.PerformLayout ();
            this.panel2.ResumeLayout (false);
            this.panel2.PerformLayout ();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownStopAfter)).EndInit ();
            this.tabPage2.ResumeLayout (false);
            this.tabPage2.PerformLayout ();
            this.ResumeLayout (false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Button buttonLogPath;
        private System.Windows.Forms.TextBox textBoxLogPath;
        private System.Windows.Forms.TextBox textBox7;
        private System.Windows.Forms.TextBox textBox8;
        private System.Windows.Forms.TextBox textBoxDbLocation;
        private System.Windows.Forms.TextBox textBoxSourcePath;
        private System.Windows.Forms.Button buttonDbLocation;
        private System.Windows.Forms.Button buttonSourcePath;
        private System.Windows.Forms.TextBox textBox;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.CheckBox checkBoxTestRun;
        private System.Windows.Forms.NumericUpDown numericUpDownStopAfter;
        private System.Windows.Forms.TextBox textBoxStopAfter;
        private System.Windows.Forms.ProgressBar progressBar;
        private System.Windows.Forms.Button buttonOK;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBoxPreprocessErrors;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.TextBox textBoxPreprocessOutput;
        private System.Windows.Forms.TextBox textBoxPreprocessSource;
        private System.Windows.Forms.Button buttonPreprocessOutput;
        private System.Windows.Forms.Button buttonPreprocessSource;
        private System.Windows.Forms.TextBox textBoxPreprocess;
        private System.Windows.Forms.Button buttonPreprocessErrors;
        private System.Windows.Forms.Button buttonPreprocessLog;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.TextBox textBoxPreprocessLog;
    }
}

