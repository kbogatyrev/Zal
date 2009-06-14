using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.IO.Pipes;
using System.Runtime.InteropServices;
using System.Threading;

namespace Converter
{
    public delegate void DelegateAddString (string s);
    public delegate void DelegateUpdateProgressBar (int iPercentDone);
    public delegate void DelegateSignalCompletion();

    public partial class ConverterForm : Form
    {
        public DelegateAddString m_DelegateAddString;
        public DelegateUpdateProgressBar m_DelegateUpdateProgressbar;
        public DelegateSignalCompletion m_DelegateSignalCompletion;

        public static AutoResetEvent sm_Event;

        public bool m_bSaveTempData;
        public bool m_bStopListener;
        public bool m_bStopConversion;
        public int m_iStopAfter;
        public int m_iSelectedTab;

        private string m_sSourcePath;
        private string m_sOutPath;
//        private string m_sErrPath;
        private string m_sLogPath;
        private string m_sDbPath;
        private string m_sSearchString; // search tab
        private bool m_bIsRegex;         //   - " -

        public string sSourcePath
        {
            get
            {
                return m_sSourcePath;
            }
        }

        public string sOutPath
        {
            get
            {
                return m_sOutPath;
            }
        }

        public string sDbPath
        {
            get
            {
                return m_sDbPath;
            }
        }

        public int iStopAfter
        {
            get
            {
                return m_iStopAfter;
            }
        }

        public int iSelectedTabIndex
        {
            get
            {
                return m_iSelectedTab;
            }
        }

        public string sSearchString
        {
            get
            {
                return m_sSearchString;
            }
        }

        public int iRegexSearch     // bool --> int to please COM
        {
            get
            {
                return m_bIsRegex ? 1 : 0;
            }
        }

        Thread m_LogListener;
        Thread m_WorkerThread;

        public ConverterForm()
        {
            InitializeComponent();

            this.StartPosition = FormStartPosition.CenterScreen;

            sm_Event = new AutoResetEvent (false);

            m_DelegateAddString = new DelegateAddString (this.AddString);
            m_DelegateUpdateProgressbar = new DelegateUpdateProgressBar (this.UpdateProgressBar);
            m_DelegateSignalCompletion = new DelegateSignalCompletion (this.OnConversionEnd);

            buttonOK.Enabled = false;

            if (numericUpDownStopAfter.Enabled)
            {
                m_iStopAfter = (int)numericUpDownStopAfter.Value;
            }
            else
            {
                m_iStopAfter = -1;
            }

            m_iSelectedTab = tabControl.SelectedIndex;
            m_bSaveTempData = false;
        }

        void StopThreads()
        {
            m_bStopConversion = true;
            if (null != m_WorkerThread)
            {
                if (m_WorkerThread.IsAlive)
                {
                    m_WorkerThread.Join();
                }
            }

            m_bStopListener = true;
            if (null == m_LogListener)
            {
                return;
            }
            if (!m_LogListener.IsAlive)
            {
                return;
            }

            //
            // Send a dummy message that will force the listener to check for 
            // termination signal
            //
            using (NamedPipeClientStream pipeClient =
                    new NamedPipeClientStream (".", "ZalConversionLog", PipeDirection.Out))
            {
                try
                {
                    pipeClient.Connect (60000);
                    using (StreamWriter sw = new StreamWriter (pipeClient, Encoding.Unicode))
                    {
                        sw.Write ("### Form closing.");
                        sw.Flush ();
                    }
                }
                catch (Exception ex)
                {
                    string sMsg = "I/O error in ConverterForm_FormClosing: ";
                    sMsg += ex.Message;
                    MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
                    return;
                }
            }
            m_LogListener.Join();

        }   //  StopThreads()

        //
        // Local event handlers
        //
        private void ConverterForm_FormClosing (object sender, FormClosingEventArgs e)
        {
            StopThreads ();
        }

