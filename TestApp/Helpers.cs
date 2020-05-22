﻿using MainLibManaged;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows;

namespace ZalTestApp
{
    public static class Helpers
    {

        #region GramHashes

        public static List<string> m_listPropNamesNoun = new List<string>()
        {
            "Noun_Sg_N", "Noun_Sg_A", "Noun_Sg_G", "Noun_Sg_P", "Noun_Sg_D", "Noun_Sg_P2",
            "Noun_Sg_I", "Noun_Sg_Part", "Noun_Pl_N", "Noun_Pl_A", "Noun_Pl_G", "Noun_Pl_P",
            "Noun_Pl_D", "Noun_Pl_L", "Noun_Pl_I"
        };

        public static List<string> m_listPropNamesPronoun = new List<string>()
        {
            "Pronoun_Sg_N", "Pronoun_Sg_A", "Pronoun_Sg_G", "Pronoun_Sg_P", "Pronoun_Sg_D", "Pronoun_Sg_P2",
            "Pronoun_Sg_I", "Pronoun_Sg_Part", "Pronoun_Pl_N", "Pronoun_Pl_A", "Pronoun_Pl_G", "Pronoun_Pl_P",
            "Pronoun_Pl_D", "Pronoun_Pl_L", "Pronoun_Pl_I"
        };

        public static List<string> m_listPropNamesNumeral = new List<string>()
        {
            "Numeral_Sg_N", "Numeral_Sg_A", "Numeral_Sg_G", "Numeral_Sg_P", "Numeral_Sg_D", "Numeral_Sg_P2",
            "Numeral_Sg_I", "Numeral_Sg_Part", "Numeral_Pl_N", "Numeral_Pl_A", "Numeral_Pl_G", "Numeral_Pl_P",
            "Numeral_Pl_D", "Numeral_Pl_L", "Numeral_Pl_I"
        };

        public static List<string> m_listPropNamesAdj = new List<string>()
        {
            "AdjL_M_Sg_N", "AdjL_M_Sg_A_Inanim", "AdjL_M_Sg_A_Anim", "AdjL_M_Sg_G", "AdjL_M_Sg_P", "AdjL_M_Sg_D",
            "AdjL_M_Sg_I", "AdjL_F_Sg_N", "AdjL_F_Sg_A", "AdjL_F_Sg_G", "AdjL_F_Sg_P", "AdjL_F_Sg_D", "AdjL_F_Sg_I",
            "AdjL_N_Sg_N", "AdjL_N_Sg_A", "AdjL_N_Sg_G", "AdjL_N_Sg_P", "AdjL_N_Sg_D", "AdjL_N_Sg_I", "AdjL_Pl_N",
            "AdjL_Pl_A_Inanim", "AdjL_Pl_A_Anim", "AdjL_Pl_G", "AdjL_Pl_P", "AdjL_Pl_D", "AdjL_Pl_I",
            "AdjS_M", "AdjS_F", "AdjS_N", "AdjS_Pl", "AdjComp"
        };

        public static List<string> m_listPropNamesPronAdj = new List<string>()
        {
            "PronAdj_M_Sg_N", "PronAdj_M_Sg_A_Inanim", "PronAdj_M_Sg_A_Anim", "PronAdj_M_Sg_G", "PronAdj_M_Sg_P", "PronAdj_M_Sg_D",
            "PronAdj_M_Sg_I", "PronAdj_F_Sg_N", "PronAdj_F_Sg_A", "PronAdj_F_Sg_G", "PronAdj_F_Sg_P", "PronAdj_F_Sg_D", "PronAdj_F_Sg_I",
            "PronAdj_N_Sg_N", "PronAdj_N_Sg_A", "PronAdj_N_Sg_G", "PronAdj_N_Sg_P", "PronAdj_N_Sg_D", "PronAdj_N_Sg_I", "PronAdj_Pl_N",
            "PronAdj_Pl_A_Inanim", "PronAdj_Pl_A_Anim",  "PronAdj_Pl_G", "PronAdj_Pl_P", "PronAdj_Pl_D", "PronAdj_Pl_I"
        };

        public static List<string> m_listPropNamesNumAdj = new List<string>()
        {
            "NumAdj_M_Sg_N", "NumAdj_M_Sg_A_Inanim", "NumAdj_M_Sg_A_Anim", "NumAdj_M_Sg_G", "NumAdj_M_Sg_P", "NumAdj_M_Sg_D",
            "NumAdj_M_Sg_I", "NumAdj_F_Sg_N", "NumAdj_F_Sg_A", "NumAdj_F_Sg_G", "NumAdj_F_Sg_P", "NumAdj_F_Sg_D", "NumAdj_F_Sg_I",
            "NumAdj_N_Sg_N", "NumAdj_N_Sg_A", "NumAdj_N_Sg_G", "NumAdj_N_Sg_P", "NumAdj_N_Sg_D", "NumAdj_N_Sg_I", "NumAdj_Pl_N",
            "NumAdj_Pl_A_Inanim", "NumAdj_Pl_A_Anim",  "NumAdj_Pl_G", "NumAdj_Pl_P", "NumAdj_Pl_D", "NumAdj_Pl_I"
        };

        public static List<string> m_listPropNamesPartPresAct = new List<string>()
        {
            "PPresA_M_Sg_N", "PPresA_M_Sg_A_Inanim", "PPresA_M_Sg_A_Anim", "PPresA_M_Sg_G", "PPresA_M_Sg_P", "PPresA_M_Sg_D",
            "PPresA_M_Sg_I", "PPresA_F_Sg_N", "PPresA_F_Sg_A", "PPresA_F_Sg_G", "PPresA_F_Sg_P", "PPresA_F_Sg_D", "PPresA_F_Sg_I",
            "PPresA_N_Sg_N", "PPresA_N_Sg_A", "PPresA_N_Sg_G", "PPresA_N_Sg_P", "PPresA_N_Sg_D", "PPresA_N_Sg_I", "PPresA_Pl_N",
            "PPresA_Pl_A_Inanim", "PPresA_Pl_A_Anim", "PPresA_Pl_G", "PPresA_Pl_P", "PPresA_Pl_D", "PPresA_Pl_I"
        };

