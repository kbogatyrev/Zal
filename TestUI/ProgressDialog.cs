using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TestUI
{
    public partial class ProgressDialog : Form
    {
        private int m_iStartPos;
        private int m_iRange;

        public int iStartPos
        {
            get
            {
                return m_iStartPos;
            }
            set
            {
                m_iStartPos = iStartPos;
            }
        }

        public int iRange
        {
            get
            {
                return m_iRange;
            }
            set
            {
                m_iRange = iRange;
            }
        }

        public void Done(bool bIsDone)
        {
            buttonClose.Enabled = bIsDone;
        }

        public ProgressDialog()
        {
            InitializeComponent();
            m_iStartPos = 0;
            m_iRange = 100;
        }

        public void SetProgressBarPos (int iPercent)
        {
            int iScaleFactor = 100 / m_iRange;
            progressBar.Value = m_iStartPos + iPercent / iScaleFactor;
        }

        public void SetMessage(string sMsg)
        {
            textBox.Text = sMsg;
        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
