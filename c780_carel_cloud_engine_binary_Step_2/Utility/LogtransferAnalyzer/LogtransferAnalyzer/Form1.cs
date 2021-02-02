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


namespace LogtransferAnalyzer
{
    public partial class Form1 : Form 
    {
        const byte LOG_TYPE_PERIODIC = 0;
        const byte LOG_TYPE_EVENT    = 1;

        const int SPARE_BYTES = 5;


        const int GENERALLOGHEADER_SIZE = 16;
        public struct _GeneralLogHeader
        {
            public byte version;
            public UInt16 headerSize;
            public UInt16 numberOfLog;
            public UInt32 logsCrc;
            public byte[] spare; //SPARE_BYTES 5
            public UInt16 crc;
        };

        const int COMMONLOGHEADER_SIZE = 12;
        public struct _CommonLogHeader
        {
            public UInt64 uuid;
            public byte type;
            public byte version;
            public UInt16 headerSize;
        };

        const int PERIODICLOGHEADER_SIZE = 50;
        public struct _PeriodicLogHeader
        {
            //public UInt64 uuid;
            //public byte   type;
            //public byte   version;
            //public UInt16 headerSize;
            public UInt16 id;
            public UInt32 samplesCount;
            public UInt32 sampleLength;
            public UInt32 currentSample;
            public UInt32 lastSampleIndex;
            public UInt32 sampleTime;
            public UInt32 startTime;
            public UInt32 lastSampleTime;
            public UInt32 spareBytes;
            public UInt16 crc;
        };

        const int EVENTLOGHEADER_SIZE = 36;

        public struct _EventLogHeader
        {
            public UInt64 uuid;
            public byte type;
            public byte version;
            public UInt16 headerSize;
            public UInt16 id;
            public UInt32 samplesCount;
            public UInt32 sampleLength;
            public UInt32 currentSample;
            public UInt32 lastSampleIndex;
            public UInt32 spareBytes;
            public UInt16 crc;
        };

        const int GUIDSIZE = 16;
        const int FLAGS = 2;

        const int VM_COMPRESSIONHEADER_SIZE = 40;
        public struct VM_CompressionHeader
        {
           public UInt16 Version;
           public byte[] ProjectId; // GUIDSIZE16
           public UInt32 SizePayload;
           public UInt32 CrcPayload;
           public UInt16 CompressionType;
           public UInt16 FileType;
           public UInt32 CompressedSize;
           public byte[] Flags;      //FLAGS 2
           public UInt16 Spare;
           public UInt16 Crc;
          };

        VM_CompressionHeader CompHeader;
        _GeneralLogHeader GenHeader;
        _CommonLogHeader ComHeader;

        _PeriodicLogHeader PerHeader;

        public byte[] poff_bit_field;
        UInt32 num_of_poff_field = 0;


        public struct _CVS_Record
        {
            public UInt64 record_id;
            public UInt32 datetime;
            public byte poff_state;
            public byte[] values ;
        };

        /* ------------------------------------------------ */
        /*              for the files                       */
        /* ------------------------------------------------ */

        StreamWriter outputFile;
        BinaryReader reader;


        /* ------------------------------------------------ */
        /*              for the user interface              */
        /* ------------------------------------------------ */

        ToolTip toolTip_textbox_Disassembled = new ToolTip();

        /* ------------------------------------------------ */
        /*                  PROGRAM BODY                    */
        /* ------------------------------------------------ */

        public string cmd_line_file = @"";
        public Form1()
        {
            InitializeComponent();
        }

        private void texbox_Disassembeld_ToolTip()
        {
            
        }


        public void set_cmd_line_file_name(string s)
        {
            cmd_line_file = s;
        }



        private void button_analyze_Click(object sender, EventArgs e)
        {
            if (File.Exists(textBox_logfile_in.Text))
            {
                Extract_LogData(textBox_logfile_in.Text);
                Convert_to_binary_file();
                Log_File_Analyzer();
                Show_Disassembled();
                Generate_CVS_File();
            }
            else
            {
                textBox_message.AppendText("File not found " + textBox_logfile_in.Text + "\r\n");
            }

        }


        /* ------------------------------------------------ */
        /*                     PROGRAM                      */
        /* ------------------------------------------------ */

