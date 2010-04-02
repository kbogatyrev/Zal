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
    public partial class AdjPanel : WordFormsPanel
    {
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
        public AdjPanel()
        {
            InitializeComponent();

            m_dictForms = new Dictionary<string, StCell>();
            m_dictForms.Add("MNSg", new StCell (textBoxMNSg));
            m_dictForms.Add("MASgInanim", new StCell (textBoxMASgInanim));
            m_dictForms.Add("MASgAnim", new StCell (textBoxMASgAnim));
            m_dictForms.Add("MGSg", new StCell (textBoxMGSg));
            m_dictForms.Add("MLSg", new StCell (textBoxMLSg));
            m_dictForms.Add("MDSg", new StCell (textBoxMDSg));
            m_dictForms.Add("MISg", new StCell (textBoxMISg));

            m_dictForms.Add("FNSg", new StCell (textBoxFNSg));
            m_dictForms.Add("FASg", new StCell (textBoxFASg));
            m_dictForms.Add("FGSg", new StCell (textBoxFGSg));
            m_dictForms.Add("FLSg", new StCell (textBoxFLSg));
            m_dictForms.Add("FDSg", new StCell (textBoxFDSg));
            m_dictForms.Add("FISg", new StCell (textBoxFISg));

            m_dictForms.Add("NNSg", new StCell (textBoxNNSg));
            m_dictForms.Add("NASg", new StCell (textBoxNASg));
            m_dictForms.Add("NGSg", new StCell (textBoxNGSg));
            m_dictForms.Add("NLSg", new StCell (textBoxNLSg));
            m_dictForms.Add("NDSg", new StCell (textBoxNDSg));
            m_dictForms.Add("NISg", new StCell (textBoxNISg));

            m_dictForms.Add("NPl", new StCell (textBoxNPl));
            m_dictForms.Add("APlInanim", new StCell (textBoxAPlInanim));
            m_dictForms.Add("APlAnim", new StCell (textBoxAPlAnim));
            m_dictForms.Add("GPl", new StCell (textBoxGPl));
            m_dictForms.Add("LPl", new StCell (textBoxLPl));
            m_dictForms.Add("DPl", new StCell (textBoxDPl));
            m_dictForms.Add("IPl", new StCell (textBoxIPl));

            m_dictForms.Add("ShortMSg", new StCell (textBoxShortM));
            m_dictForms.Add("ShortFSg", new StCell (textBoxShortF));
            m_dictForms.Add("ShortNSg", new StCell (textBoxShortN));
            m_dictForms.Add("ShortPl", new StCell (textBoxShortPl));

            m_dictForms.Add("Comparative", new StCell (textBoxComparative));
        }
    }
}
