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
        public delegate void ShowDetails (int iLexemeId);
        public event ShowDetails ShowDetailsEvent;

        public delegate void Save(int iLexemeId);
        public event Save SaveEvent;

        int iLexemeId;

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

        public LexemeDataPanel (int iId)
        {
            InitializeComponent();
            iLexemeId = iId;
        }

        private void buttonShowWF_Click (object sender, EventArgs e)
        {
            if (ShowDetailsEvent != null)
            {
                ShowDetailsEvent(iLexemeId);
            }
        }

        private void buttonSave_Click(object sender, EventArgs e)
        {
            if (SaveEvent != null)
            {
                SaveEvent(iLexemeId);
            }
        }
    }
}
