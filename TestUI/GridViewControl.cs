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

        public int iRows
        {
            get
            {
                return dataGridView.RowCount;
            }
        }

        public void SetResult (int iRow, string sText)
        {
            DataGridViewCell cell = dataGridView[3, iRow];
            cell.Value = sText;
        }

        public bool bRowChecked(int iRow)
        {
            DataGridViewCell cell = dataGridView[0, iRow];
            DataGridViewCheckBoxCell cbCheckBox = (DataGridViewCheckBoxCell)cell;
            return (bool)cbCheckBox.Value;
        }

        public uint ulLexemeHash (int iRow)
        {
            DataGridViewCell cell = dataGridView[1, iRow];
            DataGridViewTextBoxCell textBoxCell = (DataGridViewTextBoxCell)cell;
            return (uint)textBoxCell.Value;
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

        public GridViewUserControl (string sDbPath)
        {
            InitializeComponent();
            m_sDbPath = sDbPath;
        }

        public void AddLexeme (MainLib.IVerifier v)
        {
            DataGridViewRow r = new DataGridViewRow();
            r.CreateCells(dataGridView);
            r.Cells[0].Value = true;
            r.Cells[1].Value = (uint)v.LexemeId;
            r.Cells[1].Tag = v;
            r.Cells[2].Value = v.Headword;
//            m_LexemeHashToVerifier.Add(v.LexemeId, v);
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

            /*
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
                                    MainLib.IError err = (MainLib.IError)v;
                                    string sMsg = "Unexpected return from IVerifier; error msg: ";
                                    sMsg += err.LastError;
                                    MessageBox.Show (sMsg, "Error", MessageBoxButtons.OK);
                                    return;
                                }

                            }       //  switch ...

                        }   //  foreach (DataGridViewRow row in dataGridView.Rows)
            */
        }   // buttonRun_Click (...)

        private void buttonSave_Click(object sender, EventArgs e)
        {
            TestApplet ta = (TestApplet)Parent.Parent.Parent;
            ta.SaveTestResults();

            ((Button)sender).Enabled = false;
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {

        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            TestApplet ta = (TestApplet)Parent.Parent.Parent;
            ta.CloseCurrentTab();
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
                //                foreach (DataGridViewRow row in m_Caller.GridView.Rows)
                //                {
                //                    DataGridViewCheckBoxCell cbCheckBox = (DataGridViewCheckBoxCell)row.Cells[0];
                //                    if (false == (bool)cbCheckBox.Value)
                //                    {
                //                        continue;
                //                    }

                //                    DataGridViewCell cbStatus = row.Cells[3];
                //                    cbStatus.Value = "Testing...";
                //                }

                //                foreach (DataGridViewRow row in m_Caller.GridView.Rows)
                for (int iAt = 0; iAt < m_Caller.iRows; ++iAt)
                {
                    if (!m_Caller.bRowChecked(iAt))
                    {
                        continue;
                    }

                    uint uiLexemeHash = (uint)m_Caller.ulLexemeHash(iAt);
                    v = new MainLib.ZalVerifier();
                    v.DbPath = m_Caller.m_sDbPath;

                    MainLib.ET_TestResult eResult = v.Verify (uiLexemeHash);
                    switch (eResult)
                    {
                        case MainLib.ET_TestResult.TEST_RESULT_OK:
                        {
                            m_Caller.SetResult (iAt, "Pass");
                            break;
                        }
                        case MainLib.ET_TestResult.TEST_RESULT_FAIL:
                        {
                            m_Caller.SetResult (iAt, "Fail");
                            break;
                        }
                        case MainLib.ET_TestResult.TEST_RESULT_INCOMPLETE:
                        {
                            m_Caller.SetResult (iAt, "Missing forms");
                            break;
                        }
                        default:
                        {
                            MainLib.IError err = (MainLib.IError)v;
                            string sMsg = "Unexpected return from IVerifier; error msg: ";
                            sMsg += err.LastError;
                            MessageBox.Show(sMsg, "Error", MessageBoxButtons.OK);
                            return;
                        }

                    }       //  switch ...

                }   //  foreach (DataGridViewRow row in dataGridView.Rows)
            }
            catch (Exception ex)
            {
                MainLib.IError err = (MainLib.IError)v;
                string sMsg = ex.Message;
                sMsg += "\n";
                sMsg += err.LastError;
                MessageBox.Show (sMsg, "Error", MessageBoxButtons.OK);
                return;
            }

        }   //  ThreadProc()

    }   //  public class WorkerThread

}  //  namespace TestUI
