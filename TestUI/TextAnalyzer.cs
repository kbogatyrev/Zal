using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.IO;
using System.Windows.Forms;
using MainLib;

namespace TestUI
{
    class TextAnalyzer
    {
        private IAnalyzer Analyzer;
        private Dictionary<string, string> DictFrequentWordforms = new Dictionary<string, string>();

        public TextAnalyzer()
        {
            Analyzer = new ZalAnalyzer();
        }

        public TextAnalyzer(IAnalyzer _Analyzer)
        {
            Analyzer = _Analyzer;
        }

        public void LoadFrequent(string str_FilePath)
        {
          StreamReader sr_freq_list;
          try
          {
            sr_freq_list = new StreamReader(str_FilePath, Encoding.UTF8);
          }
          catch (Exception ex)
          {
            MessageBox.Show("An error occurred when opening the frequent wordforms list: " + ex.Message);
            return;
          }
          string str_line = "";
          while (!sr_freq_list.EndOfStream)
          {
            str_line = sr_freq_list.ReadLine();
            if (str_line.Length <= 5)
            {
              continue;
            }
            string[] arr_str_match = Regex.Split(str_line, "^(.*?):\\s*(.*)$");
            DictFrequentWordforms[arr_str_match[1]] = arr_str_match[2];
          }
          sr_freq_list.Close();
        }

        public int i_AnalyzeText(string str_FilePath)
        {
            if (str_FilePath.Length <= 0)
            {
                return -2;
            }
            int i_wf = 0;
            string str_line = "";
            string str_NewFilePath = Regex.Replace(str_FilePath, "\\.[^\\.]*$", "-processed.xhtml");
            StreamReader sr_in;
            StreamWriter sw_out;
            try
            {
                sr_in = new StreamReader(str_FilePath, Encoding.Default);
                sw_out = new StreamWriter(str_NewFilePath, false, Encoding.UTF8);
            }
            catch (Exception ex)
            {
                MessageBox.Show("An error occurred when opening the source file: " + ex.Message);
                return -1;
            }

            bool b_HasParagraphs = false;
            sw_out.WriteLine("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<html>\n<body>");
            while (!sr_in.EndOfStream)
            {
                Application.DoEvents(); // to be replaced by introducing a separater thread
                str_line = sr_in.ReadLine();
                if (Regex.IsMatch(str_line, "^\\s*(<[^>]*>)*\\s*$"))    // e.g. <meta ...></meta> or an empty line
                {
                    sw_out.WriteLine(str_line);
                    continue;
                }
                if (b_HasParagraphs)
                {
                  sw_out.WriteLine("</p><p>");
                }
                else
                {
                  sw_out.WriteLine("<p>");
                  b_HasParagraphs = true;
                }
                MatchCollection coll_Matches = Regex.Matches(str_line, "\\s*([^\\w\\d]*)([\\w\\d]*)([^\\w\\d]*$|[\\.,\\?\\!\\)\\]\\}»]+|[:;\\-—\\\"]|[^\\w\\d\\s]*\\s)");
                foreach (Match m in coll_Matches)
                {
                    string str_left = m.Groups[1].Value;
                    string str_wf = m.Groups[2].Value;
                  string str_wf_lc = str_wf.ToLower();
                    string str_right = m.Groups[3].Value;

                    if (Regex.IsMatch(str_wf, "[1234567890]"))
                    {
                      sw_out.WriteLine(str_left + "<w><ana lex=\"" + str_wf +
                        "\" gr=\"NUM,ciph\"></ana>" + str_wf + "</w>" + str_right);
                      ++i_wf;
                      continue;
                    }
                    if (Regex.IsMatch(str_wf, "[a-zA-Z]"))
                    {
                      sw_out.WriteLine(str_left + "<w><ana lex=\"" + str_wf +
                        "\" gr=\"NONLEX\"></ana>" + str_wf + "</w>" + str_right);
                      ++i_wf;
                      continue;
                    }
                    if (str_wf.Length <= 0)
                    {
                      if (str_left.Length > 0 || str_right.Length > 0)
                      {
                        sw_out.WriteLine(str_left + str_right);
                      }
                      continue;
                    }
                    if (DictFrequentWordforms.ContainsKey(str_wf_lc))
                    {
                      sw_out.WriteLine(str_left + DictFrequentWordforms[str_wf_lc] + str_wf + "</w>" + str_right);
                      ++i_wf;
                      continue;
                    }

                    try
                    {
                        Analyzer.Analyze(str_wf_lc);
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine("Error while analyzing: " + str_wf);
                        sw_out.WriteLine(m.Groups[0].Value);
                        continue;
                    }
                    if (Analyzer.Count <= 0)
                    {
                        sw_out.WriteLine(m.Groups[0].Value);
                        continue;
                    }

                    StringBuilder strb_ana = new StringBuilder();
                    strb_ana.Append(str_left);
                    strb_ana.Append("<w>");
                    foreach (IWordForm wf in Analyzer)
                    {
                        strb_ana.Append(str_BuildAnalysisString(wf));
                    }
                    strb_ana.Append(str_wf);
                    strb_ana.Append("</w>");
                    strb_ana.Append(str_right);
                    sw_out.WriteLine(strb_ana.ToString());
                    ++i_wf;
                }
            }
            if (b_HasParagraphs)
            {
              sw_out.WriteLine("</p>");
            }
            sw_out.WriteLine("</body>\n</html>");

            sr_in.Close();
            sw_out.Close();
            return i_wf;
        }