        void AddString (string sEntry)
        {
            switch (tabControl.SelectedIndex)
            {
                case 0:
                    textBoxConversion.Text += sEntry + "\r\n";
                    textBoxConversion.SelectionStart = textBoxConversion.Text.Length;
                    break;
                case 1:
                    textBoxPreprocess.Text += sEntry + "\r\n";
                    textBoxPreprocess.SelectionStart = textBoxConversion.Text.Length;
                    break;
                case 2:
                    textBoxSearch.Text += sEntry + "\r\n";
                    textBoxSearch.SelectionStart = textBoxConversion.Text.Length;
                    break;
                default:
                    string sMsg = "Illegal tab index";
                    MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
                    return;
            }
        }

        void UpdateProgressBar (int iProgress)
        {
            try
            {
                progressBar.Value = iProgress;
            }
            catch (Exception ex)
            {
                string sMsg = "Error in UpdateProgressBar: ";
                sMsg += ex.Message;
                MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
                return;
            }
        }

        void OnConversionEnd()
        {
            m_bStopListener = true;

            buttonCancel.Enabled = false;
            buttonOK.Enabled = true;

            buttonOK.Text = "Done";
            buttonOK.Tag = "Done";
        }

        //
        //  .Net event handlers
        //
        private void buttonOK_Click(object sender, EventArgs e)
        {
            if ("Done" == (string)buttonOK.Tag)
            {
                if (tabControl.SelectedIndex == 2)
                {
                    string sMsg = "Would you like to save search results?";
                    DialogResult dr = MessageBox.Show (sMsg, "Zal Search", MessageBoxButtons.YesNo);
                    if (System.Windows.Forms.DialogResult.Yes == dr)
                    {
                        FileDialog fd = new SaveFileDialog();
                        dr = fd.ShowDialog();
                        if (DialogResult.OK == dr)
                        {
                            m_bSaveTempData = true;
                            m_sOutPath = fd.FileName;
                        }
                        else
                        {
                            m_bSaveTempData = false;
                        }
                    }
                }
                ConverterForm.sm_Event.Set ();
                Close();
                return;
            }

            buttonOK.Enabled = false;

            m_bStopListener = false;
            m_bStopConversion = false;

//            textBoxConversion.Text = "###  Conversion started. \r\n";
            switch (tabControl.SelectedIndex)
            {
                case 0:
//                    textBoxConversion.Text = "###  Conversion started. \r\n";
                    textBoxConversion.Text = "";
                    break;
                case 1:
//                    textBoxPreprocess.Text = "###  Preprocessing started. \r\n";
                    textBoxPreprocess.Text = "";
                    break;
                case 2:
//                    textBoxSearch.Text = "###  Search started. \r\n";
                    textBoxSearch.Text = "";
                    break;
                default:
                    string sMsg = "Illegal tab index";
                    MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
                    return;
            }

            progressBar.Minimum = 0;
            progressBar.Maximum = 100;
            progressBar.Enabled = true;
            progressBar.Visible = true;

            ListenerThread listener = new ListenerThread (this, m_sLogPath);
            m_LogListener = new Thread (new ThreadStart (listener.ThreadProc));
            m_LogListener.Name = "Zal listener thread";
            m_LogListener.Start();

            WorkerThread wt = new WorkerThread (this);
            m_WorkerThread = new Thread (new ThreadStart (wt.ThreadProc));
            m_WorkerThread.Name = "Zal worker thread";
            m_WorkerThread.IsBackground = true;
            //                m_WorkerThread.Priority = ThreadPriority.Lowest;
            m_WorkerThread.SetApartmentState (ApartmentState.STA);
            m_WorkerThread.Start();
        }

        private void buttonCancel_Click (object sender, EventArgs e)
        {
            if (null == m_WorkerThread)
            {
                Close();
                return;
            }

            if (!m_WorkerThread.IsAlive)
            {
                Close();
                return;
            }

            DialogResult dr = MessageBox.Show ("Are you sure you want to stop conversion?",
                                               "GDRL Conversion",
                                               MessageBoxButtons.YesNo);
            if (System.Windows.Forms.DialogResult.Yes == dr)
            {
                sm_Event.Set();
                StopThreads();
                OnConversionEnd ();
            }
        }

