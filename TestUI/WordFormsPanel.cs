using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace TestUI
{
    public class WordFormsPanel : UserControl
    {
        public struct StCell
        {
            public TextBox textboxTarget;
            public HashSet<string> setContents;

            public StCell(TextBox textBox)
            {
                textboxTarget = textBox;
                setContents = new HashSet<string>();
            }
        }

        protected Dictionary<string, StCell> m_dictForms;

        protected void Clear()
        {
            foreach (KeyValuePair<string, StCell> entry in m_dictForms)
            {
                entry.Value.textboxTarget.Text.Remove(0);
            }
        }

        public void SetForm(string sKey, string sText)
        {
            if (m_dictForms[sKey].setContents.Contains(sText))
            {
                return;
            }

            m_dictForms[sKey].setContents.Add(sText);

            if (m_dictForms[sKey].textboxTarget.Text.Length > 0)
            {
                m_dictForms[sKey].textboxTarget.Text += "\r\n";
            }
            m_dictForms[sKey].textboxTarget.Text += sText;

            if (m_dictForms[sKey].textboxTarget.Lines.Length > 1)
            {
                m_dictForms[sKey].textboxTarget.ScrollBars = ScrollBars.Vertical;
            }
            else
            {
                m_dictForms[sKey].textboxTarget.ScrollBars = ScrollBars.None;
            }
        }
    }
}