        void Extract_LogData(string s_file_path)
        {
            string[] lines = System.IO.File.ReadAllLines(s_file_path);
            string docPath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
            string outfilefullpath;

            outfilefullpath = textBox_log_file_out.Text;   //Path.Combine(docPath, "Ale_WriteLines.txt");

            File.Delete(outfilefullpath);

            string s;

            // Append text to an existing file named "WriteLines.txt".
            using (outputFile = new StreamWriter(outfilefullpath, true))
            {
                //analizing    
                foreach (string line in lines)
                {                  
                    if (line.Contains("$$$"))
                    {
                        s = line.Replace("$", "");
                        s = s.Trim();
                        outputFile.WriteLine(s);
                    }
                }                                
            }

            textBox_message.AppendText("File succesfully processed \r\n");

        }

        public static byte[] StringToByteArray(String hex)
        {
            int NumberChars = hex.Length;
            byte[] bytes = new byte[NumberChars / 2];
            for (int i = 0; i < NumberChars; i += 2)
                bytes[i / 2] = Convert.ToByte(hex.Substring(i, 2), 16);
            return bytes;
        }


        private void Convert_to_binary_file()
        {
            byte singlebyte;
            string[] lines;
            string last_line = "";

            singlebyte = 0x00;

            try
            {

                using (FileStream stream = new FileStream(textBox_log_file_bin.Text, FileMode.Create))
                {
                    BinaryWriter writer = new BinaryWriter(stream);
                    lines = System.IO.File.ReadAllLines(textBox_log_file_out.Text);
                    byte[] b;

                    foreach (string line in lines)
                    {
                        last_line = line;  // in case of fail the last line will be reported 

                        string[] exbytes = line.Split(' ');
                        foreach (string exbyte in exbytes)
                        {
                            b = StringToByteArray(exbyte);
                            writer.Write(b[0]);
                        }
                    }

                    writer.Close();
                }

                textBox_message.AppendText("Binary file succesfully generated \r\n");

            }
            catch 
            {
                textBox_message.AppendText("Binary file generation FAIL  \r\n");
                textBox_message.AppendText(last_line);
                textBox_message.AppendText("\r\n");
            }

            
        }


        public static DateTime UnixTimeStampToDateTime(double unixTimeStamp)
        {
            // Unix timestamp is seconds past epoch
            System.DateTime dtDateTime = new DateTime(1970, 1, 1, 0, 0, 0, 0, System.DateTimeKind.Utc);
            dtDateTime = dtDateTime.AddSeconds(unixTimeStamp).ToLocalTime();
            return dtDateTime;
        }




        /* ======================================================================== */

        public void Compression_Header_Info()
        {
            outputFile.WriteLine("--- COMPRESSION HEADER ---");
            outputFile.WriteLine("Version         = " + CompHeader.Version.ToString());
            outputFile.Write("ProjectId       = ");
            foreach (var data in CompHeader.ProjectId)
            {
                string sx = String.Format("{0:X}", data);
                outputFile.Write(sx);
            }
            outputFile.WriteLine("");

            outputFile.WriteLine("SizePayload     = " + CompHeader.SizePayload.ToString());
            outputFile.WriteLine("CrcPayload      = " + CompHeader.CrcPayload.ToString());
            outputFile.WriteLine("CompressionType = " + CompHeader.CompressionType.ToString());
            outputFile.WriteLine("FileType        = " + CompHeader.FileType.ToString());
            outputFile.WriteLine("CompressedSize  = " + CompHeader.CompressedSize.ToString());

            outputFile.Write("Flags           = ");
            foreach (var data in CompHeader.Flags)
            {
                outputFile.Write(data.ToString() + " ");
            }
            outputFile.WriteLine("");

            outputFile.WriteLine("Spare           = " + CompHeader.Spare.ToString());
            outputFile.WriteLine("Crc             = " + CompHeader.Crc.ToString());
            outputFile.WriteLine("----------------------");
        }

        public void Generic_Header_Info()
        {
            outputFile.WriteLine("--- Generic Header ---");
            outputFile.Write("version             = "); outputFile.WriteLine(GenHeader.version.ToString());
            outputFile.Write("headerSize          = "); outputFile.WriteLine(GenHeader.headerSize.ToString());
            outputFile.Write("numberOfLog         = "); outputFile.WriteLine(GenHeader.numberOfLog.ToString());
            outputFile.Write("logsCrc             = "); outputFile.WriteLine(GenHeader.logsCrc.ToString());

            outputFile.Write("spare               = ");
            foreach (var data in GenHeader.spare)
            {
                outputFile.Write(data.ToString() + " ");
            }
            outputFile.WriteLine("");
            outputFile.Write("crc                 = "); outputFile.WriteLine(GenHeader.crc.ToString());
            outputFile.WriteLine("----------------------");
        }

