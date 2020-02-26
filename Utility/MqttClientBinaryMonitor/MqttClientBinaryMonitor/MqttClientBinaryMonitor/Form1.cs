using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Net;
using System.Net.Sockets;

using System.Diagnostics;

using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Exceptions;
using uPLibrary.Networking.M2Mqtt.Internal;
using uPLibrary.Networking.M2Mqtt.Messages;
using uPLibrary.Networking.M2Mqtt.Session;
using uPLibrary.Networking.M2Mqtt.Utility;

using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;
using System.Net.Security;

using Newtonsoft.Json;

using System.IO;
//using System.Json;
using PeterO;
using PeterO.Cbor;
using PeterO.Numbers;




namespace MqttClientSimulatorBinary
{

    public partial class Form1 : Form
    {
        const string JSON_VALIDATOR = "http://cbor.me/";
        // "https://jsonformatter.org/";    
        // "https://jsonformatter.curiousconcept.com/"

        const string MQTT_BROKER_ADDRESS = "192.168.16.102";

        const string SUB_TEMP  = "alessandro_bilato/f/temperature";
        const string SUB_RELE1 = "alessandro_bilato/f/rele1";
        const string PUB_RELE1 = SUB_RELE1;

        const string SUB_TEMP_carel = "sonda";
        const string PUB_VAL_carel  = "pco/topic/valore";


        const string VAL_REQ_carel  = @"/req";
        public string val_req_post;

        //const string VAL_RESP_carel = @"/res/53BCE4F1DFA0FE8E7CA126F91B35D3A6";

        public string VAL_RESP_carel = @"/res/53BCE4F1DFA0FE8E7CA126F91B35D3A6";
        public string VAL_RESP_carel_ALL = @"#";

        public string VAL_TOPIC_CONNECTED = @"";
        public string VAL_TOPIC_STATUS = @"";

        public string VAL_SUB_TARGET_POSTFIX = @"/+";


        public const int CBOR_PAYLOAD_VER = 257;   //in HEX 0x101 > Ver.1.01



        //static int value = 0;

        public string server_url;
        public int server_port;

        MqttClient client;
        public bool MQTT_Connect;

        public double start_time, stop_time;

        public double start_time_res2res;
        public bool start_stop_res2res_flag = false;

        string Myjson = null;

        public int msg_line_count = 1;

        public int targets_num;
        public string [] targets_names = new string[50];
        




        private void MessageBoxInfo(string message, string caption)
        {
            // Checks the value of the text.
            if (checkBox_ShowMsg.Checked == true)
            {
                // Initializes the variables to pass to the MessageBox.Show method.
                MessageBoxButtons buttons = MessageBoxButtons.OK; //MessageBoxButtons.YesNo;
                DialogResult result;

                // Displays the MessageBox.
                result = MessageBox.Show(message, caption, buttons);
                if (result == System.Windows.Forms.DialogResult.Yes)
                {

                }
            }
        }
        
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            Save_Last_Used_Values();

            if (MQTT_Connect == true)
            {
                client.Disconnect();
            }
        }

        public Form1()
        {
            InitializeComponent();

            //Load settings






            FormClosed += Form1_FormClosed;

            //System.Net.IPAddress IPAddress = System.Net.IPAddress.Parse(MQTT_BROKER_ADDRESS);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Debug.WriteLine("MessageId 1");
            Load_Last_Used_Values();
        }


        private void Timer_Pub2Res(bool start_stop)
        {

            string nstr;

            if (start_stop == false)
            {
                start_time = (new TimeSpan(DateTime.Now.Ticks)).TotalMilliseconds;
                nstr = @"-----";
                txtConsole.Invoke(new Action(() => textBox_timer_pub2res.Text = nstr));
            }
            else
            {
                stop_time = (new TimeSpan(DateTime.Now.Ticks)).TotalMilliseconds;
                double totalMill = stop_time - start_time;
            

                try
                {                    
                    nstr = totalMill.ToString();
                    txtConsole.Invoke(new Action(() => textBox_timer_pub2res.Text=nstr));                 
                }
                catch (System.InvalidOperationException)
                {
                    //Debug.WriteLine("-->ERROR ERROR ERROR");
                    MessageBoxUpdated("Timer_Pub2Res Error!");
                }

            }

        }




