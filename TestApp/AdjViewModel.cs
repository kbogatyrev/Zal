﻿using System;
using System.Windows.Input;
using System.Windows;
using System.Collections.Generic;

using MainLibManaged;

namespace ZalTestApp
{
    public class AdjViewModel : ViewModelBase
    {
        public delegate void BackButtonHandler();
        public event BackButtonHandler BackButtonEvent;

        MainModel m_MainModel = null;

        #region ICommand
        private ICommand m_BackCommand;
        public ICommand BackCommand
        {
            get
            {
                return m_BackCommand;
            }
            set
            {
                m_BackCommand = value;
            }
        }

        private ICommand m_EditCommand;
        public ICommand EditCommand
        {
            get
            {
                return m_EditCommand;
            }
            set
            {
                m_EditCommand = value;
            }
        }

        #endregion

        private CLexemeManaged m_Parent;
        public CLexemeManaged Parent
        {
            get
            {
                return m_Parent;
            }

            set
            {
                m_Parent = value;
            }
        }

        EM_Subparadigm m_eSubparadigm = EM_Subparadigm.SUBPARADIGM_UNDEFINED;
        public EM_Subparadigm Subparadigm
        {
            get
            {
                return m_eSubparadigm;
            }

            set
            {
                m_eSubparadigm = value;
            }
        }

        bool m_bIsDerived;
        public bool IsDerived
        {
            get
            {
                return m_bIsDerived;
            }

            set
            {
                m_bIsDerived = value;
            }
        }

        private string m_sSourceForm;
        public string SourceForm
        {
            get
            {
                return m_sSourceForm;
            }
            set
            {
                m_sSourceForm = value;
                OnPropertyChanged("SourceForm");
            }
        }

        #region Bindings_Long_Masculine_Sg

        private string m_sAdjL_M_Sg_N;
        public string AdjL_M_Sg_N
        {
            get
            {
                return m_sAdjL_M_Sg_N;
            }
            set
            {
                m_sAdjL_M_Sg_N = value;
                OnPropertyChanged("AdjL_M_Sg_N");
            }
        }

        private bool m_bAdjL_M_Sg_N_IsReadOnly = true;
        public bool AdjL_M_Sg_N_IsReadOnly
        {
            get
            {
                return m_bAdjL_M_Sg_N_IsReadOnly;
            }
            set
            {
                m_bAdjL_M_Sg_N_IsReadOnly = value;
                OnPropertyChanged("AdjL_M_Sg_N_IsReadOnly");
            }
        }

        private string m_sAdjL_M_Sg_A_Anim;
        public string AdjL_M_Sg_A_Anim
        {
            get
            {
                return m_sAdjL_M_Sg_A_Anim;
            }
            set
            {
                m_sAdjL_M_Sg_A_Anim = value;
                OnPropertyChanged("AdjL_M_Sg_A_Anim");
            }
        }

        private bool m_bAdjL_M_Sg_A_Anim_IsReadOnly = true;
        public bool AdjL_M_Sg_A_Anim_IsReadOnly
        {
            get
            {
                return m_bAdjL_M_Sg_A_Anim_IsReadOnly;
            }
            set
            {
                m_bAdjL_M_Sg_A_Anim_IsReadOnly = value;
                OnPropertyChanged("AdjL_M_Sg_A_Anim_IsReadOnly");
            }
        }

        private string m_sAdjL_M_Sg_A_Inanim;
        public string AdjL_M_Sg_A_Inanim
        {
            get
            {
                return m_sAdjL_M_Sg_A_Inanim;
            }
            set
            {
                m_sAdjL_M_Sg_A_Inanim = value;
                OnPropertyChanged("AdjL_M_Sg_A_Inanim");
            }
        }

        private bool m_bAdjL_M_Sg_A_Inanim_IsReadOnly = true;
        public bool AdjL_M_Sg_A_Inanim_IsReadOnly
        {
            get
            {
                return m_bAdjL_M_Sg_A_Inanim_IsReadOnly;
            }
            set
            {
                m_bAdjL_M_Sg_A_Inanim_IsReadOnly = value;
                OnPropertyChanged("AdjL_M_Sg_A_Inanim_IsReadOnly");
            }
        }

        private string m_sAdjL_M_Sg_G;
        public string AdjL_M_Sg_G
        {
            get
            {
                return m_sAdjL_M_Sg_G;
            }
            set
            {
                m_sAdjL_M_Sg_G = value;
                OnPropertyChanged("AdjL_M_Sg_G");
            }
        }

        private bool m_bAdjL_M_Sg_G_IsReadOnly = true;
        public bool AdjL_M_Sg_G_IsReadOnly
        {
            get
            {
                return m_bAdjL_M_Sg_G_IsReadOnly;
            }
            set
            {
                m_bAdjL_M_Sg_G_IsReadOnly = value;
                OnPropertyChanged("AdjL_M_Sg_G_IsReadOnly");
            }
        }

        private string m_sAdjL_M_Sg_P;
        public string AdjL_M_Sg_P
        {
            get
            {
                return m_sAdjL_M_Sg_P;
            }
            set
            {
                m_sAdjL_M_Sg_P = value;
                OnPropertyChanged("AdjL_M_Sg_P");
            }
        }

        private bool m_bAdjL_M_Sg_P_IsReadOnly = true;
        public bool AdjL_M_Sg_P_IsReadOnly
        {
            get
            {
                return m_bAdjL_M_Sg_P_IsReadOnly;
            }
            set
            {
                m_bAdjL_M_Sg_P_IsReadOnly = value;
                OnPropertyChanged("AdjL_M_Sg_P_IsReadOnly");
            }
        }

        private string m_sAdjL_M_Sg_D;
        public string AdjL_M_Sg_D
        {
            get
            {
                return m_sAdjL_M_Sg_D;
            }
            set
            {
                m_sAdjL_M_Sg_D = value;
                OnPropertyChanged("AdjL_M_Sg_D");
            }
        }