        public void Periodic_Log_Info()
        {
            outputFile.WriteLine("--- Periodic Header ---");
            outputFile.Write("uuid            = "); outputFile.WriteLine(ComHeader.uuid.ToString());
            outputFile.Write("type            = "); outputFile.WriteLine(ComHeader.type.ToString());
            outputFile.Write("version         = "); outputFile.WriteLine(ComHeader.version.ToString());
            outputFile.Write("headerSize      = "); outputFile.WriteLine(ComHeader.headerSize.ToString());
            outputFile.Write("id              = "); outputFile.WriteLine(PerHeader.id.ToString());
            outputFile.Write("samplesCount    = "); outputFile.WriteLine(PerHeader.samplesCount.ToString());
            outputFile.Write("sampleLength    = "); outputFile.WriteLine(PerHeader.sampleLength.ToString());
            outputFile.Write("currentSample   = "); outputFile.WriteLine(PerHeader.currentSample.ToString());
            outputFile.Write("lastSampleIndex = "); outputFile.WriteLine(PerHeader.lastSampleIndex.ToString());
            outputFile.Write("sampleTime      = "); outputFile.WriteLine(PerHeader.sampleTime.ToString());
            outputFile.Write("startTime       = "); outputFile.Write(PerHeader.startTime.ToString());
            outputFile.Write(" " + UnixTimeStampToDateTime(PerHeader.startTime).ToString() + "\r\n");

            outputFile.Write("lastSampleTime  = "); outputFile.Write(PerHeader.lastSampleTime.ToString());
            outputFile.Write(" " + UnixTimeStampToDateTime(PerHeader.lastSampleTime).ToString() + "\r\n");

            outputFile.Write("spareBytes      = "); outputFile.WriteLine(PerHeader.spareBytes.ToString());
            outputFile.Write("crc             = "); outputFile.WriteLine(PerHeader.crc.ToString());
            outputFile.WriteLine("----------------------");


            if (ComHeader.version == 1)
            {
                outputFile.WriteLine("--- poff_bit_field ---");

                for (int i = 0; i < num_of_poff_field; i++)
                {
                    string yourByteString = Convert.ToString(poff_bit_field[i], 2).PadLeft(8, '0');
                    outputFile.WriteLine(yourByteString);
                }

                outputFile.WriteLine("----------------------");
            }

        }

        public void Periodic_Log_Data(int samples_count, int sample_lenght) 
        {
            int total_bytes;

            total_bytes = samples_count * sample_lenght;

            byte[] flux = new byte[total_bytes];

            var stream = reader.BaseStream;
            long fpos = stream.Position;

            //textBox_message.AppendText("stream " + stream.Length.ToString() + "\r\n");
            //textBox_message.AppendText("char " +  sizeof(byte).ToString() + "\r\n");
                 
            reader.Read(flux, 0, total_bytes);

            int temp2;
            int temp4;

            switch (PerHeader.sampleLength) 
            {
                case 2:
                    for (int i = 0; i < (total_bytes); i+=2)
                    {
                        temp2 = ((Int16)flux[i]) << 8;                        
                        temp2 += ((Int16)flux[i+1]); //+2

                        outputFile.WriteLine(temp2.ToString("X"));

                        // Store integer 182
                        //int intValue = 182;
                        // Convert integer 182 as a hex in a string variable
                        //string hexValue = intValue.ToString("X");
                        // Convert the hex string back to the number
                        //int intAgain = int.Parse(hexValue, System.Globalization.NumberStyles.HexNumber);
                    }

                    break;

                case 4:
                    for (int i = 0; i < (total_bytes); i += 4)
                    {
                        temp4 = ((Int16)flux[i]) << 8;

                        temp4 += ((Int16)flux[i + 2]);

                        outputFile.WriteLine(temp4.ToString("X"));

                        // Store integer 182
                        //int intValue = 182;
                        // Convert integer 182 as a hex in a string variable
                        //string hexValue = intValue.ToString("X");
                        // Convert the hex string back to the number
                        //int intAgain = int.Parse(hexValue, System.Globalization.NumberStyles.HexNumber);
                    }

                    break;

                default:
                    for (int i = 0; i < total_bytes; i++)
                    {
                        outputFile.WriteLine(flux[i].ToString());
                    }
                    break;
                        
            }
         
        }