        private void buttonSourcePath_Click (object sender, EventArgs e)
        {
            FileDialog fd = new OpenFileDialog ();
            DialogResult dr = fd.ShowDialog ();
            if (DialogResult.OK == dr)
            {
                m_sSourcePath = fd.FileName;
                if (File.Exists (fd.FileName))
                {
                    switch (tabControl.SelectedIndex)
                    {
                        case 0:
                            textBoxSourcePath.Text = fd.FileName;
                            break;
                        case 1:
                            textBoxPreprocessSource.Text = fd.FileName;
                            break;
                        case 2:
                            textBoxSearchSource.Text = fd.FileName;
                            break;
                        default:
                            string sMsg = "Illegal tab index";
                            MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
                            return;
                    }
                    buttonOK.Enabled = bCanEnableOkButton();
                }
                else
                {
                    MessageBox.Show ("File Does not exist", 
                                     "Zal Error", 
                                     MessageBoxButtons.OK, 
                                     MessageBoxIcon.Exclamation);
                }
            }
        }

        private void buttonDbLocation_Click (object sender, EventArgs e)
        {
            FileDialog fd = new OpenFileDialog ();
            fd.CheckFileExists = false;
            fd.CheckPathExists = true;
            DialogResult dr = fd.ShowDialog ();
            if (DialogResult.OK == dr)
            {
                m_sDbPath = fd.FileName;
                textBoxDbLocation.Text = fd.FileName;
                buttonOK.Enabled = bCanEnableOkButton();
            }
        }

        private void buttonLogPath_Click (object sender, EventArgs e)
        {
            FileDialog fd = new OpenFileDialog ();
            fd.CheckFileExists = false;
            fd.CheckPathExists = true;
            DialogResult dr = fd.ShowDialog ();
            if (DialogResult.OK == dr)
            {
                m_sLogPath = fd.FileName;
                switch (tabControl.SelectedIndex)
                {
                    case 0:
                        textBoxLogPath.Text = fd.FileName;
                        break;
                    case 1:
                        textBoxPreprocessLog.Text = fd.FileName;
                        break;
                    case 2:
                        textBoxSearchLog.Text = fd.FileName;
                        break;
                    default:
                        string sMsg = "Illegal tab index";
                        MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
                        return;
                }
                buttonOK.Enabled = bCanEnableOkButton();
            }
        }

/*
        private void buttonPreprocessOut_Click (object sender, EventArgs e)
        {
            FileDialog fd = new OpenFileDialog ();
            fd.CheckFileExists = false;
            fd.CheckPathExists = true;
            DialogResult dr = fd.ShowDialog ();
            if (DialogResult.OK == dr)
            {
                m_sOutPath = fd.FileName;
                textBoxPreprocessOutput.Text = fd.FileName;
            }
        }

        private void buttonPreprocessErrors_Click (object sender, EventArgs e)
        {
            FileDialog fd = new OpenFileDialog ();
            fd.CheckFileExists = false;
            fd.CheckPathExists = true;
            DialogResult dr = fd.ShowDialog ();
            if (DialogResult.OK == dr)
            {
                m_sErrPath = fd.FileName;
                textBoxPreprocessErrors.Text = fd.FileName;
            }
        }
*/
        private void checkBoxTestRun_CheckedChanged (object sender, EventArgs e)
        {
            if (checkBoxTestRun.Checked)
            {
                textBoxStopAfter.Enabled = true;
                numericUpDownStopAfter.Enabled = true;
            }
            else
            {
                textBoxStopAfter.Enabled = false;
                numericUpDownStopAfter.Enabled = false;
            }
        }

        private void numericUpDownStopAfter_ValueChanged (object sender, EventArgs e)
        {
            m_iStopAfter = (int)((NumericUpDown)sender).Value;
        }

