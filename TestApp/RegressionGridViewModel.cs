﻿using System;
using System.Windows.Input;
using System.Collections.ObjectModel;
using System.Data;
using System.Windows;
using System.Collections;

using MainLibManaged;
using System.Collections.Generic;
using System.Threading;

namespace ZalTestApp
{
    public class RegressionGridViewModel : ViewModelBase
    {
        MainModel m_MainModel = null;

        public delegate void BackButtonHandler();
        public event BackButtonHandler BackButtonEvent;

        private DataTable m_RegressionData;
        public DataTable RegressionData
        {
            get
            {
                return m_RegressionData;
            }

            set
            {
                m_RegressionData = value;
            }
        }

        public int NLexemes
        {
            get
            {
                return m_RegressionData.Rows.Count;
            }
        }

        private bool m_bCancelBatchVerifier;
        public bool CancelVerifier
        {
            get
            {
                return m_bCancelBatchVerifier;
            }

            set
            {
                m_bCancelBatchVerifier = value;
            }
        }

        public void SetResult(int iRow, string sText)
        {
            try
            {
                DataRow row = m_RegressionData.Rows[iRow];
                row["TestResult"] = sText;
            }
            catch(Exception ex)
            {
                MessageBox.Show("Error: unable to change result value: " + ex.Message);
                return;
            }
        }

        public bool IsChecked(int iRow)
        {
            try
            {
                DataRow row = m_RegressionData.Rows[iRow];
                return (bool)row["IsChecked"];
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: unable retrieve selection status value: " + ex.Message);
                return false;
            }
        }

        public string LexemeHash(int iRow)
        {
            try
            {
                DataRow row = m_RegressionData.Rows[iRow];
                return (string)row["LexemeHash"];
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: unable to retrieve lexeme hash value: " + ex.Message);
                return "";
            }
        }

        public string SourceForm(int iRow)
        {
            try
            {
                DataRow row = m_RegressionData.Rows[iRow];
                return (string)row["SourceForm"];
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: unable to retrieve source form value: " + ex.Message);
                return "";
            }
        }

        public EM_ReturnCode Verify(int iRow, ref EM_TestResult eTestResult)
        {
            string sLexemeHash = LexemeHash(iRow);
            var eRet = m_MainModel.VerifyLexeme(sLexemeHash, ref eTestResult);
            return eRet;
        }

        public int Rows()
        {
            return m_RegressionData.Rows.Count;
        }

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
        private ICommand m_RunTestCommand;
        public ICommand RunTestCommand
        {
            get
            {
                return m_RunTestCommand;
            }
            set
            {
                m_RunTestCommand = value;
            }
        }
        private ICommand m_SaveCommand;
        public ICommand SaveCommand
        {
            get
            {
                return m_SaveCommand;
            }
            set
            {
                m_SaveCommand = value;
            }
        }
        private ICommand m_DeleteCommand;
        public ICommand DeleteCommand
        {
            get
            {
                return m_DeleteCommand;
            }
            set
            {
                m_DeleteCommand = value;
            }
        }
        private ICommand m_CancelCommand;
        public ICommand CancelCommand
        {
            get
            {
                return m_CancelCommand;
            }
            set
            {
                m_CancelCommand = value;
            }
        }
        #endregion

        public RegressionGridViewModel(MainModel mm)
        {
            m_MainModel = mm;
 
            RunTestCommand = new RelayCommand(new Action<object>(RunTest));
            SaveCommand = new RelayCommand(new Action<object>(Save));
            DeleteCommand = new RelayCommand(new Action<object>(Delete));
            CancelCommand = new RelayCommand(new Action<object>(Cancel));
            BackCommand = new RelayCommand(new Action<object>(GoBack));

            bool bRet = m_MainModel.GetStoredLexemeData();
            if (!bRet)
            {
                MessageBox.Show("Unable to load regression data.");
                return;
            }

            m_RegressionData = new DataTable("RegressionTable");

            DataColumn colCheckbox = new DataColumn();
            colCheckbox.DataType = Type.GetType("System.Boolean");
            colCheckbox.ColumnName = "IsChecked";
            m_RegressionData.Columns.Add(colCheckbox);

            DataColumn colHeadWord = new DataColumn();
            colHeadWord.DataType = Type.GetType("System.String");
            colHeadWord.ColumnName = "SourceForm";
            m_RegressionData.Columns.Add(colHeadWord);

            DataColumn colLexHash = new DataColumn();
            colLexHash.DataType = Type.GetType("System.String");
            colLexHash.ColumnName = "LexemeHash";
            m_RegressionData.Columns.Add(colLexHash);

            DataColumn colTestResult = new DataColumn();
            colTestResult.DataType = Type.GetType("System.String");
            colTestResult.ColumnName = "TestResult";
            m_RegressionData.Columns.Add(colTestResult);

            DataColumn[] PrimaryKeyColumns = new DataColumn[1];
            PrimaryKeyColumns[0] = m_RegressionData.Columns["LexemeHash"];
            m_RegressionData.PrimaryKey = PrimaryKeyColumns;

            IEnumerator storedLexEnumerator = m_MainModel.GetStoredLexemesEnumerator();
            while (storedLexEnumerator.MoveNext())
            {
                var headWordToHash = (KeyValuePair<string, string>)storedLexEnumerator.Current;
                DataRow row = m_RegressionData.NewRow();
                row["SourceForm"] = headWordToHash.Key;
                row["LexemeHash"] = headWordToHash.Value;
                row["IsChecked"] = true;
                m_RegressionData.Rows.Add(row);
            }
        }       //  RegressionGridViewModel()

