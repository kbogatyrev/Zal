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
    public partial class NounPanel : UserControl
    {
        Dictionary<string, TextBox> dictForms;

        public string sLexName
        {
            get
            {
                return textBoxLexName.Text;
            }

            set
            {
                textBoxLexName.Text = value;
            }
        }

        public void setForm (string sNumberCase, string sText)
        {
            dictForms[sNumberCase].Text = sText;
        }

        public NounPanel()
        {
            InitializeComponent();

            dictForms = new Dictionary<string,TextBox>();
            dictForms.Add ("NSg", textBoxNSg);
            dictForms.Add ("ASg", textBoxASg);
            dictForms.Add ("GSg", textBoxGSg);
            dictForms.Add ("LSg", textBoxLSg);
            dictForms.Add ("DSg", textBoxDSg);
            dictForms.Add ("ISg", textBoxISg);

            dictForms.Add ("NPl", textBoxNPl);
            dictForms.Add ("APl", textBoxAPl);
            dictForms.Add ("GPl", textBoxGPl);
            dictForms.Add ("LPl", textBoxLPl);
            dictForms.Add ("DPl", textBoxDPl);
            dictForms.Add ("IPl", textBoxIPl);
        }

    }
}
