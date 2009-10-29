using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TestUI
{
    public partial class AnalysisPanel : UserControl
    {

        int iWordformId;

        public string sLemma
        {
            get
            {
                return TextBox_Lemma.Text;
            }

            set
            {
                TextBox_Lemma.Text = value;
            }
        }

        public string sWordform
        {
            get
            {
                return TextBox_Wordform.Text;
            }

            set
            {
                TextBox_Wordform.Text = value;
            }
        }

        public string sID
        {
            get
            {
                return TextBox_ID.Text;
            }

            set
            {
                TextBox_ID.Text = value;
            }
        }

        public string sPanelNum
        {
            get
            {
                return TextBox_PanelNum.Text;
            }

            set
            {
                TextBox_PanelNum.Text = value;
            }
        }

        public AnalysisPanel(int iId)
        {
            InitializeComponent();
            iWordformId = iId;
            sPanelNum = iId.ToString();
        }

        private void AnalysisPanel_Load(object sender, EventArgs e)
        {

        }
    }
}
