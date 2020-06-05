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
            this.cboBaudRate = new System.Windows.Forms.ComboBox();
            this.cboDataBits = new System.Windows.Forms.ComboBox();
            this.cboStopBits = new System.Windows.Forms.ComboBox();
            this.cboParity = new System.Windows.Forms.ComboBox();
            this.cboHandShaking = new System.Windows.Forms.ComboBox();
            this.lblBreakStatus = new System.Windows.Forms.Label();
            this.lblCTSStatus = new System.Windows.Forms.Label();
            this.lblDSRStatus = new System.Windows.Forms.Label();
            this.lblRIStatus = new System.Windows.Forms.Label();
            this.btnTest = new System.Windows.Forms.Button();
            this.btnPortState = new System.Windows.Forms.Button();
            this.rtbOutgoing = new System.Windows.Forms.RichTextBox();
            this.btnHyperTerm = new System.Windows.Forms.Button();
            this.txtCommand = new System.Windows.Forms.TextBox();
            this.button_ati = new System.Windows.Forms.Button();
            this.comboBox_band = new System.Windows.Forms.ComboBox();
            this.comboBox_timeslot = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.comboBox_burst_type = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.button_force_tx = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.textBox_afc = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.textBox_arfcn = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.button_stop_stack = new System.Windows.Forms.Button();
            this.button_dummy = new System.Windows.Forms.Button();
            this.textBox_pcl = new System.Windows.Forms.TextBox();
            this.button_Clear = new System.Windows.Forms.Button();
            this.comboBox_txslot = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.comboBox_power_rollbk = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.button_setpower = new System.Windows.Forms.Button();
            this.textBox_send_at = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.button_pwr_off = new System.Windows.Forms.Button();
            this.button_help = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnGetSerialPorts
            // 
            this.btnGetSerialPorts.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnGetSerialPorts.Location = new System.Drawing.Point(16, 33);
            this.btnGetSerialPorts.Margin = new System.Windows.Forms.Padding(4);
            this.btnGetSerialPorts.Name = "btnGetSerialPorts";
            this.btnGetSerialPorts.Size = new System.Drawing.Size(100, 28);
            this.btnGetSerialPorts.TabIndex = 0;
            this.btnGetSerialPorts.Text = "COM search";
            this.btnGetSerialPorts.UseVisualStyleBackColor = true;
            this.btnGetSerialPorts.Click += new System.EventHandler(this.btnGetSerialPorts_Click);
            // 
            // rtbIncoming
            // 
            this.rtbIncoming.Location = new System.Drawing.Point(17, 69);
            this.rtbIncoming.Margin = new System.Windows.Forms.Padding(4);
            this.rtbIncoming.Name = "rtbIncoming";
            this.rtbIncoming.Size = new System.Drawing.Size(330, 408);
            this.rtbIncoming.TabIndex = 1;
            this.rtbIncoming.Text = "";
            // 
            // cboPorts
            // 
            this.cboPorts.FormattingEnabled = true;
            this.cboPorts.Location = new System.Drawing.Point(136, 36);
            this.cboPorts.Margin = new System.Windows.Forms.Padding(4);
            this.cboPorts.Name = "cboPorts";
            this.cboPorts.Size = new System.Drawing.Size(92, 24);
            this.cboPorts.TabIndex = 2;
            // 
            // cboBaudRate
            // 
            this.cboBaudRate.FormattingEnabled = true;
            this.cboBaudRate.Location = new System.Drawing.Point(1077, 339);
            this.cboBaudRate.Margin = new System.Windows.Forms.Padding(4);
            this.cboBaudRate.Name = "cboBaudRate";
            this.cboBaudRate.Size = new System.Drawing.Size(160, 24);
            this.cboBaudRate.TabIndex = 3;
            this.cboBaudRate.Visible = false;
            // 
            // cboDataBits
            // 
            this.cboDataBits.FormattingEnabled = true;
            this.cboDataBits.Location = new System.Drawing.Point(1077, 372);
            this.cboDataBits.Margin = new System.Windows.Forms.Padding(4);
            this.cboDataBits.Name = "cboDataBits";
            this.cboDataBits.Size = new System.Drawing.Size(160, 24);
            this.cboDataBits.TabIndex = 4;
            this.cboDataBits.Visible = false;
            // 
            // cboStopBits
            // 
            this.cboStopBits.FormattingEnabled = true;
            this.cboStopBits.Location = new System.Drawing.Point(1077, 405);
            this.cboStopBits.Margin = new System.Windows.Forms.Padding(4);
            this.cboStopBits.Name = "cboStopBits";
            this.cboStopBits.Size = new System.Drawing.Size(160, 24);
            this.cboStopBits.TabIndex = 5;
            this.cboStopBits.Visible = false;
            // 
            // cboParity
            // 
            this.cboParity.FormattingEnabled = true;
            this.cboParity.Location = new System.Drawing.Point(1077, 439);
            this.cboParity.Margin = new System.Windows.Forms.Padding(4);
            this.cboParity.Name = "cboParity";
            this.cboParity.Size = new System.Drawing.Size(160, 24);
            this.cboParity.TabIndex = 6;
            this.cboParity.Visible = false;
            // 
            // cboHandShaking
            // 
            this.cboHandShaking.FormattingEnabled = true;
            this.cboHandShaking.Location = new System.Drawing.Point(1077, 472);
            this.cboHandShaking.Margin = new System.Windows.Forms.Padding(4);
            this.cboHandShaking.Name = "cboHandShaking";
            this.cboHandShaking.Size = new System.Drawing.Size(160, 24);
            this.cboHandShaking.TabIndex = 7;
            this.cboHandShaking.Visible = false;
            // 
            // lblBreakStatus
            // 
            this.lblBreakStatus.AutoSize = true;
            this.lblBreakStatus.Location = new System.Drawing.Point(1077, 159);
            this.lblBreakStatus.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblBreakStatus.Name = "lblBreakStatus";
            this.lblBreakStatus.Size = new System.Drawing.Size(45, 17);
            this.lblBreakStatus.TabIndex = 8;
            this.lblBreakStatus.Text = "Break";
            this.lblBreakStatus.Visible = false;
            // 
            // lblCTSStatus
            // 
            this.lblCTSStatus.AutoSize = true;
            this.lblCTSStatus.Location = new System.Drawing.Point(1074, 170);
            this.lblCTSStatus.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblCTSStatus.Name = "lblCTSStatus";
            this.lblCTSStatus.Size = new System.Drawing.Size(35, 17);
            this.lblCTSStatus.TabIndex = 9;
            this.lblCTSStatus.Text = "CTS";
            this.lblCTSStatus.Visible = false;
            // 
            // lblDSRStatus
            // 
            this.lblDSRStatus.AutoSize = true;
            this.lblDSRStatus.Location = new System.Drawing.Point(1074, 187);
            this.lblDSRStatus.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblDSRStatus.Name = "lblDSRStatus";
            this.lblDSRStatus.Size = new System.Drawing.Size(37, 17);
            this.lblDSRStatus.TabIndex = 10;
            this.lblDSRStatus.Text = "DSR";
            this.lblDSRStatus.Visible = false;
            // 
            // lblRIStatus
            // 
            this.lblRIStatus.AutoSize = true;
            this.lblRIStatus.Location = new System.Drawing.Point(1166, 182);
            this.lblRIStatus.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblRIStatus.Name = "lblRIStatus";
            this.lblRIStatus.Size = new System.Drawing.Size(21, 17);
            this.lblRIStatus.TabIndex = 11;
            this.lblRIStatus.Text = "RI";
            this.lblRIStatus.Visible = false;
            // 
            // btnTest
            // 
            this.btnTest.Location = new System.Drawing.Point(1077, 63);
            this.btnTest.Margin = new System.Windows.Forms.Padding(4);
            this.btnTest.Name = "btnTest";
            this.btnTest.Size = new System.Drawing.Size(100, 28);
            this.btnTest.TabIndex = 12;
            this.btnTest.Text = "Test";
            this.btnTest.UseVisualStyleBackColor = true;
            this.btnTest.Click += new System.EventHandler(this.btnTest_Click);
            // 
            // btnPortState
            // 
            this.btnPortState.Location = new System.Drawing.Point(247, 32);
            this.btnPortState.Margin = new System.Windows.Forms.Padding(4);
            this.btnPortState.Name = "btnPortState";
            this.btnPortState.Size = new System.Drawing.Size(100, 28);
            this.btnPortState.TabIndex = 13;
            this.btnPortState.Text = "Connect";
            this.btnPortState.UseVisualStyleBackColor = true;
            this.btnPortState.Click += new System.EventHandler(this.btnPortState_Click);
            // 
            // rtbOutgoing
            // 
            this.rtbOutgoing.Location = new System.Drawing.Point(1077, 210);
            this.rtbOutgoing.Margin = new System.Windows.Forms.Padding(4);
            this.rtbOutgoing.Name = "rtbOutgoing";
            this.rtbOutgoing.Size = new System.Drawing.Size(284, 37);
            this.rtbOutgoing.TabIndex = 15;
            this.rtbOutgoing.Text = "";
            this.rtbOutgoing.Visible = false;
            this.rtbOutgoing.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.rtbOutgoing_KeyPress);
            // 
            // btnHyperTerm
            // 
            this.btnHyperTerm.Location = new System.Drawing.Point(1077, 99);
            this.btnHyperTerm.Margin = new System.Windows.Forms.Padding(4);
            this.btnHyperTerm.Name = "btnHyperTerm";
            this.btnHyperTerm.Size = new System.Drawing.Size(100, 28);
            this.btnHyperTerm.TabIndex = 16;
            this.btnHyperTerm.Text = "HyperTerm";
            this.btnHyperTerm.UseVisualStyleBackColor = true;
            this.btnHyperTerm.Click += new System.EventHandler(this.btnHyperTerm_Click);
            // 
            // txtCommand
            // 
            this.txtCommand.Location = new System.Drawing.Point(1077, 135);
            this.txtCommand.Margin = new System.Windows.Forms.Padding(4);
            this.txtCommand.Name = "txtCommand";
            this.txtCommand.Size = new System.Drawing.Size(132, 22);
            this.txtCommand.TabIndex = 17;
            // 
            // button_ati
            // 
            this.button_ati.Location = new System.Drawing.Point(375, 32);
            this.button_ati.Name = "button_ati";
            this.button_ati.Size = new System.Drawing.Size(93, 28);
            this.button_ati.TabIndex = 18;
            this.button_ati.Text = "Get Dev.ID";
            this.button_ati.UseVisualStyleBackColor = true;
            this.button_ati.Click += new System.EventHandler(this.button_ati_Click);
            // 
            // comboBox_band
            // 
            this.comboBox_band.FormattingEnabled = true;
            this.comboBox_band.Location = new System.Drawing.Point(480, 80);
            this.comboBox_band.Margin = new System.Windows.Forms.Padding(4);
            this.comboBox_band.Name = "comboBox_band";
            this.comboBox_band.Size = new System.Drawing.Size(261, 24);
            this.comboBox_band.TabIndex = 19;
            this.comboBox_band.SelectedIndexChanged += new System.EventHandler(this.comboBox_band_SelectedIndexChanged);
            // 
            // comboBox_timeslot
            // 
            this.comboBox_timeslot.FormattingEnabled = true;
            this.comboBox_timeslot.Location = new System.Drawing.Point(89, 69);
            this.comboBox_timeslot.Name = "comboBox_timeslot";
            this.comboBox_timeslot.Size = new System.Drawing.Size(285, 24);
            this.comboBox_timeslot.TabIndex = 22;
            this.comboBox_timeslot.SelectedIndexChanged += new System.EventHandler(this.comboBox_timeslot_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(367, 83);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 17);
            this.label2.TabIndex = 24;
            this.label2.Text = "Band";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(15, 35);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(54, 17);
            this.label3.TabIndex = 25;
            this.label3.Text = "ARFCN";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(17, 70);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(35, 17);
            this.label4.TabIndex = 26;
            this.label4.Text = "TSC";
            // 
            // comboBox_burst_type
            // 
            this.comboBox_burst_type.FormattingEnabled = true;
            this.comboBox_burst_type.Location = new System.Drawing.Point(89, 133);
            this.comboBox_burst_type.Name = "comboBox_burst_type";
            this.comboBox_burst_type.Size = new System.Drawing.Size(285, 24);
            this.comboBox_burst_type.TabIndex = 28;
            this.comboBox_burst_type.SelectedIndexChanged += new System.EventHandler(this.comboBox_burst_type_SelectedIndexChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(364, 342);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(72, 17);
            this.label6.TabIndex = 29;
            this.label6.Text = "Burst type";
            // 
            // button_force_tx
            // 
            this.button_force_tx.Location = new System.Drawing.Point(192, 172);
            this.button_force_tx.Name = "button_force_tx";
            this.button_force_tx.Size = new System.Drawing.Size(63, 31);
            this.button_force_tx.TabIndex = 30;
            this.button_force_tx.Text = "GO";
            this.button_force_tx.UseVisualStyleBackColor = true;
            this.button_force_tx.Click += new System.EventHandler(this.button_force_tx_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(22, 101);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(34, 17);
            this.label7.TabIndex = 31;
            this.label7.Text = "AFC";
            // 
            // textBox_afc
            // 
            this.textBox_afc.Location = new System.Drawing.Point(89, 103);
            this.textBox_afc.Name = "textBox_afc";
            this.textBox_afc.Size = new System.Drawing.Size(285, 22);
            this.textBox_afc.TabIndex = 32;
            this.textBox_afc.Text = "4100";
            this.textBox_afc.TextChanged += new System.EventHandler(this.textBox_afc_TextChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.textBox_arfcn);
            this.groupBox1.Controls.Add(this.label8);
            this.groupBox1.Controls.Add(this.textBox_afc);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.button_force_tx);
            this.groupBox1.Controls.Add(this.comboBox_burst_type);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.comboBox_timeslot);
            this.groupBox1.Location = new System.Drawing.Point(367, 255);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(393, 222);
            this.groupBox1.TabIndex = 33;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Force to transmit";
            this.groupBox1.Enter += new System.EventHandler(this.groupBox1_Enter);
            // 
            // textBox_arfcn
            // 
            this.textBox_arfcn.Location = new System.Drawing.Point(88, 35);
            this.textBox_arfcn.Name = "textBox_arfcn";
            this.textBox_arfcn.Size = new System.Drawing.Size(285, 22);
            this.textBox_arfcn.TabIndex = 35;
            this.textBox_arfcn.Text = "9";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(5, 136);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(77, 17);
            this.label8.TabIndex = 33;
            this.label8.Text = "Burst Type";
            this.label8.Click += new System.EventHandler(this.label8_Click);
            // 
            // button_stop_stack
            // 
            this.button_stop_stack.Location = new System.Drawing.Point(486, 32);
            this.button_stop_stack.Name = "button_stop_stack";
            this.button_stop_stack.Size = new System.Drawing.Size(92, 28);
            this.button_stop_stack.TabIndex = 34;
            this.button_stop_stack.Text = "Stop stack";
            this.button_stop_stack.UseVisualStyleBackColor = true;
            this.button_stop_stack.Click += new System.EventHandler(this.button1_Click);
            // 
            // button_dummy
            // 
            this.button_dummy.Location = new System.Drawing.Point(1077, 28);
            this.button_dummy.Name = "button_dummy";
            this.button_dummy.Size = new System.Drawing.Size(92, 28);
            this.button_dummy.TabIndex = 35;
            this.button_dummy.Text = "Dummy";
            this.button_dummy.UseVisualStyleBackColor = true;
            this.button_dummy.Visible = false;
            this.button_dummy.Click += new System.EventHandler(this.button_dummy_Click);
            // 
            // textBox_pcl
            // 
            this.textBox_pcl.Location = new System.Drawing.Point(480, 148);
            this.textBox_pcl.Name = "textBox_pcl";
            this.textBox_pcl.Size = new System.Drawing.Size(261, 22);
            this.textBox_pcl.TabIndex = 36;
            this.textBox_pcl.Text = "8";
            // 
            // button_Clear
            // 
            this.button_Clear.Location = new System.Drawing.Point(17, 487);
            this.button_Clear.Name = "button_Clear";
            this.button_Clear.Size = new System.Drawing.Size(98, 30);
            this.button_Clear.TabIndex = 36;
            this.button_Clear.Text = "Clear";
            this.button_Clear.UseVisualStyleBackColor = true;
            this.button_Clear.Click += new System.EventHandler(this.button_Clear_Click);
            // 
            // comboBox_txslot
            // 
            this.comboBox_txslot.FormattingEnabled = true;
            this.comboBox_txslot.Location = new System.Drawing.Point(480, 113);
            this.comboBox_txslot.Name = "comboBox_txslot";
            this.comboBox_txslot.Size = new System.Drawing.Size(261, 24);
            this.comboBox_txslot.TabIndex = 37;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(367, 116);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(61, 17);
            this.label1.TabIndex = 38;
            this.label1.Text = "TX Slots";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(367, 148);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(34, 17);
            this.label9.TabIndex = 39;
            this.label9.Text = "PCL";
            // 
            // comboBox_power_rollbk
            // 
            this.comboBox_power_rollbk.FormattingEnabled = true;
            this.comboBox_power_rollbk.Location = new System.Drawing.Point(480, 177);
            this.comboBox_power_rollbk.Margin = new System.Windows.Forms.Padding(4);
            this.comboBox_power_rollbk.Name = "comboBox_power_rollbk";
            this.comboBox_power_rollbk.Size = new System.Drawing.Size(260, 24);
            this.comboBox_power_rollbk.TabIndex = 40;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(367, 180);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(106, 17);
            this.label5.TabIndex = 41;
            this.label5.Text = "Pwr attenuation";
            // 
            // button_setpower
            // 
            this.button_setpower.Location = new System.Drawing.Point(747, 177);
            this.button_setpower.Name = "button_setpower";
            this.button_setpower.Size = new System.Drawing.Size(75, 23);
            this.button_setpower.TabIndex = 42;
            this.button_setpower.Text = "Set";
            this.button_setpower.UseVisualStyleBackColor = true;
            this.button_setpower.Click += new System.EventHandler(this.button_setpower_Click);
            // 
            // textBox_send_at
            // 
            this.textBox_send_at.Location = new System.Drawing.Point(367, 491);
            this.textBox_send_at.Name = "textBox_send_at";
            this.textBox_send_at.Size = new System.Drawing.Size(282, 22);
            this.textBox_send_at.TabIndex = 43;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(663, 490);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(96, 23);
            this.button1.TabIndex = 44;
            this.button1.Text = "Send AT";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click_1);
            // 
            // button_pwr_off
            // 
            this.button_pwr_off.Location = new System.Drawing.Point(649, 32);
            this.button_pwr_off.Name = "button_pwr_off";
            this.button_pwr_off.Size = new System.Drawing.Size(92, 28);
            this.button_pwr_off.TabIndex = 45;
            this.button_pwr_off.Text = "Power OFF";
            this.button_pwr_off.UseVisualStyleBackColor = true;
            this.button_pwr_off.Click += new System.EventHandler(this.button_pwr_off_Click);
            // 
            // button_help
            // 
            this.button_help.Location = new System.Drawing.Point(784, 32);
            this.button_help.Name = "button_help";
            this.button_help.Size = new System.Drawing.Size(87, 28);
            this.button_help.TabIndex = 46;
            this.button_help.Text = "Help";
            this.button_help.UseVisualStyleBackColor = true;
            this.button_help.Click += new System.EventHandler(this.button_help_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(883, 533);
            this.Controls.Add(this.button_help);
            this.Controls.Add(this.button_pwr_off);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.textBox_send_at);
            this.Controls.Add(this.button_setpower);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.comboBox_power_rollbk);
            this.Controls.Add(this.textBox_pcl);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.comboBox_txslot);
            this.Controls.Add(this.button_Clear);
            this.Controls.Add(this.button_dummy);
            this.Controls.Add(this.button_stop_stack);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.button_ati);
            this.Controls.Add(this.txtCommand);
            this.Controls.Add(this.btnHyperTerm);
            this.Controls.Add(this.rtbOutgoing);
            this.Controls.Add(this.btnPortState);
            this.Controls.Add(this.btnTest);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.comboBox_band);
            this.Controls.Add(this.lblRIStatus);
            this.Controls.Add(this.lblDSRStatus);
            this.Controls.Add(this.lblCTSStatus);
            this.Controls.Add(this.lblBreakStatus);
            this.Controls.Add(this.cboHandShaking);
            this.Controls.Add(this.cboParity);
            this.Controls.Add(this.cboStopBits);
            this.Controls.Add(this.cboDataBits);
            this.Controls.Add(this.cboBaudRate);
            this.Controls.Add(this.cboPorts);
            this.Controls.Add(this.rtbIncoming);
            this.Controls.Add(this.btnGetSerialPorts);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "Form1";
            this.Text = "CAREL Gateway Middle End Test M95 V.1.0";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnGetSerialPorts;
        private System.Windows.Forms.RichTextBox rtbIncoming;
        private System.Windows.Forms.ComboBox cboPorts;
        private System.Windows.Forms.ComboBox cboBaudRate;
        private System.Windows.Forms.ComboBox cboDataBits;
        private System.Windows.Forms.ComboBox cboStopBits;
        private System.Windows.Forms.ComboBox cboParity;
        private System.Windows.Forms.ComboBox cboHandShaking;
        private System.Windows.Forms.Label lblBreakStatus;
        private System.Windows.Forms.Label lblCTSStatus;
        private System.Windows.Forms.Label lblDSRStatus;
        private System.Windows.Forms.Label lblRIStatus;
        private System.Windows.Forms.Button btnTest;
        private System.Windows.Forms.Button btnPortState;
        private System.Windows.Forms.RichTextBox rtbOutgoing;
        private System.Windows.Forms.Button btnHyperTerm;
        private System.Windows.Forms.TextBox txtCommand;
        private System.Windows.Forms.Button button_ati;
        private System.Windows.Forms.ComboBox comboBox_band;
        private System.Windows.Forms.ComboBox comboBox_timeslot;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox comboBox_burst_type;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button button_force_tx;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox textBox_afc;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button button_stop_stack;
        private System.Windows.Forms.Button button_dummy;
        private System.Windows.Forms.TextBox textBox_arfcn;
        private System.Windows.Forms.TextBox textBox_pcl;
        private System.Windows.Forms.Button button_Clear;
        private System.Windows.Forms.ComboBox comboBox_txslot;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.ComboBox comboBox_power_rollbk;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button button_setpower;
        private System.Windows.Forms.TextBox textBox_send_at;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button_pwr_off;
        private System.Windows.Forms.Button button_help;
    }
}

