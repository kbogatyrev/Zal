using System;
using System.Collections.Generic;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using System.IO;
using System.Threading;

using MainLibManaged;

namespace TestUI
{
//    public delegate void DelegateSetProgressBar (int iStart, int iRange);
//    public delegate void DelegateUpdateProgressBar (int iPercentDone);

    public enum etDbOperation
    {
        eDbOperationUndefined,
        eExportTable,
        eImportTable
    };

    public partial class TestApplet : Form
    {
        public CDictionaryManaged m_Dictionary = new CDictionaryManaged();
        public CParserManaged m_Parser = null;
        public CVerifierManaged m_Verifier = null;
        private List<CWordFormManaged> m_listWordForms;
        private Dictionary<LexemeDataPanel, CLexemeManaged> m_hashLexemes;
        private Dictionary<CLexemeManaged, ArrayList> m_lexemeToTabs;

        private Dictionary<EM_Gender, string> m_hashGender;
        private Dictionary<EM_Number, string> m_hashNumber;
        private Dictionary<EM_Case, string> m_hashCase;
        private Dictionary<EM_Person, string> m_hashPerson;
        private Dictionary<EM_AccentType, string> m_hashAccent;

        public ProgressDialog m_ProgressDlg;
//        public DelegateUpdateProgressBar m_DelegateUpdateProgressBar;
//        public DelegateSetProgressBar m_DelegateSetProgressBar;
        private string m_sDbPath;
        private bool m_bDBOpen;
        private string m_sSearchString;

        public string sDbPath
        {
            get
            {
                return m_sDbPath;
            }
        }

        public TestApplet()
        {
            InitializeComponent();
            InitializeData();
        }

        public void SubscribeToLexemeEvents (LexemeDataPanel ldp)
        {
            ldp.ShowDetailsEvent += new LexemeDataPanel.ShowDetails (LexemeDataPanel_ShowLexemeDetails);
            ldp.SaveEvent += new LexemeDataPanel.Save (LexemeDataPanel_Save);
        }

        public void SubscribeToVerbEvents (VerbPanel vp)
        {
            vp.ShowParticipialFormsEvent += new VerbPanel.ShowParticipialForms (VerbPanel_ShowParticipialForms);
        }

        public void VerbPanel_ShowParticipialForms (CLexemeManaged lexeme, 
                                                    EM_Subparadigm eoSpLong, 
                                                    EM_Subparadigm eoSpShort)
        {
            ShowParticipialForms (lexeme, eoSpLong, eoSpShort);
        }

        public void LexemeDataPanel_ShowLexemeDetails (LexemeDataPanel ldpSource)
        {
            ShowLexemeDetails (ldpSource);
        }

        public void LexemeDataPanel_Save (LexemeDataPanel ldpSource)
        {
            try
            {
                CLexemeManaged lexeme = m_hashLexemes[ldpSource];
                lexeme.eGenerateParadigm();
                lexeme.eSaveTestData();
                MessageBox.Show ("Forms saved in the database.", "Zal", MessageBoxButtons.OK);
            }
            catch (Exception ex)
            {
//                MainLib.ZalError err = new MainLib.ZalError();
                string sMsg = "LexemeDataPanel_Save: ";
                sMsg += ex.Message;
                MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
                return;
            }
        }

        public void Verify(GridViewUserControl gv)
        {
            VerifierThread vt = new VerifierThread(gv, m_Verifier);
            Thread t = new Thread (new ThreadStart (vt.ThreadProc));
            t.Name = "Zal verifier thread";
            t.IsBackground = true;
            //              m_WorkerThread.Priority = ThreadPriority.Lowest;
            t.SetApartmentState(ApartmentState.STA);
            t.Start();
            t.Join();
        }

        //private void buttonSelect_Click(object sender, EventArgs e)
        //{
        //}

        private void TestApplet_FormClosing(object sender, FormClosingEventArgs e)
        {
            Properties.Settings.Default.DbPath = m_sDbPath;
            Properties.Settings.Default.Save();

            m_Dictionary.Dispose();
        }