        private void Timer_Res2Res(bool start_stop)
        {

            string nstr;
            

            if (start_stop_res2res_flag == false)
            {
                start_time_res2res = (new TimeSpan(DateTime.Now.Ticks)).TotalMilliseconds;
                nstr = @"-----";
                txtConsole.Invoke(new Action(() => textBox_Res2Res.Text = nstr));
                start_stop_res2res_flag = true;
            }
            else
            {
                stop_time = (new TimeSpan(DateTime.Now.Ticks)).TotalMilliseconds;
                double totalMill = stop_time - start_time_res2res;

                //for the next iteration
                start_time_res2res = stop_time;


                try
                {
                    nstr = totalMill.ToString();
                    txtConsole.Invoke(new Action(() => textBox_Res2Res.Text = nstr));
                }
                catch (System.InvalidOperationException)
                {
                    //Debug.WriteLine("-->ERROR ERROR ERROR");
                    MessageBoxUpdated("Timer_Res2Res Error!");
                }

            }

        }





        private void MessageBoxUpdated(string msg)
        {
            string s_msg, s_msg2;

            TimeSpan t = DateTime.UtcNow - new DateTime(1970, 1, 1);
            s_msg2 = ((int)t.TotalSeconds).ToString();
            s_msg = Environment.NewLine + s_msg2 + @" - " + msg_line_count.ToString() + @" " + msg ;
            textBox_Message.Invoke(new Action(() => textBox_Message.AppendText(s_msg)));
            msg_line_count += 1;
        }




        void log_mqtt_data(string fname, string data)
        {
            /* append other info to data if needed */
            var MyLog = new LoggerClass(fname, data);
        }


        void client_MqttMsgPublished(object sender, MqttMsgPublishedEventArgs e)
        {
            MessageBoxUpdated("MessageId = " + e.MessageId + " Published = " + e.IsPublished);
            Timer_Pub2Res(false);
        }



        void client_MqttMsgPublishReceived(object sender, MqttMsgPublishEventArgs e)
        {
            string payload = null;
            string log_file_name;

            Timer_Pub2Res(true);
            Timer_Res2Res(true);


            bool targer_topic_for_me = false;

            /* default log file */
            log_file_name = "MQTT_Unclassified_Message.log";

            for (int i = 0; i <= targets_num ; i++)
            {
                targer_topic_for_me = e.Topic.Contains(targets_names[i]);

                if (targer_topic_for_me == true)
                {
                    log_file_name = targets_names[i].ToString() + ".log";
                    break;
                }
            }

                      

            //if ((e.Topic == VAL_TOPIC_CONNECTED) || (e.Topic == VAL_TOPIC_STATUS))
            //{
            //    payload = Encoding.UTF8.GetString(e.Message);
            //    txtConsole.Invoke(new Action(() => txtConsole.AppendText(payload + Environment.NewLine)));
            //}
            CBORObject cbor_rx;

            try
            {
                cbor_rx = CBORObject.DecodeFromBytes(e.Message);

                if ((targer_topic_for_me == true))
                {
                    //payload = Encoding.UTF8.GetString(e.Message);
                    //txtConsole.Invoke(new Action(() => txtConsole.AppendText(payload + Environment.NewLine)));

                    if (checkBox_Split_Resp.Checked == true)
                    {
                        string data = cbor_rx.ToString();
                        string[] words = data.Split(',');
                        foreach (string word in words)
                        {
                            txtConsole.Invoke(new Action(() => txtConsole.AppendText(word + Environment.NewLine)));
                        }
                    }
                    else
                    {
                        txtConsole.Invoke(new Action(() => txtConsole.AppendText(cbor_rx.ToString() + Environment.NewLine)));
                    }

                }
                else
                {
                    //default on # box
                    //payload = Encoding.UTF8.GetString(e.Message);                                           
                    //txtConsole.Invoke(new Action(() => textBox_Resp_Hash.AppendText(payload + Environment.NewLine)));
                    txtConsole.Invoke(new Action(() => textBox_Resp_Hash.AppendText(cbor_rx.ToString())));
                }

                /* log the data */
                log_mqtt_data(log_file_name, cbor_rx.ToString());

            }
            catch {
                txtConsole.Invoke(new Action(() => txtConsole.AppendText("CBOR Decoding Error ! DUMP START:" + Environment.NewLine)));

                string log_dump;

                log_dump = "CBOR Decoding Error ! DUMP START:" + Environment.NewLine;

                foreach (var item in e.Message)
                {                   
                    txtConsole.Invoke(new Action(() => txtConsole.AppendText(item.ToString("X") + Environment.NewLine)));

                    log_dump = log_dump + item.ToString("X") + Environment.NewLine;
                }

                txtConsole.Invoke(new Action(() => txtConsole.AppendText("CBOR Decoding Error ! DUMP END " + Environment.NewLine)));

                log_dump = log_dump + "CBOR Decoding Error ! DUMP END " + Environment.NewLine;

                log_mqtt_data(log_file_name, log_dump);

            }

            //BILATO receive the CBOR payload
            MessageBoxUpdated(" e.Topic = " + e.Topic);

        }




