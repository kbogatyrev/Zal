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
    public partial class Synthesizer : Form
    {
        public Synthesizer()
        {
            InitializeComponent();
        }

        private void graphicStemToolStripMenuItem_Click(object sender, EventArgs e)
        {
            EnterDataDlg edd = new EnterDataDlg();
            DialogResult dr = edd.ShowDialog();

            if (DialogResult.OK != dr)
            {
                return;
            }

            MainLib.IDictionary dict = new MainLib.ZalDictionary();
            dict.Lexemes (edd.sLexeme);
            foreach (MainLib.ILexeme lex in dict)
            {
                string grSt = lex.GraphicStem;
                lex.WordForms();

                if (MainLib.ET_PartOfSpeech.POS_NOUN == lex.PartOfSpeech)
                {
                    NounPanel np = new NounPanel();
                    panelWF.Controls.Add(np);
                    np.sLexName = grSt;

                    foreach (MainLib.IWordForm wf in lex)
                    {
                        string sKey = "";
                        switch (wf.Case)
                        {
                            case MainLib.ET_Case.CASE_NOM:
                            {
                                sKey = "N";
                                break;
                            }
                            case MainLib.ET_Case.CASE_ACC:
                            {
                                sKey = "A";
                                break;
                            }
                            case MainLib.ET_Case.CASE_GEN:
                            {
                                sKey = "G";
                                break;
                            }
                            case MainLib.ET_Case.CASE_LOC:
                            {
                                sKey = "L";
                                break;
                            }
                            case MainLib.ET_Case.CASE_DAT:
                            {
                                sKey = "D";
                                break;
                            }
                            case MainLib.ET_Case.CASE_INST:
                            {
                                sKey = "I";
                                break;
                            }
                        }

                        sKey += (wf.Number == MainLib.ET_Number.NUM_SG) ? "Sg" : "Pl";
                        np.setForm(sKey, wf.Wordform);
                    }
                }

                if (MainLib.ET_PartOfSpeech.POS_ADJ == lex.PartOfSpeech)
                {
                    AdjPanel ap = new AdjPanel();
                    panelWF.Controls.Add(ap);
                    ap.sLexName = grSt;

                    foreach (MainLib.IWordForm wf in lex)
                    {
                        string sKey = "";
                        switch (wf.Gender)
                        {
                            case MainLib.ET_Gender.GENDER_M:
                            {
                                sKey = "M";
                                break;
                            }
                            case MainLib.ET_Gender.GENDER_F:
                            {
                                sKey = "F";
                                break;
                            }
                            case MainLib.ET_Gender.GENDER_N:
                            {
                                sKey = "N";
                                break;
                            }

                        }
                        switch (wf.Case)
                        {
                            case MainLib.ET_Case.CASE_NOM:
                            {
                                sKey += "N";
                                break;
                            }
                            case MainLib.ET_Case.CASE_ACC:
                            {
                                sKey += "A";
                                break;
                            }
                            case MainLib.ET_Case.CASE_GEN:
                            {
                                sKey += "G";
                                break;
                            }
                            case MainLib.ET_Case.CASE_LOC:
                            {
                                sKey += "L";
                                break;
                            }
                            case MainLib.ET_Case.CASE_DAT:
                            {
                                sKey += "D";
                                break;
                            }
                            case MainLib.ET_Case.CASE_INST:
                            {
                                sKey += "I";
                                break;
                            }
                        }

                        sKey += (wf.Number == MainLib.ET_Number.NUM_SG) ? "Sg" : "Pl";
                        ap.setForm(sKey, wf.Wordform);

                    }   // foreach ...

                }
            }
        }
    }
}
