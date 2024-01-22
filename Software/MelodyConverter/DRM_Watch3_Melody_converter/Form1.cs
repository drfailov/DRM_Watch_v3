using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;

namespace DRM_Watch3_Melody_converter
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            label1.Text = "" +
                "Result is array if numbers." +
                "\n If number is > 20 this is tone in Hz" +
                "\n If number is = 19 this is EOF" +
                "\n If number is < 0 this is delay in ms";
        }

        private void buttonConvert_Click(object sender, EventArgs e)
        {
            richTextBoxDst.Text = "";
            string srcString = richTextBoxSrc.Text;
            string dstString = "const int melody[]={";
            string[] srcLines = srcString.Split('\n');
            foreach (string line in srcLines)
            {
                if (line.Trim().StartsWith("\\"))
                {
                    //ignore it
                }
                else if (line.Contains("tone"))
                {
                    dstString += Regex.Match(line, @"\d+").Value + ", ";
                }
                else if (line.Contains("delay"))
                {
                    dstString += "-"+ Regex.Match(line, @"\d+").Value + ", ";
                }
            }
            dstString += "19";
            richTextBoxDst.Text = dstString.Trim().Trim(',') + "};";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            richTextBoxDst.Text = richTextBoxDst.Text.Replace(", -10,", ",");
        }
    }
}