        public void GoBack(Object obj)
        {
            BackButtonEvent?.Invoke();
        }

        public void RunTest(Object obj)
        {
            try
            {
                VerifierThread vt = new VerifierThread(this);
                System.Threading.Thread t = new Thread(new ThreadStart(vt.ThreadProc));
                t.Name = "TesApp batch verifier thread";
                t.IsBackground = true;
                //              m_WorkerThread.Priority = ThreadPriority.Lowest;
                t.SetApartmentState(ApartmentState.STA);
                t.Start();
//                t.Join();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message);
                return;
            }
        }

        public void Save(Object obj)
        {
            MessageBox.Show("Save");
        }

        public void Delete(Object obj)
        {
            MessageBox.Show("Delete");
        }

        public void Cancel(Object obj)
        {
            MessageBox.Show("Cancel");
        }

    }   //  class RegressionGridViewModel

    #region VerifierThread
    public class VerifierThread
    {
        private RegressionGridViewModel m_Caller;

        public VerifierThread(RegressionGridViewModel rvm)
        {
            m_Caller = rvm;
        }

        public void ThreadProc()
        {
            try
            {
                for (int iLexeme = 0; iLexeme < m_Caller.NLexemes; ++iLexeme)
                {
                    if (m_Caller.CancelVerifier)
                    {
                        m_Caller.SetResult(iLexeme, "Cancelled");
                        break;
                    }

                    if (!m_Caller.IsChecked(iLexeme))
                    {
                        m_Caller.SetResult(iLexeme, "Ignored");
                        continue;
                    }

                    EM_TestResult eTestResult = EM_TestResult.TEST_RESULT_UNDEFINED;
                    EM_ReturnCode eRet = m_Caller.Verify(iLexeme, ref eTestResult);
                    if (eRet != EM_ReturnCode.H_NO_ERROR)
                    {
                        MessageBox.Show(string.Format("Unable to verify lexeme hash {0}", m_Caller.LexemeHash(iLexeme)));
                    }

                    switch (eTestResult)
                    {
                        case EM_TestResult.TEST_RESULT_OK:
                        {
                            m_Caller.SetResult(iLexeme, "Pass");
                            break;
                        }
                        case EM_TestResult.TEST_RESULT_FAIL:
                        {
                            m_Caller.SetResult(iLexeme, "Fail");
                            break;
                        }
                        case EM_TestResult.TEST_RESULT_INCOMPLETE:
                        {
                            m_Caller.SetResult(iLexeme, "Missing forms");
                            break;
                        }
                        default:
                        {
                            //                            MainLib.ZalError err = new MainLib.ZalError();
                            string sMsg = "Unexpected return from IVerifier; error msg: ";
                            //                            sMsg += err.LastError;
                            sMsg += eTestResult.ToString();
                            MessageBox.Show(sMsg, "Zal Error");
                            return;
                        }

                    }       //  switch ...

                }   //  foreach (DataGridViewRow row in dataGridView.Rows)
            }
            catch (Exception ex)
            {
                //                MainLib.ZalError err = new MainLib.ZalError();
                string sMsg = ex.Message;
                sMsg += "\n";
                //                sMsg += err.LastError;
                MessageBox.Show(sMsg, "Error");
                return;
            }

        }   //  ThreadProc()

    }   //  public class VerifierThread
    #endregion
}
