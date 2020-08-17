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
            this.SuspendLayout();
            // 
            // button_select_file
            // 
            this.button_select_file.Location = new System.Drawing.Point(12, 25);
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
            this.textBox_selected_file.Size = new System.Drawing.Size(895, 22);
            this.textBox_selected_file.TabIndex = 1;
            // 
            // openFileDialog2
            // 
            this.openFileDialog2.FileName = "openFileDialog2";
            this.openFileDialog2.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog2_FileOk);
            // 
            // button_analyze
            // 
            this.button_analyze.Location = new System.Drawing.Point(12, 66);
            this.button_analyze.Name = "button_analyze";
            this.button_analyze.Size = new System.Drawing.Size(81, 35);
            this.button_analyze.TabIndex = 2;
            this.button_analyze.Text = "Analyze";
            this.button_analyze.UseVisualStyleBackColor = true;
            this.button_analyze.Click += new System.EventHandler(this.button_analyze_Click);
            // 
            // textBox_UTC2local_time
            // 
            this.textBox_UTC2local_time.Location = new System.Drawing.Point(12, 124);
            this.textBox_UTC2local_time.Name = "textBox_UTC2local_time";
            this.textBox_UTC2local_time.Size = new System.Drawing.Size(191, 22);
            this.textBox_UTC2local_time.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 104);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(67, 17);
            this.label1.TabIndex = 4;
            this.label1.Text = "Last UTC";
            // 
            // textBox_info
            // 
            this.textBox_info.Location = new System.Drawing.Point(15, 632);
            this.textBox_info.Multiline = true;
            this.textBox_info.Name = "textBox_info";
            this.textBox_info.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox_info.Size = new System.Drawing.Size(978, 77);
            this.textBox_info.TabIndex = 5;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(16, 612);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(31, 17);
            this.label2.TabIndex = 6;
            this.label2.Text = "Info";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(800, 93);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(57, 17);
            this.label3.TabIndex = 8;
            this.label3.Text = "sta time";
            // 
            // richTextBox_sta_time
            // 
            this.richTextBox_sta_time.Location = new System.Drawing.Point(803, 113);
            this.richTextBox_sta_time.Name = "richTextBox_sta_time";
            this.richTextBox_sta_time.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.richTextBox_sta_time.Size = new System.Drawing.Size(191, 513);
            this.richTextBox_sta_time.TabIndex = 9;
            this.richTextBox_sta_time.Text = "";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 149);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(59, 17);
            this.label4.TabIndex = 10;
            this.label4.Text = "Last upt";
            // 
            // textBox_upt
            // 
            this.textBox_upt.Location = new System.Drawing.Point(12, 169);
            this.textBox_upt.Name = "textBox_upt";
            this.textBox_upt.Size = new System.Drawing.Size(92, 22);
            this.textBox_upt.TabIndex = 11;
            // 
            // textBox_last_upt_days
            // 
            this.textBox_last_upt_days.Location = new System.Drawing.Point(110, 169);
            this.textBox_last_upt_days.Name = "textBox_last_upt_days";
            this.textBox_last_upt_days.Size = new System.Drawing.Size(93, 22);
            this.textBox_last_upt_days.TabIndex = 12;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(107, 149);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(40, 17);
            this.label5.TabIndex = 13;
            this.label5.Text = "Days";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(12, 198);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(58, 17);
            this.label6.TabIndex = 14;
            this.label6.Text = "Last est";
            // 
            // textBox_last_est
            // 
            this.textBox_last_est.Location = new System.Drawing.Point(12, 218);
            this.textBox_last_est.Name = "textBox_last_est";
            this.textBox_last_est.Size = new System.Drawing.Size(74, 22);
            this.textBox_last_est.TabIndex = 15;
            // 
            // richTextBox_cmd_5
            // 
            this.richTextBox_cmd_5.Location = new System.Drawing.Point(406, 268);
            this.richTextBox_cmd_5.Name = "richTextBox_cmd_5";
            this.richTextBox_cmd_5.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.richTextBox_cmd_5.Size = new System.Drawing.Size(191, 53);
            this.richTextBox_cmd_5.TabIndex = 16;
            this.richTextBox_cmd_5.Text = "";
            // 
            // richTextBox_cmd_1
            // 
            this.richTextBox_cmd_1.Location = new System.Drawing.Point(12, 268);
            this.richTextBox_cmd_1.Name = "richTextBox_cmd_1";
            this.richTextBox_cmd_1.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.richTextBox_cmd_1.Size = new System.Drawing.Size(191, 56);
            this.richTextBox_cmd_1.TabIndex = 17;
            this.richTextBox_cmd_1.Text = "";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(406, 248);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(175, 17);
            this.label7.TabIndex = 18;
            this.label7.Text = "CMD 5 - dwnl_devs_config";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(12, 248);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(153, 17);
            this.label8.TabIndex = 19;
            this.label8.Text = "CMD 1 - set_gw_config";
            // 
            // richTextBox_cmd_4
            // 
            this.richTextBox_cmd_4.Location = new System.Drawing.Point(209, 268);
            this.richTextBox_cmd_4.Name = "richTextBox_cmd_4";
            this.richTextBox_cmd_4.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.richTextBox_cmd_4.Size = new System.Drawing.Size(191, 56);
            this.richTextBox_cmd_4.TabIndex = 20;
            this.richTextBox_cmd_4.Text = "";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(206, 248);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(165, 17);
            this.label9.TabIndex = 21;
            this.label9.Text = "CMD 4 - set_lines_config";
            // 
            // textBox_lss
            // 
            this.textBox_lss.Location = new System.Drawing.Point(12, 414);
            this.textBox_lss.Name = "textBox_lss";
            this.textBox_lss.Size = new System.Drawing.Size(74, 22);
            this.textBox_lss.TabIndex = 22;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(12, 394);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(25, 17);
            this.label10.TabIndex = 23;
            this.label10.Text = "lss";
            // 
            // textBox_hss
            // 
            this.textBox_hss.Location = new System.Drawing.Point(99, 414);
            this.textBox_hss.Name = "textBox_hss";
            this.textBox_hss.Size = new System.Drawing.Size(74, 22);
            this.textBox_hss.TabIndex = 24;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(96, 394);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(30, 17);
            this.label11.TabIndex = 25;
            this.label11.Text = "hss";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(9, 343);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(31, 17);
            this.label12.TabIndex = 26;
            this.label12.Text = "pva";
            // 
            // textBox_pva
            // 
            this.textBox_pva.Location = new System.Drawing.Point(12, 363);
            this.textBox_pva.Name = "textBox_pva";
            this.textBox_pva.Size = new System.Drawing.Size(74, 22);
            this.textBox_pva.TabIndex = 27;
            // 
            // textBox_pst
            // 
            this.textBox_pst.Location = new System.Drawing.Point(99, 363);
            this.textBox_pst.Name = "textBox_pst";
            this.textBox_pst.Size = new System.Drawing.Size(74, 22);
            this.textBox_pst.TabIndex = 28;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(96, 343);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(27, 17);
            this.label13.TabIndex = 29;
            this.label13.Text = "pst";
            // 
            // textBox_mka
            // 
            this.textBox_mka.Location = new System.Drawing.Point(12, 459);
            this.textBox_mka.Name = "textBox_mka";
            this.textBox_mka.Size = new System.Drawing.Size(74, 22);
            this.textBox_mka.TabIndex = 30;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(9, 439);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(34, 17);
            this.label14.TabIndex = 31;
            this.label14.Text = "mka";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1006, 721);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.textBox_mka);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.textBox_pst);
            this.Controls.Add(this.textBox_pva);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.textBox_hss);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.textBox_lss);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.richTextBox_cmd_4);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.richTextBox_cmd_1);
            this.Controls.Add(this.richTextBox_cmd_5);
            this.Controls.Add(this.textBox_last_est);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.textBox_last_upt_days);
            this.Controls.Add(this.textBox_upt);
            this.Controls.Add(this.label4);
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
            this.Text = "GME Log Analizer";
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
    }
}

