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

        public MainLib.ET_Case eoCase
        {
            set
            {
                switch (value)
                {
                    case MainLib.ET_Case.CASE_NOM:
                            TextBox_Case.Text = "Nom";
                            break;
                    case MainLib.ET_Case.CASE_ACC:
                            TextBox_Case.Text = "Acc";
                            break;
                    case MainLib.ET_Case.CASE_GEN:
                            TextBox_Case.Text = "Gen";
                            break;
                    case MainLib.ET_Case.CASE_DAT:
                            TextBox_Case.Text = "Dat";
                            break;
                    case MainLib.ET_Case.CASE_PART:
                            TextBox_Case.Text = "Part";
                            break;
                    case MainLib.ET_Case.CASE_INST:
                            TextBox_Case.Text = "Inst";
                            break;
                    case MainLib.ET_Case.CASE_PREP:
                            TextBox_Case.Text = "Prep";
                            break;
                    case MainLib.ET_Case.CASE_LOC:
                            TextBox_Case.Text = "Loc";
                            break;
                    case MainLib.ET_Case.CASE_NUM:
                            TextBox_Case.Text = "Сч. форма";
                            break;
                }
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
