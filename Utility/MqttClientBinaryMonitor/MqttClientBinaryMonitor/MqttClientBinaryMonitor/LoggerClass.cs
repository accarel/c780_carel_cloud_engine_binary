using System;
using System.IO;


namespace MqttClientSimulatorBinary
{
    class LoggerClass
    {
        /*
        public static void Main_Example()
        {
            using (StreamWriter w = File.AppendText("log.txt"))
            {
                Log("Test1", w);
                Log("Test2", w);
            }

            using (StreamReader r = File.OpenText("log.txt"))
            {
                DumpLog(r);
            }
        }
        */

        public LoggerClass(string fn, string data)
        {
            using (StreamWriter w = File.AppendText(fn))
            {
                Log(data, w);
            }

        }

        public static void Log(string logMessage, TextWriter w)
        {
            string s_msg2;

            TimeSpan t = DateTime.UtcNow - new DateTime(1970, 1, 1);
            s_msg2 = ((int)t.TotalSeconds).ToString();

            w.WriteLine("##############################");
            //w.WriteLine($"{DateTime.Now.ToLongTimeString()} {DateTime.Now.ToLongDateString()}");
            w.WriteLine("UTC: " + s_msg2);
            w.WriteLine("##############################");
            w.WriteLine($"  :{logMessage}");
        }

        public static void DumpLog(StreamReader r)
        {
            string line;
            while ((line = r.ReadLine()) != null)
            {
                Console.WriteLine(line);
            }
        }
    }

} //namespace