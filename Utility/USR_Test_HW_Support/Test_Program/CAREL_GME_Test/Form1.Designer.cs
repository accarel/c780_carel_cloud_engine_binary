namespace CodeProjectSerialComms
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.rtbIncoming = new System.Windows.Forms.RichTextBox();
            this.button_pwr_off = new System.Windows.Forms.Button();
            this.button_help = new System.Windows.Forms.Button();
            this.tabControl_Info_Setup = new System.Windows.Forms.TabControl();
            this.tabPage2_parameters = new System.Windows.Forms.TabPage();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label_MB_Sim_Port = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.labelPrgBaud = new System.Windows.Forms.Label();
            this.textBox_debug = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.labelComPrg = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label_com_name = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.tabPage1_Info = new System.Windows.Forms.TabPage();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.textBox_read_v = new System.Windows.Forms.TextBox();
            this.button_read_v = new System.Windows.Forms.Button();
            this.button_put_in_programming = new System.Windows.Forms.Button();
            this.button_all_off = new System.Windows.Forms.Button();
            this.button_button_off = new System.Windows.Forms.Button();
            this.button_button = new System.Windows.Forms.Button();
            this.button_tp5 = new System.Windows.Forms.Button();
            this.button_tp5_off = new System.Windows.Forms.Button();
            this.button_tp_en_off = new System.Windows.Forms.Button();
            this.button_tp_en = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.buttonTestStart = new System.Windows.Forms.Button();
            this.button_test_result = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.comboBox_DUT_Type = new System.Windows.Forms.ComboBox();
            this.tabControl_Info_Setup.SuspendLayout();
            this.tabPage2_parameters.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.tabPage1_Info.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // rtbIncoming
            // 
            this.rtbIncoming.Location = new System.Drawing.Point(13, 154);
            this.rtbIncoming.Margin = new System.Windows.Forms.Padding(4);
            this.rtbIncoming.Name = "rtbIncoming";
            this.rtbIncoming.Size = new System.Drawing.Size(330, 506);
            this.rtbIncoming.TabIndex = 1;
            this.rtbIncoming.Text = "";
            // 
            // button_pwr_off
            // 
            this.button_pwr_off.Location = new System.Drawing.Point(6, 55);
            this.button_pwr_off.Name = "button_pwr_off";
            this.button_pwr_off.Size = new System.Drawing.Size(92, 28);
            this.button_pwr_off.TabIndex = 45;
            this.button_pwr_off.Text = "Power OFF";
            this.button_pwr_off.UseVisualStyleBackColor = true;
            this.button_pwr_off.Click += new System.EventHandler(this.button_pwr_off_Click);
            // 
            // button_help
            // 
            this.button_help.Location = new System.Drawing.Point(723, 12);
            this.button_help.Name = "button_help";
            this.button_help.Size = new System.Drawing.Size(87, 28);
            this.button_help.TabIndex = 46;
            this.button_help.Text = "Help";
            this.button_help.UseVisualStyleBackColor = true;
            this.button_help.Visible = false;
            this.button_help.Click += new System.EventHandler(this.button_help_Click);
            // 
            // tabControl_Info_Setup
            // 
            this.tabControl_Info_Setup.Controls.Add(this.tabPage2_parameters);
            this.tabControl_Info_Setup.Controls.Add(this.tabPage1_Info);
            this.tabControl_Info_Setup.Location = new System.Drawing.Point(421, 104);
            this.tabControl_Info_Setup.Name = "tabControl_Info_Setup";
            this.tabControl_Info_Setup.SelectedIndex = 0;
            this.tabControl_Info_Setup.Size = new System.Drawing.Size(393, 618);
            this.tabControl_Info_Setup.TabIndex = 47;
            // 
            // tabPage2_parameters
            // 
            this.tabPage2_parameters.Controls.Add(this.groupBox3);
            this.tabPage2_parameters.Location = new System.Drawing.Point(4, 25);
            this.tabPage2_parameters.Name = "tabPage2_parameters";
            this.tabPage2_parameters.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2_parameters.Size = new System.Drawing.Size(385, 589);
            this.tabPage2_parameters.TabIndex = 1;
            this.tabPage2_parameters.Text = "Parameters";
            this.tabPage2_parameters.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label_MB_Sim_Port);
            this.groupBox3.Controls.Add(this.label4);
            this.groupBox3.Controls.Add(this.labelPrgBaud);
            this.groupBox3.Controls.Add(this.textBox_debug);
            this.groupBox3.Controls.Add(this.label3);
            this.groupBox3.Controls.Add(this.labelComPrg);
            this.groupBox3.Controls.Add(this.label1);
            this.groupBox3.Controls.Add(this.label_com_name);
            this.groupBox3.Controls.Add(this.label2);
            this.groupBox3.Location = new System.Drawing.Point(7, 6);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(369, 562);
            this.groupBox3.TabIndex = 50;
            this.groupBox3.TabStop = false;
            // 
            // label_MB_Sim_Port
            // 
            this.label_MB_Sim_Port.AutoSize = true;
            this.label_MB_Sim_Port.Location = new System.Drawing.Point(140, 88);
            this.label_MB_Sim_Port.Name = "label_MB_Sim_Port";
            this.label_MB_Sim_Port.Size = new System.Drawing.Size(23, 17);
            this.label_MB_Sim_Port.TabIndex = 8;
            this.label_MB_Sim_Port.Text = "---";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(4, 85);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(122, 17);
            this.label4.TabIndex = 7;
            this.label4.Text = "MS Sim interface :";
            // 
            // labelPrgBaud
            // 
            this.labelPrgBaud.AutoSize = true;
            this.labelPrgBaud.Location = new System.Drawing.Point(102, 62);
            this.labelPrgBaud.Name = "labelPrgBaud";
            this.labelPrgBaud.Size = new System.Drawing.Size(23, 17);
            this.labelPrgBaud.TabIndex = 6;
            this.labelPrgBaud.Text = "---";
            // 
            // textBox_debug
            // 
            this.textBox_debug.Location = new System.Drawing.Point(6, 400);
            this.textBox_debug.Multiline = true;
            this.textBox_debug.Name = "textBox_debug";
            this.textBox_debug.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox_debug.Size = new System.Drawing.Size(357, 156);
            this.textBox_debug.TabIndex = 2;
            this.textBox_debug.Visible = false;
            this.textBox_debug.TextChanged += new System.EventHandler(this.textBox_debug_TextChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 62);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(75, 17);
            this.label3.TabIndex = 5;
            this.label3.Text = "Prg Baud :";
            // 
            // labelComPrg
            // 
            this.labelComPrg.AutoSize = true;
            this.labelComPrg.Location = new System.Drawing.Point(103, 45);
            this.labelComPrg.Name = "labelComPrg";
            this.labelComPrg.Size = new System.Drawing.Size(23, 17);
            this.labelComPrg.TabIndex = 4;
            this.labelComPrg.Text = "---";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 28);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(93, 17);
            this.label1.TabIndex = 0;
            this.label1.Text = "I/O interface :";
            // 
            // label_com_name
            // 
            this.label_com_name.AutoSize = true;
            this.label_com_name.Location = new System.Drawing.Point(105, 28);
            this.label_com_name.Name = "label_com_name";
            this.label_com_name.Size = new System.Drawing.Size(23, 17);
            this.label_com_name.TabIndex = 1;
            this.label_com_name.Text = "---";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 45);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(97, 17);
            this.label2.TabIndex = 3;
            this.label2.Text = "Prg interface :";
            this.label2.Click += new System.EventHandler(this.labelComPrg_Click);
            // 
            // tabPage1_Info
            // 
            this.tabPage1_Info.Controls.Add(this.groupBox1);
            this.tabPage1_Info.Location = new System.Drawing.Point(4, 25);
            this.tabPage1_Info.Name = "tabPage1_Info";
            this.tabPage1_Info.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1_Info.Size = new System.Drawing.Size(385, 589);
            this.tabPage1_Info.TabIndex = 0;
            this.tabPage1_Info.Text = "Manual";
            this.tabPage1_Info.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.textBox_read_v);
            this.groupBox1.Controls.Add(this.button_read_v);
            this.groupBox1.Controls.Add(this.button_put_in_programming);
            this.groupBox1.Controls.Add(this.button_all_off);
            this.groupBox1.Controls.Add(this.button_button_off);
            this.groupBox1.Controls.Add(this.button_button);
            this.groupBox1.Controls.Add(this.button_tp5);
            this.groupBox1.Controls.Add(this.button_tp5_off);
            this.groupBox1.Controls.Add(this.button_tp_en_off);
            this.groupBox1.Controls.Add(this.button_tp_en);
            this.groupBox1.Controls.Add(this.button1);
            this.groupBox1.Controls.Add(this.button_pwr_off);
            this.groupBox1.Location = new System.Drawing.Point(6, 6);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(373, 559);
            this.groupBox1.TabIndex = 50;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "GME";
            // 
            // textBox_read_v
            // 
            this.textBox_read_v.HideSelection = false;
            this.textBox_read_v.Location = new System.Drawing.Point(105, 195);
            this.textBox_read_v.Name = "textBox_read_v";
            this.textBox_read_v.ReadOnly = true;
            this.textBox_read_v.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox_read_v.Size = new System.Drawing.Size(216, 22);
            this.textBox_read_v.TabIndex = 58;
            this.textBox_read_v.Text = "---";
            // 
            // button_read_v
            // 
            this.button_read_v.Location = new System.Drawing.Point(7, 195);
            this.button_read_v.Name = "button_read_v";
            this.button_read_v.Size = new System.Drawing.Size(92, 28);
            this.button_read_v.TabIndex = 57;
            this.button_read_v.Text = "Read V";
            this.button_read_v.UseVisualStyleBackColor = true;
            this.button_read_v.Click += new System.EventHandler(this.button_read_v_Click);
            // 
            // button_put_in_programming
            // 
            this.button_put_in_programming.Location = new System.Drawing.Point(229, 293);
            this.button_put_in_programming.Name = "button_put_in_programming";
            this.button_put_in_programming.Size = new System.Drawing.Size(92, 32);
            this.button_put_in_programming.TabIndex = 56;
            this.button_put_in_programming.Text = "ProgModeOn";
            this.button_put_in_programming.UseVisualStyleBackColor = true;
            this.button_put_in_programming.Click += new System.EventHandler(this.button_put_in_programming_Click);
            // 
            // button_all_off
            // 
            this.button_all_off.Location = new System.Drawing.Point(229, 136);
            this.button_all_off.Name = "button_all_off";
            this.button_all_off.Size = new System.Drawing.Size(92, 28);
            this.button_all_off.TabIndex = 55;
            this.button_all_off.Text = "ALL OFF";
            this.button_all_off.UseVisualStyleBackColor = true;
            this.button_all_off.Click += new System.EventHandler(this.button_all_off_Click);
            // 
            // button_button_off
            // 
            this.button_button_off.Location = new System.Drawing.Point(229, 55);
            this.button_button_off.Name = "button_button_off";
            this.button_button_off.Size = new System.Drawing.Size(92, 28);
            this.button_button_off.TabIndex = 54;
            this.button_button_off.Text = "Button OFF";
            this.button_button_off.UseVisualStyleBackColor = true;
            this.button_button_off.Click += new System.EventHandler(this.button_button_off_Click);
            // 
            // button_button
            // 
            this.button_button.Location = new System.Drawing.Point(229, 21);
            this.button_button.Name = "button_button";
            this.button_button.Size = new System.Drawing.Size(92, 28);
            this.button_button.TabIndex = 53;
            this.button_button.Text = "Button ON";
            this.button_button.UseVisualStyleBackColor = true;
            this.button_button.Click += new System.EventHandler(this.button_button_Click);
            // 
            // button_tp5
            // 
            this.button_tp5.Location = new System.Drawing.Point(7, 102);
            this.button_tp5.Name = "button_tp5";
            this.button_tp5.Size = new System.Drawing.Size(92, 28);
            this.button_tp5.TabIndex = 52;
            this.button_tp5.Text = "TP_5 ON";
            this.button_tp5.UseVisualStyleBackColor = true;
            this.button_tp5.Click += new System.EventHandler(this.button_tp5_Click);
            // 
            // button_tp5_off
            // 
            this.button_tp5_off.Location = new System.Drawing.Point(7, 136);
            this.button_tp5_off.Name = "button_tp5_off";
            this.button_tp5_off.Size = new System.Drawing.Size(92, 28);
            this.button_tp5_off.TabIndex = 51;
            this.button_tp5_off.Text = "TP_5 OFF";
            this.button_tp5_off.UseVisualStyleBackColor = true;
            this.button_tp5_off.Click += new System.EventHandler(this.button_tp5_off_Click);
            // 
            // button_tp_en_off
            // 
            this.button_tp_en_off.Location = new System.Drawing.Point(120, 55);
            this.button_tp_en_off.Name = "button_tp_en_off";
            this.button_tp_en_off.Size = new System.Drawing.Size(92, 28);
            this.button_tp_en_off.TabIndex = 50;
            this.button_tp_en_off.Text = "TP_EN OFF";
            this.button_tp_en_off.UseVisualStyleBackColor = true;
            this.button_tp_en_off.Click += new System.EventHandler(this.button_tp_en_off_Click);
            // 
            // button_tp_en
            // 
            this.button_tp_en.Location = new System.Drawing.Point(120, 21);
            this.button_tp_en.Name = "button_tp_en";
            this.button_tp_en.Size = new System.Drawing.Size(92, 28);
            this.button_tp_en.TabIndex = 49;
            this.button_tp_en.Text = "TP_EN ON";
            this.button_tp_en.UseVisualStyleBackColor = true;
            this.button_tp_en.Click += new System.EventHandler(this.button_tp_en_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(6, 21);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(92, 28);
            this.button1.TabIndex = 48;
            this.button1.Text = "Power ON";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click_2);
            // 
            // buttonTestStart
            // 
            this.buttonTestStart.Location = new System.Drawing.Point(13, 90);
            this.buttonTestStart.Name = "buttonTestStart";
            this.buttonTestStart.Size = new System.Drawing.Size(330, 51);
            this.buttonTestStart.TabIndex = 49;
            this.buttonTestStart.Text = "PRESS TO START THE TEST";
            this.buttonTestStart.UseVisualStyleBackColor = true;
            this.buttonTestStart.Click += new System.EventHandler(this.buttonTestStart_Click);
            // 
            // button_test_result
            // 
            this.button_test_result.Location = new System.Drawing.Point(13, 667);
            this.button_test_result.Name = "button_test_result";
            this.button_test_result.Size = new System.Drawing.Size(325, 51);
            this.button_test_result.TabIndex = 50;
            this.button_test_result.Text = "---------";
            this.button_test_result.UseVisualStyleBackColor = true;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(17, 9);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(127, 17);
            this.label5.TabIndex = 52;
            this.label5.Text = "Select device type:";
            // 
            // comboBox_DUT_Type
            // 
            this.comboBox_DUT_Type.Font = new System.Drawing.Font("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.comboBox_DUT_Type.FormattingEnabled = true;
            this.comboBox_DUT_Type.Location = new System.Drawing.Point(13, 37);
            this.comboBox_DUT_Type.Name = "comboBox_DUT_Type";
            this.comboBox_DUT_Type.Size = new System.Drawing.Size(329, 25);
            this.comboBox_DUT_Type.TabIndex = 53;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(832, 762);
            this.Controls.Add(this.comboBox_DUT_Type);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.button_test_result);
            this.Controls.Add(this.buttonTestStart);
            this.Controls.Add(this.tabControl_Info_Setup);
            this.Controls.Add(this.button_help);
            this.Controls.Add(this.rtbIncoming);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "Form1";
            this.Text = "CAREL GME HW Test V.1.1.0";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.tabControl_Info_Setup.ResumeLayout(false);
            this.tabPage2_parameters.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.tabPage1_Info.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.RichTextBox rtbIncoming;
        private System.Windows.Forms.Button button_pwr_off;
        private System.Windows.Forms.Button button_help;
        private System.Windows.Forms.TabControl tabControl_Info_Setup;
        private System.Windows.Forms.TabPage tabPage1_Info;
        private System.Windows.Forms.TabPage tabPage2_parameters;
        private System.Windows.Forms.Label label_com_name;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBox_debug;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label labelComPrg;
        private System.Windows.Forms.Label labelPrgBaud;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button buttonTestStart;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label_MB_Sim_Port;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button button_tp_en;
        private System.Windows.Forms.Button button_tp_en_off;
        private System.Windows.Forms.Button button_tp5;
        private System.Windows.Forms.Button button_tp5_off;
        private System.Windows.Forms.Button button_all_off;
        private System.Windows.Forms.Button button_button_off;
        private System.Windows.Forms.Button button_button;
        private System.Windows.Forms.Button button_test_result;
        private System.Windows.Forms.Button button_put_in_programming;
        private System.Windows.Forms.TextBox textBox_read_v;
        private System.Windows.Forms.Button button_read_v;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox comboBox_DUT_Type;
    }
}

