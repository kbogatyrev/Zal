﻿using System;
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

        public VerbPanel()
        {
            InitializeComponent();

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
        }
    }
}
