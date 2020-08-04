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

            comboBox_DUT_Type.Items.Add("WIFI              - GTW000MWT0 ");
            comboBox_DUT_Type.Items.Add("2G with CAREL SIM - GTW000MGP0 ");
            comboBox_DUT_Type.SelectedIndex = 0;

            My_IO.Init_IO_Interface();
            My_SER.Init_Ser_Interface();

            launch_modbus_simulator();

            if (My_IO.ComName == "")
            {
                MessageBox.Show("ERROR! I/O COM Port not found please check CAREL_GME_Test_CFG.ini");
                this.Close();
            }
            label_com_name.Text = My_IO.ComName;


                     
            if (My_SER.ComPrgName == "")
            {
                MessageBox.Show("ERROR! I/O COM PortPrg not found please check CAREL_GME_Test_CFG.ini");
                this.Close();
            }
            labelComPrg.Text = My_SER.ComPrgName;


            if (My_IO.ComPrgBaud == "")
            {
                MessageBox.Show("ERROR! I/O COM BaudPrg not found please check CAREL_GME_Test_CFG.ini");
                this.Close();
            }
            labelPrgBaud.Text = My_IO.ComPrgBaud;


            if (My_SER.ComModbusSimulator == "")
            {
                MessageBox.Show("ERROR! I/O COM PortMBSim not set please check CAREL_GME_Test_CFG.ini");
                this.Close();
            }
            label_MB_Sim_Port.Text = My_SER.ComModbusSimulator;
         
            Application.DoEvents();
           
            buttonTestStart.BackColor = Color.LightGray;
        }

        private void button1_Click_2(object sender, EventArgs e)
        {
            My_IO.set_rele_status(My_IO.RELE_POWER, My_IO.RELE_ON);
        }



        private int launch_modbus_simulator()
        {
            
            String cmdlinepars;
            cmdlinepars = My_SER.ComModbusSimulator;

            Process pProcess = new Process();

            pProcess.StartInfo.FileName = @"Modbus_PC_Simulator.bat";
            pProcess.StartInfo.Arguments = cmdlinepars;
            pProcess.StartInfo.UseShellExecute = false;
            pProcess.StartInfo.RedirectStandardOutput = false;
            pProcess.StartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Minimized;
            pProcess.StartInfo.CreateNoWindow = false;            //not diplay a windows
            pProcess.Start();
            
            return 1;
        }


        private int call_esp_programmer()
        {
            String cmdlinepars;
            cmdlinepars = My_SER.ComPrgName + " " + My_SER.ComPrgBaud;

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



        private void test_result_indicator(bool result)
        {
            if (result == true)
            {
                button_test_result.Text = "TEST PASSED";
                button_test_result.BackColor = Color.Green;
                My_IO.open_all_rele();
            }
            else 
            {
                button_test_result.Text = "TEST FAIL";
                button_test_result.BackColor = Color.Red;
                My_IO.open_all_rele();
            }
        
        
        }



        private void test_sequence()
        {
            /* ============================================================== */
            /*                     BEGIN OF TEST SUITE                        */
            /* ============================================================== */
            bool test_pass = true;

            display_reset();
            display_status("TEST Start WiFi!");


            /* close serial if left opened by the previous failed test */
            if (My_SER.ser_is_open == true) My_SER.DeInit_Ser_Communication_Interface();

            My_IO.open_all_rele();
            display_status("Power ON !");
            My_IO.set_rele_status(My_IO.RELE_BUTTON, My_IO.RELE_ON); //press prog. button
            Thread.Sleep(500);
            My_IO.set_rele_status(My_IO.RELE_POWER, My_IO.RELE_ON);
            Thread.Sleep(500);
            My_IO.set_rele_status(My_IO.RELE_EN, My_IO.RELE_ON); //press prog. button
            

            test_pass = MessageYesNo("LED POWER TEST", "Is the power led ON ?");
            if (test_pass == false)
            {                
                display_failed_test();
                test_result_indicator(false);
                return;
            }
                       

            //TEST if power supply voltage is within limits 
            int irv;
            irv = My_IO.get_voltage_level();

            if ((irv >= My_IO.volt_min) && (irv <= My_IO.volt_max))
            {
                display_status("Voltage = " + (((((float)irv)*My_IO.volt_a))+My_IO.volt_b));
                display_status("Test OK");
            }
            else 
            {
                display_failed_test();
                test_result_indicator(false);
                return;
            }

            //now start programming the device 
            My_IO.set_rele_status(My_IO.RELE_EN, My_IO.RELE_OFF);      //leave GME reset
            Thread.Sleep(1000);
            My_IO.set_rele_status(My_IO.RELE_BUTTON, My_IO.RELE_OFF);  //prog. button release
            Thread.Sleep(1000);
            display_status("Gateway ready to program ..");

                        
            test_pass = prog_device();

            if (test_pass == false) 
            {
                display_status("Gateway programming FAILED!");
                display_failed_test();
                test_result_indicator(false);
                return;
            }



            /*
             * now we initialize the serial port previosly used for the programming
             * to get some data from the GME in test mode
             */
            My_SER.Init_Ser_Communication_Interface();
            

            MessageBox.Show("Take a look to the status led to see if blink it take some time");


            //now we put GME in test mode            
            My_IO.set_rele_status(My_IO.RELE_EN, My_IO.RELE_ON);            //maintain GME resetted
            Thread.Sleep(500);
            My_IO.set_rele_status(My_IO.RELE_TP5, My_IO.RELE_ON);           //TP5 to ground 
            Thread.Sleep(500);
            My_IO.set_rele_status(My_IO.RELE_EN, My_IO.RELE_OFF);            //leave GME reset
            display_status("Gateway in test mode ..");

            //this time is to give the possibility to the GME to encrypt the flash 
            //it take 60 sec. maximun but we short the time waiting the IMEI serial message 
            //anyway no less than 10secs are requested so that the first part is in busy wait
            Thread.Sleep(10000);


            //get MAC address
            String mac_address_str;
            int value;

            mac_address_str = My_SER.get_mac_response();
            value = String.Compare(mac_address_str, @"");

            if (value != 0)
            {
                display_status(mac_address_str);
            }
            else
            {
                display_status("FAIL to get MAC address");
                display_failed_test();
                My_SER.DeInit_Ser_Communication_Interface();
                test_result_indicator(false);
                return;
            }


            test_pass = MessageYesNo("LED STATUS TEST", "The status led have blinked green/red ?");
            if (test_pass == false)
            {
                display_failed_test();
                test_result_indicator(false);
                return;
            }
                       
            /* the GME take a while to connect to the AP */
            Thread.Sleep(2000);

            bool res;
            //test if connected to the AP
            res = My_SER.get_ap_connection_response();
            if (res == true)
            {
                display_status("AP Connected TEST PASSED !");
            }
            else
            {
                display_status("FAIL to connect the SSID=TEST_AP PWD=12345678 ");
                display_failed_test();
                My_SER.DeInit_Ser_Communication_Interface();
                test_result_indicator(false);
                return;
            }



            //read HR n.1 
            res = My_SER.get_modbus_hr_response();
            if (res == true)
            {
                display_status("Read HR through RS485 TEST PASSED !");
            }
            else
            {
                display_status("FAIL to read HR through RS485 port");
                display_failed_test();
                My_SER.DeInit_Ser_Communication_Interface();
                test_result_indicator(false);
                return;
            }



            //close the serial port 
            My_SER.DeInit_Ser_Communication_Interface();


            /* ============================================================== */
            /*                BEGIN OF USR CUSTOMIZATION PART                 */
            /* ============================================================== */

            //TODO for USR 
            //get Serial number from factory or do all in an external program
            //that also print the label 


            /* ============================================================== */
            /*                 END OF USR CUSTOMIZATION                       */
            /* ============================================================== */

            /* ============================================================== */
            /*                         END OF TEST                            */
            /* ============================================================== */
            test_result_indicator(true);
            display_status("END OF TEST ");

        }



        private void test_sequence_2g()
        {
            /* ============================================================== */
            /*                     BEGIN OF TEST SUITE 2G                     */
            /* ============================================================== */
            bool test_pass = true;

            display_reset();
            display_status("TEST Start 2G!");


            /* close serial if left opened by the previous failed test */
            if (My_SER.ser_is_open == true) My_SER.DeInit_Ser_Communication_Interface();

            My_IO.open_all_rele();
            display_status("Power ON !");
            My_IO.set_rele_status(My_IO.RELE_BUTTON, My_IO.RELE_ON); //press prog. button
            Thread.Sleep(500);
            My_IO.set_rele_status(My_IO.RELE_POWER, My_IO.RELE_ON);
            Thread.Sleep(500);
            My_IO.set_rele_status(My_IO.RELE_EN, My_IO.RELE_ON); //press prog. button


            test_pass = MessageYesNo("LED POWER TEST", "Is the power led ON ?");
            if (test_pass == false)
            {
                display_failed_test();
                test_result_indicator(false);
                return;
            }



            //TEST if power supply voltage is within limits 
            int irv;
            irv = My_IO.get_voltage_level();
            

            if ((irv >= My_IO.volt_min_2g) && (irv <= My_IO.volt_max_2g))
            {
                display_status("Voltage = " + (((((float)irv) * My_IO.volt_a)) + My_IO.volt_b));
                display_status("Test OK");
            }
            else
            {
                display_failed_test();
                test_result_indicator(false);
                return;
            }

            //now start programming the device 
            My_IO.set_rele_status(My_IO.RELE_EN, My_IO.RELE_OFF);      //leave GME reset
            Thread.Sleep(1000);
            My_IO.set_rele_status(My_IO.RELE_BUTTON, My_IO.RELE_OFF);  //prog. button release
            Thread.Sleep(1000);
            display_status("Gateway ready to program ..");


            //test_pass = prog_device();

            test_pass = true;

            if (test_pass == false)
            {
                display_status("Gateway programming FAILED!");
                display_failed_test();
                test_result_indicator(false);
                return;
            }



            /*
             * now we initialize the serial port previosly used for the programming
             * to get some data from the GME in test mode
             */
            My_SER.Init_Ser_Communication_Interface();

            //warn the operator to take a look to the leds
            MessageBox.Show("Take a look to the green leds to see if they blink in sequence it take some time");
            

            //now we put GME in test mode            
            My_IO.set_rele_status(My_IO.RELE_EN, My_IO.RELE_ON);            //maintain GME resetted
            Thread.Sleep(500);
            My_IO.set_rele_status(My_IO.RELE_TP5, My_IO.RELE_ON);           //TP5 to ground 
            Thread.Sleep(500);
            My_IO.set_rele_status(My_IO.RELE_EN, My_IO.RELE_OFF);            //leave GME reset
            
            //this time is to give the possibility to the GME to encrypt the flash 
            //it take 60 sec. maximun but we short the time waiting the IMEI serial message 
            //anyway no less than 10secs are requested so that the first part is in busy wait
            Thread.Sleep(10000);
            
            display_status("Gateway in test mode ..");
                                                  
            //get IMEI address
            String imei_address_str;
            int value;

            imei_address_str = My_SER.get_imei_response();

            value = String.Compare(imei_address_str, @"");

            if (value != 0)
            {
                display_status(imei_address_str);
            }
            else
            {
                display_status("FAIL to get IMEI ");
                display_failed_test();
                My_SER.DeInit_Ser_Communication_Interface();
                test_result_indicator(false);
                return;
            }

            test_pass = MessageYesNo("LED TEST", "The status leds have blinked in sequence ?");
            if (test_pass == false)
            {
                display_failed_test();
                test_result_indicator(false);
                return;
            }
            

            /* check SIM presence */
            bool res;            
            res = My_SER.get_sim_installed_response();
            if (res == true)
            {
                display_status("SIM is installed TEST PASSED !");
            }
            else
            {
                display_status("FAIL SIM not found please check if is installed properly");
                display_failed_test();
                My_SER.DeInit_Ser_Communication_Interface();
                test_result_indicator(false);
                return;
            }


            /* the GME take a while to search the GSM networks */
            Thread.Sleep(3000);

            res = My_SER.get_operator_response();
            if (res == true)
            {
                display_status("GSM operator found TEST PASSED !");
            }
            else
            {
                display_status("FAIL GSM operator NOT found please check the antenna ");
                display_failed_test();
                My_SER.DeInit_Ser_Communication_Interface();
                test_result_indicator(false);
                return;
            }
                                          
                                 
            //read HR n.1 
            res = My_SER.get_modbus_hr_response();
            if (res == true)
            {
                display_status("Read HR through RS485 TEST PASSED !");
            }
            else
            {
                display_status("FAIL to read HR through RS485 port");
                display_failed_test();
                My_SER.DeInit_Ser_Communication_Interface();
                test_result_indicator(false);
                return;
            }



            //close the serial port 
            My_SER.DeInit_Ser_Communication_Interface();


            /* ============================================================== */
            /*                BEGIN OF USR CUSTOMIZATION PART                 */
            /* ============================================================== */

            //TODO for USR 
            //get Serial number from factory or do all in an external program
            //that also print the label 


            /* ============================================================== */
            /*                 END OF USR CUSTOMIZATION                       */
            /* ============================================================== */

            /* ============================================================== */
            /*                         END OF TEST                            */
            /* ============================================================== */
            test_result_indicator(true);
            display_status("END OF TEST ");

        }




        private void buttonTestStart_Click(object sender, EventArgs e)
        {
            int dut_selected;

            dut_selected = comboBox_DUT_Type.SelectedIndex;
            

            buttonTestStart.Enabled = false;
            buttonTestStart.BackColor = Color.Red;
            buttonTestStart.Text = "TEST RUNNING WAIT";

            button_test_result.Text = "--------";
            button_test_result.BackColor = Color.LightGray;


            Application.DoEvents();

            switch (dut_selected)
            {
                case 0:
                    //WiFi;
                    test_sequence();
                    break;

                case 1:
                    //2G with SIM
                    test_sequence_2g();
                    break;
            }
                                                   

            buttonTestStart.Text = "PRESS TO START THE TEST";
            buttonTestStart.Enabled = true;
            buttonTestStart.BackColor = Color.LightGray;
        }

        private void button_tp_en_Click(object sender, EventArgs e)
        {
            My_IO.set_rele_status(My_IO.RELE_EN, My_IO.RELE_ON);
        }

        private void button_tp_en_off_Click(object sender, EventArgs e)
        {
            My_IO.set_rele_status(My_IO.RELE_EN, My_IO.RELE_OFF);
        }

        private void button_tp5_Click(object sender, EventArgs e)
        {
            My_IO.set_rele_status(My_IO.RELE_TP5, My_IO.RELE_ON);
        }

        private void button_tp5_off_Click(object sender, EventArgs e)
        {
            My_IO.set_rele_status(My_IO.RELE_TP5, My_IO.RELE_OFF);
        }

        private void button_button_Click(object sender, EventArgs e)
        {
            My_IO.set_rele_status(My_IO.RELE_BUTTON, My_IO.RELE_ON);
        }

        private void button_button_off_Click(object sender, EventArgs e)
        {
            My_IO.set_rele_status(My_IO.RELE_BUTTON, My_IO.RELE_OFF);
        }

        private void button_all_off_Click(object sender, EventArgs e)
        {
            My_IO.open_all_rele();
        }

        private void button_put_in_programming_Click(object sender, EventArgs e)
        {
            My_IO.open_all_rele();
            My_IO.set_rele_status(My_IO.RELE_BUTTON, My_IO.RELE_ON);
            Thread.Sleep(500);
            My_IO.set_rele_status(My_IO.RELE_POWER, My_IO.RELE_ON);
            Thread.Sleep(500);
            My_IO.set_rele_status(My_IO.RELE_BUTTON, My_IO.RELE_OFF);
            Thread.Sleep(500);
        }

        private void button_read_v_Click(object sender, EventArgs e)
        {
            int irv;
            String value;

            irv = My_IO.get_voltage_level();

            float fv = (((((float)irv) * My_IO.volt_a)) + My_IO.volt_b);
            value = fv.ToString() + @"V (raw= " + irv.ToString() + @")";

            textBox_read_v.Text = value;
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }


}