        // call back for connection SSL
        bool RemoteCertificateValidationCallback(object sender, X509Certificate certificate, X509Chain chain, SslPolicyErrors sslPolicyErrors)
        {
            // logic for validation here
            return true;
        }


        //*    json     *//

        public class Account
        {
            public string Email { get; set; }
            public bool Active { get; set; }
            public DateTime CreatedDate { get; set; }
            public IList<string> Roles { get; set; }
        }


        private void PublishTestFile(string testfilepath)
        {
            string textFile = null;
            byte[] fileBytes;

            if (File.Exists(testfilepath))
            {
                 // Read entire text file content in one string  
                 //textFile = File.ReadAllText(testfilepath);                
                 fileBytes = File.ReadAllBytes(testfilepath);
            }
            else
            {
                textBoxPublish.Text = "File not found " + testfilepath;
                return;
            }

            try
            {
                ushort msgId = client.Publish(val_req_post, fileBytes, MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);

                textBoxPublish.Text = textFile;
            }
            catch (System.NullReferenceException)
            {
                //Debug.WriteLine("Not Initialized");
                MessageBoxUpdated("MQTT not initialized!");
            }



        }

        void test_json()
        {

            Account account = new Account
            {
                Email = "carel@example.com",
                Active = true,
                CreatedDate = new DateTime(2019, 1, 20, 0, 0, 0, DateTimeKind.Utc),
                Roles = new List<string>
                {
                "User",
                "Admin"
                }
            };

            Myjson = JsonConvert.SerializeObject(account, Formatting.Indented);

            //Debug.WriteLine(Myjson);

            MessageBoxUpdated(Myjson);
        }

        private void ButtonChangeCredential_Click(object sender, EventArgs e)
        {
            string textFilePath = @".\cbor_cloud\REQ_CHANGE_CREDENTIALS.cbor";    
            
            PublishTestFile(textFilePath);
            Timer_Pub2Res(false);
        }

        private void ButtonReadValue_Click(object sender, EventArgs e)
        {
            string textFilePath = @".\cbor_cloud\REQ_READ_VALUES.cbor";
            PublishTestFile(textFilePath);
        }

        private void ButtonClear_Click(object sender, EventArgs e)
        {
            textBoxPublish.Text = "";
        }

        private void ButtonScanLine_Click(object sender, EventArgs e)
        {
            string textFilePath = @".\cbor_cloud\REQ_SCAN_DEVICES.cbor";
            PublishTestFile(textFilePath);
        }

        private void ButtonSet_Lines_Config_Click(object sender, EventArgs e)
        {
            string textFilePath = @".\cbor_cloud\REQ_SET_LINES_CONFIG.cbor";
            PublishTestFile(textFilePath);
        }

        private void buttonSet_Devs_Config_Click(object sender, EventArgs e)
        {
            string textFilePath = @".\cbor_cloud\REQ_DOWNLOAD_DEVS_CONFIG.cbor";
            PublishTestFile(textFilePath);
        }

        private void ButtonUpdate_ca_cerficates_Click(object sender, EventArgs e)
        {
            string textFilePath = @".\cbor_cloud\REQ_UPDATE_CA.cbor";
            PublishTestFile(textFilePath);
        }

        private void ButtonUpdate_dev_firmware_Click(object sender, EventArgs e)
        {
            string textFilePath = @".\cbor_cloud\REQ_UPDATE_DEV_FIRMWARE.cbor";
            PublishTestFile(textFilePath);
        }

        private void ButtonUpdate_gw_firmware_Click(object sender, EventArgs e)
        {
            string textFilePath = @".\cbor_cloud\test-update_gw_firmware-req.json";
            PublishTestFile(textFilePath);
        }