        public static List<string> m_listPropNamesPartPastAct = new List<string>()
        {
            "PPastA_M_Sg_N", "PPastA_M_Sg_A_Inanim", "PPastA_M_Sg_A_Anim", "PPastA_M_Sg_G", "PPastA_M_Sg_P", "PPastA_M_Sg_D",
            "PPastA_M_Sg_I", "PPastA_F_Sg_N", "PPastA_F_Sg_A", "PPastA_F_Sg_G", "PPastA_F_Sg_P", "PPastA_F_Sg_D", "PPastA_F_Sg_I",
            "PPastA_N_Sg_N", "PPastA_N_Sg_A", "PPastA_N_Sg_G", "PPastA_N_Sg_P", "PPastA_N_Sg_D", "PPastA_N_Sg_I", "PPastA_Pl_N",
            "PPastA_Pl_A_Anim", "PPastA_Pl_A_Inanim", "PPastA_Pl_G", "PPastA_Pl_P", "PPastA_Pl_D", "PPastA_Pl_I"
        };

        public static List<string> m_listPropNamesPartPresPass = new List<string>()
        {
            "PPresPL_M_Sg_N", "PPresPL_M_Sg_A_Anim", "PPresPL_M_Sg_A_Inanim", "PPresPL_M_Sg_G", "PPresPL_M_Sg_P",
            "PPresPL_M_Sg_D", "PPresPL_M_Sg_I", "PPresPL_F_Sg_N", "PPresPL_F_Sg_A", "PPresPL_F_Sg_G", "PPresPL_F_Sg_P",
            "PPresPL_F_Sg_D", "PPresPL_F_Sg_I", "PPresPL_N_Sg_N", "PPresPL_N_Sg_A", "PPresPL_N_Sg_G", "PPresPL_N_Sg_P",
            "PPresPL_N_Sg_D", "PPresPL_N_Sg_I", "PPresPL_Pl_N", "PPresPL_Pl_A_Anim", "PPresPL_Pl_A_Inanim", "PPresPL_Pl_G",
            "PPresPL_Pl_P", "PPresPL_Pl_D", "PPresPL_Pl_I", "PPresPS_M", "PPresPS_F", "PPresPS_N", "PPresPS_Pl"
        };

        public static List<string> m_listPropNamesPartPastPass = new List<string>()
        {
            "PPastPL_M_Sg_N", "PPastPL_M_Sg_A_Anim", "PPastPL_M_Sg_A_Inanim", "PPastPL_M_Sg_G", "PPastPL_M_Sg_P",
            "PPastPL_M_Sg_D", "PPastPL_M_Sg_I", "PPastPL_F_Sg_N", "PPastPL_F_Sg_A", "PPastPL_F_Sg_G", "PPastPL_F_Sg_P",
            "PPastPL_F_Sg_D", "PPastPL_F_Sg_I", "PPastPL_N_Sg_N", "PPastPL_N_Sg_A", "PPastPL_N_Sg_G", "PPastPL_N_Sg_P",
            "PPastPL_N_Sg_D", "PPastPL_N_Sg_I", "PPastPL_Pl_N", "PPastPL_Pl_A_Anim", "PPastPL_Pl_A_Inanim", "PPastPL_Pl_G",
            "PPastPL_Pl_P", "PPastPL_Pl_D", "PPastPL_Pl_I", "PPastPS_M", "PPastPS_F", "PPastPS_N", "PPastPS_Pl"
        };

        public static List<string> m_listPropNamesVerbMainScreen = new List<string>()
        {
            "Infinitive", "Pres_Sg_1", "Pres_Sg_2", "Pres_Sg_3", "Pres_Pl_1", "Pres_Pl_2", "Pres_Pl_3", 
            "Past_M", "Past_F", "Past_N", "Past_Pl", "Impv_Sg_2", "Impv_Pl_2", "PPresA_M_Sg_N", "VAdvPres",
            "PPastA_M_Sg_N", "VAdvPast", "PPresPL_M_Sg_N", "PPastPL_M_Sg_N"
        };
        #endregion

        public static EM_MainSymbol eMainSymbolToEnum(string sMainSymbol)
        {
            switch (sMainSymbol)
            {
                case "м":
                    return EM_MainSymbol.MS_M;
                case "мо":
                    return EM_MainSymbol.MS_MO;
                case "ж":
                    return EM_MainSymbol.MS_ZH;
                case "жо":
                    return EM_MainSymbol.MS_ZHO;
                case "с":
                    return EM_MainSymbol.MS_S;
                case "со":
                    return EM_MainSymbol.MS_SO;
                case "мо-жо":
                    return EM_MainSymbol.MS_MO_ZHO;
                case "мн.":
                    return EM_MainSymbol.MS_MN;
                case "мн.неод.":
                    return EM_MainSymbol.MS_MN_NEOD;
                case "мн.одуш.":
                    return EM_MainSymbol.MS_MN_ODUSH;
                case "мн. от":
                    return EM_MainSymbol.MS_MN_OT;
                case "п":
                    return EM_MainSymbol.MS_P;
                case "мс":
                    return EM_MainSymbol.MS_MS;
                case "мс-п":
                    return EM_MainSymbol.MS_MS_P;
                case "числ.":
                    return EM_MainSymbol.MS_CHISL;
                case "числ.-п":
                    return EM_MainSymbol.MS_CHISL_P;
                case "св":
                    return EM_MainSymbol.MS_SV;
                case "нсв":
                    return EM_MainSymbol.MS_NSV;
                case "св-нсв":
                    return EM_MainSymbol.MS_SV_NSV;
                case "н":
                    return EM_MainSymbol.MS_N;
                case "предл.":
                    return EM_MainSymbol.MS_PREDL;
                case "союз":
                    return EM_MainSymbol.MS_SOJUZ;
                case "предик.":
                    return EM_MainSymbol.MS_PREDIK;
                case "вводн.":
                    return EM_MainSymbol.MS_VVODN;
                case "сравн.":
                    return EM_MainSymbol.MS_SRAVN;
                case "част.":
                    return EM_MainSymbol.MS_CHAST;
                case "межд.":
                    return EM_MainSymbol.MS_MEZHD;
                default:
                    return EM_MainSymbol.MS_UNDEFINED;
            }
        }

