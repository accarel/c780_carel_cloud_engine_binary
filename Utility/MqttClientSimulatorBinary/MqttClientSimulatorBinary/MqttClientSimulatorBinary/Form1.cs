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

using Soft160.Data.Cryptography;




namespace MqttClientSimulatorBinary
{

    public partial class Form1 : Form
    {

        const int CBOR_CMD_SCAN_LINE = 3;
        const int CBOR_CMD_READ_HR = 6;
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

        const string DECODE_FILE_PRG = ".\\LogtransferAnalyzer.exe";


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


        bool decode_read_msg_flag = false;
        int decode_read_msg_error = -1;
        UInt16 val_value_returned = 0;
        const double DEVICE_RESPONSE_TOUT_MSEC = 10 * 1000;//sec to msec 

        bool Decode_Upload_Msg_file_complete_msg = true;


        /* variables for the unlock feature */
        public UInt16 v_mb_addr_srv_ver_maj = 0;
        public UInt16 v_mb_addr_srv_ver_min = 0;
           
        public UInt16 v_mb_addr_pn_0 = 0;
        public UInt16 v_mb_addr_pn_1 = 0;
        public UInt16 v_mb_addr_pn_2 = 0;
        public UInt16 v_mb_addr_pn_3 = 0;
        public UInt16 v_mb_addr_pn_4 = 0;
        public UInt16 v_mb_addr_pn_5 = 0;
        public UInt16 v_mb_addr_pn_6 = 0;
        public UInt16 v_mb_addr_pn_7 = 0;
        public UInt16 v_mb_addr_pn_8 = 0;
               
        public UInt16 v_mb_addr_rnd_num_h = 0;
        public UInt16 v_mb_addr_rnd_num_l = 0;
               
        public UInt16 v_mb_addr_unlock_bits_h = 0;
        public UInt16 v_mb_addr_unlock_bits_l = 0;
               
        public UInt16 v_mb_addr_rw_status = 0;

        byte[] oem_id_array = new byte[16];

        byte[] secret_carel = new byte[] { 0x39, 0x57, 0x61, 0x13, 0x72, 0xd5, 0x4e, 0x44, 0xa2, 0x67, 0x8f, 0x72, 0xce, 0x4e, 0x4f, 0x37 };

        List<byte> ulock_par_list = new List<byte>();

        byte[] lctx_buffer = new byte[0x36];
        int lctx_pointer = 0;

        /* ------------------------------ */

        double file_upload_start, file_upload_stop;

        /* ------------------------------ */

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
            textBox_Message.Invoke(new Action(() => textBox_Message.AppendText(s_msg + "\r\n")));
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

        void Decode_Read_Msg(string data_in) 
        {
            read_file_struct my_J_data;

            if (decode_read_msg_flag == false) return; //nothing scheduled
          
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
                textBox_Errors.AppendText("Decode_Read_Msg Error in the o_cmd buffer abort!");
                return;
            }

            //check if is a file upload cmd full or range
            if (cmd_value != CBOR_CMD_READ_HR)  return;

            string o_ali = (string)o["ali"];

            if (o_ali.Length == 0) return;
            
            int ali_value = 0;
            if (!(int.TryParse(o_ali, out ali_value)))
            {
                textBox_Errors.AppendText("Decode_Read_Msg Error in the o_ali buffer abort!");
                decode_read_msg_error = -2;
                goto Decode_Read_Msg_exit;
            }

            if (ali_value != 9999) goto Decode_Read_Msg_exit;  //msg not for us

            string o_val = (string)o["val"];

            if (o_val == null) 
            {
                //the echo of the request
                return; 
            }

            UInt16 val_value = 0;
            if (!(UInt16.TryParse(o_val, out val_value)))
            {
                textBox_Errors.AppendText("Decode_Read_Msg Error in the o_val buffer abort!");
                decode_read_msg_error = -3;
                goto Decode_Read_Msg_exit;
            }
            
            decode_read_msg_error = 0;
            val_value_returned = val_value;  //val_value is for us

            Decode_Read_Msg_exit:
              decode_read_msg_flag = false;

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
                textBox_Errors.AppendText("Decode_Read_Msg Error in the o_cmd buffer abort!");
                return;
            }