        private void ButtonConnect_Click(object sender, EventArgs e)
        {

            if (MQTT_Connect == true)
            {
                client.Disconnect();
                MQTT_Connect = false;
                buttonConnect.Text = "Connect";
                buttonConnect.BackColor = Color.Green;
                textBox_SubTopic.Enabled = true;
                textBox_Target.Enabled = true;
                return;
            }


            VAL_RESP_carel      = textBox_Target.Text + @"/" +  textBox_SubTopic.Text;
            VAL_TOPIC_CONNECTED = textBox_Target.Text + @"/connected";
            VAL_TOPIC_STATUS    = textBox_Target.Text + @"/status";


            val_req_post = textBox_Target.Text + VAL_REQ_carel;

            textBox_SubTopic.Enabled = false;
            textBox_Target.Enabled = false;
                                          


            // create client instance 
            MessageBoxUpdated("--> create client instance ");
            
            //client = new MqttClient("io.adafruit.com");        //(IPAddress.Parse("MQTT_BROKER_ADDRESS"));

            // import certificate   (step 1)
            string path = Directory.GetCurrentDirectory() + "\\cert\\ca.crt";
            X509Certificate2 caCert = new X509Certificate2(path);

            this.server_url = textBoxMQTT_Server_URL.Text;
            
            this.server_port = Int32.Parse(textBoxMQTT_Server_Port.Text);

            // new for Carel broker  (step 2) 
            server_url = textBoxMQTT_Server_URL.Text;

            if (checkBox_TLS.Checked == true)
            {
                client = new MqttClient(server_url, server_port, true, caCert, caCert, MqttSslProtocols.TLSv1_2, RemoteCertificateValidationCallback);
            }
            else
            {
                client = new MqttClient(server_url);

            }

            // register to message received 
            client.MqttMsgPublishReceived += client_MqttMsgPublishReceived;
            client.MqttMsgPublished += client_MqttMsgPublished;

            string clientId = Guid.NewGuid().ToString();

            //client.Connect(clientId);                                                              // for local Mosquitto
            //client.Connect(clientId, "alessandro_bilato", "51ed38a4a4d14de09f021ee0de2db993");     // for Iot Adafruit    
            client.Connect(clientId, "admin", "5Qz*(3_>K&vU!PS^");

            if (client.IsConnected)
            {
                MessageBoxUpdated("--> client CONNECT ");
            }
            else
            {
                MessageBoxUpdated("--> client NOT CONNECT ");
            }

            // subscribe to the topic "/home/temperature" with QoS 2 
            MessageBoxUpdated("--> subscribe to the topic " + VAL_RESP_carel);

            //client.Subscribe(new string[] { SUB_TEMP }, new byte[] { MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE });
            //client.Subscribe(new string[] { SUB_RELE1 }, new byte[] { MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE });

            //client.Subscribe(new string[] { SUB_TEMP_carel }, new byte[] { MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE });
            //client.Subscribe(new string[] { PUB_VAL_carel }, new byte[] { MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE });


            //TODO BILATO aggiungere qui tutti i device da monitorare 
            for (int i = 1; i <= targets_num; i++)
            {             
                client.Subscribe(new string[] { (targets_names[i-1] + VAL_SUB_TARGET_POSTFIX) }, new byte[] { MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE });
                MessageBoxUpdated("subscribed -> " + (targets_names[i-1] + VAL_SUB_TARGET_POSTFIX));
            }

            MQTT_Connect = true;
            buttonConnect.Text = "Disconnect";
            buttonConnect.BackColor = Color.Red; 
        }

        private void TextBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void ButtonClearResponse_Click(object sender, EventArgs e)
        {
            txtConsole.Text = "";
        }

        private void Label4_Click(object sender, EventArgs e)
        {

        }

        private void TextBox1_TextChanged_1(object sender, EventArgs e)
        {

        }






    


