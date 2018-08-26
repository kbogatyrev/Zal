﻿using System;
using System.Windows.Input;
using System.Collections.Generic;

using MainLibManaged;
using System.Windows;
using System.ComponentModel;

namespace ZalTestApp
{
    public class NounViewModel : ViewModelBase
    {
        public delegate void BackButtonHandler();
        public event BackButtonHandler BackButtonEvent;

        public delegate void EditButtonHandler();

        MainModel m_MainModel = null;
        CLexemeManaged m_Lexeme = null;

        private delegate bool ChangedFormHandler();
        struct FormDescriptor
        {
            public List<string> listForms { get; set; }
            public bool bCanEdit { get; set; }
            public ChangedFormHandler handler { get; set; }

            public FormDescriptor(List<string> list, bool b, ChangedFormHandler h)
            {
                listForms = list;
                bCanEdit = b;
                handler = h;
            }
        }

        List<string> m_listPropNamesNoun = new List<string>()
        {
            "Noun_Sg_N", "Noun_Sg_A", "Noun_Sg_G", "Noun_Sg_P", "Noun_Sg_D", "Noun_Sg_L",
            "Noun_Sg_I", "Noun_Sg_Part", "Noun_Pl_N", "Noun_Pl_A", "Noun_Pl_G", "Noun_Pl_P",
            "Noun_Pl_D", "Noun_Pl_L", "Noun_Pl_I"
        };

        List<string> m_listPropNamesPronoun = new List<string>()
        {
            "Pronoun_Sg_N", "Pronoun_Sg_A", "Pronoun_Sg_G", "Pronoun_Sg_P", "Pronoun_Sg_D", "Pronoun_Sg_L",
            "Pronoun_Sg_I", "Pronoun_Sg_Part", "Pronoun_Pl_N", "Pronoun_Pl_A", "Pronoun_Pl_G", "Pronoun_Pl_P",
            "Pronoun_Pl_D", "Pronoun_Pl_L", "Pronoun_Pl_I"
        };

        List<string> m_listPropNamesNumeral = new List<string>()
        {
            "Numeral_Sg_N", "Numeral_Sg_A", "Numeral_Sg_G", "Numeral_Sg_P", "Numeral_Sg_D", "Numeral_Sg_L",
            "Numeral_Sg_I", "Numeral_Sg_Part", "Numeral_Pl_N", "Numeral_Pl_A", "Numeral_Pl_G", "Numeral_Pl_P",
            "Numeral_Pl_D", "Numeral_Pl_L", "Numeral_Pl_I"
        };

        Dictionary<string, List<string>> m_DictOriginalForms = new Dictionary<string, List<string>>();
        Dictionary<string, FormDescriptor> m_DictFormStatus = new Dictionary<string, FormDescriptor>();

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

        private ICommand m_SaveFormsCommand;
        public ICommand SaveFormsCommand
        {
            get
            {
                return m_SaveFormsCommand;
            }
            set
            {
                m_SaveFormsCommand = value;
            }
        }

        #endregion

        #region Bindings_Nouns

        private bool m_bReadOnly;
        public bool ReadOnly
        {
            get
            {
                return m_bReadOnly;
            }
            set
            {
                m_bReadOnly = value;
                OnPropertyChanged("ReadOnly");
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

        public string Noun_Sg_N
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_N"))
                {
                    return "";
                }
                var text = Helpers.sListToCommaSeparatedString(m_DictFormStatus["Noun_Sg_N"].listForms);
                return text;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_N"))
                {
                    return;
                }
                Helpers.AssignDiacritics(value, ref value);
                var fd = m_DictFormStatus["Noun_Sg_N"];
                fd.listForms = Helpers.CommaSeparatedStringToList(value);
                m_DictFormStatus["Noun_Sg_N"] = fd;
                OnPropertyChanged("Noun_Sg_N");
            }
        }

