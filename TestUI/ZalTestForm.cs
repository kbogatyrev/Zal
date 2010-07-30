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

namespace TestUI
{
    public partial class TestApplet : Form
    {
        private MainLib.IDictionary m_Dictionary;
        private MainLib.ILexemeCollection m_TestData;
        private MainLib.IAnalyzer m_Analyzer;
        private List<MainLib.IWordForm> m_listWordForms;
        private Dictionary<LexemeDataPanel, MainLib.ILexeme> m_hashLexemes;

        private Dictionary<MainLib.ET_Gender, string> m_hashGender;
        private Dictionary<MainLib.ET_Number, string> m_hashNumber;
        private Dictionary<MainLib.ET_Case, string> m_hashCase;
        private Dictionary<MainLib.ET_Person, string> m_hashPerson;
        private Dictionary<MainLib.ET_AccentType, string> m_hashAccent;

        private string m_sDbPath;
        private bool m_bDBOpen;
        private string m_sSearchString;

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
                string sMsg = "byEntryFormToolStripMenuItem_Click: ";
                MainLib.IError err = (MainLib.IError)m_Dictionary;
                sMsg += err.LastError;
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

    }   //  public partial class TestApplet : Form
}