        public static string sAccenTypeToStressSchema(EM_AccentType eType)
        {
            switch (eType)
            {
                case EM_AccentType.AT_A:
                    return "a";
                case EM_AccentType.AT_A1:
                    return "a'";
                case EM_AccentType.AT_B:
                    return "b";
                case EM_AccentType.AT_B1:
                    return "b'";
                case EM_AccentType.AT_C:
                    return "c";
                case EM_AccentType.AT_C1:
                    return "c'";
                case EM_AccentType.AT_C2:
                    return "c''";
                case EM_AccentType.AT_D:
                    return "d";
                case EM_AccentType.AT_D1:
                    return "d'";
                case EM_AccentType.AT_E:
                    return "e";
                case EM_AccentType.AT_F:
                    return "f";
                case EM_AccentType.AT_F1:
                    return "f'";
                case EM_AccentType.AT_F2:
                    return "f''";
                case EM_AccentType.AT_UNDEFINED:
                default:
                    return "Undefined";
            };
        }       //  sAccenTypeToStressSchema()

        public static string sCaseToString(EM_Case eCase)
        {
            switch (eCase)
            {
                case EM_Case.CASE_NOM:
                    return "N";
                case EM_Case.CASE_ACC:
                    return "A";
                case EM_Case.CASE_GEN:
                    return "G";
                case EM_Case.CASE_LOC:
                    return "P2";
                case EM_Case.CASE_DAT:
                    return "D";
                case EM_Case.CASE_INST:
                    return "I";
                case EM_Case.CASE_PREP:
                    return "P";
                case EM_Case.CASE_PART:
                    return "Part";
                case EM_Case.CASE_NUM:
                    return "Num";
                case EM_Case.CASE_UNDEFINED:
                default:
                    return "Undefined";
            }
        }      //  CaseToString()

        public static EM_Case eStringToCase(string sCase)
        {
            switch (sCase)
            {
                case "N":
                    return EM_Case.CASE_NOM;
                case "A":
                    return EM_Case.CASE_ACC;
                case "G":
                    return EM_Case.CASE_GEN;
                case "P2":
                    return EM_Case.CASE_LOC;
                case "D":
                    return EM_Case.CASE_DAT;
                case "I":
                    return EM_Case.CASE_INST;
                case "P":
                    return EM_Case.CASE_PREP;
                case "Part":
                    return EM_Case.CASE_PART;
                case "Num":
                    return EM_Case.CASE_NUM;
                default:
                    return EM_Case.CASE_UNDEFINED;
            }
        }

        public static string sNumberToString(EM_Number eNumber)
        {
            switch (eNumber)
            {
                case EM_Number.NUM_SG:
                    return "Sg";
                case EM_Number.NUM_PL:
                    return "Pl";
                case EM_Number.NUM_UNDEFINED:
                default:
                    return "Undefined";
            }
        }

        public static EM_Number eStringToNumber(string sNumber)
        {
            switch (sNumber)
            {
                case "Sg":
                    return EM_Number.NUM_SG;
                case "Pl":
                    return EM_Number.NUM_PL;
                default:
                    return EM_Number.NUM_UNDEFINED;
            }
        }

        public static string sGenderToString(EM_Gender eGender)
        {
            switch (eGender)
            {
                case EM_Gender.GENDER_M:
                    return "M";
                case EM_Gender.GENDER_F:
                    return "F";
                case EM_Gender.GENDER_N:
                    return "N";
                case EM_Gender.GENDER_UNDEFINED:
                default:
                    return "Undefined";
            }
        }

        public static EM_Gender eStringToGender(string sGender)
        {
            switch (sGender)
            {
                case "M":
                    return EM_Gender.GENDER_M;
                case "F":
                    return EM_Gender.GENDER_F;
                case "N":
                    return EM_Gender.GENDER_N;
                default:
                    return EM_Gender.GENDER_UNDEFINED;
            }
        }

        public static string sPersonToString(EM_Person ePerson)
        {
            switch (ePerson)
            {
                case EM_Person.PERSON_1:
                    return "1";
                case EM_Person.PERSON_2:
                    return "2";
                case EM_Person.PERSON_3:
                    return "3";
                case EM_Person.PERSON_UNDEFINED:
                default:
                    return "Undefined";
            }
        }

        public static EM_Person eStringToPerson(string sPerson)
        {
            switch (sPerson)
            {
                case "1":
                    return EM_Person.PERSON_1;
                case "2":
                    return EM_Person.PERSON_2;
                case "3":
                    return EM_Person.PERSON_3;
                default:
                    return EM_Person.PERSON_UNDEFINED;
            }
        }

        public static EM_ReturnCode eGramHashToSubparadigm(string sHash, ref EM_PartOfSpeech ePartOfSpeech, ref EM_Subparadigm eSubParadigm)
        {
            ePartOfSpeech = EM_PartOfSpeech.POS_UNDEFINED;
            eSubParadigm = EM_Subparadigm.SUBPARADIGM_UNDEFINED;

            string[] arrTokens = sHash.Split('_');
            if (arrTokens.Length < 1)
            {
                return EM_ReturnCode.H_ERROR_UNEXPECTED;
            }

            string sSubparadigm = arrTokens[0];
            switch (sSubparadigm)
            {
                case "Noun":
                    ePartOfSpeech = EM_PartOfSpeech.POS_NOUN;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_NOUN;
                    break;

                case "Pronoun":
                    ePartOfSpeech = EM_PartOfSpeech.POS_PRONOUN;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_PRONOUN;
                    break;

                case "AdjL":
                    ePartOfSpeech = EM_PartOfSpeech.POS_ADJ;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_LONG_ADJ;
                    break;

                case "AdjComp":
                    ePartOfSpeech = EM_PartOfSpeech.POS_ADJ;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_COMPARATIVE;
                    break;

                case "AdjS":
                    ePartOfSpeech = EM_PartOfSpeech.POS_ADJ;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_SHORT_ADJ;
                    break;

                case "PronAdj":
                    ePartOfSpeech = EM_PartOfSpeech.POS_PRONOUN_ADJ;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_PRONOUN_ADJ;
                    break;

                case "PPresA":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_PART_PRES_ACT;
                    break;

                case "PPastA":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_PART_PAST_ACT;
                    break;

                case "PPresPL":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG;
                    break;

                case "PPastPL":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG;
                    break;

                case "Infinitive":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_INFINITIVE;
                    break;

                case "Pres":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_PRESENT_TENSE;
                    break;

                case "Past":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_PAST_TENSE;
                    break;

                case "Impv":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_IMPERATIVE;
                    break;

                case "VAdvPres":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_ADVERBIAL_PRESENT;
                    break;

                case "VAdvPast":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_ADVERBIAL_PAST;
                    break;

                default:
                    return EM_ReturnCode.H_ERROR_UNEXPECTED;

            }       //  switch (sSubparadigm)

            return EM_ReturnCode.H_NO_ERROR;

        }       //  eGramHashToSubparadigm()

