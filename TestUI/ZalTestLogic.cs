using System;
using System.Collections.Generic;
using System.Collections;
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
            m_DelegateUpdateProgressBar = new DelegateUpdateProgressBar(this.UpdateProgressBar);

            m_bDBOpen = false;

            m_sDbPath = Properties.Settings.Default.DbPath;

            m_lexemeToTabs = new Dictionary<MainLib.ILexeme, ArrayList>();

            m_Dictionary = new MainLib.ZalDictionary();
            m_LexPreprocessor = new MainLib.ZalLexPreprocessor();
            m_Analyzer = new MainLib.ZalAnalyzer();
            m_TextAnalyzer = new TextAnalyzer(m_Analyzer);
            m_hashLexemes = new Dictionary<LexemeDataPanel, MainLib.ILexeme>();
            m_listWordForms = new List<MainLib.IWordForm>();

            m_hashGender = new Dictionary<MainLib.ET_Gender, string>();
            m_hashGender.Add(MainLib.ET_Gender.GENDER_UNDEFINED, "Undefined");
            m_hashGender.Add(MainLib.ET_Gender.GENDER_M, "M");
            m_hashGender.Add(MainLib.ET_Gender.GENDER_F, "F");
            m_hashGender.Add(MainLib.ET_Gender.GENDER_N, "N");

            m_hashNumber = new Dictionary<MainLib.ET_Number, string>();
            m_hashNumber.Add(MainLib.ET_Number.NUM_UNDEFINED, "Undefined");
            m_hashNumber.Add(MainLib.ET_Number.NUM_SG, "Sg");
            m_hashNumber.Add(MainLib.ET_Number.NUM_PL, "Pl");

            m_hashCase = new Dictionary<MainLib.ET_Case, string>();
            m_hashCase.Add(MainLib.ET_Case.CASE_UNDEFINED, "Undefined");
            m_hashCase.Add(MainLib.ET_Case.CASE_NOM, "N");
            m_hashCase.Add(MainLib.ET_Case.CASE_ACC, "A");
            m_hashCase.Add(MainLib.ET_Case.CASE_GEN, "G");
            m_hashCase.Add(MainLib.ET_Case.CASE_PART, "G2");
            m_hashCase.Add(MainLib.ET_Case.CASE_LOC, "P2");
            m_hashCase.Add(MainLib.ET_Case.CASE_DAT, "D");
            m_hashCase.Add(MainLib.ET_Case.CASE_INST, "I");
            m_hashCase.Add(MainLib.ET_Case.CASE_PREP, "P");

            m_hashPerson = new Dictionary<MainLib.ET_Person, string>();
            m_hashPerson.Add(MainLib.ET_Person.PERSON_UNDEFINED, "Undefined");
            m_hashPerson.Add(MainLib.ET_Person.PERSON_1, "1");
            m_hashPerson.Add(MainLib.ET_Person.PERSON_2, "2");
            m_hashPerson.Add(MainLib.ET_Person.PERSON_3, "3");

            m_hashAccent = new Dictionary<MainLib.ET_AccentType, string>();
            m_hashAccent.Add(MainLib.ET_AccentType.AT_UNDEFINED, "Undefined");
            m_hashAccent.Add(MainLib.ET_AccentType.AT_A, "a");
            m_hashAccent.Add(MainLib.ET_AccentType.AT_A1, "a'");
            m_hashAccent.Add(MainLib.ET_AccentType.AT_B, "b");
            m_hashAccent.Add(MainLib.ET_AccentType.AT_B1, "b'");
            m_hashAccent.Add(MainLib.ET_AccentType.AT_C, "c");
            m_hashAccent.Add(MainLib.ET_AccentType.AT_C1, "c'");
            m_hashAccent.Add(MainLib.ET_AccentType.AT_C2, "c''");
            m_hashAccent.Add(MainLib.ET_AccentType.AT_D, "d");
            m_hashAccent.Add(MainLib.ET_AccentType.AT_D1, "d'");
            m_hashAccent.Add(MainLib.ET_AccentType.AT_E, "e");
            m_hashAccent.Add(MainLib.ET_AccentType.AT_F, "f");
            m_hashAccent.Add(MainLib.ET_AccentType.AT_F1, "f'");
            m_hashAccent.Add(MainLib.ET_AccentType.AT_F2, "f''");

        }   //  InitializeData()

        private void GetDbPath()
        {
            try
            {
                FileDialog fd = new OpenFileDialog();
                fd.CheckFileExists = false;
                fd.CheckPathExists = true;
                fd.FileName = m_sDbPath;
                fd.Filter = "SQLite3 Files (*.db3)|*.db3|SQLite Files (*.db)|*.db|All Files (*.*)|*.*";
//                m_bDBOpen = false;
                DialogResult dr = fd.ShowDialog();
                if (DialogResult.OK == dr)
                {
                    if (File.Exists(fd.FileName))
                    {
                        m_sDbPath = fd.FileName;
                        m_Dictionary.DbPath = m_sDbPath;
                        m_Analyzer.LoadDb(m_sDbPath);
                        m_LexPreprocessor.LoadDb(m_sDbPath);
                        toolStripStatusLabel.Text = m_sDbPath;

                        // TODO path validation

                        synthesizeToolStripMenuItem.Enabled = true;
                        analyzeToolStripMenuItem.Enabled = true;
                        testToolStripMenuItem.Enabled = true;
                        m_bDBOpen = true;
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
            catch (Exception ex)
            {
                string sMsg = "GetDbPath: ";
                MainLib.ZalError err = new MainLib.ZalError();
                sMsg += err.LastError;
                MessageBox.Show (sMsg, "Error", MessageBoxButtons.OK);
                return;
            }

        }       //  GetDbPath()

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
                m_hashLexemes.Add(ldp, lex);
                SubscribeToLexemeEvents(ldp);
                ldp.Location = new System.Drawing.Point(0, iLexeme * ldp.Size.Height + 4);
                ldp.sInitialForm = lex.InitialForm;
                ldp.sGraphicStem = lex.GraphicStem;
                ldp.iInflectionId = lex.InflectionId;
                ldp.sMainSymbol = lex.MainSymbol;
                ldp.sType = lex.Type.ToString();
                ldp.sStressType = m_hashAccent[lex.AccentType1];
                if (lex.AccentType2 != MainLib.ET_AccentType.AT_UNDEFINED)
                {
                    ldp.sStressType += "/" + m_hashAccent[lex.AccentType2];
                }

                tabPageLexemes.Controls.Add(ldp);
                ldp.Left += 20;
                ldp.Top += 20;
                if (0 == iLexeme)
                {
                    ldpFocused = ldp;
                }

                ArrayList alTabs = new ArrayList();
                m_lexemeToTabs.Add (lex, alTabs);

                ++iLexeme;
            }

            tabControl.Controls.Add (tabPageLexemes);
            tabControl.SelectTab (tabPageLexemes);
            ldpFocused.FocusShowButton();

        }   //  ShowLexemes()

        protected void ShowLexemeDetails (LexemeDataPanel ldpSource)
        {
            MainLib.ILexeme lexeme = m_hashLexemes[ldpSource];

            try
            {
                lexeme.GenerateWordForms();
            }
            catch (Exception ex)
            {
                string sMsg = "ShowLexemeDetails: ";
                MainLib.ZalError err = new MainLib.ZalError();
                sMsg += err.LastError;
                MessageBox.Show (sMsg, "Error", MessageBoxButtons.OK);
                return;
            }

            TabPage tabPageDetails = new TabPage(lexeme.InitialForm);
            ArrayList al = m_lexemeToTabs[lexeme];
            al.Add (tabPageDetails);

            string grSt = lexeme.GraphicStem;

            if ((MainLib.ET_PartOfSpeech.POS_NOUN == lexeme.PartOfSpeech) ||
                (MainLib.ET_PartOfSpeech.POS_PRONOUN == lexeme.PartOfSpeech) ||
                (MainLib.ET_PartOfSpeech.POS_NUM == lexeme.PartOfSpeech))
            {
                NounPanel np = new NounPanel();
                tabPageDetails.Controls.Add(np);
                np.sLexName = grSt;

                foreach (MainLib.IWordForm wf in lexeme)
                {
                    string sKey = m_hashCase[wf.Case];
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

                    np.SetForm (sKey, strWordForm, wf.Status);

                }   // foreach
            }

            if ((MainLib.ET_PartOfSpeech.POS_ADJ == lexeme.PartOfSpeech) ||
                (MainLib.ET_PartOfSpeech.POS_PRONOUN_ADJ == lexeme.PartOfSpeech) ||
                (MainLib.ET_PartOfSpeech.POS_NUM_ADJ == lexeme.PartOfSpeech))
            {
                AdjPanel ap = new AdjPanel();
                tabPageDetails.Controls.Add(ap);

                foreach (MainLib.IWordForm wf in lexeme)
                {
                    string sKey = "";

                    if (MainLib.ET_Subparadigm.SUBPARADIGM_LONG_ADJ == wf.Subparadigm)
                    {
                        if (MainLib.ET_Number.NUM_SG == wf.Number)
                        {
                            sKey = m_hashGender[wf.Gender];
                        }
                        sKey += m_hashCase[wf.Case];
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

                        ap.SetForm (sKey, strWordForm, wf.Status);

                        continue;
                    }

                    if (MainLib.ET_Subparadigm.SUBPARADIGM_SHORT_ADJ == wf.Subparadigm)
                    {
                        sKey = "Short";
                        if (MainLib.ET_Number.NUM_SG == wf.Number)
                        {
                            sKey += m_hashGender[wf.Gender];
                        }
                        sKey += m_hashNumber[wf.Number];

                        string strWordForm = wf.Wordform;
                        if (wf.Stress >= 0)
                        {
                            if (strWordForm[wf.Stress] != 'ё')
                            {
                                string strStressMark = new string('\x301', 1);
                                strWordForm = strWordForm.Insert(wf.Stress + 1, strStressMark);
                            }
                        }

                        ap.SetForm (sKey, strWordForm, wf.Status);

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

                        ap.SetForm (sKey, strWordForm, wf.Status);

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
                        string sKey = "Pres" + m_hashPerson[wf.Person];
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
                            }
                        }
                        catch (Exception ex)
                        {
                            string sMsg = "LexemeDataPanel_ShowLexemeDetails: ";
                            sMsg += ex.Message;
                            MessageBox.Show("Bad stress position", "Zal Error", MessageBoxButtons.OK);
                            continue;
                        }

                        if (strWordForm[wf.Stress] != 'ё')
                        {
                            string strStressMark = new string('\x301', 1);
                            strWordForm = strWordForm.Insert (wf.Stress + 1, strStressMark);
                        }
                        vp.SetForm (sKey, strWordForm, wf.Status);
                    }

                    if (MainLib.ET_Subparadigm.SUBPARADIGM_PAST_TENSE == wf.Subparadigm)
                    {
                        string sKey = "Past";
                        if (MainLib.ET_Number.NUM_SG == wf.Number)
                        {
                            sKey += m_hashGender[wf.Gender];
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

                        vp.SetForm (sKey, strWordForm, wf.Status);
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

                        vp.SetForm (sKey, strWordForm, wf.Status);
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

                            vp.SetForm (sKey, strWordForm, wf.Status);
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

                            vp.SetForm (sKey, strWordForm, wf.Status);
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
                                    MessageBox.Show ("Bad stress position", "Zal Error", MessageBoxButtons.OK);
                                    return;
                                }
                                if (strWordForm[wf.Stress] != 'ё')
                                {
                                    string strStressMark = new string ('\x301', 1);
                                    strWordForm = strWordForm.Insert (wf.Stress + 1, strStressMark);
                                }
                            }
                        }
                        catch (Exception ex)
                        {
                            string sMsg = "LexemeDataPanel_ShowLexemeDetails: ";
                            sMsg += ex.Message;
                        }

                        vp.SetForm ("PresAdverbial", strWordForm, wf.Status);
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

                        vp.SetForm ("PastAdverbial", strWordForm, wf.Status);
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

                            vp.SetForm (sKey, strWordForm, wf.Status);
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

                            vp.SetForm (sKey, strWordForm, wf.Status);
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

            MainLib.IFormFinder fd = lexeme.FormDescriptor;
            fd.PartOfSpeech = MainLib.ET_PartOfSpeech.POS_VERB;
            fd.Subparadigm = eoSpLong;
            fd.Number = MainLib.ET_Number.NUM_SG;
            fd.Case = MainLib.ET_Case.CASE_NOM;
            fd.Gender = MainLib.ET_Gender.GENDER_M;
            fd.Animacy = MainLib.ET_Animacy.ANIM_NO;
            fd.Aspect = lexeme.Aspect;
            fd.Reflexivity = lexeme.IsReflexive;
            fd.FindForms();
            if (fd.Count > 0)
            {
                MainLib.IWordForm wf = (MainLib.IWordForm)fd[1];
                AdjPanel adjPanel = new AdjPanel();
                TabPage tabPageDetails = new TabPage (wf.Wordform);
                ArrayList al = m_lexemeToTabs[lexeme];
                al.Add (tabPageDetails);
                tabPageDetails.Controls.Add (adjPanel);
                tabControl.Controls.Add (tabPageDetails);
                ShowLongParticipialForms (adjPanel, lexeme, eoSpLong);
                if (MainLib.ET_Subparadigm.SUBPARADIGM_UNDEFINED != eoSpShort)
                {
                    ShowShortParticipialForms (adjPanel, lexeme, eoSpShort);
                }
                tabControl.SelectedTab = tabPageDetails;
            }
            else
            {
                MessageBox.Show ("No forms available", "Zal Error", MessageBoxButtons.OK);
            }
        }       //  ShowParticipialForms (...)

        protected void ShowShortParticipialForms (AdjPanel ap, 
                                                  MainLib.ILexeme lexeme, 
                                                  MainLib.ET_Subparadigm eoSpShort)
        {
            MainLib.IFormFinder fd = lexeme.FormDescriptor;
            fd.PartOfSpeech = MainLib.ET_PartOfSpeech.POS_VERB;
            fd.Subparadigm = eoSpShort;
            fd.Reflexivity = lexeme.IsReflexive;
            fd.Aspect = lexeme.Aspect;
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
                    sKey += m_hashGender[wf.Gender];
                    sKey += m_hashNumber[wf.Number];
                    string strWordForm = wf.Wordform;
                    if (wf.Stress >= 0)
                    {
                        if (strWordForm[wf.Stress] != 'ё')
                        {
                            string strStressMark = new string('\x301', 1);
                            strWordForm = strWordForm.Insert(wf.Stress + 1, strStressMark);
                        }
                    }
                    ap.SetForm (sKey, strWordForm, wf.Status);
                }
            }
            fd.Number = MainLib.ET_Number.NUM_PL;
            fd.Gender = MainLib.ET_Gender.GENDER_UNDEFINED;
            fd.FindForms();
            for (int iForm = 1; iForm <= fd.Count; ++iForm)
            {
                MainLib.IWordForm wf = (MainLib.IWordForm)fd[iForm];
                string sKey = "Short";
                sKey += m_hashNumber[wf.Number];
                string strWordForm = wf.Wordform;
                if (wf.Stress >= 0)
                {
                    if (strWordForm[wf.Stress] != 'ё')
                    {
                        string strStressMark = new string('\x301', 1);
                        strWordForm = strWordForm.Insert(wf.Stress + 1, strStressMark);
                    }
                }
                ap.SetForm (sKey, strWordForm, wf.Status);
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
                    sKey = m_hashGender[wf.Gender];
                }

                sKey += m_hashCase[wf.Case];
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

                adjPanel.SetForm (sKey, strWordForm, wf.Status);

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
                m_LexPreprocessor.PrepareLexemes (l_start_id, l_end_id, i_Stress);
            }
            else
            {
                long l_lexeme_id = long.Parse (sSearchString);
                m_LexPreprocessor.PrepareLexeme(l_lexeme_id, i_Stress);
            }

        }   //  Preprocess (...)

        protected void ShowParseOutput()
        {
            int iWordform = 0, iPreviousID = -1;
            tabControl.TabPages.Clear();
            foreach (MainLib.IWordForm wf in m_Analyzer)
            {
                m_listWordForms.Add(wf);
                AnalysisPanel ap = new AnalysisPanel(iWordform);
                ap.Location = new System.Drawing.Point(0, iWordform * ap.Size.Height + 4);
                ap.sLemma = wf.Lemma;
                ap.sWordform = wf.Wordform;
                ap.sID = wf.LexemeId.ToString();
                ap.eoPOS = wf.PartOfSpeech;
                ap.eoAspect = wf.Aspect;
                ap.eoGender = wf.Gender;
                ap.eoCase = wf.Case;
                ap.eoNumber = wf.Number;
                ap.eoAnimacy = wf.Animacy;
                ap.eoReflexiveness = wf.IsReflexive;
                ap.eoSubparadigm = wf.Subparadigm;
                if (wf.LexemeId != iPreviousID)
                {
                    iWordform = 0;
                    iPreviousID = wf.LexemeId;
                    TabPage tab_Lexeme = new TabPage(wf.Lemma);
                    tab_Lexeme.AutoScroll = true;
                    tab_Lexeme.Controls.Add(ap);
                    tabControl.TabPages.Add(tab_Lexeme);
                    tab_Lexeme.Show();
                    tab_Lexeme.Focus();
                }
                else
                {
                    tabControl.TabPages[tabControl.TabPages.Count - 1].Controls.Add(ap);
                }
                ++iWordform;
                ap.Show();
            }

        }   //  ShowParseOutput()

        public void SaveTestResults()
        {
            SaveFileDialog fd = new SaveFileDialog();
            fd.CheckFileExists = false;
            fd.CheckPathExists = true;
            fd.Title = "Save Report Data"; 
            DialogResult dr = fd.ShowDialog();
            if (DialogResult.OK != dr)
            {
                return;
            }

            using (StreamWriter swReport = new StreamWriter (fd.FileName, false, Encoding.Unicode))
            {
                try
                {
                    swReport.WriteLine ("     ==== Test Report {0} ====\r\n", DateTime.Now);
                    foreach (MainLib.IVerifier v in m_TestData)
                    {
                        if (v.Result != MainLib.ET_TestResult.TEST_RESULT_UNDEFINED)
                        {
                            string sLine = v.Headword;
                            sLine += new string(' ', 40 - sLine.Length);
                            sLine += (MainLib.ET_TestResult.TEST_RESULT_OK == v.Result) ?
                                "     Pass" : "***  Fail";
                            swReport.WriteLine(sLine);
                        }
                    }
                }
                catch (Exception ex)
                {
                    string sMsg = "Error in StreamWriter: ";
                    sMsg += ex.Message;
                    MessageBox.Show (sMsg, 
                                     "Zal Error", 
                                     MessageBoxButtons.OK, 
                                     MessageBoxIcon.Exclamation);
                    return;
                }
            }
        }       //  SaveTestResults()

        public void CloseCurrentTab()
        {
            tabControl.TabPages.Remove (tabControl.SelectedTab);
        }

        public void CloseLexemeTabs (LexemeDataPanel ldp)
        {
            ArrayList alTabs = m_lexemeToTabs [m_hashLexemes[ldp]];
            foreach (TabPage tp in alTabs)
            {
                tabControl.TabPages.Remove(tp);
            }

            TabPage tpParent = (TabPage)ldp.Parent;
            tpParent.Controls.Remove(ldp);
            if (0 == tpParent.Controls.Count)
            {
                tabControl.TabPages.Remove(tpParent);
            }
        }

    }   //  public partial class TestApplet

}   //  namespace TestUI
