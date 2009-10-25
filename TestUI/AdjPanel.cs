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
        Dictionary<string, TextBox> m_dictForms;

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

        public void setForm (string sKey, string sText)
        {
            m_dictForms[sKey].Text = sText;
        }

        public AdjPanel()
        {
            InitializeComponent();

            m_dictForms = new Dictionary<string, TextBox>();
            m_dictForms.Add("MNSg", textBoxMNSg);
            m_dictForms.Add("MASgInanim", textBoxMASgInanim);
            m_dictForms.Add("MASgAnim", textBoxMASgAnim);
            m_dictForms.Add("MGSg", textBoxMGSg);
            m_dictForms.Add("MLSg", textBoxMLSg);
            m_dictForms.Add("MDSg", textBoxMDSg);
            m_dictForms.Add("MISg", textBoxMISg);

            m_dictForms.Add("FNSg", textBoxFNSg);
            m_dictForms.Add("FASg", textBoxFASg);
            m_dictForms.Add("FGSg", textBoxFGSg);
            m_dictForms.Add("FLSg", textBoxFLSg);
            m_dictForms.Add("FDSg", textBoxFDSg);
            m_dictForms.Add("FISg", textBoxFISg);

            m_dictForms.Add("NNSg", textBoxNNSg);
            m_dictForms.Add("NASg", textBoxNASg);
            m_dictForms.Add("NGSg", textBoxNGSg);
            m_dictForms.Add("NLSg", textBoxNLSg);
            m_dictForms.Add("NDSg", textBoxNDSg);
            m_dictForms.Add("NISg", textBoxNISg);

            m_dictForms.Add("NPl", textBoxNPl);
            m_dictForms.Add("APlInanim", textBoxAPlInanim);
            m_dictForms.Add("APlAnim", textBoxAPlAnim);
            m_dictForms.Add("GPl", textBoxGPl);
            m_dictForms.Add("LPl", textBoxLPl);
            m_dictForms.Add("DPl", textBoxDPl);
            m_dictForms.Add("IPl", textBoxIPl);

        }
    }
}
