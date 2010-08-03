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
    public partial class LexemeDataPanel : UserControl
    {
        public delegate void ShowDetails (LexemeDataPanel ldp);
        public event ShowDetails ShowDetailsEvent;

        public delegate void Save (LexemeDataPanel ldp);
        public event Save SaveEvent;

        public string sInitialForm
        {
            get
            {
                return textBoxInitFormValue.Text;
            }

            set
            {
                textBoxInitFormValue.Text = value;
            }
        }

        public string sGraphicStem
        {
            get
            {
                return textBoxGrStemValue.Text;
            }

            set
            {
                textBoxGrStemValue.Text = value;
            }
        }

        public string sMainSymbol
        {
            get
            {
                return textBoxMainSymbValue.Text;
            }

            set
            {
                textBoxMainSymbValue.Text = value;
            }
        }

        public string sType
        {
            get
            {
                return textBoxTypeValue.Text;
            }

            set
            {
                textBoxTypeValue.Text = value;
            }
        }

        public string sStressType
        {
            get
            {
                return textBoxStressTypeValue.Text;
            }

            set
            {
                textBoxStressTypeValue.Text = value;
            }
        }

        public string Advanced
        {
            get
            {
                return textBoxAdvancedValue.Text;
            }

            set
            {
                textBoxAdvancedValue.Text = value;
            }
        }

        public LexemeDataPanel()
        {
            InitializeComponent();
        }

        public void FocusShowButton()
        {
            buttonShowWF.Focus();
        }

        private void buttonShowWF_Click(object sender, EventArgs e)
        {
            if (ShowDetailsEvent != null)
            {
                ShowDetailsEvent (this);
            }
        }

        private void buttonSave_Click(object sender, EventArgs e)
        {
            if (SaveEvent != null)
            {
                SaveEvent (this);
            }
        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            TestApplet ta = (TestApplet)Parent.Parent.Parent;
//            ta.CloseCurrentTab();
            ta.CloseLexemeTabs (this);            
        }
    }
}
