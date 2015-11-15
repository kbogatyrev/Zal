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

using MainLibManaged;

namespace TestUI
{
    public partial class TestApplet
    {
        protected void InitializeData()
        {
//            m_DelegateUpdateProgressBar = new DelegateUpdateProgressBar(this.UpdateProgressBar);

            m_bDBOpen = false;

            m_sDbPath = Properties.Settings.Default.DbPath;

            m_lexemeToTabs = new Dictionary<CLexemeManaged, ArrayList>();

            m_Dictionary = new CDictionaryManaged();
//            m_LexPreprocessor = new MainLib.ZalLexPreprocessor();
//            m_Analyzer = new MainLib.ZalAnalyzer();
//            m_TextAnalyzer = new TextAnalyzer(m_Analyzer);
            m_hashLexemes = new Dictionary<LexemeDataPanel, CLexemeManaged>();
            m_listWordForms = new List<CWordFormManaged>();

            m_hashGender = new Dictionary<EM_Gender, string>();
            m_hashGender.Add(EM_Gender.GENDER_UNDEFINED, "Undefined");
            m_hashGender.Add(EM_Gender.GENDER_M, "M");
            m_hashGender.Add(EM_Gender.GENDER_F, "F");
            m_hashGender.Add(EM_Gender.GENDER_N, "N");

            m_hashNumber = new Dictionary<EM_Number, string>();
            m_hashNumber.Add(EM_Number.NUM_UNDEFINED, "Undefined");
            m_hashNumber.Add(EM_Number.NUM_SG, "Sg");
            m_hashNumber.Add(EM_Number.NUM_PL, "Pl");

            m_hashCase = new Dictionary<EM_Case, string>();
            m_hashCase.Add(EM_Case.CASE_UNDEFINED, "Undefined");
            m_hashCase.Add(EM_Case.CASE_NOM, "N");
            m_hashCase.Add(EM_Case.CASE_ACC, "A");
            m_hashCase.Add(EM_Case.CASE_GEN, "G");
            m_hashCase.Add(EM_Case.CASE_PART, "G2");
            m_hashCase.Add(EM_Case.CASE_LOC, "P2");
            m_hashCase.Add(EM_Case.CASE_DAT, "D");
            m_hashCase.Add(EM_Case.CASE_INST, "I");
            m_hashCase.Add(EM_Case.CASE_PREP, "P");

            m_hashPerson = new Dictionary<EM_Person, string>();
            m_hashPerson.Add(EM_Person.PERSON_UNDEFINED, "Undefined");
            m_hashPerson.Add(EM_Person.PERSON_1, "1");
            m_hashPerson.Add(EM_Person.PERSON_2, "2");
            m_hashPerson.Add(EM_Person.PERSON_3, "3");

            m_hashAccent = new Dictionary<EM_AccentType, string>();
            m_hashAccent.Add(EM_AccentType.AT_UNDEFINED, "Undefined");
            m_hashAccent.Add(EM_AccentType.AT_A, "a");
            m_hashAccent.Add(EM_AccentType.AT_A1, "a'");
            m_hashAccent.Add(EM_AccentType.AT_B, "b");
            m_hashAccent.Add(EM_AccentType.AT_B1, "b'");
            m_hashAccent.Add(EM_AccentType.AT_C, "c");
            m_hashAccent.Add(EM_AccentType.AT_C1, "c'");
            m_hashAccent.Add(EM_AccentType.AT_C2, "c''");
            m_hashAccent.Add(EM_AccentType.AT_D, "d");
            m_hashAccent.Add(EM_AccentType.AT_D1, "d'");
            m_hashAccent.Add(EM_AccentType.AT_E, "e");
            m_hashAccent.Add(EM_AccentType.AT_F, "f");
            m_hashAccent.Add(EM_AccentType.AT_F1, "f'");
            m_hashAccent.Add(EM_AccentType.AT_F2, "f''");

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
//                        m_Dictionary.DbPath = m_sDbPath;
//                        m_Analyzer.LoadDb(m_sDbPath);
//                        m_LexPreprocessor.LoadDb(m_sDbPath);
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
//                MainLib.ZalError err = new MainLib.ZalError();
//                sMsg += err.LastError;
                sMsg += ex.Message;
                MessageBox.Show (sMsg, "Error", MessageBoxButtons.OK);
                return;
            }

        }       //  GetDbPath()

