using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace TestUI
{
    public partial class GridViewUserControl : UserControl
    {
        Thread m_WorkerThread;
        public string m_sDbPath;
        public bool m_bCancelTest = false;

        public GridViewUserControl(string sDbPath)
        {
            InitializeComponent();
            m_sDbPath = sDbPath;
            checkBoxSelectAll.Checked = true;
        }

        public int iRows
        {
            get
            {
                return dataGridView.RowCount;
            }
        }

        public void SetResult (int iRow, string sText, bool bHightLight)
        {
            DataGridViewCell cell = dataGridView[3, iRow];
            cell.Value = sText;
            if (bHightLight)
            {
                cell.Style.ForeColor = Color.Red;
            }
        }

        public bool bRowChecked(int iRow)
        {
            DataGridViewCell cell = dataGridView[0, iRow];
            DataGridViewCheckBoxCell cbCheckBox = (DataGridViewCheckBoxCell)cell;
            return (bool)cbCheckBox.Value;
        }

        public string sLexemeHash (int iRow)
        {
            DataGridViewCell cell = dataGridView[1, iRow];
            DataGridViewTextBoxCell textBoxCell = (DataGridViewTextBoxCell)cell;
            return (string)textBoxCell.Value;
        }

        public MainLib.IVerifier Verifier(int iRow)
        {
            DataGridViewCell cell = dataGridView[1, iRow];
            return (MainLib.IVerifier)cell.Tag;
        }

        public string sHeadword(int iRow)
        {
            DataGridViewCell cell = dataGridView[2, iRow];
            DataGridViewTextBoxCell textBoxCell = (DataGridViewTextBoxCell)cell;
            return (string)textBoxCell.Value;
        }

        public void AddLexeme (MainLib.IVerifier v)
        {
            DataGridViewRow r = new DataGridViewRow();
            r.CreateCells(dataGridView);
            r.Cells[0].Value = true;
            r.Cells[1].Value = v.LexemeId;
//            r.Cells[1].Tag = v;
            r.Cells[2].Value = v.Headword;
            dataGridView.Rows.Add(r);
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
            buttonCancel.Enabled = true;

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

            WorkerThread wt = new WorkerThread(this);
            m_WorkerThread = new Thread(new ThreadStart(wt.ThreadProc));
            m_WorkerThread.Name = "Zal verify worker thread";
            m_WorkerThread.IsBackground = true;
            //              m_WorkerThread.Priority = ThreadPriority.Lowest;
            m_WorkerThread.SetApartmentState(ApartmentState.STA);
            m_WorkerThread.Start();
//            m_WorkerThread.Join();

        }   // buttonRun_Click (...)

        private void buttonSave_Click(object sender, EventArgs e)
        {
            TestApplet ta = (TestApplet)Parent.Parent.Parent;
            ta.SaveTestResults();

            ((Button)sender).Enabled = false;
        }

        private void buttonDelete_Click(object sender, EventArgs e)
        {
            List<DataGridViewRow> rowsToRemove = new List<DataGridViewRow>();
            foreach (DataGridViewRow r in dataGridView.Rows)
            {
                if (!bRowChecked(r.Index))
                {
                    continue;
                }

                try
                {
                    MainLib.ZalStoredLexemeData testData = new MainLib.ZalStoredLexemeData();
                    testData.DbPath = m_sDbPath;
                    testData.DeleteStoredLexeme(sLexemeHash(r.Index));
                    rowsToRemove.Add(r);
                }
                catch (Exception ex)
                {
                    MainLib.ZalError err = new MainLib.ZalError();
                    string sMsg = ex.Message;
                    sMsg += "\n";
                    sMsg += err.LastError;
                    MessageBox.Show (sMsg, "Error", MessageBoxButtons.OK);
                    return;
                }
            }   // foreach ...

            foreach (DataGridViewRow r in rowsToRemove)
            {
                dataGridView.Rows.Remove(r);
            }
            dataGridView.Update();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            m_bCancelTest = true;
        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            TestApplet ta = (TestApplet)Parent.Parent.Parent;
            ta.CloseCurrentTab();
        }

        private void checkBoxSelectAll_CheckedChanged(object sender, EventArgs e)
        {
            foreach (DataGridViewRow row in dataGridView.Rows)
            {
                DataGridViewCheckBoxCell cbCheckBox = (DataGridViewCheckBoxCell)row.Cells[0];
                cbCheckBox.Value = checkBoxSelectAll.Checked;
                buttonRun.Enabled = checkBoxSelectAll.Checked;
            }
        }

        private void dataGridView_CellValueChanged (object sender, DataGridViewCellEventArgs e)
        {
            if (0 == e.ColumnIndex)
            {
                if (true == (bool)dataGridView[e.ColumnIndex, e.RowIndex].Value)
                {
                    buttonRun.Enabled = true;
                }
            }
        }

        private void dataGridView_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            if (0 == e.ColumnIndex)
            {
                if (true == (bool)dataGridView[e.ColumnIndex, e.RowIndex].Value)
                {
                    buttonRun.Enabled = true;
                }
            }
        }

        private void dataGridView_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (0 == e.ColumnIndex)
            {
                if (true == (bool)dataGridView[e.ColumnIndex, e.RowIndex].Value)
                {
                    buttonRun.Enabled = true;
                }
            }
        }

        private void dataGridView_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            if (0 == e.ColumnIndex)
            {
                if (true == (bool)dataGridView[e.ColumnIndex, e.RowIndex].EditedFormattedValue)
                {
                    buttonRun.Enabled = true;
                }
                else
                {
                    foreach (DataGridViewRow row in dataGridView.Rows)
                    {
                        buttonRun.Enabled = false;
                        DataGridViewCell cell = row.Cells[0];
                        if ((bool)cell.EditedFormattedValue)
                        {
                            buttonRun.Enabled = true;
                        }
                    }
                }
            }
        }
    }       //  public partial class GridViewUserControl

    public class WorkerThread
    {
        private GridViewUserControl m_Caller;

        public WorkerThread(GridViewUserControl Caller)
        {
            m_Caller = Caller;
        }

        public void ThreadProc()
        {
            MainLib.IVerifier v = null;
            try
            {
                for (int iAt = 0; iAt < m_Caller.iRows; ++iAt)
                {
                    if (m_Caller.m_bCancelTest)
                    {
                        for (int iRow = iAt; iRow < m_Caller.iRows; ++iRow)
                        {
                            m_Caller.SetResult (iRow, "Cancelled", false);
                        }
                        break;
                    }

                    if (!m_Caller.bRowChecked(iAt))
                    {
                        continue;
                    }

                    string sLexemeHash = m_Caller.sLexemeHash(iAt);
                    v = new MainLib.ZalVerifier();
                    v.DbPath = m_Caller.m_sDbPath;

                    MainLib.ET_TestResult eResult = v.Verify (sLexemeHash);
                    switch (eResult)
                    {
                        case MainLib.ET_TestResult.TEST_RESULT_OK:
                        {
                            m_Caller.SetResult (iAt, "Pass", false);
                            break;
                        }
                        case MainLib.ET_TestResult.TEST_RESULT_FAIL:
                        {
                            m_Caller.SetResult (iAt, "Fail", true);
                            break;
                        }
                        case MainLib.ET_TestResult.TEST_RESULT_INCOMPLETE:
                        {
                            m_Caller.SetResult (iAt, "Missing forms", true);
                            break;
                        }
                        default:
                        {
                            MainLib.ZalError err = new MainLib.ZalError();
                            string sMsg = "Unexpected return from IVerifier; error msg: ";
                            sMsg += err.LastError;
                            MessageBox.Show(sMsg, "Zal Error", MessageBoxButtons.OK);
                            return;
                        }

                    }       //  switch ...

                }   //  foreach (DataGridViewRow row in dataGridView.Rows)
            }
            catch (Exception ex)
            {
                MainLib.ZalError err = new MainLib.ZalError();
                string sMsg = ex.Message;
                sMsg += "\n";
                sMsg += err.LastError;
                MessageBox.Show (sMsg, "Error", MessageBoxButtons.OK);
                return;
            }

        }   //  ThreadProc()

    }   //  public class WorkerThread

}  //  namespace TestUI
