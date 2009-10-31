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
            this.menuFile = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pathToDBToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tabSearch = new System.Windows.Forms.TabPage();
            this.panelLex = new System.Windows.Forms.Panel();
            this.radioButtonWordForm = new System.Windows.Forms.RadioButton();
            this.lexPanel = new System.Windows.Forms.Panel();
            this.buttonLookup = new System.Windows.Forms.Button();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.radioButtonInitForm = new System.Windows.Forms.RadioButton();
            this.radioButtonGStem = new System.Windows.Forms.RadioButton();
            this.textBoxSearchString = new System.Windows.Forms.TextBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.tabControl = new System.Windows.Forms.TabControl();
            this.menuFile.SuspendLayout();
            this.tabSearch.SuspendLayout();
            this.panelLex.SuspendLayout();
            this.tabControl.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuFile
            // 
            this.menuFile.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuFile.Location = new System.Drawing.Point(0, 0);
            this.menuFile.Name = "menuFile";
            this.menuFile.Size = new System.Drawing.Size(702, 24);
            this.menuFile.TabIndex = 1;
            this.menuFile.Text = "File";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.pathToDBToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // pathToDBToolStripMenuItem
            // 
            this.pathToDBToolStripMenuItem.Name = "pathToDBToolStripMenuItem";
            this.pathToDBToolStripMenuItem.Size = new System.Drawing.Size(130, 22);
            this.pathToDBToolStripMenuItem.Text = "Path to DB";
            this.pathToDBToolStripMenuItem.Click += new System.EventHandler(this.pathToDBToolStripMenuItem_Click);
            // 
            // tabSearch
            // 
            this.tabSearch.Controls.Add(this.panelLex);
            this.tabSearch.Location = new System.Drawing.Point(4, 22);
            this.tabSearch.Name = "tabSearch";
            this.tabSearch.Padding = new System.Windows.Forms.Padding(3);
            this.tabSearch.Size = new System.Drawing.Size(694, 791);
            this.tabSearch.TabIndex = 0;
            this.tabSearch.Text = "Search";
            this.tabSearch.UseVisualStyleBackColor = true;
            // 
            // panelLex
            // 
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
            this.panelLex.Size = new System.Drawing.Size(688, 827);
            this.panelLex.TabIndex = 0;
            // 
            // radioButtonWordForm
            // 
            this.radioButtonWordForm.AutoSize = true;
            this.radioButtonWordForm.Location = new System.Drawing.Point(445, 61);
            this.radioButtonWordForm.Name = "radioButtonWordForm";
            this.radioButtonWordForm.Size = new System.Drawing.Size(91, 17);
            this.radioButtonWordForm.TabIndex = 7;
            this.radioButtonWordForm.TabStop = true;
            this.radioButtonWordForm.Text = "Random Form";
            this.radioButtonWordForm.UseVisualStyleBackColor = true;
            // 
            // lexPanel
            // 
            this.lexPanel.AutoScroll = true;
            this.lexPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lexPanel.Location = new System.Drawing.Point(23, 131);
            this.lexPanel.Name = "lexPanel";
            this.lexPanel.Size = new System.Drawing.Size(639, 680);
            this.lexPanel.TabIndex = 6;
            // 
            // buttonLookup
            // 
            this.buttonLookup.Location = new System.Drawing.Point(559, 76);
            this.buttonLookup.Name = "buttonLookup";
            this.buttonLookup.Size = new System.Drawing.Size(75, 23);
            this.buttonLookup.TabIndex = 5;
            this.buttonLookup.Text = "Lookup";
            this.buttonLookup.UseVisualStyleBackColor = true;
            this.buttonLookup.Click += new System.EventHandler(this.buttonLookup_Click);
            // 
            // textBox3
            // 
            this.textBox3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox3.Location = new System.Drawing.Point(73, 61);
            this.textBox3.Name = "textBox3";
            this.textBox3.ReadOnly = true;
            this.textBox3.Size = new System.Drawing.Size(74, 20);
            this.textBox3.TabIndex = 4;
            this.textBox3.Text = "Search By:";
            // 
            // radioButtonInitForm
            // 
            this.radioButtonInitForm.AutoSize = true;
            this.radioButtonInitForm.Location = new System.Drawing.Point(315, 61);
            this.radioButtonInitForm.Name = "radioButtonInitForm";
            this.radioButtonInitForm.Size = new System.Drawing.Size(86, 17);
            this.radioButtonInitForm.TabIndex = 3;
            this.radioButtonInitForm.TabStop = true;
            this.radioButtonInitForm.Text = "Entry Source";
            this.radioButtonInitForm.UseVisualStyleBackColor = true;
            // 
            // radioButtonGStem
            // 
            this.radioButtonGStem.AutoSize = true;
            this.radioButtonGStem.Location = new System.Drawing.Point(174, 61);
            this.radioButtonGStem.Name = "radioButtonGStem";
            this.radioButtonGStem.Size = new System.Drawing.Size(89, 17);
            this.radioButtonGStem.TabIndex = 2;
            this.radioButtonGStem.TabStop = true;
            this.radioButtonGStem.Text = "Graphic Stem";
            this.radioButtonGStem.UseVisualStyleBackColor = true;
            // 
            // textBoxSearchString
            // 
            this.textBoxSearchString.Location = new System.Drawing.Point(174, 95);
            this.textBoxSearchString.Name = "textBoxSearchString";
            this.textBoxSearchString.Size = new System.Drawing.Size(362, 20);
            this.textBoxSearchString.TabIndex = 1;
            this.textBoxSearchString.TextChanged += new System.EventHandler(this.textBoxSearchString_TextChanged);
            // 
            // textBox1
            // 
            this.textBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox1.Location = new System.Drawing.Point(23, 95);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(124, 20);
            this.textBox1.TabIndex = 0;
            this.textBox1.Text = "Enter Search String:";
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.tabSearch);
            this.tabControl.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.tabControl.Location = new System.Drawing.Point(0, 27);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(702, 817);
            this.tabControl.TabIndex = 0;
            // 
            // TestApplet
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(702, 844);
            this.Controls.Add(this.menuFile);
            this.Controls.Add(this.tabControl);
            this.Name = "TestApplet";
            this.Text = "Zal Test Applet";
            this.menuFile.ResumeLayout(false);
            this.menuFile.PerformLayout();
            this.tabSearch.ResumeLayout(false);
            this.panelLex.ResumeLayout(false);
            this.panelLex.PerformLayout();
            this.tabControl.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuFile;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
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
        private System.Windows.Forms.ToolStripMenuItem pathToDBToolStripMenuItem;
    }
}

