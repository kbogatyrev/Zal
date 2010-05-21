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
    public partial class TestApplet
    {
        protected void InitializeData()
        {
            m_bDBOpen = false;

            m_sDbPath = Properties.Settings.Default.DbPath;

            m_Dictionary = new MainLib.ZalDictionary();
            m_Analyzer = new MainLib.ZalAnalyzer();
            m_dictLexemes = new Dictionary<LexemeDataPanel, MainLib.ILexeme>();
            m_listWordForms = new List<MainLib.IWordForm>();

            m_dictGender = new Dictionary<MainLib.ET_Gender, string>();
            m_dictGender.Add(MainLib.ET_Gender.GENDER_UNDEFINED, "Undefined");
            m_dictGender.Add(MainLib.ET_Gender.GENDER_M, "M");
            m_dictGender.Add(MainLib.ET_Gender.GENDER_F, "F");
            m_dictGender.Add(MainLib.ET_Gender.GENDER_N, "N");

            m_dictNumber = new Dictionary<MainLib.ET_Number, string>();
            m_dictNumber.Add(MainLib.ET_Number.NUM_UNDEFINED, "Undefined");
            m_dictNumber.Add(MainLib.ET_Number.NUM_SG, "Sg");
            m_dictNumber.Add(MainLib.ET_Number.NUM_PL, "Pl");

            m_dictCase = new Dictionary<MainLib.ET_Case, string>();
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

            m_dictAccent = new Dictionary<MainLib.ET_AccentType, string>();
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

        }   //  InitializeData()

        protected void ShowLexemeDetails (LexemeDataPanel ldpSource)
        {
            MainLib.ILexeme lexeme = m_dictLexemes[ldpSource];
            lexeme.GenerateWordForms();

            TabPage tabPageDetails = new TabPage(lexeme.InitialForm);

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

                    np.SetForm(sKey, strWordForm);

                }   // foreach

            }

            if (MainLib.ET_PartOfSpeech.POS_ADJ == lexeme.PartOfSpeech)
            {
                AdjPanel ap = new AdjPanel();
                tabPageDetails.Controls.Add(ap);
//                ap.sLexName = grSt;

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
                VerbPanel vp = new VerbPanel (lexeme);
                SubscribeToVerbEvents (vp);
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
            tabControl.SelectedTab = tabPageDetails;

        }   //  ShowLexemeDetails (...)

        protected void ShowParticipialForms (MainLib.ILexeme lexeme,
                                             MainLib.ET_Subparadigm eoSpLong,
                                             MainLib.ET_Subparadigm eoSpShort)
        {
            // Expect word forms to be ready by now

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
                ShowLongParticipialForms (adjPanel, lexeme, eoSpLong);
                if (MainLib.ET_Subparadigm.SUBPARADIGM_UNDEFINED != eoSpShort)
                {
                    ShowShortParticipialForms (adjPanel, lexeme, eoSpShort);
                }
                tabControl.SelectedTab = tabPageDetails;
            }
        }

        protected void ShowShortParticipialForms (AdjPanel ap, 
                                                  MainLib.ILexeme lexeme, 
                                                  MainLib.ET_Subparadigm eoSpShort)
        {
            MainLib.IFormDescriptor fd = lexeme.FormDescriptor;
            fd.PartOfSpeech = MainLib.ET_PartOfSpeech.POS_VERB;
            fd.Subparadigm = eoSpShort;
//            fd.Reflexivity = lexeme.IsReflexive;
            fd.Number = MainLib.ET_Number.NUM_SG;
            for (MainLib.ET_Gender eo_gender = MainLib.ET_Gender.GENDER_M;
                 eo_gender < MainLib.ET_Gender.GENDER_COUNT;
                 ++eo_gender)
            {
                fd.Gender = eo_gender;
                fd.FindForms();
                for (int iForm = 1; iForm <= fd.Count; ++iForm)
                {
                    MainLib.IWordForm wf = (MainLib.IWordForm)fd[iForm];
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
            fd.Number = MainLib.ET_Number.NUM_PL;
            fd.Gender = MainLib.ET_Gender.GENDER_UNDEFINED;
            fd.FindForms();
            for (int iForm = 1; iForm <= fd.Count; ++iForm)
            {
                MainLib.IWordForm wf = (MainLib.IWordForm)fd[iForm];
                string sKey = "Short";
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
        }   //  ShowShortParticipialForms (...)

        protected void ShowLongParticipialForms (AdjPanel adjPanel, 
                                                 MainLib.ILexeme lexeme, 
                                                 MainLib.ET_Subparadigm eoSpLong)
        {
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

                adjPanel.SetForm(sKey, strWordForm);

            }   //  foreach (...)

        }   //  ShowLongParticipialForms (...)

        protected void Preprocess (string sSearchString)
        {
            int i_Stress = Convert.ToInt32(true); // It should've been bool
            if (Regex.IsMatch (sSearchString, (string)">"))
            {
                i_Stress = Convert.ToInt32(true);
            }
            else
            {
                i_Stress = Convert.ToInt32(true); // Test
            }
            sSearchString = sSearchString.Replace(">", "");
            if (Regex.IsMatch (sSearchString, (string)"\\-"))
            {
                string[] arr_Range = Regex.Split (sSearchString, (string)"([0-9]*)\\-([0-9]*)");
                long l_start_id = long.Parse(arr_Range[1]);
                long l_end_id = long.Parse(arr_Range[2]);
                m_Analyzer.PrepareLexemes (l_start_id, l_end_id, i_Stress);
            }
            else
            {
                long l_lexeme_id = long.Parse (sSearchString);
                m_Analyzer.PrepareLexeme(l_lexeme_id, i_Stress);
            }

        }   //  Preprocess (...)

        protected void ShowParseOutput()
        {
            int iWordform = 0;
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
//                lexPanel.Controls.Add(ap);
                iWordform++;
            }

        }   //  ShowParseOutput()

    }   //  public partial class TestApplet

}   //  namespace TestUI
