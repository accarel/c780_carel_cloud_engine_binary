using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

using PeterO;
using PeterO.Cbor;
using PeterO.Numbers;

namespace MqttClientSimulatorBinary
{
    public partial class FormCBORUpdateLinesConfig : Form
    {
        private const int CBOR_PAYLOAD_VER = 257;   //in HEX 0x101 > Ver.1.01
        string target_file;

        public FormCBORUpdateLinesConfig()
        {
            InitializeComponent();
        }

        private void buttonUpdLinesConigCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void FormCBORUpdateLinesConfig_Load(object sender, EventArgs e)
        {
            this.CenterToScreen();

            this.target_file = @".\cbor_cloud\REQ_SET_LINES_CONFIG.cbor";

            byte[] cbor_bytes = File.ReadAllBytes(this.target_file);

            CBORObject cbor_rx = CBORObject.DecodeFromBytes(cbor_bytes);

            string data = cbor_rx.ToString();
            string[] words = data.Split(',');
            int found;
            int found_stop;

            foreach (string word in words)
            {
                if (word.Contains(@"bau"))
                {
                    found = word.IndexOf(":");
                    textBox_bau.Text = word.Substring(found + 2);
                }

                if (word.Contains(@"con"))
                {
                    int ls;
                    found = word.IndexOf(":");
                    found_stop = word.IndexOf("}");

                    if (found_stop == -1)
                    {
                        found_stop = 0;
                        ls = (word.Length - found);
                        textBox_con.Text = word.Substring(found + 1);
                    }
                    else
                    {
                        ls = (word.Length - found) - (word.Length - found_stop);
                        textBox_con.Text = word.Substring(found + 1, ls);
                    }

                }


                if (word.Contains(@"del"))
                {
                    int ls;
                    found = word.IndexOf(":");
                    found_stop = word.IndexOf("}");

                    if (found_stop == -1)
                    {
                        found_stop = 0;
                        ls = (word.Length - found);
                        textBox_del.Text = word.Substring(found + 1);
                    }
                    else
                    {
                        ls = (word.Length - found - 1) - (word.Length - found_stop);
                        textBox_del.Text = word.Substring(found + 1, ls);
                    }

                }

            }


        }

        private void buttonUpdLinesConigSave_Click(object sender, EventArgs e)
        {
            int value;
            var cbor = CBORObject.NewMap();

            cbor.Add(@"ver", CBOR_PAYLOAD_VER);
            cbor.Add(@"rto", @"lcfg");

            value = 4;

            cbor.Add(@"cmd", value);


            value = 0;
            if (Int32.TryParse(textBox_bau.Text, out value))
            {
                cbor.Add(@"bau", value);
            }
            else
            {
                MessageBox.Show("Error during conversion of > bau",
                "Error");
            }


            value = 0;
            if (Int32.TryParse(textBox_con.Text, out value))
            {
                cbor.Add(@"con", value);
            }
            else
            {
                MessageBox.Show("Error during conversion of > con",
                "Error");
            }

            value = 0;
            if (Int32.TryParse(textBox_del.Text, out value))
            {
                cbor.Add(@"del", value);
            }
            else
            {
                MessageBox.Show("Error during conversion of > did",
                "Error");
            }



            // The following converts the map to canonical CBOR
            byte[] cbor_bytes = cbor.EncodeToBytes(CBOREncodeOptions.DefaultCtap2Canonical);

            //create the file
            BinaryWriter bw;

            try
            {
                bw = new BinaryWriter(new FileStream(this.target_file, FileMode.Create));
            }
            catch (IOException ecc)
            {
                Console.WriteLine(ecc.Message + "\n Cannot create file.");
                return;
            }

            //writing into the file
            try
            {
                int count = 0;

                foreach (byte belement in cbor_bytes)
                {
                    count++;
                    bw.Write(belement);
                }

            }
            catch (IOException ecc)
            {
                MessageBox.Show("Message NOT saved ! " + ecc.Message, "Error");
                return;
            }
            bw.Close();

            this.Close();
        }
    }
}
