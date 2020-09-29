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
    public partial class FormCBORUpdateSetDevsConfig : Form
    {
        private const int CBOR_PAYLOAD_VER = 257;   //in HEX 0x101 > Ver.1.01
        string target_file;

        public FormCBORUpdateSetDevsConfig()
        {
            InitializeComponent();
        }

        private void buttonDevUpdateDataCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void buttonDevUpdateDataSave_Click(object sender, EventArgs e)
        {
            int value;
            var cbor = CBORObject.NewMap();

            cbor.Add(@"ver", CBOR_PAYLOAD_VER);
            cbor.Add(@"rto", @"setdvconfig");

            value = 5;

            cbor.Add(@"cmd", value);

            cbor.Add(@"usr", textBoxSetDevConfig_USR.Text);
            cbor.Add(@"pwd", textBoxSetDevConfig_PWD.Text);
            cbor.Add(@"uri", textBoxSetDevConfig_URL.Text);

            value = 0;
            if (Int32.TryParse(textBoxSetDevConfig_dev.Text, out value))
            {
                cbor.Add(@"dev", value);
            }
            else
            {
                MessageBox.Show("Error during conversion of > dev",
                "Error");
            }


            value = 0;
            if (Int32.TryParse(textBoxSetDevConfig_cid.Text, out value))
            {
                cbor.Add(@"cid", value);
            }
            else
            {
                MessageBox.Show("Error during conversion of > cid",
                "Error");
            }

            value = 0;
            if (Int32.TryParse(textBoxSetDevConfig_did.Text, out value))
            {
                cbor.Add(@"did", value);
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

        private void FormCBORUpdateSetDevsConfig_Load(object sender, EventArgs e)
        {
            this.CenterToScreen();

            this.target_file = @".\cbor_cloud\REQ_DOWNLOAD_DEVS_CONFIG.cbor";

            byte[] cbor_bytes = File.ReadAllBytes(this.target_file);

            CBORObject cbor_rx = CBORObject.DecodeFromBytes(cbor_bytes);

            string data = cbor_rx.ToString();
            string[] words = data.Split(',');
            int found;
            int found_stop;

            foreach (string word in words)
            {
                if (word.Contains(@"did"))
                {
                    found = word.IndexOf(": ");
                    textBoxSetDevConfig_did.Text = word.Substring(found + 2);
                }

                if (word.Contains(@"cid"))
                {
                    int ls;
                    found = word.IndexOf(":");
                    found_stop = word.IndexOf("}");

                    if (found_stop == -1)
                    {
                        found_stop = 0;
                        ls = (word.Length - found);
                        textBoxSetDevConfig_cid.Text = word.Substring(found + 1);
                    }
                    else
                    {
                        ls = (word.Length - found) - (word.Length - found_stop);
                        textBoxSetDevConfig_cid.Text = word.Substring(found + 1, ls);
                    }

                }


                if (word.Contains(@"uri"))
                {
                    found = word.IndexOf(": ");
                    textBoxSetDevConfig_URL.Text = word.Substring(found + 3, (word.Length - found - 3 - 1));
                }

                if (word.Contains(@"pwd"))
                {
                    found = word.IndexOf(": ");
                    textBoxSetDevConfig_PWD.Text = word.Substring(found + 3, (word.Length - found - 3 - 1));
                }

                if (word.Contains(@"rto"))
                {
                    found = word.IndexOf(": ");
                    //textBoxDevUpdateData_fid.Text = word.Substring(found + 2);
                }

                if (word.Contains(@"usr"))
                {
                    found = word.IndexOf(": ");
                    textBoxSetDevConfig_USR.Text = word.Substring(found + 3, (word.Length - found - 3 - 1));
                }

                if (word.Contains(@"dev"))
                {
                    found = word.IndexOf(":");
                    textBoxSetDevConfig_dev.Text = word.Substring(found + 1, (word.Length - (found + 1)));
                }

            }
        }
    }
}
