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

        public Synthesizer()
        {
            InitializeComponent();
            buttonLookup.Enabled = false;
            radioButtonInitForm.Checked = true;
            m_Dictionary = new MainLib.ZalDictionary();
            m_listLexemes = new List<MainLib.ILexeme>();
        }

        public void Subscribe (LexemeDataPanel ldp)
        {
            ldp.ShowDetailsEvent += new LexemeDataPanel.ShowDetails (LexemeDataPanel_ShowLexemeDetails);
        }

        public void LexemeDataPanel_ShowLexemeDetails (int iLexemeId)
        {
//            EnterDataDlg edd = new EnterDataDlg();
//            DialogResult dr = edd.ShowDialog();

//            if (DialogResult.OK != dr)
//            {
//                return;
//            }

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
                    string sKey = "";
                    switch (wf.Case)
                    {
                        case MainLib.ET_Case.CASE_NOM:
                        {
                            sKey = "N";
                            break;
                        }
                        case MainLib.ET_Case.CASE_ACC:
                        {
                            sKey = "A";
                            break;
                        }
                        case MainLib.ET_Case.CASE_GEN:
                        {
                            sKey = "G";
                            break;
                        }
                        case MainLib.ET_Case.CASE_LOC:
                        {
                            sKey = "L";
                            break;
                        }
                        case MainLib.ET_Case.CASE_DAT:
                        {
                            sKey = "D";
                            break;
                        }
                        case MainLib.ET_Case.CASE_INST:
                        {
                            sKey = "I";
                            break;
                        }
                    }

                    sKey += (wf.Number == MainLib.ET_Number.NUM_SG) ? "Sg" : "Pl";
                    np.setForm(sKey, wf.Wordform);

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
                    switch (wf.Gender)
                    {
                        case MainLib.ET_Gender.GENDER_M:
                        {
                            sKey = "M";
                            break;
                        }
                        case MainLib.ET_Gender.GENDER_F:
                        {
                            sKey = "F";
                            break;
                        }
                        case MainLib.ET_Gender.GENDER_N:
                        {
                            sKey = "N";
                            break;
                        }

                    }
                    switch (wf.Case)
                    {
                        case MainLib.ET_Case.CASE_NOM:
                        {
                            sKey += "N";
                            break;
                        }
                        case MainLib.ET_Case.CASE_ACC:
                        {
                            sKey += "A";
                            break;
                        }
                        case MainLib.ET_Case.CASE_GEN:
                        {
                            sKey += "G";
                            break;
                        }
                        case MainLib.ET_Case.CASE_LOC:
                        {
                            sKey += "L";
                            break;
                        }
                        case MainLib.ET_Case.CASE_DAT:
                        {
                            sKey += "D";
                            break;
                        }
                        case MainLib.ET_Case.CASE_INST:
                        {
                            sKey += "I";
                            break;
                        }
                    }

                    sKey += (wf.Number == MainLib.ET_Number.NUM_SG) ? "Sg" : "Pl";
                    ap.setForm(sKey, wf.Wordform);

                }   // foreach ...

            }   //  if (MainLib.ET_PartOfSpeech.POS_ADJ == lexeme.PartOfSpeech)

            tabControl.Controls.Add(tabPageDetails);

        }   //  LexemeDataPanel_ShowLexemeDetails (int iLexemeId)

        private void buttonLookup_Click(object sender, EventArgs e)
        {
            lexPanel.Controls.Clear();

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
