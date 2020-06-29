#define RUNNING_MODE

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO;
using System.IO.Ports;
using System.Windows.Forms;
using System.Threading;
using IO_Interface;
using gme_ser_Interface;
using System.Timers;
using System.Diagnostics;


namespace CodeProjectSerialComms
{

    public partial class Form1 : Form
    {              
        delegate void SetTextCallback(string text);
        string InputData = String.Empty;
        IO_Interface_Class My_IO = new IO_Interface_Class();
        System.Timers.Timer mytimer;


        gme_ser_Interface_Class My_SER = new gme_ser_Interface_Class();
               

        public Form1()
        {
            InitializeComponent();                      
        }


        void timer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            try
            {
                mytimer.Stop();
            }
            catch (ArgumentException aae)
            { 
            
            }

           

            /* ============================= */
            /*       cyclic activities       */
            /* ============================= */
            /* TX */
            if (My_IO.SerSendDataAvailable == 1)
            {             
                textBox_debug.Invoke(new Action(() => textBox_debug.AppendText(My_IO.SendData + Environment.NewLine)));
                My_IO.SerSendDataAvailable = 0;
            }

            /* RX */
            if (My_IO.SerDataAvailable == 1)
            {
                textBox_debug.Invoke(new Action(() => textBox_debug.AppendText(My_IO.InputData + Environment.NewLine)));
                My_IO.SerDataAvailable = 0;
            }

            /* ============================= */
            schedule_Timer();
        }


        void schedule_Timer()
        {
            //double tickTime = 1000;
            //mytimer = new System.Timers.Timer(tickTime);
            //mytimer.Elapsed += new System.Timers.ElapsedEventHandler(timer_Elapsed);
            //mytimer.Start();
        }


