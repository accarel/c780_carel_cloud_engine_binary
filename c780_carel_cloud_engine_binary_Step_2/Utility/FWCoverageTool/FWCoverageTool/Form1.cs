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
using System.Runtime.InteropServices;



namespace FWCoverageTool
{
    public partial class Form1 : Form
    {
        private static int EM_LINEINDEX = 0xbb;

        [DllImport("user32.dll")]
        extern static int SendMessage(IntPtr hwnd, int message, int wparam, int lparam);

        const string CRLF = "\r\n";

        List<string> CovRowFound = new List<string>();

        List<UInt16> CovRowFound_num = new List<UInt16>();

        List<UInt16> CovP_COVRowFound_num = new List<UInt16>();

        List<string> FilesInSrcDir = new List<string>();


        public Form1()
        {
            InitializeComponent();
        }

        private void AnalyzeFiles()
        {
            try
            {
                // Set a variable to the My Documents path.
                string docPath = textBox_Src_Dir.Text;
                //Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);

                var allfiles = Directory.EnumerateFiles(docPath, "*.c", SearchOption.TopDirectoryOnly);

                foreach (var file in allfiles)
                {
                    FilesInSrcDir.Add(file.ToString());
                    string s = Path.GetFileName(file.ToString()) + CRLF;
                    //textBox_Dir_list.AppendText(s);
                    richTextBox_Dir_list.AppendText(s);
                }

            }
            catch (UnauthorizedAccessException uAEx)
            {
                textBox_Info_Errors.AppendText(uAEx.Message);
            }
            catch (PathTooLongException pathEx)
            {
                textBox_Info_Errors.AppendText(pathEx.Message);
            }
            //catch 
            //{ 
            //}
        }

        private string get_fname(string s_line)
        {
            string[] s_line_subs = s_line.Split('/');
            int val = s_line_subs.Count() - 1;
            return s_line_subs[val];
        }

        private void AnalyzeCoverageReport(string check_fname)
        {
            try
            {
                // foreach (var line in lines) 
                foreach (string line in File.ReadLines(textBox_base_dir.Text))
                {
                    if (line.Contains("!#!") && line.Contains(check_fname))
                    {
                        textBox_Info_Errors.AppendText(line.ToString() + CRLF);

                        string[] subs = line.Split('|');

                        //subs[0]; "#!#"  
                        string fname = get_fname(subs[1]);
                        //textBox_dump.AppendText(fname + CRLF);

                        //line number  subs[1]
                        try
                        {
                            CovRowFound.Add(subs[2]);
                        }
                        catch
                        {
                            //do nothing
                        }

                        //subs[3]  value if exist
                    }

                }


            }
            catch (UnauthorizedAccessException uAEx)
            {
                textBox_Info_Errors.AppendText(uAEx.Message);
            }
            catch (PathTooLongException pathEx)
            {
                textBox_Info_Errors.AppendText(pathEx.Message);
            }


            foreach (var crow in CovRowFound)
            {
                try
                {
                    UInt16 val = UInt16.Parse(crow);
                    CovRowFound_num.Add(val);
                }
                catch
                {
                }
            }

            CovRowFound_num.Sort();

        }

        private void LoadFileForCoverage(string fname)
        {
            bool found;

            try
            {
                ushort linecount = 0;
                // foreach (var line in lines) 
                foreach (string line in File.ReadLines(fname))
                {
                    linecount++;
                    

                    found = false;

                    foreach (UInt16 aaa in CovRowFound_num)
                    {
                        if (aaa == linecount)
                        {
                            found = true;
                        }
                    }

                    richTextBox_coverage.SelectionBackColor = Color.White;

                    if (line.Contains("P_COV"))
                    {
                        richTextBox_coverage.SelectionBackColor = Color.Red;
                    }

                    if (found == true)
                    {
                        //richTextBox_coverage.Select(index, word.Length);
                        richTextBox_coverage.SelectionBackColor = Color.Green;
                    }


                    richTextBox_coverage.AppendText(linecount.ToString() + " " + line + CRLF);

                }

            }
            catch (UnauthorizedAccessException uAEx)
            {
                textBox_Info_Errors.AppendText(uAEx.Message);
            }
            catch (PathTooLongException pathEx)
            {
                textBox_Info_Errors.AppendText(pathEx.Message);
            }

            // CovP_COVRowFound_num.Sort();

        }

