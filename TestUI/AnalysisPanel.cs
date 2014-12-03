using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MainLibManaged;

namespace TestUI
{
    public partial class AnalysisPanel : UserControl
    {

        int iWordformId;

        public string sSource
        {
            get
            {
                return TextBox_Source.Text;
            }

            set
            {
                TextBox_Source.Text = value;
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

        public EM_PartOfSpeech eoPOS
        {
            set
            {
                switch (value)
                {
                    case EM_PartOfSpeech.POS_NOUN:
                        TextBox_POS.Text = "существительное";
                        break;
                    case EM_PartOfSpeech.POS_ADJ:
                        TextBox_POS.Text = "прилагательное";
                        break;
                    case EM_PartOfSpeech.POS_VERB:
                        TextBox_POS.Text = "глагол";
                        break;
                    case EM_PartOfSpeech.POS_PRONOUN:
                        TextBox_POS.Text = "местоимение";
                        break;
                    case EM_PartOfSpeech.POS_PRONOUN_ADJ:
                        TextBox_POS.Text = "местоимение-прил.";
                        break;
                    case EM_PartOfSpeech.POS_PRONOUN_PREDIC:
                        TextBox_POS.Text = "POS_PRONOUN_PREDIC";
                        break;
                    case EM_PartOfSpeech.POS_NUM:
                        TextBox_POS.Text = "числительное";
                        break;
                    case EM_PartOfSpeech.POS_NUM_ADJ:
                        TextBox_POS.Text = "порядковое числительное";
                        break;
                    case EM_PartOfSpeech.POS_ADV:
                        TextBox_POS.Text = "наречие";
                        break;
                    case EM_PartOfSpeech.POS_COMPAR:
                        TextBox_POS.Text = "сравнительная степень";
                        break;
                    case EM_PartOfSpeech.POS_PREDIC:
                        TextBox_POS.Text = "POS_PREDIC";
                        break;
                    case EM_PartOfSpeech.POS_PREP:
                        TextBox_POS.Text = "предлог";
                        break;
                    case EM_PartOfSpeech.POS_CONJ:
                        TextBox_POS.Text = "союз";
                        break;
                    case EM_PartOfSpeech.POS_PARTICLE:
                        TextBox_POS.Text = "частица";
                        break;
                    case EM_PartOfSpeech.POS_INTERJ:
                        TextBox_POS.Text = "междометие";
                        break;
                    case EM_PartOfSpeech.POS_PARENTH:
                        TextBox_POS.Text = "POS_PARENTH";
                        break;
                    case EM_PartOfSpeech.POS_NULL:
                        TextBox_POS.Text = "POS_NULL";
                        break;
                    case EM_PartOfSpeech.POS_COUNT:
                        TextBox_POS.Text = "POS_NULL";
                        break;
                    default:
                        TextBox_POS.Text = "POS not recognized";
                        break;
                }
            }
        }

        public EM_Case eoCase
        {
            set
            {
                switch (value)
                {
                    case EM_Case.CASE_NOM:
                            TextBox_Case.Text = "И";
                            break;
                    case EM_Case.CASE_ACC:
                            TextBox_Case.Text = "В";
                            break;
                    case EM_Case.CASE_GEN:
                            TextBox_Case.Text = "Р";
                            break;
                    case EM_Case.CASE_DAT:
                            TextBox_Case.Text = "Д";
                            break;
                    case EM_Case.CASE_PART:
                            TextBox_Case.Text = "Р2";
                            break;
                    case EM_Case.CASE_INST:
                            TextBox_Case.Text = "Т";
                            break;
                    case EM_Case.CASE_PREP:
                            TextBox_Case.Text = "П";
                            break;
                    case EM_Case.CASE_LOC:
                            TextBox_Case.Text = "П2";
                            break;
                    case EM_Case.CASE_NUM:
                            TextBox_Case.Text = "adnum";
                            break;
                    default:
                        TextBox_Case.Text = "";
                        break;
                }
            }
        }

        public EM_Number eoNumber
        {
            set
            {
                switch (value)
                {
                    case EM_Number.NUM_SG:
                        TextBox_Number.Text = "ед";
                        break;
                    case EM_Number.NUM_PL:
                        TextBox_Number.Text = "мн";
                        break;
                    default:
                        TextBox_Number.Text = "";
                        break;
                }
            }
        }

        public EM_Gender eoGender
        {
            set
            {
                switch (value)
                {
                    case EM_Gender.GENDER_F:
                        TextBox_Gender.Text = "ж";
                        break;
                    case EM_Gender.GENDER_M:
                        TextBox_Gender.Text = "м";
                        break;
                    case EM_Gender.GENDER_N:
                        TextBox_Gender.Text = "с";
                        break;
                    default:
                        TextBox_Gender.Text = "";
                        break;
                }
            }
        }

        public EM_Animacy eoAnimacy
        {
            set
            {
                switch (value)
                {
                    case EM_Animacy.ANIM_NO:
                        TextBox_Animacy.Text = "неод";
                        break;
                    case EM_Animacy.ANIM_YES:
                        TextBox_Animacy.Text = "одуш";
                        break;
                    default:
                        TextBox_Animacy.Text = "";
                        break;
                }
            }
        }

        public EM_Aspect eoAspect
        {
            get
            {
                switch (TextBox_Aspect.Text)
                {
                    case "нсв":
                        return EM_Aspect.ASPECT_IMPERFECTIVE;
                    case "св":
                        return EM_Aspect.ASPECT_PERFECTIVE;
                    default:
                        return EM_Aspect.ASPECT_UNDEFINED;
                }
            }
            set
            {
                switch (value)
                {
                    case EM_Aspect.ASPECT_IMPERFECTIVE:
                        TextBox_Aspect.Text = "нсв";
                        break;
                    case EM_Aspect.ASPECT_PERFECTIVE:
                        TextBox_Aspect.Text = "св";
                        break;
                    case EM_Aspect.ASPECT_UNDEFINED:
                        TextBox_Aspect.Text = "?";
                        break;
                    default:
                        TextBox_Aspect.Text = "";
                        break;
                }
            }
        }

        public EM_Reflexive eoReflexiveness
        {
            set
            {
                switch (value)
                {
                    case EM_Reflexive.REFL_YES:
                        TextBox_Reflective.Text = "возвр";
                        break;
                    default:
                        TextBox_Reflective.Text = "";
                        break;
                }
            }
        }

        public EM_Subparadigm eoSubparadigm
        {
            set
            {
                switch (value)
                {
                    case EM_Subparadigm.SUBPARADIGM_NOUN:
                        TextBox_Subparadigm.Text = "NOUN";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_PAST_TENSE:
                        TextBox_Tense.Text = "прош";
                        TextBox_Mood.Text = "изъяв";
                        TextBox_Subparadigm.Text = "PAST_TENSE";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG:
                        TextBox_Tense.Text = "прош";
                        TextBox_Special.Text = "прич";
                        TextBox_Voice.Text = "страд";
                        TextBox_Subparadigm.Text = "PART_PAST_PASS_LONG";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_PART_PAST_ACT:
                        TextBox_Tense.Text = "прош";
                        TextBox_Special.Text = "прич";
                        TextBox_Voice.Text = "действ";
                        TextBox_Subparadigm.Text = "PART_PAST_ACT";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_PRESENT_TENSE:
                        if (eoAspect == EM_Aspect.ASPECT_PERFECTIVE)
                        {
                            TextBox_Tense.Text = "буд";
                        }
                        else if (eoAspect == EM_Aspect.ASPECT_IMPERFECTIVE)
                        {
                            TextBox_Tense.Text = "наст";
                        }
                        else
                        {
                            TextBox_Tense.Text = "¬наст";
                        }
                        TextBox_Mood.Text = "изъяв";
                        TextBox_Subparadigm.Text = "PRESENT_TENSE";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG:
                        if (eoAspect == EM_Aspect.ASPECT_PERFECTIVE)
                        {
                            TextBox_Tense.Text = "буд";
                        }
                        else if (eoAspect == EM_Aspect.ASPECT_IMPERFECTIVE)
                        {
                            TextBox_Tense.Text = "наст";
                        }
                        else
                        {
                            TextBox_Tense.Text = "¬наст";
                        }
                        TextBox_Special.Text = "прич";
                        TextBox_Voice.Text = "страд";
                        TextBox_Subparadigm.Text = "PART_PRES_PASS_LONG";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_PART_PRES_ACT:
                        if (eoAspect == EM_Aspect.ASPECT_PERFECTIVE)
                        {
                            TextBox_Tense.Text = "буд";
                        }
                        else if (eoAspect == EM_Aspect.ASPECT_IMPERFECTIVE)
                        {
                            TextBox_Tense.Text = "наст";
                        }
                        else
                        {
                            TextBox_Tense.Text = "¬наст";
                        }
                        TextBox_Special.Text = "прич";
                        TextBox_Voice.Text = "действ";
                        TextBox_Subparadigm.Text = "PART_PRES_ACT";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_ADVERBIAL_PAST:
                        TextBox_Tense.Text = "прош";
                        TextBox_Special.Text = "деепр";
                        TextBox_Subparadigm.Text = "ADVERBIAL_PAST";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_ADVERBIAL_PRESENT:
                        if (eoAspect == EM_Aspect.ASPECT_PERFECTIVE)
                        {
                            TextBox_Tense.Text = "буд";
                        }
                        else if (eoAspect == EM_Aspect.ASPECT_IMPERFECTIVE)
                        {
                            TextBox_Tense.Text = "наст";
                        }
                        else
                        {
                            TextBox_Tense.Text = "¬наст";
                        }
                        TextBox_Special.Text = "деепр";
                        TextBox_Subparadigm.Text = "ADVERBIAL_PRESENT";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_INFINITIVE:
                        TextBox_Mood.Text = "инфинитив";
                        TextBox_Subparadigm.Text = "INFINITIVE";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_IMPERATIVE:
                        TextBox_Mood.Text = "повел";
                        TextBox_Subparadigm.Text = "IMPERATIVE";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_SHORT_ADJ:
                        TextBox_Subparadigm.Text = "SHORT_ADJ";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_SHORT:
                        TextBox_Subparadigm.Text = "PART_PAST_PASS_SHORT";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_SHORT:
                        TextBox_Subparadigm.Text = "PART_PRES_PASS_SHORT";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_LONG_ADJ:
                        TextBox_Subparadigm.Text = "LONG_ADJ";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_COMPARATIVE:
                        TextBox_Subparadigm.Text = "COMPARATIVE";
                        break;
                    default:
                        TextBox_Subparadigm.Text = value.ToString();
                        break;

                }
            }
        
        }       //   public EM_Subparadigm eoSubparadigm

        public EM_Person eoPerson
        {
            set
            {
                switch (value)
                {
                    case EM_Person.PERSON_1:
                        TextBox_Person.Text = "1";
                        break;
                    case EM_Person.PERSON_2:
                        TextBox_Person.Text = "2";
                        break;
                    case EM_Person.PERSON_3:
                        TextBox_Person.Text = "3";
                        break;
                    default:
                        TextBox_Person.Text = "";
                        break;
                }
            }
        }
/*
        public MainLib.ET_Tense eoTense
        {
            set
            {
                switch (value)
                {
                    case MainLib.EM_Tense.TENSE_PAST:
                        TextBox_Tense.Text = "прош.";
                        break;
                    case MainLib.EM_Tense.TENSE_PRESENT:
                        TextBox_Tense.Text = "наст.";
                        break;
                    case MainLib.EM_Tense.TENSE_FUTURE:
                        TextBox_Tense.Text = "буд.";
                        break;
                    default:
                        TextBox_Animacy.Text = "";
                        break;
                }
            }
        }
*/
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
