namespace TestUI
{
    partial class ProgressDialog
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
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.buttonClose = new System.Windows.Forms.Button();
            this.textBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // progressBar
            // 
            this.progressBar.Location = new System.Drawing.Point(35, 157);
            this.progressBar.Margin = new System.Windows.Forms.Padding(8, 7, 8, 7);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(893, 55);
            this.progressBar.TabIndex = 0;
            // 
            // buttonClose
            // 
            this.buttonClose.Enabled = false;
            this.buttonClose.Location = new System.Drawing.Point(728, 246);
            this.buttonClose.Margin = new System.Windows.Forms.Padding(8, 7, 8, 7);
            this.buttonClose.Name = "buttonClose";
            this.buttonClose.Size = new System.Drawing.Size(200, 55);
            this.buttonClose.TabIndex = 2;
            this.buttonClose.Text = "Close";
            this.buttonClose.UseVisualStyleBackColor = true;
            this.buttonClose.Click += new System.EventHandler(this.buttonClose_Click);
            // 
            // textBox
            // 
            this.textBox.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox.ForeColor = System.Drawing.SystemColors.Highlight;
            this.textBox.Location = new System.Drawing.Point(35, 29);
            this.textBox.Margin = new System.Windows.Forms.Padding(8, 7, 8, 7);
            this.textBox.Multiline = true;
            this.textBox.Name = "textBox";
            this.textBox.ReadOnly = true;
            this.textBox.Size = new System.Drawing.Size(893, 88);
            this.textBox.TabIndex = 3;
            // 
            // ProgressDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(16F, 31F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(960, 322);
            this.Controls.Add(this.textBox);
            this.Controls.Add(this.buttonClose);
            this.Controls.Add(this.progressBar);
            this.Margin = new System.Windows.Forms.Padding(8, 7, 8, 7);
            this.Name = "ProgressDialog";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "ProgressDialog";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ProgressBar progressBar;
        private System.Windows.Forms.Button buttonClose;
        private System.Windows.Forms.TextBox textBox;
    }
}