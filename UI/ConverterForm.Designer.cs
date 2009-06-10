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
            this.textBoxConversion = new System.Windows.Forms.TextBox ();
            this.panel2 = new System.Windows.Forms.Panel ();
            this.checkBoxTestRun = new System.Windows.Forms.CheckBox ();
            this.numericUpDownStopAfter = new System.Windows.Forms.NumericUpDown ();
            this.textBoxStopAfter = new System.Windows.Forms.TextBox ();
            this.tabPage2 = new System.Windows.Forms.TabPage ();
            this.buttonPreprocessLog = new System.Windows.Forms.Button ();
            this.textBox3 = new System.Windows.Forms.TextBox ();
            this.textBoxPreprocessLog = new System.Windows.Forms.TextBox ();
            this.textBox5 = new System.Windows.Forms.TextBox ();
            this.textBoxPreprocessSource = new System.Windows.Forms.TextBox ();
            this.buttonPreprocessSource = new System.Windows.Forms.Button ();
            this.textBoxPreprocess = new System.Windows.Forms.TextBox ();
            this.tabPage3 = new System.Windows.Forms.TabPage ();
            this.buttonSearchLog = new System.Windows.Forms.Button ();
            this.textBox1 = new System.Windows.Forms.TextBox ();
            this.textBoxSearchLog = new System.Windows.Forms.TextBox ();
            this.checkBoxSearchRegex = new System.Windows.Forms.CheckBox ();
            this.textBox10 = new System.Windows.Forms.TextBox ();
            this.textBoxSearchString = new System.Windows.Forms.TextBox ();
            this.textBoxSearch = new System.Windows.Forms.TextBox ();
            this.buttonSearchSource = new System.Windows.Forms.Button ();
            this.textBox6 = new System.Windows.Forms.TextBox ();
            this.textBoxSearchSource = new System.Windows.Forms.TextBox ();
            this.progressBar = new System.Windows.Forms.ProgressBar ();
            this.buttonOK = new System.Windows.Forms.Button ();
            this.buttonCancel = new System.Windows.Forms.Button ();
            this.tabControl.SuspendLayout ();
            this.tabPage1.SuspendLayout ();
            this.panel2.SuspendLayout ();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownStopAfter)).BeginInit ();
            this.tabPage2.SuspendLayout ();
            this.tabPage3.SuspendLayout ();
            this.SuspendLayout ();
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add (this.tabPage1);
            this.tabControl.Controls.Add (this.tabPage2);
            this.tabControl.Controls.Add (this.tabPage3);
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
            this.tabPage1.Controls.Add (this.textBoxConversion);
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
            // textBoxConversion
            // 
            this.textBoxConversion.Location = new System.Drawing.Point (139, 127);
            this.textBoxConversion.Multiline = true;
            this.textBoxConversion.Name = "textBoxConversion";
            this.textBoxConversion.ReadOnly = true;
            this.textBoxConversion.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxConversion.Size = new System.Drawing.Size (423, 162);
            this.textBoxConversion.TabIndex = 19;
            this.textBoxConversion.Text = "   Conversion not started.";
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
            this.tabPage2.Controls.Add (this.textBox5);
            this.tabPage2.Controls.Add (this.textBoxPreprocessSource);
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
            this.buttonPreprocessLog.Location = new System.Drawing.Point (536, 47);
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
            this.textBox3.Location = new System.Drawing.Point (8, 52);
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
            this.textBoxPreprocessLog.Location = new System.Drawing.Point (81, 49);
            this.textBoxPreprocessLog.Name = "textBoxPreprocessLog";
            this.textBoxPreprocessLog.Size = new System.Drawing.Size (448, 22);
            this.textBoxPreprocessLog.TabIndex = 45;
            this.textBoxPreprocessLog.TextChanged += new System.EventHandler (this.textBoxPreprocessLog_TextChanged);
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
            // textBoxPreprocessSource
            // 
            this.textBoxPreprocessSource.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxPreprocessSource.Location = new System.Drawing.Point (81, 10);
            this.textBoxPreprocessSource.Name = "textBoxPreprocessSource";
            this.textBoxPreprocessSource.Size = new System.Drawing.Size (448, 22);
            this.textBoxPreprocessSource.TabIndex = 35;
            this.textBoxPreprocessSource.TextChanged += new System.EventHandler (this.textBoxPreprocessSource_TextChanged);
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
            this.textBoxPreprocess.Location = new System.Drawing.Point (8, 91);
            this.textBoxPreprocess.Multiline = true;
            this.textBoxPreprocess.Name = "textBoxPreprocess";
            this.textBoxPreprocess.ReadOnly = true;
            this.textBoxPreprocess.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxPreprocess.Size = new System.Drawing.Size (562, 199);
            this.textBoxPreprocess.TabIndex = 32;
            this.textBoxPreprocess.Text = "   Pre-processing not started.";
            // 
            // tabPage3
            // 
            this.tabPage3.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage3.Controls.Add (this.buttonSearchLog);
            this.tabPage3.Controls.Add (this.textBox1);
            this.tabPage3.Controls.Add (this.textBoxSearchLog);
            this.tabPage3.Controls.Add (this.checkBoxSearchRegex);
            this.tabPage3.Controls.Add (this.textBox10);
            this.tabPage3.Controls.Add (this.textBoxSearchString);
            this.tabPage3.Controls.Add (this.textBoxSearch);
            this.tabPage3.Controls.Add (this.buttonSearchSource);
            this.tabPage3.Controls.Add (this.textBox6);
            this.tabPage3.Controls.Add (this.textBoxSearchSource);
            this.tabPage3.Location = new System.Drawing.Point (4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new System.Drawing.Size (576, 296);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Extraction";
            // 
            // buttonSearchLog
            // 
            this.buttonSearchLog.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonSearchLog.Location = new System.Drawing.Point (539, 34);
            this.buttonSearchLog.Margin = new System.Windows.Forms.Padding (1, 3, 1, 3);
            this.buttonSearchLog.Name = "buttonSearchLog";
            this.buttonSearchLog.Size = new System.Drawing.Size (34, 26);
            this.buttonSearchLog.TabIndex = 62;
            this.buttonSearchLog.Text = "•••";
            this.buttonSearchLog.UseVisualStyleBackColor = true;
            this.buttonSearchLog.Click += new System.EventHandler (this.buttonLogPath_Click);
            // 
            // textBox1
            // 
            this.textBox1.BackColor = System.Drawing.SystemColors.Control;
            this.textBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox1.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9F);
            this.textBox1.Location = new System.Drawing.Point (11, 41);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size (69, 17);
            this.textBox1.TabIndex = 61;
            this.textBox1.Text = "Log File:";
            // 
            // textBoxSearchLog
            // 
            this.textBoxSearchLog.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxSearchLog.Location = new System.Drawing.Point (95, 36);
            this.textBoxSearchLog.Name = "textBoxSearchLog";
            this.textBoxSearchLog.Size = new System.Drawing.Size (436, 22);
            this.textBoxSearchLog.TabIndex = 60;
            this.textBoxSearchLog.TextChanged += new System.EventHandler (this.textBoxSearchLog_TextChanged);
            // 
            // checkBoxSearchRegex
            // 
            this.checkBoxSearchRegex.AutoSize = true;
            this.checkBoxSearchRegex.Location = new System.Drawing.Point (516, 72);
            this.checkBoxSearchRegex.Name = "checkBoxSearchRegex";
            this.checkBoxSearchRegex.Size = new System.Drawing.Size (57, 17);
            this.checkBoxSearchRegex.TabIndex = 59;
            this.checkBoxSearchRegex.Text = "Regex";
            this.checkBoxSearchRegex.UseVisualStyleBackColor = true;
            this.checkBoxSearchRegex.CheckedChanged += new System.EventHandler (this.checkBoxSearchRegex_CheckedChanged);
            // 
            // textBox10
            // 
            this.textBox10.BackColor = System.Drawing.SystemColors.Control;
            this.textBox10.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox10.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9F);
            this.textBox10.Location = new System.Drawing.Point (10, 72);
            this.textBox10.Multiline = true;
            this.textBox10.Name = "textBox10";
            this.textBox10.ReadOnly = true;
            this.textBox10.Size = new System.Drawing.Size (79, 17);
            this.textBox10.TabIndex = 58;
            this.textBox10.Text = "Search string:";
            // 
            // textBoxSearchString
            // 
            this.textBoxSearchString.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxSearchString.Location = new System.Drawing.Point (95, 69);
            this.textBoxSearchString.Name = "textBoxSearchString";
            this.textBoxSearchString.Size = new System.Drawing.Size (415, 22);
            this.textBoxSearchString.TabIndex = 57;
            this.textBoxSearchString.TextChanged += new System.EventHandler (this.textBoxSearchString_TextChanged);
            // 
            // textBoxSearch
            // 
            this.textBoxSearch.Location = new System.Drawing.Point (7, 98);
            this.textBoxSearch.Multiline = true;
            this.textBoxSearch.Name = "textBoxSearch";
            this.textBoxSearch.ReadOnly = true;
            this.textBoxSearch.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxSearch.Size = new System.Drawing.Size (562, 194);
            this.textBoxSearch.TabIndex = 56;
            this.textBoxSearch.Text = "   Search not started.";
            // 
            // buttonSearchSource
            // 
            this.buttonSearchSource.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonSearchSource.Location = new System.Drawing.Point (539, 4);
            this.buttonSearchSource.Margin = new System.Windows.Forms.Padding (1, 3, 1, 3);
            this.buttonSearchSource.Name = "buttonSearchSource";
            this.buttonSearchSource.Size = new System.Drawing.Size (34, 26);
            this.buttonSearchSource.TabIndex = 52;
            this.buttonSearchSource.Text = "•••";
            this.buttonSearchSource.UseVisualStyleBackColor = true;
            this.buttonSearchSource.Click += new System.EventHandler (this.buttonSourcePath_Click);
            // 
            // textBox6
            // 
            this.textBox6.BackColor = System.Drawing.SystemColors.Control;
            this.textBox6.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox6.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9F);
            this.textBox6.Location = new System.Drawing.Point (10, 11);
            this.textBox6.Multiline = true;
            this.textBox6.Name = "textBox6";
            this.textBox6.ReadOnly = true;
            this.textBox6.Size = new System.Drawing.Size (69, 17);
            this.textBox6.TabIndex = 49;
            this.textBox6.Text = "Source File:";
            // 
            // textBoxSearchSource
            // 
            this.textBoxSearchSource.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxSearchSource.Location = new System.Drawing.Point (95, 6);
            this.textBoxSearchSource.Name = "textBoxSearchSource";
            this.textBoxSearchSource.Size = new System.Drawing.Size (436, 22);
            this.textBoxSearchSource.TabIndex = 48;
            this.textBoxSearchSource.TextChanged += new System.EventHandler (this.textBoxSearchSource_TextChanged);
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
            this.tabPage3.ResumeLayout (false);
            this.tabPage3.PerformLayout ();
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
        private System.Windows.Forms.TextBox textBoxConversion;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.CheckBox checkBoxTestRun;
        private System.Windows.Forms.NumericUpDown numericUpDownStopAfter;
        private System.Windows.Forms.TextBox textBoxStopAfter;
        private System.Windows.Forms.ProgressBar progressBar;
        private System.Windows.Forms.Button buttonOK;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.TextBox textBoxPreprocessSource;
        private System.Windows.Forms.Button buttonPreprocessSource;
        private System.Windows.Forms.TextBox textBoxPreprocess;
        private System.Windows.Forms.Button buttonPreprocessLog;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.TextBox textBoxPreprocessLog;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.TextBox textBox6;
        private System.Windows.Forms.TextBox textBoxSearchSource;
        private System.Windows.Forms.TextBox textBoxSearch;
        private System.Windows.Forms.Button buttonSearchSource;
        private System.Windows.Forms.CheckBox checkBoxSearchRegex;
        private System.Windows.Forms.TextBox textBox10;
        private System.Windows.Forms.TextBox textBoxSearchString;
        private System.Windows.Forms.Button buttonSearchLog;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBoxSearchLog;
    }
}

