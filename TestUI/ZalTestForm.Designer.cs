namespace TestUI
{
    partial class Synthesizer
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
            this.tabControl = new System.Windows.Forms.TabControl();
            this.tabLexeme = new System.Windows.Forms.TabPage();
            this.tabWF = new System.Windows.Forms.TabPage();
            this.menuFile = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.graphicStemToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panelLex = new System.Windows.Forms.Panel();
            this.panelWF = new System.Windows.Forms.Panel();
            this.tabControl.SuspendLayout();
            this.tabLexeme.SuspendLayout();
            this.tabWF.SuspendLayout();
            this.menuFile.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.tabLexeme);
            this.tabControl.Controls.Add(this.tabWF);
            this.tabControl.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.tabControl.Location = new System.Drawing.Point(0, 27);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(616, 704);
            this.tabControl.TabIndex = 0;
            // 
            // tabLexeme
            // 
            this.tabLexeme.Controls.Add(this.panelLex);
            this.tabLexeme.Location = new System.Drawing.Point(4, 22);
            this.tabLexeme.Name = "tabLexeme";
            this.tabLexeme.Padding = new System.Windows.Forms.Padding(3);
            this.tabLexeme.Size = new System.Drawing.Size(880, 678);
            this.tabLexeme.TabIndex = 0;
            this.tabLexeme.Text = "Lexeme";
            this.tabLexeme.UseVisualStyleBackColor = true;
            // 
            // tabWF
            // 
            this.tabWF.Controls.Add(this.panelWF);
            this.tabWF.Location = new System.Drawing.Point(4, 22);
            this.tabWF.Name = "tabWF";
            this.tabWF.Padding = new System.Windows.Forms.Padding(3);
            this.tabWF.Size = new System.Drawing.Size(608, 678);
            this.tabWF.TabIndex = 1;
            this.tabWF.Text = "Wordforms";
            this.tabWF.UseVisualStyleBackColor = true;
            // 
            // menuFile
            // 
            this.menuFile.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuFile.Location = new System.Drawing.Point(0, 0);
            this.menuFile.Name = "menuFile";
            this.menuFile.Size = new System.Drawing.Size(616, 24);
            this.menuFile.TabIndex = 1;
            this.menuFile.Text = "File";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.graphicStemToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // graphicStemToolStripMenuItem
            // 
            this.graphicStemToolStripMenuItem.Name = "graphicStemToolStripMenuItem";
            this.graphicStemToolStripMenuItem.Size = new System.Drawing.Size(145, 22);
            this.graphicStemToolStripMenuItem.Text = "Graphic Stem";
            this.graphicStemToolStripMenuItem.Click += new System.EventHandler(this.graphicStemToolStripMenuItem_Click);
            // 
            // panelLex
            // 
            this.panelLex.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelLex.Location = new System.Drawing.Point(3, 3);
            this.panelLex.Name = "panelLex";
            this.panelLex.Size = new System.Drawing.Size(874, 672);
            this.panelLex.TabIndex = 0;
            // 
            // panelWF
            // 
            this.panelWF.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelWF.Location = new System.Drawing.Point(3, 3);
            this.panelWF.Name = "panelWF";
            this.panelWF.Size = new System.Drawing.Size(602, 672);
            this.panelWF.TabIndex = 0;
            // 
            // Synthesizer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(616, 731);
            this.Controls.Add(this.menuFile);
            this.Controls.Add(this.tabControl);
            this.Name = "Synthesizer";
            this.Text = "Synthesizer";
            this.tabControl.ResumeLayout(false);
            this.tabLexeme.ResumeLayout(false);
            this.tabWF.ResumeLayout(false);
            this.menuFile.ResumeLayout(false);
            this.menuFile.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage tabLexeme;
        private System.Windows.Forms.TabPage tabWF;
        private System.Windows.Forms.MenuStrip menuFile;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem graphicStemToolStripMenuItem;
        private System.Windows.Forms.Panel panelLex;
        private System.Windows.Forms.Panel panelWF;
    }
}

