﻿/*
    Based on Jeremy Alles' blog entry: 
        http://www.japf.fr/2009/03/thinking-with-mvvm-data-templates-contentcontrol/
    
        see also https://joshsmithonwpf.wordpress.com/
 
*/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Windows.Input;
using System.Windows;
using MainLibManaged;

namespace ZalTestApp
{
    /// <summary>
    /// Base class for all ViewModel classes in the application. Provides support for 
    /// property changes notification. Original implementation by Josh Smith.
    /// </summary>
    public abstract class ViewModelBase : INotifyPropertyChanged
    {
        protected MainModel m_MainModel = null;
        protected CLexemeManaged m_Lexeme = null;

        protected Guid m_InstanceId;

        protected Dictionary<string, FormsForGramHash> m_DictFormStatus = new Dictionary<string, FormsForGramHash>();
        //                      ^-- gram hash  --> list of possibly multiple forms for that hash

        public ViewModelBase()
        {
            m_InstanceId = Guid.NewGuid();
        }

        public string InstanceId
        {
            get
            {
                return m_InstanceId.ToString();
            }
        }

        bool m_bIsDerived = false;
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

        protected string GetForm(string sDisplayHash, EM_Subparadigm eSubparadigm)
        {
            //            string sFormHash = sDisplayHashToFormHash(sHash);
            string sFormHash = sDisplayHash;
            if (!m_DictFormStatus.ContainsKey(sFormHash))
            {
                return "";
            }

            var formsForHash = m_DictFormStatus[sFormHash];
            int iAt = formsForHash.iCurrentForm;
            if (iAt < 0 || iAt >= formsForHash.listForms.Count)
            {
                MessageBox.Show("Internal error: Illegal form index.");
                return "Error";
            }

            return formsForHash.listForms[iAt].sFormText;

            //  TODO: comment

        }

        protected void SetForm(string sHash, string sForm)
        {
            if (!m_DictFormStatus.ContainsKey(sHash))
            {
                return;
            }

            Helpers.AssignDiacritics(sForm, ref sForm);

            var formsForHash = m_DictFormStatus[sHash];
            int iAt = formsForHash.iCurrentForm;
            if (iAt < 0 || iAt >= formsForHash.listForms.Count)
            {
                MessageBox.Show("Internal error: Illegal form index.");
                return;
            }

            formsForHash.listForms[iAt].sFormText = sForm;

        }

        public void FormScrollUp(Object obj)
        {
            string sGramHash = obj as string;
            FormsForGramHash formsPerHash;
            if (m_DictFormStatus.TryGetValue(sGramHash, out formsPerHash))
            {
                if (formsPerHash.listForms.Count > 1 && formsPerHash.iCurrentForm > 0)
                {
                    --formsPerHash.iCurrentForm;
                    OnPropertyChanged(sGramHash);
                }
            }
        }

        public void FormScrollDown(Object obj)
        {
            string sGramHash = obj as string;
            FormsForGramHash formsPerHash;
            if (m_DictFormStatus.TryGetValue(sGramHash, out formsPerHash))
            {
                if (formsPerHash.listForms.Count > 1 && formsPerHash.iCurrentForm < formsPerHash.listForms.Count - 1)
                {
                    ++formsPerHash.iCurrentForm;
                    OnPropertyChanged(sGramHash);
                }
            }
        }


        /// <summary>
        /// Raised when a property on this object has a new value.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// Warns the developer if this object does not have a public property with
        /// the specified name. This method does not exist in a Release build.
        /// </summary>
        [Conditional("DEBUG")]
        [DebuggerStepThrough]
        public void VerifyPropertyName(string propertyName)
        {
            // verify that the property name matches a real,  
            // public, instance property on this object.
            if (TypeDescriptor.GetProperties(this)[propertyName] == null)
            {
                Debug.Fail("Invalid property name: " + propertyName);
            }
        }

        /// <summary>
        /// Raises this object's PropertyChanged event.
        /// </summary>
        /// <param name="propertyName">The name of the property that has a new value.</param>
        protected virtual void OnPropertyChanged(string propertyName)
        {
            this.VerifyPropertyName(propertyName);

            if (this.PropertyChanged != null)
            {
                this.PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }

        protected EM_ReturnCode CreateIrregularWordForm(string sGramHash,
                                                        string sForm,
                                                        ref CWordFormManaged wf)
        {
            EM_ReturnCode eRet = EM_ReturnCode.H_NO_ERROR;

            eRet = m_Lexeme.eCreateWordForm(ref wf);
            if (eRet != EM_ReturnCode.H_NO_ERROR)
            {
                MessageBox.Show("Unable to create a word form.");
                return eRet;
            }

            wf.SetPos(m_Lexeme.ePartOfSpeech());

            EM_Number eNumber = EM_Number.NUM_UNDEFINED;
            eRet = Helpers.eGramHashToNumber(sGramHash, ref eNumber);
            if (eRet != EM_ReturnCode.H_NO_ERROR)
            {
                wf.SetNumber(eNumber);
            }

            EM_Case eCase = EM_Case.CASE_UNDEFINED;
            EM_Animacy eAnimacy = EM_Animacy.ANIM_UNDEFINED;
            eRet = Helpers.eGramHashToCase(sGramHash, ref eCase, ref eAnimacy);
            if (eRet == EM_ReturnCode.H_NO_ERROR)
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
                    eSp = EM_Subparadigm.SUBPARADIGM_NUM;
                    break;

                default:
                    MessageBox.Show("Illegal part of speech value.");
                    return EM_ReturnCode.H_ERROR_UNEXPECTED;
            }

            wf.SetSubparadigm(eSp);

            string sOutForm = "";
            Dictionary<int, EM_StressType> dictStressPos;
            Helpers.StressMarksToPosList(sForm, out sOutForm, out dictStressPos);
            wf.SetWordForm(sOutForm);
            eRet = wf.eSetIrregularStressPositions(dictStressPos);
            if (eRet != EM_ReturnCode.H_NO_ERROR)
            {
                var msg = "Internal error: unable to save stress positions";
                MessageBox.Show(msg);
                return eRet;
            }

            return EM_ReturnCode.H_NO_ERROR;

        }       //  CreateIrregularWordForm()