        private bool m_bAdjL_M_Sg_D_IsReadOnly = true;
        public bool AdjL_M_Sg_D_IsReadOnly
        {
            get
            {
                return m_bAdjL_M_Sg_D_IsReadOnly;
            }
            set
            {
                m_bAdjL_M_Sg_D_IsReadOnly = value;
                OnPropertyChanged("AdjL_M_Sg_D_IsReadOnly");
            }
        }

        private string m_sAdjL_M_Sg_I;
        public string AdjL_M_Sg_I
        {
            get
            {
                return m_sAdjL_M_Sg_I;
            }
            set
            {
                m_sAdjL_M_Sg_I = value;
                OnPropertyChanged("AdjL_M_Sg_I");
            }
        }

        private bool m_bAdjL_M_Sg_I_IsReadOnly = true;
        public bool AdjL_M_Sg_I_IsReadOnly
        {
            get
            {
                return m_bAdjL_M_Sg_I_IsReadOnly;
            }
            set
            {
                m_bAdjL_M_Sg_I_IsReadOnly = value;
                OnPropertyChanged("AdjL_M_Sg_I_IsReadOnly");
            }
        }

        #endregion

        #region Bindings_Long_Feminine_Sg

        private string m_sAdjL_F_Sg_N;
        public string AdjL_F_Sg_N
        {
            get
            {
                return m_sAdjL_F_Sg_N;
            }
            set
            {
                m_sAdjL_F_Sg_N = value;
                OnPropertyChanged("AdjL_F_Sg_N");
            }
        }

        private bool m_bAdjL_F_Sg_N_IsReadOnly = true;
        public bool AdjL_F_Sg_N_IsReadOnly
        {
            get
            {
                return m_bAdjL_F_Sg_N_IsReadOnly;
            }
            set
            {
                m_bAdjL_F_Sg_N_IsReadOnly = value;
                OnPropertyChanged("AdjL_F_Sg_N_IsReadOnly");
            }
        }

        private string m_sAdjL_F_Sg_A;
        public string AdjL_F_Sg_A
        {
            get
            {
                return m_sAdjL_F_Sg_A;
            }
            set
            {
                m_sAdjL_F_Sg_A = value;
                OnPropertyChanged("AdjL_F_Sg_A");
            }
        }

        private bool m_bAdjL_F_Sg_A_IsReadOnly = true;
        public bool AdjL_F_Sg_A_IsReadOnly
        {
            get
            {
                return m_bAdjL_F_Sg_A_IsReadOnly;
            }
            set
            {
                m_bAdjL_F_Sg_A_IsReadOnly = value;
                OnPropertyChanged("AdjL_F_Sg_A_IsReadOnly");
            }
        }

        private string m_sAdjL_F_Sg_G;
        public string AdjL_F_Sg_G
        {
            get
            {
                return m_sAdjL_F_Sg_G;
            }
            set
            {
                m_sAdjL_F_Sg_G = value;
                OnPropertyChanged("AdjL_F_Sg_G");
            }
        }

        private bool m_bAdjL_F_Sg_G_IsReadOnly = true;
        public bool AdjL_F_Sg_G_IsReadOnly
        {
            get
            {
                return m_bAdjL_F_Sg_G_IsReadOnly;
            }
            set
            {
                m_bAdjL_F_Sg_G_IsReadOnly = value;
                OnPropertyChanged("AdjL_F_Sg_G_IsReadOnly");
            }
        }

        private string m_sAdjL_F_Sg_P;
        public string AdjL_F_Sg_P
        {
            get
            {
                return m_sAdjL_F_Sg_P;
            }
            set
            {
                m_sAdjL_F_Sg_P = value;
                OnPropertyChanged("AdjL_F_Sg_P");
            }
        }

        private bool m_bAdjL_F_Sg_P_IsReadOnly = true;
        public bool AdjL_F_Sg_P_IsReadOnly
        {
            get
            {
                return m_bAdjL_F_Sg_P_IsReadOnly;
            }
            set
            {
                m_bAdjL_F_Sg_P_IsReadOnly = value;
                OnPropertyChanged("AdjL_F_Sg_P_IsReadOnly");
            }
        }

        private string m_sAdjL_F_Sg_D;
        public string AdjL_F_Sg_D
        {
            get
            {
                return m_sAdjL_F_Sg_D;
            }
            set
            {
                m_sAdjL_F_Sg_D = value;
                OnPropertyChanged("AdjL_F_Sg_D");
            }
        }

        private bool m_bAdjL_F_Sg_D_IsReadOnly = true;
        public bool AdjL_F_Sg_D_IsReadOnly
        {
            get
            {
                return m_bAdjL_F_Sg_D_IsReadOnly;
            }
            set
            {
                m_bAdjL_F_Sg_D_IsReadOnly = value;
                OnPropertyChanged("AdjL_F_Sg_D_IsReadOnly");
            }
        }

        private string m_sAdjL_F_Sg_I;
        public string AdjL_F_Sg_I
        {
            get
            {
                return m_sAdjL_F_Sg_I;
            }
            set
            {
                m_sAdjL_F_Sg_I = value;
                OnPropertyChanged("AdjL_F_Sg_I");
            }
        }

        private bool m_bAdjL_F_Sg_I_IsReadOnly = true;
        public bool AdjL_F_Sg_I_IsReadOnly
        {
            get
            {
                return m_bAdjL_F_Sg_I_IsReadOnly;
            }
            set
            {
                m_bAdjL_F_Sg_I_IsReadOnly = value;
                OnPropertyChanged("AdjL_F_Sg_I_IsReadOnly");
            }
        }

        #endregion

        #region Bindings_Long_Neuter_Sg

