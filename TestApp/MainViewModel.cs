﻿using System;
using System.Windows;
using System.Windows.Input;
using System.Collections.Generic;
using Microsoft.Win32;
using System.Collections.ObjectModel;

using MainLibManaged;
using System.Threading;

namespace ZalTestApp
{
    public class ViewPage
    {
        public ViewPage(string sHeader, ViewModelBase lexeme, ViewModelBase page)
        {
            m_sHeader = sHeader;
            m_LexemeInfo = lexeme;
            m_Page = page;
        }

        string m_sHeader;
        ViewModelBase m_LexemeInfo;
        ViewModelBase m_Page;

        public string Header
        {
            get
            {
                return m_sHeader;
            }
        }

        public ViewModelBase Page
        {
            get
            {
                return m_Page;
            }
        }

        public ViewModelBase LexemeInfo
        {
            get
            {
                return m_LexemeInfo;
            }
        }

        public string Type
        {
            get
            {
                return "Lexeme";
            }
        }
    }

    public class MainViewModel : ViewModelBase
    {
        private ProgressViewModel m_ProgressViewModel = null;

        private LinkedList<ViewModelBase> m_BreadCrumbs = null;

        private ObservableCollection<ViewPage> m_Pages = new ObservableCollection<ViewPage>();
        public ObservableCollection<ViewPage> Pages
        {
            get
            {
                return m_Pages;
            }
        }

        private LinkedListNode<ViewModelBase> m_CurrentViewModel = null;
        public ViewModelBase CurrentViewModel
        {
            get
            {
                return m_CurrentViewModel.Value;
            }

//            set
//            {
//                m_CurrentViewModel.Value = value;
//            }
        }

        LexemeViewModel m_CurrentLexeme;
        public LexemeViewModel CurrentLexeme
        {
            get
            {
                return m_CurrentLexeme;
            }

            //            set
            //            {
            //                m_CurrentViewModel.Value = value;
            //            }
        }

        private ViewPage m_CurrentViewPage;
        public ViewPage CurrentViewPage
        {
            get
            {
                return m_CurrentViewPage;
            }

            set
            {
                m_CurrentViewPage = value;
            }
        }

        public string CurrentHeader
        {
            get
            {
                if (m_CurrentViewPage != null)
                {
                    return m_CurrentViewPage.Header;
                }
                else
                {
                    MessageBox.Show("Internal error: current page is null.");
                    return "";
                }
            }
        }

        public ViewModelBase LexemeInfo
        {
            get
            {
                return m_CurrentViewPage.LexemeInfo;
            }
        }

        public string Type
        {
            get
            {
                return "Lexeme";
            }
        }

        private int m_iCurrentTab;
        public int CurrentTab
        {
            get
            {
                return m_iCurrentTab;
            }
            set
            {
                m_iCurrentTab = value;
            }
        }

        public bool IsBackButtonEnabled
        {
            get
            {
                if (null == m_CurrentViewModel)
                {
                    return false;
                }

                if (null == m_CurrentViewModel.Previous)
                {
                    return false;
                }

                return true;
            }
        }

        public bool IsForwardButtonEnabled
        {
            get
            {
                if (null == m_CurrentViewModel)
                {
                    return false;
                }

                if (null == m_CurrentViewModel.Next)
                {
                    return false;
                }

                return true;
            }
        }

        private MainModel m_MainModel;

        #region Properties

        private bool m_bDbOpen;
        public bool DbOpen
        {
            get
            {
                return m_bDbOpen;
            }
            set
            {
                m_bDbOpen = value;
                OnPropertyChanged("DbOpen");
            }
        }

        #endregion

        #region ICommand

        private RelayCommand closeCommand;
        public RelayCommand CloseCommand
        {
            get
            {
                if (this.closeCommand == null)
                {
                    this.closeCommand = new RelayCommand(w => CloseCommandMethod(w));
                }
                return this.closeCommand;
            }
        }

        private ICommand m_BackButtonCommand;
        public ICommand BackButtonCommand
        {
            get
            {
                return m_BackButtonCommand;
            }
            set
            {
                m_BackButtonCommand = value;
            }
        }

        private ICommand m_ForwardButtonCommand;
        public ICommand ForwardButtonCommand
        {
            get
            {
                return m_ForwardButtonCommand;
            }
            set
            {
                m_ForwardButtonCommand = value;
            }
        }

