using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;
using IniFile;
using CodeProjectSerialComms;
using System.Threading;


namespace IO_Interface
{
    class IO_Interface_Class
    {
        internal delegate void SerialDataReceivedEventHandlerDelegate(object sender, SerialDataReceivedEventArgs e);
        internal delegate void SerialPinChangedEventHandlerDelegate(object sender, SerialPinChangedEventArgs e);
        private SerialPinChangedEventHandler SerialPinChangedEventHandler1;

        //delegate void SetTextCallback(string text);
        public int SerDataAvailable;
        public string InputData = String.Empty;

        public int SerSendDataAvailable;
        public string SendData = String.Empty;

        public string RELE_OFF = @"0";
        public string RELE_ON  = @"1";

        public string RELE_POWER = @"A";
        public string RELE_BUTTON = @"B";
        public string RELE_TP5 = @"C";
        public string RELE_EN = @"D";
        public string ANALOG_IN = @"V";

        public String ComName = "---";
        public String ComPrgName = "---";
        public String ComPrgBaud = "115200";

        public int volt_min;
        public int volt_max;
        public int volt_min_2g;
        public int volt_max_2g;

        public float volt_a;
        public float volt_b;

               
        SerialPort ComPort = new SerialPort();
                
        internal void PinChanged(object sender, SerialPinChangedEventArgs e)
        {
            SerialPinChange SerialPinChange1 = 0;
            bool signalState = false;

            SerialPinChange1 = e.EventType;

            switch (SerialPinChange1)
            {
                case SerialPinChange.Break:                   
                    //MessageBox.Show("Break is Set");
                    break;
                case SerialPinChange.CDChanged:                    
                    //  MessageBox.Show("CD = " + signalState.ToString());
                    break;
                case SerialPinChange.CtsChanged:
                    //signalState = ComPort.CDHolding;                   
                    //MessageBox.Show("CTS = " + signalState.ToString());
                    break;
                case SerialPinChange.DsrChanged:
                    //signalState = ComPort.DsrHolding;
                    // MessageBox.Show("DSR = " + signalState.ToString());
                    break;
                case SerialPinChange.Ring:
                    //MessageBox.Show("Ring Detected");
                    break;
            }
        }


        private void port_DataReceived_1(object sender, SerialDataReceivedEventArgs e)
        {
            InputData = ComPort.ReadExisting();

            if (InputData != String.Empty)
            {
                SerDataAvailable = 1;
                //this.BeginInvoke(new SetTextCallback(SetText), new object[] { InputData });
            }
        }


        private void Init_Prg_Interface()
        {
            String cfg_file;
            String par_val;

            cfg_file = @"CAREL_GME_Test_CFG.ini";
            var MyIni = new IniFileClass(cfg_file);

            par_val = MyIni.Read("PortPrg");
            ComPrgName = par_val;

            par_val = MyIni.Read("BaudPrg");
            ComPrgBaud = par_val;
        }

        public string Init_IO_Interface()
        {
            String cfg_file;
            String par_val;

            cfg_file = @"CAREL_GME_Test_CFG.ini";
            var MyIni = new IniFileClass(cfg_file);

            /* read some costants */
            par_val = MyIni.Read("VOLT_MIN");
            volt_min = Int32.Parse(par_val);

            par_val = MyIni.Read("VOLT_MAX_2G");
            volt_max_2g = Int32.Parse(par_val);

            par_val = MyIni.Read("VOLT_MIN_2G");
            volt_min_2g = Int32.Parse(par_val);

            par_val = MyIni.Read("VOLT_MAX");
            volt_max = Int32.Parse(par_val);

            par_val = MyIni.Read("VOLT_A");
            volt_a = float.Parse(par_val);

            par_val = MyIni.Read("VOLT_B");
            volt_b = float.Parse(par_val);

                        
            /* serial port part */
            par_val = MyIni.Read("Port");            
            ComName = par_val;
          
            try
            {
                ComPort.PortName = Convert.ToString(par_val);
                ComPort.BaudRate = Convert.ToInt32(@"115200");
                ComPort.DataBits = Convert.ToInt16(@"8");
                ComPort.StopBits = (StopBits)Enum.Parse(typeof(StopBits), @"1");
                ComPort.Handshake = (Handshake)Enum.Parse(typeof(Handshake), @"None");
                ComPort.Parity = (Parity)Enum.Parse(typeof(Parity), @"None");
                               
                SerialPinChangedEventHandler1 = new SerialPinChangedEventHandler(PinChanged);
                ComPort.PinChanged += SerialPinChangedEventHandler1;
                
                ComPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(port_DataReceived_1);

                ComPort.Open();
                ComPort.RtsEnable = true;
                ComPort.DtrEnable = true;
            }
            catch
            {
                par_val = "";
            }

            return par_val;
        }

        public void DeInit_IO_Interface()
        {
            ComPort.Close();
        }
        
        public void open_all_rele() 
        {
            set_rele_status(RELE_POWER, RELE_OFF);
            set_rele_status(RELE_BUTTON, RELE_OFF);
            set_rele_status(RELE_TP5, RELE_OFF);
            set_rele_status(RELE_EN, RELE_OFF);
        }
        
        public void set_rele_status(string rele, string status)
        {
            string serial_cmd;
            serial_cmd = rele + status + '\n';

            SendData = @">" + serial_cmd;
            SerSendDataAvailable = 1;
            Thread.Sleep(200);
            ComPort.Write(serial_cmd);                     
        }
        
        public string serial_response(int timeout)
        {
            String resp = @"";
            int maxtime;

            maxtime = 0;

            do
            {
                Thread.Sleep(50);
                maxtime += 50;
                if (SerDataAvailable == 1)
                {
                    resp = InputData;
                    break;
                }

            } while (maxtime < timeout); 

            return resp;
        }


        /* ======================================================================= */
        /*                 BEGIN OF IMPLEMENTATION SPECIFIC ROUTINE                */
        /* ======================================================================= */

        public void send_read_voltage()
        {
           string serial_cmd;
           serial_cmd = ANALOG_IN + '\n';

           SendData = @">" + serial_cmd;
           SerSendDataAvailable = 1;
           Thread.Sleep(200);
           ComPort.Write(serial_cmd);
        }


        public int get_voltage_level()
        {
            String ret_val = @"";
            String result = @"";
            int irv=0;

            SerDataAvailable = 0;
            send_read_voltage();
            ret_val = serial_response(2000);

            if (ret_val != "")
            {
                string separator = @"V=";
                // Part 1: get index of separator.
                int separatorIndex = ret_val.IndexOf(separator);
                // Part 2: if separator exists, get substring.
                if (separatorIndex >= 0)
                {
                    result = ret_val.Substring(separatorIndex + separator.Length);
                    irv = Int32.Parse(result);
                }
            }

            return irv;
        }
    }


}