        private void ShowLexemes()
        {
            int iNumLexemes = 0;
            if (m_Dictionary.eCountLexemes(ref iNumLexemes) != EM_ReturnCode.H_NO_ERROR)
            {
                MessageBox.Show(this, "Not in the dictionary.", "Zal Synthesizer");
                return;
            }

            TabPage tabPageLexemes = new TabPage (m_sSearchString);
            tabPageLexemes.AutoScroll = true;

            LexemeDataPanel ldpFocused = null;

            int iLexeme = 0;
            CLexemeManaged lex = null;
            EM_ReturnCode eRet = (EM_ReturnCode)m_Dictionary.eGetFirstLexeme(ref lex);
            do
            {
                if (CErrorCode.bError(eRet))
                {
                    continue;
                }

                if (eRet != EM_ReturnCode.H_NO_ERROR)
                {
                    MessageBox.Show(this, "Unable to retrieve lexeme data.", "Zal Synthesizer");
                    return;
                }

                if (lex.sSourceForm() == m_sSearchString)
                {
                    LexemeDataPanel ldp = new LexemeDataPanel();
                    m_hashLexemes.Add(ldp, lex);
                    SubscribeToLexemeEvents(ldp);
                    ldp.Location = new System.Drawing.Point(0, iLexeme * ldp.Size.Height + 4);
                    ldp.sInitialForm = lex.sSourceForm();
                    ldp.sGraphicStem = lex.sGraphicStem();
                    ldp.iInflectionId = lex.iInflectionId();
                    ldp.sMainSymbol = lex.sMainSymbol();
                    ldp.sType = lex.iType().ToString();
                    ldp.sStressType = (m_hashAccent[lex.eAccentType1()].ToString());
                    if (lex.eAccentType2() != EM_AccentType.AT_UNDEFINED)
                    {
                        ldp.sStressType += "/" + m_hashAccent[lex.eAccentType2()];
                    }

                    tabPageLexemes.Controls.Add(ldp);
                    ldp.Left += 20;
                    ldp.Top += 20;
                    if (0 == iLexeme)
                    {
                        ldpFocused = ldp;
                    }

                    ArrayList alTabs = new ArrayList();
                    m_lexemeToTabs.Add(lex, alTabs);

                    ++iLexeme;
                }

                eRet = (EM_ReturnCode)m_Dictionary.eGetNextLexeme(ref lex);

            } while (EM_ReturnCode.H_NO_ERROR == eRet);

            tabControl.Controls.Add (tabPageLexemes);
            tabControl.SelectTab (tabPageLexemes);
            ldpFocused.FocusShowButton();

        }   //  ShowLexemes()

