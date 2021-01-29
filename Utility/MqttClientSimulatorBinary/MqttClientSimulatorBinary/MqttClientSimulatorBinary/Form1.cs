using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Timers;

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
using Newtonsoft.Json.Linq;

using System.IO;
//using System.Json;
using PeterO;
using PeterO.Cbor;
using PeterO.Numbers;




namespace MqttClientSimulatorBinary
{

    public partial class Form1 : Form
    {

        const int CBOR_CMD_SCAN_LINE = 3;
        const int CBOR_CMD_FULL_FILE_UPLOAD = 19;
        const int CBOR_CMD_RANGE_FILE_UPLOAD = 20;

        public struct read_file_struct
        {
            public Int16 version;
            public String rto;
            public Int16 cmd;
            public Int16 res;
            public Int32 fsz;
            public Int32 fst;
            public Int32 fle;
            public string ans_arr;
        };

        const string DECODE_FILE_PRG = "LogtransferAnalyzer.exe";

        const string JSON_VALIDATOR = "http://cbor.me/";
        // "https://jsonformatter.org/";    
        // "https://jsonformatter.curiousconcept.com/"

        const string VAL_REQ_carel  = @"/req";
        public string val_req_post;

        //const string VAL_RESP_carel = @"/res/53BCE4F1DFA0FE8E7CA126F91B35D3A6";

        public string VAL_RESP_carel = @"/res/53BCE4F1DFA0FE8E7CA126F91B35D3A6";
        public string VAL_RESP_carel_ALL = @"#";

        public string VAL_TOPIC_CONNECTED = @"";
        public string VAL_TOPIC_STATUS = @"";
        public string VAL_TOPIC_UPLOAD = @"";

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

        public int flags_of;

        public string latest_upload_file = @"";

        LogFile SessionLog;

        string post_processing = @"";
        string last_cmd17_payload = @"";

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

        public Form1()
        {
            InitializeComponent();

            //Load settings

            //FormClosed += Form1_Closed;

            //System.Net.IPAddress IPAddress = System.Net.IPAddress.Parse(MQTT_BROKER_ADDRESS);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Debug.WriteLine("MessageId 1");
            Load_Last_Used_Values();
            SessionLog = new LogFile();
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
            s_msg = Environment.NewLine + s_msg2 + @" - " + msg_line_count.ToString() + @" " + msg;
            textBox_Message.Invoke(new Action(() => textBox_Message.AppendText(s_msg)));
            msg_line_count += 1;
        }

        public static int GetHexVal(char hex)
        {
            int val = (int)hex;
            //For uppercase A-F letters:
            //return val - (val < 58 ? 48 : 55);
            //For lowercase a-f letters:
            //return val - (val < 58 ? 48 : 87);
            //Or the two combined, but a bit slower:
            return val - (val < 58 ? 48 : (val < 97 ? 55 : 87));
        }


        /*
         * ans_filter
         * this routine remove the h character that the conversion cbor > text > json put
         * to indicate the binary content but generate an error on deserialization
         */
        string ans_filter(string data)
        {
            string ret = "";

            ret = data;

            if (ret.Contains("ans"))
            {
                ret = ret.Replace(": h", ": ");
            }

            if (ret.Contains("mqv"))
            {
                ret = ret.Replace(": h", ": ");
            }

            if (ret.Contains("gid"))
            {
                ret = ret.Replace(": h", ": ");
            }


            return ret;
        }