        private void Button_MB_Read_HR_Click(object sender, EventArgs e)
        {
            int value;

            var cbor = CBORObject.NewMap();

            cbor.Add(@"ver", CBOR_PAYLOAD_VER);
            cbor.Add(@"rto", textBox_Target.Text + @"\hr_r_val");
            value = 6;
            cbor.Add(@"cmd", value);
            cbor.Add(@"ali", textBox_Alias.Text);

            value = 0;
            if (Int32.TryParse(textBox_MB_HR_R_Func.Text, out value))
            {
                cbor.Add(@"fun", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > fun ");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Addr.Text, out value))
            {
                cbor.Add(@"adr", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > adr ");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Dim.Text, out value))
            {
                cbor.Add(@"dim", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > dim ");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Pos.Text, out value))
            {
                cbor.Add(@"pos", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > pos ");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Len.Text, out value))
            {
                cbor.Add(@"len", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > len ");
            }
            
            cbor.Add(@"a", textBox_A.Text);
            cbor.Add(@"b", textBox_B.Text);


            value = 0;

            if (Int32.TryParse(textBox_Flags.Text, out value))
            {
                cbor.Add("flg", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > flg ");
            }


            // The following converts the map to canonical CBOR
            byte[] cbor_bytes = cbor.EncodeToBytes(CBOREncodeOptions.DefaultCtap2Canonical);
                              
            try
            {
                ushort msgId = client.Publish(val_req_post, cbor_bytes, MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
                //textBoxPublish.Text = textFile;
            }
            catch (System.NullReferenceException)
            {
                MessageBoxUpdated("MQTT not initialized !");
                
            }
        }

        private void Button_MB_Write_HR_Click(object sender, EventArgs e)
        {
            int value;
            var cbor = CBORObject.NewMap();

            cbor.Add(@"ver", CBOR_PAYLOAD_VER);
            cbor.Add(@"rto", textBox_Target.Text + @"\hr_w_val");
            value = 7;
            cbor.Add(@"cmd", value);
            cbor.Add(@"ali", textBox_Alias.Text);
                       
            value = 0;
            if (Int32.TryParse(textBox_MB_HR_W_Func.Text, out value))
            {
                cbor.Add(@"fun", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > fun ");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Addr.Text, out value))
            {
                cbor.Add(@"adr", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > adr ");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Dim.Text, out value))
            {
                cbor.Add(@"dim", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > dim ");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Pos.Text, out value))
            {
                cbor.Add(@"pos", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > pos ");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Len.Text, out value))
            {
                cbor.Add(@"len", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > len ");
            }

            cbor.Add(@"a", textBox_A.Text);
            cbor.Add(@"b", textBox_B.Text);

            value = 0;
            if (Int32.TryParse(textBox_Flags.Text, out value))
            {
                cbor.Add("flg", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > flg ");
            }

            cbor.Add(@"val", textBox_HR_Val.Text);
                       
            // The following converts the map to canonical CBOR
            byte[] cbor_bytes = cbor.EncodeToBytes(CBOREncodeOptions.DefaultCtap2Canonical);

            try
            {
                ushort msgId = client.Publish(val_req_post, cbor_bytes, MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);                
            }
            catch (System.NullReferenceException)
            {
                MessageBoxUpdated("MQTT Not Initialized");
            }

        }

        private void Button_MB_Read_COIL_Click(object sender, EventArgs e)
        {
            int value;
            var cbor = CBORObject.NewMap();

            cbor.Add(@"ver", CBOR_PAYLOAD_VER);
            cbor.Add(@"rto", textBox_Target.Text + @"\coil_r_val");
            value = 7;
            cbor.Add(@"cmd", value);
            cbor.Add(@"ali", textBox_Alias_Coil.Text);

            value = 0;
            if (Int32.TryParse(textBox_MB_COIL_R_Func.Text, out value))
            {
                cbor.Add(@"fun", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > fun ");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Addr_Coil.Text, out value))
            {
                cbor.Add(@"adr", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > adr ");
            }

            value = 0;
            if (Int32.TryParse(@"1", out value))
            {
                cbor.Add(@"dim", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > dim ");
            }

            value = 0;
            if (Int32.TryParse(@"1", out value))
            {
                cbor.Add(@"pos", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > pos ");
            }

            value = 0;
            if (Int32.TryParse(@"1", out value))
            {
                cbor.Add(@"len", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > len ");
            }

            cbor.Add(@"a", @"1.0");
            cbor.Add(@"b", @"1.0");

            value = 0;
            if (Int32.TryParse(@"0", out value))
            {
                cbor.Add("flg", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > flg ");
            }

            // The following converts the map to canonical CBOR
            byte[] cbor_bytes = cbor.EncodeToBytes(CBOREncodeOptions.DefaultCtap2Canonical);

            try
            {
                ushort msgId = client.Publish(val_req_post, cbor_bytes, MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
             
            }
            catch (System.NullReferenceException)
            {
                MessageBoxUpdated("MQTT Not Initialized");
            }
        }

        private void Button_MB_Write_COIL_Click(object sender, EventArgs e)
        {
            int value;
            var cbor = CBORObject.NewMap();

            cbor.Add(@"ver", CBOR_PAYLOAD_VER);
            cbor.Add(@"rto", textBox_Target.Text + @"\coil_w_val");
            value = 7;
            cbor.Add(@"cmd", value);
            cbor.Add(@"ali", textBox_Alias_Coil.Text);

            value = 0;
            if (Int32.TryParse(textBox_MB_COIL_W_Func.Text, out value))
            {
                cbor.Add(@"fun", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > fun ");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Addr_Coil.Text, out value))
            {
                cbor.Add(@"adr", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > adr ");
            }

            value = 0;
            if (Int32.TryParse(@"1", out value))
            {
                cbor.Add(@"dim", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > dim ");
            }

            value = 0;
            if (Int32.TryParse(@"1", out value))
            {
                cbor.Add(@"pos", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > pos ");
            }

            value = 0;
            if (Int32.TryParse(@"1", out value))
            {
                cbor.Add(@"len", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > len ");
            }

            cbor.Add(@"a", @"1.0");
            cbor.Add(@"b", @"1.0");

            value = 0;
            if (Int32.TryParse(@"0", out value))
            {
                cbor.Add("flg", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > flg ");
            }

            cbor.Add(@"val", textBox_MB_COIL_Val.Text);

            // The following converts the map to canonical CBOR
            byte[] cbor_bytes = cbor.EncodeToBytes(CBOREncodeOptions.DefaultCtap2Canonical);

            try
            {
                ushort msgId = client.Publish(val_req_post, cbor_bytes, MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);

            }
            catch (System.NullReferenceException)
            {
                MessageBoxUpdated("MQTT Not Initialized");
            }

        }

        private void Button_MB_Read_DI_Click(object sender, EventArgs e)
        {
            int value;
            var cbor = CBORObject.NewMap();

            cbor.Add(@"ver", CBOR_PAYLOAD_VER);
            cbor.Add(@"rto", textBox_Target.Text + @"\di_r_val");
            value = 7;
            cbor.Add(@"cmd", value);
            cbor.Add(@"ali", textBox_Alias_DI.Text);

            value = 0;
            if (Int32.TryParse(textBox_MB_DI_R_Func.Text, out value))
            {
                cbor.Add(@"fun", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > fun ");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Addr_DI.Text, out value))
            {
                cbor.Add(@"adr", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > adr ");
            }

            value = 0;
            if (Int32.TryParse(@"1", out value))
            {
                cbor.Add(@"dim", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > dim ");
            }

            value = 0;
            if (Int32.TryParse(@"1", out value))
            {
                cbor.Add(@"pos", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > pos ");
            }

            value = 0;
            if (Int32.TryParse(@"1", out value))
            {
                cbor.Add(@"len", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > len ");
            }

            cbor.Add(@"a", @"1.0");
            cbor.Add(@"b", @"1.0");

            value = 0;
            if (Int32.TryParse(@"0", out value))
            {
                cbor.Add("flg", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > flg ");
            }

            // The following converts the map to canonical CBOR
            byte[] cbor_bytes = cbor.EncodeToBytes(CBOREncodeOptions.DefaultCtap2Canonical);

            try
            {
                ushort msgId = client.Publish(val_req_post, cbor_bytes, MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);

            }
            catch (System.NullReferenceException)
            {
                MessageBoxUpdated("MQTT Not Initialized");
            }
        }

        private void Button_MB_Read_IR_Click(object sender, EventArgs e)
        {
            int value;
            var cbor = CBORObject.NewMap();

            cbor.Add(@"ver", CBOR_PAYLOAD_VER);
            cbor.Add(@"rto", textBox_Target.Text + @"\ir_r_val");
            value = 6;
            cbor.Add(@"cmd", value);
            cbor.Add(@"ali", textBox_Alias_IR.Text);

            value = 0;
            if (Int32.TryParse(textBox_MB_IR_R_Func.Text, out value))
            {
                cbor.Add(@"fun", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > fun ");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Addr_IR.Text, out value))
            {
                cbor.Add(@"adr", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > adr ");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Dim_IR.Text, out value))
            {
                cbor.Add(@"dim", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > dim ");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Pos_IR.Text, out value))
            {
                cbor.Add(@"pos", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > pos ");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Len_IR.Text, out value))
            {
                cbor.Add(@"len", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > len ");
            }

            cbor.Add(@"a", textBox_A_IR.Text);
            cbor.Add(@"b", textBox_B_IR.Text);


            value = 0;

            if (Int32.TryParse(textBox_Flags_IR.Text, out value))
            {
                cbor.Add("flg", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > flg ");
            }


            // The following converts the map to canonical CBOR
            byte[] cbor_bytes = cbor.EncodeToBytes(CBOREncodeOptions.DefaultCtap2Canonical);

            try
            {
                ushort msgId = client.Publish(val_req_post, cbor_bytes, MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
                //textBoxPublish.Text = textFile;
            }
            catch (System.NullReferenceException)
            {
                MessageBoxUpdated("MQTT not initialized !");

            }
        }

        private void Button_JSON_Validate_Resp_Click(object sender, EventArgs e)
        {
            MessageBoxInfo("use CTRL-V to paste the JSON", "Info");
            System.Windows.Forms.Clipboard.SetText(txtConsole.Text);
            System.Diagnostics.Process.Start(JSON_VALIDATOR);
            //SendKeys.Send("^V");
        }

        private void Button_JSON_Validate_Pub_Click(object sender, EventArgs e)
        {
            MessageBoxInfo("use CTRL-V to paste the CBOR", "Info");
            if (textBoxPublish.Text != "")
            {
              System.Windows.Forms.Clipboard.SetText(textBoxPublish.Text);
            }
            System.Diagnostics.Process.Start(JSON_VALIDATOR);
            //SendKeys.Send("^V");
        }

        private void Button_JSON_Validate_Offline_Click(object sender, EventArgs e)
        {

            string jsonString = txtConsole.Text;
            try
            {
                //var tmpObj = JsonValue.Parse(jsonString);
            }
            catch (FormatException fex)
            {
                //Invalid json format
                MessageBoxUpdated("Res MQTT NOT valid ! " + fex.ToString());
            }
            catch (Exception ex) //some other exception
            {
                
                MessageBoxUpdated("Res MQTT is valid !");
            }


            
        }

        private void Button_res2res_reset_Click(object sender, EventArgs e)
        {
            start_stop_res2res_flag = false;
            string nstr = @"-----";
            txtConsole.Invoke(new Action(() => textBox_Res2Res.Text = nstr));
        }

        private void Button_test_set_gw_config_req_Click(object sender, EventArgs e)
        {
            //string textFilePath = @"test-set_gw_config-req.json";
            string textFilePath = @".\cbor_cloud\REQ_SET_GW_CONFIG.cbor";
            PublishTestFile(textFilePath);
        }

        private void Label50_Click(object sender, EventArgs e)
        {

        }

        private void Button_start_engine_Click(object sender, EventArgs e)
        {
            string textFilePath = @".\cbor_cloud\REQ_START_ENGINE.cbor";
            PublishTestFile(textFilePath);
        }

        private void Button_stop_engine_Click(object sender, EventArgs e)
        {            
            string textFilePath = @".\cbor_cloud\REQ_STOP_ENGINE.cbor";
            PublishTestFile(textFilePath);

        }

        private void Label51_Click(object sender, EventArgs e)
        {

        }

        private void TextBoxMQTT_Server_Port_TextChanged(object sender, EventArgs e)
        {

        }

        private void CheckBox_TLS_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox_TLS.Checked == true)
            {
                textBoxMQTT_Server_Port.Text = "8883";
            }
            else
            {
                textBoxMQTT_Server_Port.Text = "1883";
            }          
        }

        private void CheckBox_ShowMsg_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void Button_Save_settings_Click(object sender, EventArgs e)
        {
            string cfg_file;

            cfg_file = @"MqttClientBinaryMonitor_CFG.ini";
            
            var MyIni = new IniFile(cfg_file);

            MyIni.Write("Server", textBoxMQTT_Server_URL.Text);
            MyIni.Write("Port", textBoxMQTT_Server_Port.Text);

            if (checkBox_TLS.Checked == true)
            {
                MyIni.Write("TLS", "1");
            }
            else
            {
                MyIni.Write("TLS", "0");
            }


            MyIni.Write("Target", textBox_Target.Text);

        }

        private void TextBoxMQTT_Server_URL_TextChanged(object sender, EventArgs e)
        {

        }

        private void CheckBox_Cfg_Dbg_Rel_CheckedChanged(object sender, EventArgs e)
        {

        }



        private void Save_Last_Used_Values()
        {
            string cfg_file;
            string val;

            cfg_file = @"MqttClientBinaryMonitor_Last_Set.ini";
            
            var MyIni = new IniFile(cfg_file);

            MyIni.Write("Server", textBoxMQTT_Server_URL.Text);
            MyIni.Write("Port", textBoxMQTT_Server_Port.Text);

            if (checkBox_TLS.Checked == true)
            {
                MyIni.Write("TLS", "1");
            }
            else
            {
                MyIni.Write("TLS", "0");
            }


            MyIni.Write("Target", textBox_Target.Text);
            MyIni.Write("Topic", textBox_SubTopic.Text);
                                             

        }


        private void Load_Last_Used_Values()
        {
            String cfg_file;
            String val;
            string par_val;

            cfg_file = @"MqttClientBinaryMonitor_CFG.ini";

            var MyIni = new IniFile(cfg_file);

            par_val = MyIni.Read("Server");
            textBoxMQTT_Server_URL.Text = par_val;

            par_val = MyIni.Read("Port");
            textBoxMQTT_Server_Port.Text = par_val;

            par_val = MyIni.Read("TLS");

            if (par_val.Equals("1"))
            {
                checkBox_TLS.Checked = true;
            }
            else
            {
                checkBox_TLS.Checked = false;
            }


            
            par_val = MyIni.Read("Targets");

            targets_num = Int16.Parse(par_val);



            for (int i = 1; i <= targets_num; i++)
            {
                targets_names[i-1] = MyIni.Read("Target"+i.ToString());
            }


            //par_val = MyIni.Read("Target");
            //textBox_Target.Text = par_val;

            /* 

            par_val = MyIni.Read("Topic");

            if (par_val == "")
              textBox_SubTopic.Text = "+";
            else
              textBox_SubTopic.Text = par_val;


            val = MyIni.Read("DBGREL");


            if (val == @"1")
            {
                checkBox_Cfg_Dbg_Rel.Checked = true;
            }
            else
            {
                checkBox_Cfg_Dbg_Rel.Checked = false;
            }

            */
        }


        private void Button_Load_settings_Click(object sender, EventArgs e)
        {
            string cfg_file;
            string par_val;

            cfg_file = @"MqttClientBinaryMonitor_CFG.ini"; 

            var MyIni = new IniFile(cfg_file);

            par_val = MyIni.Read("Server");
            textBoxMQTT_Server_URL.Text = par_val;

            par_val = MyIni.Read("Port");
            textBoxMQTT_Server_Port.Text = par_val;

            par_val = MyIni.Read("TLS");

            if (par_val.Equals("1"))
            {
                checkBox_TLS.Checked = true;
            }
            else
            {
                checkBox_TLS.Checked = false;
            }
                                  

            par_val = MyIni.Read("Target");
            textBox_Target.Text = par_val;
        }

        private void TextBox_MB_COIL_R_Func_TextChanged(object sender, EventArgs e)
        {

        }

        private void Button_CLS_Infos_Click(object sender, EventArgs e)
        {
            textBox_Message.Text = "";
        }

        private void Button7_Click(object sender, EventArgs e)
        {
            string textFilePath = @".\cbor_cloud\REQ_REBOOT.cbor";
            PublishTestFile(textFilePath);
        }

        private void Button_send_mb_adu_Click_1(object sender, EventArgs e)
        {
            const string template_name = @"test-send_mb_adu-req.json";
            string textFile = null;

            if (File.Exists(template_name))
            {
                // Read entire text file content in one string  
                //textFile = File.ReadAllText(testfilepath);
                string[] lines = File.ReadAllLines(template_name);
                textFile = String.Join(Environment.NewLine, lines);
            }
            else
            {
                textBoxPublish.Text = "ERROR " + template_name;
                return;
            }

            textBoxPublish.Text = textFile;

            try
            {
                ushort msgId = client.Publish(val_req_post, Encoding.UTF8.GetBytes(textFile), MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE, false);
                textBoxPublish.Text = textFile;
            }
            catch (System.NullReferenceException)
            {
                //Debug.WriteLine("Not Initialized");
                MessageBoxUpdated("MQTT Not Initialized");
            }
        }

    }
    }







//string textFilePath = @"C:\\hwfwdept_proj\\c780_gme_library\\Test\\Test_cases\\JSON_test_cases\\test-write_values_coil_15.json";

//string textFilePath = @"C:\\hwfwdept_proj\\c780_gme_library\\Test\\Test_cases\\JSON_test_cases\\test-write_values_hr_16-req.json";