        public void send_cmd(string tx_string)
        {
            rtbIncoming.ForeColor = Color.Green;
            rtbIncoming.Text = tx_string;
            rtbIncoming.ForeColor = Color.Black;

            #if RUNNING_MODE
            //ComPort.Write(tx_string);
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
                //ComPortName = ArrayComPortsNames[0];
            }
            //get first item print in text
            cboPorts.Text = ArrayComPortsNames[0];
                           
                       

        }

    
        private void SetText(string text)
        {
            this.rtbIncoming.Text += text;
        }


        private void btnTest_Click(object sender, EventArgs e)
        {
        }

        private void btnPortState_Click(object sender, EventArgs e)
        {
          
            if (btnPortState.Text == "Connect")
            {
                btnPortState.Text = "Disconnect";
            }
            else if (btnPortState.Text == "Disconnect")
            {
                btnPortState.Text = "Connect";
                //ComPort.Close();               
            }
        }
        private void rtbOutgoing_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)13) // enter key  
            {
                //ComPort.Write("\r\n");
                //rtbOutgoing.Text = "";
            }
            else if (e.KeyChar < 32 || e.KeyChar > 126)
            {
                e.Handled = true; // ignores anything else outside printable ASCII range  
            }
            else
            {
                //ComPort.Write(e.KeyChar.ToString());
            }
        }
        private void btnHello_Click(object sender, EventArgs e)
        {
            //ComPort.Write("ATI\r");
        }

        private void btnHyperTerm_Click(object sender, EventArgs e)
        {
            string Command1 = @"";
            string CommandSent;
            int Length, j = 0;

            Length = Command1.Length;

            for (int i = 0; i < Length; i++)
            {
                CommandSent = Command1.Substring(j, 1);
                //ComPort.Write(CommandSent);
                j++;
            }

        }

        private void button_ati_Click(object sender, EventArgs e)
        {            
            //ComPort.Write("ATI\r");
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button_force_tx_Click(object sender, EventArgs e)
        {
            string tx_string;
            int idx;


            //idx = comboBox_band.SelectedIndex;
            //tx_string += idx.ToString() + ",";


            //TODO ARCFN check
            //GSM900: 0~124, 975~1023
            //GSM1800: 512~885
            //GSM1900: 512~810
            //GSM850: 128~251            
            //tx_string += textBox_arfcn.Text + ",";

            //idx = comboBox_timeslot.SelectedIndex;
            //tx_string += idx.ToString() + ",";
            
            //tx_string += textBox_pcl.Text + ",";

            //idx = Int16.Parse(textBox_afc.Text);
            //tx_string += idx.ToString() + ",";

            //idx = comboBox_burst_type.SelectedIndex;
            //tx_string += idx.ToString() + "\r"; 

            rtbIncoming.ForeColor = Color.Green;
            //rtbIncoming.Text = tx_string;
            rtbIncoming.ForeColor = Color.Black;
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
            //ComPort.Write("AT+CFUN=0\r");
            #endif
        }

        private void button_dummy_Click(object sender, EventArgs e)
        {
            String tx_string;

            #if RUNNING_MODE
            
            rtbIncoming.ForeColor = Color.Green;
            //rtbIncoming.Text = tx_string;
            rtbIncoming.ForeColor = Color.Black;
            //ComPort.Write(tx_string);
            Thread.Sleep(1000);

                       
            rtbIncoming.ForeColor = Color.Green;
            //rtbIncoming.Text = tx_string;
            rtbIncoming.ForeColor = Color.Black;
            //ComPort.Write(tx_string);            
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

            //switch (comboBox_band.SelectedIndex)
            //{
            //    case 0: band = "900"; break;
            //    case 1: band = "1800"; break;
            //    case 2: band = "1900"; break;
            //    case 3: band = "850"; break;
            //}

            tx_string += band + ",";

            //tx_string += comboBox_txslot.SelectedIndex.ToString() + ",";
            //tx_string += textBox_pcl.Text + ",";
            //tx_string += comboBox_power_rollbk.SelectedIndex.ToString() + "\r";


            rtbIncoming.ForeColor = Color.Green;
            rtbIncoming.Text = tx_string;
            rtbIncoming.ForeColor = Color.Black;
            //ComPort.Write(tx_string);
            #endif
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            String tx_string;
            //tx_string = textBox_send_at.Text + "\r";
            //send_cmd(tx_string);
        }

        private void button_pwr_off_Click(object sender, EventArgs e)
        {
            My_IO.set_rele_status(My_IO.RELE_POWER, My_IO.RELE_OFF);

        }

        private void button_help_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start(@"CAREL_GME_HW_TEST_HELP.pdf");
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            My_IO.Init_IO_Interface();

            if (My_IO.ComName == "")
            {
                MessageBox.Show("ERROR! I/O COM Port not found please check CAREL_GME_Test_CFG.ini");
                this.Close();
            }
            label_com_name.Text = My_IO.ComName;


            My_SER.Init_Ser_Interface();           
            if (My_IO.ComPrgName == "")
            {
                MessageBox.Show("ERROR! I/O COM PortPrg not found please check CAREL_GME_Test_CFG.ini");
                this.Close();
            }
            labelComPrg.Text = My_IO.ComPrgName;


            if (My_IO.ComPrgBaud == "")
            {
                MessageBox.Show("ERROR! I/O COM BaudPrg not found please check CAREL_GME_Test_CFG.ini");
                this.Close();
            }
            labelPrgBaud.Text = My_IO.ComPrgBaud;


            //TODO eliminare
            schedule_Timer();


            buttonTestStart.BackColor = Color.Green;
        }

        private void button1_Click_2(object sender, EventArgs e)
        {
            My_IO.set_rele_status(My_IO.RELE_POWER, My_IO.RELE_ON);
        }



        private int call_esp_programmer()
        {
            String cmdlinepars;
            cmdlinepars = My_IO.ComPrgName + " " + My_IO.ComPrgBaud;
            
            /* delete previous prgout.txt */
            try
            {
                File.Delete(@"prgout.txt");
            }
            catch (FileNotFoundException e)
            { 
               //do nothing is normal at 1st run 
            }
                                 

            Process pProcess = new Process();

            pProcess.StartInfo.FileName = @"GME_ESP32_Upload.bat";
            pProcess.StartInfo.Arguments = cmdlinepars; 
            pProcess.StartInfo.UseShellExecute = false;
            pProcess.StartInfo.RedirectStandardOutput = true;
            pProcess.StartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Normal;
            pProcess.StartInfo.CreateNoWindow = true;            //not diplay a windows
            pProcess.Start();
            string output = pProcess.StandardOutput.ReadToEnd(); //The output result
            pProcess.WaitForExit();
            textBox_debug.Text = textBox_debug.Text + output;

            /* prgout.txt parse to see errors */
            string[] lines = System.IO.File.ReadAllLines(@"prgout.txt");

            bool b_err_1;
            string s_err1 = "ERROR";

            bool b_err_2;
            string s_err2 = "FAIL";

            string tmp_line;

            foreach (string line in lines)
            {
                // Use a tab to indent each line of the file.
                textBox_debug.Text = textBox_debug.Text + line;
                tmp_line = line.ToUpper();

                b_err_1 = tmp_line.Contains(s_err1);
                b_err_2 = tmp_line.Contains(s_err2);

                if (b_err_1 || b_err_2) 
                {                    
                    return 0;
                }
            }
            
            return 1;
        }

        private void button2_Click(object sender, EventArgs e)
        {


        }




        private void textBox_debug_TextChanged(object sender, EventArgs e)
        {

        }

        private void labelComPrg_Click(object sender, EventArgs e)
        {

        }


        void display_reset() 
        {
            rtbIncoming.Text = "";
        }

        void display_status(String addline)
        {
            rtbIncoming.Text = rtbIncoming.Text + addline + "\r\n";
            Application.DoEvents();
        }

        void display_failed_test()
        {            
                display_status("Test FAILED!");
                display_status("END OF TEST");           
        }
                     
        private bool prog_device()
        {
            int retval;
            display_status("Start programming take a lot be patient");         
            retval = call_esp_programmer();
            Application.DoEvents();

            if (retval == 1)
            {
                return true;
            }
            else
            {
                return false;
            }

        }
                
        private bool MessageYesNo(String caption, String msg)
        {
                     
            var result = MessageBox.Show(msg, caption,
                                         MessageBoxButtons.YesNo,
                                         MessageBoxIcon.Question);

            // If the no button was pressed ...
            if (result == DialogResult.No)
            {
                // cancel the closure of the form.
                return false;
            }
            else 
            {
                return true;
            }

        }



        private void test_sequence()
        {
            /* ============================================================== */
            /*                     BEGIN OF TEST SUITE                        */
            /* ============================================================== */
            bool test_pass = true;

            display_reset();
            display_status("TEST Start!");
            /* close serial if left opened by the previous failed test */
            if (My_SER.ser_is_open == true) My_SER.DeInit_Ser_Communication_Interface();

            My_IO.open_all_rele();
            display_status("Power ON !");
            My_IO.set_rele_status(My_IO.RELE_EN, My_IO.RELE_ON);     //maintain GME resetted
            My_IO.set_rele_status(My_IO.RELE_BUTTON, My_IO.RELE_ON); //press prog. button
            My_IO.set_rele_status(My_IO.RELE_POWER, My_IO.RELE_ON);

            test_pass = MessageYesNo("LED POWER TEST", "Is the power led ON ?");
            if (test_pass == false)
            {                
                display_failed_test();
                return;
            }
                       

            //TEST if power supply voltage is within limits 
            int irv;
            irv = My_IO.get_voltage_level();

            if ((irv >= My_IO.volt_min) && (irv <= My_IO.volt_max))
            {
                display_status("Voltage = " + (((irv*My_IO.volt_a))+My_IO.volt_b));
                display_status("Test OK");
            }
            else 
            {
                display_failed_test();
                return;
            }

            //now start programming the device 
            My_IO.set_rele_status(My_IO.RELE_EN, My_IO.RELE_OFF);      //leave GME reset
            Thread.Sleep(100);
            My_IO.set_rele_status(My_IO.RELE_BUTTON, My_IO.RELE_OFF);  //prog. button release
            Thread.Sleep(100);
            display_status("Gateway ready to program ..");

            //TODO DECOMMENTARE
            
            //test_pass = prog_device();

            if (test_pass == false) 
            {
                display_status("Gateway programming FAILED!");
                display_failed_test();
                return;
            }


            /*
             * now we initialize the serial port previosly used for the programming
             * to get some data from the GME in test mode
             */
            My_SER.Init_Ser_Communication_Interface();
                                   

            MessageBox.Show("Take a look to the status led to see if blink");

            //now we put GME in test mode            
            My_IO.set_rele_status(My_IO.RELE_EN, My_IO.RELE_ON);            //maintain GME resetted
            Thread.Sleep(100);
            My_IO.set_rele_status(My_IO.RELE_TP5, My_IO.RELE_ON);           //TP5 to gorund 
            Thread.Sleep(100);
            My_IO.set_rele_status(My_IO.RELE_EN, My_IO.RELE_ON);            //leave GME reset
            display_status("Gateway in test mode ..");

            test_pass = MessageYesNo("LED STATUS TEST", "The status led have blinked green/red ?");
            if (test_pass == false)
            {
                display_failed_test();
                return;
            }
            
            
            //get MAC address
            String mac_address_str;
            mac_address_str = My_SER.get_mac_response();

            if (mac_address_str == "")
            {
                display_status("FAIL to get MAC address");
                display_failed_test();
                My_SER.DeInit_Ser_Communication_Interface();
                return;
            }
            
            //TODO test if connected to the AP





            //close the serial port 
            My_SER.DeInit_Ser_Communication_Interface();

            /* ============================================================== */
            /*                BEGIN OF USR CUSTOMIZATION PART                 */
            /* ============================================================== */


            //get Serial number from factory 


            //print label 



            /* ============================================================== */
            /*                 END OF USR CUSTOMIZATION                       */
            /* ============================================================== */

            /* ============================================================== */
            /*                         END OF TEST                            */
            /* ============================================================== */
            display_status("TEST PASSED - END OF TEST ");


        }




        private void buttonTestStart_Click(object sender, EventArgs e)
        {
            buttonTestStart.Enabled = false;
            buttonTestStart.BackColor = Color.Red;
            Application.DoEvents();

            test_sequence();
            
            buttonTestStart.Enabled = true;
            buttonTestStart.BackColor = Color.Green;
        }
    }

}