        protected void ShowLexemeDetails (LexemeDataPanel ldpSource)
        {
            CLexemeManaged lexeme = m_hashLexemes[ldpSource];

            try
            {
                lexeme.eGenerateParadigm();
            }
            catch (Exception ex)
            {
                string sMsg = "ShowLexemeDetails: ";
//                MainLib.ZalError err = new MainLib.ZalError();
//                sMsg += err.LastError;
                sMsg += ex.Message;
                MessageBox.Show (sMsg, "Error", MessageBoxButtons.OK);
                return;
            }

            TabPage tabPageDetails = new TabPage(lexeme.sSourceForm());
            ArrayList al = m_lexemeToTabs[lexeme];
            al.Add (tabPageDetails);

            string grSt = lexeme.sGraphicStem();

            if ((EM_PartOfSpeech.POS_NOUN == lexeme.ePartOfSpeech()) ||
                (EM_PartOfSpeech.POS_PRONOUN == lexeme.ePartOfSpeech()) ||
                (EM_PartOfSpeech.POS_NUM == lexeme.ePartOfSpeech()))
            {
                NounPanel np = new NounPanel();
                tabPageDetails.Controls.Add(np);
                np.sLexName = grSt;

                CWordFormManaged wf = null;
                EM_ReturnCode eRet = (EM_ReturnCode)lexeme.eGetFirstWordForm(ref wf);
                do
                {
                    if (EM_ReturnCode.H_NO_ERROR != eRet)
                    {
                        continue;
                    }
                    string sKey = m_hashCase[wf.eCase()];
                    sKey += (wf.eNumber() == EM_Number.NUM_SG) ? "Sg" : "Pl";

                    string sWordForm = wf.sWordForm();
                    MarkStress(ref sWordForm, wf);
                    np.SetForm(sKey, sWordForm, wf.eStatus());

                    eRet = (EM_ReturnCode)lexeme.eGetNextWordForm(ref wf);

                } while (EM_ReturnCode.H_NO_ERROR == eRet);

            }

            if (EM_PartOfSpeech.POS_ADJ == lexeme.ePartOfSpeech() ||
                EM_PartOfSpeech.POS_PRONOUN_ADJ == lexeme.ePartOfSpeech() ||
                EM_PartOfSpeech.POS_NUM_ADJ == lexeme.ePartOfSpeech())
            {
                AdjPanel ap = new AdjPanel();
                tabPageDetails.Controls.Add(ap);

                CWordFormManaged wf = null;
                EM_ReturnCode eRet = (EM_ReturnCode)lexeme.eGetFirstWordForm(ref wf);
                do
                {
                    if (CErrorCode.bError(eRet))
                    {
                        continue;
                    }

                    string sKey = "";
                    if (EM_Subparadigm.SUBPARADIGM_LONG_ADJ == wf.eSubparadigm())
                    {
                        if (EM_Number.NUM_SG == wf.eNumber())
                        {
                            sKey = m_hashGender[wf.eGender()];
                        }
                        sKey += m_hashCase[wf.eCase()];
                        sKey += (EM_Number.NUM_SG == wf.eNumber()) ? "Sg" : "Pl";
                        if (EM_Case.CASE_ACC == wf.eCase())
                        {
                            if ((EM_Gender.GENDER_M == wf.eGender() &&
                                 EM_Number.NUM_SG == wf.eNumber()) ||
                                 (EM_Number.NUM_PL == wf.eNumber()))
                            {
                                sKey += (EM_Animacy.ANIM_YES == wf.eAnimacy()) ? "Anim" : "Inanim";
                            }
                        }

                        string sWordForm = wf.sWordForm();
                        MarkStress(ref sWordForm, wf);

                        ap.SetForm(sKey, sWordForm, wf.eStatus());
                    }

                    if (EM_Subparadigm.SUBPARADIGM_SHORT_ADJ == wf.eSubparadigm())
                    {
                        sKey = "Short";
                        if (EM_Number.NUM_SG == wf.eNumber())
                        {
                            sKey += m_hashGender[wf.eGender()];
                        }
                        sKey += m_hashNumber[wf.eNumber()];

                        string sWordForm = wf.sWordForm();
                        MarkStress(ref sWordForm, wf);

                        ap.SetForm(sKey, sWordForm, wf.eStatus());

                    }

                    if (EM_Subparadigm.SUBPARADIGM_COMPARATIVE == wf.eSubparadigm())
                    {
                        sKey = "Comparative";

                        string sWordForm = wf.sWordForm();
                        MarkStress(ref sWordForm, wf);
                        ap.SetForm(sKey, sWordForm, wf.eStatus());
                    }

                    eRet = (EM_ReturnCode)lexeme.eGetNextWordForm(ref wf);

                } while (EM_ReturnCode.H_NO_ERROR == eRet);

            }   //  if (MainLib.EM_PartOfSpeech.POS_ADJ == lexeme.ePartOfSpeech())

            if (EM_PartOfSpeech.POS_VERB == lexeme.ePartOfSpeech())
            {
                VerbPanel vp = new VerbPanel(lexeme);
                SubscribeToVerbEvents(vp);
                tabPageDetails.Controls.Add(vp);
                vp.sLexName = grSt;

                CWordFormManaged wf = null;
                EM_ReturnCode eRet = (EM_ReturnCode)lexeme.eGetFirstWordForm(ref wf);
                string sWordForm = wf.sWordForm();

                do
                {
                    if (CErrorCode.bError(eRet))
                    {
                        continue;
                    }

                    EM_Subparadigm eSubparadigm = wf.eSubparadigm();
                    switch (eSubparadigm)
                    {
                        case EM_Subparadigm.SUBPARADIGM_PRESENT_TENSE:
                        {
                            string sKey = "Pres" + m_hashPerson[wf.ePerson()];
                            sKey += (wf.eNumber() == EM_Number.NUM_SG) ? "Sg" : "Pl";
                            MarkStress(ref sWordForm, wf);
                            vp.SetForm(sKey, sWordForm, wf.eStatus());

                            break;
                        }

                        case EM_Subparadigm.SUBPARADIGM_PAST_TENSE:
                        {
                            string sKey = "Past";
                            if (EM_Number.NUM_SG == wf.eNumber())
                            {
                                sKey += m_hashGender[wf.eGender()];
                            }
                            sKey += (wf.eNumber() == EM_Number.NUM_SG) ? "Sg" : "Pl";

                            MarkStress(ref sWordForm, wf);
                            vp.SetForm(sKey, sWordForm, wf.eStatus());

                            break;
                        }

                        case EM_Subparadigm.SUBPARADIGM_INFINITIVE:
                        {
                            string sKey = "Infinitive";
                            MarkStress(ref sWordForm, wf);
                            vp.SetForm(sKey, sWordForm, wf.eStatus());

                            break;
                        }

                        case EM_Subparadigm.SUBPARADIGM_IMPERATIVE:
                        {
                            string sKey = "Imperative";
                            sKey += (wf.eNumber() == EM_Number.NUM_SG) ? "Sg" : "Pl";

                            MarkStress(ref sWordForm, wf);
                            vp.SetForm(sKey, sWordForm, wf.eStatus());

                            break;
                        }

                        case EM_Subparadigm.SUBPARADIGM_PART_PRES_ACT:
                        {
                            string sKey = "PartPresActive";
                            if (wf.eNumber() == EM_Number.NUM_SG &&
                                wf.eGender() == EM_Gender.GENDER_M &&
                                wf.eCase() == EM_Case.CASE_NOM)
                            {
                                MarkStress(ref sWordForm, wf);
                                vp.SetForm(sKey, sWordForm, wf.eStatus());
                            }

                            break;
                        }

                        case EM_Subparadigm.SUBPARADIGM_PART_PAST_ACT:
                        {
                            string sKey = "PartPastActive";
                            if (wf.eNumber() == EM_Number.NUM_SG &&
                                wf.eGender() == EM_Gender.GENDER_M &&
                                wf.eCase() == EM_Case.CASE_NOM)
                            {
                                MarkStress(ref sWordForm, wf);
                                vp.SetForm(sKey, sWordForm, wf.eStatus());
                            }

                            break;
                        }

                        case EM_Subparadigm.SUBPARADIGM_ADVERBIAL_PRESENT:
                        {
                            MarkStress(ref sWordForm, wf);
                            vp.SetForm("PresAdverbial", sWordForm, wf.eStatus());

                            break;
                        }

                        case EM_Subparadigm.SUBPARADIGM_ADVERBIAL_PAST:
                        {
                            MarkStress(ref sWordForm, wf);
                            vp.SetForm("PastAdverbial", sWordForm, wf.eStatus());

                            break;
                        }

                        case EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG:
                        {
                            string sKey = "PartPresPassive";
                            if (wf.eNumber() == EM_Number.NUM_SG &&
                                wf.eGender() == EM_Gender.GENDER_M &&
                                wf.eCase() == EM_Case.CASE_NOM)
                            {
                                MarkStress(ref sWordForm, wf);
                                vp.SetForm(sKey, sWordForm, wf.eStatus());
                            }

                            break;
                        }

                        case EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG:
                        {
                            string sKey = "PartPastPassive";
                            if (wf.eNumber() == EM_Number.NUM_SG &&
                                wf.eGender() == EM_Gender.GENDER_M &&
                                wf.eCase() == EM_Case.CASE_NOM)
                            {
                                MarkStress(ref sWordForm, wf);
                                vp.SetForm(sKey, sWordForm, wf.eStatus());
                            }

                            break;
                        }
                    }   // switch...

                    eRet = (EM_ReturnCode)lexeme.eGetNextWordForm(ref wf);
                    sWordForm = wf.sWordForm();

                } while (EM_ReturnCode.H_NO_ERROR == eRet);

            }   //  if (EM_PartOfSpeech.POS_VERB == lexeme.ePartOfSpeech())

            tabControl.Controls.Add(tabPageDetails);
            tabControl.SelectedTab = tabPageDetails;

        }   //  ShowLexemeDetails (...)

