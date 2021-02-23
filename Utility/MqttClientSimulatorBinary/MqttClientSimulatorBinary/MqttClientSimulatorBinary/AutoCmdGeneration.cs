using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Timers;

using System.Net;
using System.Net.Sockets;

using System.Diagnostics;

using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Exceptions;
using uPLibrary.Networking.M2Mqtt.Internal;
using uPLibrary.Networking.M2Mqtt.Messages;
using uPLibrary.Networking.M2Mqtt.Session;
using uPLibrary.Networking.M2Mqtt.Utility;

using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;
using System.Net.Security;

using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

using System.IO;
//using System.Json;
using PeterO;
using PeterO.Cbor;
using PeterO.Numbers;

using Soft160.Data.Cryptography;



namespace MqttClientSimulatorBinary
{

    public partial class Form1 : Form
    {
        int AutoCmdIteration = 0;
        int IterationDelay = 5000;

        void CmdTestSuite()
        {
            int count;
            int val = 0;
            double start_time, ct;
            int iteration = 1;
            int iteration_delay = 1;
            int value;
            string test_file;
            string n_of_test_val;
            int n_of_test_val_int;
            string par_val;
            int test_count;

            if (MQTT_Connect == false)
            {
                MessageBoxInfo("System offline try to connect!", "ERROR");
                return;
            }

            test_file = textBox_Test_File.Text;

            if (!(File.Exists(test_file)))
            {
                MessageBoxInfo("Not found " + test_file, "ERROR");
                return;
            }

            iteration = 0;
            if (Int32.TryParse(textBox_AutoCmdIteration.Text, out iteration))
            {
                AutoCmdIteration = iteration;
            }

            iteration = 0;
            if (Int32.TryParse(textBox_IterationDelay.Text, out iteration_delay))
            {
                IterationDelay = iteration_delay;
            }


            var MyIni = new IniFile(test_file);

            n_of_test_val = MyIni.Read("N_OF_TEST");

            if (Int32.TryParse(n_of_test_val, out n_of_test_val_int))
            {

            }

            for (count = 0; count < AutoCmdIteration; count++)
            {
                textBox_AutoCmdIteration.ForeColor = Color.Red;
                textBox_AutoCmdIteration.Text = count.ToString();

                for (int ciclo = 1; ciclo <= n_of_test_val_int; ciclo++)
                {


                    textBox_TestNo.Text = ciclo.ToString() + " / " + n_of_test_val_int.ToString();

                    par_val = MyIni.Read("TEST" + ciclo.ToString());

                    string[] words = par_val.Split(',');
                    string par_function = words[0];
                    int par_function_int;

                    Int32.TryParse(par_function, out par_function_int);

                    int par_addr_int;
                    int par_val_int;
                    float par_val_float;
                    int par_dim_value_int;
                    int par_pos_value;
                    int par_len_value;
                    int par_flg_value;

                    Int32.TryParse(words[1], out par_addr_int);

                    switch (par_function_int)
                    {
                        case 1:
                            Int32.TryParse(words[2], out par_val_int);
                            coil_set(par_addr_int, par_val_int);
                            break;

                        case 2:
                            di_set(par_addr_int);
                            break;

                        case 3:
                            Int32.TryParse(words[3], out par_dim_value_int);
                            Int32.TryParse(words[4], out par_pos_value);
                            Int32.TryParse(words[5], out par_len_value);
                            Int32.TryParse(words[5], out par_flg_value);

                            hr_set(par_addr_int, words[2], par_dim_value_int, par_pos_value, par_len_value, par_flg_value);
                            break;

                        case 4:
                            Int32.TryParse(words[3], out par_dim_value_int);
                            Int32.TryParse(words[4], out par_pos_value);
                            Int32.TryParse(words[5], out par_len_value);
                            Int32.TryParse(words[5], out par_flg_value);

                            ir_set(par_addr_int, words[2], par_dim_value_int, par_pos_value, par_len_value, par_flg_value);
                            break;

                        default:
                            MessageBoxInfo("Unknow function in " + test_file, "ERROR");
                            break;

                    }


                    double cts = (new TimeSpan(DateTime.Now.Ticks)).TotalMilliseconds;
                    start_time = cts + IterationDelay;
                    do
                    {
                        cts = (new TimeSpan(DateTime.Now.Ticks)).TotalMilliseconds;
                        Application.DoEvents();
                    } while (cts < start_time);

                }

                textBox_AutoCmdIteration.ForeColor = Color.Black;
                textBox_AutoCmdIteration.Text = AutoCmdIteration.ToString();

            }


        }


