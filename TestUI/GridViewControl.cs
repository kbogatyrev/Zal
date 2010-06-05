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

        public void AddLexeme (MainLib.IVerifier v)
        {
            DataGridViewRow r = new DataGridViewRow();
            r.CreateCells (dataGridView);
            r.Cells[0].Value = true;
            r.Cells[1].Value = v.LexemeId;
            r.Cells[1].Tag = v;
            r.Cells[2].Value = v.Headword;
            dataGridView.Rows.Add (r);
        }

        private void GridViewUserControl_VisibleChanged (object sender, EventArgs e)
        {
            this.Size = Parent.Size;
            dataGridView.Width = buttonRun.Location.X - 
                (Width - buttonRun.Width - buttonRun.Location.X);
            buttonRun.Focus();
        }

        private void buttonRun_Click (object sender, EventArgs e)
        {
            buttonRun.Enabled = false;
            foreach (DataGridViewRow row in dataGridView.Rows)
            {
                DataGridViewCheckBoxCell cbCheckBox = (DataGridViewCheckBoxCell)row.Cells[0];
                if (false == (bool)cbCheckBox.Value)
                {
                    continue;
                }

                DataGridViewCell cbStatus = row.Cells[3];
                cbStatus.Value = "Testing...";
            }

            foreach (DataGridViewRow row in dataGridView.Rows)
            {
                DataGridViewCheckBoxCell cbCheckBox = (DataGridViewCheckBoxCell)row.Cells[0];
                if (false == (bool)cbCheckBox.Value)
                {
                    continue;
                }

                DataGridViewCell cbId = (DataGridViewCell)row.Cells[1];
                MainLib.IVerifier v = (MainLib.IVerifier) cbId.Tag;
                MainLib.ET_TestResult eResult = v.Verify();
                DataGridViewCell cbResult = row.Cells[3];
                switch (eResult)
                {
                    case MainLib.ET_TestResult.TEST_RESULT_OK:
                    {
                        cbResult.Value = "Pass";
                        break;
                    }
                    case MainLib.ET_TestResult.TEST_RESULT_FAIL:
                    {
                        cbResult.Value = "Fail";
                        break;
                    }
                    case MainLib.ET_TestResult.TEST_RESULT_INCOMPLETE:
                    {
                        cbResult.Value = "Missing forms";
                        break;
                    }
                    default:
                    {
                        MessageBox.Show ("Unexpected return from IVerifier", 
                                         "Error", 
                                         MessageBoxButtons.OK);
                        return;
                    }

                }       //  switch ...

            }   //  foreach (DataGridViewRow row in dataGridView.Rows)

        }   // buttonRun_Click (...)

        private void buttonSave_Click (object sender, EventArgs e)
        {
            TestApplet ta = (TestApplet)Parent.Parent.Parent;
            ta.SaveTestResults();

            ((Button)sender).Enabled = false;
        }

        private void buttonCancel_Click (object sender, EventArgs e)
        {

        }

        private void buttonClose_Click (object sender, EventArgs e)
        {
            TestApplet ta = (TestApplet)Parent.Parent.Parent;
            ta.CloseCurrentTab();
        }

    }
}