        public static EM_ReturnCode eGramHashToGender(string sHash, ref EM_Gender eGender)
        {
            EM_PartOfSpeech ePartOfSpeech = EM_PartOfSpeech.POS_UNDEFINED;
            EM_Subparadigm eSubParadigm = EM_Subparadigm.SUBPARADIGM_UNDEFINED;
            eGender = EM_Gender.GENDER_UNDEFINED;

            EM_ReturnCode eRet = Helpers.eGramHashToSubparadigm(sHash, ref ePartOfSpeech, ref eSubParadigm);

            string sGender = "";

            string[] arrTokens = sHash.Split('_');

            if (EM_Subparadigm.SUBPARADIGM_LONG_ADJ == eSubParadigm &&
                EM_Subparadigm.SUBPARADIGM_PRONOUN_ADJ == eSubParadigm &&
                EM_Subparadigm.SUBPARADIGM_PART_PRES_ACT == eSubParadigm &&
                EM_Subparadigm.SUBPARADIGM_PART_PAST_ACT == eSubParadigm &&
                EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG == eSubParadigm &&
                EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG == eSubParadigm)
            {
                if (arrTokens.Length < 4)
                {
                    return EM_ReturnCode.H_ERROR_UNEXPECTED;
                }

                sGender = arrTokens[1];
            }

            if (EM_Subparadigm.SUBPARADIGM_SHORT_ADJ == eSubParadigm)
            {
                if (arrTokens.Length < 2)
                {
                    return EM_ReturnCode.H_ERROR_UNEXPECTED;
                }
                sGender = arrTokens[1];
            }

            switch (sGender)
            {
                case "M":
                    eGender = EM_Gender.GENDER_M;
                    break;

                case "N":
                    eGender = EM_Gender.GENDER_N;
                    break;

                case "F":
                    eGender = EM_Gender.GENDER_F;
                    break;

                default:
                    return EM_ReturnCode.H_ERROR_UNEXPECTED;
            }

            return EM_ReturnCode.H_NO_ERROR;

        }       //  eGramHashToGender()

        public static EM_ReturnCode eGramHashToNumber(string sHash, ref EM_Number eNumber)
        {
            EM_PartOfSpeech ePartOfSpeech = EM_PartOfSpeech.POS_UNDEFINED;
            EM_Subparadigm eSubParadigm = EM_Subparadigm.SUBPARADIGM_UNDEFINED;
            eNumber = EM_Number.NUM_UNDEFINED;

            EM_ReturnCode eRet = Helpers.eGramHashToSubparadigm(sHash, ref ePartOfSpeech, ref eSubParadigm);

            string[] arrTokens = sHash.Split('_');

            string sNumber = "";

            switch (eSubParadigm)
            {
                case EM_Subparadigm.SUBPARADIGM_NOUN:
                case EM_Subparadigm.SUBPARADIGM_PRONOUN:
                    if (arrTokens.Length < 2)
                    {
                        return EM_ReturnCode.H_NO_ERROR;
                    }
                    sNumber = arrTokens[1];
                    break;

                case EM_Subparadigm.SUBPARADIGM_LONG_ADJ:
                case EM_Subparadigm.SUBPARADIGM_PRONOUN_ADJ:
                case EM_Subparadigm.SUBPARADIGM_PART_PRES_ACT:
                case EM_Subparadigm.SUBPARADIGM_PART_PAST_ACT:
                case EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG:
                case EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG:
                    if (arrTokens.Length < 3)
                    {
                        return EM_ReturnCode.H_NO_ERROR;
                    }
                    sNumber = arrTokens[2];
                    break;

                case EM_Subparadigm.SUBPARADIGM_PRESENT_TENSE:
                    if (arrTokens.Length < 2)
                    {
                        return EM_ReturnCode.H_NO_ERROR;
                    }
                    sNumber = arrTokens[1];
                    break;

                default:
                    return EM_ReturnCode.H_ERROR_UNEXPECTED;

            }       //  switch (eSubparadigm)

            if (sNumber.Length > 0)
            {
                switch (sNumber)
                {
                    case "Sg":
                        eNumber = EM_Number.NUM_SG;
                        break;

                    case "Pl":
                        eNumber = EM_Number.NUM_PL;
                        break;

                    default:
                        return EM_ReturnCode.H_ERROR_UNEXPECTED;
                }
            }

            return EM_ReturnCode.H_ERROR_UNEXPECTED;

        }   //  eGramHashToNumber()