        private ICommand m_OpenDictionaryCommand;
        public ICommand OpenDictionaryCommand
        {
            get
            {
                return m_OpenDictionaryCommand;
            }
            set
            {
                m_OpenDictionaryCommand = value;
            }
        }

        private ICommand m_SearchByInitialFormCommand;
        public ICommand SearchByInitialFormCommand
        {
            get
            {
                return m_SearchByInitialFormCommand;
            }
            set
            {
                m_SearchByInitialFormCommand = value;
            }
        }

        private ICommand m_ShowRegressionPageCommand;
        public ICommand ShowRegressionPageCommand
        {
            get
            {
                return m_ShowRegressionPageCommand;
            }
            set
            {
                m_ShowRegressionPageCommand = value;
            }
        }

        private ICommand m_ImportRegressionDataCommand;
        public ICommand ImportRegressionDataCommand
        {
            get
            {
                return m_ImportRegressionDataCommand;
            }
            set
            {
                m_ImportRegressionDataCommand = value;
            }
        }

        private ICommand m_ExportRegressionDataCommand;
        public ICommand ExportRegressionDataCommand
        {
            get
            {
                return m_ExportRegressionDataCommand;
            }
            set
            {
                m_ExportRegressionDataCommand = value;
            }
        }
        #endregion

        public MainViewModel()
        {
            m_Pages = new ObservableCollection<ViewPage>();
            BackButtonCommand = new RelayCommand(new Action<object>(GoBack));
            ForwardButtonCommand = new RelayCommand(new Action<object>(GoForward));
            OpenDictionaryCommand = new RelayCommand(new Action<object>(OpenDictionary));
            SearchByInitialFormCommand = new RelayCommand(new Action<object>(SearchByInitialForm));
            ShowRegressionPageCommand = new RelayCommand(new Action<object>(ShowRegression));
            ImportRegressionDataCommand = new RelayCommand(new Action<object>(ImportRegressionData));
            ExportRegressionDataCommand = new RelayCommand(new Action<object>(ExportRegressionData));

            m_MainModel = new MainModel();
            m_MainModel.Path = "";
            m_BreadCrumbs = new LinkedList<ViewModelBase>();
//            m_LexemeGridViewModel = new ViewPage ("парадигмы", new LexemeGridViewModel());
//            m_Pages.Add(m_LexemeGridViewModel);
//            m_iSelectedIndex = 0;
//            m_CurrentViewModel = m_BreadCrumbs.AddLast(m_LexemeGridViewModel.Page);

            m_bDbOpen = false;
        }

        public void GoBack(object obj)
        {
            if (null == m_BreadCrumbs)
            {
                return;
            }

            if (m_BreadCrumbs.Count == 0)
            {
                return;
            }

            if (null == m_CurrentViewModel)
            {
                return;
            }

            if (null == m_CurrentViewModel.Previous)
            {
                return;
            }

            m_CurrentViewModel = m_CurrentViewModel.Previous;
            UpdateView();
        }

        public void GoForward(object obj)
        {
            if (null == m_BreadCrumbs)
            {
                return;
            }

            if (m_BreadCrumbs.Count == 0)
            {
                return;
            }

            if (null == m_CurrentViewModel)
            {
                return;
            }

            if (null == m_CurrentViewModel.Next)
            {
                return;
            }

            m_CurrentViewModel = m_CurrentViewModel.Next;
            UpdateView();
        }

        public void OpenDictionary(object obj)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();

            openFileDialog.InitialDirectory = "c:\\";
            openFileDialog.Filter = "All files (*.*)|*.*|SQLite 3 files (*.db3)|*.db3";
            openFileDialog.FilterIndex = 2;
            openFileDialog.RestoreDirectory = true;

            EM_ReturnCode eRet = EM_ReturnCode.H_NO_ERROR;
            if (true == openFileDialog.ShowDialog())
            {
                eRet = m_MainModel.OpenDictionary(openFileDialog.FileName);
            }

            if (EM_ReturnCode.H_NO_ERROR == eRet)
            {
                DbOpen = true;
            }
        }

