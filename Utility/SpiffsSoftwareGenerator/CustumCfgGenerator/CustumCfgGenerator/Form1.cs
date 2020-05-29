using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;




namespace CustumCfgGenerator
{
    public partial class Form1 : Form
    {
        public const int NUM_OF_CFG_VERSION = 2;
        public const int NUM_OF_BROKER = 60;
        public const int NUM_OF_PORT = 30;
        public const int NUM_OF_PSSW = 34;
        public const int NUM_OF_USER = 34;
        public const int NUM_OF_NTPSERVER = 30;
        public const int NUM_OF_NTPPORT = 6;
        public const int NUM_OF_CRC = 2;
        public const int NUM_OF_CHAR_IN_APN_FIELD = 64;


        public struct myDataToStore
        {
            public byte[] cfg_version;
            public ushort enc_key;
            public byte[] mqtt_broker;
            public byte[] mqtt_port;
            public byte[] mqtt_pssw;
            public byte[] mqtt_user;
            public byte[] ntp_server;
            public byte[] ntp_port;

            public byte[] apn_name;
            public byte[] apn_user;
            public byte[] apn_password;
        }

        private myDataToStore mySpiffs;

        //
        // old...without use of struct
        //
        //byte[] cfg_version = new byte[NUM_OF_CFG_VERSION];
        //ushort enc_key = 0;
        //byte[] mqtt_broker = null;
        //byte[] mqtt_port = null;
        //byte[] mqtt_pssw = null;
        //byte[] mqtt_user = null;
        //byte[] ntp_server = null;
        //byte[] ntp_port = null;


        Random rnd = new Random();

        byte[] CalcCRC = new byte[NUM_OF_CRC];