        protected void SaveForms(Object obj)
        {
            EM_ReturnCode eRet = EM_ReturnCode.H_NO_ERROR;

            foreach (KeyValuePair<string, FormsForGramHash> entry in m_DictFormStatus)
            {
                FormsForGramHash formsPerHash = entry.Value;
                if (formsPerHash.listForms.Count < 1)
                {
                    MessageBox.Show("Internal error: no forms for {0}.", entry.Key);
                    continue;
                }

                continue;
/*
                // Purge all irregular forms with this gram hash from the DB
//                string sGramHash = sDisplayHashToFormHash(entry.Key);
                eRet = m_Lexeme.eDeleteIrregularForm(sGramHash);
                if (eRet != EM_ReturnCode.H_NO_ERROR && eRet != EM_ReturnCode.H_FALSE)
                {
                    var msg = "Internal error: unable to save wordform object";
                    MessageBox.Show(msg);
                    continue;
                }

                CWordFormManaged wf = null;
                bool isVariant = false;
                foreach (var descriptor in formsPerHash.listForms)
                {
                    try
                    {
                        string sForm = descriptor.sFormText;
                        eRet = CreateIrregularWordForm(sGramHash, sForm, ref wf);
                        if (eRet != EM_ReturnCode.H_NO_ERROR)
                        {
                            var msg = "Internal error: unable to create word form object.";
                            MessageBox.Show(msg);
                            continue;
                        }

                        wf.SetIsVariant(isVariant);

                        isVariant = true;       // for subsequent forms if they exist

                        // TODO: comments

                        eRet = m_Lexeme.eSaveIrregularForm(wf.sGramHash(), ref wf);
                        if (eRet != EM_ReturnCode.H_NO_ERROR)
                        {
                            var msg = "Internal error: unable to save word form.";
                            MessageBox.Show(msg);
                            continue;
                        }
                    }
                    catch (Exception ex)
                    {
                        var msg = string.Format("Exception: {0}.", ex.Message);
                        MessageBox.Show(msg);
                        return;
                    }
                }
*/
            }       // foreach()
//            MessageBox.Show("Формы сохранены.");


        }       //  SaveForms()

        ////////////////////////////////////////////////////////
        /*
        //       public delegate bool ChangedFormHandler();
                public struct FormDescriptor
                {
                    public List<string> listForms { get; set; }
                    public List<Tuple<string, string>> listComments { get; set; }   // leading / trailing comments, irregular form only
                    //            public bool bCanEdit { get; set; }
        //            public ChangedFormHandler handler { get; set; }
                    public bool IsIrregular { get; set; }
                    public bool IsEdited { get; set; }
                    public int iCurrentFormNumber { get; set; }     // 0 unless we have more than 1 form per gramm hash

        //            public FormDescriptor(List<string> forms, List<Tuple<string, string>> comments, bool bIrregular, bool bEdited, ChangedFormHandler h)
                    public FormDescriptor(List<string> forms, List<Tuple<string, string>> comments, bool bIrregular, bool bEdited)
                    {
                        listForms = forms;
                        listComments = comments;
                        IsIrregular = bIrregular;
                        IsEdited = bEdited;
        //                handler = h;
                        iCurrentFormNumber = 0;
                    }
                }
        */
    }

    public class FormDescriptor
    {
        public string sFormText;
        public string sComment;                            // leading / trailing comments, irregular form only
        public bool HasTrailingComment;
        public bool IsIrregular;
        public bool IsEdited;
//        public EM_Subparadigm eSubparadigm;

        public FormDescriptor()
        {
            sFormText = "";
            sComment = "";
            HasTrailingComment = false;
            IsIrregular = false;
            IsEdited = false;
//            eSubparadigm = EM_Subparadigm.SUBPARADIGM_UNDEFINED;
        }
    }

    // List of forms for given gram hash + sequential number of currently displayed form
    public class FormsForGramHash
    {
        public List<FormDescriptor> listForms;
        public int iCurrentForm;

        public FormsForGramHash()
        {
            listForms = new List<FormDescriptor>();
            iCurrentForm = -1;
        }
    }

    public class RelayCommand : ICommand
    {
        private Action<object> m_Action;
        public RelayCommand(Action<object> action)
        {
            m_Action = action;
        }

        public bool CanExecute(object parameter)
        {
            return true;
        }

        public event EventHandler CanExecuteChanged;
        public void Execute(object parameter)
        {
            if (parameter != null)
            {
                m_Action(parameter);
            }
            else
            {
                m_Action("");
            }
        }
    }

}
