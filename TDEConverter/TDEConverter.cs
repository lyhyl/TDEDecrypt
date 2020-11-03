using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TDEConverter
{
    public partial class TDEConverter : Form
    {
        public TDEConverter()
        {
            InitializeComponent();
        }

        private List<string> fileNames = new List<string>();
        private const string a = "599Cc51887A8cb0C20F9CdE34cf9e0A535E5cAd1C26c7b562596ACC207Ae9A0bfB3E3161f31af5bEf1c2f064b3628174D83BF6E0739D9D6918fD9C2Eba02D5aD";
        private const string b = "0C3b676fe8d7188Cde022F71632830F36b98b181aD48Fed160006eA59";

        private void button1_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog dialog = new OpenFileDialog())
            {
                dialog.Multiselect = true;
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    fileNames.AddRange(dialog.FileNames);
                    listBox1.Items.AddRange(dialog.FileNames.Select(n => n.EndsWith(".tde") ? n + " (decrypt)" : n + " (encrypt)").ToArray());
                }
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            List<string> errors = new List<string>();
            foreach (var fileName in fileNames)
            {
                try
                {
                    Convert(fileName);
                }
                catch (Exception ex)
                {
                    errors.Add(ex.Message);
                }
            }
            if (errors.Count != 0)
            {
                MessageBox.Show(string.Join(Environment.NewLine, errors), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                MessageBox.Show("All Done", "Done", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            listBox1.Items.Clear();
        }
        private void Convert(string fileName)
        {
            string outpath = fileName.EndsWith(".tde") ? fileName.Substring(0, fileName.Length - 4) : fileName + ".tde";
            using (FileStream fin =new FileStream(fileName,FileMode.Open), fout =new FileStream(outpath,FileMode.Create))
            {
                int dat, i = 0;
                while ((dat = fin.ReadByte()) != -1)
                {
                    var d = (byte)dat ^ (byte)a[i % a.Length] ^ (byte)b[i % b.Length];
                    fout.WriteByte((byte)d);
                    i++;
                }
            }
        }
    }
}
