#define RUNNING_MODE

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO.Ports;
using System.Windows.Forms;
using System.Threading;

//CodeProjectSerialComms program 
//23/04/2013   16:29


namespace CodeProjectSerialComms
{



    public partial class Form1 : Form
    {
        SerialPort ComPort = new SerialPort();
        
        internal delegate void SerialDataReceivedEventHandlerDelegate(object sender, SerialDataReceivedEventArgs e);
        internal delegate void SerialPinChangedEventHandlerDelegate(object sender, SerialPinChangedEventArgs e);
        private SerialPinChangedEventHandler SerialPinChangedEventHandler1;
        delegate void SetTextCallback(string text);
        string InputData = String.Empty;

        public Form1()
        {
            InitializeComponent();
            SerialPinChangedEventHandler1 = new SerialPinChangedEventHandler(PinChanged);
            ComPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(port_DataReceived_1);


            comboBox_txslot.Items.Add("select n. of slots");
            comboBox_txslot.Items.Add("GPRS uplink 1 slot");
            comboBox_txslot.Items.Add("GPRS uplink 2 slots");
            comboBox_txslot.Items.Add("GPRS uplink 3 slots");
            comboBox_txslot.Items.Add("GPRS uplink 4 slots");
            comboBox_txslot.SelectedIndex = 3;

            comboBox_power_rollbk.Items.Add("Restore default value");
            comboBox_power_rollbk.Items.Add(" -1 dB");
            comboBox_power_rollbk.Items.Add(" -2 dB");
            comboBox_power_rollbk.Items.Add(" -3 dB");
            comboBox_power_rollbk.Items.Add(" -4 dB");
            comboBox_power_rollbk.Items.Add(" -5 dB");
            comboBox_power_rollbk.Items.Add(" -6 dB");
            comboBox_power_rollbk.Items.Add(" -7 dB");
            comboBox_power_rollbk.Items.Add(" -8 dB");
            comboBox_power_rollbk.Items.Add(" -9 dB");
            comboBox_power_rollbk.Items.Add("-10 dB");
            comboBox_power_rollbk.Items.Add("-11 dB");
            comboBox_power_rollbk.Items.Add("-12 dB");
            comboBox_power_rollbk.SelectedIndex = 0;


            comboBox_band.Items.Add("GSM900");
            comboBox_band.Items.Add("GSM1800");
            comboBox_band.Items.Add("GSM1900");
            comboBox_band.Items.Add("GSM850");
            comboBox_band.SelectedIndex = 0;

            comboBox_timeslot.Items.Add("0");
            comboBox_timeslot.Items.Add("1");
            comboBox_timeslot.Items.Add("2");
            comboBox_timeslot.Items.Add("3");
            comboBox_timeslot.Items.Add("4");
            comboBox_timeslot.Items.Add("5");
            comboBox_timeslot.Items.Add("6");
            comboBox_timeslot.Items.Add("7");
            comboBox_timeslot.SelectedIndex = 0;


            comboBox_burst_type.Items.Add("0 random 0 or 1 tx with TSC");
            comboBox_burst_type.Items.Add("1 all 1 without TSC");
            comboBox_burst_type.Items.Add("2 random 0 or 1 tx with synchronous sequence");
            comboBox_burst_type.Items.Add("3 continuous tx all zeroes");
            comboBox_burst_type.Items.Add("4 continuous tx all ones");
            comboBox_burst_type.Items.Add("5 continuous tx alternate bits");
            comboBox_burst_type.Items.Add("6 continuous tx pseudo random");
            comboBox_burst_type.SelectedIndex = 0;

            //Baud Rate
            cboBaudRate.Items.Add(300);
            cboBaudRate.Items.Add(600);
            cboBaudRate.Items.Add(1200);
            cboBaudRate.Items.Add(2400);
            cboBaudRate.Items.Add(9600);
            cboBaudRate.Items.Add(14400);
            cboBaudRate.Items.Add(19200);
            cboBaudRate.Items.Add(38400);
            cboBaudRate.Items.Add(57600);
            cboBaudRate.Items.Add(115200);
            cboBaudRate.Items.ToString();
            //get first item print in text
            cboBaudRate.Text = cboBaudRate.Items[0].ToString();
            //Data Bits
            cboDataBits.Items.Add(7);
            cboDataBits.Items.Add(8);
            //get the first item print it in the text 
            cboDataBits.Text = cboDataBits.Items[0].ToString();

            //Stop Bits
            cboStopBits.Items.Add("One");
            cboStopBits.Items.Add("OnePointFive");
            cboStopBits.Items.Add("Two");
            //get the first item print in the text
            cboStopBits.Text = cboStopBits.Items[0].ToString();
            //Parity 
            cboParity.Items.Add("None");
            cboParity.Items.Add("Even");
            cboParity.Items.Add("Mark");
            cboParity.Items.Add("Odd");
            cboParity.Items.Add("Space");
            //get the first item print in the text
            cboParity.Text = cboParity.Items[0].ToString();
            //Handshake
            cboHandShaking.Items.Add("None");
            cboHandShaking.Items.Add("XOnXOff");
            cboHandShaking.Items.Add("RequestToSend");
            cboHandShaking.Items.Add("RequestToSendXOnXOff");
            //get the first item print it in the text 
            cboHandShaking.Text = cboHandShaking.Items[0].ToString();
        }


