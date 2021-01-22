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
    public partial class FormUploadFileFromControllerRange : Form
    {

        private const int CBOR_PAYLOAD_VER = 257;   //in HEX 0x101 > Ver.1.01
        string target_file;

        public FormUploadFileFromControllerRange()
        {
            InitializeComponent();
        }

        private void buttonUploadFileFromController_Save_Click(object sender, EventArgs e)
        {
            int value;
            var cbor = CBORObject.NewMap();

            cbor.Add(@"ver", CBOR_PAYLOAD_VER);
            cbor.Add(@"rto", textBoxUploadFile_rto.Text);

            value = 20;
            cbor.Add(@"cmd", value);


            value = 0;
            if (Int32.TryParse(textBoxUploadFile_fid.Text, out value))
            {
                cbor.Add(@"fid", value);
            }
            else
            {
                MessageBox.Show("Error during conversion of > fid",
                "Error");
            }


            value = 0;
            if (Int32.TryParse(textBoxUploadFile_fst.Text, out value))
            {
                cbor.Add(@"fst", value);
            }
            else
            {
                MessageBox.Show("Error during conversion of > fst",
                "Error");
            }

            value = 0;
            if (Int32.TryParse(textBoxUploadFile_fle.Text, out value))
            {
                cbor.Add(@"fle", value);
            }
            else
            {
                MessageBox.Show("Error during conversion of > fle",
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

        private void FormUploadFileFromController_Load(object sender, EventArgs e)
        {
            this.CenterToScreen();

            this.target_file = @".\cbor_cloud\REQ_upload_range_file.cbor";

            try
            {
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
                        textBoxUploadFile_fid.Text = word.Substring(found + 2);
                    }

                    if (word.Contains(@"fst"))
                    {
                        found = word.IndexOf(": ");
                        textBoxUploadFile_fst.Text = word.Substring(found + 2);
                    }

                    if (word.Contains(@"fle"))
                    {
                        found = word.IndexOf(": ");
                        textBoxUploadFile_fle.Text = word.Substring(found + 2);
                    }


                    if (word.Contains(@"rto"))
                    {
                        found = word.IndexOf(": ");
                        string st = word.Substring(found + 2);                       
                        st = st.Substring(1, st.Length - 2);
                        textBoxUploadFile_rto.Text = st;
                    }

                }

            }
            catch 
            { 
            
            }

            }


    }
}