        void Decode_Upload_Msg(string data_in)
        {
            read_file_struct my_J_data;

            //use this sample to debug the below routine 
            // the extracted data more or less are similar to this example
            //string data1 = @"{
            //'ver': 257,                           								
            //'rto': 'A126F91B35D3A6',    
            //'cmd': 19,
            //'res': 0,                   
            //'fsz': 1700000,  
            //'fst':  4,       
            //'fle':  4,     
            //'ans': 'FF102030' 
            // }";

            string data = ans_filter(data_in);

            //to test simply change data > data1 in the row below
            JObject o = JObject.Parse(data);

            string o_ver = (string)o["ver"];
            string o_rto = (string)o["rto"];
            string o_cmd = (string)o["cmd"];

            //not a cmd
            if (o_cmd == null) return;

            int cmd_value = 0;
            if (!(int.TryParse(o_cmd, out cmd_value)))
            {
                DialogResult result1 = MessageBox.Show("Error in the o_cmd buffer abort!",
                                                       "Important Question",
                                                       MessageBoxButtons.OK);
                return;
            }

            //check if is a file upload cmd full or range
            if ((cmd_value != CBOR_CMD_FULL_FILE_UPLOAD) || (cmd_value != CBOR_CMD_RANGE_FILE_UPLOAD)) return;

            string o_res = (string)o["res"];
            string o_fsz = (string)o["fsz"];
            string o_fst = (string)o["fst"];
            string o_fle = (string)o["fle"];
            string o_ans = (string)o["ans"];

            string curFile = o_rto + @".bin";

            //if empty means the echo of the command sent
            if (o_ans == null) return;

            int fsz_value = 0;
            if (!(int.TryParse(o_fsz, out fsz_value)))
            {                
                DialogResult result1 = MessageBox.Show("Error in the o_fsz buffer abort!",
                                                       "Important Question",
                                                       MessageBoxButtons.OK);
                return;                                    
            }

            int fst_value = 0;
            if (!(int.TryParse(o_fst, out fst_value)))
            {
                DialogResult result1 = MessageBox.Show("Error in the o_fst buffer abort!",
                                                       "Important Question",
                                                       MessageBoxButtons.OK);
                return;
            }

            int fle_value = 0;
            if (!(int.TryParse(o_fle, out fle_value)))
            {
                DialogResult result1 = MessageBox.Show("Error in the o_fst buffer abort!",
                                                       "Important Question",
                                                       MessageBoxButtons.OK);
                return;
            }

            

            //create an empty but filled with zero file if not exist
            if (!(File.Exists(curFile)))
            {                
                    byte value_to_write = 0;
                    //we create a new file filled with zero 
                    using (BinaryWriter writer = new BinaryWriter(File.Open(curFile, FileMode.Create)))
                    {
                        for (int i = 0; i < fsz_value; i++) writer.Write(value_to_write);
                    }

                textBox_upload_file_info.AppendText("Created " + curFile + " size " + fsz_value.ToString());
                latest_upload_file = curFile;
            }

            //insert the given file chunk in the right position
            if ((fst_value + fle_value) >= fsz_value) 
            {
                string sdata;

                sdata = String.Format("{0} {1} of {2}", "File transfer reached the size", (fst_value + fle_value), fsz_value);

                //take in mind that > (major) is not possible theoretichally anyway not to bad to have a feedback
                DialogResult result1 = MessageBox.Show(sdata,
                                                       "Try to decode!",
                                                        MessageBoxButtons.OK);
            }

            using (FileStream fileStream = new FileStream(curFile, FileMode.Open))
            {
               // Set the stream position to the beginning of the file.
               fileStream.Seek(fst_value, SeekOrigin.Begin);
               
               int s_len = o_ans.Length;
               int iSidx = 0;
               
               while (iSidx < s_len) 
               {
                   string subS = o_ans.Substring(iSidx, 2);
                   iSidx += 2;
               
                   byte val=0; 
               
                   for (int i = 0; i < subS.Length >> 1; ++i)
                   {
                       val = (byte)((GetHexVal(subS[i << 1]) << 4) + (GetHexVal(subS[(i << 1) + 1])));
                   }
               
                   fileStream.WriteByte(val);
               }
               
               textBox_upload_file_info.AppendText("Chunk " + fst_value.ToString() + ":" + fle_value.ToString());
            }
           
        }
        