        public Form1()
        {   
            InitializeComponent();

            mySpiffs = new myDataToStore();
            mySpiffs.cfg_version = new byte[NUM_OF_CFG_VERSION];

            mySpiffs.mqtt_broker = new byte[NUM_OF_BROKER];
            mySpiffs.mqtt_port = new byte[NUM_OF_PORT];
            mySpiffs.mqtt_pssw = new byte[NUM_OF_PSSW];
            mySpiffs.mqtt_user = new byte[NUM_OF_USER];
            mySpiffs.ntp_server = new byte[NUM_OF_NTPSERVER];
            mySpiffs.ntp_port = new byte[NUM_OF_NTPPORT];

            mySpiffs.apn_password = new byte[NUM_OF_CHAR_IN_APN_FIELD];
            mySpiffs.apn_user = new byte[NUM_OF_CHAR_IN_APN_FIELD];
            mySpiffs.apn_name = new byte[NUM_OF_CHAR_IN_APN_FIELD];

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {   
            string cfg_data     = null;
            string cfg_data_usr = null;

            ushort randomNum = 0;

            string batch_file = "\\spiffs_image\\go_make_spiffs.bat";

            // generazione casuale della chiave di cifratura
            randomNum = (ushort)(rnd.Next(1, 65535));

            mySpiffs.enc_key = randomNum;


            //
            //  Nego il risultato di ogni check in quanto ritorna --->  true se tutto a 0, false se c'è qualcosa.
            //
            bool ThereIsZeroes = (mySpiffs.mqtt_broker.All(singleByte => singleByte == 0)) ||
                                 (mySpiffs.mqtt_port.All(singleByte => singleByte == 0)) ||
                                 (mySpiffs.mqtt_pssw.All(singleByte => singleByte == 0)) ||
                                 (mySpiffs.mqtt_user.All(singleByte => singleByte == 0)) ||
                                 (mySpiffs.ntp_port.All(singleByte => singleByte == 0)) ||
                                 (mySpiffs.ntp_server.All(singleByte => singleByte == 0)) ||
                                 (mySpiffs.apn_name.All(singleByte => singleByte == 0)) ||
                                 (mySpiffs.apn_user.All(singleByte => singleByte == 0)) ||
                                 (mySpiffs.apn_password.All(singleByte => singleByte == 0)) ||
                                 (mySpiffs.cfg_version.All(singleByte => singleByte == 0));

            

            // genera file .bin per esp32
            // controllo il contenuto delle textbox
            try
            {
                if(!ThereIsZeroes)
                {
                    string exeFolder = System.IO.Path.GetDirectoryName(Application.ExecutablePath);

                    cfg_data = exeFolder + "\\spiffs_image\\dir_image\\cfgdef.bin";
                    File.Delete(cfg_data);

                    //cfg_data_usr = exeFolder + "\\spiffs_image\\dir_image\\cfgusr.bin";
                    //File.Delete(cfg_data_usr);

                    using (FileStream stream = new FileStream(cfg_data, FileMode.Create))
                    {
                        using (BinaryWriter writer = new BinaryWriter(stream))
                        {
                            writer.Write(mySpiffs.cfg_version);
                            writer.Write(mySpiffs.enc_key);
                            writer.Write(mySpiffs.mqtt_broker);
                            writer.Write(mySpiffs.mqtt_port);

                            encrypt(mySpiffs.mqtt_pssw, randomNum);                           
                            writer.Write(mySpiffs.mqtt_pssw);

                            encrypt(mySpiffs.mqtt_user, randomNum);
                            writer.Write(mySpiffs.mqtt_user);

                            writer.Write(mySpiffs.ntp_server);
                            writer.Write(mySpiffs.ntp_port);

                            writer.Write(mySpiffs.apn_name);
                            writer.Write(mySpiffs.apn_user);
                            writer.Write(mySpiffs.apn_password);

                            writer.Close();

                            var source = @cfg_data;
                           // var destination = @cfg_data_usr;

                            
                            byte[] file = File.ReadAllBytes(source);

                            //GetCRC(file, CalcCRC);
                            ushort MyCrc = Crc16.ComputeCrc(file);
                            CalcCRC[0] = (byte)MyCrc;
                            CalcCRC[1] = (byte)(MyCrc>>8);

                            int total_len = file.Length + 2;

                            byte[] newFile = new byte[total_len];

                            Array.Copy(file, 0, newFile, 0, file.Length);

                            newFile[total_len - 2] = CalcCRC[0];
                            newFile[total_len - 1] = CalcCRC[1];

                            File.WriteAllBytes(source, newFile);

                            // clone the file
                        //    try
                        //    {
                        //        File.Copy(source, destination);
                        //    }
                        //    catch
                        //    {
                        //       MessageBox.Show("Rimuovere i file precedenti o rinominarli");
                        //   }


                            Process proc = null;
                            try
                            {
                                string targetDir = string.Format(@exeFolder + "\\spiffs_image");//this is where mybatch.bat lies
                                proc = new Process();
                                proc.StartInfo.WorkingDirectory = targetDir;
                                proc.StartInfo.FileName = "go_make_spiffs.bat";
                                proc.StartInfo.Arguments = string.Format("10");//this is argument
                                proc.StartInfo.CreateNoWindow = false;
                                proc.Start();
                                proc.WaitForExit();
                                proc.Close();

                                // feedback operazione conclusa
                                MessageBox.Show(" File Spiffs.bin generated correctly");              
                            }
                            catch (Exception ex)
                            {
                                Console.WriteLine("Exception Occurred :{0},{1}", ex.Message, ex.StackTrace.ToString());
                            }

                            button1.BackColor = Color.Green;
                        }
                    }
                }
                else
                {
                    MessageBox.Show("Immettere tutti i campi");
                }
            }
            catch (Exception)
            {
                MessageBox.Show("Immettere tutti i campi");
            }
        }

        private void textMqttBroker_TextChanged(object sender, EventArgs e)
        {
            button1.BackColor = Color.Gray;

            string newBroker = textMqttBroker.Text;

            byte[] tmp = Encoding.ASCII.GetBytes(newBroker);

            //int startAt = mqtt_broker.Length - tmp.Length;
            if(tmp.Length != 0)
            {
                mySpiffs.mqtt_broker = new byte[NUM_OF_BROKER];
                Array.Copy(tmp, 0, mySpiffs.mqtt_broker, 0, tmp.Length);

            }
            else
                mySpiffs.mqtt_broker = new byte[NUM_OF_BROKER];
        }

        private void textMqttPort_TextChanged(object sender, EventArgs e)
        {           
            button1.BackColor = Color.Gray;

            string newPort = textMqttPort.Text;

            byte[] tmp = Encoding.ASCII.GetBytes(newPort);
             
            if(tmp.Length != 0)
            {
                mySpiffs.mqtt_port = new byte[NUM_OF_PORT];
                Array.Copy(tmp, 0, mySpiffs.mqtt_port, 0, tmp.Length);
            }
            else
              mySpiffs.mqtt_port = new byte[NUM_OF_PORT];
        }

        private void textMqttPassword_TextChanged(object sender, EventArgs e)
        {
            button1.BackColor = Color.Gray;

            string newPass = textMqttPassword.Text;

            byte[] tmp = Encoding.ASCII.GetBytes(newPass);

            if(tmp.Length != 0)
            {
                mySpiffs.mqtt_pssw = new byte[NUM_OF_PSSW];
                Array.Copy(tmp, 0, mySpiffs.mqtt_pssw, 0, tmp.Length);
            }
            else
              mySpiffs.mqtt_pssw = new byte[NUM_OF_PSSW];
        }
        private void textMqttUser_TextChanged(object sender, EventArgs e)
        {
            button1.BackColor = Color.Gray;

            string newUsr = textMqttUser.Text;

            byte[] tmp = Encoding.ASCII.GetBytes(newUsr);

            if(tmp.Length != 0)
            {
                mySpiffs.mqtt_user = new byte[NUM_OF_USER];
                Array.Copy(tmp, 0, mySpiffs.mqtt_user, 0, tmp.Length);
            }
            else
              mySpiffs.mqtt_user = new byte[NUM_OF_USER];
        }

        private void textNtpAddress_TextChanged(object sender, EventArgs e)
        {  
            button1.BackColor = Color.Gray;

            string newNtpServer = textNtpAddress.Text;

            byte[] tmp = Encoding.ASCII.GetBytes(newNtpServer);

            if(tmp.Length != 0)
            {
                mySpiffs.ntp_server = new byte[NUM_OF_NTPSERVER];
                Array.Copy(tmp, 0, mySpiffs.ntp_server, 0, tmp.Length);
            }
            else
             mySpiffs.ntp_server = new byte[NUM_OF_NTPSERVER];
        }

        private void textNtpPort_TextChanged(object sender, EventArgs e)
        {         
            button1.BackColor = Color.Gray;

            string newNtpPort = textNtpPort.Text;

            byte[] tmp = Encoding.ASCII.GetBytes(newNtpPort);

            if (tmp.Length != 0)
            {
                mySpiffs.ntp_port = new byte[NUM_OF_NTPPORT];
                Array.Copy(tmp, 0, mySpiffs.ntp_port, 0, tmp.Length);
            }
            else
                mySpiffs.ntp_port = new byte[NUM_OF_NTPPORT];
        }

        //
        //  APN 
        //
        private void textApnName_TextChanged(object sender, EventArgs e)
        {
            button1.BackColor = Color.Gray;

            string newApnName = textApnName.Text;

            byte[] tmp = Encoding.ASCII.GetBytes(newApnName);

            if(tmp.Length != 0) 
            {
                mySpiffs.apn_name = new byte[NUM_OF_CHAR_IN_APN_FIELD];
                Array.Copy(tmp, 0, mySpiffs.apn_name, 0, tmp.Length);
            }
            else
              mySpiffs.apn_name = new byte[NUM_OF_CHAR_IN_APN_FIELD];
        }

        private void textApnUser_TextChanged(object sender, EventArgs e)
        {              
            button1.BackColor = Color.Gray;

            string newApnUser = textApnUser.Text;

            byte[] tmp = Encoding.ASCII.GetBytes(newApnUser);

            if(tmp.Length != 0)
            { 
                mySpiffs.apn_user = new byte[NUM_OF_CHAR_IN_APN_FIELD];
                Array.Copy(tmp, 0, mySpiffs.apn_user, 0, tmp.Length);
            }
            else
              mySpiffs.apn_user = new byte[NUM_OF_CHAR_IN_APN_FIELD];
        }

        private void textApnPassword_TextChanged(object sender, EventArgs e)
        {
            button1.BackColor = Color.Gray;

            string newApnPassword = textApnPassword.Text;

            byte[] tmp = Encoding.ASCII.GetBytes(newApnPassword);

            if(tmp.Length != 0)
            {
                mySpiffs.apn_password = new byte[NUM_OF_CHAR_IN_APN_FIELD];
                Array.Copy(tmp, 0, mySpiffs.apn_password, 0, tmp.Length);
            }
            else
                mySpiffs.apn_password = new byte[NUM_OF_CHAR_IN_APN_FIELD];
        }


        private void textVersion_TextChanged(object sender, EventArgs e)
        {
            button1.BackColor = Color.Gray;

            string newVersion = textVersion.Text;

            if (newVersion != "")
            {
                mySpiffs.cfg_version[0] = 0;

                try
                {
                    byte Majnumber = Convert.ToByte(newVersion);

                    if (Enumerable.Range(1, 256).Contains(Majnumber))
                    {
                        mySpiffs.cfg_version[0] = Majnumber;
                    }
                    else
                    {
                        MessageBox.Show("Valore errato (range valido da 1 - 256)");
                    }
                }
                catch
                {
                    MessageBox.Show("Valore errato (range valido da 1 - 256)");
                }
            }
            else
            {
                mySpiffs.cfg_version[0] = 0;
            }
        }

        private void textBetaVersion_TextChanged(object sender, EventArgs e)
        {
            button1.BackColor = Color.Gray;

            string newVersion = textBetaVersion.Text;
        
            if(newVersion != "")
            {
                mySpiffs.cfg_version[1] = 0;

                try
                {
                    byte Minnumber = Convert.ToByte(newVersion);

                    if (Enumerable.Range(0, 99).Contains(Minnumber))
                    {
                        mySpiffs.cfg_version[1] = Minnumber;
                    }
                    else
                    {
                        MessageBox.Show("Valore errato (range valido da 0 - 99)");
                    }
                }
                catch
                {
                    MessageBox.Show("Valore errato (range valido da 0 - 99)");
                }
            }
            else
            {
                mySpiffs.cfg_version[1] = 0;
            }
        }


        private void button2_Click(object sender, EventArgs e)
        {
            textMqttBroker.Clear();
            textMqttPort.Clear();
            textMqttPassword.Clear();
            textMqttUser.Clear();
            textNtpAddress.Clear();
            textNtpPort.Clear();
            textVersion.Clear();
            textBetaVersion.Clear();
            textApnName.Clear();
            textApnPassword.Clear();
            textApnUser.Clear();
        }


        private void encrypt(byte[] password, ushort key)
        {
            byte i = 0;
            byte len = 0;
            ushort res = 0;
   
            while (password[i] != 0)
            {
                len++;
                i++;
            }

            for (i = 0; i < len; i++)
            {
                res = (ushort)password[i];
                res -= (ushort)key;

                password[i] = (byte)res;
            }
        }

        private void dencrypt(byte[] password, ushort key)
        {
            byte i = 0;
            byte len = 0;
            ushort res = 0;

            while(password[i] != 0)
            {
                len++;
                i++;
            }

            for (i = 0; i < len; i++)
            {
                res = (ushort)password[i];
                res += (ushort)key;

                password[i] = (byte)res;
            }
        }


        private void GetCRC(byte[] message, byte[] CRC)
        {
            //Function expects a modbus message of any length as well as a 2 byte CRC array in which to 
            //return the CRC values:

            ushort CRCFull = 0xFFFF;
            byte CRCHigh = 0xFF, CRCLow = 0xFF;
            char CRCLSB;

            for (int i = 0; i < (message.Length); i++)
            {
                CRCFull = (ushort)(CRCFull ^ message[i]);

                for (int j = 0; j < 8; j++)
                {
                    CRCLSB = (char)(CRCFull & 0x0001);
                    CRCFull = (ushort)((CRCFull >> 1) & 0x7FFF);

                    if (CRCLSB == 1)
                        CRCFull = (ushort)(CRCFull ^ 0xA001);
                }
            }
            CRC[1] = CRCHigh = (byte)((CRCFull >> 8) & 0xFF);
            CRC[0] = CRCLow = (byte)(CRCFull & 0xFF);
        }


        public class Crc16
        {
            private static ushort[] CrcTable = {
        0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
        0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
        0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
        0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
        0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
        0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
        0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
        0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
        0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
        0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
        0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
        0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
        0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
        0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
        0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
        0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
        0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
        0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
        0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
        0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
        0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
        0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
        0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
        0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
        0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
        0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
        0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
        0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
        0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
        0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
        0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
        0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };

            public static UInt16 ComputeCrc(byte[] data)
            {
                ushort crc = 0xFFFF;

                foreach (byte datum in data)
                {
                    crc = (ushort)((crc >> 8) ^ CrcTable[(crc ^ datum) & 0xFF]);
                }

                return crc;
            }
        }
    }
}


