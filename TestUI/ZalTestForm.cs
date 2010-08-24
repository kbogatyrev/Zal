﻿using System;
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

namespace TestUI
{
    public delegate void DelegateUpdateProgressBar (int iPercentDone);

    public partial class TestApplet : Form
    {
        private MainLib.IDictionary m_Dictionary;
        private MainLib.ZalStoredLexemeData m_TestData;
        private MainLib.IAnalyzer m_Analyzer;
        private List<MainLib.IWordForm> m_listWordForms;
        private Dictionary<LexemeDataPanel, MainLib.ILexeme> m_hashLexemes;
        private Dictionary<MainLib.ILexeme, ArrayList> m_lexemeToTabs;

        private Dictionary<MainLib.ET_Gender, string> m_hashGender;
        private Dictionary<MainLib.ET_Number, string> m_hashNumber;
        private Dictionary<MainLib.ET_Case, string> m_hashCase;
        private Dictionary<MainLib.ET_Person, string> m_hashPerson;
        private Dictionary<MainLib.ET_AccentType, string> m_hashAccent;

        ProgressDialog m_ProgressDlg;
        public DelegateUpdateProgressBar m_DelegateUpdateProgressBar;
        private string m_sDbPath;
        private bool m_bDBOpen;
        private string m_sSearchString;
//        private string m_sImportFilePath;

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

