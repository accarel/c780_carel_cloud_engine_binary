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
        private SerialPinChangedEventHandler SerialPinChangedEventHandler_ttl;

        //delegate void SetTextCallback(string text);
        public int SerDataAvailable;
        public string InputData = String.Empty;
        public string CumulatedData = String.Empty;

        public int SerSendDataAvailable;
        public string SendData = String.Empty;

        public String ComPrgName = "---";
        public String ComPrgBaud = "115200";

        public String ComModbusSimulator = "---";


        public bool ser_is_open = false;
            
        SerialPort SerPort_ttl = new SerialPort();
                
        internal void PinChanged_ttl(object sender, SerialPinChangedEventArgs e)
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


        private void port_DataReceived_ttl(object sender, SerialDataReceivedEventArgs e)
        {
            InputData = SerPort_ttl.ReadExisting();

            if (InputData != String.Empty)
            {
                SerDataAvailable = 1;
                Console.WriteLine(InputData);
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

            par_val = MyIni.Read("PortMBSim");
            ComModbusSimulator = par_val;
                       
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
                SerPort_ttl.PortName = Convert.ToString(ComPrgName);
                SerPort_ttl.BaudRate = Convert.ToInt32(@"115200");
                SerPort_ttl.DataBits = Convert.ToInt16(@"8");
                SerPort_ttl.StopBits = (StopBits)Enum.Parse(typeof(StopBits), @"1");
                SerPort_ttl.Handshake = (Handshake)Enum.Parse(typeof(Handshake), @"None");
                SerPort_ttl.Parity = (Parity)Enum.Parse(typeof(Parity), @"None");
                               
                SerialPinChangedEventHandler_ttl = new SerialPinChangedEventHandler(PinChanged_ttl);
                SerPort_ttl.PinChanged += SerialPinChangedEventHandler_ttl;
                
                SerPort_ttl.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(port_DataReceived_ttl);

                SerPort_ttl.Open();
                SerPort_ttl.RtsEnable = true;
                SerPort_ttl.DtrEnable = true;
                ser_is_open = true;
            }
            catch
            {
              
            }

            reset_ser();
            return par_val;
        }

        public void DeInit_Ser_Communication_Interface()
        {
            SerPort_ttl.Close();
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
                    CumulatedData = CumulatedData + InputData;
                    resp = CumulatedData;
                    break;
                }

            } while (maxtime < timeout); 

            return resp;
        }


        /* ======================================================================= */
        /*                 BEGIN OF IMPLEMENTATION SPECIFIC ROUTINE                */
        /* ======================================================================= */

        public void reset_ser()
        {
            SerDataAvailable = 0;
        }
        

        public string get_mac_response()
        {
            String ret_val = @"";
            String result = @"";
            int irv=0;
            int value;
            int maxtime;
            int loctimeout;


            maxtime = 0;
            loctimeout = 5000;  //5 sec

            /*
             at startup the ESP32 emit some bootloader msg, so that 
             we need to receive all this "spurious" data before
             get the MAC             
             */
            do
            {                         
                ret_val = serial_response(500);                
                value = String.Compare(ret_val, @"");                
                if (value != 0)
                {
                    string separator = @"MAC=";
                    // Part 1: get index of separator.
                    int separatorIndex = ret_val.IndexOf(separator);
                    // Part 2: if separator exists, get substring.
                    if (separatorIndex >= 0)
                    {
                        result = ret_val.Substring(separatorIndex + separator.Length, 12);                   
                    }
                }

                Thread.Sleep(50);
                maxtime += 50;

            } while (maxtime < loctimeout);

            return result;
        }
    }


}
