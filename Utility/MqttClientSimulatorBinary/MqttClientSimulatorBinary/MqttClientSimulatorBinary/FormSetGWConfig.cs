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
    public partial class FormSetGWConfig : Form
    {

        private const int CBOR_PAYLOAD_VER = 257;   //in HEX 0x101 > Ver.1.01
        string target_file;
        public FormSetGWConfig()
        {
            InitializeComponent();
        }



        private void FormSetGWConfig_Load(object sender, EventArgs e)
        {
            this.CenterToScreen();

            this.target_file = @".\cbor_cloud\REQ_SET_GW_CONFIG.cbor";

            byte[] cbor_bytes = File.ReadAllBytes(this.target_file);

            CBORObject cbor_rx = CBORObject.DecodeFromBytes(cbor_bytes);

            string data = cbor_rx.ToString();
            string[] words = data.Split(',');
            int found;
            int found_stop;

            foreach (string word in words)
            {
                if (word.Contains(@"pva"))
                {
                    found = word.IndexOf(": ");
                    textBox_PVA.Text = word.Substring(found + 2);
                }

                if (word.Contains(@"pst"))
                {
                    found = word.IndexOf(": ");
                    textBox_PST.Text = word.Substring(found + 2); 
                }

                if (word.Contains(@"mka"))
                {
                    found = word.IndexOf(": ");
                    textBox_MKA.Text = word.Substring(found + 2);
                }

                if (word.Contains(@"lss"))
                {
                    found = word.IndexOf(": ");
                    textBox_LSS.Text = word.Substring(found + 2);
                }

                if (word.Contains(@"hss"))
                {
                    found = word.IndexOf(": ");
                    textBox_HSS.Text = word.Substring(found + 2);
                }

            }
        }



        private void buttonDevUpdateDataSave_Click_1(object sender, EventArgs e)
        {
            int value;
            var cbor = CBORObject.NewMap();

            cbor.Add(@"ver", CBOR_PAYLOAD_VER);
            cbor.Add(@"rto", @"SET_GW_CONFIG");

            value = 1;
            cbor.Add(@"cmd", value);

            value = 0;
            if (Int32.TryParse(textBox_PVA.Text, out value))
            {
                cbor.Add(@"pva", value);
            }
            else
            {
                MessageBox.Show("Error during conversion of > pva",
                "Error");
            }

            value = 0;
            if (Int32.TryParse(textBox_PST.Text, out value))
            {
                //TODO check non zero
                cbor.Add(@"pst", value);
            }
            else
            {
                MessageBox.Show("Error during conversion of > pst",
                                "Error");
            }

            value = 0;
            if (Int32.TryParse(textBox_MKA.Text, out value))
            {
                //TODO check non zero
                cbor.Add(@"mka", value);
            }
            else
            {
                MessageBox.Show("Error during conversion of > mka",
                                "Error");
            }

            value = 0;
            if (Int32.TryParse(textBox_LSS.Text, out value))
            {
                //TODO check non zero
                cbor.Add(@"lss", value);
            }
            else
            {
                MessageBox.Show("Error during conversion of > lss",
                                "Error");
            }

            value = 0;
            if (Int32.TryParse(textBox_HSS.Text, out value))
            {
                //TODO check non zero
                cbor.Add(@"hss", value);
            }
            else
            {
                MessageBox.Show("Error during conversion of > hss",
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

        private void buttonDevUpdateDataCancel_Click_1(object sender, EventArgs e)
        {
            this.Close();
        }


    }
}