        public static EM_ReturnCode eGramHashToCase(string sHash, ref EM_Case eCase, ref EM_Animacy eAnimacy)
        {
            EM_PartOfSpeech ePartOfSpeech = EM_PartOfSpeech.POS_UNDEFINED;
            EM_Subparadigm eSubParadigm = EM_Subparadigm.SUBPARADIGM_UNDEFINED;

            eCase = EM_Case.CASE_UNDEFINED;
            eAnimacy = EM_Animacy.ANIM_UNDEFINED;

            EM_ReturnCode eRet = Helpers.eGramHashToSubparadigm(sHash, ref ePartOfSpeech, ref eSubParadigm);

            string[] arrTokens = sHash.Split('_');

            string sCase = "";

            bool bAdjParadigm = false;

            switch (eSubParadigm)
            {
                case EM_Subparadigm.SUBPARADIGM_NOUN:
                case EM_Subparadigm.SUBPARADIGM_PRONOUN:
                    if (arrTokens.Length < 3)
                    {
                        return EM_ReturnCode.H_NO_ERROR;
                    }
                    sCase = arrTokens[2];
                    break;

                case EM_Subparadigm.SUBPARADIGM_LONG_ADJ:
                case EM_Subparadigm.SUBPARADIGM_PRONOUN_ADJ:
                case EM_Subparadigm.SUBPARADIGM_PART_PRES_ACT:
                case EM_Subparadigm.SUBPARADIGM_PART_PAST_ACT:
                case EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG:
                case EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG:
                    bAdjParadigm = true;

                    if (arrTokens.Length < 4)
                    {
                        return EM_ReturnCode.H_NO_ERROR;
                    }
                    sCase = arrTokens[3];
                    break;

                default:
                    return EM_ReturnCode.H_ERROR_UNEXPECTED;

            }       //  switch (eSubparadigm)

            if (sCase.Length <= 0)
            {
                return EM_ReturnCode.H_ERROR_UNEXPECTED;
            }

            switch (sCase)
            {
                case "N":
                    eCase = EM_Case.CASE_NOM;
                    break;
                case "A":
                    eCase = EM_Case.CASE_ACC;
                    if (bAdjParadigm)
                    {
                        if (arrTokens.Length >= 5)
                        {
                            string sAnimacy = arrTokens[4];
                            if ("Anim" == sAnimacy)
                            {
                                eAnimacy = EM_Animacy.ANIM_YES;
                            }
                            if ("Inanim" == sAnimacy)
                            {
                                eAnimacy = EM_Animacy.ANIM_YES;
                            }
                            else
                            {
                                return EM_ReturnCode.H_ERROR_UNEXPECTED;
                            }
                        }
                    }
                    break;
                case "G":
                    eCase = EM_Case.CASE_GEN;
                    break;
                case "L":
                    eCase = EM_Case.CASE_LOC;
                    break;
                case "D":
                    eCase = EM_Case.CASE_DAT;
                    break;
                case "I":
                    eCase = EM_Case.CASE_INST;
                    break;
                case "P":
                    eCase = EM_Case.CASE_PREP;
                    break;
                case "Part":
                    eCase = EM_Case.CASE_PART;
                    break;
                case "Num":
                    eCase = EM_Case.CASE_NUM;
                    break;
                default:
                    return EM_ReturnCode.H_ERROR_UNEXPECTED;

            }       //  switch (sCase)

            return EM_ReturnCode.H_NO_ERROR;

        }   //  eGramHashToCase()

        public static string sFormHashToDisplayHash(string sFormHash)
        {
            EM_PartOfSpeech ePartOfSpeech = EM_PartOfSpeech.POS_UNDEFINED;
            EM_Subparadigm eSubparadigm = EM_Subparadigm.SUBPARADIGM_UNDEFINED;
            EM_ReturnCode eRet = eGramHashToSubparadigm(sFormHash, ref ePartOfSpeech, ref eSubparadigm);
            if (eRet != EM_ReturnCode.H_NO_ERROR)
            {
                MessageBox.Show(string.Format("Unable to parse gramm. hash {0}", sFormHash));
                return "";
            }

            int iKeyIdx = -1;
            string sDisplayHash;

            try
            {
                switch (ePartOfSpeech)
                {
                    // Nominal inflection
                    case EM_PartOfSpeech.POS_NOUN:
                        sDisplayHash = sFormHash;
                        break;

                    case EM_PartOfSpeech.POS_PRONOUN:
                        iKeyIdx = m_listPropNamesPronoun.IndexOf(sFormHash);
                        sDisplayHash = m_listPropNamesNoun[iKeyIdx];
                        break;

                    case EM_PartOfSpeech.POS_NUM:
                        iKeyIdx = m_listPropNamesNumeral.IndexOf(sFormHash);
                        sDisplayHash = m_listPropNamesNoun[iKeyIdx];
                        break;

                    case EM_PartOfSpeech.POS_ADJ:
                        if (eSubparadigm == EM_Subparadigm.SUBPARADIGM_LONG_ADJ)
                        {
                            sDisplayHash = sFormHash;
                        }
                        else if (eSubparadigm == EM_Subparadigm.SUBPARADIGM_PRONOUN_ADJ)
                        {
                            iKeyIdx = m_listPropNamesPronAdj.IndexOf(sFormHash);
                            sDisplayHash = m_listPropNamesAdj[iKeyIdx];
                        }
                        else if (ePartOfSpeech == EM_PartOfSpeech.POS_VERB)
                        {
                            //                                IsDerived = true;

                            switch (eSubparadigm)
                            {
                                case EM_Subparadigm.SUBPARADIGM_PART_PRES_ACT:
                                    iKeyIdx = m_listPropNamesPartPresAct.IndexOf(sFormHash);
                                    break;

                                case EM_Subparadigm.SUBPARADIGM_PART_PAST_ACT:
                                    iKeyIdx = m_listPropNamesPartPastAct.IndexOf(sFormHash);
                                    break;

                                case EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG:
                                case EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_SHORT:
                                    iKeyIdx = m_listPropNamesPartPresPass.IndexOf(sFormHash);
                                    break;

                                case EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG:
                                case EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_SHORT:
                                    iKeyIdx = m_listPropNamesPartPastPass.IndexOf(sFormHash);
                                    break;

                                default:
                                    MessageBox.Show(string.Format("Subparadigm {0} was not recognized", eSubparadigm));
                                    return "";
                            }

                            sDisplayHash = m_listPropNamesAdj[iKeyIdx];

                        }

                        break;

                    default:
                        MessageBox.Show(string.Format("Part of speech {0} was not recognized", ePartOfSpeech));
                        return "";
                 
                }       //  switch (ePartOfSpeech)


            }
            catch (Exception ex)
            {
                MessageBox.Show(String.Format("Exception: {0}", ex.Message));
                return "";
            }

            return m_listPropNamesNoun[iKeyIdx];

        }       //  sFormHashToDisplayHash()