        public void send_cmd(string tx_string)
        {
            rtbIncoming.ForeColor = Color.Green;
            rtbIncoming.Text = tx_string;
            rtbIncoming.ForeColor = Color.Black;

            #if RUNNING_MODE
            ComPort.Write(tx_string);
            #endif
        }


        private void btnGetSerialPorts_Click(object sender, EventArgs e)
        {
            string[] ArrayComPortsNames = null;
            int index = -1;
            string ComPortName = null;
           
//Com Ports
            ArrayComPortsNames = SerialPort.GetPortNames();
            do
            {
                index += 1;
                cboPorts.Items.Add(ArrayComPortsNames[index]);
               
              
            } while (!((ArrayComPortsNames[index] == ComPortName) || (index == ArrayComPortsNames.GetUpperBound(0))));
            Array.Sort(ArrayComPortsNames);
           
            if (index == ArrayComPortsNames.GetUpperBound(0))
            {
                ComPortName = ArrayComPortsNames[0];
            }
            //get first item print in text
            cboPorts.Text = ArrayComPortsNames[0];
                           
                       

 

        }

     


        private void port_DataReceived_1(object sender, SerialDataReceivedEventArgs e)
        {
            InputData = ComPort.ReadExisting();
            if (InputData != String.Empty)
            {
                this.BeginInvoke(new SetTextCallback(SetText), new object[] { InputData });
            }
        }
        private void SetText(string text)
        {
            this.rtbIncoming.Text += text;
        }
        internal void PinChanged(object sender, SerialPinChangedEventArgs e)
        {
            SerialPinChange SerialPinChange1 = 0;
            bool signalState = false;

            SerialPinChange1 = e.EventType;
            lblCTSStatus.BackColor = Color.Green;
            lblDSRStatus.BackColor = Color.Green;
            lblRIStatus.BackColor = Color.Green;
            lblBreakStatus.BackColor = Color.Green;
            switch (SerialPinChange1)
            {
                case SerialPinChange.Break:
                    lblBreakStatus.BackColor = Color.Red;
                    //MessageBox.Show("Break is Set");
                    break;
                case SerialPinChange.CDChanged:
                    signalState = ComPort.CtsHolding;
                  //  MessageBox.Show("CD = " + signalState.ToString());
                    break;
                case SerialPinChange.CtsChanged:
                    signalState = ComPort.CDHolding;
                    lblCTSStatus.BackColor = Color.Red;
                    //MessageBox.Show("CTS = " + signalState.ToString());
                    break;
                case SerialPinChange.DsrChanged:
                    signalState = ComPort.DsrHolding;
                    lblDSRStatus.BackColor = Color.Red;
                    // MessageBox.Show("DSR = " + signalState.ToString());
                    break;
                case SerialPinChange.Ring:
                    lblRIStatus.BackColor = Color.Red;
                    //MessageBox.Show("Ring Detected");
                    break;
            }
        }

        private void btnTest_Click(object sender, EventArgs e)
        {
            //SerialPinChangedEventHandler1 = new SerialPinChangedEventHandler(PinChanged);
            //ComPort.PinChanged += SerialPinChangedEventHandler1;
            //ComPort.Open();

            //ComPort.RtsEnable = true;
            //ComPort.DtrEnable = true;
            //btnTest.Enabled = false;

        }