        public void Log_File_Analyzer()
        {                      
            PerHeader.id = 0;
            PerHeader.samplesCount =0;
            PerHeader.sampleLength=0;
            PerHeader.currentSample=0;
            PerHeader.lastSampleIndex=0;
            PerHeader.sampleTime=0;
            PerHeader.startTime=0;
            PerHeader.lastSampleTime=0;
            PerHeader.spareBytes=0;
            PerHeader.crc=0;


            outputFile = new StreamWriter(textBox_Disassembled.Text + ".TXT");

            using (reader = new BinaryReader(File.Open(textBox_log_file_bin.Text, FileMode.Open)))
            {
                if (checkBox_Strip_Comp_Head.Checked == false)
                {
                    CompHeader.ProjectId = new byte[GUIDSIZE];
                    CompHeader.Flags = new byte[FLAGS];

                    //get compressed header
                    CompHeader.Version = reader.ReadUInt16();
                    CompHeader.ProjectId = reader.ReadBytes(GUIDSIZE);
                    CompHeader.SizePayload = reader.ReadUInt32();
                    CompHeader.CrcPayload = reader.ReadUInt32();
                    CompHeader.CompressionType = reader.ReadUInt16();
                    CompHeader.FileType = reader.ReadUInt16();
                    CompHeader.CompressedSize = reader.ReadUInt32();
                    CompHeader.Flags = reader.ReadBytes(FLAGS);
                    CompHeader.Spare = reader.ReadUInt16();
                    CompHeader.Crc = reader.ReadUInt16();
                    //save to TXT
                    Compression_Header_Info();
                }

                //get general header 
                GenHeader.version = reader.ReadByte();
                GenHeader.headerSize = reader.ReadUInt16();
                GenHeader.numberOfLog = reader.ReadUInt16();
                GenHeader.logsCrc = reader.ReadUInt32();
                GenHeader.spare = reader.ReadBytes(SPARE_BYTES);
                GenHeader.crc = reader.ReadUInt16();

                //save to TXT
                Generic_Header_Info();

                /* --------------------------------------- */
                /*             extract the logs            */
                /* --------------------------------------- */
                for (int nlogs = 0; nlogs < GenHeader.numberOfLog; nlogs++)
                {
                    ComHeader.uuid = (UInt64)reader.ReadInt64();
                    ComHeader.type = reader.ReadByte();
                    ComHeader.version = reader.ReadByte();
                    ComHeader.headerSize = reader.ReadUInt16();

                    switch (ComHeader.type)
                    {
                        case LOG_TYPE_PERIODIC:

                            PerHeader.id = reader.ReadUInt16();

                            PerHeader.samplesCount = reader.ReadUInt32();
                            PerHeader.sampleLength = reader.ReadUInt32();
                            PerHeader.currentSample = reader.ReadUInt32();
                            PerHeader.lastSampleIndex = reader.ReadUInt32();
                            PerHeader.sampleTime = reader.ReadUInt32();
                            PerHeader.startTime = reader.ReadUInt32();
                            PerHeader.lastSampleTime = reader.ReadUInt32();
                            PerHeader.spareBytes = reader.ReadUInt32();
                            PerHeader.crc = reader.ReadUInt16();
                                                      

                            switch (ComHeader.version)
                            {
                                case 1:
                                  /* V1 have a hard to understand method of power off bit field with a dinamic size */
                                  num_of_poff_field = PerHeader.samplesCount / 8;
                                  if ((PerHeader.samplesCount % 8) > 0) num_of_poff_field++;
                                  poff_bit_field = new byte[num_of_poff_field];
                                  poff_bit_field = reader.ReadBytes((int)num_of_poff_field);

                                  Periodic_Log_Info();
                                  Periodic_Log_Data((int)PerHeader.samplesCount, (int)PerHeader.sampleLength);
                                  break;

                                case 2:
                                  textBox_message.AppendText("Log Periodic Version 2 currently not managed!\r\n");
                                  break;

                                default:
                                  textBox_message.AppendText("Unknow Log version schema!\r\n");
                                  break;
                            }


                            break;


                        case LOG_TYPE_EVENT:

                            break;

                        default:
                            textBox_message.AppendText("Found an unknow log type !\r\n");
                            break;

                    }

                }


                outputFile.Close();

            }

        }
        

    public void Show_Disassembled() 
    {   
            
      string[] lines = System.IO.File.ReadAllLines(textBox_Disassembled.Text + ".TXT");
      foreach (string line in lines)
      {
          textBox_Show_Disassembled.AppendText(line + "\r\n");
      }

    }

