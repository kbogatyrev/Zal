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
    public partial class AdjPanel : UserControl
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

        public void setForm (string sGenderNumberCase, string sText)
        {
            dictForms[sGenderNumberCase].Text = sText;
        }

        public AdjPanel()
        {
            InitializeComponent();

            dictForms = new Dictionary<string, TextBox>();
            dictForms.Add("MNSg", textBoxMNSg);
            dictForms.Add("MASg", textBoxMASg);
            dictForms.Add("MGSg", textBoxMGSg);
            dictForms.Add("MLSg", textBoxMLSg);
            dictForms.Add("MDSg", textBoxMDSg);
            dictForms.Add("MISg", textBoxMISg);

            dictForms.Add("MNPl", textBoxMNPl);
            dictForms.Add("MAPl", textBoxMAPl);
            dictForms.Add("MGPl", textBoxMGPl);
            dictForms.Add("MLPl", textBoxMLPl);
            dictForms.Add("MDPl", textBoxMDPl);
            dictForms.Add("MIPl", textBoxMIPl);

            dictForms.Add("FNSg", textBoxFNSg);
            dictForms.Add("FASg", textBoxFASg);
            dictForms.Add("FGSg", textBoxFGSg);
            dictForms.Add("FLSg", textBoxFLSg);
            dictForms.Add("FDSg", textBoxFDSg);
            dictForms.Add("FISg", textBoxFISg);

            dictForms.Add("FNPl", textBoxFNPl);
            dictForms.Add("FAPl", textBoxFAPl);
            dictForms.Add("FGPl", textBoxFGPl);
            dictForms.Add("FLPl", textBoxFLPl);
            dictForms.Add("FDPl", textBoxFDPl);
            dictForms.Add("FIPl", textBoxFIPl);

            dictForms.Add("NNSg", textBoxNNSg);
            dictForms.Add("NASg", textBoxNASg);
            dictForms.Add("NGSg", textBoxNGSg);
            dictForms.Add("NLSg", textBoxNLSg);
            dictForms.Add("NDSg", textBoxNDSg);
            dictForms.Add("NISg", textBoxNISg);

            dictForms.Add("NNPl", textBoxNNPl);
            dictForms.Add("NAPl", textBoxNAPl);
            dictForms.Add("NGPl", textBoxNGPl);
            dictForms.Add("NLPl", textBoxNLPl);
            dictForms.Add("NDPl", textBoxNDPl);
            dictForms.Add("NIPl", textBoxNIPl);

        }
    }
}