        private void btnPortState_Click(object sender, EventArgs e)
        {
          
            if (btnPortState.Text == "Connect")
            {
                btnPortState.Text = "Disconnect";
                ComPort.PortName = Convert.ToString(cboPorts.Text);
                ComPort.BaudRate = Convert.ToInt32(@"115200");
                ComPort.DataBits = Convert.ToInt16(@"8");
                ComPort.StopBits = (StopBits)Enum.Parse(typeof(StopBits), @"1");
                ComPort.Handshake = (Handshake)Enum.Parse(typeof(Handshake), @"None");
                ComPort.Parity = (Parity)Enum.Parse(typeof(Parity), @"None");
                ComPort.Open();
            }
            else if (btnPortState.Text == "Disconnect")
            {
                btnPortState.Text = "Connect";
                ComPort.Close();               
            }
        }
        private void rtbOutgoing_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)13) // enter key  
            {
                ComPort.Write("\r\n");
                rtbOutgoing.Text = "";
            }
            else if (e.KeyChar < 32 || e.KeyChar > 126)
            {
                e.Handled = true; // ignores anything else outside printable ASCII range  
            }
            else
            {
                ComPort.Write(e.KeyChar.ToString());
            }
        }
        private void btnHello_Click(object sender, EventArgs e)
        {
            ComPort.Write("ATI\r");
        }

        private void btnHyperTerm_Click(object sender, EventArgs e)
        {
            string Command1 = txtCommand.Text;
            string CommandSent;
            int Length, j = 0;

            Length = Command1.Length;

            for (int i = 0; i < Length; i++)
            {
                CommandSent = Command1.Substring(j, 1);
                ComPort.Write(CommandSent);
                j++;
            }

        }

        private void button_ati_Click(object sender, EventArgs e)
        {            
            ComPort.Write("ATI\r");
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button_force_tx_Click(object sender, EventArgs e)
        {
            string tx_string;
            int idx;

#if RUNNING_MODE
            ComPort.Write("AT+CFUN=0\r");
            Thread.Sleep(1000);
#endif


            tx_string = "AT+QRFTEST=1,";

            idx = comboBox_band.SelectedIndex;
            tx_string += idx.ToString() + ",";


            //TODO ARCFN check
            //GSM900: 0~124, 975~1023
            //GSM1800: 512~885
            //GSM1900: 512~810
            //GSM850: 128~251            
            tx_string += textBox_arfcn.Text + ",";

            idx = comboBox_timeslot.SelectedIndex;
            tx_string += idx.ToString() + ",";
            
            tx_string += textBox_pcl.Text + ",";

            idx = Int16.Parse(textBox_afc.Text);
            tx_string += idx.ToString() + ",";

            idx = comboBox_burst_type.SelectedIndex;
            tx_string += idx.ToString() + "\r"; 

            rtbIncoming.ForeColor = Color.Green;
            rtbIncoming.Text = tx_string;
            rtbIncoming.ForeColor = Color.Black;


            #if RUNNING_MODE
            ComPort.Write(tx_string);
            Thread.Sleep(1000);
            #endif

            //button_force_tx_stop.Enabled = true;
            button_force_tx.Enabled = false;
        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void comboBox_band_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void textBox_afc_TextChanged(object sender, EventArgs e)
        {

        }

        private void comboBox_burst_type_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void comboBox_pcl_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void comboBox_timeslot_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void comboBox_arfcn_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void label8_Click(object sender, EventArgs e)
        {

        }

        private void button_force_tx_stop_Click(object sender, EventArgs e)
        {
            MessageBox.Show("You need to [Power OFF]\r\nthe device to STOP the test mode");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            #if RUNNING_MODE
            ComPort.Write("AT+CFUN=0\r");
            #endif
        }

        private void button_dummy_Click(object sender, EventArgs e)
        {
            String tx_string;

            #if RUNNING_MODE
            tx_string = "AT+CFUN=0\r";
            rtbIncoming.ForeColor = Color.Green;
            rtbIncoming.Text = tx_string;
            rtbIncoming.ForeColor = Color.Black;
            ComPort.Write(tx_string);
            Thread.Sleep(1000);

            tx_string = "AT+QRFTEST=1,0,9,0,8,4100,0\r";                         
            rtbIncoming.ForeColor = Color.Green;
            rtbIncoming.Text = tx_string;
            rtbIncoming.ForeColor = Color.Black;
            ComPort.Write(tx_string);            
            #endif
        }

        private void button_Clear_Click(object sender, EventArgs e)
        {
            rtbIncoming.Text = "";
        }

        private void button_setpower_Click(object sender, EventArgs e)
        {
            String tx_string;
            String band = "900";

            #if RUNNING_MODE
            tx_string = "AT+CDETXPW=";

            switch (comboBox_band.SelectedIndex)
            {
                case 0: band = "900"; break;
                case 1: band = "1800"; break;
                case 2: band = "1900"; break;
                case 3: band = "850"; break;
            }

            tx_string += band + ",";

            tx_string += comboBox_txslot.SelectedIndex.ToString() + ",";

            tx_string += textBox_pcl.Text + ",";

            tx_string += comboBox_power_rollbk.SelectedIndex.ToString() + "\r";


            rtbIncoming.ForeColor = Color.Green;
            rtbIncoming.Text = tx_string;
            rtbIncoming.ForeColor = Color.Black;
            ComPort.Write(tx_string);
            #endif
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            String tx_string;
            tx_string = textBox_send_at.Text + "\r";
            send_cmd(tx_string);
        }

        private void button_pwr_off_Click(object sender, EventArgs e)
        {
            send_cmd("AT+QPOWD=1\r");

            MessageBox.Show("You need to press the button\r\nto restart the device");
            button_force_tx.Enabled = true;
        }

        private void button_help_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start(@"CAREL_M95_Test.pdf");
        }
    }
}