        public void SearchByInitialForm(object obj)
        {
            EnterDataDlg edd = new EnterDataDlg();
            edd.Owner = Application.Current.MainWindow;
            bool? bnRet = edd.ShowDialog();
            if (bnRet != true)
            {
                return;
            }

            EnterDataViewModel eddvm = (EnterDataViewModel)edd.DataContext;
            m_MainModel.SearchByInitialForm(eddvm.DataString);
            if (m_MainModel.NLexemes < 1)
            {
                return;
            }

            foreach (string sLexemeHash in m_MainModel)
            {
                CLexemeManaged lexeme = m_MainModel.LexemeFromHash(sLexemeHash);
                if (null == lexeme)
                {
                    MessageBox.Show("Internal error: lexeme descriptor is corrupt.");
                    return;
                }

                bool bIsNewLexeme = true;
                foreach (ViewPage page in m_Pages)
                {
                    LexemeViewModel knownLvm = (LexemeViewModel)page.LexemeInfo;
                    if (knownLvm.Lexeme == null)
                    {
                        continue;
                    }

                    if (lexeme.llLexemeId() == knownLvm.Lexeme.llLexemeId())
                    {
                        bIsNewLexeme = false;
                        break;
                    }
                }

                if (!bIsNewLexeme)
                {
                    continue;
                }

                LexemeViewModel lexemeViewModel = new LexemeViewModel(lexeme);
//                lvm.RemoveLexemeEvent += new LexemeViewModel.RemoveLexemeHandler(RemoveLexeme);
                m_CurrentLexeme = lexemeViewModel;
                ViewModelBase paradigmViewModel = null;

                switch (lexeme.ePartOfSpeech())
                {
                    case EM_PartOfSpeech.POS_NOUN:
                        paradigmViewModel = new NounViewModel(lexeme, m_MainModel);
                        break;
                    case EM_PartOfSpeech.POS_ADJ:
                        paradigmViewModel = new AdjViewModel(lexeme, EM_Subparadigm.SUBPARADIGM_LONG_ADJ, m_MainModel);
                        break;
                    case EM_PartOfSpeech.POS_VERB:
                        VerbViewModel vvm = new VerbViewModel(lexeme, m_MainModel, lexemeViewModel);
                        vvm.ShowParticipleFormsEvent += new VerbViewModel.ShowParticipleForms(ShowParticiple);
                        paradigmViewModel = vvm;
                        break;
                    default:
                        MessageBox.Show("Illegal part of speech value in lexeme descriptor.");
                        return;
                }

                m_CurrentViewPage = new ViewPage(lexeme.sSourceForm(), lexemeViewModel, paradigmViewModel);
                m_Pages.Add(m_CurrentViewPage);
                m_iCurrentTab = m_Pages.Count - 1;
//                m_CurrentViewModel = m_BreadCrumbs.AddLast(nvp.Page);

            }       // foreach (ViewPage ...)

            //            m_CurrentViewModel = m_BreadCrumbs.AddAfter(m_CurrentViewModel, m_LexemeGridViewModel);
            UpdateView();

        }   // SearchByInitialForm()

