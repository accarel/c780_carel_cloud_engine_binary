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
    public partial class FormDevUpdateData : Form
    {
        private const int CBOR_PAYLOAD_VER = 257;   //in HEX 0x101 > Ver.1.01

        string target_file;
        public FormDevUpdateData()
        {
            InitializeComponent();
        }

        private void buttonDevUpdateDataSave_Click(object sender, EventArgs e)
        {
            int value;
            var cbor = CBORObject.NewMap();

            cbor.Add(@"ver", CBOR_PAYLOAD_VER);
            cbor.Add(@"rto", @"update_dev_firmware");
                        
            value = 11;
            cbor.Add(@"cmd", value);

            cbor.Add(@"usr", textBoxDevUpdateData_USR.Text);
            cbor.Add(@"pwd", textBoxDevUpdateData_PWD.Text);
            cbor.Add(@"uri", textBoxDevUpdateData_URL.Text);

            value = 0;
            if (Int32.TryParse(textBoxDevUpdateData_fid.Text, out value))
            {
                cbor.Add(@"fid", value);
            }
            else
            {
                MessageBox.Show("Error during conversion of > fid",
                "Error");
            }

            value = 0;
            if (Int32.TryParse(textBoxDevUpdateData_wet.Text, out value))
            {
                cbor.Add(@"wet", value);
            }
            else
            {
                MessageBox.Show("Error during conversion of > wet",
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
        


        private void FormDevUpdateData_Load(object sender, EventArgs e)
        {
            this.CenterToScreen();

            this.target_file = @".\cbor_cloud\REQ_UPDATE_DEV_FIRMWARE.cbor";
      
            byte[] cbor_bytes = File.ReadAllBytes(this.target_file);
            
            CBORObject cbor_rx = CBORObject.DecodeFromBytes(cbor_bytes);

            string data = cbor_rx.ToString();
            string[] words = data.Split(',');
            int found;

            foreach (string word in words)
            {
                if (word.Contains(@"fid"))
                {
                    found = word.IndexOf(": ");
                    textBoxDevUpdateData_fid.Text = word.Substring(found + 2) ;                   
                }

                if (word.Contains(@"wet"))
                {
                    found = word.IndexOf(": ");
                    textBoxDevUpdateData_wet.Text = word.Substring(found + 2, (word.Length - found - 2 - 1));
                }


                if (word.Contains(@"uri"))
                {
                    found = word.IndexOf(": ");
                    textBoxDevUpdateData_URL.Text = word.Substring(found + 3, (word.Length - found - 3 - 1));
                }

                if (word.Contains(@"pwd"))
                {
                    found = word.IndexOf(": ");
                    textBoxDevUpdateData_PWD.Text = word.Substring(found + 3, (word.Length - found - 3 - 1));
                }

                if (word.Contains(@"rto"))
                {
                    found = word.IndexOf(": ");                    
                    //textBoxDevUpdateData_fid.Text = word.Substring(found + 2);
                }

                if (word.Contains(@"usr"))
                {
                    found = word.IndexOf(": ");
                    textBoxDevUpdateData_USR.Text = word.Substring(found + 3, (word.Length - found - 3 - 1));
                }

            }

        }




        private void buttonDevUpdateDataCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