        private bool bCanEnableOkButton()
        {
            switch (tabControl.SelectedIndex)
            {
                case 0:
                    if ((textBoxSourcePath.Text.Length > 0) &&
                        (textBoxDbLocation.Text.Length > 0) &&
                        (textBoxLogPath.Text.Length > 0))
                    {
                        return true;
                    }
                    return false;
                case 1:
                    if ((textBoxPreprocessSource.Text.Length > 0) &&
//                        (textBoxPreprocessOutput.Text.Length > 0) &&
//                        (textBoxPreprocessErrors.Text.Length > 0) &&
                        (textBoxPreprocessLog.Text.Length > 0))
                    {
                        return true;
                    }
                    return false;
                case 2:
                    if ((textBoxSearchSource.Text.Length > 0) &&
                        (textBoxSearchString.Text.Length > 0))
                    {
                        return true;
                    }
                    return false;
                default:
                    string sMsg = "Illegal tab index";
                    MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
                    return false;
            }
        }

        private void textBoxSourcePath_TextChanged (object sender, EventArgs e)
        {
            m_sSourcePath = textBoxSourcePath.Text;
            buttonOK.Enabled = bCanEnableOkButton();
        }

        private void textBoxDbLocation_TextChanged (object sender, EventArgs e)
        {
            m_sDbPath = textBoxDbLocation.Text;
            buttonOK.Enabled = bCanEnableOkButton();
        }

        private void textBoxLogPath_TextChanged (object sender, EventArgs e)
        {
            m_sLogPath = textBoxLogPath.Text;
            buttonOK.Enabled = bCanEnableOkButton();
        }

        private void textBoxPreprocessSource_TextChanged (object sender, EventArgs e)
        {
            m_sSourcePath = textBoxPreprocessSource.Text;
            buttonOK.Enabled = bCanEnableOkButton();
        }
/*
        private void textBoxPreprocessOutput_TextChanged (object sender, EventArgs e)
        {
            m_sOutPath = textBoxPreprocessOutput.Text;
            buttonOK.Enabled = bCanEnableOkButton();
        }

        private void textBoxPreprocessErrors_TextChanged (object sender, EventArgs e)
        {
            m_sErrPath = textBoxPreprocessErrors.Text;
            buttonOK.Enabled = bCanEnableOkButton();
        }
*/
        private void textBoxPreprocessLog_TextChanged (object sender, EventArgs e)
        {
            m_sLogPath = textBoxPreprocessLog.Text;
            buttonOK.Enabled = bCanEnableOkButton();
        }

        private void tabControl_SelectedIndexChanged (object sender, EventArgs e)
        {
            m_iSelectedTab = tabControl.SelectedIndex;
        }

        private void textBoxSearchSource_TextChanged (object sender, EventArgs e)
        {
            m_sSourcePath = textBoxSearchSource.Text;
            buttonOK.Enabled = bCanEnableOkButton();
        }

        private void checkBoxSearchRegex_CheckedChanged (object sender, EventArgs e)
        {
            m_bIsRegex = (CheckState.Checked == checkBoxSearchRegex.CheckState);
        }

        private void textBoxSearchString_TextChanged (object sender, EventArgs e)
        {
            m_sSearchString = textBoxSearchString.Text;
            buttonOK.Enabled = bCanEnableOkButton();
        }

        private void textBoxSearchLog_TextChanged (object sender, EventArgs e)
        {
            m_sLogPath = textBoxSearchLog.Text;
            buttonOK.Enabled = bCanEnableOkButton();
        }

    }   // ConverterForm class

    public class EventSink : ZalConversionLib.IZalNotification
    {
        private ConverterForm m_Form;
        public EventSink (ConverterForm Form)
        {
            m_Form = Form;
        }

        public void ProgressNotification (int iPercentDone)
        {
            if (m_Form.InvokeRequired)
            {
                m_Form.BeginInvoke (m_Form.m_DelegateUpdateProgressbar,
                                    new Object[] { iPercentDone });
            }
        }

        public void StatusCheck (out int iCancel)
        {
            if (m_Form.m_bStopConversion)
            {
                iCancel = 1;
            }
            else
            {
                iCancel = 0;
            }
        }

    }   // EventSink 