        public void HighlightText(RichTextBox myRtb, string word, Color color)
        {

            if (word == string.Empty)
                return;

            int s_start = myRtb.SelectionStart, startIndex = 0, index;

            while ((index = myRtb.Text.IndexOf(word, startIndex)) != -1)
            {
                myRtb.Select(index, word.Length);
                //myRtb.SelectionColor = color;
                myRtb.SelectionBackColor = color;

                startIndex = index + word.Length;
            }

            myRtb.SelectionStart = s_start;
            myRtb.SelectionLength = 0;
            myRtb.SelectionColor = Color.Black;
        }

        public void HighlightText_Cov(RichTextBox myRtb, string word, Color color)
        {
            if (word == string.Empty)
                return;

            int s_start = myRtb.SelectionStart, startIndex = 0, index;

            bool found;
            while ((index = myRtb.Text.IndexOf(word, startIndex)) != -1)
            {
                found = false;

                foreach (UInt16 aaa in CovRowFound_num) 
                {
                    if (aaa == index)
                    {
                        found = true;
                    }
                }

                if (found == true) 
                {
                    myRtb.Select(index, word.Length);
                    myRtb.SelectionBackColor = color;
                }

                startIndex = index + word.Length;
            }

            myRtb.SelectionStart = s_start;
            myRtb.SelectionLength = 0;
            myRtb.SelectionColor = Color.Black;



        }

        private void ShowCoverage(string fullpath)
        {
            richTextBox_coverage.Text = "";

            string fname;

            fname = Path.GetFileName(fullpath);

            AnalyzeCoverageReport(fname);

            LoadFileForCoverage(fullpath);
        }

        private void button_analyze_src_Click(object sender, EventArgs e)
        {
            //ShowCoverage("main_CAREL.c");
            AnalyzeFiles();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void Form1_CursorChanged(object sender, EventArgs e)
        {

        }

        private void richTextBox_coverage_CursorChanged(object sender, EventArgs e)
        {
            update_coords();
        }

        private void richTextBox_coverage_MouseClick(object sender, MouseEventArgs e)
        {
            update_coords();
        }

        private void update_coords()
        {
            //int caretPos = richTextBox_coverage.SelectionStart;
            //label_Row.Text = caretPos.ToString();
        }

        private void UpdateCaretPos()
        {
            int line, col, index;
            index = richTextBox_coverage.SelectionStart;
            line = richTextBox_coverage.GetLineFromCharIndex(index);
            col = index - SendMessage(richTextBox_coverage.Handle, EM_LINEINDEX, -1, 0);
            label_Row.Text = (++line).ToString(); 
            label_Col.Text = (++col).ToString();
        }

        private void richTextBox_coverage_MouseDown(object sender, MouseEventArgs e)
        {
            UpdateCaretPos();
        }

        private void richTextBox_coverage_TextChanged(object sender, EventArgs e)
        {
            UpdateCaretPos();
        }

        private void richTextBox_coverage_KeyDown(object sender, KeyEventArgs e)
        {
            UpdateCaretPos();
        }

        private void richTextBox_coverage_KeyUp(object sender, KeyEventArgs e)
        {
            UpdateCaretPos();
        }

        private void textBox_Dir_list_MouseClick(object sender, MouseEventArgs e)
        {
          
        }

        private void richTextBox_Dir_list_MouseClick(object sender, MouseEventArgs e)
        {
            int sel_start = richTextBox_Dir_list.SelectionStart;
            int idx = richTextBox_Dir_list.GetLineFromCharIndex(sel_start);

            if (idx < FilesInSrcDir.Count)
            {
                label_src_name.Text = FilesInSrcDir[idx];
                CovP_COVRowFound_num.Clear();
                CovRowFound_num.Clear();
                CovRowFound.Clear();

                richTextBox_Dir_list.Text = "";
                ShowCoverage(label_src_name.Text);
                AnalyzeFiles();

                string s = Path.GetFileName(label_src_name.Text);
                HighlightText(richTextBox_Dir_list, s, Color.Blue);
            }


        }
    }

}