        public static EM_ReturnCode eParseGramHash(string sHash, ref EM_PartOfSpeech ePartOfSpeech, ref EM_Subparadigm eSubParadigm,
            ref EM_Number eNumber, ref EM_Gender eGender, ref EM_Case eCase, ref EM_Person ePerson, ref EM_Animacy eAnimacy)
        {
            ePartOfSpeech = EM_PartOfSpeech.POS_UNDEFINED;
            eSubParadigm = EM_Subparadigm.SUBPARADIGM_UNDEFINED;
            eNumber = EM_Number.NUM_UNDEFINED;
            eGender = EM_Gender.GENDER_UNDEFINED;
            eCase = EM_Case.CASE_UNDEFINED;
            ePerson = EM_Person.PERSON_UNDEFINED;

            string[] arrTokens = sHash.Split('_');
            if (arrTokens.Length < 1)
            {
                return EM_ReturnCode.H_ERROR_UNEXPECTED;
            }

            bool bAdjParadigm = false;

            string sSubparadigm = arrTokens[0];
            switch (sSubparadigm)
            {
                case "Noun":
                    ePartOfSpeech = EM_PartOfSpeech.POS_NOUN;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_NOUN;
                    break;

                case "Pronoun":
                    ePartOfSpeech = EM_PartOfSpeech.POS_PRONOUN;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_PRONOUN;
                    break;

                case "AdjL":
                    ePartOfSpeech = EM_PartOfSpeech.POS_ADJ;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_LONG_ADJ;
                    break;

                case "PronAdj":
                    ePartOfSpeech = EM_PartOfSpeech.POS_PRONOUN_ADJ;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_PRONOUN_ADJ;
                    break;

                case "PPresA":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_PART_PRES_ACT;
                    break;

                case "PPastA":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_PART_PAST_ACT;
                    break;

                case "PPresPL":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG;
                    break;

                case "PPastPL":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG;
                    break;

                case "Infinitive":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_INFINITIVE;
                    break;

                case "Pres":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_PRESENT_TENSE;
                    break;

                case "Past":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_PAST_TENSE;
                    break;

                case "Impv":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_IMPERATIVE;
                    break;

                case "VAdvPres":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_ADVERBIAL_PRESENT;
                    break;

                case "VAdvPast":
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    eSubParadigm = EM_Subparadigm.SUBPARADIGM_ADVERBIAL_PAST;
                    break;

                default:
                    return EM_ReturnCode.H_ERROR_UNEXPECTED;

            }       //  switch (sSubparadigm)

            if (arrTokens.Length < 2)
            {
                return EM_ReturnCode.H_NO_ERROR;
            }

            if (EM_Subparadigm.SUBPARADIGM_LONG_ADJ == eSubParadigm ||
                EM_Subparadigm.SUBPARADIGM_PRONOUN_ADJ == eSubParadigm ||
                EM_Subparadigm.SUBPARADIGM_PART_PRES_ACT == eSubParadigm ||
                EM_Subparadigm.SUBPARADIGM_PART_PAST_ACT == eSubParadigm ||
                EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG == eSubParadigm ||
                EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG == eSubParadigm)
            {
                bAdjParadigm = true;
            }

            //
            // Gender
            //
            if (bAdjParadigm)
            {
                if (arrTokens.Length < 2)
                {
                    return EM_ReturnCode.H_NO_ERROR;
                }

                string sGender = arrTokens[1];
                switch (sGender)
                {
                    case "M":
                        eGender = EM_Gender.GENDER_M;
                        break;

                    case "N":
                        eGender = EM_Gender.GENDER_N;
                        break;

                    case "F":
                        eGender = EM_Gender.GENDER_F;
                        break;

                    default:
                        return EM_ReturnCode.H_ERROR_UNEXPECTED;
                }

                if (arrTokens.Length < 3)
                {
                    return EM_ReturnCode.H_ERROR_UNEXPECTED;
                }
            }

            //
            // Number
            //
            string sNumber = "";
            if (EM_Subparadigm.SUBPARADIGM_NOUN == eSubParadigm ||
                EM_Subparadigm.SUBPARADIGM_PRONOUN == eSubParadigm)
            {
                if (arrTokens.Length < 2)
                {
                    return EM_ReturnCode.H_NO_ERROR;
                }
                sNumber = arrTokens[1];
            }
            else if (bAdjParadigm)
            {
                if (arrTokens.Length < 3)
                {
                    return EM_ReturnCode.H_NO_ERROR;
                }
                sNumber = arrTokens[2];
            }
            else if (EM_Subparadigm.SUBPARADIGM_PRESENT_TENSE == eSubParadigm ||
                     EM_Subparadigm.SUBPARADIGM_IMPERATIVE == eSubParadigm)
            {
                if (arrTokens.Length < 2)
                {
                    return EM_ReturnCode.H_NO_ERROR;
                }
                sNumber = arrTokens[1];
            }

            if (sNumber.Length > 0)
            {
                switch (sNumber)
                {
                    case "Sg":
                        eNumber = EM_Number.NUM_SG;
                        break;

                    case "Pl":
                        eNumber = EM_Number.NUM_PL;
                        break;

                    default:
                        return EM_ReturnCode.H_ERROR_UNEXPECTED;
                }
            }

            //
            // Case
            //
            string sCase = "";
            if (EM_Subparadigm.SUBPARADIGM_NOUN == eSubParadigm ||
                EM_Subparadigm.SUBPARADIGM_PRONOUN == eSubParadigm)
            {
                if (arrTokens.Length < 3)
                {
                    return EM_ReturnCode.H_NO_ERROR;
                }
                sCase = arrTokens[2];
            }
            else if (bAdjParadigm)
            {
                if (arrTokens.Length < 4)
                {
                    return EM_ReturnCode.H_NO_ERROR;
                }
                sCase = arrTokens[3];
            }

            if (sCase.Length > 0)
            {
                switch (sCase)
                {

                    case "N":
                        eCase = EM_Case.CASE_NOM;
                        break;
                    case "A":
                        eCase = EM_Case.CASE_ACC;
                        break;
                    case "G":
                        eCase = EM_Case.CASE_GEN;
                        break;
                    case "L":
                        eCase = EM_Case.CASE_LOC;
                        break;
                    case "D":
                        eCase = EM_Case.CASE_DAT;
                        break;
                    case "I":
                        eCase = EM_Case.CASE_INST;
                        break;
                    case "P":
                        eCase = EM_Case.CASE_PREP;
                        break;
                    case "Part":
                        eCase = EM_Case.CASE_PART;
                        break;
                    case "Num":
                        eCase = EM_Case.CASE_NUM;
                        break;
                    default:
                        return EM_ReturnCode.H_ERROR_UNEXPECTED;

                }
            }       //  if (sCase.Length > 0)

            //
            // Animacy
            //
            eAnimacy = EM_Animacy.ANIM_UNDEFINED;
            if (bAdjParadigm)
            {
                if (EM_Case.CASE_ACC == eCase)
                {
                    if (arrTokens.Length >= 5)
                    {
                        string sAnimacy = arrTokens[4];
                        if ("Anim" == sAnimacy)
                        {
                            eAnimacy = EM_Animacy.ANIM_YES;
                        }
                        if ("Inanim" == sAnimacy)
                        {
                            eAnimacy = EM_Animacy.ANIM_YES;
                        }
                        else
                        {
                            return EM_ReturnCode.H_ERROR_UNEXPECTED;
                        }
                    }
                }
            }

            ePerson = EM_Person.PERSON_UNDEFINED;
            if (EM_Subparadigm.SUBPARADIGM_PRESENT_TENSE == eSubParadigm || 
                EM_Subparadigm.SUBPARADIGM_IMPERATIVE == eSubParadigm)
            {
                if (arrTokens.Length < 3)
                {
                    return EM_ReturnCode.H_ERROR_UNEXPECTED;
                }

                string sPerson = arrTokens[2];
                switch (sPerson)
                {
                    case "1":
                        ePerson = EM_Person.PERSON_1;
                        break;
                    case "2":
                        ePerson = EM_Person.PERSON_2;
                        break;
                    case "3":
                        ePerson = EM_Person.PERSON_3;
                        break;
                    default:
                        return EM_ReturnCode.H_ERROR_UNEXPECTED;
                }
            }

            if (EM_Subparadigm.SUBPARADIGM_PAST_TENSE == eSubParadigm)
            {
                if (arrTokens.Length < 2)
                {
                    return EM_ReturnCode.H_ERROR_UNEXPECTED;
                }

                eNumber = EM_Number.NUM_SG;
                switch (arrTokens[1])
                {
                    case "M":
                        eGender = EM_Gender.GENDER_M;
                        break;
                    case "F":
                        eGender = EM_Gender.GENDER_F;
                        break;
                    case "N":
                        eGender = EM_Gender.GENDER_N;
                        break;
                    case "Pl":
                        eNumber = EM_Number.NUM_PL;
                        break;
                    default:
                        return EM_ReturnCode.H_ERROR_UNEXPECTED;
                }
            }

            return EM_ReturnCode.H_NO_ERROR;

        }       //  eParseGramHash()