        private string m_sAdjL_N_Sg_N;
        public string AdjL_N_Sg_N
        {
            get
            {
                return m_sAdjL_N_Sg_N;
            }
            set
            {
                m_sAdjL_N_Sg_N = value;
                OnPropertyChanged("AdjL_N_Sg_N");
            }
        }

        private bool m_bAdjL_N_Sg_N_IsReadOnly = true;
        public bool AdjL_N_Sg_N_IsReadOnly
        {
            get
            {
                return m_bAdjL_N_Sg_N_IsReadOnly;
            }
            set
            {
                m_bAdjL_N_Sg_N_IsReadOnly = value;
                OnPropertyChanged("AdjL_N_Sg_N_IsReadOnly");
            }
        }

        private string m_sAdjL_N_Sg_A;
        public string AdjL_N_Sg_A
        {
            get
            {
                return m_sAdjL_N_Sg_A;
            }
            set
            {
                m_sAdjL_N_Sg_A = value;
                OnPropertyChanged("AdjL_N_Sg_A");
            }
        }

        private bool m_bAdjL_N_Sg_A_IsReadOnly = true;
        public bool AdjL_N_Sg_A_IsReadOnly
        {
            get
            {
                return m_bAdjL_N_Sg_A_IsReadOnly;
            }
            set
            {
                m_bAdjL_N_Sg_A_IsReadOnly = value;
                OnPropertyChanged("AdjL_N_Sg_A_IsReadOnly");
            }
        }

        private string m_sAdjL_N_Sg_G;
        public string AdjL_N_Sg_G
        {
            get
            {
                return m_sAdjL_N_Sg_G;
            }
            set
            {
                m_sAdjL_N_Sg_G = value;
                OnPropertyChanged("AdjL_N_Sg_G");
            }
        }

        private bool m_bAdjL_N_Sg_G_IsReadOnly = true;
        public bool AdjL_N_Sg_G_IsReadOnly
        {
            get
            {
                return m_bAdjL_N_Sg_G_IsReadOnly;
            }
            set
            {
                m_bAdjL_N_Sg_G_IsReadOnly = value;
                OnPropertyChanged("AdjL_N_Sg_G_IsReadOnly");
            }
        }

        private string m_sAdjL_N_Sg_P;
        public string AdjL_N_Sg_P
        {
            get
            {
                return m_sAdjL_N_Sg_P;
            }
            set
            {
                m_sAdjL_N_Sg_P = value;
                OnPropertyChanged("AdjL_N_Sg_P");
            }
        }

        private bool m_bAdjL_N_Sg_P_IsReadOnly = true;
        public bool AdjL_N_Sg_P_IsReadOnly
        {
            get
            {
                return m_bAdjL_N_Sg_P_IsReadOnly;
            }
            set
            {
                m_bAdjL_N_Sg_P_IsReadOnly = value;
                OnPropertyChanged("AdjL_N_Sg_P_IsReadOnly");
            }
        }

        private string m_sAdjL_N_Sg_D;
        public string AdjL_N_Sg_D
        {
            get
            {
                return m_sAdjL_N_Sg_D;
            }
            set
            {
                m_sAdjL_N_Sg_D = value;
                OnPropertyChanged("AdjL_N_Sg_D");
            }
        }

        private bool m_bAdjL_N_Sg_D_IsReadOnly = true;
        public bool AdjL_N_Sg_D_IsReadOnly
        {
            get
            {
                return m_bAdjL_N_Sg_D_IsReadOnly;
            }
            set
            {
                m_bAdjL_N_Sg_D_IsReadOnly = value;
                OnPropertyChanged("AdjL_N_Sg_D_IsReadOnly");
            }
        }

        private string m_sAdjL_N_Sg_I;
        public string AdjL_N_Sg_I
        {
            get
            {
                return m_sAdjL_N_Sg_I;
            }
            set
            {
                m_sAdjL_N_Sg_I = value;
                OnPropertyChanged("AdjL_N_Sg_I");
            }
        }

        private bool m_bAdjL_N_Sg_I_IsReadOnly = true;
        public bool AdjL_N_Sg_I_IsReadOnly
        {
            get
            {
                return m_bAdjL_N_Sg_I_IsReadOnly;
            }
            set
            {
                m_bAdjL_N_Sg_I_IsReadOnly = value;
                OnPropertyChanged("AdjL_N_Sg_I_IsReadOnly");
            }
        }

        #endregion

        #region Bindings_Long_Plural

        private string m_sAdjL_Pl_N;
        public string AdjL_Pl_N
        {
            get
            {
                return m_sAdjL_Pl_N;
            }
            set
            {
                m_sAdjL_Pl_N = value;
                OnPropertyChanged("AdjL_Pl_N");
            }
        }

        private bool m_bAdjL_Pl_N_IsReadOnly = true;
        public bool AdjL_Pl_N_IsReadOnly
        {
            get
            {
                return m_bAdjL_Pl_N_IsReadOnly;
            }
            set
            {
                m_bAdjL_Pl_N_IsReadOnly = value;
                OnPropertyChanged("AdjL_Pl_N_IsReadOnly");
            }
        }

        private string m_sAdjL_Pl_A_Anim;
        public string AdjL_Pl_A_Anim
        {
            get
            {
                return m_sAdjL_Pl_A_Anim;
            }
            set
            {
                m_sAdjL_Pl_A_Anim = value;
                OnPropertyChanged("AdjL_Pl_A_Anim");
            }
        }

        private bool m_bAdjL_Pl_A_Anim_IsReadOnly = true;
        public bool AdjL_Pl_A_Anim_IsReadOnly
        {
            get
            {
                return m_bAdjL_Pl_A_Anim_IsReadOnly;
            }
            set
            {
                m_bAdjL_Pl_A_Anim_IsReadOnly = value;
                OnPropertyChanged("AdjL_Pl_A_Anim_IsReadOnly");
            }
        }

