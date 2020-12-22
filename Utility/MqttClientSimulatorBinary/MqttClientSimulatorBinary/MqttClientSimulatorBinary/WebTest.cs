using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.IO;
using System.Threading;


namespace MqttClientSimulatorBinary
{
    public partial class WebTest : Form
    {
        public WebTest()
        {
            InitializeComponent();
        }


        public long GetFileSize(string url)
        {
            long result = -1;

            String username = "test";
            String password = "password";
            String encoded = System.Convert.ToBase64String(System.Text.Encoding.GetEncoding("ISO-8859-1").GetBytes(username + ":" + password));


            CookieContainer myContainer = new CookieContainer();
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);

            request.Credentials = System.Net.CredentialCache.DefaultCredentials;
            request.Credentials = new NetworkCredential(@"test", @"password");
            request.CookieContainer = myContainer;
            request.PreAuthenticate = true;
            request.ServerCertificateValidationCallback += (sender, certificate, chain, sslPolicyErrors) => true;

            HttpWebResponse response = (HttpWebResponse)request.GetResponse();

            // Display the status.
            textBox_webtest_result.Text = ((System.Net.HttpWebResponse)response).StatusDescription;

            if (long.TryParse(response.Headers.Get("Content-Length"), out long ContentLength))
            {
                result = ContentLength;
            }

            // Close the response.
            response.Close();

            return result;
        }


        private void button_get_fsize_Click(object sender, EventArgs e)
        {
            long file_size;

            file_size = GetFileSize(textBox_web_test_url.Text);
            textBox_webtest_result.Text += "\r\n";
            textBox_webtest_result.Text += "Size : " + file_size.ToString() +  "\r\n";
        }

        private void WebTest_Load(object sender, EventArgs e)
        {
            this.CenterToScreen();
        }




        public void test_download() 
        {
            double start_time, stop_time;

            start_time = (new TimeSpan(DateTime.Now.Ticks)).TotalMilliseconds;

            CookieContainer myContainer = new CookieContainer();

            // Construct HTTP request to get the logo
            HttpWebRequest httpRequest = (HttpWebRequest)WebRequest.Create(textBox_web_test_url.Text);
            httpRequest.Method = WebRequestMethods.Http.Get;
            httpRequest.Credentials = System.Net.CredentialCache.DefaultCredentials;
            httpRequest.Credentials = new NetworkCredential(@"test", @"password");
            httpRequest.CookieContainer = myContainer;
            httpRequest.PreAuthenticate = true;
            httpRequest.ServerCertificateValidationCallback += (sender, certificate, chain, sslPolicyErrors) => true;

            // Get back the HTTP response for web server
            HttpWebResponse httpResponse = (HttpWebResponse)httpRequest.GetResponse();
            Stream httpResponseStream = httpResponse.GetResponseStream();

            // Define buffer and buffer size
            int bufferSize = 200;
            byte[] buffer = new byte[bufferSize];
            int bytesRead = 0;

            // Read from response and write to file
            FileStream fileStream = File.Create("test_download.bin");

            long block_counter = 0;
            int dly_ms;


            if (Int32.TryParse(textBox_dly_value.Text, out dly_ms)) 
            {
                textBox_webtest_result.Text = "";
            }
            else
            {
                dly_ms = 0;
                textBox_dly_value.Text = "0";
            }

            long total_read_bytes = 0;

            try
            {
               
                while ((bytesRead = httpResponseStream.Read(buffer, 0, bufferSize)) != 0)
                {

                    stop_time = (new TimeSpan(DateTime.Now.Ticks)).TotalMilliseconds;
                    double totalMill = stop_time - start_time;

                    block_counter++;
                    total_read_bytes += bytesRead;

                    textBox_webtest_result.AppendText(block_counter.ToString() + " - Byte R : " + bytesRead.ToString() + " - Total :" + total_read_bytes.ToString()
                                                      + " " + totalMill.ToString() + " \r\n");

                    //textBox_webtest_result.Text += block_counter.ToString() + "\r\n";

                    fileStream.Write(buffer, 0, bytesRead);


                    Application.DoEvents();

                    if (dly_ms > 0)
                    {
                        Thread.Sleep(dly_ms);
                        Application.DoEvents();
                    }

                } // end while
            }
            catch(IOException ecc)
            {
                textBox_webtest_result.AppendText("FAIL!\r\n");
            }

            fileStream.Close();

        }



        private void button1_Click(object sender, EventArgs e)
        {
            test_download(); 
        }



    }
}