        public static EM_PartOfSpeech SubparadigmToPOS(EM_Subparadigm eSubparadigm)
        {
            EM_PartOfSpeech ePartOfSpeech = EM_PartOfSpeech.POS_UNDEFINED;

            switch (eSubparadigm)
            {
                case EM_Subparadigm.SUBPARADIGM_NOUN:
                    ePartOfSpeech = EM_PartOfSpeech.POS_NOUN;
                    break;
                case EM_Subparadigm.SUBPARADIGM_LONG_ADJ:
                case EM_Subparadigm.SUBPARADIGM_SHORT_ADJ:
                case EM_Subparadigm.SUBPARADIGM_COMPARATIVE:
                    ePartOfSpeech = EM_PartOfSpeech.POS_ADJ;
                    break;

                case EM_Subparadigm.SUBPARADIGM_PRONOUN:
                    ePartOfSpeech = EM_PartOfSpeech.POS_PRONOUN;
                    break;

                case EM_Subparadigm.SUBPARADIGM_PRONOUN_ADJ:
                    ePartOfSpeech = EM_PartOfSpeech.POS_PRONOUN_ADJ;
                    break;

                case EM_Subparadigm.SUBPARADIGM_INFINITIVE:
                case EM_Subparadigm.SUBPARADIGM_PRESENT_TENSE:
                case EM_Subparadigm.SUBPARADIGM_PAST_TENSE:
                case EM_Subparadigm.SUBPARADIGM_IMPERATIVE:
                case EM_Subparadigm.SUBPARADIGM_ADVERBIAL_PRESENT:
                case EM_Subparadigm.SUBPARADIGM_ADVERBIAL_PAST:
                case EM_Subparadigm.SUBPARADIGM_PART_PRES_ACT:
                case EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG:
                case EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_SHORT:
                case EM_Subparadigm.SUBPARADIGM_PART_PAST_ACT:
                case EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG:
                case EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_SHORT:
                    ePartOfSpeech = EM_PartOfSpeech.POS_VERB;
                    break;

                case EM_Subparadigm.SUBPARADIGM_NUM:
                case EM_Subparadigm.SUBPARADIGM_NUM_ADJ:
                case EM_Subparadigm.SUBPARADIGM_ASPECT_PAIR:

                case EM_Subparadigm.SUBPARADIGM_UNDEFINED:
                case EM_Subparadigm.SUBPARADIGM_COUNT:
                default:
                    break;
            }

            return ePartOfSpeech;

        }       //  SubparadigmToPOS()

        public static EM_AccentType eStringToAccentType(string sAccentType)
        {
            switch(sAccentType)
            {
                case "a":
                    return EM_AccentType.AT_A;
                case "a'":
                    return EM_AccentType.AT_A1;
                case "b":
                    return EM_AccentType.AT_B;
                case "b'":
                    return EM_AccentType.AT_B1;
                case "c":
                    return EM_AccentType.AT_C;
                case "c'":
                    return EM_AccentType.AT_C1;
                case "c''":
                    return EM_AccentType.AT_C2;
                case "d":
                    return EM_AccentType.AT_D;
                case "d'":
                    return EM_AccentType.AT_D1;
                case "e":
                    return EM_AccentType.AT_E;
                case "f":
                    return EM_AccentType.AT_F;
                case "f'":
                    return EM_AccentType.AT_F1;
                case "f''":
                    return EM_AccentType.AT_F2;
                default:
                    return EM_AccentType.AT_UNDEFINED;
            }
        }

        public static void StressMarksToPosList(string sWord, out string sOutWord, out Dictionary<int, EM_StressType> dictPositions)
        {
            dictPositions = new Dictionary<int, EM_StressType>();
            sOutWord = "";
            int iRemoved = 0;

            for (int iPos = 0; iPos < sWord.Length; ++iPos)
            {
                char chr = sWord[iPos];
                if ('\x300' == chr)
                {
                    dictPositions[iPos-(iRemoved++)-1] = EM_StressType.STRESS_SECONDARY;
                }
                else if ('\x301' == chr)
                {
                    dictPositions[iPos-(iRemoved++)-1] = EM_StressType.STRESS_PRIMARY;
                }
                else if ('ё' == chr)
                {
                    dictPositions[iPos-iRemoved] = EM_StressType.STRESS_PRIMARY;
                    sOutWord += chr;
                }
                else
                {
                    sOutWord += chr;
                }
            }
        }

