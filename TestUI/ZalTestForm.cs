using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TestUI
{
    public partial class Synthesizer : Form
    {
        private MainLib.IDictionary m_Dictionary;
        private List<MainLib.ILexeme> m_listLexemes;
        private Dictionary<MainLib.ET_Gender, string> m_dictGender;
        private Dictionary<MainLib.ET_Number, string> m_dictNumber;
        private Dictionary<MainLib.ET_Case, string> m_dictCase;
        private Dictionary<MainLib.ET_AccentType, string> m_dictAccent;

        public Synthesizer()
        {
            InitializeComponent();
            buttonLookup.Enabled = false;
            radioButtonInitForm.Checked = true;
            m_Dictionary = new MainLib.ZalDictionary();
            m_listLexemes = new List<MainLib.ILexeme>();

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
                    np.setForm (sKey, wf.Wordform);

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
                    if (MainLib.ET_Number.NUM_SG == wf.Number)
                    {
                        sKey = m_dictGender[wf.Gender];
                    }
                    sKey += m_dictCase[wf.Case];
                    sKey += (MainLib.ET_Number.NUM_SG == wf.Number) ? "Sg" : "Pl";
                    if (MainLib.ET_Case.CASE_ACC == wf.Case)
                    {
                        if ((MainLib.ET_Gender.GENDER_M == wf.Gender && MainLib.ET_Number.NUM_SG == wf.Number) ||
                            (MainLib.ET_Number.NUM_PL == wf.Number))
                        {
                            sKey += (MainLib.ET_Animacy.ANIM_YES == wf.Animacy) ? "Anim" : "Inanim";
                        }
                    }
                    ap.setForm (sKey, wf.Wordform);

                }   // foreach ...

            }   //  if (MainLib.ET_PartOfSpeech.POS_ADJ == lexeme.PartOfSpeech)

            tabControl.Controls.Add(tabPageDetails);

        }   //  LexemeDataPanel_ShowLexemeDetails (int iLexemeId)

        private void buttonLookup_Click(object sender, EventArgs e)
        {
            lexPanel.Controls.Clear();
            m_listLexemes.Clear();

            if (radioButtonGStem.Checked)
            {
                m_Dictionary.GetLexemesByGraphicStem (textBoxSearchString.Text);
            }
            else
            {
                m_Dictionary.GetLexemesByInitialForm (textBoxSearchString.Text);
            }

            if (m_Dictionary.Count < 1)
            {
                MessageBox.Show (this, "Not in the dictionary.", "Zal Synthesizer");
            }

            int iLexeme = 0;
            foreach (MainLib.ILexeme lex in m_Dictionary)
            {
                m_listLexemes.Add (lex);
                LexemeDataPanel ldp = new LexemeDataPanel (iLexeme);
                Subscribe(ldp);
                ldp.Location = new System.Drawing.Point (0, iLexeme*ldp.Size.Height+4);
                ldp.sInitialForm = lex.InitialForm;
                ldp.sGraphicStem = lex.GraphicStem;
                ldp.sMainSymbol = lex.MainSymbol;
                ldp.sType = lex.Type.ToString();
                ldp.sStressType = m_dictAccent[lex.AccentType1];
                if (lex.AccentType2 != MainLib.ET_AccentType.AT_UNDEFINED)
                {
                    ldp.sStressType += "/" + m_dictAccent[lex.AccentType2];
                }
                lexPanel.Controls.Add (ldp);
                ++iLexeme;
            }
        }

        private void textBoxSearchString_TextChanged(object sender, EventArgs e)
        {
            if (textBoxSearchString.Text.Length > 0)
            {
                buttonLookup.Enabled = true;
            }
            else
            {
                buttonLookup.Enabled = false;
            }
        }
    }
}
