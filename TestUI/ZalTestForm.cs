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

        private bool bDBOpen;

        public TestApplet()
        {
            InitializeComponent();
            buttonLookup.Enabled = false;
            radioButtonInitForm.Checked = true;
            bDBOpen = false;

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

            m_dictPerson = new Dictionary<MainLib.ET_Person, string>();
            m_dictPerson.Add(MainLib.ET_Person.PERSON_UNDEFINED, "Undefined");
            m_dictPerson.Add(MainLib.ET_Person.PERSON_1, "1");
            m_dictPerson.Add(MainLib.ET_Person.PERSON_2, "2");
            m_dictPerson.Add(MainLib.ET_Person.PERSON_3, "3");

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

        public void SubscribeToLexemeEvents (LexemeDataPanel ldp)
        {
            ldp.ShowDetailsEvent += new LexemeDataPanel.ShowDetails (LexemeDataPanel_ShowLexemeDetails);
        }

        public void SubscribeToVerbEvents (VerbPanel vp)
        {
            vp.ShowParticipialFormsEvent += new VerbPanel.ShowParticipialForms (VerbPanel_ShowParticipialForms);
        }

        public void VerbPanel_ShowParticipialForms (int iLexemeId, 
                                                    MainLib.ET_Subparadigm eoSpLong, 
                                                    MainLib.ET_Subparadigm eoSpShort)
        {
            // Expect word forms to be ready by now
            MainLib.ILexeme lexeme = m_listLexemes[iLexemeId];

            MainLib.IFormDescriptor fd = lexeme.FormDescriptor;
            fd.PartOfSpeech = MainLib.ET_PartOfSpeech.POS_VERB;
            fd.Subparadigm = eoSpLong;
            fd.Number = MainLib.ET_Number.NUM_SG;
            fd.Case = MainLib.ET_Case.CASE_NOM;
            fd.Gender = MainLib.ET_Gender.GENDER_M;
            fd.Animacy = MainLib.ET_Animacy.ANIM_NO;
            fd.Reflexivity = lexeme.IsReflexive;
            fd.FindForms();
            if (fd.Count > 0)
            {
                MainLib.IWordForm wf = (MainLib.IWordForm)fd[1];
                AdjPanel adjPanel = new AdjPanel();
                TabPage tabPageDetails = new TabPage (wf.Wordform);
                tabPageDetails.Controls.Add (adjPanel);
                tabControl.Controls.Add (tabPageDetails);
                ShowLongParticipialForms (adjPanel, iLexemeId, eoSpLong);
                if (MainLib.ET_Subparadigm.SUBPARADIGM_UNDEFINED != eoSpShort)
                {
                    ShowShortParticipialForms (adjPanel, iLexemeId, eoSpShort);
                }
            }
        }

        public void ShowShortParticipialForms (AdjPanel ap, int iLexemeId, MainLib.ET_Subparadigm eoSpShort)
        {
            MainLib.ILexeme lexeme = m_listLexemes[iLexemeId];

            MainLib.IFormDescriptor fd = lexeme.FormDescriptor;
            fd.PartOfSpeech = MainLib.ET_PartOfSpeech.POS_VERB;
            fd.Subparadigm = eoSpShort;
            fd.Reflexivity = lexeme.IsReflexive;
            fd.Number = MainLib.ET_Number.NUM_SG;
            for (MainLib.ET_Gender eo_gender = MainLib.ET_Gender.GENDER_M;
                 eo_gender < MainLib.ET_Gender.GENDER_COUNT;
                 ++eo_gender)
            {
                fd.Gender = eo_gender;
                fd.FindForms();
                if (fd.Count > 0)
                {
                    MainLib.IWordForm wf = (MainLib.IWordForm)fd[1];
                    string sKey = "Short";
                    sKey += m_dictGender[wf.Gender];
                    sKey += m_dictNumber[wf.Number];
                    string strWordForm = wf.Wordform;
                    if (wf.Stress >= 0)
                    {
                        if (strWordForm[wf.Stress] != 'ё')
                        {
                            string strStressMark = new string('\x301', 1);
                            strWordForm = strWordForm.Insert(wf.Stress + 1, strStressMark);
                        }
                    }
                    ap.SetForm(sKey, strWordForm);
                }
            }
        }

        public void ShowLongParticipialForms (AdjPanel ap, int iLexemeId, MainLib.ET_Subparadigm eoSpLong)
        {
            MainLib.ILexeme lexeme = m_listLexemes[iLexemeId];
            foreach (MainLib.IWordForm wf in lexeme)
            {
                string sKey = "";

                if (eoSpLong != wf.Subparadigm)
                {
                    continue;
                }

                if (eoSpLong == wf.Subparadigm && MainLib.ET_Number.NUM_SG == wf.Number)
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
                for (int iStressPos = 0; iStressPos < wf.StressCount; ++iStressPos)
                {
                    if (strWordForm[wf.Stress] != 'ё')
                    {
                        string sStressMark;
                        if (0 != wf.get_IsPrimaryStress (wf.Stress))
                        {
                            sStressMark = new string('\x301', 1);
                        }
                        else
                        {
                            sStressMark = new string('\x300', 1);

                        }
                        strWordForm = strWordForm.Insert(wf.Stress + 1, sStressMark);
                    }
                }

                ap.SetForm(sKey, strWordForm);

            }   //  foreach (...)

        }   //  VerbPanel_ShowParticipialForms (...)

        public void LexemeDataPanel_ShowLexemeDetails (int iLexemeId)
        {
            MainLib.ILexeme lexeme = m_listLexemes[iLexemeId];
            lexeme.GenerateWordForms();

            TabPage tabPageDetails = new TabPage (lexeme.InitialForm);

            string grSt = lexeme.GraphicStem;

            if (MainLib.ET_PartOfSpeech.POS_NOUN == lexeme.PartOfSpeech)
            {
                NounPanel np = new NounPanel();
                tabPageDetails.Controls.Add (np);
                np.sLexName = grSt;

                foreach (MainLib.IWordForm wf in lexeme)
                {
                    string sKey = m_dictCase[wf.Case];
                    sKey += (wf.Number == MainLib.ET_Number.NUM_SG) ? "Sg" : "Pl";

                    string strWordForm = wf.Wordform;
                    if (wf.Stress >= 0)
                    {
                        if (wf.Stress >= wf.Wordform.Length)
                        {
                            MessageBox.Show ("Bad stress position", "Zal Error", MessageBoxButtons.OK);
                            return;
                        }
                        if (strWordForm[wf.Stress] != 'ё')
                        {
                            string strStressMark = new string('\x301', 1);
                            strWordForm = strWordForm.Insert(wf.Stress + 1, strStressMark);
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

                    if (MainLib.ET_Subparadigm.SUBPARADIGM_LONG_ADJ == wf.Subparadigm)
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
                        if (wf.Stress >= 0)
                        {
                            if (strWordForm[wf.Stress] != 'ё')
                            {
                                string strStressMark = new string('\x301', 1);
                                strWordForm = strWordForm.Insert(wf.Stress + 1, strStressMark);
                            }
                        }

                        ap.SetForm(sKey, strWordForm);

                        continue;
                    }

                    if (MainLib.ET_Subparadigm.SUBPARADIGM_SHORT_ADJ == wf.Subparadigm)
                    {
                        sKey = "Short";
                        if (MainLib.ET_Number.NUM_SG == wf.Number)
                        {
                            sKey += m_dictGender[wf.Gender];
                        }
                        sKey += m_dictNumber[wf.Number];

                        string strWordForm = wf.Wordform;
                        if (wf.Stress >= 0)
                        {
                            if (strWordForm[wf.Stress] != 'ё')
                            {
                                string strStressMark = new string('\x301', 1);
                                strWordForm = strWordForm.Insert(wf.Stress + 1, strStressMark);
                            }
                        }

                        ap.SetForm(sKey, strWordForm);

                        continue;
                    }

                    if (MainLib.ET_Subparadigm.SUBPARADIGM_COMPARATIVE == wf.Subparadigm)
                    {
                        sKey = "Comparative";

                        string strWordForm = wf.Wordform;

                        try
                        {
                            if (wf.Stress >= 0)
                            {
                                if (strWordForm[wf.Stress] != 'ё')
                                {
                                    string strStressMark = new string('\x301', 1);
                                    strWordForm = strWordForm.Insert(wf.Stress + 1, strStressMark);
                                }
                            }
                        }
                        catch (Exception ex)
                        {
                            string sMsg = "LexemeDataPanel_ShowLexemeDetails: ";
                            sMsg += ex.Message;
                        }

                        ap.SetForm(sKey, strWordForm);

                        continue;
                    }

                }   // foreach ...

            }   //  if (MainLib.ET_PartOfSpeech.POS_ADJ == lexeme.PartOfSpeech)

            if (MainLib.ET_PartOfSpeech.POS_VERB == lexeme.PartOfSpeech)
            {
                VerbPanel vp = new VerbPanel (iLexemeId);
                SubscribeToVerbEvents(vp);
                tabPageDetails.Controls.Add (vp);
                vp.sLexName = grSt;

                foreach (MainLib.IWordForm wf in lexeme)
                {
                    if (MainLib.ET_Subparadigm.SUBPARADIGM_PRESENT_TENSE == wf.Subparadigm)
                    {
                        string sKey = "Pres" + m_dictPerson[wf.Person];
                        sKey += (wf.Number == MainLib.ET_Number.NUM_SG) ? "Sg" : "Pl";

                        string strWordForm = wf.Wordform;
                        if (wf.Stress >= 0)
                        {
                            if (wf.Stress >= wf.Wordform.Length)
                            {
                                MessageBox.Show("Bad stress position", "Zal Error", MessageBoxButtons.OK);
                                return;
                            }
                            if (strWordForm[wf.Stress] != 'ё')
                            {
                                string strStressMark = new string('\x301', 1);
                                strWordForm = strWordForm.Insert(wf.Stress + 1, strStressMark);
                            }
                        }
                        vp.SetForm(sKey, strWordForm);
                    }

                    if (MainLib.ET_Subparadigm.SUBPARADIGM_PAST_TENSE == wf.Subparadigm)
                    {
                        string sKey = "Past";
                        if (MainLib.ET_Number.NUM_SG == wf.Number)
                        {
                            sKey += m_dictGender[wf.Gender];
                        }
                        sKey += (wf.Number == MainLib.ET_Number.NUM_SG) ? "Sg" : "Pl";

                        string strWordForm = wf.Wordform;
                        try
                        {
                            if (wf.Stress >= 0)
                            {
                                if (wf.Stress >= wf.Wordform.Length)
                                {
                                    MessageBox.Show("Bad stress position", "Zal Error", MessageBoxButtons.OK);
                                    return;
                                }
                                if (strWordForm[wf.Stress] != 'ё')
                                {
                                    string strStressMark = new string('\x301', 1);
                                    strWordForm = strWordForm.Insert(wf.Stress + 1, strStressMark);
                                }
                            }
                        }
                        catch (Exception ex)
                        {
                            string sMsg = "LexemeDataPanel_ShowLexemeDetails: ";
                            sMsg += ex.Message;
                        }

                        vp.SetForm(sKey, strWordForm);
                    }

                    if (MainLib.ET_Subparadigm.SUBPARADIGM_IMPERATIVE == wf.Subparadigm)
                    {
                        string sKey = "Imperative";
                        sKey += (wf.Number == MainLib.ET_Number.NUM_SG) ? "Sg" : "Pl";

                        string strWordForm = wf.Wordform;
                        try
                        {
                            if (wf.Stress >= 0)
                            {
                                if (wf.Stress >= wf.Wordform.Length)
                                {
                                    MessageBox.Show("Bad stress position", "Zal Error", MessageBoxButtons.OK);
                                    return;
                                }
                                if (strWordForm[wf.Stress] != 'ё')
                                {
                                    string strStressMark = new string('\x301', 1);
                                    strWordForm = strWordForm.Insert(wf.Stress + 1, strStressMark);
                                }
                            }
                        }
                        catch (Exception ex)
                        {
                            string sMsg = "LexemeDataPanel_ShowLexemeDetails: ";
                            sMsg += ex.Message;
                        }

                        vp.SetForm(sKey, strWordForm);
                    }

                    if (MainLib.ET_Subparadigm.SUBPARADIGM_PART_PRES_ACT == wf.Subparadigm)
                    {
                        string sKey = "PartPresActive";
                        if (wf.Number == MainLib.ET_Number.NUM_SG && 
                            wf.Gender == MainLib.ET_Gender.GENDER_M &&
                            wf.Case == MainLib.ET_Case.CASE_NOM)
                        {
                            string strWordForm = wf.Wordform;
                            try
                            {
                                if (wf.Stress >= 0)
                                {
                                    if (wf.Stress >= wf.Wordform.Length)
                                    {
                                        MessageBox.Show("Bad stress position", "Zal Error", MessageBoxButtons.OK);
                                        return;
                                    }
                                    if (strWordForm[wf.Stress] != 'ё')
                                    {
                                        string strStressMark = new string('\x301', 1);
                                        strWordForm = strWordForm.Insert(wf.Stress + 1, strStressMark);
                                    }
                                }
                            }
                            catch (Exception ex)
                            {
                                string sMsg = "LexemeDataPanel_ShowLexemeDetails: ";
                                sMsg += ex.Message;
                            }

                            vp.SetForm(sKey, strWordForm);
                        }
                    }

                    if (MainLib.ET_Subparadigm.SUBPARADIGM_PART_PAST_ACT == wf.Subparadigm)
                    {
                        string sKey = "PartPastActive";
                        if (wf.Number == MainLib.ET_Number.NUM_SG &&
                            wf.Gender == MainLib.ET_Gender.GENDER_M &&
                            wf.Case == MainLib.ET_Case.CASE_NOM)
                        {
                            string strWordForm = wf.Wordform;
                            try
                            {
                                if (wf.Stress >= 0)
                                {
                                    if (wf.Stress >= wf.Wordform.Length)
                                    {
                                        MessageBox.Show("Bad stress position", "Zal Error", MessageBoxButtons.OK);
                                        return;
                                    }
                                    if (strWordForm[wf.Stress] != 'ё')
                                    {
                                        string strStressMark = new string('\x301', 1);
                                        strWordForm = strWordForm.Insert(wf.Stress + 1, strStressMark);
                                    }
                                }
                            }
                            catch (Exception ex)
                            {
                                string sMsg = "LexemeDataPanel_ShowLexemeDetails: ";
                                sMsg += ex.Message;
                            }

                            vp.SetForm(sKey, strWordForm);
                        }
                    }

                    if (MainLib.ET_Subparadigm.SUBPARADIGM_ADVERBIAL_PRESENT == wf.Subparadigm)
                    {
                        string strWordForm = wf.Wordform;
                        try
                        {
                            if (wf.Stress >= 0)
                            {
                                if (wf.Stress >= wf.Wordform.Length)
                                {
                                    MessageBox.Show("Bad stress position", "Zal Error", MessageBoxButtons.OK);
                                    return;
                                }
                                if (strWordForm[wf.Stress] != 'ё')
                                {
                                    string strStressMark = new string('\x301', 1);
                                    strWordForm = strWordForm.Insert(wf.Stress + 1, strStressMark);
                                }
                            }
                        }
                        catch (Exception ex)
                        {
                            string sMsg = "LexemeDataPanel_ShowLexemeDetails: ";
                            sMsg += ex.Message;
                        }

                        vp.SetForm("PresAdverbial", strWordForm);
                    }

                    if (MainLib.ET_Subparadigm.SUBPARADIGM_ADVERBIAL_PAST == wf.Subparadigm)
                    {
                        string strWordForm = wf.Wordform;
                        try
                        {
                            if (wf.Stress >= 0)
                            {
                                if (wf.Stress >= wf.Wordform.Length)
                                {
                                    MessageBox.Show("Bad stress position", "Zal Error", MessageBoxButtons.OK);
                                    return;
                                }
                                if (strWordForm[wf.Stress] != 'ё')
                                {
                                    string strStressMark = new string('\x301', 1);
                                    strWordForm = strWordForm.Insert(wf.Stress + 1, strStressMark);
                                }
                            }
                        }
                        catch (Exception ex)
                        {
                            string sMsg = "LexemeDataPanel_ShowLexemeDetails: ";
                            sMsg += ex.Message;
                        }

                        vp.SetForm("PastAdverbial", strWordForm);
                    }

                    if (MainLib.ET_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG == wf.Subparadigm)
                    {
                        string sKey = "PartPresPassive";
                        if (wf.Number == MainLib.ET_Number.NUM_SG &&
                            wf.Gender == MainLib.ET_Gender.GENDER_M &&
                            wf.Case == MainLib.ET_Case.CASE_NOM)
                        {
                            string strWordForm = wf.Wordform;
                            try
                            {
                                if (wf.Stress >= 0)
                                {
                                    if (wf.Stress >= wf.Wordform.Length)
                                    {
                                        MessageBox.Show("Bad stress position", "Zal Error", MessageBoxButtons.OK);
                                        return;
                                    }
                                    if (strWordForm[wf.Stress] != 'ё')
                                    {
                                        string strStressMark = new string('\x301', 1);
                                        strWordForm = strWordForm.Insert(wf.Stress + 1, strStressMark);
                                    }
                                }
                            }
                            catch (Exception ex)
                            {
                                string sMsg = "LexemeDataPanel_ShowLexemeDetails: ";
                                sMsg += ex.Message;
                            }

                            vp.SetForm(sKey, strWordForm);
                        }
                    }

                    if (MainLib.ET_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG == wf.Subparadigm)
                    {
                        string sKey = "PartPastPassive";
                        if (wf.Number == MainLib.ET_Number.NUM_SG &&
                            wf.Gender == MainLib.ET_Gender.GENDER_M &&
                            wf.Case == MainLib.ET_Case.CASE_NOM)
                        {
                            string strWordForm = wf.Wordform;
                            try
                            {
                                if (wf.Stress >= 0)
                                {
                                    if (wf.Stress >= wf.Wordform.Length)
                                    {
                                        MessageBox.Show("Bad stress position", "Zal Error", MessageBoxButtons.OK);
                                        return;
                                    }
                                    if (strWordForm[wf.Stress] != 'ё')
                                    {
                                        string strStressMark = new string('\x301', 1);
                                        strWordForm = strWordForm.Insert(wf.Stress + 1, strStressMark);
                                    }
                                }
                            }
                            catch (Exception ex)
                            {
                                string sMsg = "LexemeDataPanel_ShowLexemeDetails: ";
                                sMsg += ex.Message;
                            }

                            vp.SetForm(sKey, strWordForm);
                        }
                    }

                }   // foreach
            }

            tabControl.Controls.Add(tabPageDetails);

        }   //  LexemeDataPanel_ShowLexemeDetails (int iLexemeId)

        private void buttonLookup_Click(object sender, EventArgs e)
        {
            if (bDBOpen == false)
            {
                m_Dictionary.DbPath = textBoxDBPath.Text;
                m_Analyzer.DbPath = textBoxDBPath.Text;
                bDBOpen = true;
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
                // Call Tim's analysis code here
                int iWordform = 0;
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
                foreach (MainLib.IWordForm wf in m_Analyzer)
                {
                    m_listWordForms.Add(wf);
                    AnalysisPanel ap = new AnalysisPanel(iWordform);
                    ap.Location = new System.Drawing.Point(0, iWordform * ap.Size.Height + 4);
                    ap.sLemma = wf.Lemma;
                    ap.sWordform = wf.Wordform;
                    ap.eoPOS = wf.PartOfSpeech;
                    ap.eoGender = wf.Gender;
                    ap.eoCase = wf.Case;
                    ap.eoNumber = wf.Number;
                    ap.eoSubparadigm = wf.Subparadigm;
                    ap.eoAnimacy = wf.Animacy;
                    if (wf.Subparadigm == MainLib.ET_Subparadigm.SUBPARADIGM_PAST_TENSE
                        || wf.Subparadigm == MainLib.ET_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG
                        || wf.Subparadigm == MainLib.ET_Subparadigm.SUBPARADIGM_PART_PAST_ACT)
                    {
                        ap.eoTense = MainLib.ET_Tense.TENSE_PAST;
                    }
                    if ((wf.Subparadigm == MainLib.ET_Subparadigm.SUBPARADIGM_PRESENT_TENSE
                        || wf.Subparadigm == MainLib.ET_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG
                        || wf.Subparadigm == MainLib.ET_Subparadigm.SUBPARADIGM_PART_PRES_ACT)
                        && wf.Aspect == MainLib.ET_Aspect.ASPECT_PERFECTIVE)
                    {
                        ap.eoTense = MainLib.ET_Tense.TENSE_FUTURE;
                    }
                    if ((wf.Subparadigm == MainLib.ET_Subparadigm.SUBPARADIGM_PRESENT_TENSE
                        || wf.Subparadigm == MainLib.ET_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG
                        || wf.Subparadigm == MainLib.ET_Subparadigm.SUBPARADIGM_PART_PRES_ACT)
                        && wf.Aspect == MainLib.ET_Aspect.ASPECT_IMPERFECTIVE)
                    {
                        ap.eoTense = MainLib.ET_Tense.TENSE_PRESENT;
                    }
                    if (wf.Subparadigm != MainLib.ET_Subparadigm.SUBPARADIGM_PRESENT_TENSE
                        && wf.Subparadigm != MainLib.ET_Subparadigm.SUBPARADIGM_PAST_TENSE
                        && wf.Subparadigm != MainLib.ET_Subparadigm.SUBPARADIGM_PART_PAST_ACT
                        && wf.Subparadigm != MainLib.ET_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG
                        && wf.Subparadigm != MainLib.ET_Subparadigm.SUBPARADIGM_PART_PRES_ACT
                        && wf.Subparadigm != MainLib.ET_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG)
                    {
                        ap.eoTense = MainLib.ET_Tense.TENSE_UNDEFINED;
                    }
                    //ap.i_lexeme_id = wf.LexemeId;
                    lexPanel.Controls.Add(ap);
                    iWordform++;
                }
            }
            else if (radioButtonPreprocess.Checked)
            {
                bSynthesis = false;
                try
                {
                    int i_Stress = Convert.ToInt32(true); // It should've been bool
                    if (Regex.IsMatch(textBoxSearchString.Text, (string)">"))
                    {
                        i_Stress = Convert.ToInt32(true);
                    }
                    else
                    {
                        i_Stress = Convert.ToInt32(true); // Test
                    }
                    textBoxSearchString.Text = textBoxSearchString.Text.Replace(">", "");
                    if (Regex.IsMatch(textBoxSearchString.Text, (string)"\\-"))
                    {
                        string[] arr_Range = Regex.Split(textBoxSearchString.Text, (string)"([0-9]*)\\-([0-9]*)");
                        long l_start_id = long.Parse(arr_Range[1]);
                        long l_end_id = long.Parse(arr_Range[2]);
                        m_Analyzer.PrepareLexemes(l_start_id, l_end_id, i_Stress);
                    }
                    else
                    {
                        long l_lexeme_id = long.Parse(textBoxSearchString.Text);
                        m_Analyzer.PrepareLexeme(l_lexeme_id, i_Stress);
                    }
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
            bDBOpen = false;
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
            bDBOpen = false;
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
