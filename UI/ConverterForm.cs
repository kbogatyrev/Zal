using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;

namespace Converter
{
    public delegate void DelegateAddString (string s);
    public delegate void DelegateUpdateProgressBar (int iPercentDone);
    public delegate void DelegateLogMessage (string sText);

    public partial class ConverterForm : Form
    {
        public DelegateAddString m_DelegateAddString;
        public DelegateUpdateProgressBar m_DelegateUpdateProgressbar;

        public ConverterForm()
        {
            InitializeComponent();

            m_DelegateAddString = new DelegateAddString (this.AddString);
            m_DelegateUpdateProgressbar = new DelegateUpdateProgressBar (this.UpdateProgressBar);
        }

        private void buttonLeft_Click(object sender, EventArgs e)
        {
            FileDialog fd = new OpenFileDialog();
            DialogResult dr = fd.ShowDialog();
            if (DialogResult.OK == dr)
            {
                progressBar.Minimum = 0;
                progressBar.Maximum = 100;
                progressBar.Enabled = true;
                progressBar.Visible = true;

                WorkerThread co_wt = new WorkerThread (this, fd.FileName);
                Thread t = new Thread(new ThreadStart(co_wt.ThreadProc));
                t.IsBackground = true;
//                t.Priority = ThreadPriority.Lowest;
                t.SetApartmentState (ApartmentState.STA);
                t.Start();
//                t.Join();
            }
        }

        private void buttonRight_Click (object sender, EventArgs e)
        {

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

    }   // ConverterForm class


    public class WorkerThread
    {
        private ConverterForm m_form;
        private string m_sPath;

        public WorkerThread (ConverterForm co_form, string sPath)
        {
            m_form = co_form;
            m_sPath = sPath;
        }

        public void ThreadProc()
        {
            CT_ConversionLibTest ct = new CT_ConversionLibTest();
            int iRet = ct.test();
            iRet = ct.testStringConversion ("аибгдежзИКЛМН");

            try
            {
                FileInfo fi = new FileInfo(m_sPath);
                long lFileLength = fi.Length;
                long lProcessedLength = 0;
                int iPercentDone = 0;
int i = 0;
                using (StreamReader sr = fi.OpenText())
                {
                    string sEntry;
                    while ((sEntry = sr.ReadLine()) != null)
                    {
                        ct.testInsert(sEntry);
                        lProcessedLength += sEntry.Length;
                        double dPercent = (double)lProcessedLength / (double)lFileLength;
                        dPercent *= 100;
                        if ((int)dPercent > iPercentDone)
                        {
                            iPercentDone = (int)dPercent;
                            if (m_form.InvokeRequired)
                            {
                                m_form.BeginInvoke (m_form.m_DelegateUpdateProgressbar, 
                                                    new Object[] { iPercentDone });
                            }
                        }

                            if (m_form.InvokeRequired)
                            {
                                m_form.BeginInvoke (m_form.m_DelegateAddString, new Object[] { sEntry });
                            }
                        if (++i > 5000) break;
                    }
                }   // using...
            }
            catch (Exception ex)
            {
                //&&&&
            }

        }   //  ThreadProc()

    }   //  public class WorkerThread
}

