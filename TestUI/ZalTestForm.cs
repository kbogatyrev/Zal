using System;
using System.Collections.Generic;
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
        private MainLib.IAnalyzer m_Analyzer;
        private List<MainLib.IWordForm> m_listWordForms;
        private Dictionary<LexemeDataPanel, MainLib.ILexeme> m_dictLexemes;

        private Dictionary<MainLib.ET_Gender, string> m_dictGender;
        private Dictionary<MainLib.ET_Number, string> m_dictNumber;
        private Dictionary<MainLib.ET_Case, string> m_dictCase;
        private Dictionary<MainLib.ET_Person, string> m_dictPerson;
        private Dictionary<MainLib.ET_AccentType, string> m_dictAccent;

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
            MainLib.ILexeme lexeme = m_dictLexemes[ldpSource];

            foreach (MainLib.IWordForm wf in lexeme)
            {
                wf.SaveTestData();
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
            FileDialog fd = new OpenFileDialog ();
            fd.CheckFileExists = false;
            fd.CheckPathExists = true;
            fd.FileName = m_sDbPath;
            fd.Filter = "SQLite3 Files (*.db3)|*.db3|SQLite Files (*.db)|*.db|All Files (*.*)|*.*";
            m_bDBOpen = false;
            DialogResult dr = fd.ShowDialog();
            if (DialogResult.OK == dr)
            {
                if (File.Exists (fd.FileName))
                {
                    m_sDbPath = fd.FileName;
                    m_Dictionary.DbPath = m_sDbPath;
                    m_Analyzer.DbPath = m_sDbPath;
                    toolStripStatusLabel.Text = m_sDbPath;

// TODO path validation

                    synthesizeToolStripMenuItem.Enabled = true;
                    analyzeToolStripMenuItem.Enabled = true;
                    testToolStripMenuItem.Enabled = true;
                }
                else
                {
                    MessageBox.Show ("File Does not exist",
                                     "Zal Test Error",
                                     MessageBoxButtons.OK,
                                     MessageBoxIcon.Exclamation);
                }
            }

        }   //  dBPathToolStripMenuItem_Click (...)

        private void byEntryFormToolStripMenuItem_Click (object sender, EventArgs e)
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
                sMsg += ex.Message;
                MessageBox.Show(sMsg, "Zal Error", MessageBoxButtons.OK);
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
                sMsg += ex.Message;
                MessageBox.Show(sMsg, "Zal Error", MessageBoxButtons.OK);
                return;
            }

            ShowParseOutput();

        }   //  parseWordFormToolStripMenuItem_Click (...)

        private void batchTestToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void ShowLexemes()
        {
            if (m_Dictionary.Count < 1)
            {
                MessageBox.Show(this, "Not in the dictionary.", "Zal Synthesizer");
                return;
            }

            TabPage tabPageLexemes = new TabPage (m_sSearchString);
            tabPageLexemes.AutoScroll = true;

            LexemeDataPanel ldpFocused = null;

            int iLexeme = 0;
            foreach (MainLib.ILexeme lex in m_Dictionary)
            {
                LexemeDataPanel ldp = new LexemeDataPanel();
                m_dictLexemes.Add (ldp, lex);
                SubscribeToLexemeEvents (ldp);
                ldp.Location = new System.Drawing.Point (0, iLexeme * ldp.Size.Height + 4);
                ldp.sInitialForm = lex.InitialForm;
                ldp.sGraphicStem = lex.GraphicStem;
                ldp.sMainSymbol = lex.MainSymbol;
                ldp.sType = lex.Type.ToString();
                ldp.sStressType = m_dictAccent[lex.AccentType1];
                if (lex.AccentType2 != MainLib.ET_AccentType.AT_UNDEFINED)
                {
                    ldp.sStressType += "/" + m_dictAccent[lex.AccentType2];
                }

                tabPageLexemes.Controls.Add (ldp);
                ldp.Left += 20;
                ldp.Top += 20;
                if (0 == iLexeme)
                {
                    ldpFocused = ldp;
                }
                ++iLexeme;
            }

            tabControl.Controls.Add (tabPageLexemes);
            tabControl.SelectTab(tabPageLexemes);
            ldpFocused.FocusShowButton();

        }   // ShowLexemes()

    }   //  public partial class TestApplet : Form
}
