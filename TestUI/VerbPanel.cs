using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using MainLibManaged;

namespace TestUI
{
    public partial class VerbPanel : WordFormsPanel
    {
        public delegate void ShowParticipialForms (CLexemeManaged lexeme, 
                                                   EM_Subparadigm eoSubparadigmLong,
                                                   EM_Subparadigm eoSubparadigmShort);
        public event ShowParticipialForms ShowParticipialFormsEvent;

        //int iLexemeId;
        CLexemeManaged Lexeme;

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

        public VerbPanel (CLexemeManaged lexeme)
        {
            InitializeComponent();

//            iLexemeId = iId;
            Lexeme = lexeme;

            m_dictForms = new Dictionary<string, StCell>();
            m_dictForms.Add ("Pres1Sg", new StCell (textBoxPres1Sg));
            m_dictForms.Add("Pres2Sg", new StCell (textBoxPres2Sg));
            m_dictForms.Add("Pres3Sg", new StCell (textBoxPres3Sg));
            m_dictForms.Add("Pres1Pl", new StCell (textBoxPres1Pl));
            m_dictForms.Add("Pres2Pl", new StCell (textBoxPres2Pl));
            m_dictForms.Add("Pres3Pl", new StCell (textBoxPres3Pl));

            m_dictForms.Add("PastMSg", new StCell (textBoxPastMSg));
            m_dictForms.Add("PastFSg", new StCell (textBoxPastFSg));
            m_dictForms.Add("PastNSg", new StCell (textBoxPastNSg));
            m_dictForms.Add("PastPl", new StCell (textBoxPastPl));

            m_dictForms.Add("ImperativeSg", new StCell (textBoxImperativeSg));
            m_dictForms.Add("ImperativePl", new StCell (textBoxImperativePl));
            m_dictForms.Add("PartPresActive", new StCell (textBoxPartPresActive));
            m_dictForms.Add("PresAdverbial", new StCell (textBoxPresAdverbial));
            m_dictForms.Add("PartPastActive", new StCell (textBoxPartPastActive));
            m_dictForms.Add("PastAdverbial", new StCell (textBoxPastAdverbial));
            m_dictForms.Add("PartPresPassive", new StCell (textBoxPartPresPassive));
            m_dictForms.Add("PartPastPassive", new StCell (textBoxPartPastPassive));
        }

        private void btnShowPresActive_Click (object sender, EventArgs e)
        {
            if (ShowParticipialFormsEvent != null)
            {
                ShowParticipialFormsEvent (Lexeme, 
                                           EM_Subparadigm.SUBPARADIGM_PART_PRES_ACT,
                                           EM_Subparadigm.SUBPARADIGM_UNDEFINED);
            }
        }

        private void btnShowPastActive_Click (object sender, EventArgs e)
        {
            if (ShowParticipialFormsEvent != null)
            {
                ShowParticipialFormsEvent (Lexeme,
                                           EM_Subparadigm.SUBPARADIGM_PART_PAST_ACT,
                                           EM_Subparadigm.SUBPARADIGM_UNDEFINED);
            }
        }

        private void btnShowPresPassive_Click (object sender, EventArgs e)
        {
            if (ShowParticipialFormsEvent != null)
            {
                ShowParticipialFormsEvent (Lexeme, 
                                           EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_LONG,
                                           EM_Subparadigm.SUBPARADIGM_PART_PRES_PASS_SHORT);
            }
        }

        private void btnShowPastPassive_Click (object sender, EventArgs e)
        {
            if (ShowParticipialFormsEvent != null)
            {
                ShowParticipialFormsEvent (Lexeme, 
                                           EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_LONG,
                                           EM_Subparadigm.SUBPARADIGM_PART_PAST_PASS_SHORT);
            }
        }
    }
}