        public bool Noun_Sg_N_IsReadOnly
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_N"))
                {
                    return false;
                }
                return !m_DictFormStatus["Noun_Sg_N"].bCanEdit;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_N"))
                {
                    return;
                }
                var fd = m_DictFormStatus["Noun_Sg_N"];
                fd.bCanEdit = !value;
                m_DictFormStatus["Noun_Sg_N"] = fd;
                OnPropertyChanged("Noun_Sg_N_IsReadOnly");
            }
        }

        public string Noun_Sg_A
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_A"))
                {
                    return "";
                }
                var text = Helpers.sListToCommaSeparatedString(m_DictFormStatus["Noun_Sg_A"].listForms);
                return text;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_A"))
                {
                    return;
                }
                Helpers.AssignDiacritics(value, ref value);
                var fd = m_DictFormStatus["Noun_Sg_A"];
                fd.listForms = Helpers.CommaSeparatedStringToList(value);
                m_DictFormStatus["Noun_Sg_A"] = fd;
                OnPropertyChanged("Noun_Sg_A");
            }
        }

        public bool Noun_Sg_A_IsReadOnly
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_A"))
                {
                    return false;
                }
                return !m_DictFormStatus["Noun_Sg_A"].bCanEdit;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_A"))
                {
                    return;
                }
                var fd = m_DictFormStatus["Noun_Sg_A"];
                fd.bCanEdit = !value;
                m_DictFormStatus["Noun_Sg_A"] = fd;
                OnPropertyChanged("Noun_Sg_A_IsReadOnly");
            }
        }

        public string Noun_Sg_G
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_G"))
                {
                    return "";
                }
                var text = Helpers.sListToCommaSeparatedString(m_DictFormStatus["Noun_Sg_G"].listForms);
                return text;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_G"))
                {
                    return;
                }

                Helpers.AssignDiacritics(value, ref value);
                var fd = m_DictFormStatus["Noun_Sg_G"];
                fd.listForms = Helpers.CommaSeparatedStringToList(value);
                m_DictFormStatus["Noun_Sg_G"] = fd;
                OnPropertyChanged("Noun_Sg_G");
            }
        }

        public bool Noun_Sg_G_IsReadOnly
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_G"))
                {
                    return false;
                }
                return !m_DictFormStatus["Noun_Sg_G"].bCanEdit;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_G"))
                {
                    return;
                }

                var fd = m_DictFormStatus["Noun_Sg_G"];
                fd.bCanEdit = !value;
                m_DictFormStatus["Noun_Sg_G"] = fd;
                OnPropertyChanged("Noun_Sg_G_IsReadOnly");
            }
        }

        public string Noun_Sg_P
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_P"))
                {
                    return "";
                }

                var text = Helpers.sListToCommaSeparatedString(m_DictFormStatus["Noun_Sg_P"].listForms);
                return text;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_P"))
                {
                    return;
                }

                Helpers.AssignDiacritics(value, ref value);
                var fd = m_DictFormStatus["Noun_Sg_P"];
                fd.listForms = Helpers.CommaSeparatedStringToList(value);
                m_DictFormStatus["Noun_Sg_P"] = fd;
                OnPropertyChanged("Noun_Sg_P");
            }
        }

        public bool Noun_Sg_P_IsReadOnly
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_P"))
                {
                    return false;
                }
                return !m_DictFormStatus["Noun_Sg_P"].bCanEdit;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_P"))
                {
                    return;
                }

                var fd = m_DictFormStatus["Noun_Sg_P"];
                fd.bCanEdit = !value;
                m_DictFormStatus["Noun_Sg_P"] = fd;
                OnPropertyChanged("Noun_Sg_P_IsReadOnly");
            }
        }

        public string Noun_Sg_D
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_D"))
                {
                    return "";
                }

                var text = Helpers.sListToCommaSeparatedString(m_DictFormStatus["Noun_Sg_D"].listForms);
                return text;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_D"))
                {
                    return;
                }

                Helpers.AssignDiacritics(value, ref value);
                var fd = m_DictFormStatus["Noun_Sg_D"];
                fd.listForms = Helpers.CommaSeparatedStringToList(value);
                m_DictFormStatus["Noun_Sg_D"] = fd;
                OnPropertyChanged("Noun_Sg_D");
            }
        }

        public bool Noun_Sg_D_IsReadOnly
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_D"))
                {
                    return false;
                }
                return !m_DictFormStatus["Noun_Sg_D"].bCanEdit;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_D"))
                {
                    return;
                }

                var fd = m_DictFormStatus["Noun_Sg_D"];
                fd.bCanEdit = !value;
                m_DictFormStatus["Noun_Sg_D"] = fd;
                OnPropertyChanged("Noun_Sg_D_IsReadOnly");
            }
        }

        public string Noun_Sg_I
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_I"))
                {
                    return "";
                }

                var text = Helpers.sListToCommaSeparatedString(m_DictFormStatus["Noun_Sg_I"].listForms);
                return text;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_I"))
                {
                    return;
                }

                Helpers.AssignDiacritics(value, ref value);
                var fd = m_DictFormStatus["Noun_Sg_I"];
                fd.listForms = Helpers.CommaSeparatedStringToList(value);
                m_DictFormStatus["Noun_Sg_I"] = fd;
                OnPropertyChanged("Noun_Sg_I");
            }
        }

        public bool Noun_Sg_I_IsReadOnly
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_I"))
                {
                    return false;
                }

                return !m_DictFormStatus["Noun_Sg_I"].bCanEdit;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_I"))
                {
                    return;
                }

                var fd = m_DictFormStatus["Noun_Sg_I"];
                fd.bCanEdit = !value;
                m_DictFormStatus["Noun_Sg_I"] = fd;
                OnPropertyChanged("Noun_Sg_I_IsReadOnly");
            }
        }

        public string Noun_Sg_Part
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_Part"))
                {
                    return "";
                }

                var text = Helpers.sListToCommaSeparatedString(m_DictFormStatus["Noun_Sg_Part"].listForms);
                return text;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_Part"))
                {
                    return;
                }

                Helpers.AssignDiacritics(value, ref value);
                var fd = m_DictFormStatus["Noun_Sg_Part"];
                fd.listForms = Helpers.CommaSeparatedStringToList(value);
                m_DictFormStatus["Noun_Sg_Part"] = fd;
                OnPropertyChanged("Noun_Sg_Part");
            }
        }

        public bool Noun_Sg_Part_IsReadOnly
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_Part"))
                {
                    return false;
                }
                return !m_DictFormStatus["Noun_Sg_Part"].bCanEdit;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_Part"))
                {
                    return;
                }

                var fd = m_DictFormStatus["Noun_Sg_Part"];
                fd.bCanEdit = !value;
                m_DictFormStatus["Noun_Sg_Part"] = fd;
                OnPropertyChanged("Noun_Sg_Part_IsReadOnly");
            }
        }

        public string Noun_Sg_L
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_L"))
                {
                    return "";
                }

                var text = Helpers.sListToCommaSeparatedString(m_DictFormStatus["Noun_Sg_L"].listForms);
                return text;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_L"))
                {
                    return;
                }

                Helpers.AssignDiacritics(value, ref value);
                var fd = m_DictFormStatus["Noun_Sg_L"];
                fd.listForms = Helpers.CommaSeparatedStringToList(value);
                m_DictFormStatus["Noun_Sg_L"] = fd;
                OnPropertyChanged("Noun_Sg_L");
            }
        }

        public bool Noun_Sg_L_IsReadOnly
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_L"))
                {
                    return false;
                }

                return !m_DictFormStatus["Noun_Sg_L"].bCanEdit;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Sg_L"))
                {
                    return;
                }

                var fd = m_DictFormStatus["Noun_Sg_L"];
                fd.bCanEdit = !value;
                m_DictFormStatus["Noun_Sg_L"] = fd;
                OnPropertyChanged("Noun_Sg_L_IsReadOnly");
            }
        }

        private bool m_bIs_L2_optional = false;
        public bool Is_L2_optional
        {
            get
            {
                return m_bIs_L2_optional;
            }
            set
            {
                m_bIs_L2_optional = value;
                OnPropertyChanged("Is_L2_optional");
            }
        }

        private bool m_bIs_L2_optional_IsReadOnly = true;
        public bool IsL2_optional_IsReadOnly
        {
            get
            {
                return m_bIs_L2_optional_IsReadOnly;
            }
            set
            {
                m_bIs_L2_optional_IsReadOnly = value;
                OnPropertyChanged("Is_L2_optional_IsReadOnly");
            }
        }

        private string m_sNoun_Sg_L2_Prepositions;
        public string Noun_Sg_L2_Prepositions
        {
            get
            {
                return m_sNoun_Sg_L2_Prepositions;
            }
            set
            {
                m_sNoun_Sg_L2_Prepositions = value;
                OnPropertyChanged("Noun_Sg_L2_Prepositions");
            }
        }

        private bool m_bNoun_Sg_L2_Prepositions_IsReadOnly = true;
        public bool Noun_Sg_L2_Prepositions_IsReadOnly
        {
            get
            {
                return m_bNoun_Sg_L2_Prepositions_IsReadOnly;
            }
            set
            {
                m_bNoun_Sg_L2_Prepositions_IsReadOnly = value;
                OnPropertyChanged("Noun_Sg_L2_Prepositions_IsReadOnly");
            }
        }

        public string Noun_Pl_N
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_N"))
                {
                    return "";
                }

                var text = Helpers.sListToCommaSeparatedString(m_DictFormStatus["Noun_Pl_N"].listForms);
                return text;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_N"))
                {
                    return;
                }

                Helpers.AssignDiacritics(value, ref value);
                var fd = m_DictFormStatus["Noun_Pl_N"];
                fd.listForms = Helpers.CommaSeparatedStringToList(value);
                m_DictFormStatus["Noun_Pl_N"] = fd;
                OnPropertyChanged("Noun_Pl_N");
            }
        }

        public bool Noun_Pl_N_IsReadOnly
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_N"))
                {
                    return false;
                }
                return !m_DictFormStatus["Noun_Pl_N"].bCanEdit;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_N"))
                {

                }
                    var fd = m_DictFormStatus["Noun_Pl_N"];
                fd.bCanEdit = !value;
                m_DictFormStatus["Noun_Pl_N"] = fd;
                OnPropertyChanged("Noun_Pl_N_IsReadOnly");
            }
        }

        public string Noun_Pl_A
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_A"))
                {
                    return "";
                }
                var text = Helpers.sListToCommaSeparatedString(m_DictFormStatus["Noun_Pl_A"].listForms);
                return text;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_A"))
                {
                    return;
                }
                Helpers.AssignDiacritics(value, ref value);
                var fd = m_DictFormStatus["Noun_Pl_A"];
                fd.listForms = Helpers.CommaSeparatedStringToList(value);
                m_DictFormStatus["Noun_Pl_A"] = fd;
                OnPropertyChanged("Noun_Pl_A");
            }
        }

        public bool Noun_Pl_A_IsReadOnly
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_A"))
                {
                    return false;
                }
                return !m_DictFormStatus["Noun_Pl_A"].bCanEdit;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_A"))
                {
                    return;
                }
                var fd = m_DictFormStatus["Noun_Pl_A"];
                fd.bCanEdit = !value;
                m_DictFormStatus["Noun_Pl_A"] = fd;
                OnPropertyChanged("Noun_Pl_A_IsReadOnly");
            }
        }

        public string Noun_Pl_G
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_G"))
                {
                    return "";
                }
                var text = Helpers.sListToCommaSeparatedString(m_DictFormStatus["Noun_Pl_G"].listForms);
                return text;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_G"))
                {
                    return;
                }
                Helpers.AssignDiacritics(value, ref value);
                var fd = m_DictFormStatus["Noun_Pl_G"];
                fd.listForms = Helpers.CommaSeparatedStringToList(value);
                m_DictFormStatus["Noun_Pl_G"] = fd;
                OnPropertyChanged("Noun_Pl_G");
            }
        }

        public bool Noun_Pl_G_IsReadOnly
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_G"))
                {
                    return false;
                }
                return !m_DictFormStatus["Noun_Pl_G"].bCanEdit;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_G"))
                {
                    return;
                }
                var fd = m_DictFormStatus["Noun_Pl_G"];
                fd.bCanEdit = !value;
                m_DictFormStatus["Noun_Pl_G"] = fd;
                OnPropertyChanged("Noun_Pl_G_IsReadOnly");
            }
        }

        public string Noun_Pl_P
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_P"))
                {
                    return "";
                }
                var text = Helpers.sListToCommaSeparatedString(m_DictFormStatus["Noun_Pl_P"].listForms);
                return text;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_P"))
                {
                    return;
                }
                Helpers.AssignDiacritics(value, ref value);
                var fd = m_DictFormStatus["Noun_Pl_P"];
                fd.listForms = Helpers.CommaSeparatedStringToList(value);
                m_DictFormStatus["Noun_Pl_P"] = fd;
                OnPropertyChanged("Noun_Pl_P");
            }
        }

        public bool Noun_Pl_P_IsReadOnly
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_P"))
                {
                    return false;
                }
                return !m_DictFormStatus["Noun_Pl_P"].bCanEdit;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_P"))
                {
                    return;
                }
                var fd = m_DictFormStatus["Noun_Pl_P"];
                fd.bCanEdit = !value;
                m_DictFormStatus["Noun_Pl_P"] = fd;
                OnPropertyChanged("Noun_Pl_P_IsReadOnly");
            }
        }

        public string Noun_Pl_D
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_D"))
                {
                    return "";
                }
                var text = Helpers.sListToCommaSeparatedString(m_DictFormStatus["Noun_Pl_D"].listForms);
                return text;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_D"))
                {
                    return;
                }
                Helpers.AssignDiacritics(value, ref value);
                var fd = m_DictFormStatus["Noun_Pl_D"];
                fd.listForms = Helpers.CommaSeparatedStringToList(value);
                m_DictFormStatus["Noun_Pl_D"] = fd;
                OnPropertyChanged("Noun_Pl_D");
            }
        }

        public bool Noun_Pl_D_IsReadOnly
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_D"))
                {
                    return false;
                }

                return !m_DictFormStatus["Noun_Pl_D"].bCanEdit;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_D"))
                {
                    return;
                }
                var fd = m_DictFormStatus["Noun_Pl_D"];
                fd.bCanEdit = !value;
                m_DictFormStatus["Noun_Pl_D"] = fd;
                OnPropertyChanged("Noun_Pl_D_IsReadOnly");
            }
        }

        public string Noun_Pl_I
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_I"))
                {
                    return "";
                }
                var text = Helpers.sListToCommaSeparatedString(m_DictFormStatus["Noun_Pl_I"].listForms);
                return text;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_I"))
                {
                    return;
                }
                Helpers.AssignDiacritics(value, ref value);
                var fd = m_DictFormStatus["Noun_Pl_I"];
                fd.listForms = Helpers.CommaSeparatedStringToList(value);
                m_DictFormStatus["Noun_Pl_I"] = fd;
                OnPropertyChanged("Noun_Pl_I");
            }
        }

        public bool Noun_Pl_I_IsReadOnly
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_I"))
                {
                    return false;
                }
                return !m_DictFormStatus["Noun_Pl_I"].bCanEdit;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_I"))
                {
                    return;
                }
                var fd = m_DictFormStatus["Noun_Pl_I"];
                fd.bCanEdit = !value;
                m_DictFormStatus["Noun_Pl_I"] = fd;
                OnPropertyChanged("Noun_Pl_I_IsReadOnly");
            }
        }

        public string Noun_Pl_L
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_L"))
                {
                    return "";
                }
                var text = Helpers.sListToCommaSeparatedString(m_DictFormStatus["Noun_Pl_L"].listForms);
                return text;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_L"))
                {
                    return;
                }
                Helpers.AssignDiacritics(value, ref value);
                var fd = m_DictFormStatus["Noun_Pl_L"];
                fd.listForms = Helpers.CommaSeparatedStringToList(value);
                m_DictFormStatus["Noun_Pl_L"] = fd;
                OnPropertyChanged("Noun_Pl_L");
            }
        }

        public bool Noun_Pl_L_IsReadOnly
        {
            get
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_L"))
                {
                    return false;
                }
                return !m_DictFormStatus["Noun_Pl_L"].bCanEdit;
            }
            set
            {
                if (!m_DictFormStatus.ContainsKey("Noun_Pl_L"))
                {
                    return;
                }
                var fd = m_DictFormStatus["Noun_Pl_L"];
                fd.bCanEdit = !value;
                m_DictFormStatus["Noun_Pl_L"] = fd;
                OnPropertyChanged("Noun_Pl_L_IsReadOnly");
            }
        }

        private bool m_bEditEnabled = false;
        public bool EditEnabled
        {
            get
            {
                return m_bEditEnabled;
            }
            set
            {
                m_bEditEnabled = value;
                OnPropertyChanged("EditEnabled");
            }
        }

        #endregion

        #region Property_Delegates

        private bool InitFormHandlers()
        {
            try
            {
                string sLexemeHash = m_Lexeme.sHash();
                List<string> listKeys = null;

                switch (m_Lexeme.ePartOfSpeech())
                {
                    case EM_PartOfSpeech.POS_NOUN:
                        listKeys = m_listPropNamesNoun;
                        break;

                    case EM_PartOfSpeech.POS_PRONOUN:
                        listKeys = m_listPropNamesPronoun;
                        break;

                    case EM_PartOfSpeech.POS_NUM:
                        listKeys = m_listPropNamesNumeral;
                        break;

                    default:
                        MessageBox.Show("Illegal part of speech value.");
                        return false;
                }

                if (null == listKeys)
                {
                    MessageBox.Show("Internal error: unable to determine gram hashes.");
                }

                foreach (var sHash in listKeys)
                {
                    //  FormDescriptor fd = m_DictFormStatus[sHash];
                    FormDescriptor fd = new FormDescriptor(null, false, null);
                    List<string> listForms = null;
                    m_MainModel.GetFormsByGramHash(sLexemeHash, sHash, out listForms);
                    fd.listForms = listForms;
                    fd.handler = () =>
                    {
                        FormDescriptor fd1 = m_DictFormStatus[sFormHashToDisplayHash(sHash)];
                        if (!fd1.bCanEdit)
                        {
                            return true;
                        }

                        var sFormString = Helpers.sListToCommaSeparatedString(fd1.listForms);
                        Helpers.AssignDiacritics(sFormString, ref sFormString);
                    //                    OnPropertyChanged(hash);
                    return true;
                    };

                    m_DictFormStatus[sFormHashToDisplayHash(sHash)] = fd;
                    m_DictOriginalForms[sFormHashToDisplayHash(sHash)] = listForms;
                }
            }
            catch (Exception ex)
            {
                var msg = "Internal error: unable to initiate form handlers: ";
                msg += ex.Message;
                MessageBox.Show(msg);
                return false;
            }

            return true;

        }   //  InitFormHandlers()

        #endregion

        public NounViewModel(CLexemeManaged lexeme, MainModel m)
        {
            BackCommand = new RelayCommand(new Action<object>(GoBack));
            EditCommand = new RelayCommand(new Action<object>(EditForm));
            SaveFormsCommand = new RelayCommand(new Action<object>(SaveForms));

            m_MainModel = m;

            m_Lexeme = lexeme;

            InitFormHandlers();

            PropertyChanged += nounViewModel_PropertyChanged;

            EditEnabled = true;

        }

        public void GoBack(Object obj)
        {
            BackButtonEvent?.Invoke();
        }

        public void EditForm(Object obj)
        {
            var sPropName = obj as string;

            try
            {
                var fd = m_DictFormStatus[sPropName];
                fd.bCanEdit = !fd.bCanEdit;
                m_DictFormStatus[sPropName] = fd;
                sPropName += "_IsReadOnly";
                OnPropertyChanged(sPropName);
            }
            catch (Exception ex)
            {
                var msg = "Internal error: unable to invoke word form change handler: ";
                msg += ex.Message;
                MessageBox.Show(msg);
            }
        }       //  EditForm()

        public void SaveForms(Object obj)
        {
            foreach (KeyValuePair<string, List<string>> entry in m_DictOriginalForms)
            {
                EM_ReturnCode eRet = EM_ReturnCode.H_NO_ERROR;

                List<string> originalForms = entry.Value;
                FormDescriptor formDescriptor;
                if (m_DictFormStatus.TryGetValue(entry.Key, out formDescriptor))
                {
                    List<string> changedForms = formDescriptor.listForms;
                    if (changedForms != originalForms)
                    {
                        foreach (string sForm in changedForms)
                        {
                            var idx = changedForms.IndexOf(sForm);
                            if (idx < 0)
                            {
                                var msg = "Internal error: form index out of range";
                                MessageBox.Show(msg);
                                return;
                            }

//                            string sFormHash = sDisplayHashToFormHash(entry.Key);
//                            if (sFormHash.Length < 1)
//                            {
//                                continue;
//                            }

                            CWordFormManaged wf = null;
                            if (null == originalForms)
                            {
                                eRet = m_Lexeme.eCreateWordForm(ref wf);
                                if (eRet != EM_ReturnCode.H_NO_ERROR)
                                {
                                    MessageBox.Show("Unable to create a word form.");
                                    continue;
                                }

                                wf.SetPos(m_Lexeme.ePartOfSpeech());

                                EM_Number eNumber = EM_Number.NUM_UNDEFINED;
                                eRet = Helpers.eGramHashToNumber(entry.Key, ref eNumber);
                                if (eRet != EM_ReturnCode.H_NO_ERROR)
                                {
                                    wf.SetNumber(eNumber);
                                }

                                EM_Case eCase = EM_Case.CASE_UNDEFINED;
                                EM_Animacy eAnimacy = EM_Animacy.ANIM_UNDEFINED;
                                eRet = Helpers.eGramHashToCase(entry.Key, ref eCase, ref eAnimacy);
                                if (eRet != EM_ReturnCode.H_NO_ERROR)
                                {
                                    wf.SetCase(eCase);
                                    wf.SetAnimacy(eAnimacy);
                                }

                                EM_Subparadigm eSp = EM_Subparadigm.SUBPARADIGM_UNDEFINED;

                                switch (m_Lexeme.ePartOfSpeech())
                                {
                                    case EM_PartOfSpeech.POS_NOUN:
                                        eSp = EM_Subparadigm.SUBPARADIGM_NOUN;
                                        break;

                                    case EM_PartOfSpeech.POS_PRONOUN:
                                        eSp = EM_Subparadigm.SUBPARADIGM_PRONOUN;
                                        break;

                                    case EM_PartOfSpeech.POS_NUM:
                                        eSp = EM_Subparadigm.SUBPARADIGM_NUMERAL;
                                        break;

                                    default:
                                        MessageBox.Show("Illegal part of speech value.");
                                        continue;
                                }

                                wf.SetSubparadigm(eSp);
                            }
                            else
                            {
                                eRet = m_Lexeme.eWordFormFromHash(entry.Key, changedForms.IndexOf(sForm), ref wf);
                                if (eRet != EM_ReturnCode.H_NO_ERROR)
                                {
                                    var msg = "Internal error: unable to create wordform object";
                                    MessageBox.Show(msg);
                                    return;
                                }
                            }

                            string sOutForm = "";
                            Dictionary<int, EM_StressType> dictStressPos;
                            Helpers.StressMarksToPosList(sForm, out sOutForm, out dictStressPos);
                            wf.SetWordForm(sOutForm);
                            eRet = wf.eSetIrregularStressPositions(dictStressPos);
                            if (eRet != EM_ReturnCode.H_NO_ERROR)
                            {
                                var msg = "Internal error: unable to save stress positions";
                                MessageBox.Show(msg);
                                return;
                            }
                            eRet = wf.eSaveIrregularForm();
                            if (eRet != EM_ReturnCode.H_NO_ERROR)
                            {
                                var msg = "Internal error: unable to save wordform object";
                                MessageBox.Show(msg);
                                return;
                            }
                        }
                    }
                }
            }       // foreach ...

            foreach (string sKey in m_DictFormStatus.Keys)
            {
                List<string> originalForms;
                if (m_DictOriginalForms.TryGetValue(sKey, out originalForms))
                {
                    FormDescriptor formDescriptor;
                    if (m_DictFormStatus.TryGetValue(sKey, out formDescriptor))
                    {
                        List<string> changedForms = formDescriptor.listForms;
                        if (changedForms != originalForms)
                        {
                            m_DictOriginalForms[sKey] = changedForms;
                        }
                    }
                }
            }           // foreach

            MessageBox.Show("Формы сохранены.");

        }       //  SaveForms()

        public void nounViewModel_PropertyChanged(object sender, PropertyChangedEventArgs arg)
        {
            var sFormHash = arg.PropertyName.ToString();
            if (null == sFormHash || 0 == sFormHash.Length)
            {
                return;
            }

            if (!m_DictFormStatus.ContainsKey(sFormHash))
            {
                return;
            }

            try
            {
                ChangedFormHandler handler = null;
                FormDescriptor fd = m_DictFormStatus[sFormHash];
                handler = fd.handler;
                var ret = handler();
            }
            catch (Exception ex)
            {
                var msg = "Internal error: unable to invoke word form change handler: ";
                msg += ex.Message;
                MessageBox.Show(msg);
            }
        }

        private string sFormHashToDisplayHash(string sFormHash)
        {
            int iKeyIdx = -1;

            try
            {
                switch (m_Lexeme.ePartOfSpeech())
                {
                    case EM_PartOfSpeech.POS_NOUN:
                        iKeyIdx = m_listPropNamesNoun.IndexOf(sFormHash);
                        break;

                    case EM_PartOfSpeech.POS_PRONOUN:
                        iKeyIdx = m_listPropNamesPronoun.IndexOf(sFormHash);
                        break;

                    case EM_PartOfSpeech.POS_NUM:
                        iKeyIdx = m_listPropNamesNumeral.IndexOf(sFormHash);
                        break;

                    default:
                        string sMsg = "Part of speech :";
                        sMsg += m_Lexeme.ePartOfSpeech();
                        sMsg += " was not recognized. ";
                        MessageBox.Show(sMsg);
                        return "";
                }

                return m_listPropNamesNoun[iKeyIdx];
            }
            catch (Exception ex)
            {
                string sMsg = "Exception while retrieving form hash: ";
                sMsg += ex.Message;
                MessageBox.Show(sMsg);
                return "";
            }

            return "";

        }       //  sFormHashToDisplayHash()

        private string sDisplayHashToFormHash(string sDisplayHash)
        {
            int iKeyIdx = m_listPropNamesNoun.IndexOf(sDisplayHash);
            if (iKeyIdx < 0)
            {
                string sMsg = $"Display hash {iKeyIdx} not recognized.";
                MessageBox.Show(sMsg);
                return "";
            }

            string sFormHash = "";
            try
            {
                switch (m_Lexeme.ePartOfSpeech())
                {
                    case EM_PartOfSpeech.POS_NOUN:
                        sFormHash = m_listPropNamesNoun[iKeyIdx];
                        break;

                    case EM_PartOfSpeech.POS_PRONOUN:
                        sFormHash = m_listPropNamesPronoun[iKeyIdx];
                        break;

                    case EM_PartOfSpeech.POS_NUM:
                        sFormHash = m_listPropNamesNumeral[iKeyIdx];
                        break;

                    default:
                        string sMsg = "Part of speech :";
                        sMsg += m_Lexeme.ePartOfSpeech();
                        sMsg += " was not recognized. ";
                        MessageBox.Show(sMsg);
                        return "";
                }

                return sFormHash;
            }
            catch (Exception ex)
            {
                string sMsg = "Exception while retrieving form hash: ";
                sMsg += ex.Message;
                MessageBox.Show(sMsg);
                return "";
            }

            return "";

        }       //  sDisplayHashToFormHash()

    }       //  public class NounViewModel ...
}
