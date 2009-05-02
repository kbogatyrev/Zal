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

        public bool m_bStopListener;
        public bool m_bStopConversion;
        public int m_iStopAfter;

        private string m_sSourcePath;
        private string m_sOutPath;
        private string m_sErrPath;
        private string m_sLogPath;
        private string m_sDbPath;

        public string sSourcePath
        {
            get
            {
                return m_sSourcePath;
            }
        }

        public string sDbPath
        {
            get
            {
                return m_sDbPath;
            }
        }

        public string sOutputPath
        {
            get
            {
                return m_sOutPath;
            }
        }
        
        public string sErrPath
        {
            get
            {
                return m_sErrPath;
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
                return tabControl.SelectedIndex;
            }
        }

        Thread m_LogListener;
        Thread m_WorkerThread;

        public ConverterForm()
        {
            InitializeComponent();

            this.StartPosition = FormStartPosition.CenterScreen;

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
            textBox.Text += sEntry + "\r\n";
            textBox.SelectionStart = textBox.Text.Length;
        }

        void UpdateProgressBar (int iProgress)
        {
            progressBar.Value = iProgress;
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
                Close();
                return;
            }

            buttonOK.Enabled = false;

            m_bStopListener = false;
            m_bStopConversion = false;

            textBox.Text = "###  Conversion started. \r\n";
            progressBar.Minimum = 0;
            progressBar.Maximum = 100;
            progressBar.Enabled = true;
            progressBar.Visible = true;

            ListenerThread listener = new ListenerThread (this, m_sLogPath);
            m_LogListener = new Thread (new ThreadStart (listener.ThreadProc));
            m_LogListener.Name = "Zal listener thread";
            m_LogListener.Start ();

            WorkerThread wt = new WorkerThread (this);
            m_WorkerThread = new Thread (new ThreadStart (wt.ThreadProc));
            m_WorkerThread.Name = "Zal worker thread";
            m_WorkerThread.IsBackground = true;
            //                m_WorkerThread.Priority = ThreadPriority.Lowest;
            m_WorkerThread.SetApartmentState (ApartmentState.STA);
            m_WorkerThread.Start ();
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
                    if (0 == tabControl.SelectedIndex)
                    {
                        textBoxSourcePath.Text = fd.FileName;
                    }
                    else
                    {
                        if (1 == tabControl.SelectedIndex)
                        {
                            textBoxPreprocessSource.Text = fd.FileName;
                        }
                        else
                        {
                            string sMsg = "Illegal tab index";
                            MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
                            return;
                        }
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
                if (0 == tabControl.SelectedIndex)
                {
                    textBoxLogPath.Text = fd.FileName;
                }
                else
                {
                    if (1 == tabControl.SelectedIndex)
                    {
                        textBoxPreprocessLog.Text = fd.FileName;
                    }
                    else
                    {
                        string sMsg = "Illegal tab index";
                        MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
                        return;
                    }
                }
                buttonOK.Enabled = bCanEnableOkButton();
            }
        }

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
            if (0 == tabControl.SelectedIndex)
            {
                if ((textBoxSourcePath.Text.Length > 0) &&
                    (textBoxDbLocation.Text.Length > 0) &&
                    (textBoxLogPath.Text.Length > 0))
                {
                    return true;
                }
                return false;
            }

            if (1 == tabControl.SelectedIndex)
            {
                if ((textBoxPreprocessSource.Text.Length > 0) &&
                    (textBoxPreprocessOutput.Text.Length > 0) &&
                    (textBoxPreprocessErrors.Text.Length > 0) &&
                    (textBoxPreprocessLog.Text.Length > 0))
                {
                    return true;
                }
                return false;
            }

            string sMsg = "Illegal tab index";
            MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
            return false;
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

        private void textBoxPreprocessLog_TextChanged (object sender, EventArgs e)
        {
            m_sLogPath = textBoxPreprocessLog.Text;
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
            try
            {
                ZalConversionLib.ZalSourceReader reader = new ZalConversionLib.ZalSourceReader();
                EventSink sink = new EventSink (m_Form);
                reader.ProgressNotification += sink.ProgressNotification;
                reader.StatusCheck += sink.StatusCheck;
                if (0 == m_Form.iSelectedTabIndex)
                {
                    reader.ConvertSourceFile (m_Form.sSourcePath, m_Form.sDbPath, m_Form.iStopAfter);
                }
                else
                {
                    if (1 == m_Form.iSelectedTabIndex)
                    {
                        reader.PreprocessSourceFile (m_Form.sSourcePath, m_Form.sOutputPath, m_Form.sErrPath);
                    }
                    else
                    {
                        string sMsg = "Illegal tab index";
                        MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK);
                    }
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
                                swLogFile.WriteLine (sLine);
                                swLogFile.Flush();

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