        private void dBPathToolStripMenuItem_Click (object sender, EventArgs e)
        {
            GetDbPath();
            m_Dictionary.eSetDbPath(m_sDbPath);
        }


        private void FreqWFListToolStripMenuItem_Click(object sender, EventArgs e)
        {
          string str_FreqWfFilePath = "";
          OpenFileDialog fDialog = new OpenFileDialog();
          fDialog.Title = "Open frequent wordforms list";
          fDialog.Filter = "Файлы txt|*.txt|Все|*";
          if (fDialog.ShowDialog() == DialogResult.OK)
          {
            str_FreqWfFilePath = fDialog.FileName.ToString();
          }
          else
          {
            return;
          }
//          m_TextAnalyzer.LoadFrequent(str_FreqWfFilePath);
        }

        private void byEntryFormToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                EnterDataDlg enterDataDlg = new EnterDataDlg();
                enterDataDlg.Text = "Enter Source Form";
                DialogResult dr = enterDataDlg.ShowDialog();
                if (DialogResult.OK == dr)
                {
                    m_sSearchString = enterDataDlg.sData;
                    m_Dictionary.eGetLexemesByInitialForm (m_sSearchString);
                    ShowLexemes();
                }
            }
            catch (Exception ex)
            {
                string sMsg = "byEntryFormToolStripMenuItem_Click \n";
//                MainLib.ZalError err = new MainLib.ZalError();
                sMsg += "MainLib error: ";
//                sMsg += err.LastError;
                sMsg += "\n";
                sMsg += ".Net client error: ";
                sMsg += ex.Message;
                MessageBox.Show(sMsg, "Zal Error", MessageBoxButtons.OK);
                return;
            }

        }   //  byEntryFormToolStripMenuItem_Click 

        private void byGraphicStemToolStripMenuItem_Click (object sender, EventArgs e)
        {
            try
            {
                EnterDataDlg dlg = new EnterDataDlg();
                dlg.Text = "Enter Graphic Stem";
                DialogResult dr = dlg.ShowDialog();
                if (DialogResult.OK == dr)
                {
                    m_Dictionary.eGetLexemesByGraphicStem (dlg.sData);
                }
            }
            catch (Exception ex)
            {
                string sMsg = "Error: ";
//                MainLib.ZalError err = new MainLib.ZalError();
//                sMsg += err.LastError;
                sMsg = ex.Message;
                MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
                return;
            }

        }   //  byGraphicStemToolStripMenuItem_Click (...)

        private void preprocessToolStripMenuItem_Click (object sender, EventArgs e)
        {
            try
            {
                EnterDataDlg dlg = new EnterDataDlg();
                dlg.Text = "Enter Data";
                DialogResult dr = dlg.ShowDialog();
                if (DialogResult.OK == dr)
                {
//                    Preprocess (dlg.sData);
                }
            }
            catch (Exception ex)
            {
                string sMsg = "Error in buttonLookup_Click: ";
                sMsg += ex.Message;
                MessageBox.Show(sMsg, "Zal Error", MessageBoxButtons.OK);
                return;
            }

        }   //  preprocessToolStripMenuItem_Click (...)

        private void parseWordFormToolStripMenuItem_Click (object sender, EventArgs e)
        {
            EM_ReturnCode eRet = EM_ReturnCode.H_NO_ERROR;
            try
            {
                EnterDataDlg dlg = new EnterDataDlg();
                dlg.Text = "Enter Word Form";
                DialogResult dr = dlg.ShowDialog();
                if (DialogResult.OK == dr)
                {
                    if (null == m_Parser)
                    {
                        eRet = m_Dictionary.eGetParser(ref m_Parser);
                        if (eRet != EM_ReturnCode.H_NO_ERROR)
                        {
                            MessageBox.Show("Unable to retrieve IParser interface.", "Zal Error", MessageBoxButtons.OK);
                            return;
                        }
                    }
                    eRet = m_Parser.eAnalyze (dlg.sData);
                }
                else
                {
                    return;
                }
            }
            catch (Exception ex)
            {
                string sMsg = "Error: ";
//                MainLib.ZalError err = new MainLib.ZalError();
//                sMsg += err.LastError;
                MessageBox.Show(sMsg, "Zal Error", MessageBoxButtons.OK);
                return;
            }

            if (EM_ReturnCode.H_NO_ERROR == eRet)
            {
                ShowParseOutput();
            }
            else
            {
                MessageBox.Show("Form not found.", "Zal", MessageBoxButtons.OK);
            }

        }   //  parseWordFormToolStripMenuItem_Click (...)

