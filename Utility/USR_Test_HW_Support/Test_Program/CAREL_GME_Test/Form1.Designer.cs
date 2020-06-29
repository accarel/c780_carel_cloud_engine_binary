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
            this.btnGetSerialPorts = new System.Windows.Forms.Button();
            this.rtbIncoming = new System.Windows.Forms.RichTextBox();
            this.cboPorts = new System.Windows.Forms.ComboBox();
            this.btnPortState = new System.Windows.Forms.Button();
            this.button_Clear = new System.Windows.Forms.Button();
            this.button_pwr_off = new System.Windows.Forms.Button();
            this.button_help = new System.Windows.Forms.Button();
            this.tabControl_Info_Setup = new System.Windows.Forms.TabControl();
            this.tabPage1_Info = new System.Windows.Forms.TabPage();
            this.tabPage2_parameters = new System.Windows.Forms.TabPage();
            this.textBox_debug = new System.Windows.Forms.TextBox();
            this.label_com_name = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.labelComPrg = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.labelPrgBaud = new System.Windows.Forms.Label();
            this.buttonTestStart = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.tabControl_Info_Setup.SuspendLayout();
            this.tabPage1_Info.SuspendLayout();
            this.tabPage2_parameters.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnGetSerialPorts
            // 
            this.btnGetSerialPorts.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnGetSerialPorts.Location = new System.Drawing.Point(714, 669);
            this.btnGetSerialPorts.Margin = new System.Windows.Forms.Padding(4);
            this.btnGetSerialPorts.Name = "btnGetSerialPorts";
            this.btnGetSerialPorts.Size = new System.Drawing.Size(100, 28);
            this.btnGetSerialPorts.TabIndex = 0;
            this.btnGetSerialPorts.Text = "COM search";
            this.btnGetSerialPorts.UseVisualStyleBackColor = true;
            this.btnGetSerialPorts.Visible = false;
            this.btnGetSerialPorts.Click += new System.EventHandler(this.btnGetSerialPorts_Click);
            // 
            // rtbIncoming
            // 
            this.rtbIncoming.Location = new System.Drawing.Point(13, 96);
            this.rtbIncoming.Margin = new System.Windows.Forms.Padding(4);
            this.rtbIncoming.Name = "rtbIncoming";
            this.rtbIncoming.Size = new System.Drawing.Size(330, 568);
            this.rtbIncoming.TabIndex = 1;
            this.rtbIncoming.Text = "";
            // 
            // cboPorts
            // 
            this.cboPorts.FormattingEnabled = true;
            this.cboPorts.Location = new System.Drawing.Point(614, 673);
            this.cboPorts.Margin = new System.Windows.Forms.Padding(4);
            this.cboPorts.Name = "cboPorts";
            this.cboPorts.Size = new System.Drawing.Size(92, 24);
            this.cboPorts.TabIndex = 2;
            this.cboPorts.Visible = false;
            // 
            // btnPortState
            // 
            this.btnPortState.Location = new System.Drawing.Point(506, 669);
            this.btnPortState.Margin = new System.Windows.Forms.Padding(4);
            this.btnPortState.Name = "btnPortState";
            this.btnPortState.Size = new System.Drawing.Size(100, 28);
            this.btnPortState.TabIndex = 13;
            this.btnPortState.Text = "Connect";
            this.btnPortState.UseVisualStyleBackColor = true;
            this.btnPortState.Visible = false;
            this.btnPortState.Click += new System.EventHandler(this.btnPortState_Click);
            // 
            // button_Clear
            // 
            this.button_Clear.Location = new System.Drawing.Point(119, 671);
            this.button_Clear.Name = "button_Clear";
            this.button_Clear.Size = new System.Drawing.Size(98, 30);
            this.button_Clear.TabIndex = 36;
            this.button_Clear.Text = "Clear";
            this.button_Clear.UseVisualStyleBackColor = true;
            this.button_Clear.Click += new System.EventHandler(this.button_Clear_Click);
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
            this.button_help.Click += new System.EventHandler(this.button_help_Click);
            // 
            // tabControl_Info_Setup
            // 
            this.tabControl_Info_Setup.Controls.Add(this.tabPage2_parameters);
            this.tabControl_Info_Setup.Controls.Add(this.tabPage1_Info);
            this.tabControl_Info_Setup.Location = new System.Drawing.Point(421, 46);
            this.tabControl_Info_Setup.Name = "tabControl_Info_Setup";
            this.tabControl_Info_Setup.SelectedIndex = 0;
            this.tabControl_Info_Setup.Size = new System.Drawing.Size(393, 618);
            this.tabControl_Info_Setup.TabIndex = 47;
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
            // textBox_debug
            // 
            this.textBox_debug.Location = new System.Drawing.Point(6, 400);
            this.textBox_debug.Multiline = true;
            this.textBox_debug.Name = "textBox_debug";
            this.textBox_debug.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox_debug.Size = new System.Drawing.Size(357, 156);
            this.textBox_debug.TabIndex = 2;
            this.textBox_debug.TextChanged += new System.EventHandler(this.textBox_debug_TextChanged);
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
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 28);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(93, 17);
            this.label1.TabIndex = 0;
            this.label1.Text = "I/O interface :";
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
            // groupBox3
            // 
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
            // labelComPrg
            // 
            this.labelComPrg.AutoSize = true;
            this.labelComPrg.Location = new System.Drawing.Point(103, 45);
            this.labelComPrg.Name = "labelComPrg";
            this.labelComPrg.Size = new System.Drawing.Size(23, 17);
            this.labelComPrg.TabIndex = 4;
            this.labelComPrg.Text = "---";
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
            // labelPrgBaud
            // 
            this.labelPrgBaud.AutoSize = true;
            this.labelPrgBaud.Location = new System.Drawing.Point(102, 62);
            this.labelPrgBaud.Name = "labelPrgBaud";
            this.labelPrgBaud.Size = new System.Drawing.Size(23, 17);
            this.labelPrgBaud.TabIndex = 6;
            this.labelPrgBaud.Text = "---";
            // 
            // buttonTestStart
            // 
            this.buttonTestStart.Location = new System.Drawing.Point(18, 32);
            this.buttonTestStart.Name = "buttonTestStart";
            this.buttonTestStart.Size = new System.Drawing.Size(325, 51);
            this.buttonTestStart.TabIndex = 49;
            this.buttonTestStart.Text = "TEST START";
            this.buttonTestStart.UseVisualStyleBackColor = true;
            this.buttonTestStart.Click += new System.EventHandler(this.buttonTestStart_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.button1);
            this.groupBox1.Controls.Add(this.button_pwr_off);
            this.groupBox1.Location = new System.Drawing.Point(6, 6);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(207, 191);
            this.groupBox1.TabIndex = 50;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "GME";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(850, 710);
            this.Controls.Add(this.buttonTestStart);
            this.Controls.Add(this.tabControl_Info_Setup);
            this.Controls.Add(this.button_help);
            this.Controls.Add(this.button_Clear);
            this.Controls.Add(this.btnPortState);
            this.Controls.Add(this.cboPorts);
            this.Controls.Add(this.rtbIncoming);
            this.Controls.Add(this.btnGetSerialPorts);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "Form1";
            this.Text = "CAREL GME HW Test V.1.0";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.tabControl_Info_Setup.ResumeLayout(false);
            this.tabPage1_Info.ResumeLayout(false);
            this.tabPage2_parameters.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnGetSerialPorts;
        private System.Windows.Forms.RichTextBox rtbIncoming;
        private System.Windows.Forms.ComboBox cboPorts;
        private System.Windows.Forms.Button btnPortState;
        private System.Windows.Forms.Button button_Clear;
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
    }
}