        void coil_set(Int32 coil_addr, Int32 coil_val) 
        {
            int value;
            var cbor = CBORObject.NewMap();

            cbor.Add(@"ver", CBOR_PAYLOAD_VER);
            cbor.Add(@"rto", textBox_Target.Text + @"/coil_w_val");
            value = 7;
            cbor.Add(@"cmd", value);
            cbor.Add(@"ali", textBox_Alias_Coil.Text);

            value = 0;
            cbor.Add(@"fun", 5);
            cbor.Add(@"adr", coil_addr);
            cbor.Add(@"dim", 1);
            cbor.Add(@"pos", 1);
            cbor.Add(@"len", 1);
            cbor.Add(@"a", @"1.0");
            cbor.Add(@"b", @"1.0");
            cbor.Add("flg", 0);
            cbor.Add(@"val", coil_val.ToString());

            // The following converts the map to canonical CBOR
            byte[] cbor_bytes = cbor.EncodeToBytes(CBOREncodeOptions.DefaultCtap2Canonical);

            try
            {
                ushort msgId = client.Publish(val_req_post, cbor_bytes, qos_selected(), false);
            }
            catch (System.NullReferenceException)
            {
                MessageBoxUpdated("MQTT Not Initialized");
            }

        }
                
        void hr_set(int addr_val, string set_val, int dim_value, int pos_value, int len_value, int flg_value)
        {
            int value;            
            var cbor = CBORObject.NewMap();
            int func_num;

            cbor.Add(@"ver", CBOR_PAYLOAD_VER);
            cbor.Add(@"rto", textBox_Target.Text + @"/hr_w_val");
            cbor.Add(@"cmd", 7);
            cbor.Add(@"ali", textBox_Alias.Text);
            cbor.Add(@"dim", dim_value);

            if (dim_value == 32) func_num = 16;            
            else func_num = 6;
            
            cbor.Add(@"fun", func_num);
            cbor.Add(@"adr", addr_val);
            cbor.Add(@"pos", pos_value);
            cbor.Add(@"len", len_value);
            cbor.Add(@"a", "1.0");
            cbor.Add(@"b", "0.0");
            cbor.Add("flg", flg_value);
            cbor.Add(@"val", set_val);

            // The following converts the map to canonical CBOR
            byte[] cbor_bytes = cbor.EncodeToBytes(CBOREncodeOptions.DefaultCtap2Canonical);

            try
            {
                ushort msgId = client.Publish(val_req_post, cbor_bytes, qos_selected(), false);
            }
            catch (System.NullReferenceException)
            {
                MessageBoxUpdated("MQTT Not Initialized");
            }
        }

        void di_set(Int32 di_addr)
        {
            var cbor = CBORObject.NewMap();

            cbor.Add(@"ver", CBOR_PAYLOAD_VER);
            cbor.Add(@"rto", textBox_Target.Text + @"/di_r_val");
            cbor.Add(@"cmd", 6);
            cbor.Add(@"ali", textBox_Alias_DI.Text);
            cbor.Add(@"fun", 2);
            cbor.Add(@"adr", di_addr);
            cbor.Add(@"dim", 1);
            cbor.Add(@"pos", 1);
            cbor.Add(@"len", 1);
            cbor.Add(@"a", @"1.0");
            cbor.Add(@"b", @"1.0");
            cbor.Add("flg", 0);

            // The following converts the map to canonical CBOR
            byte[] cbor_bytes = cbor.EncodeToBytes(CBOREncodeOptions.DefaultCtap2Canonical);

            try
            {
                ushort msgId = client.Publish(val_req_post, cbor_bytes, qos_selected(), false);

            }
            catch (System.NullReferenceException)
            {
                MessageBoxUpdated("MQTT Not Initialized");
            }
        }

        void ir_set(int addr_val, string set_val, int dim_value, int pos_value, int len_value, int flg_value)
        {
            int value;
            var cbor = CBORObject.NewMap();
            int func_num;

            cbor.Add(@"ver", CBOR_PAYLOAD_VER);
            cbor.Add(@"rto", textBox_Target.Text + @"/hr_w_val");
            cbor.Add(@"cmd", 6);
            cbor.Add(@"ali", textBox_Alias.Text);
            cbor.Add(@"dim", dim_value);

            if (dim_value == 32) func_num = 16;
            else func_num = 6;

            cbor.Add(@"fun", func_num);
            cbor.Add(@"adr", addr_val);
            cbor.Add(@"pos", pos_value);
            cbor.Add(@"len", len_value);
            cbor.Add(@"a", "1.0");
            cbor.Add(@"b", "0.0");
            cbor.Add("flg", flg_value);
            cbor.Add(@"val", set_val);

            // The following converts the map to canonical CBOR
            byte[] cbor_bytes = cbor.EncodeToBytes(CBOREncodeOptions.DefaultCtap2Canonical);

            try
            {
                ushort msgId = client.Publish(val_req_post, cbor_bytes, qos_selected(), false);
            }
            catch (System.NullReferenceException)
            {
                MessageBoxUpdated("MQTT Not Initialized");
            }
        }





    }


}