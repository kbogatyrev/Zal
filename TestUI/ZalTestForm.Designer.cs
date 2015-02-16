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
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.dBPathToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.FreqWFListToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.synthesizeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.byEntryFormToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.byGraphicStemToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.analyzeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.preprocessToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.parseWordFormToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.parseTextToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.testToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.batchTestToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.testRangeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exportTestDataToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.importTestDataToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.tabControl = new System.Windows.Forms.TabControl();
            this.menuStrip.SuspendLayout();
            this.statusStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip
            // 
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.synthesizeToolStripMenuItem,
            this.analyzeToolStripMenuItem,
            this.testToolStripMenuItem});
            this.menuStrip.Location = new System.Drawing.Point(0, 0);
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.Size = new System.Drawing.Size(784, 24);
            this.menuStrip.TabIndex = 3;
            this.menuStrip.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.dBPathToolStripMenuItem,
            this.FreqWFListToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // dBPathToolStripMenuItem
            // 
            this.dBPathToolStripMenuItem.Name = "dBPathToolStripMenuItem";
            this.dBPathToolStripMenuItem.Size = new System.Drawing.Size(230, 22);
            this.dBPathToolStripMenuItem.Text = "Open Dictionary";
            this.dBPathToolStripMenuItem.Click += new System.EventHandler(this.dBPathToolStripMenuItem_Click);
            // 
            // FreqWFListToolStripMenuItem
            // 
            this.FreqWFListToolStripMenuItem.Name = "FreqWFListToolStripMenuItem";
            this.FreqWFListToolStripMenuItem.Size = new System.Drawing.Size(230, 22);
            this.FreqWFListToolStripMenuItem.Text = "Open frequent wordforms list";
            this.FreqWFListToolStripMenuItem.Click += new System.EventHandler(this.FreqWFListToolStripMenuItem_Click);
            // 
            // synthesizeToolStripMenuItem
            // 
            this.synthesizeToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.byEntryFormToolStripMenuItem,
            this.byGraphicStemToolStripMenuItem});
            this.synthesizeToolStripMenuItem.Enabled = false;
            this.synthesizeToolStripMenuItem.Name = "synthesizeToolStripMenuItem";
            this.synthesizeToolStripMenuItem.Size = new System.Drawing.Size(74, 20);
            this.synthesizeToolStripMenuItem.Text = "Synthesize";
            // 
            // byEntryFormToolStripMenuItem
            // 
            this.byEntryFormToolStripMenuItem.Name = "byEntryFormToolStripMenuItem";
            this.byEntryFormToolStripMenuItem.Size = new System.Drawing.Size(161, 22);
            this.byEntryFormToolStripMenuItem.Text = "By Entry Source";
            this.byEntryFormToolStripMenuItem.Click += new System.EventHandler(this.byEntryFormToolStripMenuItem_Click);
            // 
            // byGraphicStemToolStripMenuItem
            // 
            this.byGraphicStemToolStripMenuItem.Name = "byGraphicStemToolStripMenuItem";
            this.byGraphicStemToolStripMenuItem.Size = new System.Drawing.Size(161, 22);
            this.byGraphicStemToolStripMenuItem.Text = "By Graphic Stem";
            this.byGraphicStemToolStripMenuItem.Click += new System.EventHandler(this.byGraphicStemToolStripMenuItem_Click);
            // 
            // analyzeToolStripMenuItem
            // 
            this.analyzeToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.preprocessToolStripMenuItem,
            this.parseWordFormToolStripMenuItem,
            this.parseTextToolStripMenuItem});
            this.analyzeToolStripMenuItem.Enabled = false;
            this.analyzeToolStripMenuItem.Name = "analyzeToolStripMenuItem";
            this.analyzeToolStripMenuItem.Size = new System.Drawing.Size(60, 20);
            this.analyzeToolStripMenuItem.Text = "Analyze";
            // 
            // preprocessToolStripMenuItem
            // 
            this.preprocessToolStripMenuItem.Enabled = false;
            this.preprocessToolStripMenuItem.Name = "preprocessToolStripMenuItem";
            this.preprocessToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.preprocessToolStripMenuItem.Text = "Preprocess";
            // 
            // parseWordFormToolStripMenuItem
            // 
            this.parseWordFormToolStripMenuItem.Name = "parseWordFormToolStripMenuItem";
            this.parseWordFormToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.parseWordFormToolStripMenuItem.Text = "Parse Word Form";
            this.parseWordFormToolStripMenuItem.Click += new System.EventHandler(this.parseWordFormToolStripMenuItem_Click);
            // 
            // parseTextToolStripMenuItem
            // 
            this.parseTextToolStripMenuItem.Enabled = false;
            this.parseTextToolStripMenuItem.Name = "parseTextToolStripMenuItem";
            this.parseTextToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.parseTextToolStripMenuItem.Text = "Parse Text";
            // 
            // testToolStripMenuItem
            // 
            this.testToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.batchTestToolStripMenuItem,
            this.testRangeToolStripMenuItem,
            this.exportTestDataToolStripMenuItem,
            this.importTestDataToolStripMenuItem});
            this.testToolStripMenuItem.Enabled = false;
            this.testToolStripMenuItem.Name = "testToolStripMenuItem";
            this.testToolStripMenuItem.Size = new System.Drawing.Size(41, 20);
            this.testToolStripMenuItem.Text = "Test";
            // 
            // batchTestToolStripMenuItem
            // 
            this.batchTestToolStripMenuItem.Name = "batchTestToolStripMenuItem";
            this.batchTestToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.batchTestToolStripMenuItem.Text = "Test All";
            this.batchTestToolStripMenuItem.Click += new System.EventHandler(this.batchTestToolStripMenuItem_Click);
            // 
            // testRangeToolStripMenuItem
            // 
            this.testRangeToolStripMenuItem.Enabled = false;
            this.testRangeToolStripMenuItem.Name = "testRangeToolStripMenuItem";
            this.testRangeToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.testRangeToolStripMenuItem.Text = "Test Range";
            // 
            // exportTestDataToolStripMenuItem
            // 
            this.exportTestDataToolStripMenuItem.Name = "exportTestDataToolStripMenuItem";
            this.exportTestDataToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.exportTestDataToolStripMenuItem.Text = "Export Test Data";
            // 
            // importTestDataToolStripMenuItem
            // 
            this.importTestDataToolStripMenuItem.Name = "importTestDataToolStripMenuItem";
            this.importTestDataToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.importTestDataToolStripMenuItem.Text = "Import Test Data";
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel});
            this.statusStrip.Location = new System.Drawing.Point(0, 839);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(784, 22);
            this.statusStrip.TabIndex = 4;
            // 
            // toolStripStatusLabel
            // 
            this.toolStripStatusLabel.Name = "toolStripStatusLabel";
            this.toolStripStatusLabel.Size = new System.Drawing.Size(87, 17);
            this.toolStripStatusLabel.Text = "No data source";
            // 
            // tabControl
            // 
            this.tabControl.Location = new System.Drawing.Point(0, 25);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(660, 820);
            this.tabControl.TabIndex = 0;
            // 
            // TestApplet
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(784, 861);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.tabControl);
            this.Controls.Add(this.menuStrip);
            this.MainMenuStrip = this.menuStrip;
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(800, 900);
            this.MinimumSize = new System.Drawing.Size(800, 900);
            this.Name = "TestApplet";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Zal Test Applet";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.TestApplet_FormClosing);
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem dBPathToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem synthesizeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem byEntryFormToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem byGraphicStemToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem analyzeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem preprocessToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem parseWordFormToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem testToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem batchTestToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem testRangeToolStripMenuItem;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel;
        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.ToolStripMenuItem exportTestDataToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem importTestDataToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem parseTextToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem FreqWFListToolStripMenuItem;
    }
}

