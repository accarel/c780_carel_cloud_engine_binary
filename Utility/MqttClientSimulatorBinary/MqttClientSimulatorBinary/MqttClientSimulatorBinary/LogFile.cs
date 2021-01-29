using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Globalization;


namespace MqttClientSimulatorBinary
{
    class LogFile
    {
        public string session_log_file_name = @"";
        StreamWriter sw;

        private void WriteTS()
        {

            string s_msg2;

            TimeSpan t = DateTime.UtcNow - new DateTime(1970, 1, 1);
            s_msg2 = ((int)t.TotalSeconds).ToString();
                      
            sw.Write(s_msg2);
        }

        public string GenerateLogFileName()
        {
            string sfn = @"";

            if (session_log_file_name != "") 
            { 
            
            }
            else
            {
                string cSS = DateTime.Now.Second.ToString();
                string cMn = DateTime.Now.Minute.ToString();
                string cHH = DateTime.Now.Hour.ToString();
                string cDD = DateTime.Now.Day.ToString();
                string cMM = DateTime.Now.Month.ToString();
                string cYY = DateTime.Now.Year.ToString();

                sfn = String.Format("{0:4}{1:2}{2:2}{3:2}{4:2}{5:2}{6}", cYY,cMM,cDD,cHH,cMn,cSS,"_log.txt");  

                session_log_file_name = sfn;
            }


            return sfn;
        }

        public void OpenLogFile()
        {

            if (sw != null) return;  //already open skip

            session_log_file_name = this.GenerateLogFileName();

            if (!File.Exists(session_log_file_name))
            {
                // Create a file to write to.
                sw = File.CreateText(session_log_file_name);                                
            }
            else 
            {
                sw = File.AppendText(session_log_file_name);
            }

            DateTimeOffset dto = new DateTimeOffset(1970, 1, 1, 0, 0, 0, TimeSpan.Zero);
            WriteTS();
            sw.WriteLine(" === LOG START ===");
        }

        public void CloseLogFile() 
        {
            if (sw != null)
            {
                if (sw.BaseStream != null)
                {
                    WriteTS();
                    sw.WriteLine(" === LOG STOP ===");
                    sw.Flush();
                    sw.Close();
                    session_log_file_name = "";
                }
            }
        }

        public void Write(string s) 
        {
            if (sw != null) sw.Write(s);
        }

        public void WriteLine(string s)
        {
            if (sw.BaseStream != null) sw.WriteLine(s);
        }

    }

}