        private string m_sAdjL_Pl_A_Inanim;
        public string AdjL_Pl_A_Inanim
        {
            get
            {
                return m_sAdjL_Pl_A_Inanim;
            }
            set
            {
                m_sAdjL_Pl_A_Inanim = value;
                OnPropertyChanged("AdjL_Pl_A_Inanim");
            }
        }

        private bool m_bAdjL_Pl_A_Inanim_IsReadOnly = true;
        public bool AdjL_Pl_A_Inanim_IsReadOnly
        {
            get
            {
                return m_bAdjL_Pl_A_Inanim_IsReadOnly;
            }
            set
            {
                m_bAdjL_Pl_A_Inanim_IsReadOnly = value;
                OnPropertyChanged("AdjL_Pl_A_Inanim_IsReadOnly");
            }
        }

        private string m_sAdjL_Pl_G;
        public string AdjL_Pl_G
        {
            get
            {
                return m_sAdjL_Pl_G;
            }
            set
            {
                m_sAdjL_Pl_G = value;
                OnPropertyChanged("AdjL_Pl_G");
            }
        }

        private bool m_bAdjL_Pl_G_IsReadOnly = true;
        public bool AdjL_Pl_G_IsReadOnly
        {
            get
            {
                return m_bAdjL_Pl_G_IsReadOnly;
            }
            set
            {
                m_bAdjL_Pl_G_IsReadOnly = value;
                OnPropertyChanged("AdjL_Pl_G_IsReadOnly");
            }
        }

        private string m_sAdjL_Pl_P;
        public string AdjL_Pl_P
        {
            get
            {
                return m_sAdjL_Pl_P;
            }
            set
            {
                m_sAdjL_Pl_P = value;
                OnPropertyChanged("AdjL_Pl_P");
            }
        }

        private bool m_bAdjL_Pl_P_IsReadOnly = true;
        public bool AdjL_Pl_P_IsReadOnly
        {
            get
            {
                return m_bAdjL_Pl_P_IsReadOnly;
            }
            set
            {
                m_bAdjL_Pl_P_IsReadOnly = value;
                OnPropertyChanged("AdjL_Pl_P_IsReadOnly");
            }
        }

        private string m_sAdjL_Pl_D;
        public string AdjL_Pl_D
        {
            get
            {
                return m_sAdjL_Pl_D;
            }
            set
            {
                m_sAdjL_Pl_D = value;
                OnPropertyChanged("AdjL_Pl_D");
            }
        }

        private bool m_bAdjL_Pl_D_IsReadOnly = true;
        public bool AdjL_Pl_D_IsReadOnly
        {
            get
            {
                return m_bAdjL_Pl_D_IsReadOnly;
            }
            set
            {
                m_bAdjL_Pl_D_IsReadOnly = value;
                OnPropertyChanged("AdjL_Pl_D_IsReadOnly");
            }
        }

        private string m_sAdjL_Pl_I;
        public string AdjL_Pl_I
        {
            get
            {
                return m_sAdjL_Pl_I;
            }
            set
            {
                m_sAdjL_Pl_I = value;
                OnPropertyChanged("AdjL_Pl_I");
            }
        }

        private bool m_bAdjL_Pl_I_IsReadOnly = true;
        public bool AdjL_Pl_I_IsReadOnly
        {
            get
            {
                return m_bAdjL_Pl_I_IsReadOnly;
            }
            set
            {
                m_bAdjL_Pl_I_IsReadOnly = value;
                OnPropertyChanged("AdjL_Pl_I_IsReadOnly");
            }
        }

        #endregion

        #region Bindings_Short

        private string m_sAdjS_M;
        public string AdjS_M
        {
            get
            {
                return m_sAdjS_M;
            }
            set
            {
                m_sAdjS_M = value;
                OnPropertyChanged("AdjS_M");
            }
        }

        private bool m_bAdjS_M_IsReadOnly = true;
        public bool AdjS_M_IsReadOnly
        {
            get
            {
                return m_bAdjS_M_IsReadOnly;
            }
            set
            {
                m_bAdjS_M_IsReadOnly = value;
                OnPropertyChanged("AdjS_M_IsReadOnly");
            }
        }

        private string m_sAdjS_F;
        public string AdjS_F
        {
            get
            {
                return m_sAdjS_F;
            }
            set
            {
                m_sAdjS_F = value;
                OnPropertyChanged("AdjS_F");
            }
        }

        private bool m_bAdjS_F_IsReadOnly = true;
        public bool AdjS_F_IsReadOnly
        {
            get
            {
                return m_bAdjS_F_IsReadOnly;
            }
            set
            {
                m_bAdjS_F_IsReadOnly = value;
                OnPropertyChanged("AdjS_F_IsReadOnly");
            }
        }

        private string m_sAdjS_N;
        public string AdjS_N
        {
            get
            {
                return m_sAdjS_N;
            }
            set
            {
                m_sAdjS_N = value;
                OnPropertyChanged("AdjS_N");
            }
        }

        private bool m_bAdjS_N_IsReadOnly = true;
        public bool AdjS_N_IsReadOnly
        {
            get
            {
                return m_bAdjS_N_IsReadOnly;
            }
            set
            {
                m_bAdjS_N_IsReadOnly = value;
                OnPropertyChanged("AdjS_N_IsReadOnly");
            }
        }

        private string m_sAdjS_Pl;
        public string AdjS_Pl
        {
            get
            {
                return m_sAdjS_Pl;
            }
            set
            {
                m_sAdjS_Pl = value;
                OnPropertyChanged("AdjS_Pl");
            }
        }

