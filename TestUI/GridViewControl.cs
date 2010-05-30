using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TestUI
{
    public partial class GridViewUserControl : UserControl
    {
        public GridViewUserControl()
        {
            InitializeComponent();
        }

        public void AddLexeme (int iId, string sSource)
        {
            DataGridViewRow r = new DataGridViewRow();
            r.CreateCells (dataGridView);
            r.Cells[0].Value = true;
            r.Cells[1].Value = iId;
            r.Cells[1].Tag = iId;
            r.Cells[2].Value = sSource;
            dataGridView.Rows.Add (r);
        }

        private void GridViewUserControl_VisibleChanged (object sender, EventArgs e)
        {
            this.Size = Parent.Size;
            dataGridView.Width = buttonRun.Location.X - 
                (Width - buttonRun.Width - buttonRun.Location.X);
        }

        private void buttonRun_Click (object sender, EventArgs e)
        {
            MainLib.IDictionary dict = new MainLib.ZalDictionary();
            foreach (DataGridViewRow row in dataGridView.Rows)
            {
                DataGridViewCheckBoxCell cbCheckBox = (DataGridViewCheckBoxCell)row.Cells[0];
                if (false == (bool)cbCheckBox.Value)
                {
                    continue;
                }

                DataGridViewCell cbId = (DataGridViewCell)row.Cells[1];
                int iLexemeId = (int)cbId.Tag;
                TestApplet ta = (TestApplet)Parent.Parent.Parent;
                bool bRet = ta.VerifyLexeme (iLexemeId);
                DataGridViewCell cbResult = row.Cells[3];
                cbResult.Value = bRet ? "Pass" : "Fail";
            }
        }

        private void buttonSave_Click(object sender, EventArgs e)
        {

        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {

        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
        }

    }
}
