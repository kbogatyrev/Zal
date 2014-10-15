﻿using System;
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

        public EM_PartOfSpeech eoPOS
        {
            set
            {
                switch (value)
                {
                    case EM_PartOfSpeech.POS_NOUN:
                        TextBox_POS.Text = "N";
                        break;
                    case EM_PartOfSpeech.POS_ADJ:
                        TextBox_POS.Text = "Adj";
                        break;
                    case EM_PartOfSpeech.POS_VERB:
                        TextBox_POS.Text = "V";
                        break;
                    default:
                        TextBox_POS.Text = "";
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
                            TextBox_Case.Text = "nom";
                            break;
                    case EM_Case.CASE_ACC:
                            TextBox_Case.Text = "acc";
                            break;
                    case EM_Case.CASE_GEN:
                            TextBox_Case.Text = "gen";
                            break;
                    case EM_Case.CASE_DAT:
                            TextBox_Case.Text = "dat";
                            break;
                    case EM_Case.CASE_PART:
                            TextBox_Case.Text = "part";
                            break;
                    case EM_Case.CASE_INST:
                            TextBox_Case.Text = "inst";
                            break;
                    case EM_Case.CASE_PREP:
                            TextBox_Case.Text = "prep";
                            break;
                    case EM_Case.CASE_LOC:
                            TextBox_Case.Text = "loc";
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
                        TextBox_Number.Text = "sg";
                        break;
                    case EM_Number.NUM_PL:
                        TextBox_Number.Text = "pl";
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
                        TextBox_Gender.Text = "f";
                        break;
                    case EM_Gender.GENDER_M:
                        TextBox_Gender.Text = "m";
                        break;
                    case EM_Gender.GENDER_N:
                        TextBox_Gender.Text = "n";
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
                        TextBox_Animacy.Text = "inan";
                        break;
                    case EM_Animacy.ANIM_YES:
                        TextBox_Animacy.Text = "anim";
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
                    case "ipfv":
                        return EM_Aspect.ASPECT_IMPERFECTIVE;
                    case "pfv":
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
                        TextBox_Aspect.Text = "ipfv";
                        break;
                    case EM_Aspect.ASPECT_PERFECTIVE:
                        TextBox_Aspect.Text = "pfv";
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
                        TextBox_Reflective.Text = "med";
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
                switch (value) // Verbs
                {
                    case EM_Subparadigm.SUBPARADIGM_PAST_TENSE:
                        TextBox_Tense.Text = "past";
                        TextBox_Mood.Text = "indic";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG:
                        TextBox_Tense.Text = "past";
                        TextBox_Special.Text = "partcp";
                        TextBox_Voice.Text = "pass";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_PART_PAST_ACT:
                        TextBox_Tense.Text = "past";
                        TextBox_Special.Text = "partcp";
                        TextBox_Voice.Text = "act";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_PRESENT_TENSE:
                        if (eoAspect == EM_Aspect.ASPECT_PERFECTIVE)
                        {
                            TextBox_Tense.Text = "fut";
                        }
                        else if (eoAspect == EM_Aspect.ASPECT_IMPERFECTIVE)
                        {
                            TextBox_Tense.Text = "praes";
                        }
                        else
                        {
                            TextBox_Tense.Text = "nonpast";
                        }
                        TextBox_Mood.Text = "indic";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG:
                        if (eoAspect == EM_Aspect.ASPECT_PERFECTIVE)
                        {
                            TextBox_Tense.Text = "fut";
                        }
                        else if (eoAspect == EM_Aspect.ASPECT_IMPERFECTIVE)
                        {
                            TextBox_Tense.Text = "praes";
                        }
                        else
                        {
                            TextBox_Tense.Text = "nonpast";
                        }
                        TextBox_Special.Text = "partcp";
                        TextBox_Voice.Text = "pass";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_PART_PRES_ACT:
                        if (eoAspect == EM_Aspect.ASPECT_PERFECTIVE)
                        {
                            TextBox_Tense.Text = "fut";
                        }
                        else if (eoAspect == EM_Aspect.ASPECT_IMPERFECTIVE)
                        {
                            TextBox_Tense.Text = "praes";
                        }
                        else
                        {
                            TextBox_Tense.Text = "nonpast";
                        }
                        TextBox_Special.Text = "partcp";
                        TextBox_Voice.Text = "act";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_ADVERBIAL_PAST:
                        TextBox_Tense.Text = "past";
                        TextBox_Special.Text = "ger";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_ADVERBIAL_PRESENT:
                        if (eoAspect == EM_Aspect.ASPECT_PERFECTIVE)
                        {
                            TextBox_Tense.Text = "fut";
                        }
                        else if (eoAspect == EM_Aspect.ASPECT_IMPERFECTIVE)
                        {
                            TextBox_Tense.Text = "praes";
                        }
                        else
                        {
                            TextBox_Tense.Text = "nonpast";
                        }
                        TextBox_Special.Text = "ger";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_INFINITIVE:
                        TextBox_Mood.Text = "inf";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_IMPERATIVE:
                        TextBox_Mood.Text = "imper";
                        break;
                }
                switch (value) // Adjectives and participles (short / long)
                {
                    case EM_Subparadigm.SUBPARADIGM_SHORT_ADJ:
                    case EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_SHORT:
                    case EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_SHORT:
                        TextBox_Subparadigm.Text = "brev";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_LONG_ADJ:
                    case EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG:
                    case EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG:
                        TextBox_Subparadigm.Text = "plen";
                        break;
                    case EM_Subparadigm.SUBPARADIGM_COMPARATIVE:
                        TextBox_Subparadigm.Text = "compar";
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
