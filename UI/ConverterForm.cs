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

    public partial class ConverterForm : Form
    {
        public DelegateAddString m_DelegateAddString;
        public DelegateUpdateProgressBar m_DelegateUpdateProgressbar;

        public bool TerminateThread
        {
            get { return m_bAppTerminating; }
        }

        public bool m_bAppTerminating;
        public int m_iStopAfter;

        private string m_sSourcePath;
        private string m_sLogPath;
        private string m_sDbPath;
        Thread m_LogListener;

        public ConverterForm()
        {
            InitializeComponent();

            this.StartPosition = FormStartPosition.CenterScreen;

            m_DelegateAddString = new DelegateAddString (this.AddString);
            m_DelegateUpdateProgressbar = new DelegateUpdateProgressBar (this.UpdateProgressBar);

            m_bAppTerminating = false;

            buttonOK.Enabled = false;

            m_iStopAfter = (int)numericUpDownStopAfter.Value;

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

        void LogMessage (string sText)
        {
            MessageBox.Show (sText);
        }

        bool bIsAppterminating ()
        {
            return m_bAppTerminating;
        }

        //
        //  Event handlers
        //
        private void buttonOK_Click(object sender, EventArgs e)
        {
            progressBar.Minimum = 0;
            progressBar.Maximum = 100;
            progressBar.Enabled = true;
            progressBar.Visible = true;

            ListenerThread listener = new ListenerThread (this, m_sLogPath);
            m_LogListener = new Thread (new ThreadStart (listener.ThreadProc));
            m_LogListener.Name = "Zal listener thread";
            m_LogListener.Start ();

            WorkerThread wt = new WorkerThread (this, m_sSourcePath);
            Thread t = new Thread (new ThreadStart (wt.ThreadProc));
            t.Name = "Zal worker thread";
            t.IsBackground = true;
            //                t.Priority = ThreadPriority.Lowest;
            t.SetApartmentState (ApartmentState.STA);
            t.Start ();
        }

        private void buttonCancel_Click (object sender, EventArgs e)
        {
            Close();
        }

        private void ConverterForm_FormClosing (object sender, FormClosingEventArgs e)
        {
            m_bAppTerminating = true;
            if (null == m_LogListener)
            {
                return;
            }
            if (!m_LogListener.IsAlive)
            {
                return;
            }

            using (NamedPipeClientStream pipeClient =
                    new NamedPipeClientStream (".", "ZalConversionLog", PipeDirection.Out))
            {
                try
                {
                    pipeClient.Connect (60000);
                    using (StreamWriter sw = new StreamWriter (pipeClient, Encoding.Unicode))
                    {
                        sw.Write ("Form closing.");
                        sw.Flush();
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
                    textBoxSourcePath.Text = fd.FileName;
                    if ((textBoxDbLocation.Text.Length > 0) &&
                        (textBoxLogPath.Text.Length > 0))
                    {
                        buttonOK.Enabled = true;
                    }
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
                buttonOK.Enabled = true;
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
                textBoxLogPath.Text = fd.FileName;
//                buttonOK.Enabled = true;
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

    }   // ConverterForm class

    public class WorkerThread
    {
        private ConverterForm m_Form;
        private string m_sPath;

        public WorkerThread (ConverterForm Form, string sPath)
        {
            m_Form = Form;
            m_sPath = sPath;
        }

        public void ThreadProc()
        {
            CT_ConversionLibTest ct = new CT_ConversionLibTest();
int iRet = ct.test();
iRet = ct.testStringConversion ("аибгдежзИКЛМН");
iRet = ct.testError();

            try
            {
                FileInfo fi = new FileInfo(m_sPath);
                long lFileLength = fi.Length;
                long lProcessedLength = 0;
                int iPercentDone = 0;

                using (StreamReader sr = fi.OpenText())
                {
                    string sEntry;
                    for (int iRecordsProcessed = 0; 
                        (iRecordsProcessed <= m_Form.m_iStopAfter) && ((sEntry = sr.ReadLine()) != null);
                        ++iRecordsProcessed)
                    {
                        ct.testInsert(sEntry);
                        lProcessedLength += sEntry.Length;
                        double dPercent = (double)lProcessedLength / (double)lFileLength;
                        dPercent *= 100;
                        if ((int)dPercent > iPercentDone)
                        {
                            iPercentDone = (int)dPercent;
                            if (m_Form.InvokeRequired)
                            {
                                m_Form.BeginInvoke (m_Form.m_DelegateUpdateProgressbar, 
                                                    new Object[] { iPercentDone });
                            }
                        }

                        if (m_Form.InvokeRequired)
                        {
                            m_Form.BeginInvoke (m_Form.m_DelegateAddString, new Object[] { sEntry });
                        }

                        if (iRecordsProcessed % 10 == 0)
                        {
                            using (NamedPipeClientStream pipeClient =
                                new NamedPipeClientStream (".", "ZalConversionLog", PipeDirection.Out))
                            {
                                int iLapsed = 0;
                                const int ciTimeout = 120000;
                                while (iLapsed < ciTimeout)
                                {
                                    try
                                    {
                                        pipeClient.Connect (50);
                                        break;
                                    }
                                    catch
                                    {
                                        Thread.Sleep (1000);
                                    }
                                }
                                if (iLapsed >= ciTimeout)
                                {
                                    string sMsg = "I/O error in WorkerThread: ";
                                    sMsg += "Named pipe client unable to connect.";
                                    MessageBox.Show ("File Does not exist",
                                                     "Zal Error",
                                                     MessageBoxButtons.OK,
                                                     MessageBoxIcon.Exclamation);
                                    return;
                                }
                                using (StreamWriter sw = new StreamWriter (pipeClient, Encoding.Unicode))
                                {
                                    sw.Write (sEntry);
                                    sw.Flush();
                                }
                            }
                        }
 
                    }
                }   // using...
            }
            catch (Exception ex)
            {
                string sMsg = "I/O error in WorkerThread: ";
                sMsg += ex.Message;
                MessageBox.Show (sMsg, "Zal Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
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
                            while (!bTerminate ())
                            {
                                pipeServer.WaitForConnection ();
                                string sLine = srInStream.ReadToEnd ();
                                swLogFile.WriteLine (sLine);
                                swLogFile.Flush();
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
            if (m_Form.TerminateThread)
            {
                return true;
            }

            return false;
        }

    }   //  public class ListenerThread

}   // namespace