/*
        private void parseTextToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Choose the new file path
            string str_FilePath = "";
            OpenFileDialog fDialog = new OpenFileDialog();
            fDialog.Title = "Open Raw Text";
            fDialog.Filter = "Text files|*.txt|All files|*";
            fDialog.InitialDirectory = @"\";
            if (fDialog.ShowDialog() == DialogResult.OK)
            {
                str_FilePath = fDialog.FileName.ToString();
                int i_wf = m_TextAnalyzer.i_AnalyzeText(str_FilePath);
                if (i_wf >= 0)
                {
                    MessageBox.Show("The file has been parsed successfully, " + i_wf.ToString() + " wordforms processed.");
                }
                else
                {
                    MessageBox.Show("An error occurred during parsing");
                }
            }
            else
            {
                return;
            }
        }   //  parseTextToolStripMenuItem_Click (...)
*/

        private void batchTestToolStripMenuItem_Click (object sender, EventArgs e)
        {
            if (!m_bDBOpen)
            {
                GetDbPath();
            }

            EM_ReturnCode eRet = m_Dictionary.eGetVerifier(ref m_Verifier);
            if (eRet != EM_ReturnCode.H_NO_ERROR)
            {
                MessageBox.Show("Unable to load Verifier.");
                return;
            }

            eRet = m_Verifier.eLoadStoredLexemes();
            if (eRet != EM_ReturnCode.H_NO_ERROR)
            {
                MessageBox.Show("Unable to load stored data.");
                return;
            }
            
            if (m_Verifier.iCount() <= 0)
            {
                MessageBox.Show ("No test forms in the database", "Zal Test", MessageBoxButtons.OK);
                return;
            }

            TabPage tabPageTestCases = new TabPage ("Test");
            GridViewUserControl gv = new GridViewUserControl (m_sDbPath);
            gv.m_Parent = this;

            tabPageTestCases.Controls.Add(gv);

            string sLexemeHash = null;
            string sHeadword = null;

            gv.Clear();

            eRet = m_Verifier.eGetFirstLexemeData(ref sLexemeHash, ref sHeadword);
            while (EM_ReturnCode.H_NO_ERROR == eRet)
            {
                gv.AddLexeme(sHeadword, sLexemeHash);
                eRet = m_Verifier.eGetNextLexemeData(ref sLexemeHash, ref sHeadword);
            }

            tabControl.Controls.Add (tabPageTestCases);
            tabControl.SelectTab (tabPageTestCases);

        }   //  batchTestToolStripMenuItem_Click (...)

        private void testRangeToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void exportTestDataToolStripMenuItem_Click (object sender, EventArgs e)
        {
            if (!m_bDBOpen)
            {
                GetDbPath();
            }

            FileDialog fd = new OpenFileDialog();
            fd.CheckFileExists = false;
            fd.CheckPathExists = true;
            fd.Filter = "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*";
            fd.FileName = "";
            while (0 == fd.FileName.Length)
            {
                DialogResult dr = fd.ShowDialog();
                if (DialogResult.OK == dr)
                {
                    if (File.Exists(fd.FileName))
                    {
                        string sMsg = "File ";
                        sMsg += fd.FileName + " ";
                        sMsg += "already exists. Do you want to overwrite it?";
                        DialogResult dRes = MessageBox.Show (sMsg,
                                                             "Zal Test Export",
                                                             MessageBoxButtons.YesNoCancel,
                                                             MessageBoxIcon.Question);
                        switch (dRes)
                        {
                            case DialogResult.Yes:
                                break;
                            case DialogResult.No:
                                fd.FileName = "";
                                break;
                            case DialogResult.Cancel:
                                return;
                            default:
                                sMsg = "Illegal dialog return.";
                                MessageBox.Show(sMsg, "Zal Error", MessageBoxButtons.OK);
                                return;

                        }       // switch
                    }
                    //                    else
                    //                    {
                    //                        return;
                    //                    }
                }
                else
                {
                    return;
                }
            }       // while ...

            try
            {
                m_ProgressDlg = new ProgressDialog();
                m_ProgressDlg.Text = "Table Export";
                string sMsg = "Exporting test results to ";
                sMsg += fd.FileName;
                m_ProgressDlg.SetMessage(sMsg);
                m_ProgressDlg.StartPosition = FormStartPosition.CenterScreen;
                m_ProgressDlg.Show();
//                m_DelegateUpdateProgressBar = new DelegateUpdateProgressBar (this.UpdateProgressBar);
                DbOperationThread exportThread = new DbOperationThread (etDbOperation.eExportTable, this, fd.FileName);
                Thread t = new Thread (new ThreadStart (exportThread.ThreadProc));
                t.Name = "Zal export worker thread";
                t.IsBackground = true;
                //              m_WorkerThread.Priority = ThreadPriority.Lowest;
                t.SetApartmentState (ApartmentState.STA);

                t.Start();
//                t.Join();

                // m_ProgressDlg.Close();
            }
            catch (Exception ex)
            {
//                MainLib.ZalError err = new MainLib.ZalError();
                string sMsg = "exportTestDataToolStripMenuItem_Click: ";
                sMsg += ex.Message;
                sMsg += "\n";
//                sMsg += err.LastError;
                MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
                return;
            }

        }  //  exportTestDataToolStripMenuItem_Click (...)

        private void importTestDataToolStripMenuItem_Click (object sender, EventArgs e)
        {
            if (!m_bDBOpen)
            {
                GetDbPath();
            }

            FileDialog fd = new OpenFileDialog();
            fd.CheckFileExists = true;
            fd.CheckPathExists = true;
            fd.Filter = "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*";
            fd.FileName = "";
            while (0 == fd.FileName.Length)
            {
                DialogResult dr = fd.ShowDialog();
                if (DialogResult.OK != dr)
                {
                    return;
                }
            }       // while ...

            m_ProgressDlg = new ProgressDialog();
            m_ProgressDlg.Text = "Table Import";
            string sTxt = "Importing stored test results from ";
            sTxt += fd.FileName;
            m_ProgressDlg.SetMessage (sTxt);
            m_ProgressDlg.StartPosition = FormStartPosition.CenterScreen;
            m_ProgressDlg.Show();
//            m_DelegateUpdateProgressBar = new DelegateUpdateProgressBar (this.UpdateProgressBar);
            DbOperationThread importThread = new DbOperationThread (etDbOperation.eImportTable, this, fd.FileName);
            Thread t = new Thread (new ThreadStart (importThread.ThreadProc));
            t.Name = "Zal import worker thread";
            t.IsBackground = true;
            //              m_WorkerThread.Priority = ThreadPriority.Lowest;
            t.SetApartmentState (ApartmentState.STA);

            t.Start();
//            t.Join();
            
//            m_ProgressDlg.Close();

        }   //  importTestDataToolStripMenuItem_Click (...)

        public void UpdateProgressBar (int iProgress)
        {
            if (m_ProgressDlg.InvokeRequired)
            {
                m_ProgressDlg.Invoke(new Action(() => 
                {
                    m_ProgressDlg.SetProgressBarPos(iProgress);
                    m_ProgressDlg.Refresh();
                }));
            }
            else
            {
                m_ProgressDlg.SetProgressBarPos(iProgress);
                m_ProgressDlg.Refresh();
            }
        }

    }   //  public partial class TestApplet : Form

    public class VerifierThread
    {
        private GridViewUserControl m_Caller;
        private CVerifierManaged m_Verifier;

        public VerifierThread(GridViewUserControl caller, CVerifierManaged verifier)
        {
            m_Caller = caller;
            m_Verifier = verifier;
        }

        public void ThreadProc()
        {
            try
            {
                for (int iAt = 0; iAt < m_Caller.iRows; ++iAt)
                {
                    if (m_Caller.m_bCancelTest)
                    {
                        for (int iRow = iAt; iRow < m_Caller.iRows; ++iRow)
                        {
                            m_Caller.SetResult(iRow, "Cancelled", false);
                        }
                        break;
                    }

                    if (!m_Caller.bRowChecked(iAt))
                    {
                        continue;
                    }

                    string sLexemeHash = m_Caller.sLexemeHash(iAt);
                    EM_ReturnCode eRet = m_Verifier.eVerify(sLexemeHash);
                    if (eRet != EM_ReturnCode.H_NO_ERROR)
                    {
                        MessageBox.Show (string.Format("Unable to verify lexeme hash {0}", sLexemeHash));
                    }

                    EM_TestResult eTestResult = m_Verifier.eResult();
                    switch (eTestResult)
                    {
                        case EM_TestResult.TEST_RESULT_OK:
                        {
                            m_Caller.SetResult (iAt, "Pass", false);
                            break;
                        }
                        case EM_TestResult.TEST_RESULT_FAIL:
                        {
                            m_Caller.SetResult (iAt, "Fail", true);
                            break;
                        }
                        case EM_TestResult.TEST_RESULT_INCOMPLETE:
                        {
                            m_Caller.SetResult (iAt, "Missing forms", true);
                            break;
                        }
                        default:
                        {
//                            MainLib.ZalError err = new MainLib.ZalError();
                            string sMsg = "Unexpected return from IVerifier; error msg: ";
//                            sMsg += err.LastError;
                            sMsg += eTestResult.ToString();
                            MessageBox.Show(sMsg, "Zal Error", MessageBoxButtons.OK);
                            return;
                        }

                    }       //  switch ...

                }   //  foreach (DataGridViewRow row in dataGridView.Rows)
            }
            catch (Exception ex)
            {
                //                MainLib.ZalError err = new MainLib.ZalError();
                string sMsg = ex.Message;
                sMsg += "\n";
                //                sMsg += err.LastError;
                MessageBox.Show(sMsg, "Error", MessageBoxButtons.OK);
                return;
            }

        }   //  ThreadProc()

    }   //  public class WorkerThread

    public class DbOperationThread
    {
        etDbOperation m_eOperationType;
        TestApplet m_formParent;
        string m_sPath;
        private delegate void Progress(int iPercentDone);
        Progress m_Progress;

        public DbOperationThread (etDbOperation eOpType, TestApplet formParent, string sPath)
        {
            m_eOperationType = eOpType;
            m_formParent = formParent;
            m_sPath = sPath;
            m_Progress = m_formParent.UpdateProgressBar;
        }

        public void ThreadProc()
        {            
            try
            {
                MainLibManaged.DelegateProgress DelegateProgress = new MainLibManaged.DelegateProgress(m_formParent.UpdateProgressBar);

                EM_ReturnCode eRet = EM_ReturnCode.H_NO_ERROR;

                if (etDbOperation.eExportTable == m_eOperationType)
                {
                    eRet = m_formParent.m_Dictionary.eExportTestData(m_sPath, DelegateProgress);
                }
                else if (etDbOperation.eImportTable == m_eOperationType)
                {
                    eRet = m_formParent.m_Dictionary.eImportTestData(m_sPath, DelegateProgress);
                }
                else
                {
                    MessageBox.Show ("Internal error: bad DB opcode", 
                                     "Zal Error", 
                                     MessageBoxButtons.OK);
                    return;
                }

                if (eRet < 0)
                {
                    string sMsg = "Database error.";
                    MessageBox.Show(sMsg, "Zal Error", MessageBoxButtons.OK);
                }
            }
            catch (Exception ex)
            {
                string sMsg = "importTestDataToolStripMenuItem_Click: ";
                sMsg += ex.Message;
                sMsg += "\n";
                MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
                return;
            }

        }   //  ThreadProc()

    }   //  public class ImportThread

}   //  namespace TestUI