        public static void MarkStress(ref string sWordForm, CWordFormManaged wf)
        {
            int iPos = -1;
            EM_StressType eType = EM_StressType.STRESS_TYPE_UNDEFINED;
            char chrMark = ' ';
            int iStressMarksInserted = 0;

            EM_ReturnCode eRet = wf.eGetFirstStressPos(ref iPos, ref eType);
            do
            {
                if (eRet != EM_ReturnCode.H_NO_ERROR && eRet != EM_ReturnCode.H_FALSE)
                {
                    continue;
                }

                if (EM_StressType.STRESS_PRIMARY == eType)
                {
                    if (sWordForm[iPos+iStressMarksInserted] != 'ё')
                    {
                        chrMark = '\x301';
                        sWordForm = sWordForm.Insert(iPos + iStressMarksInserted++ + 1, chrMark.ToString());
                    }
                }
                else if (EM_StressType.STRESS_SECONDARY == eType)
                {
                    chrMark = '\x300';
                    sWordForm = sWordForm.Insert(iPos + iStressMarksInserted++ + 1, chrMark.ToString());
                }

                eRet = wf.eGetNextStressPos(ref iPos, ref eType);

            } while (EM_ReturnCode.H_NO_ERROR == eRet);

        }    //  MarkStress()

        // "/", "\" --> diacritics
        public static void AssignDiacritics(string sSource, ref string sTarget)
        {
            char chrStressMark = '\x301';
            char chrSecondaryStressMark = '\x300';
            int iPos = 0;
            sTarget = sSource;
            iPos = sTarget.IndexOf('/', iPos);
            while (iPos >= 0 && iPos < sTarget.Length - 1)
            {
                if (sTarget[iPos] != sTarget[iPos + 1] && (sTarget[iPos + 1] != 'ё'))
                {
                    sTarget = sTarget.Substring(0, iPos) + sTarget.Substring(iPos + 1, 1) + chrStressMark.ToString() + sTarget.Substring(iPos + 2);
                }
                else if ('ё' == sTarget[iPos + 1])
                {
                    sTarget = sTarget.Substring(0, iPos) + sTarget.Substring(iPos + 1, 1) + sTarget.Substring(iPos + 2);
                }
                else
                {
                    iPos += 2;
                }
                iPos = sTarget.IndexOf('/', iPos);
            }
            iPos = sTarget.IndexOf('\\', 0);
            while (iPos >= 0 && iPos < sTarget.Length - 1)
            {
                if (sTarget[iPos] != sTarget[iPos + 1])
                {
                    sTarget = sTarget.Substring(0, iPos) + sTarget.Substring(iPos + 1, 1) + chrSecondaryStressMark.ToString() + sTarget.Substring(iPos + 2);
                }
                else
                {
                    iPos += 2;
                }
                iPos = sTarget.IndexOf('\\', iPos);
            }
        }       //  ExtractStressMarks()

        // Diacritics --> "/", "\"
        public static void RestoreStressMarks(ref string sWord)
        {
            char chrStressMark = '\x301';
            char chrSecondaryStressMark = '\x300';
            char[] arrStressMarks = { chrStressMark, chrSecondaryStressMark };

            int iPos = 0;
            iPos = sWord.IndexOfAny(arrStressMarks, iPos);
            while (iPos > 0 && iPos < sWord.Length - 1)
            {
                StringBuilder sb = new StringBuilder(sWord);
                char chrDiacritic = (chrStressMark == sb[iPos]) ? '/' : '\\';
                sb[iPos] = sb[iPos-1];
                sb[iPos-1] = chrDiacritic;
                sWord = sb.ToString();

                iPos = sWord.IndexOfAny(arrStressMarks, iPos+1);
            }
        }

        public static string sListToCommaSeparatedString(List<string> forms)
        {
            string sOut = "";
            if (null == forms)
            {
                return sOut;
            }

            foreach (var sForm in forms)
            {
                if (sOut.Length > 0)
                {
                    sOut += ", ";
                }
                sOut += sForm;
            }
            return sOut;
        }

        public static string sListToCommaSeparatedString(List<string> forms, List<Tuple<string, string>> comments)
        {
            string sOut = "";
            if (null == forms)
            {
                return sOut;
            }

            for (int iAt = 0; iAt < forms.Count; ++iAt)
            {
                if (sOut.Length > 0)
                {
                    sOut += ", ";
                }

                if (iAt < comments.Count && comments[iAt].Item1.Length > 0)
                {
                    sOut += String.Format("({0}) ", comments[iAt].Item1);
                }
                sOut += forms[iAt];
                if (iAt < comments.Count && comments[iAt].Item2.Length > 0)
                {
                    sOut += String.Format(" ({0})", comments[iAt].Item2);
                }
            }
            return sOut;
        }

        public static List<string> CommaSeparatedStringToList(string str)
        {
            return str.Split(',').Select(sValue => sValue.Trim()).ToList<string>();
        }

        public static bool CommaSeparatedStringToList(string str, out List<string> listForms, out List<Tuple<string, string>> listComments)
        {
            listForms = new List<string>();
            listComments = new List<Tuple<string, string>>();

            Regex regex = new Regex(@"^(?:\(([^\)]+)\))?([^\(]+)(?:\(([^\)]+)?\))?");

            List<string> listSegments = str.Split(',').Select(sValue => sValue.Trim()).ToList<string>();
            foreach (var sSegment in listSegments)
            {
                Match m = regex.Match(sSegment);
                if (m.Groups.Count != 4)
                {
                    return false;
                }

                string sForm = m.Groups[2].ToString().Trim();
                if (sForm.Length < 1)
                {
                    return false;
                }

                string sLeftComment = m.Groups[1].ToString().Trim();
                string sRightComment = m.Groups[3].ToString().Trim();
                listForms.Add(sForm);
                listComments.Add(new Tuple<string, string>(sLeftComment, sRightComment));
            }

            return true;
        }

        public static bool Swap<T>(IList<T> list, int indexA, int indexB)
        {
            try
            {
                T tmp = list[indexA];
                list[indexA] = list[indexB];
                list[indexB] = tmp;
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show("Internal error: exception during list item swap. " + ex.Message);
                return false;
            }

            return true;
        }

        public static string sStripPunctuation(string sWord)
        {
            var stripped = new StringBuilder();
            foreach (char chr in sWord)
            {
                if (!char.IsPunctuation(chr))
                {
                    stripped.Append(chr);
                }
            }

            return stripped.ToString();
        }

    }       //  public static class HelpersC:\git-repos\Zal-Windows\TestApp\packages.config
}       //  namespace ZalTestApp
