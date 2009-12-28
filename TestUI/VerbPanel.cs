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
            m_dictForms.Add ("1Sg", new StCell (textBox1Sg));
            m_dictForms.Add ("2Sg", new StCell (textBox2Sg));
            m_dictForms.Add ("3Sg", new StCell (textBox3Sg));
            m_dictForms.Add ("1Pl", new StCell (textBox1Pl));
            m_dictForms.Add ("2Pl", new StCell (textBox2Pl));
            m_dictForms.Add ("3Pl", new StCell (textBox3Pl));
        }
    }
}
