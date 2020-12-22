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

        const int SPARE_BYTES = 5;
        public struct _GeneralLogHeader
        {
            public byte version;
            public UInt16 headerSize;
            public UInt16 numberOfLog;
            public UInt32 logsCrc;
            public byte[] spare; //SPARE_BYTES 5
            public UInt16 crc;
        };


        public struct _PeriodicLogHeader
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
            public UInt32 sampleTime;
            public UInt32 startTime;
            public UInt32 lastSampleTime;
            public UInt32 spareBytes;
            public UInt16 crc;
        };

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
        public struct VM_CompressionHeader
        {
           public UInt16 Version;
           public byte[] ProjectId;  //GUIDSIZE  16
           public UInt32 SizePayload;
           public UInt32 CrcPayload;
           public UInt16 CompressionType;
           public UInt16 FileType;
           public UInt32 CompressedSize;
           public byte[] Flags;      //FLAGS 2
           public UInt16 Spare;
           public UInt16 Crc;
          };







        public Form1()
        {
            InitializeComponent();
        }

        void Extract_LogData(string s_file_path)
        {
            string[] lines = System.IO.File.ReadAllLines(s_file_path);
            string docPath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
            string outfilefullpath;

            outfilefullpath = textBox_log_file_out.Text;   //Path.Combine(docPath, "Ale_WriteLines.txt");

            File.Delete(outfilefullpath);

            string s;

            // Append text to an existing file named "WriteLines.txt".
            using (StreamWriter outputFile = new StreamWriter(outfilefullpath, true))
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






    public void Log_File_Analyzer()
    {
                        
            VM_CompressionHeader CompHeader;
            _GeneralLogHeader GenHeader;
            _PeriodicLogHeader PerHeader;

            using (BinaryReader reader = new BinaryReader(File.Open(textBox_log_file_bin.Text , FileMode.Open)))
            {               
                CompHeader.ProjectId = new byte[GUIDSIZE];
                CompHeader.Flags = new byte[FLAGS];

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

                //is a PERIODIC log file 
                GenHeader.version = reader.ReadByte();
                GenHeader.headerSize = reader.ReadUInt16();
                GenHeader.numberOfLog = reader.ReadUInt16();
                GenHeader.logsCrc = reader.ReadUInt32();
                GenHeader.spare = reader.ReadBytes(SPARE_BYTES);
                GenHeader.crc = reader.ReadUInt16();
                               
                PerHeader.uuid = (UInt64)reader.ReadInt64();
                PerHeader.type = reader.ReadByte(); ;
                PerHeader.version = reader.ReadByte(); 
                PerHeader.headerSize = reader.ReadUInt16(); 
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
            }

            using (StreamWriter outputFile = new StreamWriter(textBox_Disassembled.Text))
            {
                outputFile.WriteLine("--- COMPRESSION HEADER ---");
                outputFile.WriteLine("Version = " + CompHeader.Version.ToString());
                outputFile.Write("ProjectId = ");
                foreach (var data in CompHeader.ProjectId)
                {
                    outputFile.Write(data.ToString() + " ");
                }
                outputFile.WriteLine("");

                outputFile.WriteLine("SizePayload = " + CompHeader.SizePayload.ToString());
                outputFile.WriteLine("CrcPayload = " + CompHeader.CrcPayload.ToString());
                outputFile.WriteLine("CompressionType = " + CompHeader.CompressionType.ToString());
                outputFile.WriteLine("FileType = " + CompHeader.FileType.ToString());
                outputFile.WriteLine("CompressedSize = " + CompHeader.CompressedSize.ToString());

                outputFile.Write("Flags = ");
                foreach (var data in CompHeader.Flags)
                {
                    outputFile.Write(data.ToString() + " ");
                }
                outputFile.WriteLine("");

                outputFile.WriteLine("Spare = " + CompHeader.Spare.ToString());
                outputFile.WriteLine("Crc = " + CompHeader.Crc.ToString());
                outputFile.WriteLine("----------------------");
                
                /* -------------------------------------------------------------- */
                outputFile.WriteLine("--- Generic Header ---");
                outputFile.Write("version = ");      outputFile.WriteLine(GenHeader.version.ToString());
                outputFile.Write("headerSize = ");   outputFile.WriteLine(GenHeader.headerSize.ToString());
                outputFile.Write("numberOfLog = ");     outputFile.WriteLine(GenHeader.numberOfLog.ToString());
                outputFile.Write("logsCrc = ");         outputFile.WriteLine(GenHeader.logsCrc.ToString());
                                
                outputFile.Write("spare = ");          
                foreach (var data in GenHeader.spare)
                {
                    outputFile.Write(data.ToString() + " ");
                }
                outputFile.WriteLine("");
                outputFile.Write("crc = ");             outputFile.WriteLine(GenHeader.crc.ToString());
                outputFile.WriteLine("----------------------");

                /* ---------------------------------------------------------------- */
                outputFile.WriteLine("--- Periodic Header ---");
                outputFile.Write("uuid = ");                   outputFile.WriteLine(PerHeader.uuid.ToString());
                outputFile.Write("type = ");                    outputFile.WriteLine(PerHeader.type.ToString());
                outputFile.Write("version = ");                outputFile.WriteLine(PerHeader.version.ToString());
                outputFile.Write("headerSize = ");             outputFile.WriteLine(PerHeader.headerSize.ToString());
                outputFile.Write("id = ");                     outputFile.WriteLine(PerHeader.id.ToString());
                outputFile.Write("samplesCount = ");           outputFile.WriteLine(PerHeader.samplesCount.ToString());
                outputFile.Write("sampleLength = ");           outputFile.WriteLine(PerHeader.sampleLength.ToString());
                outputFile.Write("currentSample= ");           outputFile.WriteLine(PerHeader.currentSample.ToString());
                outputFile.Write("lastSampleIndex = ");        outputFile.WriteLine(PerHeader.lastSampleIndex.ToString());
                outputFile.Write("sampleTime = ");             outputFile.WriteLine(PerHeader.sampleTime.ToString());
                outputFile.Write("startTime = ");              outputFile.WriteLine(PerHeader.startTime.ToString());
                outputFile.Write("lastSampleTime = ");         outputFile.WriteLine(PerHeader.lastSampleTime.ToString());
                outputFile.Write("spareBytes = ");             outputFile.WriteLine(PerHeader.spareBytes.ToString());
                outputFile.Write("crc = ");                    outputFile.WriteLine(PerHeader.crc.ToString());
                outputFile.WriteLine("----------------------");

            }

        }
        


    public void Show_Disassembled() 
    {
      
      string[] lines = System.IO.File.ReadAllLines(textBox_Disassembled.Text);
      foreach (string line in lines)
      {
          textBox_Show_Disassembled.AppendText(line + "\r\n");
      }

    }



    private void button_analyze_Click(object sender, EventArgs e)
    {
       if (File.Exists(textBox_logfile_in.Text))
       {
           Extract_LogData(textBox_logfile_in.Text);
           Convert_to_binary_file();                
           Log_File_Analyzer();

           Show_Disassembled();

       }
       else 
       {
           textBox_message.AppendText("File not found " + textBox_logfile_in.Text);
       }

    }



    private void Form1_Load(object sender, EventArgs e)
    {

    }

    private void label3_Click(object sender, EventArgs e)
    {

    }


    }



}