    private void Generate_CVS_File()
    {
            UInt64 count;
            _CVS_Record[] cvs_data = new _CVS_Record[PerHeader.samplesCount];

            /* allocate and initialize structures */
            for (count = 0; count < PerHeader.samplesCount; count++)
            {
                cvs_data[count].values = new byte[PerHeader.sampleLength]; 
            }

            for (count = 0; count < PerHeader.samplesCount; count++) 
            {
                cvs_data[count].record_id = count + 1;
                cvs_data[count].datetime = 0;
                cvs_data[count].poff_state = 255; /* Just to mark as uninit */
            }

            /* --- poff_bit_field --- */           
            int cvs_data_index = 0;

            /*LOG model V1 */
            if (ComHeader.version == 1)
            {
                int byte_field;
                
                for (int i = 0; i < num_of_poff_field; i++)
                {
                    byte_field = (int)poff_bit_field[i];
                    int shift_val;

                    for (int bf=0; bf < 8; bf++) 
                    {
                        shift_val= 1 << bf;
                        Int32 res = byte_field & shift_val;

                        if ( res != 0)
                        {
                            cvs_data[cvs_data_index].poff_state = 0;
                        }
                        else 
                        {
                            cvs_data[cvs_data_index].poff_state = 0;
                        }
                        
                        cvs_data_index++;
                    }

                }
                
                cvs_data_index = 0;
                /* set the timestamp of each sample */
                if (PerHeader.currentSample == PerHeader.samplesCount)
                { /* we need to manage the wrap around */

                    textBox_message.AppendText("WARNING! Periodic wrap around found check data carefully!\r\n");

                    /* WORK IN PROGRESS NOT SURE IF CORRECTLY MANAGED */
                    for (UInt32 i = 0; i < PerHeader.lastSampleIndex; i++)
                    {
                        cvs_data[i].datetime = PerHeader.startTime + (i * PerHeader.sampleTime);
                    }

                    /* now from lastSampleIndex to the end of the circular buffer the time is older 
                       so that how many old is it ? a quick calculation needed
                     */
                    UInt32 oldtimestart = PerHeader.startTime - (PerHeader.sampleTime * (PerHeader.samplesCount - PerHeader.lastSampleIndex));

                    for (UInt32 i = PerHeader.lastSampleIndex; i < PerHeader.samplesCount; i++)
                    {
                        cvs_data[i].datetime = oldtimestart + (i * PerHeader.sampleTime);
                    }

                }
                else 
                { /* no wrap around reached */

                    for (UInt32 i = 0; i < PerHeader.samplesCount; i++)
                    {
                        cvs_data[i].datetime = PerHeader.startTime + (i * PerHeader.sampleTime);
                    }

                }

            }




        }

    private void Form1_Load(object sender, EventArgs e)
    {      
       // Set up the delays for the ToolTip.
       toolTip_textbox_Disassembled.AutoPopDelay = 1000;
       toolTip_textbox_Disassembled.InitialDelay = 250;
       toolTip_textbox_Disassembled.ReshowDelay = 500;
       // Force the ToolTip text to be displayed whether or not the form is active.
       toolTip_textbox_Disassembled.ShowAlways = true;
       //toolTip_textbox_Disassembled.IsBalloon = true;
       // Set up the ToolTip 
       toolTip_textbox_Disassembled.SetToolTip(textBox_Disassembled, "without extension");
       //toolTip_textbox_Disassembled.SetToolTip(this.label4, "without extension");
       //toolTip_textbox_Disassembled.SetToolTip(this.checkBox1, "My checkBox1");
   
        }

    private void label3_Click(object sender, EventArgs e)
        {

        }


    public Form1(string ftype, string set_cmd_line_file_name)
    {
       InitializeComponent();


       Int16 val = Int16.Parse(ftype);

       this.cmd_line_file = set_cmd_line_file_name;
       if (val == 1) 
       {  //text file
          textBox_logfile_in.Text = this.cmd_line_file;
       }


       if (val == 2)
       {  //binary
           this.cmd_line_file = set_cmd_line_file_name;           
           textBox_log_file_bin.Text = this.cmd_line_file;
           textBox_logfile_in.Text = "";
           textBox_log_file_out.Text = "";
       }





        }

        private void button1_Click(object sender, EventArgs e)
        {
            Log_File_Analyzer();
            Show_Disassembled();
            Generate_CVS_File();
        }
    }



}
