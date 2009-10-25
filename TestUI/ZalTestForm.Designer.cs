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
            this.menuFile = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tabLexeme = new System.Windows.Forms.TabPage();
            this.panelLex = new System.Windows.Forms.Panel();
            this.lexPanel = new System.Windows.Forms.Panel();
            this.buttonLookup = new System.Windows.Forms.Button();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.radioButtonInitForm = new System.Windows.Forms.RadioButton();
            this.radioButtonGStem = new System.Windows.Forms.RadioButton();
            this.textBoxSearchString = new System.Windows.Forms.TextBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.tabControl = new System.Windows.Forms.TabControl();
            this.menuFile.SuspendLayout();
            this.tabLexeme.SuspendLayout();
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
            this.menuFile.Size = new System.Drawing.Size(679, 24);
            this.menuFile.TabIndex = 1;
            this.menuFile.Text = "File";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // tabLexeme
            // 
            this.tabLexeme.Controls.Add(this.panelLex);
            this.tabLexeme.Location = new System.Drawing.Point(4, 22);
            this.tabLexeme.Name = "tabLexeme";
            this.tabLexeme.Padding = new System.Windows.Forms.Padding(3);
            this.tabLexeme.Size = new System.Drawing.Size(671, 678);
            this.tabLexeme.TabIndex = 0;
            this.tabLexeme.Text = "Lexeme";
            this.tabLexeme.UseVisualStyleBackColor = true;
            // 
            // panelLex
            // 
            this.panelLex.Controls.Add(this.lexPanel);
            this.panelLex.Controls.Add(this.buttonLookup);
            this.panelLex.Controls.Add(this.textBox3);
            this.panelLex.Controls.Add(this.radioButtonInitForm);
            this.panelLex.Controls.Add(this.radioButtonGStem);
            this.panelLex.Controls.Add(this.textBoxSearchString);
            this.panelLex.Controls.Add(this.textBox1);
            this.panelLex.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelLex.Location = new System.Drawing.Point(3, 3);
            this.panelLex.Name = "panelLex";
            this.panelLex.Size = new System.Drawing.Size(665, 672);
            this.panelLex.TabIndex = 0;
            // 
            // lexPanel
            // 
            this.lexPanel.AutoScroll = true;
            this.lexPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lexPanel.Location = new System.Drawing.Point(23, 112);
            this.lexPanel.Name = "lexPanel";
            this.lexPanel.Size = new System.Drawing.Size(615, 533);
            this.lexPanel.TabIndex = 6;
            // 
            // buttonLookup
            // 
            this.buttonLookup.Location = new System.Drawing.Point(563, 51);
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
            this.textBox3.Location = new System.Drawing.Point(23, 32);
            this.textBox3.Name = "textBox3";
            this.textBox3.ReadOnly = true;
            this.textBox3.Size = new System.Drawing.Size(79, 20);
            this.textBox3.TabIndex = 4;
            this.textBox3.Text = "Search By:";
            // 
            // radioButtonInitForm
            // 
            this.radioButtonInitForm.AutoSize = true;
            this.radioButtonInitForm.Location = new System.Drawing.Point(281, 33);
            this.radioButtonInitForm.Name = "radioButtonInitForm";
            this.radioButtonInitForm.Size = new System.Drawing.Size(75, 17);
            this.radioButtonInitForm.TabIndex = 3;
            this.radioButtonInitForm.TabStop = true;
            this.radioButtonInitForm.Text = "Initial Form";
            this.radioButtonInitForm.UseVisualStyleBackColor = true;
            // 
            // radioButtonGStem
            // 
            this.radioButtonGStem.AutoSize = true;
            this.radioButtonGStem.Location = new System.Drawing.Point(173, 34);
            this.radioButtonGStem.Name = "radioButtonGStem";
            this.radioButtonGStem.Size = new System.Drawing.Size(89, 17);
            this.radioButtonGStem.TabIndex = 2;
            this.radioButtonGStem.TabStop = true;
            this.radioButtonGStem.Text = "Graphic Stem";
            this.radioButtonGStem.UseVisualStyleBackColor = true;
            // 
            // textBoxSearchString
            // 
            this.textBoxSearchString.Location = new System.Drawing.Point(174, 66);
            this.textBoxSearchString.Name = "textBoxSearchString";
            this.textBoxSearchString.Size = new System.Drawing.Size(216, 20);
            this.textBoxSearchString.TabIndex = 1;
            this.textBoxSearchString.TextChanged += new System.EventHandler(this.textBoxSearchString_TextChanged);
            // 
            // textBox1
            // 
            this.textBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox1.Location = new System.Drawing.Point(23, 66);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(100, 20);
            this.textBox1.TabIndex = 0;
            this.textBox1.Text = "Enter Lexeme:";
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.tabLexeme);
            this.tabControl.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.tabControl.Location = new System.Drawing.Point(0, 27);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(679, 704);
            this.tabControl.TabIndex = 0;
            // 
            // Synthesizer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(679, 731);
            this.Controls.Add(this.menuFile);
            this.Controls.Add(this.tabControl);
            this.Name = "Synthesizer";
            this.Text = "Zal Synthesizer";
            this.menuFile.ResumeLayout(false);
            this.menuFile.PerformLayout();
            this.tabLexeme.ResumeLayout(false);
            this.panelLex.ResumeLayout(false);
            this.panelLex.PerformLayout();
            this.tabControl.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuFile;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.TabPage tabLexeme;
        private System.Windows.Forms.Panel panelLex;
        private System.Windows.Forms.Panel lexPanel;
        private System.Windows.Forms.Button buttonLookup;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.RadioButton radioButtonInitForm;
        private System.Windows.Forms.RadioButton radioButtonGStem;
        private System.Windows.Forms.TextBox textBoxSearchString;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TabControl tabControl;
    }
}

