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


namespace CustumCfgGenerator
{
    public partial class Form1 : Form
    {
        public const int NUM_OF_CFG_VERSION = 2;
        public const int NUM_OF_BROKER      = 60;
        public const int NUM_OF_PORT        = 30;
        public const int NUM_OF_PSSW        = 34;
        public const int NUM_OF_USER        = 34;
        public const int NUM_OF_NTPSERVER   = 30;
        public const int NUM_OF_NTPPORT     = 6;

        byte[] cfg_version = new byte[NUM_OF_CFG_VERSION];
        byte[] mqtt_broker = null;
        byte[] mqtt_port = null;
        byte[] mqtt_pssw = null;
        byte[] mqtt_user = null;
        byte[] ntp_server = null;
        byte[] ntp_port = null;
        
  

        public Form1()
        {
            InitializeComponent();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            string cfg_data     = null;
            string cfg_data_usr = null; 


            // genera file .bin per esp32
            // controllo il contenuto delle textbox
            try
            {   
                if (mqtt_broker.Length != null && mqtt_port.Length != null && mqtt_pssw.Length != null && mqtt_user.Length != null && ntp_server.Length != null && ntp_port.Length != null)
                {
                    string exeFolder = System.IO.Path.GetDirectoryName(Application.ExecutablePath);

                    // "C:\\Users\\alessandro.chiebao\\Desktop\\CHIEBAO_PROGETTI\\GATEWAY_MIDDLE_END\\Software\\CfgCreator\\

                    cfg_data = exeFolder + "\\cfg_data.bin";
                    File.Delete(cfg_data);
                    cfg_data_usr = exeFolder + "\\cfg_data_usr.bin";
                    File.Delete(cfg_data_usr);

                    using (FileStream stream = new FileStream(cfg_data, FileMode.Create))
                    {
                        using (BinaryWriter writer = new BinaryWriter(stream))
                        {
                            writer.Write(cfg_version);
                            writer.Write(mqtt_broker);
                            writer.Write(mqtt_port);

                            writer.Write(mqtt_pssw);
                            writer.Write(mqtt_user);

                            writer.Write(ntp_server);
                            writer.Write(ntp_port);
                            writer.Close();


                            var source = @cfg_data;
                            var destination = @cfg_data_usr;
                            try
                            {
                                File.Copy(source, destination);
                            }
                            catch
                            {
                                MessageBox.Show("Rimuovere i file precedenti o rinominarli");
                            }

                            button1.BackColor = Color.Green;

                            mqtt_broker = null;
                            mqtt_port = null;
                            mqtt_pssw = null;
                            mqtt_user = null;
                            ntp_server = null;
                            ntp_port = null;                        
                        }
                    }
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

            mqtt_broker = new byte[NUM_OF_BROKER];

            string newBroker = textMqttBroker.Text;

            byte[] tmp = Encoding.ASCII.GetBytes(newBroker);

            //int startAt = mqtt_broker.Length - tmp.Length;
            Array.Copy(tmp, 0, mqtt_broker, 0, tmp.Length);
        }

        private void textMqttPort_TextChanged(object sender, EventArgs e)
        {
            mqtt_port = new byte[NUM_OF_PORT];
            
            button1.BackColor = Color.Gray;

            string newPort = textMqttPort.Text;

            byte[] tmp = Encoding.ASCII.GetBytes(newPort);
             
            Array.Copy(tmp, 0, mqtt_port, 0, tmp.Length);
        }

        private void textMqttPassword_TextChanged(object sender, EventArgs e)
        {
            mqtt_pssw = new byte[NUM_OF_PSSW];

            button1.BackColor = Color.Gray;

            string newPass = textMqttPassword.Text;

            byte[] tmp = Encoding.ASCII.GetBytes(newPass);

            Array.Copy(tmp, 0, mqtt_pssw, 0, tmp.Length);  
        }
        private void textMqttUser_TextChanged(object sender, EventArgs e)
        {
            mqtt_user = new byte[NUM_OF_USER];

            button1.BackColor = Color.Gray;

            string newUsr = textMqttUser.Text;

            byte[] tmp = Encoding.ASCII.GetBytes(newUsr);

            Array.Copy(tmp, 0, mqtt_user, 0, tmp.Length);
        }

        private void textNtpAddress_TextChanged(object sender, EventArgs e)
        {
            ntp_server = new byte[NUM_OF_NTPSERVER];
            
            button1.BackColor = Color.Gray;

            string newNtpServer = textNtpAddress.Text;

            byte[] tmp = Encoding.ASCII.GetBytes(newNtpServer);

            Array.Copy(tmp, 0, ntp_server, 0, tmp.Length);
        }

        private void textNtpPort_TextChanged(object sender, EventArgs e)
        {
            ntp_port = new byte[NUM_OF_NTPPORT];

            button1.BackColor = Color.Gray;

            string newNtpPort = textNtpPort.Text;

            byte[] tmp = Encoding.ASCII.GetBytes(newNtpPort);

            Array.Copy(tmp, 0, ntp_port, 0, tmp.Length);
        }

        private void textVersion_TextChanged(object sender, EventArgs e)
        {
            button1.BackColor = Color.Gray;

            string newVersion = textVersion.Text;

            try
            {
                byte Majnumber = Convert.ToByte(newVersion);

                if (Enumerable.Range(1, 256).Contains(Majnumber))
                {
                    cfg_version[0] = Majnumber;
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

        private void textBetaVersion_TextChanged(object sender, EventArgs e)
        {
            button1.BackColor = Color.Gray;

            string newVersion = textBetaVersion.Text;
        
            try
            {
                byte Minnumber = Convert.ToByte(newVersion);

                if (Enumerable.Range(0, 99).Contains(Minnumber))
                {
                   cfg_version[1] = Minnumber;
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
    }
}
