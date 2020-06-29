using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;
using IniFile;
using CodeProjectSerialComms;
using System.Threading;


namespace gme_ser_Interface
{
    class gme_ser_Interface_Class
    {
        internal delegate void SerialDataReceivedEventHandlerDelegate(object sender, SerialDataReceivedEventArgs e);
        internal delegate void SerialPinChangedEventHandlerDelegate(object sender, SerialPinChangedEventArgs e);
        private SerialPinChangedEventHandler SerialPinChangedEventHandler1;

        //delegate void SetTextCallback(string text);
        public int SerDataAvailable;
        public string InputData = String.Empty;

        public int SerSendDataAvailable;
        public string SendData = String.Empty;

        public String ComPrgName = "---";
        public String ComPrgBaud = "115200";

        public bool ser_is_open = false;
            
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


        private void port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            InputData = ComPort.ReadExisting();

            if (InputData != String.Empty)
            {
                SerDataAvailable = 1;               
            }
        }


        public void Init_Ser_Interface()
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


        public string Init_Ser_Communication_Interface()
        {
            String cfg_file;
            String par_val;

            par_val = "";

            cfg_file = @"CAREL_GME_Test_CFG.ini";
            var MyIni = new IniFileClass(cfg_file);
                        
            /* serial port part */                          
            try
            {
                ComPort.PortName = Convert.ToString(ComPrgName);
                ComPort.BaudRate = Convert.ToInt32(@"115200");
                ComPort.DataBits = Convert.ToInt16(@"8");
                ComPort.StopBits = (StopBits)Enum.Parse(typeof(StopBits), @"1");
                ComPort.Handshake = (Handshake)Enum.Parse(typeof(Handshake), @"None");
                ComPort.Parity = (Parity)Enum.Parse(typeof(Parity), @"None");
                               
                SerialPinChangedEventHandler1 = new SerialPinChangedEventHandler(PinChanged);
                ComPort.PinChanged += SerialPinChangedEventHandler1;
                
                ComPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(port_DataReceived);

                ComPort.Open();
                ComPort.RtsEnable = true;
                ComPort.DtrEnable = true;
                ser_is_open = true;
            }
            catch
            {
              
            }

            return par_val;
        }

        public void DeInit_Ser_Communication_Interface()
        {
            ComPort.Close();
            ser_is_open = false;
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

        public string get_mac_response()
        {
            String ret_val = @"";
            String result = @"";
            int irv=0;

            SerDataAvailable = 0;
            
            ret_val = serial_response(1000);

            if (ret_val != "")
            {
                string separator = @"MAC=";
                // Part 1: get index of separator.
                int separatorIndex = ret_val.IndexOf(separator);
                // Part 2: if separator exists, get substring.
                if (separatorIndex >= 0)
                {
                    result = ret_val.Substring(separatorIndex + separator.Length);                   
                }
            }

            return result;
        }
    }


}