        protected void ShowParticipialForms (CLexemeManaged lexeme,
                                             EM_Subparadigm eoSpLong,
                                             EM_Subparadigm eoSpShort)
        {
            // Expect word forms to be ready by now

            CGramHasherManaged hasher = new CGramHasherManaged();

            hasher.SetPartOfSpeech(EM_PartOfSpeech.POS_VERB);
            hasher.SetSubparadigm(eoSpLong);
            hasher.SetNumber(EM_Number.NUM_SG);
            hasher.SetCase(EM_Case.CASE_NOM);
            hasher.SetGender(EM_Gender.GENDER_M);
            hasher.SetAnimacy(EM_Animacy.ANIM_NO);
            hasher.SetAspect(lexeme.eAspect());
            hasher.SetReflexivity(lexeme.eIsReflexive());

            CWordFormManaged wf = null;
            uint uiForms = lexeme.uiFormCount(hasher.iHash());
            if (uiForms > 0)
            {
                lexeme.eWordFormFromHash(hasher.iHash(), 0, ref wf);
                AdjPanel adjPanel = new AdjPanel();
                TabPage tabPageDetails = new TabPage (wf.sWordForm());
                ArrayList al = m_lexemeToTabs[lexeme];
                al.Add (tabPageDetails);
                tabPageDetails.Controls.Add (adjPanel);
                tabControl.Controls.Add (tabPageDetails);
                ShowLongParticipialForms (adjPanel, lexeme, eoSpLong);
                if (EM_Subparadigm.SUBPARADIGM_UNDEFINED != eoSpShort)
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

        protected void ShowShortParticipialForms (AdjPanel ap, CLexemeManaged lexeme, EM_Subparadigm eoSpShort)
        {
            CGramHasherManaged hasher = new CGramHasherManaged();
            hasher.SetPartOfSpeech(EM_PartOfSpeech.POS_VERB);
            hasher.SetSubparadigm(eoSpShort);
            hasher.SetReflexivity(lexeme.eIsReflexive());
            hasher.SetAspect(lexeme.eAspect());
            hasher.SetNumber(EM_Number.NUM_SG);
            for (EM_Gender eGender = EM_Gender.GENDER_M;
                 eGender < EM_Gender.GENDER_COUNT;
                 ++eGender)
            {
                hasher.SetGender(eGender);
                uint uiForms = lexeme.uiFormCount(hasher.iHash());
                for (uint uiForm = 0; uiForm < uiForms; ++uiForm)
                {
                    CWordFormManaged wf = null;
//                    lexeme.eWordFormFromHash(hasher.iHash(), lexeme.uiFormCount(hasher.iHash()), ref wf);
                    lexeme.eWordFormFromHash(hasher.iHash(), uiForm, ref wf);
                    string sKey = "Short";
                    sKey += m_hashGender[wf.eGender()];
                    sKey += m_hashNumber[wf.eNumber()];
                    string sWordForm = wf.sWordForm();
                    MarkStress(ref sWordForm, wf);
                    ap.SetForm (sKey, sWordForm, wf.eStatus());
                }
            }

            hasher.SetNumber(EM_Number.NUM_PL);
            hasher.SetGender(EM_Gender.GENDER_UNDEFINED);
            for (uint uiForm = 0; uiForm < lexeme.uiFormCount(hasher.iHash()); ++uiForm)
            {
                CWordFormManaged wf = null;
                lexeme.eWordFormFromHash(hasher.iHash(), uiForm, ref wf);
                string sKey = "Short";
                sKey += m_hashNumber[wf.eNumber()];
                string sWordForm = wf.sWordForm();
                MarkStress(ref sWordForm, wf);
                ap.SetForm(sKey, sWordForm, wf.eStatus());
            }

        }   //  ShowShortParticipialForms (...)

        protected void ShowLongParticipialForms (AdjPanel adjPanel, CLexemeManaged lexeme, EM_Subparadigm eoSpLong)
        {
            CWordFormManaged wf = null;
            EM_ReturnCode eRet = (EM_ReturnCode)lexeme.eGetFirstWordForm(ref wf);
            do
            {
                if (!CErrorCode.bError(eRet) && (eoSpLong == wf.eSubparadigm()))
                {
                    string sKey = string.Empty;
                    if (eoSpLong == wf.eSubparadigm() && EM_Number.NUM_SG == wf.eNumber())
                    {
                        sKey = m_hashGender[wf.eGender()];
                    }

                    sKey += m_hashCase[wf.eCase()];
                    sKey += (EM_Number.NUM_SG == wf.eNumber()) ? "Sg" : "Pl";
                    if (EM_Case.CASE_ACC == wf.eCase())
                    {
                        if ((EM_Gender.GENDER_M == wf.eGender() &&
                             EM_Number.NUM_SG == wf.eNumber()) ||
                             (EM_Number.NUM_PL == wf.eNumber()))
                        {
                            sKey += (EM_Animacy.ANIM_YES == wf.eAnimacy()) ? "Anim" : "Inanim";
                        }
                    }

                    string sWordForm = wf.sWordForm();
                    MarkStress(ref sWordForm, wf);

                    adjPanel.SetForm(sKey, sWordForm, wf.eStatus());
                }

                eRet = (EM_ReturnCode)lexeme.eGetNextWordForm(ref wf);

            } while (EM_ReturnCode.H_NO_ERROR == eRet);

        }   //  ShowLongParticipialForms (...)

/*
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
*/
        protected void ShowParseOutput()
        {
            int iWordform = 0, iPreviousID = -1;
            tabControl.TabPages.Clear();

            if (null == m_Parser)
            {
                MessageBox.Show("Internal error: Parser object is null.", "Zal error", MessageBoxButtons.OK);
                return;
            }

            int iWordForm = 0;
            CWordFormManaged wordform = null;
            EM_ReturnCode eRet = (EM_ReturnCode)m_Parser.eGetFirstWordForm(ref wordform);
            if (CErrorCode.bError(eRet))
            {
                MessageBox.Show("Error");
                return;
            }
            if(EM_ReturnCode.H_NO_ERROR != eRet)
            {
                MessageBox.Show("Form not found");
                return;
            }

            do
            {
                m_listWordForms.Add(wordform);
                AnalysisPanel ap = new AnalysisPanel(iWordform);
//                ap.Location = new System.Drawing.Point(0, iWordform * ap.Size.Height + 4);
                string sWordForm = wordform.sWordForm();
                MarkStress(ref sWordForm, wordform);
                ap.sWordform = sWordForm;
                ap.sID = wordform.llLexemeId().ToString();
                ap.eoPOS = wordform.ePos();
                ap.eoAspect = wordform.eAspect();
                ap.eoGender = wordform.eGender();
                ap.eoCase = wordform.eCase();
                ap.eoNumber = wordform.eNumber();
                ap.eoAnimacy = wordform.eAnimacy();
                ap.eoPerson = wordform.ePerson();
                ap.eoReflexiveness = wordform.eReflexive();
                ap.eoSubparadigm = wordform.eSubparadigm();
                iWordform = 0;
                iPreviousID = (int)wordform.llLexemeId();
                TabPage tab_Lexeme = new TabPage(wordform.sWordForm());
//                tab_Lexeme.Text = wordform.sWordForm();
                tab_Lexeme.AutoScroll = true;
                tab_Lexeme.Controls.Add(ap);
                tabControl.TabPages.Add(tab_Lexeme);
                tab_Lexeme.Show();
                tab_Lexeme.Focus();
                ++iWordform;
                ap.Show();

                eRet = (EM_ReturnCode)m_Parser.eGetNextWordForm(ref wordform);

            } while (EM_ReturnCode.H_NO_ERROR == eRet);

        }   //  ShowParseOutput()

/*
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
                        if (v.Result != MainLib.EM_TestResult.TEST_RESULT_UNDEFINED)
                        {
                            string sLine = v.Headword;
                            sLine += new string(' ', 40 - sLine.Length);
                            sLine += (MainLib.EM_TestResult.TEST_RESULT_OK == v.Result) ?
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
*/
        public void CloseCurrentTab()
        {
            tabControl.TabPages.Remove (tabControl.SelectedTab);
        }

        public void CloseLexemeTabs (LexemeDataPanel ldp)
        {
            EM_ReturnCode eRet = m_Dictionary.Clear(m_hashLexemes[ldp]);

            if (eRet != EM_ReturnCode.H_NO_ERROR)
            {
                MessageBox.Show("Possible internal error: unable to delete lexeme.",
                                "Zal Warning",
                                 MessageBoxButtons.OK,
                                 MessageBoxIcon.Warning);
            }

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

        public void MarkStress(ref string sWordForm, CWordFormManaged wf)
        {
            int iPos = -1;
            EM_StressType eType = EM_StressType.STRESS_TYPE_UNDEFINED;
            char chrMark = ' ';

            EM_ReturnCode eRet = wf.eGetFirstStressPos(ref iPos, ref eType);
            do
            {
                if (eRet != EM_ReturnCode.H_NO_ERROR && eRet != EM_ReturnCode.H_FALSE)
                {
                    continue;
                }

                if (EM_StressType.STRESS_PRIMARY == eType)
                {
                    if (sWordForm[iPos] != 'ё')
                    {
                        chrMark = '\x301';
                        sWordForm = sWordForm.Insert(iPos+1, chrMark.ToString());
                    }
                }
                else if (EM_StressType.STRESS_SECONDARY == eType)
                {
                    chrMark = '\x300';
                }

                eRet = wf.eGetNextStressPos(ref iPos, ref eType);
            
            }   while (EM_ReturnCode.H_NO_ERROR == eRet);
        }

    }   //  public partial class TestApplet

}   //  namespace TestUI
