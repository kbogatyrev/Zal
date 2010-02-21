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
    public partial class VerbPanel : WordFormsPanel
    {
        public delegate void ShowParticipialForms (int iLexemeId, MainLib.ET_Subparadigm eoSubparadigm);
        public event ShowParticipialForms ShowParticipialFormsEvent;

        int iLexemeId;

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

        public VerbPanel (int iId)
        {
            InitializeComponent();

            iLexemeId = iId;

            m_dictForms = new Dictionary<string, StCell>();
            m_dictForms.Add ("Pres1Sg", new StCell (textBoxPres1Sg));
            m_dictForms.Add ("Pres2Sg", new StCell (textBoxPres2Sg));
            m_dictForms.Add ("Pres3Sg", new StCell (textBoxPres3Sg));
            m_dictForms.Add ("Pres1Pl", new StCell (textBoxPres1Pl));
            m_dictForms.Add ("Pres2Pl", new StCell (textBoxPres2Pl));
            m_dictForms.Add ("Pres3Pl", new StCell (textBoxPres3Pl));

            m_dictForms.Add("PastMSg", new StCell(textBoxPastMSg));
            m_dictForms.Add("PastFSg", new StCell(textBoxPastFSg));
            m_dictForms.Add("PastNSg", new StCell(textBoxPastNSg));
            m_dictForms.Add("PastPl", new StCell(textBoxPastPl));

            m_dictForms.Add("ImperativeSg", new StCell(textBoxImperativeSg));
            m_dictForms.Add("ImperativePl", new StCell(textBoxImperativePl));
            m_dictForms.Add("PartPresActive", new StCell(textBoxPartPresActive));            
        }

        private void btnShowPresActive_Click (object sender, EventArgs e)
        {
            if (ShowParticipialFormsEvent != null)
            {
                ShowParticipialFormsEvent (iLexemeId, MainLib.ET_Subparadigm.SUBPARADIGM_PARTICIPLE_PRESENT_ACTIVE);
            }
        }
    }
}