        void ShowParticiple(CLexemeManaged lexeme, EM_Subparadigm sp, ViewModelBase parent)
        {
            LexemeViewModel parentViewModel = (LexemeViewModel)parent;

            foreach (ViewPage viewPage in m_Pages)
            {
                LexemeViewModel currentLexeme = (LexemeViewModel)viewPage.LexemeInfo;
                if (parentViewModel.Lexeme == currentLexeme.Lexeme)
                {
                    if (typeof(AdjViewModel) == viewPage.Page.GetType())
                    {
                        AdjViewModel avm = (AdjViewModel)viewPage.Page;
                        if (avm.Subparadigm == sp)
                        {
                            m_CurrentViewModel = m_BreadCrumbs.AddLast(avm);
                            m_CurrentViewPage = viewPage;
                            return;
                        }
                    }
                }
            }

            switch (sp)
            {
                case EM_Subparadigm.SUBPARADIGM_PART_PRES_ACT:
                    AdjViewModel avmPresAct = new AdjViewModel(lexeme, sp, m_MainModel);
                    ViewPage avpPresAct = new ViewPage(lexeme.sSourceForm() + " прич. наст. д.", parent, avmPresAct);
                    m_Pages.Add(avpPresAct);
                    m_CurrentViewModel = m_BreadCrumbs.AddLast(avpPresAct.Page);
                    m_CurrentViewPage = avpPresAct;
                    break;

                case EM_Subparadigm.SUBPARADIGM_PART_PAST_ACT:
                    AdjViewModel avmPastAct = new AdjViewModel(lexeme, sp, m_MainModel);
                    ViewPage avpPastAct = new ViewPage(lexeme.sSourceForm() + " прич. прош. д.", parent, avmPastAct);
                    m_Pages.Add(avpPastAct);
//                        avmPastAct.BackButtonEvent += new AdjViewModel.BackButtonHandler(GoBack);
                    m_CurrentViewModel = m_BreadCrumbs.AddLast(avpPastAct.Page);
                    m_CurrentViewPage = avpPastAct;
                    break;

                case EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG:
                    AdjViewModel avmPresPass = new AdjViewModel(lexeme, sp, m_MainModel);
                    ViewPage avpPresPass = new ViewPage(lexeme.sSourceForm() + " прич. наст. страд.", parent, avmPresPass);
                    m_Pages.Add(avpPresPass);
//                        avmPresPass.BackButtonEvent += new AdjViewModel.BackButtonHandler(GoBack);
                    m_CurrentViewModel = m_BreadCrumbs.AddLast(avpPresPass.Page);
                    m_CurrentViewPage = avpPresPass;
                    break;

                case EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG:
                    AdjViewModel avmPastPass = new AdjViewModel(lexeme, sp, m_MainModel);
                    ViewPage avpPastPass = new ViewPage(lexeme.sSourceForm() + " прич. прош. страд.", parent, avmPastPass);
                    //                        avmPastPass.BackButtonEvent += new AdjViewModel.BackButtonHandler(GoBack);
                    m_Pages.Add(avpPastPass);
                    m_CurrentViewModel = m_BreadCrumbs.AddLast(avpPastPass.Page);
                    m_CurrentViewPage = avpPastPass;
                    break;

                case EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_SHORT:
                case EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_SHORT:
                    // handled together with long forms, no need for separate processing
                    break;

                default:
                    MessageBox.Show("Unexpected subparadigm.");
                    return;
            }

            m_iCurrentTab = m_Pages.Count - 1;
            UpdateView();

        }       //  ShowParticiple()

        public void ShowRegression(object obj)
        {
//            m_MainModel.Clear();

            ViewPage rvp = new ViewPage("Тест", new LexemeViewModel(), new RegressionGridViewModel(m_MainModel));

//                m_RegressionGridViewModel.BackButtonEvent += new RegressionGridViewModel.BackButtonHandler(GoBack);
            m_Pages.Add(rvp);
            m_CurrentViewModel = m_BreadCrumbs.AddLast(rvp.Page);
            m_CurrentViewPage = rvp;
            m_iCurrentTab = m_Pages.Count - 1;
            UpdateView();
        }

        public void ImportRegressionData(object obj)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();

            openFileDialog.InitialDirectory = "c:\\";
            openFileDialog.Filter = "CSV files (*.csv)|*.csv|All files (*.*)|*.*";
            openFileDialog.FilterIndex = 2;
            openFileDialog.RestoreDirectory = true;