        void Decode_Scan_Line_Msg(string data_in)
        {
           read_file_struct my_J_data;

           string data = ans_filter(data_in);
            
           JObject o = JObject.Parse(data);

           string o_ver = (string)o["ver"];
           string o_rto = (string)o["rto"];
           string o_cmd = (string)o["cmd"];

           if (o_cmd == null) return;

           int cmd_value = 0;
           if (!(int.TryParse(o_cmd, out cmd_value)))
           {
               DialogResult result1 = MessageBox.Show("Error in the o_cmd buffer abort!",
                                                      "Important Question",
                                                      MessageBoxButtons.OK);
               return;
           }

           //check if is a file upload cmd full or range
           if (cmd_value != CBOR_CMD_SCAN_LINE) return;

           string o_res = (string)o["res"];
           string o_dev = (string)o["dev"];
           string o_ans = (string)o["ans"];

           //this is cmd echo 
           if (o_ans == null) return;

            last_cmd17_payload = o_ans;

           //textBox_upload_file_info.AppendText(o_ans);                
        }

        void client_MqttMsgPublished(object sender, MqttMsgPublishedEventArgs e)
        {
            //Debug.WriteLine("MessageId = " + e.MessageId + " Published = " + e.IsPublished);

            MessageBoxUpdated("MessageId = " + e.MessageId + " Published = " + e.IsPublished);
            Timer_Pub2Res(false);
        }