        private bool m_bAdjS_Pl_IsReadOnly = true;
        public bool AdjS_Pl_IsReadOnly
        {
            get
            {
                return m_bAdjS_Pl_IsReadOnly;
            }
            set
            {
                m_bAdjS_Pl_IsReadOnly = value;
                OnPropertyChanged("AdjS_Pl_IsReadOnly");
            }
        }

        #endregion

        #region Bindings_Comparative

        private string m_sAdjComp;
        public string AdjComp
        {
            get
            {
                return m_sAdjComp;
            }
            set
            {
                m_sAdjComp = value;
                OnPropertyChanged("AdjComp");
            }
        }

        private bool m_bAdjComp_IsReadOnly = true;
        public bool AdjComp_IsReadOnly
        {
            get
            {
                return m_bAdjComp_IsReadOnly;
            }
            set
            {
                m_bAdjComp_IsReadOnly = value;
                OnPropertyChanged("AdjComp_IsReadOnly");
            }
        }

        #endregion

        public AdjViewModel(CLexemeManaged lexeme, EM_Subparadigm eSubparadigm, MainModel m)
        {
            m_Parent = lexeme;
            m_MainModel = m;

            BackCommand = new RelayCommand(new Action<object>(GoBack));
            EditCommand = new RelayCommand(new Action<object>(EditForm));

            m_eSubparadigm = eSubparadigm;
            m_bIsDerived = false;

            string sLexemeHash = lexeme.sHash();

            if (lexeme.ePartOfSpeech() == EM_PartOfSpeech.POS_ADJ || lexeme.ePartOfSpeech() == EM_PartOfSpeech.POS_PRONOUN_ADJ)
            {
                List<string> forms = null;
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_M_Sg_N", out forms);
                AdjL_M_Sg_N = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_M_Sg_A_Anim", out forms);
                AdjL_M_Sg_A_Anim = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_M_Sg_A_Inanim", out forms);
                AdjL_M_Sg_A_Inanim = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_M_Sg_G", out forms);
                AdjL_M_Sg_G = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_M_Sg_P", out forms);
                AdjL_M_Sg_P = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_M_Sg_D", out forms);
                AdjL_M_Sg_D = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_M_Sg_I", out forms);
                AdjL_M_Sg_I = Helpers.sListToCommaSeparatedString(forms);

                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_F_Sg_N", out forms);
                AdjL_F_Sg_N = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_F_Sg_A", out forms);
                AdjL_F_Sg_A = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_F_Sg_G", out forms);
                AdjL_F_Sg_G = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_F_Sg_P", out forms);
                AdjL_F_Sg_P = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_F_Sg_D", out forms);
                AdjL_F_Sg_D = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_F_Sg_I", out forms);
                AdjL_F_Sg_I = Helpers.sListToCommaSeparatedString(forms);

                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_N_Sg_N", out forms);
                AdjL_N_Sg_N = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_N_Sg_A", out forms);
                AdjL_N_Sg_A = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_N_Sg_G", out forms);
                AdjL_N_Sg_G = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_N_Sg_P", out forms);
                AdjL_N_Sg_P = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_N_Sg_D", out forms);
                AdjL_N_Sg_D = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_N_Sg_I", out forms);
                AdjL_N_Sg_I = Helpers.sListToCommaSeparatedString(forms);

                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_Pl_N", out forms);
                AdjL_Pl_N = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_Pl_A_Anim", out forms);
                AdjL_Pl_A_Anim = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_Pl_A_Inanim", out forms);
                AdjL_Pl_A_Inanim = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_Pl_G", out forms);
                AdjL_Pl_G = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_Pl_P", out forms);
                AdjL_Pl_P = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_Pl_D", out forms);
                AdjL_Pl_D = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjL_Pl_I", out forms);
                AdjL_Pl_I = Helpers.sListToCommaSeparatedString(forms);

                if (lexeme.ePartOfSpeech() == EM_PartOfSpeech.POS_ADJ)
                {
                    m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjS_M", out forms);
                    AdjS_M = Helpers.sListToCommaSeparatedString(forms);
                    m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjS_F", out forms);
                    AdjS_F = Helpers.sListToCommaSeparatedString(forms);
                    m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjS_N", out forms);
                    AdjS_N = Helpers.sListToCommaSeparatedString(forms);
                    m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjS_Pl", out forms);
                    AdjS_Pl = Helpers.sListToCommaSeparatedString(forms);

                    m_MainModel.GetFormsByGramHash(sLexemeHash, "AdjComp", out forms);
                    AdjComp = Helpers.sListToCommaSeparatedString(forms);
                }
            }
/*
            else if (lexeme.ePartOfSpeech() == EM_PartOfSpeech.POS_PRONOUN_ADJ)
            {
                List<string> forms = null;
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_M_Sg_N", out forms);
                AdjL_M_Sg_N = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_M_Sg_A_Anim", out forms);
                AdjL_M_Sg_A_Anim = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_M_Sg_A_Inanim", out forms);
                AdjL_M_Sg_A_Inanim = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_M_Sg_G", out forms);
                AdjL_M_Sg_G = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_M_Sg_P", out forms);
                AdjL_M_Sg_P = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_M_Sg_D", out forms);
                AdjL_M_Sg_D = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_M_Sg_I", out forms);
                AdjL_M_Sg_I = Helpers.sListToCommaSeparatedString(forms);

                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_F_Sg_N", out forms);
                AdjL_F_Sg_N = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_F_Sg_A", out forms);
                AdjL_F_Sg_A = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_F_Sg_G", out forms);
                AdjL_F_Sg_G = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_F_Sg_P", out forms);
                AdjL_F_Sg_P = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_F_Sg_D", out forms);
                AdjL_F_Sg_D = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_F_Sg_I", out forms);
                AdjL_F_Sg_I = Helpers.sListToCommaSeparatedString(forms);

                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_N_Sg_N", out forms);
                AdjL_N_Sg_N = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_N_Sg_A", out forms);
                AdjL_N_Sg_A = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_N_Sg_G", out forms);
                AdjL_N_Sg_G = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_N_Sg_P", out forms);
                AdjL_N_Sg_P = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_N_Sg_D", out forms);
                AdjL_N_Sg_D = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_N_Sg_I", out forms);
                AdjL_N_Sg_I = Helpers.sListToCommaSeparatedString(forms);

                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_Pl_N", out forms);
                AdjL_Pl_N = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_Pl_A_Anim", out forms);
                AdjL_Pl_A_Anim = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_Pl_A_Inanim", out forms);
                AdjL_Pl_A_Inanim = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_Pl_G", out forms);
                AdjL_Pl_G = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_Pl_P", out forms);
                AdjL_Pl_P = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_Pl_D", out forms);
                AdjL_Pl_D = Helpers.sListToCommaSeparatedString(forms);
                m_MainModel.GetFormsByGramHash(sLexemeHash, "PronAdj_Pl_I", out forms);
                AdjL_Pl_I = Helpers.sListToCommaSeparatedString(forms);
            }
*/
            else if (lexeme.ePartOfSpeech() == EM_PartOfSpeech.POS_VERB)
            {
                m_bIsDerived = true;
                List<string> forms = null;

                switch (m_eSubparadigm)
                {
                    case EM_Subparadigm.SUBPARADIGM_PART_PRES_ACT:
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_M_Sg_N", out forms);
                        AdjL_M_Sg_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_M_Sg_A_Anim", out forms);
                        AdjL_M_Sg_A_Anim = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_M_Sg_A_Inanim", out forms);
                        AdjL_M_Sg_A_Inanim = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_M_Sg_G", out forms);
                        AdjL_M_Sg_G = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_M_Sg_P", out forms);
                        AdjL_M_Sg_P = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_M_Sg_D", out forms);
                        AdjL_M_Sg_D = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_M_Sg_I", out forms);
                        AdjL_M_Sg_I = Helpers.sListToCommaSeparatedString(forms);

                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_F_Sg_N", out forms);
                        AdjL_F_Sg_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_F_Sg_A", out forms);
                        AdjL_F_Sg_A = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_F_Sg_G", out forms);
                        AdjL_F_Sg_G = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_F_Sg_P", out forms);
                        AdjL_F_Sg_P = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_M_Sg_I", out forms);
                        AdjL_F_Sg_I = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_F_Sg_D", out forms);
                        AdjL_F_Sg_D = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_F_Sg_I", out forms);
                        AdjL_F_Sg_I = Helpers.sListToCommaSeparatedString(forms);

                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_N_Sg_N", out forms);
                        AdjL_N_Sg_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_N_Sg_A", out forms);
                        AdjL_N_Sg_A = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_N_Sg_G", out forms);
                        AdjL_N_Sg_G = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_N_Sg_P", out forms);
                        AdjL_N_Sg_P = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_N_Sg_D", out forms);
                        AdjL_N_Sg_D = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_N_Sg_I", out forms);
                        AdjL_N_Sg_I = Helpers.sListToCommaSeparatedString(forms);

                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_Pl_N", out forms);
                        AdjL_Pl_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_Pl_A_Anim", out forms);
                        AdjL_Pl_A_Anim = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_Pl_A_Inanim", out forms);
                        AdjL_Pl_A_Inanim = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_Pl_G", out forms);
                        AdjL_Pl_G = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_Pl_P", out forms);
                        AdjL_Pl_P = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_Pl_D", out forms);
                        AdjL_Pl_D = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresA_Pl_I", out forms);
                        AdjL_Pl_I = Helpers.sListToCommaSeparatedString(forms);

                        break;

                    case EM_Subparadigm.SUBPARADIGM_PART_PAST_ACT:

                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_M_Sg_N", out forms);
                        AdjL_M_Sg_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_M_Sg_A_Anim", out forms);
                        AdjL_M_Sg_A_Anim = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_M_Sg_A_Inanim", out forms);
                        AdjL_M_Sg_A_Inanim = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_M_Sg_G", out forms);
                        AdjL_M_Sg_G = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_M_Sg_P", out forms);
                        AdjL_M_Sg_P = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_M_Sg_D", out forms);
                        AdjL_M_Sg_D = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_M_Sg_I", out forms);
                        AdjL_M_Sg_I = Helpers.sListToCommaSeparatedString(forms);

                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_F_Sg_N", out forms);
                        AdjL_F_Sg_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_F_Sg_A", out forms);
                        AdjL_F_Sg_A = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_F_Sg_G", out forms);
                        AdjL_F_Sg_G = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_F_Sg_P", out forms);
                        AdjL_F_Sg_P = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_F_Sg_D", out forms);
                        AdjL_F_Sg_D = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_F_Sg_I", out forms);
                        AdjL_F_Sg_I = Helpers.sListToCommaSeparatedString(forms);

                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_N_Sg_N", out forms);
                        AdjL_N_Sg_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_N_Sg_A", out forms);
                        AdjL_N_Sg_A = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_N_Sg_G", out forms);
                        AdjL_N_Sg_G = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_N_Sg_P", out forms);
                        AdjL_N_Sg_P = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_N_Sg_D", out forms);
                        AdjL_N_Sg_D = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_N_Sg_I", out forms);
                        AdjL_N_Sg_I = Helpers.sListToCommaSeparatedString(forms);

                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_Pl_N", out forms);
                        AdjL_Pl_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_Pl_A_Anim", out forms);
                        AdjL_Pl_A_Anim = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_Pl_A_Inanim", out forms);
                        AdjL_Pl_A_Inanim = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_Pl_G", out forms);
                        AdjL_Pl_G = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_Pl_P", out forms);
                        AdjL_Pl_P = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_Pl_D", out forms);
                        AdjL_Pl_D = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastA_Pl_I", out forms);
                        AdjL_Pl_I = Helpers.sListToCommaSeparatedString(forms);

                        break;

                    case EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG:

                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_M_Sg_N", out forms);
                        AdjL_M_Sg_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_M_Sg_A_Anim", out forms);
                        AdjL_M_Sg_A_Anim = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_M_Sg_A_Inanim", out forms);
                        AdjL_M_Sg_A_Inanim = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_M_Sg_G", out forms);
                        AdjL_M_Sg_G = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_M_Sg_P", out forms);
                        AdjL_M_Sg_P = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_M_Sg_D", out forms);
                        AdjL_M_Sg_D = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_M_Sg_I", out forms);
                        AdjL_M_Sg_I = Helpers.sListToCommaSeparatedString(forms);

                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_F_Sg_N", out forms);
                        AdjL_F_Sg_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_F_Sg_A", out forms);
                        AdjL_F_Sg_A = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_F_Sg_G", out forms);
                        AdjL_F_Sg_G = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_F_Sg_P", out forms);
                        AdjL_F_Sg_P = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_F_Sg_D", out forms);
                        AdjL_F_Sg_D = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_F_Sg_I", out forms);
                        AdjL_F_Sg_I = Helpers.sListToCommaSeparatedString(forms);

                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_N_Sg_N", out forms);
                        AdjL_N_Sg_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_N_Sg_A", out forms);
                        AdjL_N_Sg_A = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_N_Sg_G", out forms);
                        AdjL_N_Sg_G = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_N_Sg_P", out forms);
                        AdjL_N_Sg_P = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_N_Sg_D", out forms);
                        AdjL_N_Sg_D = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_N_Sg_I", out forms);
                        AdjL_N_Sg_I = Helpers.sListToCommaSeparatedString(forms);

                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_Pl_N", out forms);
                        AdjL_Pl_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_Pl_A_Anim", out forms);
                        AdjL_Pl_A_Anim = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_Pl_A_Inanim", out forms);
                        AdjL_Pl_A_Inanim = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_Pl_G", out forms);
                        AdjL_Pl_G = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_Pl_P", out forms);
                        AdjL_Pl_P = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_Pl_D", out forms);
                        AdjL_Pl_D = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPL_Pl_I", out forms);
                        AdjL_Pl_I = Helpers.sListToCommaSeparatedString(forms);

                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPS_M", out forms);
                        AdjS_M = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPS_F", out forms);
                        AdjS_F = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPS_N", out forms);
                        AdjS_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPS_Pl", out forms);
                        AdjS_Pl = Helpers.sListToCommaSeparatedString(forms);
                        break;

                    case EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_SHORT:

                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPS_M", out forms);
                        AdjS_M = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPS_F", out forms);
                        AdjS_F = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPS_N", out forms);
                        AdjS_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPresPS_Pl", out forms);
                        AdjS_Pl = Helpers.sListToCommaSeparatedString(forms);

                        break;

                    case EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG:

                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_M_Sg_N", out forms);
                        AdjL_M_Sg_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_M_Sg_A_Anim", out forms);
                        AdjL_M_Sg_A_Anim = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_M_Sg_A_Inanim", out forms);
                        AdjL_M_Sg_A_Inanim = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_M_Sg_G", out forms);
                        AdjL_M_Sg_G = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_M_Sg_P", out forms);
                        AdjL_M_Sg_P = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_M_Sg_D", out forms);
                        AdjL_M_Sg_D = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_M_Sg_I", out forms);
                        AdjL_M_Sg_I = Helpers.sListToCommaSeparatedString(forms);

                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_F_Sg_N", out forms);
                        AdjL_F_Sg_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_F_Sg_A", out forms);
                        AdjL_F_Sg_A = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_F_Sg_G", out forms);
                        AdjL_F_Sg_G = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_F_Sg_P", out forms);
                        AdjL_F_Sg_P = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_F_Sg_D", out forms);
                        AdjL_F_Sg_D = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_F_Sg_I", out forms);
                        AdjL_F_Sg_I = Helpers.sListToCommaSeparatedString(forms);

                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_N_Sg_N", out forms);
                        AdjL_N_Sg_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_N_Sg_A", out forms);
                        AdjL_N_Sg_A = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_N_Sg_G", out forms);
                        AdjL_N_Sg_G = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_N_Sg_P", out forms);
                        AdjL_N_Sg_P = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_N_Sg_D", out forms);
                        AdjL_N_Sg_D = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_N_Sg_I", out forms);
                        AdjL_N_Sg_I = Helpers.sListToCommaSeparatedString(forms);

                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_Pl_N", out forms);
                        AdjL_Pl_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_Pl_A_Anim", out forms);
                        AdjL_Pl_A_Anim = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_Pl_A_Inanim", out forms);
                        AdjL_Pl_A_Inanim = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_Pl_G", out forms);
                        AdjL_Pl_G = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_Pl_P", out forms);
                        AdjL_Pl_P = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_Pl_D", out forms);
                        AdjL_Pl_D = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPL_Pl_I", out forms);
                        AdjL_Pl_I = Helpers.sListToCommaSeparatedString(forms);

                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPS_M", out forms);
                        AdjS_M = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPS_F", out forms);
                        AdjS_F = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPS_N", out forms);
                        AdjS_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPS_Pl", out forms);
                        AdjS_Pl = Helpers.sListToCommaSeparatedString(forms);

                        break;

                    case EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_SHORT:

                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPS_M", out forms);
                        AdjS_M = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPS_F", out forms);
                        AdjS_F = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPS_N", out forms);
                        AdjS_N = Helpers.sListToCommaSeparatedString(forms);
                        m_MainModel.GetFormsByGramHash(sLexemeHash, "PPastPS_Pl", out forms);
                        AdjS_Pl = Helpers.sListToCommaSeparatedString(forms);

                        break;

                    default:
                        MessageBox.Show("Illegal subparadigm.");
                        break;

                }   //  switch (m_eSubparadigm)
            }
            else
            {
                MessageBox.Show("Illegal part of speech.");
            }
        }       //  AdjViewModel()