    public class WorkerThread
    {
        private ConverterForm m_Form;

        public WorkerThread (ConverterForm Form)
        {
            m_Form = Form;
        }

        public void ThreadProc()
        {
            ZalConversionLib.ZalSourceReader reader;
            try
            {
                reader = new ZalConversionLib.ZalSourceReader();
                EventSink sink = new EventSink (m_Form);
                reader.ProgressNotification += sink.ProgressNotification;
                reader.StatusCheck += sink.StatusCheck;
                switch (m_Form.iSelectedTabIndex)
                {
                    case 0:
                        reader.ConvertSourceFile (m_Form.sSourcePath, m_Form.sDbPath, m_Form.iStopAfter);
                        break;
                    case 1:
                        reader.PreprocessSourceFile (m_Form.sSourcePath, 
                                                     Path.GetDirectoryName (m_Form.sSourcePath));
                        break;
                    case 2:
                        reader.SearchSourceFile (m_Form.sSourcePath, 
                                                 m_Form.sSearchString, 
                                                 (int)m_Form.iRegexSearch);
                        break;
                    default:
                        string sMsg = "Illegal tab index";
                        MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
                        break;
                }
            }
            catch (Exception ex)
            {
                string sMsg = "Error in WorkerThread: ";
                sMsg += ex.Message;
                MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }

            if (m_Form.InvokeRequired)
            {
                m_Form.BeginInvoke (m_Form.m_DelegateSignalCompletion);
            }

            ConverterForm.sm_Event.WaitOne();

            if (m_Form.m_bSaveTempData)
            {
                if (m_Form.m_iSelectedTab != 2)
                {
                    string sMsg = "Error in WorkerThread: Unexpected tab value.";
                    MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    return;
                }
                reader.SaveOutput (m_Form.sOutPath);
            }

        }   //  ThreadProc()

    }   //  public class WorkerThread

    public class ListenerThread
    {
        private ConverterForm m_Form;
        private string m_sPath;

        public ListenerThread (ConverterForm Form, string sLogPath)
        {
            m_Form = Form;
            m_sPath = sLogPath;
        }

        public void ThreadProc()
        {
            using (NamedPipeServerStream pipeServer =
                    new NamedPipeServerStream ("ZalConversionLog", PipeDirection.In))
            {
                using (StreamReader srInStream = new StreamReader (pipeServer, Encoding.Unicode))
                {
                    using (StreamWriter swLogFile = new StreamWriter (m_sPath, true, Encoding.Unicode))
                    {
                        try
                        {
                            while (!bTerminate())
                            {
                                pipeServer.WaitForConnection();
                                string sLine = srInStream.ReadToEnd();
                                string sNoLogPrefix = ("*** NO_LOG *** ");
                                if (sLine.StartsWith(sNoLogPrefix))
                                {
                                    sLine = sLine.Substring (sNoLogPrefix.Length);
                                }
                                else
                                {
                                    swLogFile.WriteLine (sLine);
                                    swLogFile.Flush ();
                                }

                                if (m_Form.InvokeRequired)
                                {
                                    m_Form.BeginInvoke (m_Form.m_DelegateAddString, 
                                                        new Object[] { sLine });
                                }
                                pipeServer.Disconnect();
                            }
                        }
                        catch (Exception ex)
                        {
                            string sMsg = "I/O error in ListenerThread/StreamReader: ";
                            sMsg += ex.Message;
                            MessageBox.Show (sMsg, 
                                             "Zal Error", 
                                             MessageBoxButtons.OK, 
                                             MessageBoxIcon.Exclamation);
                            return;
                        }
                    }
                }       //  using (StreamReader sr...)

            }   //  using (NamedPipeServerStream pipeServer ...)
            
        }   //  ThreadProc()

        private bool bTerminate()
        {
            if (m_Form.m_bStopListener)
            {
                return true;
            }

            return false;
        }

    }   //  public class ListenerThread

}   // namespace