            //check if is a file upload cmd full or range
            if ((cmd_value != CBOR_CMD_FULL_FILE_UPLOAD) && (cmd_value != CBOR_CMD_RANGE_FILE_UPLOAD)) return;

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
                textBox_Errors.AppendText("Decode_Read_Msg Error in the o_fsz buffer abort!");
                return;                                    
            }

            int fst_value = 0;
            if (!(int.TryParse(o_fst, out fst_value)))
            {
                textBox_Errors.AppendText("Decode_Read_Msg Error in the o_fst buffer abort!");
                return;
            }

            int fle_value = 0;
            if (!(int.TryParse(o_fle, out fle_value)))
            {
                textBox_Errors.AppendText("Decode_Read_Msg Error in the o_fle buffer abort!");
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

                textBox_upload_file_info.AppendText("Created " + curFile + " size " + fsz_value.ToString()+ "\r\n" );
                latest_upload_file = curFile;
            }

            //insert the given file chunk in the right position


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

                string s = "Chunk " + fst_value.ToString() + ":" + fle_value.ToString() + "\r\n";

               textBox_upload_file_info.AppendText(s);

                if (s.Contains("Chunk 0:200"))
                {
                    file_upload_start = (new TimeSpan(DateTime.Now.Ticks)).TotalMilliseconds;
                }

            }

            if ((fst_value + fle_value) >= fsz_value)
            {
                string sdata;
                int total_size;

                total_size = fst_value + fle_value;
                sdata = String.Format("{0} {1} of {2}", "File transfer reached the size", (total_size), fsz_value);

                if (Decode_Upload_Msg_file_complete_msg == false)
                {
                    Decode_Upload_Msg_file_complete_msg = true;

                    //take in mind that > (major) is not possible theoretichally anyway not to bad to have a feedback
                    textBox_upload_file_info.AppendText(sdata + " Try to decode!\r\n");

                    file_upload_stop = (new TimeSpan(DateTime.Now.Ticks)).TotalMilliseconds;
                    double time_val = (file_upload_stop - file_upload_start) / 1000;

                    double speed = (double)(total_size) / time_val;

                    sdata = String.Format("Speed {0:0.0} bytes/sec.\r\n", speed);
                    textBox_upload_file_info.AppendText(sdata);

                    sdata = String.Format("Trasf. time {0:0.0} sec.\r\n", time_val);
                    textBox_upload_file_info.AppendText(sdata);
                }
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
               textBox_Errors.AppendText("Error in the o_cmd buffer abort!\r\n");
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
            Save_Last_Used_Values();
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
                textBox_Message.AppendText("Error during conversion of > fun \r\n");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Addr.Text, out value))
            {
                cbor.Add(@"adr", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > adr \r\n");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Dim.Text, out value))
            {
                cbor.Add(@"dim", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > dim \r\n");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Pos.Text, out value))
            {
                cbor.Add(@"pos", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > pos \r\n");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Len.Text, out value))
            {
                cbor.Add(@"len", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > len \r\n");
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
                textBox_Message.AppendText("Error during conversion of > flg \r\n");
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
                textBox_Message.AppendText("Error during conversion of > dim \r\n");
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
                textBox_Message.AppendText("Error during conversion of > fun \r\n");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Addr.Text, out value))
            {
                cbor.Add(@"adr", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > adr \r\n");
            }



            value = 0;
            if (Int32.TryParse(textBox_MB_Pos.Text, out value))
            {
                cbor.Add(@"pos", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > pos \r\n");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Len.Text, out value))
            {
                cbor.Add(@"len", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > len \r\n");
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
                textBox_Message.AppendText("Error during conversion of > flg \r\n");
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
                textBox_Message.AppendText("Error during conversion of > fun \r\n");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Addr_Coil.Text, out value))
            {
                cbor.Add(@"adr", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > adr \r\n");
            }

            value = 0;
            if (Int32.TryParse(@"1", out value))
            {
                cbor.Add(@"dim", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > dim \r\n");
            }

            value = 0;
            if (Int32.TryParse(@"1", out value))
            {
                cbor.Add(@"pos", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > pos \r\n");
            }

            value = 0;
            if (Int32.TryParse(@"1", out value))
            {
                cbor.Add(@"len", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > len \r\n");
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
                textBox_Message.AppendText("Error during conversion of > flg \r\n");
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
                textBox_Message.AppendText("Error during conversion of > fun \r\n");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Addr_Coil.Text, out value))
            {
                cbor.Add(@"adr", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > adr \r\n");
            }

            value = 0;
            if (Int32.TryParse(@"1", out value))
            {
                cbor.Add(@"dim", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > dim \r\n");
            }

            value = 0;
            if (Int32.TryParse(@"1", out value))
            {
                cbor.Add(@"pos", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > pos \r\n");
            }

            value = 0;
            if (Int32.TryParse(@"1", out value))
            {
                cbor.Add(@"len", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > len \r\n");
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
                textBox_Message.AppendText("Error during conversion of > flg \r\n");
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
                textBox_Message.AppendText("Error during conversion of > fun \r\n");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Addr_DI.Text, out value))
            {
                cbor.Add(@"adr", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > adr \r\n");
            }

            value = 0;
            if (Int32.TryParse(@"1", out value))
            {
                cbor.Add(@"dim", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > dim \r\n");
            }

            value = 0;
            if (Int32.TryParse(@"1", out value))
            {
                cbor.Add(@"pos", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > pos \r\n");
            }

            value = 0;
            if (Int32.TryParse(@"1", out value))
            {
                cbor.Add(@"len", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > len \r\n");
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
                textBox_Message.AppendText("Error during conversion of > flg \r\n");
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
                textBox_Message.AppendText("Error during conversion of > fun \r\n");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Addr_IR.Text, out value))
            {
                cbor.Add(@"adr", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > adr \r\n");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Dim_IR.Text, out value))
            {
                cbor.Add(@"dim", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > dim \r\n");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Pos_IR.Text, out value))
            {
                cbor.Add(@"pos", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > pos \r\n");
            }

            value = 0;
            if (Int32.TryParse(textBox_MB_Len_IR.Text, out value))
            {
                cbor.Add(@"len", value);
            }
            else
            {
                textBox_Message.AppendText("Error during conversion of > len \r\n");
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
                textBox_Message.AppendText("Error during conversion of > flg \r\n");
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
                Decode_Upload_Msg_file_complete_msg = false;
                PublishTestFile(textFilePath);
            }
        }

        private void button_Decode_File_Click(object sender, EventArgs e)
        {
            //latest_upload_file = @"ciao"; //if you need to test the program call 1=txt 2=bin
            System.Diagnostics.Process.Start(DECODE_FILE_PRG, "2 " + latest_upload_file);
        }


         /* 
         true= SUCCESS
         false = fail
         */
        bool manage_tout(bool monitored_var)
        {
            bool ret_val = false;

            double start_time = (new TimeSpan(DateTime.Now.Ticks)).TotalMilliseconds;

            start_time += DEVICE_RESPONSE_TOUT_MSEC;
            bool exitfromloop = true;

            while (exitfromloop)
            {
                if (monitored_var == false) exitfromloop = false; //msg received
                double elapsed_time = (new TimeSpan(DateTime.Now.Ticks)).TotalMilliseconds;
                if (elapsed_time > start_time)
                {
                    //ok timout reached 
                    exitfromloop = false;
                }

                Application.DoEvents();
            }

            if (monitored_var == false) ret_val = true;

            return ret_val;
        }


        /*
         * hr_request
         * 
         true= SUCCESS
         false = fail
         */
        public bool hr_request(string targetname, int hraddress)
        {
            int value;
            int dim_value;
            bool ret_val=false;

            var cbor = CBORObject.NewMap();

            cbor.Add(@"ver", CBOR_PAYLOAD_VER);
            cbor.Add(@"rto", targetname + @"/hr_w_val");
            value = 6;
            cbor.Add(@"cmd", value);
            cbor.Add(@"ali", @"9999");

            dim_value = 16;
            cbor.Add(@"dim", dim_value);

            int dim_fun_n = 3;  
            cbor.Add(@"fun", dim_fun_n);
            cbor.Add(@"adr", hraddress);

            int pos_value = 1;
            cbor.Add(@"pos", pos_value);

            int len_value = 16;
            cbor.Add(@"len", len_value);

            cbor.Add(@"a", "1.0");
            cbor.Add(@"b", "0.0");

            int flg_value = 0;
            cbor.Add("flg", flg_value);

            // The following converts the map to canonical CBOR
            byte[] cbor_bytes = cbor.EncodeToBytes(CBOREncodeOptions.DefaultCtap2Canonical);

            try
            {
                ushort msgId = client.Publish(val_req_post, cbor_bytes, qos_selected(), false);
                decode_read_msg_error = -1;
                val_value_returned = 0;
                decode_read_msg_flag = true;
            }
            catch (System.NullReferenceException)
            {
                MessageBoxUpdated("MQTT Not Initialized");
                ret_val = false;
                goto hr_request_exit;
            }

            //now wait for the answer or maybe error                                     
            double start_time = (new TimeSpan(DateTime.Now.Ticks)).TotalMilliseconds;

            start_time += DEVICE_RESPONSE_TOUT_MSEC;
            bool exitfromloop = true;

            while (exitfromloop)
            {
                if (decode_read_msg_flag == false) exitfromloop = false; //msg received
                double elapsed_time = (new TimeSpan(DateTime.Now.Ticks)).TotalMilliseconds;
                if (elapsed_time > start_time)
                {
                    //ok timout reached 
                    exitfromloop = false;
                }

                Application.DoEvents();
            }

            if (decode_read_msg_flag == false) ret_val = true;

            hr_request_exit:
            Task.Delay(1500);
            return ret_val;

        }

        public bool hr_w_request(string targetname, int hraddress, UInt16 value4target)
        {
            int value;
            int dim_value;
            bool ret_val = false;

            var cbor = CBORObject.NewMap();

            cbor.Add(@"ver", CBOR_PAYLOAD_VER);
            cbor.Add(@"rto", targetname + @"/hr_w_val");
            value = 7;
            cbor.Add(@"cmd", value);
            cbor.Add(@"ali", @"9999");
           
            dim_value = 16;
            cbor.Add(@"dim", dim_value);

            value = 6;
            cbor.Add(@"fun", value);

            cbor.Add(@"adr", hraddress);

            value = 1;
            cbor.Add(@"pos", value);

            int len_value = 16;
            cbor.Add(@"len", len_value);

            cbor.Add(@"a", "1.0");
            cbor.Add(@"b", "0.0");

            int flg_value = 0;
            cbor.Add("flg", flg_value);

            cbor.Add(@"val", value4target.ToString());

            // The following converts the map to canonical CBOR
            byte[] cbor_bytes = cbor.EncodeToBytes(CBOREncodeOptions.DefaultCtap2Canonical);

            try
            {
                ushort msgId = client.Publish(val_req_post, cbor_bytes, qos_selected(), false);
                decode_read_msg_error = -1;
                val_value_returned = 0;
                decode_read_msg_flag = false;
            }
            catch (System.NullReferenceException)
            {
                MessageBoxUpdated("MQTT Not Initialized");
                ret_val = false;
                goto hr_w_request_exit;
            }

            //now wait for the answer or maybe error                                
            //if (manage_tout(decode_read_msg_flag) == true) ret_val = true;


            double start_time = (new TimeSpan(DateTime.Now.Ticks)).TotalMilliseconds;

            start_time += DEVICE_RESPONSE_TOUT_MSEC;
            bool exitfromloop = true;

            while (exitfromloop)
            {
                if (decode_read_msg_flag == false) exitfromloop = false; //msg received
                double elapsed_time = (new TimeSpan(DateTime.Now.Ticks)).TotalMilliseconds;
                if (elapsed_time > start_time)
                {
                    //ok timout reached 
                    exitfromloop = false;
                }

                Application.DoEvents();
            }

            if (decode_read_msg_flag == false) ret_val = true;


         hr_w_request_exit:
            Task.Delay(1500);
            return ret_val;
        }

        private void Dump_Unlock_Values(bool show_oem_id)
        {
            string sline;
            sline = String.Format("MAJ/MIN - {0}{1}\r\n", v_mb_addr_srv_ver_maj, v_mb_addr_srv_ver_min);
            textBox_Message.AppendText(sline);

            sline = String.Format("PN - {0}{1}{2}{3}{4}{5}{6}{7}{8}{9}{10}{11}{12}{13}{14}{15}{16}{17}  \r\n",
                                 (char)(v_mb_addr_pn_0 >> 8), (char)(v_mb_addr_pn_0 & 0xFF),
                                 (char)(v_mb_addr_pn_1 >> 8), (char)(v_mb_addr_pn_1 & 0xFF),
                                 (char)(v_mb_addr_pn_2 >> 8), (char)(v_mb_addr_pn_2 & 0xFF),
                                 (char)(v_mb_addr_pn_3 >> 8), (char)(v_mb_addr_pn_3 & 0xFF),
                                 (char)(v_mb_addr_pn_4 >> 8), (char)(v_mb_addr_pn_4 & 0xFF),
                                 (char)(v_mb_addr_pn_5 >> 8), (char)(v_mb_addr_pn_5 & 0xFF),
                                 (char)(v_mb_addr_pn_6 >> 8), (char)(v_mb_addr_pn_6 & 0xFF),
                                 (char)(v_mb_addr_pn_7 >> 8), (char)(v_mb_addr_pn_7 & 0xFF),
                                 (char)(v_mb_addr_pn_8 >> 8), (char)(v_mb_addr_pn_8 & 0xFF)
                                 );
            textBox_Message.AppendText(sline);

            sline = String.Format("RND H/L - {0:X} {1:X}\r\n", v_mb_addr_rnd_num_h, v_mb_addr_rnd_num_l);
            textBox_Message.AppendText(sline);

            int vl;
            sline = String.Format("Unlock bits H/L\r\n");
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_l & 0x01;
            sline = String.Format("bit 0: Stc(file 11)       ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_l & 0x02;
            sline = String.Format("bit 1: logs(file 1000)    ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_l & 0x04;
            sline = String.Format("bit 2: allarmi(file 3000) ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_l & 0x08;
            sline = String.Format("bit 3: upgrade(file 5000) ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_l & 0x10;
            sline = String.Format("bit 4: block register/coil={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_l & 0x20;
            sline = String.Format("bit 5: NA                 ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_l & 0x40;
            sline = String.Format("bit 6: NA                 ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_l & 0x80;
            sline = String.Format("bit 7: NA                 ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_l & 0x0100;
            sline = String.Format("bit 8: NA                 ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_l & 0x200;
            sline = String.Format("bit 9: NA                 ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_l & 0x400;
            sline = String.Format("bit 10: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_l & 0x800;
            sline = String.Format("bit 11: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_l & 0x1000;
            sline = String.Format("bit 12: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_l & 0x2000;
            sline = String.Format("bit 13: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_l & 0x4000;
            sline = String.Format("bit 14: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_l & 0x8000;
            sline = String.Format("bit 15: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);
            //change the reg
            vl = v_mb_addr_unlock_bits_h & 0x01;
            sline = String.Format("bit 16: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_h & 0x02;
            sline = String.Format("bit 17: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_h & 0x04;
            sline = String.Format("bit 18: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_h & 0x08;
            sline = String.Format("bit 19: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_h & 0x10;
            sline = String.Format("bit 20: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_h & 0x20;
            sline = String.Format("bit 21: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_h & 0x40;
            sline = String.Format("bit 22: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_h & 0x80;
            sline = String.Format("bit 23: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_h & 0x0100;
            sline = String.Format("bit 24: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_h & 0x200;
            sline = String.Format("bit 25: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_h & 0x400;
            sline = String.Format("bit 26: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_h & 0x800;
            sline = String.Format("bit 27: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_h & 0x1000;
            sline = String.Format("bit 28: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_h & 0x2000;
            sline = String.Format("bit 28: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_h & 0x4000;
            sline = String.Format("bit 30: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_unlock_bits_h & 0x8000;
            sline = String.Format("bit 31: NA                ={0}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);


            /* ---------------------------- */
            sline = String.Format("Status bits H/L\r\n");
            textBox_Message.AppendText(sline);

            vl = v_mb_addr_rw_status; 
            sline = String.Format("0=blocked 1=unblock {0:X}\r\n", vl.ToString());
            textBox_Message.AppendText(sline);


            /* ---------------------------- */
            if (show_oem_id == true)
            {
                sline = "OEM ID \r\n";
                for (int count = 0; count < 16; count++)
                {
                    sline += String.Format("{0:X} ", oem_id_array[count]);
                }
                textBox_Message.AppendText(sline + "\r\n");
            }
            /* ---------------------------- */

        }


        private int copy_and_increase(UInt16 in_val, int count)
        {
            int counter;            
            byte h_val = 0;
            byte l_val = 0;

            counter = count;

            h_val = (byte)(((in_val & 0xFF00) >> 8));
            l_val = (byte)((in_val & 0xFF));

            lctx_buffer[counter] = h_val; counter++;
            lctx_buffer[counter] = l_val; counter++;
            return counter;
        }


        bool read_ulock_block_60000() 
        {    /* reset all vars */
             v_mb_addr_srv_ver_maj = 0;
             v_mb_addr_srv_ver_min = 0;
             
             v_mb_addr_pn_0 = 0;
             v_mb_addr_pn_1 = 0;
             v_mb_addr_pn_2 = 0;
             v_mb_addr_pn_3 = 0;
             v_mb_addr_pn_4 = 0;
             v_mb_addr_pn_5 = 0;
             v_mb_addr_pn_6 = 0;
             v_mb_addr_pn_7 = 0;
             v_mb_addr_pn_8 = 0;
             
             v_mb_addr_rnd_num_h = 0;
             v_mb_addr_rnd_num_l = 0;
             
             v_mb_addr_unlock_bits_h = 0;
             v_mb_addr_unlock_bits_l = 0;
             
             v_mb_addr_rw_status = 0;

            /* OK now we need to read some HR @ 60000 */
            if (!hr_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_SRV_VER_MAJ)) goto read_ulock_block_60000_exit; v_mb_addr_srv_ver_maj = val_value_returned;
            if (!hr_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_SRV_VER_MIN)) goto read_ulock_block_60000_exit; v_mb_addr_srv_ver_min = val_value_returned;
            if (!hr_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_PN_0)) goto read_ulock_block_60000_exit; v_mb_addr_pn_0 = val_value_returned;
            if (!hr_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_PN_1)) goto read_ulock_block_60000_exit; v_mb_addr_pn_1 = val_value_returned;
            if (!hr_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_PN_2)) goto read_ulock_block_60000_exit; v_mb_addr_pn_2 = val_value_returned;
            if (!hr_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_PN_3)) goto read_ulock_block_60000_exit; v_mb_addr_pn_3 = val_value_returned;
            if (!hr_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_PN_4)) goto read_ulock_block_60000_exit; v_mb_addr_pn_4 = val_value_returned;
            if (!hr_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_PN_5)) goto read_ulock_block_60000_exit; v_mb_addr_pn_5 = val_value_returned;
            if (!hr_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_PN_6)) goto read_ulock_block_60000_exit; v_mb_addr_pn_6 = val_value_returned;
            if (!hr_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_PN_7)) goto read_ulock_block_60000_exit; v_mb_addr_pn_7 = val_value_returned;
            if (!hr_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_PN_8)) goto read_ulock_block_60000_exit; v_mb_addr_pn_8 = val_value_returned;
            if (!hr_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_RND_NUM_H)) goto read_ulock_block_60000_exit; v_mb_addr_rnd_num_h = val_value_returned;
            if (!hr_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_RND_NUM_L)) goto read_ulock_block_60000_exit; v_mb_addr_rnd_num_l = val_value_returned;
            if (!hr_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_UNLOCK_BITS_H)) goto read_ulock_block_60000_exit; v_mb_addr_unlock_bits_h = val_value_returned;
            if (!hr_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_UNLOCK_BITS_L)) goto read_ulock_block_60000_exit; v_mb_addr_unlock_bits_l = val_value_returned;
            if (!hr_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_RW_STATUS)) goto read_ulock_block_60000_exit; v_mb_addr_rw_status = val_value_returned;

            return false;

            read_ulock_block_60000_exit:
            return true;

        }

        unsafe private void Unlock_Device_Routine()
        {
            const double DEVICE_RESPONSE_TOUT_MSEC = 10 * 1000;//sec to msec 

            last_cmd17_payload = "";
            string textFilePath = @".\cbor_cloud\REQ_SCAN_DEVICES.cbor";
            PublishTestFile(textFilePath);
            Application.DoEvents();

            //WAIT for answer with timeout
            double start_time = (new TimeSpan(DateTime.Now.Ticks)).TotalMilliseconds;

            start_time += DEVICE_RESPONSE_TOUT_MSEC;
            bool exitfromloop = true;

            while (exitfromloop)
            {
                if (last_cmd17_payload.Length > 0) exitfromloop = false; //msg received
                double elapsed_time = (new TimeSpan(DateTime.Now.Ticks)).TotalMilliseconds;
                if (elapsed_time > start_time)
                {
                    //ok timout reached 
                    exitfromloop = false;
                }

                Application.DoEvents();
            }

            if (last_cmd17_payload.Length == 0)
            {
                MessageBoxInfo(("Timeout! the device not respond in " + DEVICE_RESPONSE_TOUT_MSEC.ToString() + " msec"),
                               "Info");
                goto unlock_device_end;
            }

            FormDecodeCMD17 frm = new FormDecodeCMD17();
            frm.set_data(last_cmd17_payload);
            frm.Silent_Decoding();
            bool locked = frm.is_Control_Locked();

            if (!locked)
            {
                textBox_upload_file_info.AppendText("Unlock - locked bit NOT present ... end \r\n");
                goto unlock_device_end;
            }
            textBox_Message.AppendText("Unlock - locked bit present go ahead \r\n");


            string oem_id_str;
            byte[] oem_id_data = new byte[16];

            string tmp_oem_id_str = frm.get_OEM_ID();
            string temp_ov_oem_id_str = frm.get_OV_OEM_ID();

            /* check if we use the OV in this case we use it*/
            if (temp_ov_oem_id_str.Length > 0)
            {
                oem_id_str = temp_ov_oem_id_str;
                oem_id_data = frm.get_OV_OEM_ID_array();
            }
            else 
            {
                oem_id_str = tmp_oem_id_str;
                oem_id_data = frm.get_OEM_ID_array();
            }


            /* OK now we need to read some HR @ 60000 */     
            if (read_ulock_block_60000() == true) goto unlock_device_fail;

            /* convert oem id string into a byte array */
            string stmp;
            for (int count = 0; count < oem_id_str.Length; count += 2)
            {
                stmp = oem_id_str.Substring(count, 2);
                int idx = count / 2;
                oem_id_array[idx] = Convert.ToByte(stmp, 16);
            }


            /* all data retrieved just for reference show it */
            Dump_Unlock_Values(true);
            /* --------------------------------------------- */

            /* put all in the buffer for CRC calculation of the passpartout */
            uint oem_unlock_key = 0; 
            lctx_pointer = 0;

            /* PART NUMBER */
            lctx_pointer = copy_and_increase(v_mb_addr_pn_0, lctx_pointer);
            lctx_pointer = copy_and_increase(v_mb_addr_pn_1, lctx_pointer);
            lctx_pointer = copy_and_increase(v_mb_addr_pn_2, lctx_pointer);
            lctx_pointer = copy_and_increase(v_mb_addr_pn_3, lctx_pointer);
            lctx_pointer = copy_and_increase(v_mb_addr_pn_4, lctx_pointer);
            lctx_pointer = copy_and_increase(v_mb_addr_pn_5, lctx_pointer);
            lctx_pointer = copy_and_increase(v_mb_addr_pn_6, lctx_pointer);
            lctx_pointer = copy_and_increase(v_mb_addr_pn_7, lctx_pointer);
            lctx_pointer = copy_and_increase(v_mb_addr_pn_8, lctx_pointer);

            /* OEM ID or OV OEM ID see above */
            for (int count = 0; count < 16; count++)
            {
                lctx_buffer[lctx_pointer] = oem_id_array[count]; 
                lctx_pointer++;
            }
            
            /* UNCLOCK BITS all */
            UInt16[] val_split_ulock = new UInt16[2];
            val_split_ulock[0] = (UInt16)((unlock_feat_data.UNLOCK_BITS & 0x0000FFFF));
            val_split_ulock[1] = (UInt16)((unlock_feat_data.UNLOCK_BITS & 0xFFFF0000) >> 16);

            lctx_pointer = copy_and_increase(val_split_ulock[0], lctx_pointer);
            lctx_pointer = copy_and_increase(val_split_ulock[1], lctx_pointer);


            /* CAREL SECRET */
            for (int count = 0; count < 16; count++) 
            {
                lctx_buffer[lctx_pointer] = secret_carel[count];
                lctx_pointer++;
            }

            oem_unlock_key = CRC.Crc32(lctx_buffer, oem_unlock_key);  //this is the passpartout base

            /* now compute the session key based on RND + oem_unlock_key */
            lctx_pointer = 0;
            lctx_pointer = copy_and_increase(v_mb_addr_rnd_num_h, lctx_pointer);
            lctx_pointer = copy_and_increase(v_mb_addr_rnd_num_l, lctx_pointer);

            UInt16 v;
            v = (UInt16)((oem_unlock_key & 0xFFFF0000) >> 16);
            lctx_pointer = copy_and_increase(v, lctx_pointer);

            v = (UInt16)((oem_unlock_key & 0x0000FFFF));
            lctx_pointer = copy_and_increase(v, lctx_pointer);


            /* a buffer of exact size for the CRC routine */
            byte[] session_buff = new byte[lctx_pointer];
            for (int ii=0; ii< lctx_pointer; ii++) 
            {
                session_buff[ii] = lctx_buffer[ii];
            }

            uint session_unlock_key = 0;
            session_unlock_key = CRC.Crc32(session_buff, session_unlock_key);


            /* session_unlock_key contains now the unlock key now write it to the controller */
            UInt16 session_l = 0;
            UInt16 session_h = 0;

            session_h = (UInt16)(((session_unlock_key & 0xFFFF0000) >> 16));
            session_l = (UInt16)((session_unlock_key & 0x0000FFFF));

            textBox_Message.AppendText(String.Format("Session H = {0:X} \r\n", session_h));
            textBox_Message.AppendText(String.Format("Session L = {0:X} \r\n", session_l));

            /* write and try to unlock */
            hr_w_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_UNLOCK_BITS_H, val_split_ulock[1]);
            hr_w_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_UNLOCK_BITS_L, val_split_ulock[0]);

            hr_w_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_SESSION_UNLOCK_KEY_H, session_h);
            hr_w_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_SESSION_UNLOCK_KEY_L, session_l);

            /* trigger */
            hr_w_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_RW_STATUS, unlock_feat_data.RW_STATUS_KEY_WRITTEN);


            /* check the locking status */
            get_ulock_info();

            unlock_device_end:
            button_Unlock_Device.Enabled = true;
            return;

            unlock_device_fail:
            button_Unlock_Device.Enabled = true;
            return;
        }


        private void button_Unlock_Device_Click(object sender, EventArgs e)
        {
            button_Unlock_Device.Enabled = false;
            Unlock_Device_Routine();
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
            if (MQTT_Connect == true)
            {
                mqtt_connect(false);
            }
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
                      Decode_Read_Msg(post_processing);
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
            frm.set_data(last_cmd17_payload);
            frm.Show();
            frm.VisibleChanged += formVisibleChanged;
        }

        private void textBox_timer_pub2res_TextChanged(object sender, EventArgs e)
        {

        }

        private void checkBox_Show_Usr_Pwd_CheckedChanged(object sender, EventArgs e)
        {

            if (checkBox_Show_Usr_Pwd.Checked == true)
            {
                textBox_MQTT_USR.PasswordChar = (char)(0);
                textBox_MQTT_PWD.PasswordChar = (char)(0); 
            }
            else
            {
                textBox_MQTT_USR.PasswordChar = '*';
                textBox_MQTT_PWD.PasswordChar = '*';
            }

        }

        private void button_error_notification_Click(object sender, EventArgs e)
        {
            textBox_Errors.Text = "";
        }

        private void get_ulock_info() 
        {
            if (hr_w_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_RW_STATUS, unlock_feat_data.RW_STATUS_GET_SERVICE) == true)
            {
                if (hr_request(textBox_Target.Text, unlock_feat_data.MB_ADDR_RW_STATUS))
                {
                    v_mb_addr_rw_status = val_value_returned;
                    string sline;
                    sline = String.Format("Status bits H/L\r\n");
                    textBox_Message.AppendText(sline);
                    sline = String.Format("0=blocked 1=unblock NOW = {0:X}\r\n", v_mb_addr_rw_status.ToString());
                    textBox_Message.AppendText(sline);
                }

            }

        }

        private void button_get_ulock_info_Click(object sender, EventArgs e)
        {
            get_ulock_info();
        }

        private void button_upload_abort_Click(object sender, EventArgs e)
        {
            string textFilePath = @".\cbor_cloud\REQ_abort_upload.cbor";

            DialogResult result1 = MessageBox.Show("ABORT are you really sure ?",
               "Important Question",
               MessageBoxButtons.YesNo);

            if (result1 == DialogResult.Yes)
            {
                textBox_upload_file_info.Text = "";
                PublishTestFile(textFilePath);
            }
        }

        private void button_Setup_uploadAbort_Click(object sender, EventArgs e)
        {
            Form_Abort_Upload frm = new Form_Abort_Upload();
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





