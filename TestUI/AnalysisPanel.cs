using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TestUI
{
    public partial class AnalysisPanel : UserControl
    {

        int iWordformId;

        public string sLemma
        {
            get
            {
                return TextBox_Lemma.Text;
            }

            set
            {
                TextBox_Lemma.Text = value;
            }
        }

        public string sWordform
        {
            get
            {
                return TextBox_Wordform.Text;
            }

            set
            {
                TextBox_Wordform.Text = value;
            }
        }

        public string sID
        {
            get
            {
                return TextBox_ID.Text;
            }

            set
            {
                TextBox_ID.Text = value;
            }
        }

        public string sPanelNum
        {
            get
            {
                return TextBox_PanelNum.Text;
            }

            set
            {
                TextBox_PanelNum.Text = value;
            }
        }

        public MainLib.ET_PartOfSpeech eoPOS
        {
            set
            {
                switch (value)
                {
                    case MainLib.ET_PartOfSpeech.POS_NOUN:
                        TextBox_POS.Text = "N";
                        break;
                    case MainLib.ET_PartOfSpeech.POS_ADJ:
                        TextBox_POS.Text = "Adj";
                        break;
                    case MainLib.ET_PartOfSpeech.POS_VERB:
                        TextBox_POS.Text = "V";
                        break;
                    default:
                        TextBox_POS.Text = "";
                        break;
                }
            }
        }

        public MainLib.ET_Case eoCase
        {
            set
            {
                switch (value)
                {
                    case MainLib.ET_Case.CASE_NOM:
                            TextBox_Case.Text = "Nom";
                            break;
                    case MainLib.ET_Case.CASE_ACC:
                            TextBox_Case.Text = "Acc";
                            break;
                    case MainLib.ET_Case.CASE_GEN:
                            TextBox_Case.Text = "Gen";
                            break;
                    case MainLib.ET_Case.CASE_DAT:
                            TextBox_Case.Text = "Dat";
                            break;
                    case MainLib.ET_Case.CASE_PART:
                            TextBox_Case.Text = "Part";
                            break;
                    case MainLib.ET_Case.CASE_INST:
                            TextBox_Case.Text = "Inst";
                            break;
                    case MainLib.ET_Case.CASE_PREP:
                            TextBox_Case.Text = "Prep";
                            break;
                    case MainLib.ET_Case.CASE_LOC:
                            TextBox_Case.Text = "Loc";
                            break;
                    case MainLib.ET_Case.CASE_NUM:
                            TextBox_Case.Text = "Сч. форма";
                            break;
                    default:
                        TextBox_Case.Text = "";
                        break;
                }
            }
        }

        public MainLib.ET_Number eoNumber
        {
            set
            {
                switch (value)
                {
                    case MainLib.ET_Number.NUM_SG:
                        TextBox_Number.Text = "Sg";
                        break;
                    case MainLib.ET_Number.NUM_PL:
                        TextBox_Number.Text = "Pl";
                        break;
                    default:
                        TextBox_Number.Text = "";
                        break;
                }
            }
        }

        public MainLib.ET_Gender eoGender
        {
            set
            {
                switch (value)
                {
                    case MainLib.ET_Gender.GENDER_F:
                        TextBox_Gender.Text = "F";
                        break;
                    case MainLib.ET_Gender.GENDER_M:
                        TextBox_Gender.Text = "M";
                        break;
                    case MainLib.ET_Gender.GENDER_N:
                        TextBox_Gender.Text = "N";
                        break;
                    default:
                        TextBox_Gender.Text = "";
                        break;
                }
            }
        }

        public MainLib.ET_Animacy eoAnimacy
        {
            set
            {
                switch (value)
                {
                    case MainLib.ET_Animacy.ANIM_NO:
                        TextBox_Animacy.Text = "неодуш.";
                        break;
                    case MainLib.ET_Animacy.ANIM_YES:
                        TextBox_Animacy.Text = "одуш.";
                        break;
                    default:
                        TextBox_Animacy.Text = "";
                        break;
                }
            }
        }

        public MainLib.ET_Subparadigm eoSubparadigm
        {
            set
            {
                switch (value)
                {
                    case MainLib.ET_Subparadigm.SUBPARADIGM_SHORT_ADJ:
                        TextBox_Subparadigm.Text = "краткая";
                        break;
                    case MainLib.ET_Subparadigm.SUBPARADIGM_LONG_ADJ:
                        TextBox_Subparadigm.Text = "полная";
                        break;
                    case MainLib.ET_Subparadigm.SUBPARADIGM_COMPARATIVE:
                        TextBox_Subparadigm.Text = "сравн.";
                        break;
                    default:
                        TextBox_Animacy.Text = "";
                        break;
                }
            }
        }

        public MainLib.ET_Tense eoTense
        {
            set
            {
                switch (value)
                {
                    case MainLib.ET_Tense.TENSE_PAST:
                        TextBox_Tense.Text = "прош.";
                        break;
                    case MainLib.ET_Tense.TENSE_PRESENT:
                        TextBox_Tense.Text = "наст.";
                        break;
                    case MainLib.ET_Tense.TENSE_FUTURE:
                        TextBox_Tense.Text = "буд.";
                        break;
                    default:
                        TextBox_Animacy.Text = "";
                        break;
                }
            }
        }

        public int i_lexeme_id
        {
            set
            {
                i_lexeme_id = value;
                TextBox_ID.Text = i_lexeme_id.ToString();
            }
            get
            {
                return i_lexeme_id;
            }
        }

        public AnalysisPanel(int iId)
        {
            InitializeComponent();
            iWordformId = iId;
            sPanelNum = iId.ToString();
        }

        private void AnalysisPanel_Load(object sender, EventArgs e)
        {

        }
    }
}
