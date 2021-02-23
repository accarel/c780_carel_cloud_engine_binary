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
    public partial class Form_Abort_Upload : Form
    {

        private const int CBOR_PAYLOAD_VER = 257;   //in HEX 0x101 > Ver.1.01
        string target_file;

        public Form_Abort_Upload()
        {
            InitializeComponent();
        }

        private void buttonUploadFileFromCtrlFull_Cancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void buttonUploadFileFromCtrlFull_Save_Click(object sender, EventArgs e)
        {
            int value;
            var cbor = CBORObject.NewMap();

            cbor.Add(@"ver", CBOR_PAYLOAD_VER);
            cbor.Add(@"rto", textBoxUploadFile_rid.Text);

            value = 21;
            cbor.Add(@"cmd", value);


            if (textBoxUploadFile_rid.TextLength > 0)
            { 
               cbor.Add(@"rid", textBoxUploadFile_rid.Text);
            }            
            else
            {
                MessageBox.Show("Error during conversion of > rid",
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

        private void Form_Abort_Upload_Load(object sender, EventArgs e)
        {
            this.CenterToScreen();

            this.target_file = @".\cbor_cloud\REQ_abort_upload.cbor";

            try
            {
                byte[] cbor_bytes = File.ReadAllBytes(this.target_file);

                CBORObject cbor_rx = CBORObject.DecodeFromBytes(cbor_bytes);

                string data = cbor_rx.ToString();
                string[] words = data.Split(',');
                int found;

                foreach (string word in words)
                {

                    if (word.Contains(@"rid"))
                    {
                        found = word.IndexOf(": ");
                        string st = word.Substring(found + 2);
                        st = st.Substring(1, st.Length - 2);
                        textBoxUploadFile_rid.Text = st;
                    }

                }

            }
            catch
            {

            }
        }

    }
}
