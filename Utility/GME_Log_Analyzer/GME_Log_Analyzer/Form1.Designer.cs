namespace GME_Log_Analyzer
{
    partial class Form1
    {
        /// <summary>
        /// Variabile di progettazione necessaria.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Pulire le risorse in uso.
        /// </summary>
        /// <param name="disposing">ha valore true se le risorse gestite devono essere eliminate, false in caso contrario.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Codice generato da Progettazione Windows Form

        /// <summary>
        /// Metodo necessario per il supporto della finestra di progettazione. Non modificare
        /// il contenuto del metodo con l'editor di codice.
        /// </summary>
        private void InitializeComponent()
        {
            this.button_select_file = new System.Windows.Forms.Button();
            this.textBox_selected_file = new System.Windows.Forms.TextBox();
            this.openFileDialog2 = new System.Windows.Forms.OpenFileDialog();
            this.button_analyze = new System.Windows.Forms.Button();
            this.textBox_UTC2local_time = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.textBox_info = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.richTextBox_sta_time = new System.Windows.Forms.RichTextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textBox_upt = new System.Windows.Forms.TextBox();
            this.textBox_last_upt_days = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.textBox_last_est = new System.Windows.Forms.TextBox();
            this.richTextBox_cmd_5 = new System.Windows.Forms.RichTextBox();
            this.richTextBox_cmd_1 = new System.Windows.Forms.RichTextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.richTextBox_cmd_4 = new System.Windows.Forms.RichTextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.textBox_lss = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.textBox_hss = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.textBox_pva = new System.Windows.Forms.TextBox();
            this.textBox_pst = new System.Windows.Forms.TextBox();
            this.label13 = new System.Windows.Forms.Label();
            this.textBox_mka = new System.Windows.Forms.TextBox();
            this.label14 = new System.Windows.Forms.Label();
            this.textBox_pn = new System.Windows.Forms.TextBox();
            this.label15 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.textBox_hwv = new System.Windows.Forms.TextBox();
            this.textBox_fwv = new System.Windows.Forms.TextBox();
            this.label17 = new System.Windows.Forms.Label();
            this.textBox_bau = new System.Windows.Forms.TextBox();
            this.label18 = new System.Windows.Forms.Label();
            this.textBox_last_cid = new System.Windows.Forms.TextBox();
            this.label19 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.richTextBox_cmd_13 = new System.Windows.Forms.RichTextBox();
            this.label20 = new System.Windows.Forms.Label();
            this.richTextBox_other = new System.Windows.Forms.RichTextBox();
            this.label21 = new System.Windows.Forms.Label();
            this.label22 = new System.Windows.Forms.Label();
            this.richTextBox_cmd_10_upg_dbg_fw = new System.Windows.Forms.RichTextBox();
            this.label23 = new System.Windows.Forms.Label();
            this.richTextBox_cmd_11_upg_dev_fw = new System.Windows.Forms.RichTextBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // button_select_file
            // 
            this.button_select_file.Location = new System.Drawing.Point(12, 18);
            this.button_select_file.Name = "button_select_file";
            this.button_select_file.Size = new System.Drawing.Size(81, 35);
            this.button_select_file.TabIndex = 0;
            this.button_select_file.Text = "Select File";
            this.button_select_file.UseVisualStyleBackColor = true;
            this.button_select_file.Click += new System.EventHandler(this.button_select_file_Click);
            // 
            // textBox_selected_file
            // 
            this.textBox_selected_file.Location = new System.Drawing.Point(99, 31);
            this.textBox_selected_file.Name = "textBox_selected_file";
            this.textBox_selected_file.Size = new System.Drawing.Size(808, 22);
            this.textBox_selected_file.TabIndex = 1;
            // 
            // openFileDialog2
            // 
            this.openFileDialog2.FileName = "openFileDialog2";
            this.openFileDialog2.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog2_FileOk);
            // 
            // button_analyze
            // 
            this.button_analyze.Location = new System.Drawing.Point(913, 18);
            this.button_analyze.Name = "button_analyze";
            this.button_analyze.Size = new System.Drawing.Size(81, 35);
            this.button_analyze.TabIndex = 2;
            this.button_analyze.Text = "Analyze";
            this.button_analyze.UseVisualStyleBackColor = true;
            this.button_analyze.Click += new System.EventHandler(this.button_analyze_Click);
            // 
            // textBox_UTC2local_time
            // 
            this.textBox_UTC2local_time.Location = new System.Drawing.Point(1059, 29);
            this.textBox_UTC2local_time.Name = "textBox_UTC2local_time";
            this.textBox_UTC2local_time.Size = new System.Drawing.Size(191, 22);
            this.textBox_UTC2local_time.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(1056, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(97, 17);
            this.label1.TabIndex = 4;
            this.label1.Text = "Last msg UTC";
            // 
            // textBox_info
            // 
            this.textBox_info.Location = new System.Drawing.Point(15, 666);
            this.textBox_info.Multiline = true;
            this.textBox_info.Name = "textBox_info";
            this.textBox_info.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox_info.Size = new System.Drawing.Size(413, 43);
            this.textBox_info.TabIndex = 5;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(18, 646);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(31, 17);
            this.label2.TabIndex = 6;
            this.label2.Text = "Info";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(1056, 54);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(57, 17);
            this.label3.TabIndex = 8;
            this.label3.Text = "sta time";
            // 
            // richTextBox_sta_time
            // 
            this.richTextBox_sta_time.Location = new System.Drawing.Point(1059, 81);
            this.richTextBox_sta_time.Name = "richTextBox_sta_time";
            this.richTextBox_sta_time.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.richTextBox_sta_time.Size = new System.Drawing.Size(191, 545);
            this.richTextBox_sta_time.TabIndex = 9;
            this.richTextBox_sta_time.Text = "";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(7, 18);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(59, 17);
            this.label4.TabIndex = 10;
            this.label4.Text = "Last upt";
            // 
            // textBox_upt
            // 
            this.textBox_upt.Location = new System.Drawing.Point(8, 38);
            this.textBox_upt.Name = "textBox_upt";
            this.textBox_upt.Size = new System.Drawing.Size(92, 22);
            this.textBox_upt.TabIndex = 11;
            // 
            // textBox_last_upt_days
            // 
            this.textBox_last_upt_days.Location = new System.Drawing.Point(106, 38);
            this.textBox_last_upt_days.Name = "textBox_last_upt_days";
            this.textBox_last_upt_days.Size = new System.Drawing.Size(93, 22);
            this.textBox_last_upt_days.TabIndex = 12;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(103, 18);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(40, 17);
            this.label5.TabIndex = 13;
            this.label5.Text = "Days";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(8, 61);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(58, 17);
            this.label6.TabIndex = 14;
            this.label6.Text = "Last est";
            // 
            // textBox_last_est
            // 
            this.textBox_last_est.Location = new System.Drawing.Point(8, 81);
            this.textBox_last_est.Name = "textBox_last_est";
            this.textBox_last_est.Size = new System.Drawing.Size(74, 22);
            this.textBox_last_est.TabIndex = 15;
            // 
            // richTextBox_cmd_5
            // 
            this.richTextBox_cmd_5.Location = new System.Drawing.Point(431, 284);
            this.richTextBox_cmd_5.Name = "richTextBox_cmd_5";
            this.richTextBox_cmd_5.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.richTextBox_cmd_5.Size = new System.Drawing.Size(267, 53);
            this.richTextBox_cmd_5.TabIndex = 16;
            this.richTextBox_cmd_5.Text = "";
            // 
            // richTextBox_cmd_1
            // 
            this.richTextBox_cmd_1.Location = new System.Drawing.Point(6, 38);
            this.richTextBox_cmd_1.Name = "richTextBox_cmd_1";
            this.richTextBox_cmd_1.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.richTextBox_cmd_1.Size = new System.Drawing.Size(199, 56);
            this.richTextBox_cmd_1.TabIndex = 17;
            this.richTextBox_cmd_1.Text = "";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(431, 264);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(175, 17);
            this.label7.TabIndex = 18;
            this.label7.Text = "CMD 5 - dwnl_devs_config";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 18);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(98, 17);
            this.label8.TabIndex = 19;
            this.label8.Text = "set_gw_config";
            // 
            // richTextBox_cmd_4
            // 
            this.richTextBox_cmd_4.Location = new System.Drawing.Point(234, 284);
            this.richTextBox_cmd_4.Name = "richTextBox_cmd_4";
            this.richTextBox_cmd_4.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.richTextBox_cmd_4.Size = new System.Drawing.Size(191, 56);
            this.richTextBox_cmd_4.TabIndex = 20;
            this.richTextBox_cmd_4.Text = "";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(231, 264);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(165, 17);
            this.label9.TabIndex = 21;
            this.label9.Text = "CMD 4 - set_lines_config";
            // 
            // textBox_lss
            // 
            this.textBox_lss.Location = new System.Drawing.Point(9, 167);
            this.textBox_lss.Name = "textBox_lss";
            this.textBox_lss.Size = new System.Drawing.Size(74, 22);
            this.textBox_lss.TabIndex = 22;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(9, 147);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(25, 17);
            this.label10.TabIndex = 23;
            this.label10.Text = "lss";
            // 
            // textBox_hss
            // 
            this.textBox_hss.Location = new System.Drawing.Point(96, 167);
            this.textBox_hss.Name = "textBox_hss";
            this.textBox_hss.Size = new System.Drawing.Size(74, 22);
            this.textBox_hss.TabIndex = 24;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(93, 147);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(30, 17);
            this.label11.TabIndex = 25;
            this.label11.Text = "hss";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(6, 96);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(31, 17);
            this.label12.TabIndex = 26;
            this.label12.Text = "pva";
            // 
            // textBox_pva
            // 
            this.textBox_pva.Location = new System.Drawing.Point(9, 116);
            this.textBox_pva.Name = "textBox_pva";
            this.textBox_pva.Size = new System.Drawing.Size(74, 22);
            this.textBox_pva.TabIndex = 27;
            // 
            // textBox_pst
            // 
            this.textBox_pst.Location = new System.Drawing.Point(96, 116);
            this.textBox_pst.Name = "textBox_pst";
            this.textBox_pst.Size = new System.Drawing.Size(74, 22);
            this.textBox_pst.TabIndex = 28;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(93, 96);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(27, 17);
            this.label13.TabIndex = 29;
            this.label13.Text = "pst";
            // 
            // textBox_mka
            // 
            this.textBox_mka.Location = new System.Drawing.Point(9, 212);
            this.textBox_mka.Name = "textBox_mka";
            this.textBox_mka.Size = new System.Drawing.Size(74, 22);
            this.textBox_mka.TabIndex = 30;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(6, 192);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(34, 17);
            this.label14.TabIndex = 31;
            this.label14.Text = "mka";
            // 
            // textBox_pn
            // 
            this.textBox_pn.Location = new System.Drawing.Point(9, 38);
            this.textBox_pn.Name = "textBox_pn";
            this.textBox_pn.Size = new System.Drawing.Size(191, 22);
            this.textBox_pn.TabIndex = 32;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(6, 18);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(24, 17);
            this.label15.TabIndex = 33;
            this.label15.Text = "pn";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(9, 63);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(32, 17);
            this.label16.TabIndex = 34;
            this.label16.Text = "hwv";
            // 
            // textBox_hwv
            // 
            this.textBox_hwv.Location = new System.Drawing.Point(9, 83);
            this.textBox_hwv.Name = "textBox_hwv";
            this.textBox_hwv.Size = new System.Drawing.Size(74, 22);
            this.textBox_hwv.TabIndex = 35;
            // 
            // textBox_fwv
            // 
            this.textBox_fwv.Location = new System.Drawing.Point(96, 83);
            this.textBox_fwv.Name = "textBox_fwv";
            this.textBox_fwv.Size = new System.Drawing.Size(74, 22);
            this.textBox_fwv.TabIndex = 36;
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(93, 63);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(28, 17);
            this.label17.TabIndex = 37;
            this.label17.Text = "fwv";
            // 
            // textBox_bau
            // 
            this.textBox_bau.Location = new System.Drawing.Point(9, 132);
            this.textBox_bau.Name = "textBox_bau";
            this.textBox_bau.Size = new System.Drawing.Size(74, 22);
            this.textBox_bau.TabIndex = 38;
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(9, 112);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(32, 17);
            this.label18.TabIndex = 39;
            this.label18.Text = "bau";
            // 
            // textBox_last_cid
            // 
            this.textBox_last_cid.Location = new System.Drawing.Point(96, 132);
            this.textBox_last_cid.Name = "textBox_last_cid";
            this.textBox_last_cid.Size = new System.Drawing.Size(74, 22);
            this.textBox_last_cid.TabIndex = 40;
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(93, 112);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(26, 17);
            this.label19.TabIndex = 41;
            this.label19.Text = "cid";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.textBox_fwv);
            this.groupBox1.Controls.Add(this.label19);
            this.groupBox1.Controls.Add(this.textBox_pn);
            this.groupBox1.Controls.Add(this.textBox_last_cid);
            this.groupBox1.Controls.Add(this.label15);
            this.groupBox1.Controls.Add(this.label18);
            this.groupBox1.Controls.Add(this.label16);
            this.groupBox1.Controls.Add(this.textBox_bau);
            this.groupBox1.Controls.Add(this.textBox_hwv);
            this.groupBox1.Controls.Add(this.label17);
            this.groupBox1.Location = new System.Drawing.Point(12, 81);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(211, 162);
            this.groupBox1.TabIndex = 42;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Last Hello";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.textBox_last_upt_days);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.textBox_upt);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.textBox_last_est);
            this.groupBox2.Location = new System.Drawing.Point(229, 81);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(207, 162);
            this.groupBox2.TabIndex = 43;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Last Status";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.richTextBox_cmd_1);
            this.groupBox3.Controls.Add(this.label8);
            this.groupBox3.Controls.Add(this.textBox_lss);
            this.groupBox3.Controls.Add(this.label14);
            this.groupBox3.Controls.Add(this.label10);
            this.groupBox3.Controls.Add(this.textBox_mka);
            this.groupBox3.Controls.Add(this.textBox_hss);
            this.groupBox3.Controls.Add(this.label13);
            this.groupBox3.Controls.Add(this.label11);
            this.groupBox3.Controls.Add(this.textBox_pst);
            this.groupBox3.Controls.Add(this.label12);
            this.groupBox3.Controls.Add(this.textBox_pva);
            this.groupBox3.Location = new System.Drawing.Point(12, 249);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(211, 242);
            this.groupBox3.TabIndex = 44;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "CMD 1";
            // 
            // richTextBox_cmd_13
            // 
            this.richTextBox_cmd_13.Location = new System.Drawing.Point(234, 365);
            this.richTextBox_cmd_13.Name = "richTextBox_cmd_13";
            this.richTextBox_cmd_13.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.richTextBox_cmd_13.Size = new System.Drawing.Size(191, 56);
            this.richTextBox_cmd_13.TabIndex = 45;
            this.richTextBox_cmd_13.Text = "";
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(231, 345);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(142, 17);
            this.label20.TabIndex = 46;
            this.label20.Text = "CMD 13 - upd ca cert";
            // 
            // richTextBox_other
            // 
            this.richTextBox_other.Location = new System.Drawing.Point(776, 365);
            this.richTextBox_other.Name = "richTextBox_other";
            this.richTextBox_other.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.richTextBox_other.Size = new System.Drawing.Size(267, 261);
            this.richTextBox_other.TabIndex = 47;
            this.richTextBox_other.Text = "";
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(773, 345);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(75, 17);
            this.label21.TabIndex = 48;
            this.label21.Text = "CMD other";
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(231, 430);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(133, 17);
            this.label22.TabIndex = 50;
            this.label22.Text = "CMD 10 - upd gw fw";
            // 
            // richTextBox_cmd_10_upg_dbg_fw
            // 
            this.richTextBox_cmd_10_upg_dbg_fw.Location = new System.Drawing.Point(234, 450);
            this.richTextBox_cmd_10_upg_dbg_fw.Name = "richTextBox_cmd_10_upg_dbg_fw";
            this.richTextBox_cmd_10_upg_dbg_fw.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.richTextBox_cmd_10_upg_dbg_fw.Size = new System.Drawing.Size(191, 56);
            this.richTextBox_cmd_10_upg_dbg_fw.TabIndex = 49;
            this.richTextBox_cmd_10_upg_dbg_fw.Text = "";
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(231, 513);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(139, 17);
            this.label23.TabIndex = 52;
            this.label23.Text = "CMD 11 - upd dev fw";
            // 
            // richTextBox_cmd_11_upg_dev_fw
            // 
            this.richTextBox_cmd_11_upg_dev_fw.Location = new System.Drawing.Point(234, 533);
            this.richTextBox_cmd_11_upg_dev_fw.Name = "richTextBox_cmd_11_upg_dev_fw";
            this.richTextBox_cmd_11_upg_dev_fw.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.richTextBox_cmd_11_upg_dev_fw.Size = new System.Drawing.Size(191, 56);
            this.richTextBox_cmd_11_upg_dev_fw.TabIndex = 51;
            this.richTextBox_cmd_11_upg_dev_fw.Text = "";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1262, 721);
            this.Controls.Add(this.label23);
            this.Controls.Add(this.richTextBox_cmd_11_upg_dev_fw);
            this.Controls.Add(this.label22);
            this.Controls.Add(this.richTextBox_cmd_10_upg_dbg_fw);
            this.Controls.Add(this.label21);
            this.Controls.Add(this.richTextBox_other);
            this.Controls.Add(this.label20);
            this.Controls.Add(this.richTextBox_cmd_13);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.richTextBox_cmd_4);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.richTextBox_cmd_5);
            this.Controls.Add(this.richTextBox_sta_time);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textBox_info);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBox_UTC2local_time);
            this.Controls.Add(this.button_analyze);
            this.Controls.Add(this.textBox_selected_file);
            this.Controls.Add(this.button_select_file);
            this.Name = "Form1";
            this.Text = "GME Log Analizer V.1.0";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button_select_file;
        private System.Windows.Forms.TextBox textBox_selected_file;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.OpenFileDialog openFileDialog2;
        private System.Windows.Forms.Button button_analyze;
        private System.Windows.Forms.TextBox textBox_UTC2local_time;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBox_info;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.RichTextBox richTextBox_sta_time;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBox_upt;
        private System.Windows.Forms.TextBox textBox_last_upt_days;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBox_last_est;
        private System.Windows.Forms.RichTextBox richTextBox_cmd_5;
        private System.Windows.Forms.RichTextBox richTextBox_cmd_1;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.RichTextBox richTextBox_cmd_4;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox textBox_lss;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox textBox_hss;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TextBox textBox_pva;
        private System.Windows.Forms.TextBox textBox_pst;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.TextBox textBox_mka;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TextBox textBox_pn;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.TextBox textBox_hwv;
        private System.Windows.Forms.TextBox textBox_fwv;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.TextBox textBox_bau;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.TextBox textBox_last_cid;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.RichTextBox richTextBox_cmd_13;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.RichTextBox richTextBox_other;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.RichTextBox richTextBox_cmd_10_upg_dbg_fw;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.RichTextBox richTextBox_cmd_11_upg_dev_fw;
    }
}