        public void GoBack(Object obj)
        {
            BackButtonEvent?.Invoke();
        }

        public void EditForm(Object obj)
        {
            var sPropName = obj as string;
            switch (obj as string)
            {
                case "AdjL_M_Sg_N":
                    AdjL_M_Sg_N_IsReadOnly = !AdjL_M_Sg_N_IsReadOnly;
                    break;
                case "AdjL_M_Sg_A_Anim":
                    m_bAdjL_M_Sg_A_Anim_IsReadOnly = !m_bAdjL_M_Sg_A_Anim_IsReadOnly;
                    break;
                case "AdjL_M_Sg_A_Inanim":
                    AdjL_M_Sg_A_Inanim_IsReadOnly = !AdjL_M_Sg_A_Inanim_IsReadOnly;
                    break;
                case "AdjL_M_Sg_G":
                    AdjL_M_Sg_G_IsReadOnly = !AdjL_M_Sg_G_IsReadOnly;
                    break;
                case "AdjL_M_Sg_P":
                    AdjL_M_Sg_P_IsReadOnly = !AdjL_M_Sg_P_IsReadOnly;
                    break;
                case "AdjL_M_Sg_D":
                    AdjL_M_Sg_D_IsReadOnly = !AdjL_M_Sg_D_IsReadOnly;
                    break;
                case "AdjL_M_Sg_I":
                    AdjL_M_Sg_I_IsReadOnly = !AdjL_M_Sg_I_IsReadOnly;
                    break;

                case "AdjL_F_Sg_N":
                    AdjL_F_Sg_N_IsReadOnly = !AdjL_F_Sg_N_IsReadOnly;
                    break;
                case "AdjL_F_Sg_A":
                    AdjL_F_Sg_A_IsReadOnly = !AdjL_F_Sg_A_IsReadOnly;
                    break;
                case "AdjL_F_Sg_G":
                    AdjL_F_Sg_G_IsReadOnly = !AdjL_F_Sg_G_IsReadOnly;
                    break;
                case "AdjL_F_Sg_P":
                    AdjL_F_Sg_P_IsReadOnly = !AdjL_F_Sg_P_IsReadOnly;
                    break;
                case "AdjL_F_Sg_D":
                    AdjL_F_Sg_D_IsReadOnly = !AdjL_F_Sg_D_IsReadOnly;
                    break;
                case "AdjL_F_Sg_I":
                    AdjL_F_Sg_I_IsReadOnly = !AdjL_F_Sg_I_IsReadOnly;
                    break;

                case "AdjL_N_Sg_N":
                    AdjL_N_Sg_N_IsReadOnly = !AdjL_N_Sg_N_IsReadOnly;
                    break;
                case "AdjL_N_Sg_A":
                    AdjL_N_Sg_A_IsReadOnly = !AdjL_N_Sg_A_IsReadOnly;
                    break;
                case "AdjL_N_Sg_G":
                    AdjL_N_Sg_G_IsReadOnly = !AdjL_N_Sg_G_IsReadOnly;
                    break;
                case "AdjL_N_Sg_P":
                    AdjL_N_Sg_P_IsReadOnly = !AdjL_N_Sg_P_IsReadOnly;
                    break;
                case "AdjL_N_Sg_D":
                    AdjL_N_Sg_D_IsReadOnly = !AdjL_N_Sg_D_IsReadOnly;
                    break;
                case "AdjL_N_Sg_I":
                    AdjL_N_Sg_I_IsReadOnly = !AdjL_N_Sg_I_IsReadOnly;
                    break;

                case "AdjL_Pl_N":
                    AdjL_Pl_N_IsReadOnly = !AdjL_Pl_N_IsReadOnly;
                    break;
                case "AdjL_Pl_A_Anim":
                    AdjL_Pl_A_Anim_IsReadOnly = !AdjL_Pl_A_Anim_IsReadOnly;
                    break;
                case "AdjL_Pl_A_Inanim":
                    AdjL_Pl_A_Inanim_IsReadOnly = !AdjL_Pl_A_Inanim_IsReadOnly;
                    break;
                case "AdjL_Pl_G":
                    AdjL_Pl_G_IsReadOnly = !AdjL_Pl_G_IsReadOnly;
                    break;
                case "AdjL_Pl_P":
                    AdjL_Pl_P_IsReadOnly = !AdjL_Pl_P_IsReadOnly;
                    break;
                case "AdjL_Pl_D":
                    AdjL_Pl_D_IsReadOnly = !AdjL_Pl_D_IsReadOnly;
                    break;
                case "AdjL_Pl_I":
                    AdjL_Pl_I_IsReadOnly = !AdjL_Pl_I_IsReadOnly;
                    break;

                case "AdjS_M":
                    AdjS_M_IsReadOnly = !AdjS_M_IsReadOnly;
                    break;
                case "AdjS_F":
                    AdjS_F_IsReadOnly = !AdjS_F_IsReadOnly;
                    break;
                case "AdjS_N":
                    AdjS_N_IsReadOnly = !AdjS_N_IsReadOnly;
                    break;
                case "AdjS_Pl":
                    AdjS_Pl_IsReadOnly = !AdjS_Pl_IsReadOnly;
                    break;

                case "AdjComp":
                    AdjComp_IsReadOnly = !AdjComp_IsReadOnly;
                    break;

                default:
                    MessageBox.Show("Internal error: unknown word form hash.");
                    break;
            }       // switch
        }
}       //  AdjViewModel 

}       //  namespace ZalTestApp