            if (true == openFileDialog.ShowDialog())
            {
                if (null == m_ProgressViewModel)
                {
                    m_ProgressViewModel = new ProgressViewModel();
                }

                ProgressDialog pd = new ProgressDialog(m_ProgressViewModel);
                pd.Owner = Application.Current.MainWindow;
                pd.Show();

                try
                {
                    RegressionDataImportThread rt = new RegressionDataImportThread(m_MainModel, m_ProgressViewModel, pd, openFileDialog.FileName);
                    System.Threading.Thread t = new Thread(new ThreadStart(rt.ThreadProc));
                    t.Name = "TestApp test data import thread";
                    t.IsBackground = true;
                    //              m_WorkerThread.Priority = ThreadPriority.Lowest;
                    t.SetApartmentState(ApartmentState.STA);
                    t.Start();
//                    t.Join();
//                    pd.Close();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error: " + ex.Message);
                    return;
                }
            }
        }

        public void ExportRegressionData(object obj)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();

            openFileDialog.InitialDirectory = "c:\\";
            openFileDialog.Filter = "CSV files (*.csv)|*.csv|All files (*.*)|*.*";
            openFileDialog.FilterIndex = 2;
            openFileDialog.RestoreDirectory = true;
            openFileDialog.CheckFileExists = false;

            if (true == openFileDialog.ShowDialog())
            {
                MainLibManaged.DelegateProgress progress = new MainLibManaged.DelegateProgress(UpdateProgress);
                m_MainModel.ExportRegressionData(openFileDialog.FileName, progress);
            }
        }

        private void CloseCommandMethod(object arg)
        {
            ViewModelBase vm = (ViewModelBase)arg;

            List<ViewPage> pagesToRemove = new List<ViewPage>();

            foreach (ViewPage page in m_Pages)
            {
                if (page.Page.InstanceId == vm.InstanceId)
                {
                    pagesToRemove.Add(page);
                }
            }

            List<ViewPage> derivedPagesToRemove = new List<ViewPage>();
            foreach (ViewPage pageToRemove in pagesToRemove)
            {
                if (pageToRemove.Page.GetType() == typeof(VerbViewModel))
                {
                    foreach (ViewPage page in m_Pages)
                    {
                        if (page.Page.InstanceId == pageToRemove.Page.InstanceId)
                        {
                            continue;
                        }

                        if (((LexemeViewModel)page.LexemeInfo).Lexeme == null)
                        {
                            continue;
                        }

                        if (((LexemeViewModel)page.LexemeInfo).Lexeme == ((LexemeViewModel)pageToRemove.LexemeInfo).Lexeme)
                        {
                            derivedPagesToRemove.Add(page);
                        }
                    }
                }
            }

            foreach (ViewPage pageView in pagesToRemove)
            {
                m_Pages.Remove(pageView);

                if (pageView.Page.GetType() == typeof(AdjViewModel))
                {
                    if (((AdjViewModel)pageView.Page).IsDerived)
                    {
                        continue;
                    }
                }
                CLexemeManaged l = ((LexemeViewModel)pageView.LexemeInfo).Lexeme;
                if (l != null)
                {
                    m_MainModel.RemoveLexeme(l);
                }
            }

            foreach (var pageView in derivedPagesToRemove)
            {
                m_Pages.Remove(pageView);
            }

        }   //  CloseCommandMethod()

        void UpdateProgress(int iPercent, bool bDone)
        {

        }

        private void UpdateView()
        {
            this.OnPropertyChanged("CurrentViewModel");
            this.OnPropertyChanged("CurrentTab");
            this.OnPropertyChanged("IsBackButtonEnabled");
            this.OnPropertyChanged("IsForwardButtonEnabled");
            this.OnPropertyChanged("Type");
        }

    }   //  class MainViewModel 

    #region RegressionDataImportThread

    public class RegressionDataImportThread
    {
        private MainModel m_Caller;
        private ProgressViewModel m_ProgressViewModel;
        private ProgressDialog m_ProgressDialog;
        private string m_sPath;
        private int m_iPercentDone;
        private delegate void DelegateProgress(int iPercentDone, bool bOperationComplete);

        private void UpdateProgress(int iPercentDone, bool bOperationComplete)
        {
            m_iPercentDone = iPercentDone;
            m_ProgressViewModel.Progress = m_iPercentDone;
        }

        public RegressionDataImportThread(MainModel mm, ProgressViewModel pvm, ProgressDialog pd, string sPath)
        {
            m_Caller = mm;
            m_ProgressViewModel = pvm;
            m_ProgressDialog = pd;
            m_sPath = sPath;
        }
       
        public void ThreadProc()
        {
            try
            {
                MainLibManaged.DelegateProgress progress = new MainLibManaged.DelegateProgress(UpdateProgress);
                m_Caller.ImportRegressionData(m_sPath, progress);
            }
            catch (Exception ex)
            {
                //                MainLib.ZalError err = new MainLib.ZalError();
                string sMsg = ex.Message;
                sMsg += "\n";
                //                sMsg += err.LastError;
                MessageBox.Show(sMsg, "Error");
                m_ProgressDialog.Close();
                return;
            }

            Application.Current.Dispatcher.Invoke(new Action(() => { m_ProgressDialog.Close(); }));

        }   //  ThreadProc()

    }   //  public class RegressionDataThread

    #endregion

}   //  namespace ZalTestApp
