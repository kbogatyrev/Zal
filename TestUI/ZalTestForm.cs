using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
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
        private Dictionary<MainLib.ET_AccentType, string> m_dictAccent;

        public TestApplet()
        {
            InitializeComponent();
            buttonLookup.Enabled = false;
            radioButtonInitForm.Checked = true;

            textBoxDBPath.Text = Properties.Settings.Default.DbPath;

            m_Dictionary = new MainLib.ZalDictionary();
            m_Analyzer = new MainLib.ZalAnalyzer();
            m_listLexemes = new List<MainLib.ILexeme>();
            m_listWordForms = new List<MainLib.IWordForm>();

            m_dictGender = new Dictionary<MainLib.ET_Gender,string>();
            m_dictGender.Add(MainLib.ET_Gender.GENDER_UNDEFINED, "Undefined");
            m_dictGender.Add(MainLib.ET_Gender.GENDER_M, "M");
            m_dictGender.Add(MainLib.ET_Gender.GENDER_F, "F");
            m_dictGender.Add(MainLib.ET_Gender.GENDER_N, "N");

            m_dictNumber = new Dictionary<MainLib.ET_Number,string>();
            m_dictNumber.Add(MainLib.ET_Number.NUM_UNDEFINED, "Undefined");
            m_dictNumber.Add(MainLib.ET_Number.NUM_SG, "Sg");
            m_dictNumber.Add(MainLib.ET_Number.NUM_PL, "Pl");

            m_dictCase = new Dictionary<MainLib.ET_Case,string>();
            m_dictCase.Add(MainLib.ET_Case.CASE_UNDEFINED, "Undefined");
            m_dictCase.Add(MainLib.ET_Case.CASE_NOM, "N");
            m_dictCase.Add(MainLib.ET_Case.CASE_ACC, "A");
            m_dictCase.Add(MainLib.ET_Case.CASE_GEN, "G");
            m_dictCase.Add(MainLib.ET_Case.CASE_LOC, "L");
            m_dictCase.Add(MainLib.ET_Case.CASE_DAT, "D");
            m_dictCase.Add(MainLib.ET_Case.CASE_INST, "I");

            m_dictAccent = new Dictionary<MainLib.ET_AccentType,string>();
            m_dictAccent.Add(MainLib.ET_AccentType.AT_UNDEFINED, "Undefined");
            m_dictAccent.Add(MainLib.ET_AccentType.AT_A, "a");
            m_dictAccent.Add(MainLib.ET_AccentType.AT_A1, "a'");
            m_dictAccent.Add(MainLib.ET_AccentType.AT_B, "b");
            m_dictAccent.Add(MainLib.ET_AccentType.AT_B1, "b'");
            m_dictAccent.Add(MainLib.ET_AccentType.AT_C, "c");
            m_dictAccent.Add(MainLib.ET_AccentType.AT_C1, "c'");
            m_dictAccent.Add(MainLib.ET_AccentType.AT_C2, "c''");
            m_dictAccent.Add(MainLib.ET_AccentType.AT_D, "d");
            m_dictAccent.Add(MainLib.ET_AccentType.AT_D1, "d'");
            m_dictAccent.Add(MainLib.ET_AccentType.AT_E, "e");
            m_dictAccent.Add(MainLib.ET_AccentType.AT_F, "f");
            m_dictAccent.Add(MainLib.ET_AccentType.AT_F1, "f'");
            m_dictAccent.Add(MainLib.ET_AccentType.AT_F2, "f''");
        }

        public void Subscribe (LexemeDataPanel ldp)
        {
            ldp.ShowDetailsEvent += new LexemeDataPanel.ShowDetails (LexemeDataPanel_ShowLexemeDetails);
        }

        public void LexemeDataPanel_ShowLexemeDetails (int iLexemeId)
        {
            MainLib.ILexeme lexeme = m_listLexemes[iLexemeId];
            lexeme.GenerateWordForms();

            TabPage tabPageDetails = new TabPage (lexeme.InitialForm);

            string grSt = lexeme.GraphicStem;

            if (MainLib.ET_PartOfSpeech.POS_NOUN == lexeme.PartOfSpeech)
            {
                NounPanel np = new NounPanel();
                tabPageDetails.Controls.Add(np);
                np.sLexName = grSt;

                foreach (MainLib.IWordForm wf in lexeme)
                {
                    string sKey = m_dictCase[wf.Case];
                    sKey += (wf.Number == MainLib.ET_Number.NUM_SG) ? "Sg" : "Pl";

                    string strWordForm = wf.Wordform;
                    if (wf.StressPos >= 0)
                    {
                        if (wf.StressPos >= wf.Wordform.Length)
                        {
                            MessageBox.Show ("Bad stress position", "Zal Error", MessageBoxButtons.OK);
                            return;
                        }
                        if (strWordForm[wf.StressPos] != 'ё')
                        {
                            string strStressMark = new string('\x301', 1);
                            strWordForm = strWordForm.Insert(wf.StressPos + 1, strStressMark);
                        }
                    }

                    np.SetForm (sKey, strWordForm);

                }   // foreach
            }

            if (MainLib.ET_PartOfSpeech.POS_ADJ == lexeme.PartOfSpeech)
            {
                AdjPanel ap = new AdjPanel();
                tabPageDetails.Controls.Add(ap);
                ap.sLexName = grSt;

                foreach (MainLib.IWordForm wf in lexeme)
                {
                    string sKey = "";

                    if (MainLib.ET_AdjForm.ADJ_FORM_LONG == wf.AdjForm)
                    {
                        if (MainLib.ET_Number.NUM_SG == wf.Number)
                        {
                            sKey = m_dictGender[wf.Gender];
                        }
                        sKey += m_dictCase[wf.Case];
                        sKey += (MainLib.ET_Number.NUM_SG == wf.Number) ? "Sg" : "Pl";
                        if (MainLib.ET_Case.CASE_ACC == wf.Case)
                        {
                            if ((MainLib.ET_Gender.GENDER_M == wf.Gender && 
                                 MainLib.ET_Number.NUM_SG == wf.Number) ||
                                 (MainLib.ET_Number.NUM_PL == wf.Number))
                            {
                                sKey += (MainLib.ET_Animacy.ANIM_YES == wf.Animacy) ? "Anim" : "Inanim";
                            }
                        }

                        string strWordForm = wf.Wordform;
                        if (wf.StressPos >= 0)
                        {
                            if (strWordForm[wf.StressPos] != 'ё')
                            {
                                string strStressMark = new string('\x301', 1);
                                strWordForm = strWordForm.Insert(wf.StressPos + 1, strStressMark);
                            }
                        }

                        ap.SetForm(sKey, strWordForm);

                        continue;
                    }

                    if (MainLib.ET_AdjForm.ADJ_FORM_SHORT == wf.AdjForm)
                    {
                        sKey = "Short";
                        if (MainLib.ET_Number.NUM_SG == wf.Number)
                        {
                            sKey += m_dictGender[wf.Gender];
                        }
                        sKey += m_dictNumber[wf.Number];

                        string strWordForm = wf.Wordform;
                        if (wf.StressPos >= 0)
                        {
                            if (strWordForm[wf.StressPos] != 'ё')
                            {
                                string strStressMark = new string('\x301', 1);
                                strWordForm = strWordForm.Insert(wf.StressPos + 1, strStressMark);
                            }
                        }

                        ap.SetForm(sKey, strWordForm);

                        continue;
                    }

                    if (MainLib.ET_AdjForm.ADJ_FORM_COMPARATIVE == wf.AdjForm)
                    {
                        sKey = "Comparative";

                        string strWordForm = wf.Wordform;
                        if (wf.StressPos >= 0)
                        {
                            if (strWordForm[wf.StressPos] != 'ё')
                            {
                                string strStressMark = new string('\x301', 1);
                                strWordForm = strWordForm.Insert(wf.StressPos + 1, strStressMark);
                            }
                        }

                        ap.SetForm(sKey, strWordForm);

                        continue;
                    }

                }   // foreach ...

            }   //  if (MainLib.ET_PartOfSpeech.POS_ADJ == lexeme.PartOfSpeech)

            tabControl.Controls.Add(tabPageDetails);

        }   //  LexemeDataPanel_ShowLexemeDetails (int iLexemeId)

        private void buttonLookup_Click(object sender, EventArgs e)
        {
            m_Dictionary.DbPath = textBoxDBPath.Text;
            m_Analyzer.DbPath = textBoxDBPath.Text;

            lexPanel.Controls.Clear();
            m_listLexemes.Clear();

            bool bSynthesis = true;

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
            else
            {
                if (radioButtonInitForm.Checked)
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
                else
                {
                    bSynthesis = false;
                    // Call Tim's analysis code here
                    int iWordform = 0;
                    m_Analyzer.Analyze(textBoxSearchString.Text);
                    foreach (MainLib.IWordForm wf in m_Analyzer)
                    {
                        m_listWordForms.Add(wf);
                        AnalysisPanel ap = new AnalysisPanel(iWordform);
                        //Subscribe(ldp);
                        ap.Location = new System.Drawing.Point(0, iWordform * ap.Size.Height + 4);
                        lexPanel.Controls.Add(ap);
                        iWordform++;
                    }
                    MessageBox.Show ("Analysis not yet implemented.", "Zal Test", MessageBoxButtons.OK);
                }
            }

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
                    Subscribe(ldp);
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
                    MessageBox.Show("File Does not exist",
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