        private string str_BuildAnalysisString(IWordForm wf)
        {
            StringBuilder strb_ana = new StringBuilder();
            strb_ana.Append("<ana lex=\"");
            strb_ana.Append(wf.Lemma);
            strb_ana.Append("\" gr=\"");
            if (wf.PartOfSpeech == ET_PartOfSpeech.POS_ADJ ||
                wf.PartOfSpeech == ET_PartOfSpeech.POS_PRONOUN_ADJ)
            {
                if (wf.PartOfSpeech == ET_PartOfSpeech.POS_ADJ)
                {
                    strb_ana.Append("A");
                }
                else if (wf.PartOfSpeech == ET_PartOfSpeech.POS_PRONOUN_ADJ)
                {
                    strb_ana.Append("APRO");
                }

                // Gender
                if (wf.Gender == ET_Gender.GENDER_F)
                {
                    strb_ana.Append(",f");
                }
                else if (wf.Gender == ET_Gender.GENDER_M)
                {
                    strb_ana.Append(",m");
                }
                else if (wf.Gender == ET_Gender.GENDER_N)
                {
                    strb_ana.Append(",n");
                }

                // Number
                if (wf.Number == ET_Number.NUM_SG)
                {
                    strb_ana.Append(",sg");
                }
                else if (wf.Number == ET_Number.NUM_PL)
                {
                    strb_ana.Append(",pl");
                }

                // Case
                if (wf.Case == ET_Case.CASE_NOM)
                {
                    strb_ana.Append(",nom");
                }
                else if (wf.Case == ET_Case.CASE_ACC)
                {
                    strb_ana.Append(",acc");
                    // Animacy
                    if (wf.Animacy == ET_Animacy.ANIM_NO)
                    {
                        strb_ana.Append(",inan");
                    }
                    else if (wf.Animacy == ET_Animacy.ANIM_YES)
                    {
                        strb_ana.Append(",anim");
                    }
                }
                else if (wf.Case == ET_Case.CASE_GEN)
                {
                    strb_ana.Append(",gen");
                }
                else if (wf.Case == ET_Case.CASE_DAT)
                {
                    strb_ana.Append(",dat");
                }
                else if (wf.Case == ET_Case.CASE_INST)
                {
                    strb_ana.Append(",ins");
                }
                else if (wf.Case == ET_Case.CASE_PREP)
                {
                    strb_ana.Append(",loc");
                }
                else if (wf.Case == ET_Case.CASE_LOC)
                {
                    strb_ana.Append(",loc2");
                }
                else if (wf.Case == ET_Case.CASE_PART)
                {
                    strb_ana.Append(",gen2");
                }
                else if (wf.Case == ET_Case.CASE_NUM)
                {
                    strb_ana.Append(",adnum");
                }

                // Unchangeable adjectives
                if (wf.Subparadigm != ET_Subparadigm.SUBPARADIGM_COMPARATIVE &&
                    wf.Gender == ET_Gender.GENDER_UNDEFINED &&
                    wf.Number == ET_Number.NUM_UNDEFINED &&
                    wf.Case == ET_Case.CASE_UNDEFINED)
                {
                    strb_ana.Append(",0");
                }

                // Long/short/comparative
                if (wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_LONG_ADJ)
                {
                    strb_ana.Append(",plen");
                }
                else if (wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_SHORT_ADJ)
                {
                    strb_ana.Append(",brev");
                }
                else if (wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_COMPARATIVE)
                {
                    strb_ana.Append(",comp");
                }
            }

            else if (wf.PartOfSpeech == ET_PartOfSpeech.POS_ADV)
            {
                strb_ana.Append("ADV");
            }

            else if (wf.PartOfSpeech == ET_PartOfSpeech.POS_COMPAR)
            {
                strb_ana.Append("COMP");
            }

            else if (wf.PartOfSpeech == ET_PartOfSpeech.POS_CONJ)
            {
                strb_ana.Append("CONJ");
            }

            else if (wf.PartOfSpeech == ET_PartOfSpeech.POS_INTERJ)
            {
                strb_ana.Append("INTERJ");
            }

            else if (wf.PartOfSpeech == ET_PartOfSpeech.POS_NOUN)
            {
                strb_ana.Append("S");

                // Animacy
                if (wf.Animacy == ET_Animacy.ANIM_NO)
                {
                    strb_ana.Append(",inan");
                }
                else if (wf.Animacy == ET_Animacy.ANIM_YES)
                {
                    strb_ana.Append(",anim");
                }

                // Gender
                if (wf.Gender == ET_Gender.GENDER_F)
                {
                    strb_ana.Append(",f");
                }
                else if (wf.Gender == ET_Gender.GENDER_M)
                {
                    strb_ana.Append(",m");
                }
                else if (wf.Gender == ET_Gender.GENDER_N)
                {
                    strb_ana.Append(",n");
                }
                // TODO: M-F, pluralia tantum

                // Number
                if (wf.Number == ET_Number.NUM_SG)
                {
                    strb_ana.Append(",sg");
                }
                else if (wf.Number == ET_Number.NUM_PL)
                {
                    strb_ana.Append(",pl");
                }

                // Case
                if (wf.Case == ET_Case.CASE_NOM)
                {
                    strb_ana.Append(",nom");
                }
                else if (wf.Case == ET_Case.CASE_ACC)
                {
                    strb_ana.Append(",acc");
                }
                else if (wf.Case == ET_Case.CASE_GEN)
                {
                    strb_ana.Append(",gen");
                }
                else if (wf.Case == ET_Case.CASE_DAT)
                {
                    strb_ana.Append(",dat");
                }
                else if (wf.Case == ET_Case.CASE_INST)
                {
                    strb_ana.Append(",ins");
                }
                else if (wf.Case == ET_Case.CASE_PREP)
                {
                    strb_ana.Append(",loc");
                }
                else if (wf.Case == ET_Case.CASE_LOC)
                {
                    strb_ana.Append(",loc2");
                }
                else if (wf.Case == ET_Case.CASE_PART)
                {
                    strb_ana.Append(",gen2");
                }
                else if (wf.Case == ET_Case.CASE_NUM)
                {
                    strb_ana.Append(",adnum");
                }

                // Unchangeable nouns
                if (wf.Number == ET_Number.NUM_UNDEFINED &&
                    wf.Case == ET_Case.CASE_UNDEFINED)
                {
                    strb_ana.Append(",0");
                }
            }

            else if (wf.PartOfSpeech == ET_PartOfSpeech.POS_NULL)
            {
                strb_ana.Append("NULL");
            }

            else if (wf.PartOfSpeech == ET_PartOfSpeech.POS_NUM ||
                     wf.PartOfSpeech == ET_PartOfSpeech.POS_NUM_ADJ)
            {
                if (wf.PartOfSpeech == ET_PartOfSpeech.POS_NUM)
                {
                    strb_ana.Append("NUM");
                }
                else if (wf.PartOfSpeech == ET_PartOfSpeech.POS_NUM_ADJ)
                {
                    strb_ana.Append("ANUM");
                }

                // Gender (POS_NUM can also have it: два vs. две)
                if (wf.Gender == ET_Gender.GENDER_F)
                {
                    strb_ana.Append(",f");
                }
                else if (wf.Gender == ET_Gender.GENDER_M)
                {
                    strb_ana.Append(",m");
                }
                else if (wf.Gender == ET_Gender.GENDER_N)
                {
                    strb_ana.Append(",n");
                }

                // Number (POS_NUM can also have it: один vs. одни)
                if (wf.Number == ET_Number.NUM_SG)
                {
                    strb_ana.Append(",sg");
                }
                else if (wf.Number == ET_Number.NUM_PL)
                {
                    strb_ana.Append(",pl");
                }

                // Case
                if (wf.Case == ET_Case.CASE_NOM)
                {
                    strb_ana.Append(",nom");
                }
                else if (wf.Case == ET_Case.CASE_ACC)
                {
                    strb_ana.Append(",acc");
                    // Animacy
                    if (wf.Animacy == ET_Animacy.ANIM_NO)
                    {
                        strb_ana.Append(",inan");
                    }
                    else if (wf.Animacy == ET_Animacy.ANIM_YES)
                    {
                        strb_ana.Append(",anim");
                    }
                }
                else if (wf.Case == ET_Case.CASE_GEN)
                {
                    strb_ana.Append(",gen");
                }
                else if (wf.Case == ET_Case.CASE_DAT)
                {
                    strb_ana.Append(",dat");
                }
                else if (wf.Case == ET_Case.CASE_INST)
                {
                    strb_ana.Append(",ins");
                }
                else if (wf.Case == ET_Case.CASE_PREP)
                {
                    strb_ana.Append(",loc");
                }
            }

            else if (wf.PartOfSpeech == ET_PartOfSpeech.POS_PARENTH)
            {
                strb_ana.Append("PARENTH");
            }

            else if (wf.PartOfSpeech == ET_PartOfSpeech.POS_PARTICLE)
            {
                strb_ana.Append("PART");
            }

            else if (wf.PartOfSpeech == ET_PartOfSpeech.POS_PREDIC)
            {
                strb_ana.Append("PRAEDIC");
            }

            else if (wf.PartOfSpeech == ET_PartOfSpeech.POS_PREP)
            {
                strb_ana.Append("PR");
            }

            else if (wf.PartOfSpeech == ET_PartOfSpeech.POS_PRONOUN)
            {
                strb_ana.Append("SPRO");

                // Person
                if (wf.Person == ET_Person.PERSON_1)
                {
                    strb_ana.Append(",1p");
                }
                else if (wf.Person == ET_Person.PERSON_2)
                {
                    strb_ana.Append(",2p");
                }
                else if (wf.Person == ET_Person.PERSON_3)
                {
                    strb_ana.Append(",3p");
                }

                // Animacy
                if (wf.Animacy == ET_Animacy.ANIM_NO)
                {
                    strb_ana.Append(",inan");
                }
                else if (wf.Animacy == ET_Animacy.ANIM_YES)
                {
                    strb_ana.Append(",anim");
                }

                // Gender
                if (wf.Gender == ET_Gender.GENDER_F)
                {
                    strb_ana.Append(",f");
                }
                else if (wf.Gender == ET_Gender.GENDER_M)
                {
                    strb_ana.Append(",m");
                }
                else if (wf.Gender == ET_Gender.GENDER_N)
                {
                    strb_ana.Append(",n");
                }
                // TODO: M-F, pluralia tantum

                // Number
                if (wf.Number == ET_Number.NUM_SG)
                {
                    strb_ana.Append(",sg");
                }
                else if (wf.Number == ET_Number.NUM_PL)
                {
                    strb_ana.Append(",pl");
                }

                // Case
                if (wf.Case == ET_Case.CASE_NOM)
                {
                    strb_ana.Append(",nom");
                }
                else if (wf.Case == ET_Case.CASE_ACC)
                {
                    strb_ana.Append(",acc");
                }
                else if (wf.Case == ET_Case.CASE_GEN)
                {
                    strb_ana.Append(",gen");
                }
                else if (wf.Case == ET_Case.CASE_DAT)
                {
                    strb_ana.Append(",dat");
                }
                else if (wf.Case == ET_Case.CASE_INST)
                {
                    strb_ana.Append(",ins");
                }
                else if (wf.Case == ET_Case.CASE_PREP)
                {
                    strb_ana.Append(",loc");
                }
            }

            else if (wf.PartOfSpeech == ET_PartOfSpeech.POS_PRONOUN_PREDIC)
            {
                strb_ana.Append("PRAEDICPRO");
            }

            else if (wf.PartOfSpeech == ET_PartOfSpeech.POS_VERB)
            {
                strb_ana.Append("V");

                // TODO: transitivity

                // Aspect
                if (wf.Aspect == ET_Aspect.ASPECT_IMPERFECTIVE)
                {
                    strb_ana.Append(",ipfv");
                }
                else if (wf.Aspect == ET_Aspect.ASPECT_PERFECTIVE)
                {
                    strb_ana.Append(",pfv");
                }

                // Subparadigm
                if (wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_PRESENT_TENSE)
                {
                    strb_ana.Append(",indic");

                    // Reflexivity
                    if (wf.IsReflexive == ET_Reflexive.REFL_NO)
                    {
                        strb_ana.Append(",act");
                    }
                    else if (wf.IsReflexive == ET_Reflexive.REFL_YES)
                    {
                        strb_ana.Append(",med");
                    }

                    // Tense
                    if (wf.Aspect == ET_Aspect.ASPECT_IMPERFECTIVE)
                    {
                        strb_ana.Append(",praes");
                    }
                    else if (wf.Aspect == ET_Aspect.ASPECT_PERFECTIVE)
                    {
                        strb_ana.Append(",fut");
                    }
                    else if (wf.Aspect == ET_Aspect.ASPECT_UNDEFINED)
                    {
                        strb_ana.Append(",nonpast");
                    }

                    // Person
                    if (wf.Person == ET_Person.PERSON_1)
                    {
                        strb_ana.Append(",1p");
                    }
                    else if (wf.Person == ET_Person.PERSON_2)
                    {
                        strb_ana.Append(",2p");
                    }
                    else if (wf.Person == ET_Person.PERSON_3)
                    {
                        strb_ana.Append(",3p");
                    }

                    // Number
                    if (wf.Number == ET_Number.NUM_SG)
                    {
                        strb_ana.Append(",sg");
                    }
                    else if (wf.Number == ET_Number.NUM_PL)
                    {
                        strb_ana.Append(",pl");
                    }
                }
                else if (wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_PAST_TENSE)
                {
                    strb_ana.Append(",indic");

                    // Reflexivity
                    if (wf.IsReflexive == ET_Reflexive.REFL_NO)
                    {
                        strb_ana.Append(",act");
                    }
                    else if (wf.IsReflexive == ET_Reflexive.REFL_YES)
                    {
                        strb_ana.Append(",med");
                    }

                    // Tense
                    strb_ana.Append(",praet");

                    // Gender
                    if (wf.Gender == ET_Gender.GENDER_F)
                    {
                        strb_ana.Append(",f");
                    }
                    else if (wf.Gender == ET_Gender.GENDER_M)
                    {
                        strb_ana.Append(",m");
                    }
                    else if (wf.Gender == ET_Gender.GENDER_N)
                    {
                        strb_ana.Append(",n");
                    }

                    // Number
                    if (wf.Number == ET_Number.NUM_SG)
                    {
                        strb_ana.Append(",sg");
                    }
                    else if (wf.Number == ET_Number.NUM_PL)
                    {
                        strb_ana.Append(",pl");
                    }
                }
                else if (wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_PART_PAST_ACT ||
                         wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_PART_PRES_ACT)
                {
                    strb_ana.Append(",partcp");

                    // Reflexivity
                    if (wf.IsReflexive == ET_Reflexive.REFL_NO)
                    {
                        strb_ana.Append(",act");
                    }
                    else if (wf.IsReflexive == ET_Reflexive.REFL_YES)
                    {
                        strb_ana.Append(",med");
                    }

                    // Tense
                    if (wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_PART_PAST_ACT)
                    {
                        strb_ana.Append(",praet");
                    }
                    else if (wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_PART_PRES_ACT)
                    {
                        if (wf.Aspect == ET_Aspect.ASPECT_IMPERFECTIVE)
                        {
                            strb_ana.Append(",praes");
                        }
                        else if (wf.Aspect == ET_Aspect.ASPECT_PERFECTIVE)
                        {
                            strb_ana.Append(",fut");
                        }
                        else if (wf.Aspect == ET_Aspect.ASPECT_UNDEFINED)
                        {
                            strb_ana.Append(",nonpast");
                        }
                    }

                    // Gender
                    if (wf.Gender == ET_Gender.GENDER_F)
                    {
                        strb_ana.Append(",f");
                    }
                    else if (wf.Gender == ET_Gender.GENDER_M)
                    {
                        strb_ana.Append(",m");
                    }
                    else if (wf.Gender == ET_Gender.GENDER_N)
                    {
                        strb_ana.Append(",n");
                    }

                    // Number
                    if (wf.Number == ET_Number.NUM_SG)
                    {
                        strb_ana.Append(",sg");
                    }
                    else if (wf.Number == ET_Number.NUM_PL)
                    {
                        strb_ana.Append(",pl");
                    }

                    // Case
                    if (wf.Case == ET_Case.CASE_NOM)
                    {
                        strb_ana.Append(",nom");
                    }
                    else if (wf.Case == ET_Case.CASE_ACC)
                    {
                        strb_ana.Append(",acc");
                        // Animacy
                        if (wf.Animacy == ET_Animacy.ANIM_NO)
                        {
                            strb_ana.Append(",inan");
                        }
                        else if (wf.Animacy == ET_Animacy.ANIM_YES)
                        {
                            strb_ana.Append(",anim");
                        }
                    }
                    else if (wf.Case == ET_Case.CASE_GEN)
                    {
                        strb_ana.Append(",gen");
                    }
                    else if (wf.Case == ET_Case.CASE_DAT)
                    {
                        strb_ana.Append(",dat");
                    }
                    else if (wf.Case == ET_Case.CASE_INST)
                    {
                        strb_ana.Append(",ins");
                    }
                    else if (wf.Case == ET_Case.CASE_PREP)
                    {
                        strb_ana.Append(",loc");
                    }

                    strb_ana.Append(",plen");
                }

                else if (wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG ||
                         wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG)
                {
                    strb_ana.Append(",partcp");

                    // Reflexivity
                    if (wf.IsReflexive == ET_Reflexive.REFL_NO)
                    {
                        strb_ana.Append(",pass");
                    }
                    else if (wf.IsReflexive == ET_Reflexive.REFL_YES)
                    {
                        strb_ana.Append(",pass,med");
                    }

                    // Tense
                    if (wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG)
                    {
                        strb_ana.Append(",praet");
                    }
                    else if (wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG)
                    {
                        if (wf.Aspect == ET_Aspect.ASPECT_IMPERFECTIVE)
                        {
                            strb_ana.Append(",praes");
                        }
                        else if (wf.Aspect == ET_Aspect.ASPECT_PERFECTIVE)
                        {
                            strb_ana.Append(",fut");
                        }
                        else if (wf.Aspect == ET_Aspect.ASPECT_UNDEFINED)
                        {
                            strb_ana.Append(",nonpast");
                        }
                    }

                    // Gender
                    if (wf.Gender == ET_Gender.GENDER_F)
                    {
                        strb_ana.Append(",f");
                    }
                    else if (wf.Gender == ET_Gender.GENDER_M)
                    {
                        strb_ana.Append(",m");
                    }
                    else if (wf.Gender == ET_Gender.GENDER_N)
                    {
                        strb_ana.Append(",n");
                    }

                    // Number
                    if (wf.Number == ET_Number.NUM_SG)
                    {
                        strb_ana.Append(",sg");
                    }
                    else if (wf.Number == ET_Number.NUM_PL)
                    {
                        strb_ana.Append(",pl");
                    }

                    // Case
                    if (wf.Case == ET_Case.CASE_NOM)
                    {
                        strb_ana.Append(",nom");
                    }
                    else if (wf.Case == ET_Case.CASE_ACC)
                    {
                        strb_ana.Append(",acc");
                        // Animacy
                        if (wf.Animacy == ET_Animacy.ANIM_NO)
                        {
                            strb_ana.Append(",inan");
                        }
                        else if (wf.Animacy == ET_Animacy.ANIM_YES)
                        {
                            strb_ana.Append(",anim");
                        }
                    }
                    else if (wf.Case == ET_Case.CASE_GEN)
                    {
                        strb_ana.Append(",gen");
                    }
                    else if (wf.Case == ET_Case.CASE_DAT)
                    {
                        strb_ana.Append(",dat");
                    }
                    else if (wf.Case == ET_Case.CASE_INST)
                    {
                        strb_ana.Append(",ins");
                    }
                    else if (wf.Case == ET_Case.CASE_PREP)
                    {
                        strb_ana.Append(",loc");
                    }

                    strb_ana.Append(",plen");
                }

                else if (wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_PART_PAST_PASS_SHORT ||
                         wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_PART_PRES_PASS_SHORT)
                {
                    strb_ana.Append(",partcp");

                    // Reflexivity
                    if (wf.IsReflexive == ET_Reflexive.REFL_NO)
                    {
                        strb_ana.Append(",pass");
                    }
                    else if (wf.IsReflexive == ET_Reflexive.REFL_YES)
                    {
                        strb_ana.Append(",pass,med");
                    }

                    // Tense
                    if (wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_PART_PAST_ACT)
                    {
                        strb_ana.Append(",praet");
                    }
                    else if (wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_PART_PRES_ACT)
                    {
                        if (wf.Aspect == ET_Aspect.ASPECT_IMPERFECTIVE)
                        {
                            strb_ana.Append(",praes");
                        }
                        else if (wf.Aspect == ET_Aspect.ASPECT_PERFECTIVE)
                        {
                            strb_ana.Append(",fut");
                        }
                        else if (wf.Aspect == ET_Aspect.ASPECT_UNDEFINED)
                        {
                            strb_ana.Append(",nonpast");
                        }
                    }

                    // Gender
                    if (wf.Gender == ET_Gender.GENDER_F)
                    {
                        strb_ana.Append(",f");
                    }
                    else if (wf.Gender == ET_Gender.GENDER_M)
                    {
                        strb_ana.Append(",m");
                    }
                    else if (wf.Gender == ET_Gender.GENDER_N)
                    {
                        strb_ana.Append(",n");
                    }

                    // Number
                    if (wf.Number == ET_Number.NUM_SG)
                    {
                        strb_ana.Append(",sg");
                    }
                    else if (wf.Number == ET_Number.NUM_PL)
                    {
                        strb_ana.Append(",pl");
                    }

                    strb_ana.Append(",brev");
                }

                else if (wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_ADVERBIAL_PAST ||
                         wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_ADVERBIAL_PRESENT)
                {
                    strb_ana.Append(",ger");

                    // Reflexivity
                    if (wf.IsReflexive == ET_Reflexive.REFL_NO)
                    {
                        strb_ana.Append(",act");
                    }
                    else if (wf.IsReflexive == ET_Reflexive.REFL_YES)
                    {
                        strb_ana.Append(",med");
                    }

                    // Tense
                    if (wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_PART_PAST_ACT)
                    {
                        strb_ana.Append(",praet");
                    }
                    else if (wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_PART_PRES_ACT)
                    {
                        if (wf.Aspect == ET_Aspect.ASPECT_IMPERFECTIVE)
                        {
                            strb_ana.Append(",praes");
                        }
                        else if (wf.Aspect == ET_Aspect.ASPECT_PERFECTIVE)
                        {
                            strb_ana.Append(",fut");
                        }
                        else if (wf.Aspect == ET_Aspect.ASPECT_UNDEFINED)
                        {
                            strb_ana.Append(",nonpast");
                        }
                    }
                }

                else if (wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_IMPERATIVE)
                {
                    strb_ana.Append(",imper");

                    // Reflexivity
                    if (wf.IsReflexive == ET_Reflexive.REFL_NO)
                    {
                        strb_ana.Append(",act");
                    }
                    else if (wf.IsReflexive == ET_Reflexive.REFL_YES)
                    {
                        strb_ana.Append(",med");
                    }

                    // Person
                    if (wf.Person == ET_Person.PERSON_1)
                    {
                        strb_ana.Append(",1p");
                    }
                    else if (wf.Person == ET_Person.PERSON_2)
                    {
                        strb_ana.Append(",2p");
                    }

                    // Number
                    if (wf.Number == ET_Number.NUM_SG)
                    {
                        strb_ana.Append(",sg");
                    }
                    else if (wf.Number == ET_Number.NUM_PL)
                    {
                        strb_ana.Append(",pl");
                    }
                }

                else if (wf.Subparadigm == ET_Subparadigm.SUBPARADIGM_INFINITIVE)
                {
                    strb_ana.Append(",inf");
                }
            }
            if (wf.LexemeId <= 0)
            {
                strb_ana.Append(",bastard");
            }

            strb_ana.Append("\"></ana>");
            return strb_ana.ToString();
        }
    }
}