        public void VerbPanel_ShowParticipialForms (MainLib.ILexeme lexeme, 
                                                    MainLib.ET_Subparadigm eoSpLong, 
                                                    MainLib.ET_Subparadigm eoSpShort)
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
                MainLib.ILexeme lexeme = m_hashLexemes[ldpSource];
                lexeme.GenerateWordForms();
                lexeme.SaveTestData();
                MessageBox.Show ("Forms saved in the database.", "Zal", MessageBoxButtons.OK);
            }
            catch (Exception ex)
            {
                MainLib.IError err = (MainLib.IError)m_hashLexemes[ldpSource];
                string sMsg = "LexemeDataPanel_Save: ";
                sMsg += err.LastError;
                MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
                return;
            }
        }

        //private void buttonSelect_Click(object sender, EventArgs e)
        //{
        //}

        private void TestApplet_FormClosing(object sender, FormClosingEventArgs e)
        {
            Properties.Settings.Default.DbPath = m_sDbPath;
            Properties.Settings.Default.Save();
        }

        private void dBPathToolStripMenuItem_Click (object sender, EventArgs e)
        {
            GetDbPath();
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
                    m_Dictionary.GetLexemesByInitialForm (m_sSearchString);
                    ShowLexemes();
                }
            }
            catch (Exception ex)
            {
                string sMsg = "byEntryFormToolStripMenuItem_Click \n";
                MainLib.IError err = (MainLib.IError)m_Dictionary;
                sMsg += "MainLib error: ";
                sMsg += err.LastError;
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
                    m_Dictionary.GetLexemesByGraphicStem (dlg.sData);
                }
            }
            catch (Exception ex)
            {
                string sMsg = "Error: ";
                MainLib.IError err = (MainLib.IError)m_Dictionary;
                sMsg += err.LastError;
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
                    Preprocess (dlg.sData);
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
            try
            {
                EnterDataDlg dlg = new EnterDataDlg();
                dlg.Text = "Enter Word Form";
                DialogResult dr = dlg.ShowDialog();
                if (DialogResult.OK == dr)
                {
                    m_Analyzer.Analyze (dlg.sData);
                }
            }
            catch (Exception ex)
            {
                string sMsg = "Error: ";
                MainLib.IError err = (MainLib.IError)m_Analyzer;
                sMsg += err.LastError;
                MessageBox.Show(sMsg, "Zal Error", MessageBoxButtons.OK);
                return;
            }

            ShowParseOutput();

        }   //  parseWordFormToolStripMenuItem_Click (...)

        private void batchTestToolStripMenuItem_Click (object sender, EventArgs e)
        {
            if (!m_bDBOpen)
            {
                GetDbPath();
            }

            if (null == m_TestData)
            {
                m_TestData = new MainLib.ZalStoredLexemeData();
                EventSink sink = new EventSink (this);
                m_TestData.ProgressNotification += sink.ProgressNotification;
                m_TestData.DbPath = m_sDbPath;
            }

            m_TestData.LoadStoredLexemes();
            if (m_TestData.Count <= 0)
            {
                MessageBox.Show ("No test forms in the database", "Zal Test", MessageBoxButtons.OK);
                return;
            }

            TabPage tabPageTestCases = new TabPage ("Test");
            GridViewUserControl gv = new GridViewUserControl (m_sDbPath);

            tabPageTestCases.Controls.Add(gv);

            foreach (MainLib.IVerifier v in m_TestData)
            {
                gv.AddLexeme(v);
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

            if (null == m_TestData)
            {
                m_TestData = new MainLib.ZalStoredLexemeData();
                EventSink sink = new EventSink (this);
                m_TestData.ProgressNotification += sink.ProgressNotification;
                m_TestData.DbPath = m_sDbPath;
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
                        DialogResult dRes = MessageBox.Show(sMsg,
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
//                m_TestData.ExportStoredTestData(fd.FileName);
            }
            catch (Exception ex)
            {
                MainLib.IError err = (MainLib.IError)m_TestData;
                string sMsg = "exportTestDataToolStripMenuItem_Click: ";
                sMsg += err.LastError;
                MessageBox.Show(sMsg, "Zal Error", MessageBoxButtons.OK);
                return;
            }

        }  //  exportTestDataToolStripMenuItem_Click (...)

        private void importTestDataToolStripMenuItem_Click (object sender, EventArgs e)
        {
            if (!m_bDBOpen)
            {
                GetDbPath();
            }

            if (null == m_TestData)
            {
//                m_TestData = new MainLib.ZalStoredLexemeData();
//                EventSink sink = new EventSink(m_DelegateUpdateProgressBar);
//                m_TestData.ProgressNotification += sink.ProgressNotification;
//                m_TestData.DbPath = m_sDbPath;
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

/*
            try
            {
                m_TestData.ImportStoredTestData (fd.FileName);
            }
            catch (Exception ex)
            {
                MainLib.IError err = (MainLib.IError)m_TestData;
                string sMsg = "importTestDataToolStripMenuItem_Click: ";
                sMsg += err.LastError;
                MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
                return;
            }
*/
            m_ProgressDlg = new ProgressDialog();
            m_ProgressDlg.Text = "Table Import";
            string sMsg = "Importing stored test results from ";
            sMsg += fd.FileName;
            m_ProgressDlg.SetMessage (sMsg);
            m_ProgressDlg.StartPosition = FormStartPosition.CenterScreen;
            m_ProgressDlg.Show();
            m_DelegateUpdateProgressBar = new DelegateUpdateProgressBar (this.UpdateProgressBar);
            EventSink sink = new EventSink (this);
            ImportThread importThread = new ImportThread (this, fd.FileName);
            Thread t = new Thread (new ThreadStart (importThread.ThreadProc));
            t.Name = "Zal import worker thread";
            t.IsBackground = true;
            //              m_WorkerThread.Priority = ThreadPriority.Lowest;
            t.SetApartmentState(ApartmentState.STA);

            t.Start();
            t.Join();
            
//            m_ProgressDlg.Close();

        }   //  importTestDataToolStripMenuItem_Click (...)

        void UpdateProgressBar (int iProgress)
        {
            m_ProgressDlg.SetProgressBarPos (iProgress);
            m_ProgressDlg.Refresh();
        }

/*
        public void ImportThreadProc()
        {
            try
            {
                m_TestData.ImportStoredTestData(m_sImportFilePath);
            }
            catch (Exception ex)
            {
                MainLib.IError err = (MainLib.IError)m_TestData;
                string sMsg = "importTestDataToolStripMenuItem_Click: ";
                sMsg += err.LastError;
                MessageBox.Show(sMsg, "Zal Error", MessageBoxButtons.OK);
                return;
            }

        }   //  ThreadProc()
*/
    }   //  public partial class TestApplet : Form

    public class ImportThread
    {
        TestApplet m_Parent;
        string m_sImportFilePath;

        public ImportThread (TestApplet parent, string sPath)
        {
            m_Parent = parent;
            m_sImportFilePath = sPath;
        }

        public void ThreadProc()
        {
            MainLib.ZalSqliteWrapper sqlite = null;
            try
            {
                sqlite = new MainLib.ZalSqliteWrapper();
                EventSink sink = new EventSink (m_Parent);
                sqlite.ProgressNotification += sink.ProgressNotification;
                sqlite.DbPath = m_Parent.sDbPath;
                sqlite.ImportTable (m_sImportFilePath, "test_data");
//                m_iTestData.ImportStoredTestData (m_sImportFilePath);
            }
            catch (Exception ex)
            {
                MainLib.IError err = (MainLib.IError)sqlite;
                string sMsg = "importTestDataToolStripMenuItem_Click: ";
                sMsg += err.LastError;
                MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
                return;
            }

        }   //  ThreadProc()

    }   //  public class ImportThread

/*
    public class EventSink : MainLib.IZalNotification
    {
        private DelegateUpdateProgressBar m_UpdateProgressBar;
        public EventSink (DelegateUpdateProgressBar updateProgressBar)
        {
            m_UpdateProgressBar = updateProgressBar;
        }

        public void ProgressNotification (int iPercentDone)
        {
            m_UpdateProgressBar (iPercentDone);
        }

    }   // EventSink 
*/

    public class EventSink : MainLib.IZalNotification
    {
        private TestApplet m_Form;
        public EventSink(TestApplet Form)
        {
            m_Form = Form;
        }

        public void ProgressNotification (int iPercentDone)
        {
            if (m_Form.InvokeRequired)
            {
                m_Form.BeginInvoke (m_Form.m_DelegateUpdateProgressBar,
                                    new Object[] { iPercentDone });
            }
        }

    }   // EventSink 

}   //  namespace TestUI

/*
&&&&
[DllImport("ole32.dll")]
static extern int CoMarshalInterThreadInterfaceInStream([In] ref Guid riid,
   [MarshalAs(UnmanagedType.IUnknown)] object pUnk, out UCOMIStream ppStm);

[DllImport("ole32.dll")]
static extern int CoGetInterfaceAndReleaseStream(UCOMIStream pStm, [In] ref
   Guid riid, [MarshalAs(UnmanagedType.IUnknown)] out object ppv);
*/
