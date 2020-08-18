using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Security;


namespace GME_Log_Analyzer
{
    public partial class Form1 : Form
    {
        public string SelectedFile;
        public long last_utc;
        public long last_upt;
        public int last_est;

        SortedList<int, string> sta_values = new SortedList<int, string>();

        public Form1()
        {
            InitializeComponent();
        }


        private void button_select_file_Click(object sender, EventArgs e)
        {
            if (openFileDialog2.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    SelectedFile = openFileDialog2.FileName;
                    textBox_selected_file.Text = SelectedFile;

                }
                catch (SecurityException ex)
                {
                    MessageBox.Show($"Security error.\n\nError message: {ex.Message}\n\n" +
                    $"Details:\n\n{ex.StackTrace}");
                }
            }
        }


        private void openFileDialog2_FileOk(object sender, CancelEventArgs e)
        {

        }

                                   

        bool SkipSeparator(string line)
        {
            bool retval = false;
            string s2 = @"##############################";
            bool b = line.Contains(s2);
            if (b == true) retval = true;
            return (retval);
        }



        private static readonly DateTime epoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);

        public static DateTime FromUnixTime(long unixTime)
        {
            return epoch.AddSeconds(unixTime);
        }


        long Check_UTC_Pattern(string line)
        {
            long retval = 0;
            string s2 = @"UTC: ";
            bool b = line.Contains(s2);

            if (b == true)
            {
                int index = line.IndexOf(s2);
                string s3 = line.Substring(index + s2.Length);
                retval = Int32.Parse(s3);
                last_utc = retval;
            }

            return retval;
        }


        long Check_upt_Pattern(string line)
        {
            long retval = 0;
            string s2 = @"""upt"": ";

            bool b = line.Contains(s2);

            if (b == true)
            {

                //  :{"ver": 257, "t": 1596458936, "upt": 336826, "fme": 121344, "est": 2, "sgn": -65   }  
                //1      2    3    4      5          6      7       8      9      10    11   12    13   14
                string[] seps = { ":", "," ,
                                  ":", ",",
                                  ":", ",",
                                  ":", ",", 
                                  ":", ",",
                                  ":", "}" 
                                 };

                string[] parts = line.Split(seps, StringSplitOptions.RemoveEmptyEntries);

                int count = 1;
                foreach (string z in parts)
                {
                    if (count == 7)
                    {
                        // get upt
                        last_upt = Int64.Parse(z);
                    }

                    if (count == 11)
                    {
                        // get est
                        last_est = Int16.Parse(z);
                    }

                    Console.WriteLine(count.ToString());
                    Console.WriteLine(z);
                    count++;                    
                }
                                                                                    
            }

            return retval;
        }
         

        void Check_sta_Pattern(string line)
        {
          
            string s2 = @"sta"":";
            string s3 = @" ""t"":";

            bool b = line.Contains(s2);

   
            if (b == true)
            {
                //extract sta value 
                int index_sta = line.IndexOf(s2);
                string sta_val = line.Substring(index_sta + s2.Length + 1, 1);

                int index_t = line.IndexOf(s3);
                string t_val = line.Substring(index_t + s3.Length, 11);

                int t_utc = Int32.Parse(t_val);
                string s_concat = sta_val + @" = " + FromUnixTime(t_utc).ToString() ;

                try
                {
                    if (sta_values.ContainsKey(t_utc))
                    {
                        string old_result;

                        if (sta_values.TryGetValue(t_utc, out old_result))
                        { 

                          if (old_result.Contains(s_concat)) 
                            { 
                            //same key at same time nothing to do
                            }
                          else 
                            {
                                //key value changed so we retain the latest one
                                sta_values.Remove(t_utc);
                                sta_values.Add(t_utc, s_concat);
                            }
                        }
                               
                    }
                    else
                    {
                        sta_values.Add(t_utc, s_concat);
                    }
                }
                catch 
                { 
                    //eliminated duplicate entry due to restart of the monitoring program
                }

                Application.DoEvents();
            }

        }

               
        long Check_cmd_Pattern(string line)
        {
            long retval = 0;
            string s2 = @"""cmd"": ";

            bool b = line.Contains(s2);

            if (b == true)
            {
                // :{"ver": 257, "rto": "reboot",        "cmd": 2, "cid": 65536}
                // :{"ver": 257, "rto": "start_engine",  "cmd": 17}
                // :{"ver": 257, "rto": "stop_engine",   "cmd": 18}
                // :{"ver": 257, "rto": "scan_line",     "cmd": 3, "lid": 1,   "dev": 0}
                // :{"ver": 257, "rto": "set_gw_config", "cmd": 1, "pva": 600, "pst": 300, "mka": 60, "lss": 60, "hss": 30}
                //1    2     3     4      5               6     7    8     9     10    11   12    13   14    15   16    17   18
                string[] seps = { ":", "," ,
                                  ":", ",",
                                  ":", ",",
                                  ":", ",",
                                  ":", ",",
                                  ":", ",",
                                  ":", ",",
                                  ":", ",",
                                  ":", "}"
                                 };

                string[] parts = line.Split(seps, StringSplitOptions.RemoveEmptyEntries);

                //there are other command with cmd inside
                if (parts.GetValue(6 - 1).ToString().Contains(@"cmd"))
                {
                            string val = parts.GetValue(7 - 1).ToString();
                            int v = Int32.Parse(val);

                            //select case for the right cmd parsing
                            switch (v)
                            {
                                case 1:
                                    richTextBox_cmd_1.AppendText(FromUnixTime(last_utc).ToString() + "\n");
                                    textBox_pva.Text = parts.GetValue(9 - 1).ToString();
                                    textBox_pst.Text = parts.GetValue(11 - 1).ToString();
                                    textBox_mka.Text = parts.GetValue(13 - 1).ToString();
                                    textBox_lss.Text = parts.GetValue(15 - 1).ToString();
                                    textBox_hss.Text = parts.GetValue(17 - 1).ToString();
                                    break;

                                case 2:
                                    richTextBox_other.AppendText(" 2 (reboot) " + FromUnixTime(last_utc).ToString() + 
                                        " cid :" + parts.GetValue(9-1).ToString() + "\n");
                                    break;

                                case 3:
                                    richTextBox_other.AppendText(" 3 (scan_line) " + FromUnixTime(last_utc).ToString() + "\n");
                                    break;

                                case 4:
                                    richTextBox_cmd_4.AppendText(FromUnixTime(last_utc).ToString() + "\n");
                                    break;

                                case 5:
                                    richTextBox_cmd_5.AppendText(FromUnixTime(last_utc).ToString() + " cid :" + parts.GetValue(18 - 1).ToString() + "\n");
                                    break;

                                case 17:
                                    richTextBox_other.SelectionColor = Color.Green;
                                    richTextBox_other.AppendText("17 (start_engine) " + FromUnixTime(last_utc).ToString() + "\n");
                                    richTextBox_other.SelectionColor = Color.Black;
                                    break;

                                case 18:
                                    richTextBox_other.SelectionColor = Color.Red;
                                    richTextBox_other.AppendText("18 (stop_engine) " + FromUnixTime(last_utc).ToString() + "\n");
                                    richTextBox_other.SelectionColor = Color.Black;
                                    break;
                            }
                                                             
                }
            }

            return retval;
        }



        long Check_hello_Pattern(string line)
        {
            long retval = 0;
            string s2 = @"""pn"": ";

            bool b = line.Contains(s2);

            if (b == true)
            {

                //:{ "ver": 257, "pn": "GTW000MWT0", "hwv": "100", "fwv": "081", "btm": 1596543813, "bau": 19200, "mqv": h'030101', "did": 23, "gid": h'2448CD0230074CCC9D5C4A1D2A51D4CE', "crc": 10589, "cid": 121}
                //1    2     3     4      5            6      7      8      9      10       11        12    13     14       15       16    17   18           19                              20    21      22    23            
                string[] seps = { ":", "," ,
                                  ":", ",",
                                  ":", ",",
                                  ":", ",",
                                  ":", ",",
                                  ":", ",",
                                  ":", ",",
                                  ":", ",",
                                  ":", ",",
                                  ":", ",",
                                  ":", "}"
                                 };

                string[] parts = line.Split(seps, StringSplitOptions.RemoveEmptyEntries);


                textBox_pn.Text  = parts.GetValue(5-1).ToString();
                textBox_hwv.Text = parts.GetValue(7-1).ToString();
                textBox_fwv.Text = parts.GetValue(9-1).ToString();
                textBox_bau.Text = parts.GetValue(13-1).ToString();
                textBox_last_cid.Text = parts.GetValue(23-1).ToString();
                                             

                int count = 1;
                foreach (string z in parts)
                {
                    Console.WriteLine(count.ToString());
                    Console.WriteLine(z);
                    count++;
                }

            }

            return retval;
        }



        long Check_cmd_upgrade_Pattern(string line)
        {
            long retval = 0;
            string s2 = @"""cmd"": ";

            bool b = line.Contains(s2);

            if (b == true)
            {
                // :{"ver": 257, "rto": "rr/GXT4WB/170a4e1062844ddcb88b3e150b2d42c5", "cmd": 13, "usr": "URjBUEu96EI8", "pwd": "dyw7L4HJOYsNKpDaf5IACy4PLKc9Y", "uri": "https://test-gwfs.remotepro.io/gwfs/v1/certificates/3", "crc": 63671}                
                //1    2     3     4                     5                              6     7    8           9          10       11                             12     13                                                       14    15  
                string[] seps = { ":", "," ,
                                  ":", ",",
                                  ":", ",",
                                  ":", ",",
                                  ":", ",",
                                  ":", ",",
                                  ":", "}"
                                 };

                string[] parts = line.Split(seps, StringSplitOptions.RemoveEmptyEntries);

                //there are other command with cmd inside
                if (parts.GetValue(6 - 1).ToString().Contains(@"cmd"))
                {

                    string val = parts.GetValue(7 - 1).ToString();
                    int v = Int32.Parse(val);

                    switch (v)
                    {
                        case 10:
                            richTextBox_cmd_10_upg_dbg_fw.AppendText(FromUnixTime(last_utc).ToString() + "\n");
                            break;

                        case 11:
                            richTextBox_cmd_11_upg_dev_fw.AppendText(FromUnixTime(last_utc).ToString() + "\n");
                            break;

                        case 13:
                            richTextBox_cmd_13.AppendText(FromUnixTime(last_utc).ToString() + "\n");                            
                            break;                                      
                    }
                                      

                    int count = 1;
                    foreach (string z in parts)
                    {
                        Console.WriteLine(count.ToString());
                        Console.WriteLine(z);
                        count++;
                    }
                }
            }

            return retval;
        }

                     

        void reset_text_box()
        {
            textBox_info.Text = "";            
            richTextBox_sta_time.Text = "";
            richTextBox_cmd_1.Text = "";
            richTextBox_cmd_4.Text = "";
            richTextBox_cmd_5.Text = "";
            textBox_UTC2local_time.Text = "";
            textBox_upt.Text = "";
            textBox_last_upt_days.Text = "";
            textBox_last_est.Text = "";
            textBox_lss.Text = "";
            textBox_hss.Text = "";
            textBox_mka.Text = "";
            textBox_pva.Text = "";
            textBox_pst.Text = "";
            richTextBox_cmd_10_upg_dbg_fw.Text = "";            
            richTextBox_cmd_11_upg_dev_fw.Text = "";
            richTextBox_other.Text = "";
        }
                                    

        private void button_analyze_Click(object sender, EventArgs e)
        {
            reset_text_box();
            sta_values.Clear();

            SelectedFile = textBox_selected_file.Text;
            string[] lines = System.IO.File.ReadAllLines(SelectedFile);


            //analizing    
            foreach (string line in lines)
            {


                if (SkipSeparator(line) == false)
                {
                    Console.WriteLine("\t" + line);

                    if (Check_UTC_Pattern(line) != 0) 
                    {
                        textBox_UTC2local_time.Text = FromUnixTime(last_utc).ToString();
                        continue;
                    }

                    Check_sta_Pattern(line);
                    Check_upt_Pattern(line);
                    Check_cmd_Pattern(line);
                    Check_cmd_upgrade_Pattern(line);
                    Check_hello_Pattern(line);

                }

            }


            //update text box
            foreach (KeyValuePair<int, string> kvp in sta_values)
            {

                if (kvp.Value.Contains("0 ="))
                {
                    richTextBox_sta_time.SelectionColor = Color.Red;
                }
                else 
                {
                    richTextBox_sta_time.SelectionColor = Color.Black;
                }


                richTextBox_sta_time.AppendText(kvp.Value + "\n");

            }


            textBox_upt.Text = last_upt.ToString();
            float upt_days;
            upt_days = (float)last_upt / (float)(60 * 60 * 24);
            textBox_last_upt_days.Text = upt_days.ToString();


            textBox_last_est.Text = last_est.ToString();


            textBox_info.Text = textBox_info.Text + "Analysis done!\r\n";
        }


    }
}