        void client_MqttMsgPublishReceived(object sender, MqttMsgPublishEventArgs e)
        {
            string payload = null;

            Timer_Pub2Res(true);
            Timer_Res2Res(true);


            bool targer_topic_for_me = false;
            targer_topic_for_me = e.Topic.Contains(textBox_Target.Text);


            //if ((e.Topic == VAL_TOPIC_CONNECTED) || (e.Topic == VAL_TOPIC_STATUS))
            //{
            //    payload = Encoding.UTF8.GetString(e.Message);
            //    txtConsole.Invoke(new Action(() => txtConsole.AppendText(payload + Environment.NewLine)));
            //}
            CBORObject cbor_rx;

            try
            {
                string data=@"";

                cbor_rx = CBORObject.DecodeFromBytes(e.Message);

                if ((targer_topic_for_me == true))
                {
                    //payload = Encoding.UTF8.GetString(e.Message);
                    //txtConsole.Invoke(new Action(() => txtConsole.AppendText(payload + Environment.NewLine)));

                    if (checkBox_Split_Resp.Checked == true)
                    {
                        data = cbor_rx.ToString();
                        string[] words = data.Split(',');
                        foreach (string word in words)
                        {
                            string sdata = word + Environment.NewLine;
                            txtConsole.Invoke(new Action(() => txtConsole.AppendText(sdata)));
                            SessionLog.WriteLine(sdata);
                        }
                    }
                    else
                    {
                        data = cbor_rx.ToString() + Environment.NewLine;
                        txtConsole.Invoke(new Action(() => txtConsole.AppendText(data)));
                        SessionLog.WriteLine(data);
                    }


                    //wait last msg processed TODO BILATO da sistemare con una coda
                    while (post_processing.Length > 0) ;


                    post_processing = data;
                    //try to decode the file upload if a background file transfer is running
                    //Decode_Upload_Msg(cbor_rx_clone);

                    //try to decode scan line result
                    //Decode_Scan_Line_Msg(data);

                }
                else
                {
                    //default on # box
                    //payload = Encoding.UTF8.GetString(e.Message);                                           
                    //txtConsole.Invoke(new Action(() => textBox_Resp_Hash.AppendText(payload + Environment.NewLine)));
                    txtConsole.Invoke(new Action(() => textBox_Resp_Hash.AppendText(cbor_rx.ToString())));
                }

            }
            catch {

                if ((targer_topic_for_me == true))
                {
                    string sdata = "CBOR Decoding Error ! DUMP START:" + Environment.NewLine;

                    txtConsole.Invoke(new Action(() => txtConsole.AppendText(sdata)));
                    SessionLog.WriteLine(sdata);

                    foreach (var item in e.Message)
                    {
                        sdata = item.ToString("X") + Environment.NewLine;
                        txtConsole.Invoke(new Action(() => txtConsole.AppendText(sdata)));
                        SessionLog.WriteLine(sdata);
                    }

                    sdata = "CBOR Decoding Error ! DUMP END " + Environment.NewLine;
                    txtConsole.Invoke(new Action(() => txtConsole.AppendText(sdata)));
                    SessionLog.WriteLine(sdata);
                }

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


        byte qos_selected()
        {
            byte retval = MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE; ;
            if (radioButton_qos1.Checked == true) retval = MqttMsgBase.QOS_LEVEL_AT_LEAST_ONCE;
            
            return retval;
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

                if (MQTT_Connect == true) 
                {
                    ushort msgId = client.Publish(val_req_post, fileBytes, qos_selected(), false);
                    textBoxPublish.Text = textFile;
                }

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

            DialogResult result1 = MessageBox.Show("Are you really sure to update DEV config ?",
               "Important Question",
               MessageBoxButtons.YesNo);

            if (result1 == DialogResult.Yes)
            {
               PublishTestFile(textFilePath);
            }
        }

        private void ButtonUpdate_ca_cerficates_Click(object sender, EventArgs e)
        {
            DialogResult result1 = MessageBox.Show("Are you really sure to update CA certificate ?",
                           "Important Question",
                           MessageBoxButtons.YesNo);

            if (result1 == DialogResult.Yes)
            {
                string textFilePath = @".\cbor_cloud\REQ_UPDATE_CA.cbor";
                PublishTestFile(textFilePath);
            }
        }

        private void ButtonUpdate_dev_firmware_Click(object sender, EventArgs e)
        {
            DialogResult result1 = MessageBox.Show("Are you really sure to update Device Firmware ?",
                                       "Important Question",
                                       MessageBoxButtons.YesNo);

            if (result1 == DialogResult.Yes)
            {
                string textFilePath = @".\cbor_cloud\REQ_UPDATE_DEV_FIRMWARE.cbor";
                PublishTestFile(textFilePath);
            }
        }



        private void ButtonUpdate_gw_firmware_Click(object sender, EventArgs e)
        {

            DialogResult result1 = MessageBox.Show("Are you really sure to update GW Firmware ?",
                                                   "Important Question",
                                                   MessageBoxButtons.YesNo);

            if (result1 == DialogResult.Yes) 
            {
                string textFilePath = @".\cbor_cloud\REQ_GW_UPDATE_FW.cbor";
                PublishTestFile(textFilePath);
            }

        }


        private void ButtonConnect_Click(object sender, EventArgs e)
        {
            mqtt_connect(false);
        }




        public void mqtt_connect(bool force_reconn)
        {

            if ((MQTT_Connect == true) && (force_reconn == false))
            {
                //if not don't crash
                if (client.IsConnected)
                {
                    client.Disconnect();                    
                }

                SessionLog.CloseLogFile();

                MQTT_Connect = false;
                timer_check_is_alive.Enabled = false;
                timer_post_processing.Enabled = false;
                buttonConnect.Text = "Connect";
                buttonConnect.BackColor = Color.Green;
                textBox_SubTopic.Enabled = true;
                textBox_Target.Enabled = true;
                checkBox_mqtt_alive.Checked = false;
                return;
            }

            //already connected
            if (MQTT_Connect == false)
            {
                SessionLog.OpenLogFile();
            }

            VAL_RESP_carel      = textBox_Target.Text + @"/" +  textBox_SubTopic.Text;
            VAL_TOPIC_CONNECTED = textBox_Target.Text + @"/connected";
            VAL_TOPIC_STATUS    = textBox_Target.Text + @"/status";
            VAL_TOPIC_UPLOAD    = textBox_Target.Text + @"/upload";

            val_req_post = textBox_Target.Text + VAL_REQ_carel;

            textBox_SubTopic.Enabled = false;
            textBox_Target.Enabled = false;
            radioButton_qos1.Enabled = false;
            radioButton_qos0.Enabled = false;


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
                try
                {
                    client = new MqttClient(server_url, server_port, true, caCert, caCert, MqttSslProtocols.TLSv1_2, RemoteCertificateValidationCallback);
                }
                catch
                {
                    return;
                }

            }
            else
            {
                try
                {
                    client = new MqttClient(server_url);
                }
                catch
                {
                    return;
                }

            }

            // register to message received 
            client.MqttMsgPublishReceived += client_MqttMsgPublishReceived;
            client.MqttMsgPublished += client_MqttMsgPublished;

            //string clientId = Guid.NewGuid().ToString();

            //@"A0A1A2A3A4A5A6"; //fixed for the simulator
            string clientId = textBox_MQTT_ID.Text;
            string ClientUsr = textBox_MQTT_USR.Text;
            string clientPwd = textBox_MQTT_PWD.Text;

            //client.Connect(clientId);                                                              // for local Mosquitto
            //client.Connect(clientId, "alessandro_bilato", "51ed38a4a4d14de09f021ee0de2db993");     // for Iot Adafruit    
            //client.Connect(clientId, "admin", "5Qz*(3_>K&vU!PS^");                                 // mqtt-dev.tera.systems     

            //mqtts://test-mqtt.remotepro.io   user=from IoT      clientId=MAC

            client.Connect(clientId, ClientUsr, clientPwd);

            if (client.IsConnected)
            {
                MessageBoxUpdated("--> client CONNECT ");
            }
            else
            {
                MessageBoxUpdated("--> client NOT CONNECT ");
                client.Disconnect();
            }

            // subscribe to the topic "/home/temperature" with QoS 2 
            MessageBoxUpdated("--> subscribe to the topic " + VAL_RESP_carel);

            //client.Subscribe(new string[] { SUB_TEMP }, new byte[] { MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE });
            //client.Subscribe(new string[] { SUB_RELE1 }, new byte[] { MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE });

            //client.Subscribe(new string[] { SUB_TEMP_carel }, new byte[] { MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE });
            //client.Subscribe(new string[] { PUB_VAL_carel }, new byte[] { MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE });


            client.Subscribe(new string[] { VAL_RESP_carel_ALL }, new byte[] { qos_selected() });
                        

            MQTT_Connect = true;
            timer_check_is_alive.Enabled = true;
            timer_post_processing.Enabled = true;
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
            cbor.Add(@"rto", textBox_Target.Text + @"/hr_r_val");
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
                ushort msgId = client.Publish(val_req_post, cbor_bytes, qos_selected(), false);
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
            int dim_value;
            var cbor = CBORObject.NewMap();

            cbor.Add(@"ver", CBOR_PAYLOAD_VER);
            cbor.Add(@"rto", textBox_Target.Text + @"/hr_w_val");
            value = 7;
            cbor.Add(@"cmd", value);
            cbor.Add(@"ali", textBox_Alias.Text);

            value = 0;
            dim_value = 16;

            if (Int32.TryParse(textBox_MB_Dim.Text, out value))
            {
                cbor.Add(@"dim", value);
                dim_value = value;
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > dim ");
            }

            if (dim_value == 32)
            {
                textBox_MB_HR_W_Func.Text = "16";
            }
            else 
            {
                textBox_MB_HR_W_Func.Text = "6";
            }


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
                ushort msgId = client.Publish(val_req_post, cbor_bytes, qos_selected(), false);                
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
            cbor.Add(@"rto", textBox_Target.Text + @"/coil_r_val");
            value = 6;
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
                ushort msgId = client.Publish(val_req_post, cbor_bytes, qos_selected(), false);
             
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
            cbor.Add(@"rto", textBox_Target.Text + @"/coil_w_val");
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
                ushort msgId = client.Publish(val_req_post, cbor_bytes, qos_selected(), false);

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
            cbor.Add(@"rto", textBox_Target.Text + @"/di_r_val");
            value = 6;
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
                ushort msgId = client.Publish(val_req_post, cbor_bytes, qos_selected(), false);

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
            cbor.Add(@"rto", textBox_Target.Text + @"/ir_r_val");
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
                ushort msgId = client.Publish(val_req_post, cbor_bytes, qos_selected(), false);
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

            if (checkBox_Cfg_Dbg_Rel.Checked == true)
            {
                cfg_file = @"MqttClientSimulatorBinary_REL_Set.ini";
            }
            else
            {
                cfg_file = @"MqttClientSimulatorBinary_DBG_Set.ini";
            }

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
            
            cfg_file = @"MqttClientSimulatorBinary_Last_Set.ini";
            
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

            if (checkBox_Cfg_Dbg_Rel.Checked == true)
            {
                val = @"1";
            }
            else
            {
                val = @"0";
            }

            MyIni.Write("Topic", textBox_SubTopic.Text);
                                             
            if (checkBox_Cfg_Dbg_Rel.Checked == true)
                MyIni.Write("DBGREL", @"1");
            else
                MyIni.Write("DBGREL", @"0");

            MyIni.Write("MQTT_ID", textBox_MQTT_ID.Text);
            MyIni.Write("MQTT_PWD", textBox_MQTT_PWD.Text);
            MyIni.Write("MQTT_USR", textBox_MQTT_USR.Text);

        }


        private void Load_Last_Used_Values()
        {
            String cfg_file;
            String val;
            string par_val;

            cfg_file = @"MqttClientSimulatorBinary_Last_Set.ini";

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

            par_val = MyIni.Read("MQTT_ID");

            if (par_val == "")
                textBox_MQTT_ID.Text = @"A0A1A2A3A4A5A6";
            else
                textBox_MQTT_ID.Text = par_val;

            
            par_val = MyIni.Read("MQTT_PWD");

            if (par_val == "")
                textBox_MQTT_PWD.Text = @"7fTU6z2dH84CYry3";
            else
                textBox_MQTT_PWD.Text = par_val;

           
            par_val = MyIni.Read("MQTT_USR");

            if (par_val == "")
                textBox_MQTT_USR.Text = @"......";
            else
                textBox_MQTT_USR.Text = par_val;

        }


        private void Button_Load_settings_Click(object sender, EventArgs e)
        {
            string cfg_file;
            string par_val;


            if (checkBox_Cfg_Dbg_Rel.Checked == true)
            {
                cfg_file = @"MqttClientSimulatorBinary_REL_Set.ini";
            }
            else
            {
                cfg_file = @"MqttClientSimulatorBinary_DBG_Set.ini";
            }

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

            par_val = MyIni.Read("MQTT_PWD");

            if (par_val == "")
                textBox_MQTT_PWD.Text = @"7fTU6z2dH84CYry3";
            else
                textBox_MQTT_PWD.Text = par_val;



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

        private void button_flush_Click(object sender, EventArgs e)
        {
            string textFilePath = @".\cbor_cloud\REQ_FLUSH_VALUES.cbor";
            PublishTestFile(textFilePath);
        }

        private void buttonSet_Lines_Config_TTL_Click(object sender, EventArgs e)
        {
            string textFilePath = @".\cbor_cloud\REQ_SET_LINES_CONFIG_TTL.cbor";
            PublishTestFile(textFilePath);
        }

        private void button_ReqDBGInfo_Click(object sender, EventArgs e)
        {
            string textFilePath = @".\cbor_cloud\REQ_DBG_INFO.cbor";
            PublishTestFile(textFilePath);
        }

        private void textBox_MB_Pos_TextChanged(object sender, EventArgs e)
        {

        }

        private void set_flags_form_call()
        {
            FormFlags frm = new FormFlags();
            frm.Show();            
            frm.VisibleChanged += formVisibleChanged;
        }

        private void button_SetFlags_Click(object sender, EventArgs e)
        {
            flags_of = 1;
            set_flags_form_call();
        }

        private void formVisibleChanged(object sender, EventArgs e)
        {
            FormFlags frm = (FormFlags)sender;
            if (!frm.Visible)
            {
                string value = frm.ReturnText;
                if (flags_of == 1) textBox_Flags.Text = value;
                if (flags_of == 2) textBox_Flags_IR.Text = value;

                frm.Dispose();
            }
        }

        private void set_dev_update_form_call()
        {
            FormDevUpdateData frm = new FormDevUpdateData();
            frm.Show();
            frm.VisibleChanged += formVisibleChanged;
        }

        private void button_SetFlags_IR_Click(object sender, EventArgs e)
        {
            flags_of = 2;
            set_flags_form_call();
        }

        private void textBox_MB_Dim_TextChanged(object sender, EventArgs e)
        {

        }

        private void radioButton_qos0_Clik(object sender, EventArgs e)
        {
        }

        private void radioButton_qos0_CheckedChanged(object sender, EventArgs e)
        {
        }

        private void radioButton_qos1_Click(object sender, EventArgs e)
        {
        }

        private void radioButton_qos1_CheckedChanged(object sender, EventArgs e)
        {
        }

        private void timer_check_is_alive_Tick(object sender, EventArgs e)
        {

            if (MQTT_Connect == true)
            {
                //if connected I will check the connection 

                if (client.IsConnected == true)
                {
                    checkBox_mqtt_alive.Checked = true;
                    timer_check_is_alive.Interval = 1000;
                }
                else
                {                    
                    timer_check_is_alive.Interval = 10000; //delayed next check 
                    checkBox_mqtt_alive.Checked = false;
                    //the connetion goes down we try again
                    mqtt_connect(true);
                }


                if (radioButton_alive.Checked == false)
                {
                    radioButton_alive.Checked = true;
                }
                else
                {
                    radioButton_alive.Checked = false;
                }

            }
            else
            {
                //nothing to do      
                radioButton_alive.Checked = false;
            }

        }

        private void button7_Click_1(object sender, EventArgs e)
        {
            FormDevUpdateData frm = new FormDevUpdateData();
            frm.Show();
            frm.VisibleChanged += formVisibleChanged;
        }

        private void buttonSet_UpdateGwFW_Click(object sender, EventArgs e)
        {
            FormCBORUpdateGWFW frm = new FormCBORUpdateGWFW();
            frm.Show();
            frm.VisibleChanged += formVisibleChanged;
        }

        private void buttonUpdateCAData_Click(object sender, EventArgs e)
        {
            FormCBORUpdateCA frm = new FormCBORUpdateCA();
            frm.Show();
            frm.VisibleChanged += formVisibleChanged;
        }

        private void button_setDevsConfig_Click(object sender, EventArgs e)
        {
            FormCBORUpdateSetDevsConfig frm = new FormCBORUpdateSetDevsConfig();
            frm.Show();
            frm.VisibleChanged += formVisibleChanged;
        }

        private void buttonUpdateSetLinesConfig_Click(object sender, EventArgs e)
        {       
            FormCBORUpdateLinesConfig frm = new FormCBORUpdateLinesConfig();
            frm.Show();
            frm.VisibleChanged += formVisibleChanged;
        }

        private void button_web_test_Click(object sender, EventArgs e)
        {
            WebTest frm = new WebTest();
            frm.Show();
            frm.VisibleChanged += formVisibleChanged;
        }

        private void button_upload_log_file_Click(object sender, EventArgs e)
        {
            string textFilePath = @".\cbor_cloud\REQ_upload_range_file.cbor";

            DialogResult result1 = MessageBox.Show("Are you really sure to get the File ?",
               "Important Question",
               MessageBoxButtons.YesNo);

            if (result1 == DialogResult.Yes)
            {
                textBox_upload_file_info.Text = "";
                PublishTestFile(textFilePath);
            }

        }

        private void button_SetUploadLogCfg_Click(object sender, EventArgs e)
        {
            FormUploadFileFromControllerRange frm = new FormUploadFileFromControllerRange();
            frm.Show();
            frm.VisibleChanged += formVisibleChanged;
        }

        private void button_SetUploadLogCfgFull_Click(object sender, EventArgs e)
        {
            FormUploadFileFromControllerFull frm = new FormUploadFileFromControllerFull();
            frm.Show();
            frm.VisibleChanged += formVisibleChanged;
        }

        private void button_upload_log_file_full_Click(object sender, EventArgs e)
        {
            string textFilePath = @".\cbor_cloud\REQ_upload_full_file.cbor";

            DialogResult result1 = MessageBox.Show("Are you really sure to get the File ?",
               "Important Question",
               MessageBoxButtons.YesNo);

            if (result1 == DialogResult.Yes)
            {
                textBox_upload_file_info.Text = "";
                PublishTestFile(textFilePath);
            }
        }

        private void button_Decode_File_Click(object sender, EventArgs e)
        {
            //latest_upload_file = @"ciao"; //if you need to test the program call 
            System.Diagnostics.Process.Start(DECODE_FILE_PRG, latest_upload_file);
        }

        private void button_Unlock_Device_Click(object sender, EventArgs e)
        {
            
            //cmd 17
            string textFilePath = @".\cbor_cloud\REQ_SCAN_DEVICES.cbor";
            PublishTestFile(textFilePath);
            
            //wait for data and extract
         
        }

        private void Form1_Closing(object sender, FormClosingEventArgs e)
        {
            
            if (client != null)
            {
                //client.Disconnect();             
            }

            SessionLog.CloseLogFile();
        }

        private void Form1_Closed(object sender, FormClosedEventArgs e)
        {

        }

        private void timer_post_processing_Tick(object sender, EventArgs e)
        {
            if (MQTT_Connect == true)
            {
                if (client.IsConnected == true)
                {
                   timer_post_processing.Interval = 100;
            
                   if (post_processing.Length > 0)
                   {
                      Decode_Upload_Msg(post_processing);
                      Decode_Scan_Line_Msg(post_processing);
                      post_processing = @"";
                   }
                }
            }
         
        }

        private void button_decode_cmd17_Click(object sender, EventArgs e)
        {
            FormDecodeCMD17 frm = new FormDecodeCMD17();
            //pan   
            //last_cmd17_payload   = "0111B4BDFF01380406015400067D07D00000000000000000000005004300000000E3575E010189E9B200975E45729862C6B67352C13D010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000019332404F0036030204074D6F646275735F53616C76655F53696D756C61746370434F4D696E690000000000000000000000000000000000000000013A60";
            //chieb 
            //last_cmd17_payload = "0111B4BDFF01380407015400067D07D0000000000000000000000500000000000003E85E010189E9B200975E45729862C6B67352C13D0100000000000000000000000000000000000000000000000000000000000000000000008ADD804A739B4F33BF4A763EA3C19DA67465737400000000000000000000000000009332404F0036030204074D6F646275735F53616C76655F53696D756C61746370434F4D696E69000000000000000000000000000000000000000002F650";            

            frm.set_data(last_cmd17_payload);
            frm.Show();
            frm.VisibleChanged += formVisibleChanged;
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
                ushort msgId = client.Publish(val_req_post, Encoding.UTF8.GetBytes(textFile), qos_selected(), false);
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





