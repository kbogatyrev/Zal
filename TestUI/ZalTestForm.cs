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
        private List<MainLib.ILexeme> m_listLexemes;

        private Dictionary<MainLib.ET_Gender, string> m_dictGender;
        private Dictionary<MainLib.ET_Number, string> m_dictNumber;
        private Dictionary<MainLib.ET_Case, string> m_dictCase;
        private Dictionary<MainLib.ET_Person, string> m_dictPerson;
        private Dictionary<MainLib.ET_AccentType, string> m_dictAccent;

        private bool m_bDBOpen;

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

        public void VerbPanel_ShowParticipialForms (int iLexemeId, 
                                                    MainLib.ET_Subparadigm eoSpLong, 
                                                    MainLib.ET_Subparadigm eoSpShort)
        {
            ShowParticipialForms(iLexemeId, eoSpLong, eoSpShort);
        }

        public void LexemeDataPanel_ShowLexemeDetails (int iLexemeId)
        {
            ShowLexemeDetails (iLexemeId);
        }   //  LexemeDataPanel_ShowLexemeDetails (int iLexemeId)

        public void LexemeDataPanel_Save (int iLexemeId)
        {
            MainLib.ILexeme lexeme = m_listLexemes[iLexemeId];

            foreach (MainLib.IWordForm wf in lexeme)
            {
                wf.SaveTestData();
            }

        }   //  LexemeDataPanel_Save()
        
        private void buttonLookup_Click(object sender, EventArgs e)
        {
            if (m_bDBOpen == false)
            {
                m_Dictionary.DbPath = textBoxDBPath.Text;
                m_Analyzer.DbPath = textBoxDBPath.Text;
                m_bDBOpen = true;
            }

            lexPanel.Controls.Clear();
            m_listLexemes.Clear();

            bool bSynthesis = true;

            // <radioButtons>
            if (radioButtonGStem.Checked)
            {
                bSynthesis = true;
                try
                {
                    m_Dictionary.GetLexemesByGraphicStem(textBoxSearchString.Text);
                }
                catch (Exception ex)
                {
                    string sMsg = "Error in buttonLookup_Click: ";
                    sMsg += ex.Message;
                    MessageBox.Show(sMsg, "Zal Error", MessageBoxButtons.OK);
                    return;
                }
            }
            else if (radioButtonInitForm.Checked)
            {
                bSynthesis = true;
                try
                {
                    m_Dictionary.GetLexemesByInitialForm(textBoxSearchString.Text);
                }
                catch (Exception ex)
                {
                    string sMsg = "Error in buttonLookup_Click: ";
                    sMsg += ex.Message;
                    MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
                    return;
                }
            }
            else if (radioButtonWordForm.Checked)
            {
                bSynthesis = false;
                try
                {
                    m_Analyzer.Analyze(textBoxSearchString.Text);
                }
                catch (Exception ex)
                {
                    string sMsg = "Error in buttonLookup_Click: ";
                    sMsg += ex.Message;
                    MessageBox.Show(sMsg, "Zal Error", MessageBoxButtons.OK);
                    return;
                }

                ShowParseOutput();
            }
            else if (radioButtonPreprocess.Checked)
            {
                bSynthesis = false;
                try
                {
                    Preprocess (textBoxSearchString.Text);
                }
                catch (Exception ex)
                {
                    string sMsg = "Error in buttonLookup_Click: ";
                    sMsg += ex.Message;
                    MessageBox.Show(sMsg, "Zal Error", MessageBoxButtons.OK);
                    return;
                }
            }
            // </radioButtons>

            if (bSynthesis)
            {
                if (m_Dictionary.Count < 1)
                {
                    MessageBox.Show(this, "Not in the dictionary.", "Zal Synthesizer");
                    return;
                }

                int iLexeme = 0;
                foreach (MainLib.ILexeme lex in m_Dictionary)
                {
                    m_listLexemes.Add(lex);
                    LexemeDataPanel ldp = new LexemeDataPanel(iLexeme);
                    SubscribeToLexemeEvents (ldp);
                    ldp.Location = new System.Drawing.Point(0, iLexeme * ldp.Size.Height + 4);
                    ldp.sInitialForm = lex.InitialForm;
                    ldp.sGraphicStem = lex.GraphicStem;
                    ldp.sMainSymbol = lex.MainSymbol;
                    ldp.sType = lex.Type.ToString();
                    ldp.sStressType = m_dictAccent[lex.AccentType1];
                    if (lex.AccentType2 != MainLib.ET_AccentType.AT_UNDEFINED)
                    {
                        ldp.sStressType += "/" + m_dictAccent[lex.AccentType2];
                    }
                    lexPanel.Controls.Add(ldp);
                    ++iLexeme;
                }
            }     // if (bSynthesis)

        }   //  buttonLookup_Click (...)

        private void textBoxSearchString_TextChanged(object sender, EventArgs e)
        {
            if (textBoxSearchString.Text.Length > 0)
            {
                if (textBoxDBPath.Text.Length > 0)
                {
                    buttonLookup.Enabled = true;
                }
            }
            else
            {
                buttonLookup.Enabled = false;
            }
        }

        private void textBoxDBPath_TextChanged(object sender, EventArgs e)
        {
            m_bDBOpen = false;
            if (textBoxDBPath.Text.Length > 0)
            {
                if (textBoxSearchString.Text.Length > 0)
                {
                    buttonLookup.Enabled = true;
                }
            }
            else
            {
                buttonLookup.Enabled = false;
            }
        }

        private void buttonSelect_Click(object sender, EventArgs e)
        {
            FileDialog fd = new OpenFileDialog();
            fd.CheckFileExists = false;
            fd.CheckPathExists = true;
            m_bDBOpen = false;
            DialogResult dr = fd.ShowDialog();
            if (DialogResult.OK == dr)
            {
                string sDbPath = fd.FileName;
                if (File.Exists(fd.FileName))
                {
                    textBoxDBPath.Text = fd.FileName;
                    m_Analyzer.DbPath = fd.FileName;
                }
                else
                {
                    MessageBox.Show ("File Does not exist",
                                     "Zal Test Error",
                                     MessageBoxButtons.OK,
                                     MessageBoxIcon.Exclamation);
                }
            }
        }

        private void TestApplet_FormClosing(object sender, FormClosingEventArgs e)
        {
            Properties.Settings.Default.DbPath = textBoxDBPath.Text;
            Properties.Settings.Default.Save();
        }
    }
}
