namespace TestUI
{
    partial class TestApplet
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
            this.tabSearch = new System.Windows.Forms.TabPage();
            this.panelLex = new System.Windows.Forms.Panel();
            this.radioButtonPreprocess = new System.Windows.Forms.RadioButton();
            this.buttonSelect = new System.Windows.Forms.Button();
            this.textBoxDBPath = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.radioButtonWordForm = new System.Windows.Forms.RadioButton();
            this.lexPanel = new System.Windows.Forms.Panel();
            this.buttonLookup = new System.Windows.Forms.Button();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.radioButtonInitForm = new System.Windows.Forms.RadioButton();
            this.radioButtonGStem = new System.Windows.Forms.RadioButton();
            this.textBoxSearchString = new System.Windows.Forms.TextBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.tabControl = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.buttonSelect2 = new System.Windows.Forms.Button();
            this.textBoxDbPath2 = new System.Windows.Forms.TextBox();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.buttonLookup2 = new System.Windows.Forms.Button();
            this.textBox6 = new System.Windows.Forms.TextBox();
            this.radioButtonTestBySource = new System.Windows.Forms.RadioButton();
            this.radioButtonTestByGrStem = new System.Windows.Forms.RadioButton();
            this.textBoxSearchString2 = new System.Windows.Forms.TextBox();
            this.textBox8 = new System.Windows.Forms.TextBox();
            this.radioButtonTestAll = new System.Windows.Forms.RadioButton();
            this.tabSearch.SuspendLayout();
            this.panelLex.SuspendLayout();
            this.tabControl.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabSearch
            // 
            this.tabSearch.Controls.Add(this.panelLex);
            this.tabSearch.Location = new System.Drawing.Point(4, 22);
            this.tabSearch.Name = "tabSearch";
            this.tabSearch.Padding = new System.Windows.Forms.Padding(3);
            this.tabSearch.Size = new System.Drawing.Size(694, 759);
            this.tabSearch.TabIndex = 0;
            this.tabSearch.Text = "Search";
            this.tabSearch.UseVisualStyleBackColor = true;
            // 
            // panelLex
            // 
            this.panelLex.Controls.Add(this.radioButtonPreprocess);
            this.panelLex.Controls.Add(this.buttonSelect);
            this.panelLex.Controls.Add(this.textBoxDBPath);
            this.panelLex.Controls.Add(this.textBox2);
            this.panelLex.Controls.Add(this.radioButtonWordForm);
            this.panelLex.Controls.Add(this.lexPanel);
            this.panelLex.Controls.Add(this.buttonLookup);
            this.panelLex.Controls.Add(this.textBox3);
            this.panelLex.Controls.Add(this.radioButtonInitForm);
            this.panelLex.Controls.Add(this.radioButtonGStem);
            this.panelLex.Controls.Add(this.textBoxSearchString);
            this.panelLex.Controls.Add(this.textBox1);
            this.panelLex.Location = new System.Drawing.Point(3, -44);
            this.panelLex.Name = "panelLex";
            this.panelLex.Size = new System.Drawing.Size(688, 859);
            this.panelLex.TabIndex = 0;
            // 
            // radioButtonPreprocess
            // 
            this.radioButtonPreprocess.AutoSize = true;
            this.radioButtonPreprocess.Location = new System.Drawing.Point(480, 94);
            this.radioButtonPreprocess.Name = "radioButtonPreprocess";
            this.radioButtonPreprocess.Size = new System.Drawing.Size(78, 17);
            this.radioButtonPreprocess.TabIndex = 6;
            this.radioButtonPreprocess.TabStop = true;
            this.radioButtonPreprocess.Text = "Preprocess";
            this.radioButtonPreprocess.UseVisualStyleBackColor = true;
            // 
            // buttonSelect
            // 
            this.buttonSelect.Location = new System.Drawing.Point(574, 59);
            this.buttonSelect.Name = "buttonSelect";
            this.buttonSelect.Size = new System.Drawing.Size(75, 23);
            this.buttonSelect.TabIndex = 2;
            this.buttonSelect.Text = "Select...";
            this.buttonSelect.UseVisualStyleBackColor = true;
            this.buttonSelect.Click += new System.EventHandler(this.buttonSelect_Click);
            // 
            // textBoxDBPath
            // 
            this.textBoxDBPath.Location = new System.Drawing.Point(166, 58);
            this.textBoxDBPath.Name = "textBoxDBPath";
            this.textBoxDBPath.Size = new System.Drawing.Size(392, 20);
            this.textBoxDBPath.TabIndex = 1;
            this.textBoxDBPath.TextChanged += new System.EventHandler(this.textBoxDBPath_TextChanged);
            // 
            // textBox2
            // 
            this.textBox2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox2.Location = new System.Drawing.Point(73, 59);
            this.textBox2.Name = "textBox2";
            this.textBox2.ReadOnly = true;
            this.textBox2.Size = new System.Drawing.Size(74, 20);
            this.textBox2.TabIndex = 0;
            this.textBox2.TabStop = false;
            this.textBox2.Text = "DB Path:";
            // 
            // radioButtonWordForm
            // 
            this.radioButtonWordForm.AutoSize = true;
            this.radioButtonWordForm.Location = new System.Drawing.Point(373, 94);
            this.radioButtonWordForm.Name = "radioButtonWordForm";
            this.radioButtonWordForm.Size = new System.Drawing.Size(91, 17);
            this.radioButtonWordForm.TabIndex = 5;
            this.radioButtonWordForm.TabStop = true;
            this.radioButtonWordForm.Text = "Random Form";
            this.radioButtonWordForm.UseVisualStyleBackColor = true;
            // 
            // lexPanel
            // 
            this.lexPanel.AutoScroll = true;
            this.lexPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lexPanel.Location = new System.Drawing.Point(23, 167);
            this.lexPanel.Name = "lexPanel";
            this.lexPanel.Size = new System.Drawing.Size(639, 676);
            this.lexPanel.TabIndex = 0;
            // 
            // buttonLookup
            // 
            this.buttonLookup.Location = new System.Drawing.Point(574, 126);
            this.buttonLookup.Name = "buttonLookup";
            this.buttonLookup.Size = new System.Drawing.Size(75, 23);
            this.buttonLookup.TabIndex = 8;
            this.buttonLookup.Text = "Lookup";
            this.buttonLookup.UseVisualStyleBackColor = true;
            this.buttonLookup.Click += new System.EventHandler(this.buttonLookup_Click);
            // 
            // textBox3
            // 
            this.textBox3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox3.Location = new System.Drawing.Point(73, 94);
            this.textBox3.Name = "textBox3";
            this.textBox3.ReadOnly = true;
            this.textBox3.Size = new System.Drawing.Size(74, 20);
            this.textBox3.TabIndex = 0;
            this.textBox3.TabStop = false;
            this.textBox3.Text = "Search By:";
            // 
            // radioButtonInitForm
            // 
            this.radioButtonInitForm.AutoSize = true;
            this.radioButtonInitForm.Location = new System.Drawing.Point(271, 94);
            this.radioButtonInitForm.Name = "radioButtonInitForm";
            this.radioButtonInitForm.Size = new System.Drawing.Size(86, 17);
            this.radioButtonInitForm.TabIndex = 4;
            this.radioButtonInitForm.TabStop = true;
            this.radioButtonInitForm.Text = "Entry Source";
            this.radioButtonInitForm.UseVisualStyleBackColor = true;
            // 
            // radioButtonGStem
            // 
            this.radioButtonGStem.AutoSize = true;
            this.radioButtonGStem.Location = new System.Drawing.Point(166, 94);
            this.radioButtonGStem.Name = "radioButtonGStem";
            this.radioButtonGStem.Size = new System.Drawing.Size(89, 17);
            this.radioButtonGStem.TabIndex = 3;
            this.radioButtonGStem.TabStop = true;
            this.radioButtonGStem.Text = "Graphic Stem";
            this.radioButtonGStem.UseVisualStyleBackColor = true;
            // 
            // textBoxSearchString
            // 
            this.textBoxSearchString.Location = new System.Drawing.Point(166, 128);
            this.textBoxSearchString.Name = "textBoxSearchString";
            this.textBoxSearchString.Size = new System.Drawing.Size(392, 20);
            this.textBoxSearchString.TabIndex = 7;
            this.textBoxSearchString.TextChanged += new System.EventHandler(this.textBoxSearchString_TextChanged);
            // 
            // textBox1
            // 
            this.textBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox1.Location = new System.Drawing.Point(23, 128);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(124, 20);
            this.textBox1.TabIndex = 0;
            this.textBox1.TabStop = false;
            this.textBox1.Text = "Enter Search String:";
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.tabSearch);
            this.tabControl.Controls.Add(this.tabPage1);
            this.tabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl.Location = new System.Drawing.Point(0, 0);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(702, 785);
            this.tabControl.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.radioButtonTestAll);
            this.tabPage1.Controls.Add(this.buttonSelect2);
            this.tabPage1.Controls.Add(this.textBoxDbPath2);
            this.tabPage1.Controls.Add(this.textBox5);
            this.tabPage1.Controls.Add(this.panel1);
            this.tabPage1.Controls.Add(this.buttonLookup2);
            this.tabPage1.Controls.Add(this.textBox6);
            this.tabPage1.Controls.Add(this.radioButtonTestBySource);
            this.tabPage1.Controls.Add(this.radioButtonTestByGrStem);
            this.tabPage1.Controls.Add(this.textBoxSearchString2);
            this.tabPage1.Controls.Add(this.textBox8);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(694, 759);
            this.tabPage1.TabIndex = 1;
            this.tabPage1.Text = "Test";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // buttonSelect2
            // 
            this.buttonSelect2.Location = new System.Drawing.Point(577, 15);
            this.buttonSelect2.Name = "buttonSelect2";
            this.buttonSelect2.Size = new System.Drawing.Size(75, 23);
            this.buttonSelect2.TabIndex = 14;
            this.buttonSelect2.Text = "Select...";
            this.buttonSelect2.UseVisualStyleBackColor = true;
            // 
            // textBoxDbPath2
            // 
            this.textBoxDbPath2.Location = new System.Drawing.Point(169, 14);
            this.textBoxDbPath2.Name = "textBoxDbPath2";
            this.textBoxDbPath2.Size = new System.Drawing.Size(392, 20);
            this.textBoxDbPath2.TabIndex = 13;
            // 
            // textBox5
            // 
            this.textBox5.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox5.Location = new System.Drawing.Point(76, 15);
            this.textBox5.Name = "textBox5";
            this.textBox5.ReadOnly = true;
            this.textBox5.Size = new System.Drawing.Size(74, 20);
            this.textBox5.TabIndex = 9;
            this.textBox5.TabStop = false;
            this.textBox5.Text = "DB Path:";
            // 
            // panel1
            // 
            this.panel1.AutoScroll = true;
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Location = new System.Drawing.Point(26, 123);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(639, 676);
            this.panel1.TabIndex = 12;
            // 
            // buttonLookup2
            // 
            this.buttonLookup2.Location = new System.Drawing.Point(577, 82);
            this.buttonLookup2.Name = "buttonLookup2";
            this.buttonLookup2.Size = new System.Drawing.Size(75, 23);
            this.buttonLookup2.TabIndex = 20;
            this.buttonLookup2.Text = "Lookup";
            this.buttonLookup2.UseVisualStyleBackColor = true;
            // 
            // textBox6
            // 
            this.textBox6.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox6.Location = new System.Drawing.Point(76, 50);
            this.textBox6.Name = "textBox6";
            this.textBox6.ReadOnly = true;
            this.textBox6.Size = new System.Drawing.Size(74, 20);
            this.textBox6.TabIndex = 11;
            this.textBox6.TabStop = false;
            this.textBox6.Text = "Test Cases:";
            // 
            // radioButtonTestBySource
            // 
            this.radioButtonTestBySource.AutoSize = true;
            this.radioButtonTestBySource.Location = new System.Drawing.Point(238, 50);
            this.radioButtonTestBySource.Name = "radioButtonTestBySource";
            this.radioButtonTestBySource.Size = new System.Drawing.Size(134, 17);
            this.radioButtonTestBySource.TabIndex = 16;
            this.radioButtonTestBySource.TabStop = true;
            this.radioButtonTestBySource.Text = "Select By Entry Source";
            this.radioButtonTestBySource.UseVisualStyleBackColor = true;
            // 
            // radioButtonTestByGrStem
            // 
            this.radioButtonTestByGrStem.AutoSize = true;
            this.radioButtonTestByGrStem.Location = new System.Drawing.Point(396, 50);
            this.radioButtonTestByGrStem.Name = "radioButtonTestByGrStem";
            this.radioButtonTestByGrStem.Size = new System.Drawing.Size(137, 17);
            this.radioButtonTestByGrStem.TabIndex = 15;
            this.radioButtonTestByGrStem.TabStop = true;
            this.radioButtonTestByGrStem.Text = "Select By Graphic Stem";
            this.radioButtonTestByGrStem.UseVisualStyleBackColor = true;
            // 
            // textBoxSearchString2
            // 
            this.textBoxSearchString2.Location = new System.Drawing.Point(169, 84);
            this.textBoxSearchString2.Name = "textBoxSearchString2";
            this.textBoxSearchString2.Size = new System.Drawing.Size(392, 20);
            this.textBoxSearchString2.TabIndex = 19;
            // 
            // textBox8
            // 
            this.textBox8.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox8.Location = new System.Drawing.Point(26, 84);
            this.textBox8.Name = "textBox8";
            this.textBox8.ReadOnly = true;
            this.textBox8.Size = new System.Drawing.Size(124, 20);
            this.textBox8.TabIndex = 10;
            this.textBox8.TabStop = false;
            this.textBox8.Text = "Enter Search String:";
            // 
            // radioButtonTestAll
            // 
            this.radioButtonTestAll.AutoSize = true;
            this.radioButtonTestAll.Location = new System.Drawing.Point(169, 50);
            this.radioButtonTestAll.Name = "radioButtonTestAll";
            this.radioButtonTestAll.Size = new System.Drawing.Size(36, 17);
            this.radioButtonTestAll.TabIndex = 21;
            this.radioButtonTestAll.TabStop = true;
            this.radioButtonTestAll.Text = "All";
            this.radioButtonTestAll.UseVisualStyleBackColor = true;
            // 
            // TestApplet
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(702, 785);
            this.Controls.Add(this.tabControl);
            this.Name = "TestApplet";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Zal Test Applet";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.TestApplet_FormClosing);
            this.tabSearch.ResumeLayout(false);
            this.panelLex.ResumeLayout(false);
            this.panelLex.PerformLayout();
            this.tabControl.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabPage tabSearch;
        private System.Windows.Forms.Panel panelLex;
        private System.Windows.Forms.Panel lexPanel;
        private System.Windows.Forms.Button buttonLookup;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.RadioButton radioButtonInitForm;
        private System.Windows.Forms.RadioButton radioButtonGStem;
        private System.Windows.Forms.TextBox textBoxSearchString;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.RadioButton radioButtonWordForm;
        private System.Windows.Forms.Button buttonSelect;
        private System.Windows.Forms.TextBox textBoxDBPath;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.RadioButton radioButtonPreprocess;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Button buttonSelect2;
        private System.Windows.Forms.TextBox textBoxDbPath2;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button buttonLookup2;
        private System.Windows.Forms.TextBox textBox6;
        private System.Windows.Forms.RadioButton radioButtonTestBySource;
        private System.Windows.Forms.RadioButton radioButtonTestByGrStem;
        private System.Windows.Forms.TextBox textBoxSearchString2;
        private System.Windows.Forms.TextBox textBox8;
        private System.Windows.Forms.RadioButton radioButtonTestAll;
    }
}

