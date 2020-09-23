namespace MqttClientSimulatorBinary
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.txtConsole = new System.Windows.Forms.TextBox();
            this.buttonChangeCredential = new System.Windows.Forms.Button();
            this.buttonReadValue = new System.Windows.Forms.Button();
            this.textBoxPublish = new System.Windows.Forms.TextBox();
            this.labelPublish = new System.Windows.Forms.Label();
            this.labelResponse = new System.Windows.Forms.Label();
            this.buttonClear = new System.Windows.Forms.Button();
            this.buttonScanLine = new System.Windows.Forms.Button();
            this.buttonSet_Devs_Config = new System.Windows.Forms.Button();
            this.buttonSet_Lines_Config = new System.Windows.Forms.Button();
            this.buttonUpdate_ca_cerficates = new System.Windows.Forms.Button();
            this.buttonUpdate_dev_firmware = new System.Windows.Forms.Button();
            this.buttonUpdate_gw_firmware = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxMQTT_Server_URL = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.textBoxMQTT_Server_Port = new System.Windows.Forms.TextBox();
            this.buttonConnect = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.button_ReqDBGInfo = new System.Windows.Forms.Button();
            this.buttonSet_Lines_Config_TTL = new System.Windows.Forms.Button();
            this.button_flush = new System.Windows.Forms.Button();
            this.button_reboot = new System.Windows.Forms.Button();
            this.button_stop_engine = new System.Windows.Forms.Button();
            this.button_start_engine = new System.Windows.Forms.Button();
            this.button_test_set_gw_config_req = new System.Windows.Forms.Button();
            this.button_send_mb_adu = new System.Windows.Forms.Button();
            this.buttonClearResponse = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.button_SetFlags = new System.Windows.Forms.Button();
            this.textBox_MB_HR_R_Func = new System.Windows.Forms.TextBox();
            this.button_MB_Write_HR = new System.Windows.Forms.Button();
            this.button_MB_Read_HR = new System.Windows.Forms.Button();
            this.label12 = new System.Windows.Forms.Label();
            this.textBox_Flags = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.textBox_B = new System.Windows.Forms.TextBox();
            this.textBox_A = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.textBox_MB_Len = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.textBox_MB_Pos = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.textBox_MB_Addr = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.textBox_Alias = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.textBox_MB_Dim = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.textBox_MB_HR_W_Func = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textBox_HR_Val = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.textBox6 = new System.Windows.Forms.TextBox();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.textBox7 = new System.Windows.Forms.TextBox();
            this.label17 = new System.Windows.Forms.Label();
            this.textBox8 = new System.Windows.Forms.TextBox();
            this.label20 = new System.Windows.Forms.Label();
            this.textBox9 = new System.Windows.Forms.TextBox();
            this.label23 = new System.Windows.Forms.Label();
            this.textBox10 = new System.Windows.Forms.TextBox();
            this.label24 = new System.Windows.Forms.Label();
            this.textBox11 = new System.Windows.Forms.TextBox();
            this.button5 = new System.Windows.Forms.Button();
            this.button6 = new System.Windows.Forms.Button();
            this.textBox12 = new System.Windows.Forms.TextBox();
            this.label25 = new System.Windows.Forms.Label();
            this.textBox13 = new System.Windows.Forms.TextBox();
            this.label26 = new System.Windows.Forms.Label();
            this.textBox14 = new System.Windows.Forms.TextBox();
            this.label27 = new System.Windows.Forms.Label();
            this.textBox15 = new System.Windows.Forms.TextBox();
            this.label28 = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.label13 = new System.Windows.Forms.Label();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.label14 = new System.Windows.Forms.Label();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.label15 = new System.Windows.Forms.Label();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.label16 = new System.Windows.Forms.Label();
            this.textBox_MB_COIL_W_Func = new System.Windows.Forms.TextBox();
            this.button_MB_Write_COIL = new System.Windows.Forms.Button();
            this.button_MB_Read_COIL = new System.Windows.Forms.Button();
            this.textBox_MB_Addr_Coil = new System.Windows.Forms.TextBox();
            this.label18 = new System.Windows.Forms.Label();
            this.textBox_Alias_Coil = new System.Windows.Forms.TextBox();
            this.label19 = new System.Windows.Forms.Label();
            this.textBox_MB_COIL_R_Func = new System.Windows.Forms.TextBox();
            this.label21 = new System.Windows.Forms.Label();
            this.textBox_MB_COIL_Val = new System.Windows.Forms.TextBox();
            this.label22 = new System.Windows.Forms.Label();
            this.groupBox7 = new System.Windows.Forms.GroupBox();
            this.groupBox10 = new System.Windows.Forms.GroupBox();
            this.textBox26 = new System.Windows.Forms.TextBox();
            this.button11 = new System.Windows.Forms.Button();
            this.button12 = new System.Windows.Forms.Button();
            this.textBox27 = new System.Windows.Forms.TextBox();
            this.label37 = new System.Windows.Forms.Label();
            this.textBox28 = new System.Windows.Forms.TextBox();
            this.label38 = new System.Windows.Forms.Label();
            this.textBox29 = new System.Windows.Forms.TextBox();
            this.label39 = new System.Windows.Forms.Label();
            this.textBox30 = new System.Windows.Forms.TextBox();
            this.label40 = new System.Windows.Forms.Label();
            this.button_MB_Read_DI = new System.Windows.Forms.Button();
            this.textBox_MB_Addr_DI = new System.Windows.Forms.TextBox();
            this.label41 = new System.Windows.Forms.Label();
            this.textBox_Alias_DI = new System.Windows.Forms.TextBox();
            this.label42 = new System.Windows.Forms.Label();
            this.textBox_MB_DI_R_Func = new System.Windows.Forms.TextBox();
            this.label43 = new System.Windows.Forms.Label();
            this.groupBox8 = new System.Windows.Forms.GroupBox();
            this.button_SetFlags_IR = new System.Windows.Forms.Button();
            this.textBox_MB_IR_R_Func = new System.Windows.Forms.TextBox();
            this.button_MB_Read_IR = new System.Windows.Forms.Button();
            this.label29 = new System.Windows.Forms.Label();
            this.textBox_Flags_IR = new System.Windows.Forms.TextBox();
            this.label30 = new System.Windows.Forms.Label();
            this.textBox_B_IR = new System.Windows.Forms.TextBox();
            this.textBox_A_IR = new System.Windows.Forms.TextBox();
            this.label31 = new System.Windows.Forms.Label();
            this.textBox_MB_Len_IR = new System.Windows.Forms.TextBox();
            this.label32 = new System.Windows.Forms.Label();
            this.textBox_MB_Pos_IR = new System.Windows.Forms.TextBox();
            this.label33 = new System.Windows.Forms.Label();
            this.textBox_MB_Addr_IR = new System.Windows.Forms.TextBox();
            this.label34 = new System.Windows.Forms.Label();
            this.textBox_Alias_IR = new System.Windows.Forms.TextBox();
            this.label35 = new System.Windows.Forms.Label();
            this.textBox_MB_Dim_IR = new System.Windows.Forms.TextBox();
            this.label36 = new System.Windows.Forms.Label();
            this.label44 = new System.Windows.Forms.Label();
            this.label45 = new System.Windows.Forms.Label();
            this.button_JSON_Validate_Resp = new System.Windows.Forms.Button();
            this.button_JSON_Validate_Pub = new System.Windows.Forms.Button();
            this.checkBox_ShowMsg = new System.Windows.Forms.CheckBox();
            this.groupBox9 = new System.Windows.Forms.GroupBox();
            this.button_res2res_reset = new System.Windows.Forms.Button();
            this.label48 = new System.Windows.Forms.Label();
            this.textBox_Res2Res = new System.Windows.Forms.TextBox();
            this.textBox_timer_pub2res = new System.Windows.Forms.TextBox();
            this.label46 = new System.Windows.Forms.Label();
            this.textBox_Message = new System.Windows.Forms.TextBox();
            this.textBox_SubTopic = new System.Windows.Forms.TextBox();
            this.label47 = new System.Windows.Forms.Label();
            this.textBox_Resp_Hash = new System.Windows.Forms.TextBox();
            this.label50 = new System.Windows.Forms.Label();
            this.label49 = new System.Windows.Forms.Label();
            this.textBox_Target = new System.Windows.Forms.TextBox();
            this.label51 = new System.Windows.Forms.Label();
            this.checkBox_TLS = new System.Windows.Forms.CheckBox();
            this.button_Save_settings = new System.Windows.Forms.Button();
            this.button_Load_settings = new System.Windows.Forms.Button();
            this.checkBox_Cfg_Dbg_Rel = new System.Windows.Forms.CheckBox();
            this.checkBox_Split_Resp = new System.Windows.Forms.CheckBox();
            this.button_CLS_Infos = new System.Windows.Forms.Button();
            this.label52 = new System.Windows.Forms.Label();
            this.textBox_MQTT_ID = new System.Windows.Forms.TextBox();
            this.textBox_MQTT_PWD = new System.Windows.Forms.TextBox();
            this.label53 = new System.Windows.Forms.Label();
            this.radioButton_qos0 = new System.Windows.Forms.RadioButton();
            this.groupBox11 = new System.Windows.Forms.GroupBox();
            this.radioButton_qos1 = new System.Windows.Forms.RadioButton();
            this.timer_check_is_alive = new System.Windows.Forms.Timer(this.components);
            this.radioButton_alive = new System.Windows.Forms.RadioButton();
            this.checkBox_mqtt_alive = new System.Windows.Forms.CheckBox();
            this.groupBox12 = new System.Windows.Forms.GroupBox();
            this.button_setDevsConfig = new System.Windows.Forms.Button();
            this.buttonUpdateCAData = new System.Windows.Forms.Button();
            this.buttonSet_UpdateGwFW = new System.Windows.Forms.Button();
            this.buttonDevUpdateData = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox7.SuspendLayout();
            this.groupBox10.SuspendLayout();
            this.groupBox8.SuspendLayout();
            this.groupBox9.SuspendLayout();
            this.groupBox11.SuspendLayout();
            this.groupBox12.SuspendLayout();
            this.SuspendLayout();
            // 
            // txtConsole
            // 
            this.txtConsole.AcceptsReturn = true;
            this.txtConsole.AcceptsTab = true;
            this.txtConsole.Location = new System.Drawing.Point(27, 465);
            this.txtConsole.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.txtConsole.Multiline = true;
            this.txtConsole.Name = "txtConsole";
            this.txtConsole.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtConsole.Size = new System.Drawing.Size(457, 260);
            this.txtConsole.TabIndex = 3;
            this.txtConsole.Text = "\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n";
            // 
            // buttonChangeCredential
            // 
            this.buttonChangeCredential.Location = new System.Drawing.Point(9, 23);
            this.buttonChangeCredential.Margin = new System.Windows.Forms.Padding(4);
            this.buttonChangeCredential.Name = "buttonChangeCredential";
            this.buttonChangeCredential.Size = new System.Drawing.Size(129, 46);
            this.buttonChangeCredential.TabIndex = 4;
            this.buttonChangeCredential.Text = "Change Credential";
            this.buttonChangeCredential.UseVisualStyleBackColor = true;
            this.buttonChangeCredential.Click += new System.EventHandler(this.ButtonChangeCredential_Click);
            // 
            // buttonReadValue
            // 
            this.buttonReadValue.Location = new System.Drawing.Point(9, 74);
            this.buttonReadValue.Margin = new System.Windows.Forms.Padding(4);
            this.buttonReadValue.Name = "buttonReadValue";
            this.buttonReadValue.Size = new System.Drawing.Size(129, 46);
            this.buttonReadValue.TabIndex = 5;
            this.buttonReadValue.Text = "Read Value";
            this.buttonReadValue.UseVisualStyleBackColor = true;
            this.buttonReadValue.Click += new System.EventHandler(this.ButtonReadValue_Click);
            // 
            // textBoxPublish
            // 
            this.textBoxPublish.Location = new System.Drawing.Point(27, 210);
            this.textBoxPublish.Margin = new System.Windows.Forms.Padding(4);
            this.textBoxPublish.Multiline = true;
            this.textBoxPublish.Name = "textBoxPublish";
            this.textBoxPublish.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxPublish.Size = new System.Drawing.Size(457, 189);
            this.textBoxPublish.TabIndex = 6;
            // 
            // labelPublish
            // 
            this.labelPublish.AutoSize = true;
            this.labelPublish.Location = new System.Drawing.Point(30, 183);
            this.labelPublish.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.labelPublish.Name = "labelPublish";
            this.labelPublish.Size = new System.Drawing.Size(54, 17);
            this.labelPublish.TabIndex = 7;
            this.labelPublish.Text = "Publish";
            // 
            // labelResponse
            // 
            this.labelResponse.AutoSize = true;
            this.labelResponse.Location = new System.Drawing.Point(23, 447);
            this.labelResponse.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.labelResponse.Name = "labelResponse";
            this.labelResponse.Size = new System.Drawing.Size(72, 17);
            this.labelResponse.TabIndex = 9;
            this.labelResponse.Text = "Response";
            // 
            // buttonClear
            // 
            this.buttonClear.Location = new System.Drawing.Point(300, 158);
            this.buttonClear.Margin = new System.Windows.Forms.Padding(4);
            this.buttonClear.Name = "buttonClear";
            this.buttonClear.Size = new System.Drawing.Size(57, 42);
            this.buttonClear.TabIndex = 10;
            this.buttonClear.Text = "CLS";
            this.buttonClear.UseVisualStyleBackColor = true;
            this.buttonClear.Click += new System.EventHandler(this.ButtonClear_Click);
            // 
            // buttonScanLine
            // 
            this.buttonScanLine.Location = new System.Drawing.Point(9, 127);
            this.buttonScanLine.Margin = new System.Windows.Forms.Padding(4);
            this.buttonScanLine.Name = "buttonScanLine";
            this.buttonScanLine.Size = new System.Drawing.Size(129, 46);
            this.buttonScanLine.TabIndex = 11;
            this.buttonScanLine.Text = "Scan Line";
            this.buttonScanLine.UseVisualStyleBackColor = true;
            this.buttonScanLine.Click += new System.EventHandler(this.ButtonScanLine_Click);
            // 
            // buttonSet_Devs_Config
            // 
            this.buttonSet_Devs_Config.Location = new System.Drawing.Point(9, 184);
            this.buttonSet_Devs_Config.Margin = new System.Windows.Forms.Padding(4);
            this.buttonSet_Devs_Config.Name = "buttonSet_Devs_Config";
            this.buttonSet_Devs_Config.Size = new System.Drawing.Size(129, 46);
            this.buttonSet_Devs_Config.TabIndex = 12;
            this.buttonSet_Devs_Config.Text = "set_devs_config";
            this.buttonSet_Devs_Config.UseVisualStyleBackColor = true;
            this.buttonSet_Devs_Config.Click += new System.EventHandler(this.buttonSet_Devs_Config_Click);
            // 
            // buttonSet_Lines_Config
            // 
            this.buttonSet_Lines_Config.Location = new System.Drawing.Point(9, 179);
            this.buttonSet_Lines_Config.Margin = new System.Windows.Forms.Padding(4);
            this.buttonSet_Lines_Config.Name = "buttonSet_Lines_Config";
            this.buttonSet_Lines_Config.Size = new System.Drawing.Size(129, 46);
            this.buttonSet_Lines_Config.TabIndex = 13;
            this.buttonSet_Lines_Config.Text = "set_lines_config";
            this.buttonSet_Lines_Config.UseVisualStyleBackColor = true;
            this.buttonSet_Lines_Config.Click += new System.EventHandler(this.ButtonSet_Lines_Config_Click);
            // 
            // buttonUpdate_ca_cerficates
            // 
            this.buttonUpdate_ca_cerficates.Location = new System.Drawing.Point(7, 24);
            this.buttonUpdate_ca_cerficates.Margin = new System.Windows.Forms.Padding(4);
            this.buttonUpdate_ca_cerficates.Name = "buttonUpdate_ca_cerficates";
            this.buttonUpdate_ca_cerficates.Size = new System.Drawing.Size(129, 46);
            this.buttonUpdate_ca_cerficates.TabIndex = 14;
            this.buttonUpdate_ca_cerficates.Text = "update_ca_cerficates";
            this.buttonUpdate_ca_cerficates.UseVisualStyleBackColor = true;
            this.buttonUpdate_ca_cerficates.Click += new System.EventHandler(this.ButtonUpdate_ca_cerficates_Click);
            // 
            // buttonUpdate_dev_firmware
            // 
            this.buttonUpdate_dev_firmware.Location = new System.Drawing.Point(7, 76);
            this.buttonUpdate_dev_firmware.Margin = new System.Windows.Forms.Padding(4);
            this.buttonUpdate_dev_firmware.Name = "buttonUpdate_dev_firmware";
            this.buttonUpdate_dev_firmware.Size = new System.Drawing.Size(129, 46);
            this.buttonUpdate_dev_firmware.TabIndex = 15;
            this.buttonUpdate_dev_firmware.Text = "update_dev_firmware";
            this.buttonUpdate_dev_firmware.UseVisualStyleBackColor = true;
            this.buttonUpdate_dev_firmware.Click += new System.EventHandler(this.ButtonUpdate_dev_firmware_Click);
            // 
            // buttonUpdate_gw_firmware
            // 
            this.buttonUpdate_gw_firmware.Location = new System.Drawing.Point(7, 130);
            this.buttonUpdate_gw_firmware.Margin = new System.Windows.Forms.Padding(4);
            this.buttonUpdate_gw_firmware.Name = "buttonUpdate_gw_firmware";
            this.buttonUpdate_gw_firmware.Size = new System.Drawing.Size(129, 46);
            this.buttonUpdate_gw_firmware.TabIndex = 16;
            this.buttonUpdate_gw_firmware.Text = "update_gw_firmware";
            this.buttonUpdate_gw_firmware.UseVisualStyleBackColor = true;
            this.buttonUpdate_gw_firmware.Click += new System.EventHandler(this.ButtonUpdate_gw_firmware_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(8, 17);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(50, 17);
            this.label2.TabIndex = 17;
            this.label2.Text = "Server";
            // 
            // textBoxMQTT_Server_URL
            // 
            this.textBoxMQTT_Server_URL.Location = new System.Drawing.Point(63, 12);
            this.textBoxMQTT_Server_URL.Margin = new System.Windows.Forms.Padding(4);
            this.textBoxMQTT_Server_URL.Name = "textBoxMQTT_Server_URL";
            this.textBoxMQTT_Server_URL.Size = new System.Drawing.Size(168, 22);
            this.textBoxMQTT_Server_URL.TabIndex = 18;
            this.textBoxMQTT_Server_URL.Text = "mqtt-dev.tera.systems";
            this.textBoxMQTT_Server_URL.TextChanged += new System.EventHandler(this.TextBoxMQTT_Server_URL_TextChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(237, 16);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(34, 17);
            this.label3.TabIndex = 19;
            this.label3.Text = "Port";
            // 
            // textBoxMQTT_Server_Port
            // 
            this.textBoxMQTT_Server_Port.Location = new System.Drawing.Point(280, 12);
            this.textBoxMQTT_Server_Port.Margin = new System.Windows.Forms.Padding(4);
            this.textBoxMQTT_Server_Port.Name = "textBoxMQTT_Server_Port";
            this.textBoxMQTT_Server_Port.Size = new System.Drawing.Size(55, 22);
            this.textBoxMQTT_Server_Port.TabIndex = 20;
            this.textBoxMQTT_Server_Port.Text = "8883";
            this.textBoxMQTT_Server_Port.TextChanged += new System.EventHandler(this.TextBoxMQTT_Server_Port_TextChanged);
            // 
            // buttonConnect
            // 
            this.buttonConnect.BackColor = System.Drawing.Color.Lime;
            this.buttonConnect.Location = new System.Drawing.Point(465, 2);
            this.buttonConnect.Margin = new System.Windows.Forms.Padding(4);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(75, 38);
            this.buttonConnect.TabIndex = 21;
            this.buttonConnect.Text = "Connect";
            this.buttonConnect.UseVisualStyleBackColor = false;
            this.buttonConnect.Click += new System.EventHandler(this.ButtonConnect_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.button_ReqDBGInfo);
            this.groupBox1.Controls.Add(this.buttonSet_Lines_Config_TTL);
            this.groupBox1.Controls.Add(this.button_flush);
            this.groupBox1.Controls.Add(this.button_reboot);
            this.groupBox1.Controls.Add(this.button_stop_engine);
            this.groupBox1.Controls.Add(this.button_start_engine);
            this.groupBox1.Controls.Add(this.button_test_set_gw_config_req);
            this.groupBox1.Controls.Add(this.button_send_mb_adu);
            this.groupBox1.Controls.Add(this.buttonReadValue);
            this.groupBox1.Controls.Add(this.buttonScanLine);
            this.groupBox1.Controls.Add(this.buttonSet_Lines_Config);
            this.groupBox1.Controls.Add(this.buttonChangeCredential);
            this.groupBox1.Location = new System.Drawing.Point(915, 134);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox1.Size = new System.Drawing.Size(285, 346);
            this.groupBox1.TabIndex = 22;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "MQTT Payloads";
            // 
            // button_ReqDBGInfo
            // 
            this.button_ReqDBGInfo.Location = new System.Drawing.Point(9, 286);
            this.button_ReqDBGInfo.Margin = new System.Windows.Forms.Padding(4);
            this.button_ReqDBGInfo.Name = "button_ReqDBGInfo";
            this.button_ReqDBGInfo.Size = new System.Drawing.Size(129, 46);
            this.button_ReqDBGInfo.TabIndex = 24;
            this.button_ReqDBGInfo.Text = "Req. DBG Info";
            this.button_ReqDBGInfo.UseVisualStyleBackColor = true;
            this.button_ReqDBGInfo.Click += new System.EventHandler(this.button_ReqDBGInfo_Click);
            // 
            // buttonSet_Lines_Config_TTL
            // 
            this.buttonSet_Lines_Config_TTL.Location = new System.Drawing.Point(141, 231);
            this.buttonSet_Lines_Config_TTL.Margin = new System.Windows.Forms.Padding(4);
            this.buttonSet_Lines_Config_TTL.Name = "buttonSet_Lines_Config_TTL";
            this.buttonSet_Lines_Config_TTL.Size = new System.Drawing.Size(129, 46);
            this.buttonSet_Lines_Config_TTL.TabIndex = 23;
            this.buttonSet_Lines_Config_TTL.Text = "set_lines_config TTL";
            this.buttonSet_Lines_Config_TTL.UseVisualStyleBackColor = true;
            this.buttonSet_Lines_Config_TTL.Click += new System.EventHandler(this.buttonSet_Lines_Config_TTL_Click);
            // 
            // button_flush
            // 
            this.button_flush.Location = new System.Drawing.Point(145, 179);
            this.button_flush.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button_flush.Name = "button_flush";
            this.button_flush.Size = new System.Drawing.Size(131, 44);
            this.button_flush.TabIndex = 22;
            this.button_flush.Text = "Flush";
            this.button_flush.UseVisualStyleBackColor = true;
            this.button_flush.Click += new System.EventHandler(this.button_flush_Click);
            // 
            // button_reboot
            // 
            this.button_reboot.Location = new System.Drawing.Point(145, 128);
            this.button_reboot.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button_reboot.Name = "button_reboot";
            this.button_reboot.Size = new System.Drawing.Size(131, 44);
            this.button_reboot.TabIndex = 21;
            this.button_reboot.Text = "ReBoot";
            this.button_reboot.UseVisualStyleBackColor = true;
            this.button_reboot.Click += new System.EventHandler(this.Button7_Click);
            // 
            // button_stop_engine
            // 
            this.button_stop_engine.Location = new System.Drawing.Point(145, 76);
            this.button_stop_engine.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button_stop_engine.Name = "button_stop_engine";
            this.button_stop_engine.Size = new System.Drawing.Size(131, 44);
            this.button_stop_engine.TabIndex = 20;
            this.button_stop_engine.Text = "Stop_Engine";
            this.button_stop_engine.UseVisualStyleBackColor = true;
            this.button_stop_engine.Click += new System.EventHandler(this.Button_stop_engine_Click);
            // 
            // button_start_engine
            // 
            this.button_start_engine.Location = new System.Drawing.Point(147, 23);
            this.button_start_engine.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button_start_engine.Name = "button_start_engine";
            this.button_start_engine.Size = new System.Drawing.Size(131, 44);
            this.button_start_engine.TabIndex = 19;
            this.button_start_engine.Text = "Start_Engine";
            this.button_start_engine.UseVisualStyleBackColor = true;
            this.button_start_engine.Click += new System.EventHandler(this.Button_start_engine_Click);
            // 
            // button_test_set_gw_config_req
            // 
            this.button_test_set_gw_config_req.Location = new System.Drawing.Point(9, 231);
            this.button_test_set_gw_config_req.Margin = new System.Windows.Forms.Padding(4);
            this.button_test_set_gw_config_req.Name = "button_test_set_gw_config_req";
            this.button_test_set_gw_config_req.Size = new System.Drawing.Size(129, 46);
            this.button_test_set_gw_config_req.TabIndex = 18;
            this.button_test_set_gw_config_req.Text = "set_gw_config-req";
            this.button_test_set_gw_config_req.UseVisualStyleBackColor = true;
            this.button_test_set_gw_config_req.Click += new System.EventHandler(this.Button_test_set_gw_config_req_Click);
            // 
            // button_send_mb_adu
            // 
            this.button_send_mb_adu.Location = new System.Drawing.Point(145, 284);
            this.button_send_mb_adu.Margin = new System.Windows.Forms.Padding(4);
            this.button_send_mb_adu.Name = "button_send_mb_adu";
            this.button_send_mb_adu.Size = new System.Drawing.Size(129, 46);
            this.button_send_mb_adu.TabIndex = 17;
            this.button_send_mb_adu.Text = "send_mb_adu";
            this.button_send_mb_adu.UseVisualStyleBackColor = true;
            this.button_send_mb_adu.Click += new System.EventHandler(this.Button_send_mb_adu_Click_1);
            // 
            // buttonClearResponse
            // 
            this.buttonClearResponse.Location = new System.Drawing.Point(301, 420);
            this.buttonClearResponse.Margin = new System.Windows.Forms.Padding(4);
            this.buttonClearResponse.Name = "buttonClearResponse";
            this.buttonClearResponse.Size = new System.Drawing.Size(57, 42);
            this.buttonClearResponse.TabIndex = 23;
            this.buttonClearResponse.Text = "CLS";
            this.buttonClearResponse.UseVisualStyleBackColor = true;
            this.buttonClearResponse.Click += new System.EventHandler(this.ButtonClearResponse_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.button_SetFlags);
            this.groupBox2.Controls.Add(this.textBox_MB_HR_R_Func);
            this.groupBox2.Controls.Add(this.button_MB_Write_HR);
            this.groupBox2.Controls.Add(this.button_MB_Read_HR);
            this.groupBox2.Controls.Add(this.label12);
            this.groupBox2.Controls.Add(this.textBox_Flags);
            this.groupBox2.Controls.Add(this.label11);
            this.groupBox2.Controls.Add(this.textBox_B);
            this.groupBox2.Controls.Add(this.textBox_A);
            this.groupBox2.Controls.Add(this.label10);
            this.groupBox2.Controls.Add(this.textBox_MB_Len);
            this.groupBox2.Controls.Add(this.label9);
            this.groupBox2.Controls.Add(this.textBox_MB_Pos);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.textBox_MB_Addr);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.textBox_Alias);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.textBox_MB_Dim);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.textBox_MB_HR_W_Func);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.textBox_HR_Val);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Location = new System.Drawing.Point(500, 134);
            this.groupBox2.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox2.Size = new System.Drawing.Size(195, 346);
            this.groupBox2.TabIndex = 24;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Modbus Value Req HR";
            // 
            // button_SetFlags
            // 
            this.button_SetFlags.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button_SetFlags.Location = new System.Drawing.Point(103, 261);
            this.button_SetFlags.Name = "button_SetFlags";
            this.button_SetFlags.Size = new System.Drawing.Size(66, 21);
            this.button_SetFlags.TabIndex = 45;
            this.button_SetFlags.Text = "< SET";
            this.button_SetFlags.UseVisualStyleBackColor = true;
            this.button_SetFlags.Click += new System.EventHandler(this.button_SetFlags_Click);
            // 
            // textBox_MB_HR_R_Func
            // 
            this.textBox_MB_HR_R_Func.Location = new System.Drawing.Point(44, 70);
            this.textBox_MB_HR_R_Func.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_MB_HR_R_Func.Name = "textBox_MB_HR_R_Func";
            this.textBox_MB_HR_R_Func.ReadOnly = true;
            this.textBox_MB_HR_R_Func.Size = new System.Drawing.Size(55, 22);
            this.textBox_MB_HR_R_Func.TabIndex = 44;
            this.textBox_MB_HR_R_Func.Text = "3";
            // 
            // button_MB_Write_HR
            // 
            this.button_MB_Write_HR.Location = new System.Drawing.Point(100, 298);
            this.button_MB_Write_HR.Margin = new System.Windows.Forms.Padding(4);
            this.button_MB_Write_HR.Name = "button_MB_Write_HR";
            this.button_MB_Write_HR.Size = new System.Drawing.Size(85, 32);
            this.button_MB_Write_HR.TabIndex = 43;
            this.button_MB_Write_HR.Text = "Write";
            this.button_MB_Write_HR.UseVisualStyleBackColor = true;
            this.button_MB_Write_HR.Click += new System.EventHandler(this.Button_MB_Write_HR_Click);
            // 
            // button_MB_Read_HR
            // 
            this.button_MB_Read_HR.Location = new System.Drawing.Point(12, 298);
            this.button_MB_Read_HR.Margin = new System.Windows.Forms.Padding(4);
            this.button_MB_Read_HR.Name = "button_MB_Read_HR";
            this.button_MB_Read_HR.Size = new System.Drawing.Size(85, 32);
            this.button_MB_Read_HR.TabIndex = 42;
            this.button_MB_Read_HR.Text = "Read";
            this.button_MB_Read_HR.UseVisualStyleBackColor = true;
            this.button_MB_Read_HR.Click += new System.EventHandler(this.Button_MB_Read_HR_Click);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(1, 265);
            this.label12.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(42, 17);
            this.label12.TabIndex = 41;
            this.label12.Text = "Flags";
            // 
            // textBox_Flags
            // 
            this.textBox_Flags.Location = new System.Drawing.Point(44, 260);
            this.textBox_Flags.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_Flags.Name = "textBox_Flags";
            this.textBox_Flags.Size = new System.Drawing.Size(55, 22);
            this.textBox_Flags.TabIndex = 40;
            this.textBox_Flags.Text = "0";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(11, 236);
            this.label11.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(16, 17);
            this.label11.TabIndex = 39;
            this.label11.Text = "b";
            // 
            // textBox_B
            // 
            this.textBox_B.Location = new System.Drawing.Point(44, 231);
            this.textBox_B.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_B.Name = "textBox_B";
            this.textBox_B.Size = new System.Drawing.Size(55, 22);
            this.textBox_B.TabIndex = 38;
            this.textBox_B.Text = "0.0";
            // 
            // textBox_A
            // 
            this.textBox_A.Location = new System.Drawing.Point(44, 204);
            this.textBox_A.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_A.Name = "textBox_A";
            this.textBox_A.Size = new System.Drawing.Size(55, 22);
            this.textBox_A.TabIndex = 37;
            this.textBox_A.Text = "1.0";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(9, 209);
            this.label10.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(16, 17);
            this.label10.TabIndex = 36;
            this.label10.Text = "a";
            // 
            // textBox_MB_Len
            // 
            this.textBox_MB_Len.Location = new System.Drawing.Point(44, 177);
            this.textBox_MB_Len.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_MB_Len.Name = "textBox_MB_Len";
            this.textBox_MB_Len.Size = new System.Drawing.Size(55, 22);
            this.textBox_MB_Len.TabIndex = 35;
            this.textBox_MB_Len.Text = "16";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(9, 182);
            this.label9.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(32, 17);
            this.label9.TabIndex = 34;
            this.label9.Text = "Len";
            // 
            // textBox_MB_Pos
            // 
            this.textBox_MB_Pos.Location = new System.Drawing.Point(44, 151);
            this.textBox_MB_Pos.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_MB_Pos.Name = "textBox_MB_Pos";
            this.textBox_MB_Pos.Size = new System.Drawing.Size(55, 22);
            this.textBox_MB_Pos.TabIndex = 33;
            this.textBox_MB_Pos.Text = "1";
            this.textBox_MB_Pos.TextChanged += new System.EventHandler(this.textBox_MB_Pos_TextChanged);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(9, 156);
            this.label8.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(32, 17);
            this.label8.TabIndex = 32;
            this.label8.Text = "Pos";
            // 
            // textBox_MB_Addr
            // 
            this.textBox_MB_Addr.Location = new System.Drawing.Point(44, 98);
            this.textBox_MB_Addr.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_MB_Addr.Name = "textBox_MB_Addr";
            this.textBox_MB_Addr.Size = new System.Drawing.Size(55, 22);
            this.textBox_MB_Addr.TabIndex = 31;
            this.textBox_MB_Addr.Text = "1";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(1, 102);
            this.label7.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(38, 17);
            this.label7.TabIndex = 30;
            this.label7.Text = "Addr";
            // 
            // textBox_Alias
            // 
            this.textBox_Alias.Location = new System.Drawing.Point(44, 20);
            this.textBox_Alias.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_Alias.Name = "textBox_Alias";
            this.textBox_Alias.Size = new System.Drawing.Size(55, 22);
            this.textBox_Alias.TabIndex = 28;
            this.textBox_Alias.Text = "33";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(7, 25);
            this.label6.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(38, 17);
            this.label6.TabIndex = 27;
            this.label6.Text = "Alias";
            // 
            // textBox_MB_Dim
            // 
            this.textBox_MB_Dim.Location = new System.Drawing.Point(44, 126);
            this.textBox_MB_Dim.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_MB_Dim.Name = "textBox_MB_Dim";
            this.textBox_MB_Dim.Size = new System.Drawing.Size(55, 22);
            this.textBox_MB_Dim.TabIndex = 26;
            this.textBox_MB_Dim.Text = "16";
            this.textBox_MB_Dim.TextChanged += new System.EventHandler(this.textBox_MB_Dim_TextChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(11, 130);
            this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(32, 17);
            this.label5.TabIndex = 25;
            this.label5.Text = "Dim";
            // 
            // textBox_MB_HR_W_Func
            // 
            this.textBox_MB_HR_W_Func.Location = new System.Drawing.Point(115, 70);
            this.textBox_MB_HR_W_Func.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_MB_HR_W_Func.Name = "textBox_MB_HR_W_Func";
            this.textBox_MB_HR_W_Func.ReadOnly = true;
            this.textBox_MB_HR_W_Func.Size = new System.Drawing.Size(55, 22);
            this.textBox_MB_HR_W_Func.TabIndex = 24;
            this.textBox_MB_HR_W_Func.Text = "6";
            this.textBox_MB_HR_W_Func.TextChanged += new System.EventHandler(this.TextBox1_TextChanged_1);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(11, 74);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(32, 17);
            this.label4.TabIndex = 23;
            this.label4.Text = "Fun";
            this.label4.Click += new System.EventHandler(this.Label4_Click);
            // 
            // textBox_HR_Val
            // 
            this.textBox_HR_Val.Location = new System.Drawing.Point(115, 43);
            this.textBox_HR_Val.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_HR_Val.Name = "textBox_HR_Val";
            this.textBox_HR_Val.Size = new System.Drawing.Size(55, 22);
            this.textBox_HR_Val.TabIndex = 22;
            this.textBox_HR_Val.Text = "345";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(11, 49);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(28, 17);
            this.label1.TabIndex = 21;
            this.label1.Text = "Val";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.groupBox5);
            this.groupBox3.Controls.Add(this.groupBox4);
            this.groupBox3.Controls.Add(this.textBox_MB_COIL_W_Func);
            this.groupBox3.Controls.Add(this.button_MB_Write_COIL);
            this.groupBox3.Controls.Add(this.button_MB_Read_COIL);
            this.groupBox3.Controls.Add(this.textBox_MB_Addr_Coil);
            this.groupBox3.Controls.Add(this.label18);
            this.groupBox3.Controls.Add(this.textBox_Alias_Coil);
            this.groupBox3.Controls.Add(this.label19);
            this.groupBox3.Controls.Add(this.textBox_MB_COIL_R_Func);
            this.groupBox3.Controls.Add(this.label21);
            this.groupBox3.Controls.Add(this.textBox_MB_COIL_Val);
            this.groupBox3.Controls.Add(this.label22);
            this.groupBox3.Location = new System.Drawing.Point(500, 498);
            this.groupBox3.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox3.Size = new System.Drawing.Size(195, 186);
            this.groupBox3.TabIndex = 44;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Modbus Value Req Coil";
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.groupBox6);
            this.groupBox5.Controls.Add(this.textBox11);
            this.groupBox5.Controls.Add(this.button5);
            this.groupBox5.Controls.Add(this.button6);
            this.groupBox5.Controls.Add(this.textBox12);
            this.groupBox5.Controls.Add(this.label25);
            this.groupBox5.Controls.Add(this.textBox13);
            this.groupBox5.Controls.Add(this.label26);
            this.groupBox5.Controls.Add(this.textBox14);
            this.groupBox5.Controls.Add(this.label27);
            this.groupBox5.Controls.Add(this.textBox15);
            this.groupBox5.Controls.Add(this.label28);
            this.groupBox5.Location = new System.Drawing.Point(212, 10);
            this.groupBox5.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox5.Size = new System.Drawing.Size(195, 186);
            this.groupBox5.TabIndex = 46;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Modbus Value Req Coil";
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.textBox6);
            this.groupBox6.Controls.Add(this.button3);
            this.groupBox6.Controls.Add(this.button4);
            this.groupBox6.Controls.Add(this.textBox7);
            this.groupBox6.Controls.Add(this.label17);
            this.groupBox6.Controls.Add(this.textBox8);
            this.groupBox6.Controls.Add(this.label20);
            this.groupBox6.Controls.Add(this.textBox9);
            this.groupBox6.Controls.Add(this.label23);
            this.groupBox6.Controls.Add(this.textBox10);
            this.groupBox6.Controls.Add(this.label24);
            this.groupBox6.Location = new System.Drawing.Point(229, 6);
            this.groupBox6.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox6.Size = new System.Drawing.Size(195, 186);
            this.groupBox6.TabIndex = 45;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Modbus Value Req Coil";
            // 
            // textBox6
            // 
            this.textBox6.Location = new System.Drawing.Point(115, 71);
            this.textBox6.Margin = new System.Windows.Forms.Padding(4);
            this.textBox6.Name = "textBox6";
            this.textBox6.ReadOnly = true;
            this.textBox6.Size = new System.Drawing.Size(55, 22);
            this.textBox6.TabIndex = 44;
            this.textBox6.Text = "15";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(100, 143);
            this.button3.Margin = new System.Windows.Forms.Padding(4);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(85, 32);
            this.button3.TabIndex = 43;
            this.button3.Text = "Write";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(12, 143);
            this.button4.Margin = new System.Windows.Forms.Padding(4);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(85, 32);
            this.button4.TabIndex = 42;
            this.button4.Text = "Read";
            this.button4.UseVisualStyleBackColor = true;
            // 
            // textBox7
            // 
            this.textBox7.Location = new System.Drawing.Point(44, 98);
            this.textBox7.Margin = new System.Windows.Forms.Padding(4);
            this.textBox7.Name = "textBox7";
            this.textBox7.Size = new System.Drawing.Size(55, 22);
            this.textBox7.TabIndex = 31;
            this.textBox7.Text = "1";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(1, 102);
            this.label17.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(38, 17);
            this.label17.TabIndex = 30;
            this.label17.Text = "Addr";
            // 
            // textBox8
            // 
            this.textBox8.Location = new System.Drawing.Point(44, 20);
            this.textBox8.Margin = new System.Windows.Forms.Padding(4);
            this.textBox8.Name = "textBox8";
            this.textBox8.Size = new System.Drawing.Size(55, 22);
            this.textBox8.TabIndex = 28;
            this.textBox8.Text = "1";
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(7, 25);
            this.label20.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(38, 17);
            this.label20.TabIndex = 27;
            this.label20.Text = "Alias";
            // 
            // textBox9
            // 
            this.textBox9.Location = new System.Drawing.Point(44, 71);
            this.textBox9.Margin = new System.Windows.Forms.Padding(4);
            this.textBox9.Name = "textBox9";
            this.textBox9.ReadOnly = true;
            this.textBox9.Size = new System.Drawing.Size(55, 22);
            this.textBox9.TabIndex = 24;
            this.textBox9.Text = "1";
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(11, 74);
            this.label23.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(32, 17);
            this.label23.TabIndex = 23;
            this.label23.Text = "Fun";
            // 
            // textBox10
            // 
            this.textBox10.Location = new System.Drawing.Point(115, 44);
            this.textBox10.Margin = new System.Windows.Forms.Padding(4);
            this.textBox10.Name = "textBox10";
            this.textBox10.Size = new System.Drawing.Size(55, 22);
            this.textBox10.TabIndex = 22;
            this.textBox10.Text = "0";
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(11, 49);
            this.label24.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(28, 17);
            this.label24.TabIndex = 21;
            this.label24.Text = "Val";
            // 
            // textBox11
            // 
            this.textBox11.Location = new System.Drawing.Point(115, 71);
            this.textBox11.Margin = new System.Windows.Forms.Padding(4);
            this.textBox11.Name = "textBox11";
            this.textBox11.ReadOnly = true;
            this.textBox11.Size = new System.Drawing.Size(55, 22);
            this.textBox11.TabIndex = 44;
            this.textBox11.Text = "15";
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(100, 143);
            this.button5.Margin = new System.Windows.Forms.Padding(4);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(85, 32);
            this.button5.TabIndex = 43;
            this.button5.Text = "Write";
            this.button5.UseVisualStyleBackColor = true;
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(12, 143);
            this.button6.Margin = new System.Windows.Forms.Padding(4);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(85, 32);
            this.button6.TabIndex = 42;
            this.button6.Text = "Read";
            this.button6.UseVisualStyleBackColor = true;
            // 
            // textBox12
            // 
            this.textBox12.Location = new System.Drawing.Point(44, 98);
            this.textBox12.Margin = new System.Windows.Forms.Padding(4);
            this.textBox12.Name = "textBox12";
            this.textBox12.Size = new System.Drawing.Size(55, 22);
            this.textBox12.TabIndex = 31;
            this.textBox12.Text = "1";
            // 
            // label25
            // 
            this.label25.AutoSize = true;
            this.label25.Location = new System.Drawing.Point(1, 102);
            this.label25.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(38, 17);
            this.label25.TabIndex = 30;
            this.label25.Text = "Addr";
            // 
            // textBox13
            // 
            this.textBox13.Location = new System.Drawing.Point(44, 20);
            this.textBox13.Margin = new System.Windows.Forms.Padding(4);
            this.textBox13.Name = "textBox13";
            this.textBox13.Size = new System.Drawing.Size(55, 22);
            this.textBox13.TabIndex = 28;
            this.textBox13.Text = "1";
            // 
            // label26
            // 
            this.label26.AutoSize = true;
            this.label26.Location = new System.Drawing.Point(7, 25);
            this.label26.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(38, 17);
            this.label26.TabIndex = 27;
            this.label26.Text = "Alias";
            // 
            // textBox14
            // 
            this.textBox14.Location = new System.Drawing.Point(44, 71);
            this.textBox14.Margin = new System.Windows.Forms.Padding(4);
            this.textBox14.Name = "textBox14";
            this.textBox14.ReadOnly = true;
            this.textBox14.Size = new System.Drawing.Size(55, 22);
            this.textBox14.TabIndex = 24;
            this.textBox14.Text = "1";
            // 
            // label27
            // 
            this.label27.AutoSize = true;
            this.label27.Location = new System.Drawing.Point(11, 74);
            this.label27.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label27.Name = "label27";
            this.label27.Size = new System.Drawing.Size(32, 17);
            this.label27.TabIndex = 23;
            this.label27.Text = "Fun";
            // 
            // textBox15
            // 
            this.textBox15.Location = new System.Drawing.Point(115, 44);
            this.textBox15.Margin = new System.Windows.Forms.Padding(4);
            this.textBox15.Name = "textBox15";
            this.textBox15.Size = new System.Drawing.Size(55, 22);
            this.textBox15.TabIndex = 22;
            this.textBox15.Text = "0";
            // 
            // label28
            // 
            this.label28.AutoSize = true;
            this.label28.Location = new System.Drawing.Point(11, 49);
            this.label28.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label28.Name = "label28";
            this.label28.Size = new System.Drawing.Size(28, 17);
            this.label28.TabIndex = 21;
            this.label28.Text = "Val";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.textBox1);
            this.groupBox4.Controls.Add(this.button1);
            this.groupBox4.Controls.Add(this.button2);
            this.groupBox4.Controls.Add(this.textBox2);
            this.groupBox4.Controls.Add(this.label13);
            this.groupBox4.Controls.Add(this.textBox3);
            this.groupBox4.Controls.Add(this.label14);
            this.groupBox4.Controls.Add(this.textBox4);
            this.groupBox4.Controls.Add(this.label15);
            this.groupBox4.Controls.Add(this.textBox5);
            this.groupBox4.Controls.Add(this.label16);
            this.groupBox4.Location = new System.Drawing.Point(229, 6);
            this.groupBox4.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox4.Size = new System.Drawing.Size(195, 186);
            this.groupBox4.TabIndex = 45;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Modbus Value Req Coil";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(115, 71);
            this.textBox1.Margin = new System.Windows.Forms.Padding(4);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(55, 22);
            this.textBox1.TabIndex = 44;
            this.textBox1.Text = "15";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(100, 143);
            this.button1.Margin = new System.Windows.Forms.Padding(4);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(85, 32);
            this.button1.TabIndex = 43;
            this.button1.Text = "Write";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(12, 143);
            this.button2.Margin = new System.Windows.Forms.Padding(4);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(85, 32);
            this.button2.TabIndex = 42;
            this.button2.Text = "Read";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(44, 98);
            this.textBox2.Margin = new System.Windows.Forms.Padding(4);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(55, 22);
            this.textBox2.TabIndex = 31;
            this.textBox2.Text = "1";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(1, 102);
            this.label13.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(38, 17);
            this.label13.TabIndex = 30;
            this.label13.Text = "Addr";
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(44, 20);
            this.textBox3.Margin = new System.Windows.Forms.Padding(4);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(55, 22);
            this.textBox3.TabIndex = 28;
            this.textBox3.Text = "1";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(7, 25);
            this.label14.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(38, 17);
            this.label14.TabIndex = 27;
            this.label14.Text = "Alias";
            // 
            // textBox4
            // 
            this.textBox4.Location = new System.Drawing.Point(44, 71);
            this.textBox4.Margin = new System.Windows.Forms.Padding(4);
            this.textBox4.Name = "textBox4";
            this.textBox4.ReadOnly = true;
            this.textBox4.Size = new System.Drawing.Size(55, 22);
            this.textBox4.TabIndex = 24;
            this.textBox4.Text = "1";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(11, 74);
            this.label15.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(32, 17);
            this.label15.TabIndex = 23;
            this.label15.Text = "Fun";
            // 
            // textBox5
            // 
            this.textBox5.Location = new System.Drawing.Point(115, 44);
            this.textBox5.Margin = new System.Windows.Forms.Padding(4);
            this.textBox5.Name = "textBox5";
            this.textBox5.Size = new System.Drawing.Size(55, 22);
            this.textBox5.TabIndex = 22;
            this.textBox5.Text = "0";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(11, 49);
            this.label16.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(28, 17);
            this.label16.TabIndex = 21;
            this.label16.Text = "Val";
            // 
            // textBox_MB_COIL_W_Func
            // 
            this.textBox_MB_COIL_W_Func.Location = new System.Drawing.Point(115, 71);
            this.textBox_MB_COIL_W_Func.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_MB_COIL_W_Func.Name = "textBox_MB_COIL_W_Func";
            this.textBox_MB_COIL_W_Func.ReadOnly = true;
            this.textBox_MB_COIL_W_Func.Size = new System.Drawing.Size(55, 22);
            this.textBox_MB_COIL_W_Func.TabIndex = 44;
            this.textBox_MB_COIL_W_Func.Text = "5";
            // 
            // button_MB_Write_COIL
            // 
            this.button_MB_Write_COIL.Location = new System.Drawing.Point(100, 143);
            this.button_MB_Write_COIL.Margin = new System.Windows.Forms.Padding(4);
            this.button_MB_Write_COIL.Name = "button_MB_Write_COIL";
            this.button_MB_Write_COIL.Size = new System.Drawing.Size(85, 32);
            this.button_MB_Write_COIL.TabIndex = 43;
            this.button_MB_Write_COIL.Text = "Write";
            this.button_MB_Write_COIL.UseVisualStyleBackColor = true;
            this.button_MB_Write_COIL.Click += new System.EventHandler(this.Button_MB_Write_COIL_Click);
            // 
            // button_MB_Read_COIL
            // 
            this.button_MB_Read_COIL.Location = new System.Drawing.Point(12, 143);
            this.button_MB_Read_COIL.Margin = new System.Windows.Forms.Padding(4);
            this.button_MB_Read_COIL.Name = "button_MB_Read_COIL";
            this.button_MB_Read_COIL.Size = new System.Drawing.Size(85, 32);
            this.button_MB_Read_COIL.TabIndex = 42;
            this.button_MB_Read_COIL.Text = "Read";
            this.button_MB_Read_COIL.UseVisualStyleBackColor = true;
            this.button_MB_Read_COIL.Click += new System.EventHandler(this.Button_MB_Read_COIL_Click);
            // 
            // textBox_MB_Addr_Coil
            // 
            this.textBox_MB_Addr_Coil.Location = new System.Drawing.Point(44, 98);
            this.textBox_MB_Addr_Coil.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_MB_Addr_Coil.Name = "textBox_MB_Addr_Coil";
            this.textBox_MB_Addr_Coil.Size = new System.Drawing.Size(55, 22);
            this.textBox_MB_Addr_Coil.TabIndex = 31;
            this.textBox_MB_Addr_Coil.Text = "1";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(1, 102);
            this.label18.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(38, 17);
            this.label18.TabIndex = 30;
            this.label18.Text = "Addr";
            // 
            // textBox_Alias_Coil
            // 
            this.textBox_Alias_Coil.Location = new System.Drawing.Point(44, 20);
            this.textBox_Alias_Coil.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_Alias_Coil.Name = "textBox_Alias_Coil";
            this.textBox_Alias_Coil.Size = new System.Drawing.Size(55, 22);
            this.textBox_Alias_Coil.TabIndex = 28;
            this.textBox_Alias_Coil.Text = "1";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(7, 25);
            this.label19.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(38, 17);
            this.label19.TabIndex = 27;
            this.label19.Text = "Alias";
            // 
            // textBox_MB_COIL_R_Func
            // 
            this.textBox_MB_COIL_R_Func.Location = new System.Drawing.Point(44, 71);
            this.textBox_MB_COIL_R_Func.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_MB_COIL_R_Func.Name = "textBox_MB_COIL_R_Func";
            this.textBox_MB_COIL_R_Func.ReadOnly = true;
            this.textBox_MB_COIL_R_Func.Size = new System.Drawing.Size(55, 22);
            this.textBox_MB_COIL_R_Func.TabIndex = 24;
            this.textBox_MB_COIL_R_Func.Text = "1";
            this.textBox_MB_COIL_R_Func.TextChanged += new System.EventHandler(this.TextBox_MB_COIL_R_Func_TextChanged);
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(11, 74);
            this.label21.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(32, 17);
            this.label21.TabIndex = 23;
            this.label21.Text = "Fun";
            // 
            // textBox_MB_COIL_Val
            // 
            this.textBox_MB_COIL_Val.Location = new System.Drawing.Point(115, 44);
            this.textBox_MB_COIL_Val.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_MB_COIL_Val.Name = "textBox_MB_COIL_Val";
            this.textBox_MB_COIL_Val.Size = new System.Drawing.Size(55, 22);
            this.textBox_MB_COIL_Val.TabIndex = 22;
            this.textBox_MB_COIL_Val.Text = "0";
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(11, 49);
            this.label22.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(28, 17);
            this.label22.TabIndex = 21;
            this.label22.Text = "Val";
            // 
            // groupBox7
            // 
            this.groupBox7.Controls.Add(this.groupBox10);
            this.groupBox7.Controls.Add(this.button_MB_Read_DI);
            this.groupBox7.Controls.Add(this.textBox_MB_Addr_DI);
            this.groupBox7.Controls.Add(this.label41);
            this.groupBox7.Controls.Add(this.textBox_Alias_DI);
            this.groupBox7.Controls.Add(this.label42);
            this.groupBox7.Controls.Add(this.textBox_MB_DI_R_Func);
            this.groupBox7.Controls.Add(this.label43);
            this.groupBox7.Location = new System.Drawing.Point(712, 497);
            this.groupBox7.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox7.Name = "groupBox7";
            this.groupBox7.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox7.Size = new System.Drawing.Size(195, 186);
            this.groupBox7.TabIndex = 47;
            this.groupBox7.TabStop = false;
            this.groupBox7.Text = "Modbus Value Req DI";
            // 
            // groupBox10
            // 
            this.groupBox10.Controls.Add(this.textBox26);
            this.groupBox10.Controls.Add(this.button11);
            this.groupBox10.Controls.Add(this.button12);
            this.groupBox10.Controls.Add(this.textBox27);
            this.groupBox10.Controls.Add(this.label37);
            this.groupBox10.Controls.Add(this.textBox28);
            this.groupBox10.Controls.Add(this.label38);
            this.groupBox10.Controls.Add(this.textBox29);
            this.groupBox10.Controls.Add(this.label39);
            this.groupBox10.Controls.Add(this.textBox30);
            this.groupBox10.Controls.Add(this.label40);
            this.groupBox10.Location = new System.Drawing.Point(229, 6);
            this.groupBox10.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox10.Name = "groupBox10";
            this.groupBox10.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox10.Size = new System.Drawing.Size(195, 186);
            this.groupBox10.TabIndex = 45;
            this.groupBox10.TabStop = false;
            this.groupBox10.Text = "Modbus Value Req Coil";
            // 
            // textBox26
            // 
            this.textBox26.Location = new System.Drawing.Point(115, 71);
            this.textBox26.Margin = new System.Windows.Forms.Padding(4);
            this.textBox26.Name = "textBox26";
            this.textBox26.ReadOnly = true;
            this.textBox26.Size = new System.Drawing.Size(55, 22);
            this.textBox26.TabIndex = 44;
            this.textBox26.Text = "15";
            // 
            // button11
            // 
            this.button11.Location = new System.Drawing.Point(100, 143);
            this.button11.Margin = new System.Windows.Forms.Padding(4);
            this.button11.Name = "button11";
            this.button11.Size = new System.Drawing.Size(85, 32);
            this.button11.TabIndex = 43;
            this.button11.Text = "Write";
            this.button11.UseVisualStyleBackColor = true;
            // 
            // button12
            // 
            this.button12.Location = new System.Drawing.Point(12, 143);
            this.button12.Margin = new System.Windows.Forms.Padding(4);
            this.button12.Name = "button12";
            this.button12.Size = new System.Drawing.Size(85, 32);
            this.button12.TabIndex = 42;
            this.button12.Text = "Read";
            this.button12.UseVisualStyleBackColor = true;
            // 
            // textBox27
            // 
            this.textBox27.Location = new System.Drawing.Point(44, 98);
            this.textBox27.Margin = new System.Windows.Forms.Padding(4);
            this.textBox27.Name = "textBox27";
            this.textBox27.Size = new System.Drawing.Size(55, 22);
            this.textBox27.TabIndex = 31;
            this.textBox27.Text = "1";
            // 
            // label37
            // 
            this.label37.AutoSize = true;
            this.label37.Location = new System.Drawing.Point(1, 102);
            this.label37.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label37.Name = "label37";
            this.label37.Size = new System.Drawing.Size(38, 17);
            this.label37.TabIndex = 30;
            this.label37.Text = "Addr";
            // 
            // textBox28
            // 
            this.textBox28.Location = new System.Drawing.Point(44, 20);
            this.textBox28.Margin = new System.Windows.Forms.Padding(4);
            this.textBox28.Name = "textBox28";
            this.textBox28.Size = new System.Drawing.Size(55, 22);
            this.textBox28.TabIndex = 28;
            this.textBox28.Text = "1";
            // 
            // label38
            // 
            this.label38.AutoSize = true;
            this.label38.Location = new System.Drawing.Point(7, 25);
            this.label38.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label38.Name = "label38";
            this.label38.Size = new System.Drawing.Size(38, 17);
            this.label38.TabIndex = 27;
            this.label38.Text = "Alias";
            // 
            // textBox29
            // 
            this.textBox29.Location = new System.Drawing.Point(44, 71);
            this.textBox29.Margin = new System.Windows.Forms.Padding(4);
            this.textBox29.Name = "textBox29";
            this.textBox29.ReadOnly = true;
            this.textBox29.Size = new System.Drawing.Size(55, 22);
            this.textBox29.TabIndex = 24;
            this.textBox29.Text = "1";
            // 
            // label39
            // 
            this.label39.AutoSize = true;
            this.label39.Location = new System.Drawing.Point(11, 74);
            this.label39.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label39.Name = "label39";
            this.label39.Size = new System.Drawing.Size(32, 17);
            this.label39.TabIndex = 23;
            this.label39.Text = "Fun";
            // 
            // textBox30
            // 
            this.textBox30.Location = new System.Drawing.Point(115, 44);
            this.textBox30.Margin = new System.Windows.Forms.Padding(4);
            this.textBox30.Name = "textBox30";
            this.textBox30.Size = new System.Drawing.Size(55, 22);
            this.textBox30.TabIndex = 22;
            this.textBox30.Text = "0";
            // 
            // label40
            // 
            this.label40.AutoSize = true;
            this.label40.Location = new System.Drawing.Point(11, 49);
            this.label40.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label40.Name = "label40";
            this.label40.Size = new System.Drawing.Size(28, 17);
            this.label40.TabIndex = 21;
            this.label40.Text = "Val";
            // 
            // button_MB_Read_DI
            // 
            this.button_MB_Read_DI.Location = new System.Drawing.Point(12, 143);
            this.button_MB_Read_DI.Margin = new System.Windows.Forms.Padding(4);
            this.button_MB_Read_DI.Name = "button_MB_Read_DI";
            this.button_MB_Read_DI.Size = new System.Drawing.Size(85, 32);
            this.button_MB_Read_DI.TabIndex = 42;
            this.button_MB_Read_DI.Text = "Read";
            this.button_MB_Read_DI.UseVisualStyleBackColor = true;
            this.button_MB_Read_DI.Click += new System.EventHandler(this.Button_MB_Read_DI_Click);
            // 
            // textBox_MB_Addr_DI
            // 
            this.textBox_MB_Addr_DI.Location = new System.Drawing.Point(44, 98);
            this.textBox_MB_Addr_DI.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_MB_Addr_DI.Name = "textBox_MB_Addr_DI";
            this.textBox_MB_Addr_DI.Size = new System.Drawing.Size(55, 22);
            this.textBox_MB_Addr_DI.TabIndex = 31;
            this.textBox_MB_Addr_DI.Text = "1";
            // 
            // label41
            // 
            this.label41.AutoSize = true;
            this.label41.Location = new System.Drawing.Point(1, 102);
            this.label41.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label41.Name = "label41";
            this.label41.Size = new System.Drawing.Size(38, 17);
            this.label41.TabIndex = 30;
            this.label41.Text = "Addr";
            // 
            // textBox_Alias_DI
            // 
            this.textBox_Alias_DI.Location = new System.Drawing.Point(44, 20);
            this.textBox_Alias_DI.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_Alias_DI.Name = "textBox_Alias_DI";
            this.textBox_Alias_DI.Size = new System.Drawing.Size(55, 22);
            this.textBox_Alias_DI.TabIndex = 28;
            this.textBox_Alias_DI.Text = "1";
            // 
            // label42
            // 
            this.label42.AutoSize = true;
            this.label42.Location = new System.Drawing.Point(7, 25);
            this.label42.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label42.Name = "label42";
            this.label42.Size = new System.Drawing.Size(38, 17);
            this.label42.TabIndex = 27;
            this.label42.Text = "Alias";
            // 
            // textBox_MB_DI_R_Func
            // 
            this.textBox_MB_DI_R_Func.Location = new System.Drawing.Point(44, 71);
            this.textBox_MB_DI_R_Func.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_MB_DI_R_Func.Name = "textBox_MB_DI_R_Func";
            this.textBox_MB_DI_R_Func.ReadOnly = true;
            this.textBox_MB_DI_R_Func.Size = new System.Drawing.Size(55, 22);
            this.textBox_MB_DI_R_Func.TabIndex = 24;
            this.textBox_MB_DI_R_Func.Text = "2";
            // 
            // label43
            // 
            this.label43.AutoSize = true;
            this.label43.Location = new System.Drawing.Point(11, 74);
            this.label43.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label43.Name = "label43";
            this.label43.Size = new System.Drawing.Size(32, 17);
            this.label43.TabIndex = 23;
            this.label43.Text = "Fun";
            // 
            // groupBox8
            // 
            this.groupBox8.Controls.Add(this.button_SetFlags_IR);
            this.groupBox8.Controls.Add(this.textBox_MB_IR_R_Func);
            this.groupBox8.Controls.Add(this.button_MB_Read_IR);
            this.groupBox8.Controls.Add(this.label29);
            this.groupBox8.Controls.Add(this.textBox_Flags_IR);
            this.groupBox8.Controls.Add(this.label30);
            this.groupBox8.Controls.Add(this.textBox_B_IR);
            this.groupBox8.Controls.Add(this.textBox_A_IR);
            this.groupBox8.Controls.Add(this.label31);
            this.groupBox8.Controls.Add(this.textBox_MB_Len_IR);
            this.groupBox8.Controls.Add(this.label32);
            this.groupBox8.Controls.Add(this.textBox_MB_Pos_IR);
            this.groupBox8.Controls.Add(this.label33);
            this.groupBox8.Controls.Add(this.textBox_MB_Addr_IR);
            this.groupBox8.Controls.Add(this.label34);
            this.groupBox8.Controls.Add(this.textBox_Alias_IR);
            this.groupBox8.Controls.Add(this.label35);
            this.groupBox8.Controls.Add(this.textBox_MB_Dim_IR);
            this.groupBox8.Controls.Add(this.label36);
            this.groupBox8.Controls.Add(this.label44);
            this.groupBox8.Controls.Add(this.label45);
            this.groupBox8.Location = new System.Drawing.Point(712, 134);
            this.groupBox8.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox8.Name = "groupBox8";
            this.groupBox8.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox8.Size = new System.Drawing.Size(195, 346);
            this.groupBox8.TabIndex = 45;
            this.groupBox8.TabStop = false;
            this.groupBox8.Text = "Modbus Value Req IR";
            // 
            // button_SetFlags_IR
            // 
            this.button_SetFlags_IR.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button_SetFlags_IR.Location = new System.Drawing.Point(104, 261);
            this.button_SetFlags_IR.Name = "button_SetFlags_IR";
            this.button_SetFlags_IR.Size = new System.Drawing.Size(66, 21);
            this.button_SetFlags_IR.TabIndex = 46;
            this.button_SetFlags_IR.Text = "< SET";
            this.button_SetFlags_IR.UseVisualStyleBackColor = true;
            this.button_SetFlags_IR.Click += new System.EventHandler(this.button_SetFlags_IR_Click);
            // 
            // textBox_MB_IR_R_Func
            // 
            this.textBox_MB_IR_R_Func.Location = new System.Drawing.Point(44, 70);
            this.textBox_MB_IR_R_Func.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_MB_IR_R_Func.Name = "textBox_MB_IR_R_Func";
            this.textBox_MB_IR_R_Func.ReadOnly = true;
            this.textBox_MB_IR_R_Func.Size = new System.Drawing.Size(55, 22);
            this.textBox_MB_IR_R_Func.TabIndex = 44;
            this.textBox_MB_IR_R_Func.Text = "4";
            // 
            // button_MB_Read_IR
            // 
            this.button_MB_Read_IR.Location = new System.Drawing.Point(12, 298);
            this.button_MB_Read_IR.Margin = new System.Windows.Forms.Padding(4);
            this.button_MB_Read_IR.Name = "button_MB_Read_IR";
            this.button_MB_Read_IR.Size = new System.Drawing.Size(85, 32);
            this.button_MB_Read_IR.TabIndex = 42;
            this.button_MB_Read_IR.Text = "Read";
            this.button_MB_Read_IR.UseVisualStyleBackColor = true;
            this.button_MB_Read_IR.Click += new System.EventHandler(this.Button_MB_Read_IR_Click);
            // 
            // label29
            // 
            this.label29.AutoSize = true;
            this.label29.Location = new System.Drawing.Point(1, 265);
            this.label29.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label29.Name = "label29";
            this.label29.Size = new System.Drawing.Size(42, 17);
            this.label29.TabIndex = 41;
            this.label29.Text = "Flags";
            // 
            // textBox_Flags_IR
            // 
            this.textBox_Flags_IR.Location = new System.Drawing.Point(44, 260);
            this.textBox_Flags_IR.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_Flags_IR.Name = "textBox_Flags_IR";
            this.textBox_Flags_IR.Size = new System.Drawing.Size(55, 22);
            this.textBox_Flags_IR.TabIndex = 40;
            this.textBox_Flags_IR.Text = "0";
            // 
            // label30
            // 
            this.label30.AutoSize = true;
            this.label30.Location = new System.Drawing.Point(11, 236);
            this.label30.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label30.Name = "label30";
            this.label30.Size = new System.Drawing.Size(16, 17);
            this.label30.TabIndex = 39;
            this.label30.Text = "b";
            // 
            // textBox_B_IR
            // 
            this.textBox_B_IR.Location = new System.Drawing.Point(44, 231);
            this.textBox_B_IR.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_B_IR.Name = "textBox_B_IR";
            this.textBox_B_IR.Size = new System.Drawing.Size(55, 22);
            this.textBox_B_IR.TabIndex = 38;
            this.textBox_B_IR.Text = "0.0";
            // 
            // textBox_A_IR
            // 
            this.textBox_A_IR.Location = new System.Drawing.Point(44, 204);
            this.textBox_A_IR.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_A_IR.Name = "textBox_A_IR";
            this.textBox_A_IR.Size = new System.Drawing.Size(55, 22);
            this.textBox_A_IR.TabIndex = 37;
            this.textBox_A_IR.Text = "1.0";
            // 
            // label31
            // 
            this.label31.AutoSize = true;
            this.label31.Location = new System.Drawing.Point(9, 209);
            this.label31.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label31.Name = "label31";
            this.label31.Size = new System.Drawing.Size(16, 17);
            this.label31.TabIndex = 36;
            this.label31.Text = "a";
            // 
            // textBox_MB_Len_IR
            // 
            this.textBox_MB_Len_IR.Location = new System.Drawing.Point(44, 177);
            this.textBox_MB_Len_IR.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_MB_Len_IR.Name = "textBox_MB_Len_IR";
            this.textBox_MB_Len_IR.Size = new System.Drawing.Size(55, 22);
            this.textBox_MB_Len_IR.TabIndex = 35;
            this.textBox_MB_Len_IR.Text = "16";
            // 
            // label32
            // 
            this.label32.AutoSize = true;
            this.label32.Location = new System.Drawing.Point(9, 182);
            this.label32.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label32.Name = "label32";
            this.label32.Size = new System.Drawing.Size(32, 17);
            this.label32.TabIndex = 34;
            this.label32.Text = "Len";
            // 
            // textBox_MB_Pos_IR
            // 
            this.textBox_MB_Pos_IR.Location = new System.Drawing.Point(44, 151);
            this.textBox_MB_Pos_IR.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_MB_Pos_IR.Name = "textBox_MB_Pos_IR";
            this.textBox_MB_Pos_IR.Size = new System.Drawing.Size(55, 22);
            this.textBox_MB_Pos_IR.TabIndex = 33;
            this.textBox_MB_Pos_IR.Text = "1";
            // 
            // label33
            // 
            this.label33.AutoSize = true;
            this.label33.Location = new System.Drawing.Point(9, 156);
            this.label33.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label33.Name = "label33";
            this.label33.Size = new System.Drawing.Size(32, 17);
            this.label33.TabIndex = 32;
            this.label33.Text = "Pos";
            // 
            // textBox_MB_Addr_IR
            // 
            this.textBox_MB_Addr_IR.Location = new System.Drawing.Point(44, 98);
            this.textBox_MB_Addr_IR.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_MB_Addr_IR.Name = "textBox_MB_Addr_IR";
            this.textBox_MB_Addr_IR.Size = new System.Drawing.Size(55, 22);
            this.textBox_MB_Addr_IR.TabIndex = 31;
            this.textBox_MB_Addr_IR.Text = "1";
            // 
            // label34
            // 
            this.label34.AutoSize = true;
            this.label34.Location = new System.Drawing.Point(1, 102);
            this.label34.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label34.Name = "label34";
            this.label34.Size = new System.Drawing.Size(38, 17);
            this.label34.TabIndex = 30;
            this.label34.Text = "Addr";
            // 
            // textBox_Alias_IR
            // 
            this.textBox_Alias_IR.Location = new System.Drawing.Point(44, 20);
            this.textBox_Alias_IR.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_Alias_IR.Name = "textBox_Alias_IR";
            this.textBox_Alias_IR.Size = new System.Drawing.Size(55, 22);
            this.textBox_Alias_IR.TabIndex = 28;
            this.textBox_Alias_IR.Text = "33";
            // 
            // label35
            // 
            this.label35.AutoSize = true;
            this.label35.Location = new System.Drawing.Point(7, 25);
            this.label35.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label35.Name = "label35";
            this.label35.Size = new System.Drawing.Size(38, 17);
            this.label35.TabIndex = 27;
            this.label35.Text = "Alias";
            // 
            // textBox_MB_Dim_IR
            // 
            this.textBox_MB_Dim_IR.Location = new System.Drawing.Point(44, 126);
            this.textBox_MB_Dim_IR.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_MB_Dim_IR.Name = "textBox_MB_Dim_IR";
            this.textBox_MB_Dim_IR.Size = new System.Drawing.Size(55, 22);
            this.textBox_MB_Dim_IR.TabIndex = 26;
            this.textBox_MB_Dim_IR.Text = "16";
            // 
            // label36
            // 
            this.label36.AutoSize = true;
            this.label36.Location = new System.Drawing.Point(11, 130);
            this.label36.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label36.Name = "label36";
            this.label36.Size = new System.Drawing.Size(32, 17);
            this.label36.TabIndex = 25;
            this.label36.Text = "Dim";
            // 
            // label44
            // 
            this.label44.AutoSize = true;
            this.label44.Location = new System.Drawing.Point(11, 74);
            this.label44.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label44.Name = "label44";
            this.label44.Size = new System.Drawing.Size(32, 17);
            this.label44.TabIndex = 23;
            this.label44.Text = "Fun";
            // 
            // label45
            // 
            this.label45.AutoSize = true;
            this.label45.Location = new System.Drawing.Point(11, 49);
            this.label45.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label45.Name = "label45";
            this.label45.Size = new System.Drawing.Size(28, 17);
            this.label45.TabIndex = 21;
            this.label45.Text = "Val";
            // 
            // button_JSON_Validate_Resp
            // 
            this.button_JSON_Validate_Resp.Location = new System.Drawing.Point(367, 420);
            this.button_JSON_Validate_Resp.Margin = new System.Windows.Forms.Padding(4);
            this.button_JSON_Validate_Resp.Name = "button_JSON_Validate_Resp";
            this.button_JSON_Validate_Resp.Size = new System.Drawing.Size(119, 42);
            this.button_JSON_Validate_Resp.TabIndex = 48;
            this.button_JSON_Validate_Resp.Text = "CBOR WEB";
            this.button_JSON_Validate_Resp.UseVisualStyleBackColor = true;
            this.button_JSON_Validate_Resp.Click += new System.EventHandler(this.Button_JSON_Validate_Resp_Click);
            // 
            // button_JSON_Validate_Pub
            // 
            this.button_JSON_Validate_Pub.Location = new System.Drawing.Point(366, 158);
            this.button_JSON_Validate_Pub.Margin = new System.Windows.Forms.Padding(4);
            this.button_JSON_Validate_Pub.Name = "button_JSON_Validate_Pub";
            this.button_JSON_Validate_Pub.Size = new System.Drawing.Size(119, 42);
            this.button_JSON_Validate_Pub.TabIndex = 49;
            this.button_JSON_Validate_Pub.Text = "CBOR WEB";
            this.button_JSON_Validate_Pub.UseVisualStyleBackColor = true;
            this.button_JSON_Validate_Pub.Click += new System.EventHandler(this.Button_JSON_Validate_Pub_Click);
            // 
            // checkBox_ShowMsg
            // 
            this.checkBox_ShowMsg.AutoSize = true;
            this.checkBox_ShowMsg.Checked = true;
            this.checkBox_ShowMsg.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_ShowMsg.Location = new System.Drawing.Point(27, 154);
            this.checkBox_ShowMsg.Margin = new System.Windows.Forms.Padding(4);
            this.checkBox_ShowMsg.Name = "checkBox_ShowMsg";
            this.checkBox_ShowMsg.Size = new System.Drawing.Size(94, 21);
            this.checkBox_ShowMsg.TabIndex = 50;
            this.checkBox_ShowMsg.Text = "Show Msg";
            this.checkBox_ShowMsg.UseVisualStyleBackColor = true;
            this.checkBox_ShowMsg.CheckedChanged += new System.EventHandler(this.CheckBox_ShowMsg_CheckedChanged);
            // 
            // groupBox9
            // 
            this.groupBox9.Controls.Add(this.button_res2res_reset);
            this.groupBox9.Controls.Add(this.label48);
            this.groupBox9.Controls.Add(this.textBox_Res2Res);
            this.groupBox9.Controls.Add(this.textBox_timer_pub2res);
            this.groupBox9.Controls.Add(this.label46);
            this.groupBox9.Location = new System.Drawing.Point(685, 7);
            this.groupBox9.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox9.Name = "groupBox9";
            this.groupBox9.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox9.Size = new System.Drawing.Size(553, 102);
            this.groupBox9.TabIndex = 51;
            this.groupBox9.TabStop = false;
            this.groupBox9.Text = "Timers";
            // 
            // button_res2res_reset
            // 
            this.button_res2res_reset.Location = new System.Drawing.Point(168, 71);
            this.button_res2res_reset.Margin = new System.Windows.Forms.Padding(4);
            this.button_res2res_reset.Name = "button_res2res_reset";
            this.button_res2res_reset.Size = new System.Drawing.Size(64, 30);
            this.button_res2res_reset.TabIndex = 4;
            this.button_res2res_reset.Text = "Reset";
            this.button_res2res_reset.UseVisualStyleBackColor = true;
            this.button_res2res_reset.Click += new System.EventHandler(this.Button_res2res_reset_Click);
            // 
            // label48
            // 
            this.label48.AutoSize = true;
            this.label48.Location = new System.Drawing.Point(164, 21);
            this.label48.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label48.Name = "label48";
            this.label48.Size = new System.Drawing.Size(110, 17);
            this.label48.TabIndex = 3;
            this.label48.Text = "Res to Res (ms)";
            // 
            // textBox_Res2Res
            // 
            this.textBox_Res2Res.Location = new System.Drawing.Point(165, 42);
            this.textBox_Res2Res.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_Res2Res.Name = "textBox_Res2Res";
            this.textBox_Res2Res.Size = new System.Drawing.Size(139, 22);
            this.textBox_Res2Res.TabIndex = 2;
            // 
            // textBox_timer_pub2res
            // 
            this.textBox_timer_pub2res.Location = new System.Drawing.Point(16, 42);
            this.textBox_timer_pub2res.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_timer_pub2res.Name = "textBox_timer_pub2res";
            this.textBox_timer_pub2res.Size = new System.Drawing.Size(139, 22);
            this.textBox_timer_pub2res.TabIndex = 1;
            // 
            // label46
            // 
            this.label46.AutoSize = true;
            this.label46.Location = new System.Drawing.Point(12, 21);
            this.label46.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label46.Name = "label46";
            this.label46.Size = new System.Drawing.Size(146, 17);
            this.label46.TabIndex = 0;
            this.label46.Text = "From Pub to Res (ms)";
            // 
            // textBox_Message
            // 
            this.textBox_Message.Location = new System.Drawing.Point(1217, 318);
            this.textBox_Message.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_Message.Multiline = true;
            this.textBox_Message.Name = "textBox_Message";
            this.textBox_Message.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox_Message.Size = new System.Drawing.Size(515, 407);
            this.textBox_Message.TabIndex = 52;
            this.textBox_Message.Text = "Welcome !";
            // 
            // textBox_SubTopic
            // 
            this.textBox_SubTopic.Location = new System.Drawing.Point(312, 66);
            this.textBox_SubTopic.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_SubTopic.Name = "textBox_SubTopic";
            this.textBox_SubTopic.Size = new System.Drawing.Size(139, 22);
            this.textBox_SubTopic.TabIndex = 53;
            this.textBox_SubTopic.Text = "+";
            // 
            // label47
            // 
            this.label47.AutoSize = true;
            this.label47.Location = new System.Drawing.Point(237, 70);
            this.label47.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label47.Name = "label47";
            this.label47.Size = new System.Drawing.Size(43, 17);
            this.label47.TabIndex = 54;
            this.label47.Text = "Topic";
            // 
            // textBox_Resp_Hash
            // 
            this.textBox_Resp_Hash.AcceptsReturn = true;
            this.textBox_Resp_Hash.AcceptsTab = true;
            this.textBox_Resp_Hash.Location = new System.Drawing.Point(1216, 159);
            this.textBox_Resp_Hash.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBox_Resp_Hash.Multiline = true;
            this.textBox_Resp_Hash.Name = "textBox_Resp_Hash";
            this.textBox_Resp_Hash.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBox_Resp_Hash.Size = new System.Drawing.Size(516, 115);
            this.textBox_Resp_Hash.TabIndex = 55;
            this.textBox_Resp_Hash.Text = "\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n";
            // 
            // label50
            // 
            this.label50.AutoSize = true;
            this.label50.Location = new System.Drawing.Point(1214, 134);
            this.label50.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label50.Name = "label50";
            this.label50.Size = new System.Drawing.Size(94, 17);
            this.label50.TabIndex = 57;
            this.label50.Text = "Response \"#\"";
            this.label50.Click += new System.EventHandler(this.Label50_Click);
            // 
            // label49
            // 
            this.label49.AutoSize = true;
            this.label49.Location = new System.Drawing.Point(237, 46);
            this.label49.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label49.Name = "label49";
            this.label49.Size = new System.Drawing.Size(50, 17);
            this.label49.TabIndex = 58;
            this.label49.Text = "Target";
            // 
            // textBox_Target
            // 
            this.textBox_Target.Location = new System.Drawing.Point(312, 41);
            this.textBox_Target.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_Target.Name = "textBox_Target";
            this.textBox_Target.Size = new System.Drawing.Size(139, 22);
            this.textBox_Target.TabIndex = 59;
            this.textBox_Target.Text = "30AEA4EEC3A4";
            // 
            // label51
            // 
            this.label51.AutoSize = true;
            this.label51.Location = new System.Drawing.Point(1214, 288);
            this.label51.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label51.Name = "label51";
            this.label51.Size = new System.Drawing.Size(38, 17);
            this.label51.TabIndex = 60;
            this.label51.Text = "Infos";
            this.label51.Click += new System.EventHandler(this.Label51_Click);
            // 
            // checkBox_TLS
            // 
            this.checkBox_TLS.AutoSize = true;
            this.checkBox_TLS.Checked = true;
            this.checkBox_TLS.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_TLS.Location = new System.Drawing.Point(341, 12);
            this.checkBox_TLS.Margin = new System.Windows.Forms.Padding(4);
            this.checkBox_TLS.Name = "checkBox_TLS";
            this.checkBox_TLS.Size = new System.Drawing.Size(56, 21);
            this.checkBox_TLS.TabIndex = 61;
            this.checkBox_TLS.Text = "TLS";
            this.checkBox_TLS.UseVisualStyleBackColor = true;
            this.checkBox_TLS.CheckedChanged += new System.EventHandler(this.CheckBox_TLS_CheckedChanged);
            // 
            // button_Save_settings
            // 
            this.button_Save_settings.Font = new System.Drawing.Font("Microsoft Sans Serif", 7F);
            this.button_Save_settings.Location = new System.Drawing.Point(1661, 9);
            this.button_Save_settings.Margin = new System.Windows.Forms.Padding(4);
            this.button_Save_settings.Name = "button_Save_settings";
            this.button_Save_settings.Size = new System.Drawing.Size(72, 36);
            this.button_Save_settings.TabIndex = 62;
            this.button_Save_settings.Text = "Savecfg";
            this.button_Save_settings.UseVisualStyleBackColor = true;
            this.button_Save_settings.Click += new System.EventHandler(this.Button_Save_settings_Click);
            // 
            // button_Load_settings
            // 
            this.button_Load_settings.Font = new System.Drawing.Font("Microsoft Sans Serif", 7F);
            this.button_Load_settings.Location = new System.Drawing.Point(588, 53);
            this.button_Load_settings.Margin = new System.Windows.Forms.Padding(4);
            this.button_Load_settings.Name = "button_Load_settings";
            this.button_Load_settings.Size = new System.Drawing.Size(72, 36);
            this.button_Load_settings.TabIndex = 63;
            this.button_Load_settings.Text = "Loadcfg";
            this.button_Load_settings.UseVisualStyleBackColor = true;
            this.button_Load_settings.Click += new System.EventHandler(this.Button_Load_settings_Click);
            // 
            // checkBox_Cfg_Dbg_Rel
            // 
            this.checkBox_Cfg_Dbg_Rel.AutoSize = true;
            this.checkBox_Cfg_Dbg_Rel.Checked = true;
            this.checkBox_Cfg_Dbg_Rel.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_Cfg_Dbg_Rel.Location = new System.Drawing.Point(465, 68);
            this.checkBox_Cfg_Dbg_Rel.Margin = new System.Windows.Forms.Padding(4);
            this.checkBox_Cfg_Dbg_Rel.Name = "checkBox_Cfg_Dbg_Rel";
            this.checkBox_Cfg_Dbg_Rel.Size = new System.Drawing.Size(107, 21);
            this.checkBox_Cfg_Dbg_Rel.TabIndex = 64;
            this.checkBox_Cfg_Dbg_Rel.Text = "Cfg.Release";
            this.checkBox_Cfg_Dbg_Rel.UseVisualStyleBackColor = true;
            this.checkBox_Cfg_Dbg_Rel.CheckedChanged += new System.EventHandler(this.CheckBox_Cfg_Dbg_Rel_CheckedChanged);
            // 
            // checkBox_Split_Resp
            // 
            this.checkBox_Split_Resp.AutoSize = true;
            this.checkBox_Split_Resp.Checked = true;
            this.checkBox_Split_Resp.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_Split_Resp.Location = new System.Drawing.Point(104, 439);
            this.checkBox_Split_Resp.Margin = new System.Windows.Forms.Padding(4);
            this.checkBox_Split_Resp.Name = "checkBox_Split_Resp";
            this.checkBox_Split_Resp.Size = new System.Drawing.Size(94, 21);
            this.checkBox_Split_Resp.TabIndex = 65;
            this.checkBox_Split_Resp.Text = "Split Resp";
            this.checkBox_Split_Resp.UseVisualStyleBackColor = true;
            // 
            // button_CLS_Infos
            // 
            this.button_CLS_Infos.Location = new System.Drawing.Point(1675, 287);
            this.button_CLS_Infos.Margin = new System.Windows.Forms.Padding(4);
            this.button_CLS_Infos.Name = "button_CLS_Infos";
            this.button_CLS_Infos.Size = new System.Drawing.Size(57, 23);
            this.button_CLS_Infos.TabIndex = 66;
            this.button_CLS_Infos.Text = "CLS";
            this.button_CLS_Infos.UseVisualStyleBackColor = true;
            this.button_CLS_Infos.Click += new System.EventHandler(this.Button_CLS_Infos_Click);
            // 
            // label52
            // 
            this.label52.AutoSize = true;
            this.label52.Location = new System.Drawing.Point(8, 46);
            this.label52.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label52.Name = "label52";
            this.label52.Size = new System.Drawing.Size(21, 17);
            this.label52.TabIndex = 67;
            this.label52.Text = "ID";
            // 
            // textBox_MQTT_ID
            // 
            this.textBox_MQTT_ID.Location = new System.Drawing.Point(63, 42);
            this.textBox_MQTT_ID.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_MQTT_ID.Name = "textBox_MQTT_ID";
            this.textBox_MQTT_ID.Size = new System.Drawing.Size(168, 22);
            this.textBox_MQTT_ID.TabIndex = 68;
            this.textBox_MQTT_ID.Text = "C00000000000001";
            // 
            // textBox_MQTT_PWD
            // 
            this.textBox_MQTT_PWD.Location = new System.Drawing.Point(63, 66);
            this.textBox_MQTT_PWD.Margin = new System.Windows.Forms.Padding(4);
            this.textBox_MQTT_PWD.Name = "textBox_MQTT_PWD";
            this.textBox_MQTT_PWD.Size = new System.Drawing.Size(168, 22);
            this.textBox_MQTT_PWD.TabIndex = 69;
            this.textBox_MQTT_PWD.Text = "ClIjN8uaM3rwN5nE";
            // 
            // label53
            // 
            this.label53.AutoSize = true;
            this.label53.Location = new System.Drawing.Point(9, 71);
            this.label53.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label53.Name = "label53";
            this.label53.Size = new System.Drawing.Size(40, 17);
            this.label53.TabIndex = 70;
            this.label53.Text = "PWD";
            // 
            // radioButton_qos0
            // 
            this.radioButton_qos0.AutoSize = true;
            this.radioButton_qos0.Checked = true;
            this.radioButton_qos0.Location = new System.Drawing.Point(6, 15);
            this.radioButton_qos0.Name = "radioButton_qos0";
            this.radioButton_qos0.Size = new System.Drawing.Size(37, 21);
            this.radioButton_qos0.TabIndex = 71;
            this.radioButton_qos0.TabStop = true;
            this.radioButton_qos0.Text = "0";
            this.radioButton_qos0.UseVisualStyleBackColor = true;
            this.radioButton_qos0.CheckedChanged += new System.EventHandler(this.radioButton_qos0_CheckedChanged);
            this.radioButton_qos0.Click += new System.EventHandler(this.radioButton_qos0_Clik);
            // 
            // groupBox11
            // 
            this.groupBox11.Controls.Add(this.radioButton_qos1);
            this.groupBox11.Controls.Add(this.radioButton_qos0);
            this.groupBox11.Location = new System.Drawing.Point(312, 95);
            this.groupBox11.Name = "groupBox11";
            this.groupBox11.Size = new System.Drawing.Size(91, 39);
            this.groupBox11.TabIndex = 72;
            this.groupBox11.TabStop = false;
            this.groupBox11.Text = "QOS";
            // 
            // radioButton_qos1
            // 
            this.radioButton_qos1.AutoSize = true;
            this.radioButton_qos1.Location = new System.Drawing.Point(44, 15);
            this.radioButton_qos1.Name = "radioButton_qos1";
            this.radioButton_qos1.Size = new System.Drawing.Size(37, 21);
            this.radioButton_qos1.TabIndex = 72;
            this.radioButton_qos1.Text = "1";
            this.radioButton_qos1.UseVisualStyleBackColor = true;
            this.radioButton_qos1.CheckedChanged += new System.EventHandler(this.radioButton_qos1_CheckedChanged);
            this.radioButton_qos1.Click += new System.EventHandler(this.radioButton_qos1_Click);
            // 
            // timer_check_is_alive
            // 
            this.timer_check_is_alive.Interval = 1000;
            this.timer_check_is_alive.Tick += new System.EventHandler(this.timer_check_is_alive_Tick);
            // 
            // radioButton_alive
            // 
            this.radioButton_alive.AutoSize = true;
            this.radioButton_alive.Enabled = false;
            this.radioButton_alive.Location = new System.Drawing.Point(550, 2);
            this.radioButton_alive.Name = "radioButton_alive";
            this.radioButton_alive.Size = new System.Drawing.Size(100, 21);
            this.radioButton_alive.TabIndex = 73;
            this.radioButton_alive.TabStop = true;
            this.radioButton_alive.Text = "Alive Conn.";
            this.radioButton_alive.UseVisualStyleBackColor = true;
            // 
            // checkBox_mqtt_alive
            // 
            this.checkBox_mqtt_alive.AutoSize = true;
            this.checkBox_mqtt_alive.Enabled = false;
            this.checkBox_mqtt_alive.Location = new System.Drawing.Point(550, 29);
            this.checkBox_mqtt_alive.Name = "checkBox_mqtt_alive";
            this.checkBox_mqtt_alive.Size = new System.Drawing.Size(103, 21);
            this.checkBox_mqtt_alive.TabIndex = 74;
            this.checkBox_mqtt_alive.Text = "MQTT alive";
            this.checkBox_mqtt_alive.UseVisualStyleBackColor = true;
            // 
            // groupBox12
            // 
            this.groupBox12.Controls.Add(this.button_setDevsConfig);
            this.groupBox12.Controls.Add(this.buttonUpdateCAData);
            this.groupBox12.Controls.Add(this.buttonSet_UpdateGwFW);
            this.groupBox12.Controls.Add(this.buttonDevUpdateData);
            this.groupBox12.Controls.Add(this.buttonUpdate_ca_cerficates);
            this.groupBox12.Controls.Add(this.buttonUpdate_dev_firmware);
            this.groupBox12.Controls.Add(this.buttonUpdate_gw_firmware);
            this.groupBox12.Controls.Add(this.buttonSet_Devs_Config);
            this.groupBox12.Location = new System.Drawing.Point(915, 487);
            this.groupBox12.Name = "groupBox12";
            this.groupBox12.Size = new System.Drawing.Size(285, 238);
            this.groupBox12.TabIndex = 75;
            this.groupBox12.TabStop = false;
            this.groupBox12.Text = "Uploads";
            // 
            // button_setDevsConfig
            // 
            this.button_setDevsConfig.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button_setDevsConfig.Location = new System.Drawing.Point(141, 186);
            this.button_setDevsConfig.Name = "button_setDevsConfig";
            this.button_setDevsConfig.Size = new System.Drawing.Size(66, 21);
            this.button_setDevsConfig.TabIndex = 50;
            this.button_setDevsConfig.Text = "< SET";
            this.button_setDevsConfig.UseVisualStyleBackColor = true;
            this.button_setDevsConfig.Click += new System.EventHandler(this.button_setDevsConfig_Click);
            // 
            // buttonUpdateCAData
            // 
            this.buttonUpdateCAData.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonUpdateCAData.Location = new System.Drawing.Point(141, 25);
            this.buttonUpdateCAData.Name = "buttonUpdateCAData";
            this.buttonUpdateCAData.Size = new System.Drawing.Size(66, 21);
            this.buttonUpdateCAData.TabIndex = 49;
            this.buttonUpdateCAData.Text = "< SET";
            this.buttonUpdateCAData.UseVisualStyleBackColor = true;
            this.buttonUpdateCAData.Click += new System.EventHandler(this.buttonUpdateCAData_Click);
            // 
            // buttonSet_UpdateGwFW
            // 
            this.buttonSet_UpdateGwFW.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonSet_UpdateGwFW.Location = new System.Drawing.Point(141, 133);
            this.buttonSet_UpdateGwFW.Name = "buttonSet_UpdateGwFW";
            this.buttonSet_UpdateGwFW.Size = new System.Drawing.Size(66, 21);
            this.buttonSet_UpdateGwFW.TabIndex = 48;
            this.buttonSet_UpdateGwFW.Text = "< SET";
            this.buttonSet_UpdateGwFW.UseVisualStyleBackColor = true;
            this.buttonSet_UpdateGwFW.Click += new System.EventHandler(this.buttonSet_UpdateGwFW_Click);
            // 
            // buttonDevUpdateData
            // 
            this.buttonDevUpdateData.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonDevUpdateData.Location = new System.Drawing.Point(141, 77);
            this.buttonDevUpdateData.Name = "buttonDevUpdateData";
            this.buttonDevUpdateData.Size = new System.Drawing.Size(66, 21);
            this.buttonDevUpdateData.TabIndex = 47;
            this.buttonDevUpdateData.Text = "< SET";
            this.buttonDevUpdateData.UseVisualStyleBackColor = true;
            this.buttonDevUpdateData.Click += new System.EventHandler(this.button7_Click_1);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(1768, 745);
            this.Controls.Add(this.groupBox12);
            this.Controls.Add(this.checkBox_mqtt_alive);
            this.Controls.Add(this.radioButton_alive);
            this.Controls.Add(this.groupBox11);
            this.Controls.Add(this.label53);
            this.Controls.Add(this.textBox_MQTT_PWD);
            this.Controls.Add(this.textBox_MQTT_ID);
            this.Controls.Add(this.label52);
            this.Controls.Add(this.button_CLS_Infos);
            this.Controls.Add(this.checkBox_Split_Resp);
            this.Controls.Add(this.checkBox_Cfg_Dbg_Rel);
            this.Controls.Add(this.button_Load_settings);
            this.Controls.Add(this.button_Save_settings);
            this.Controls.Add(this.checkBox_TLS);
            this.Controls.Add(this.label51);
            this.Controls.Add(this.textBox_Target);
            this.Controls.Add(this.label49);
            this.Controls.Add(this.label50);
            this.Controls.Add(this.textBox_Resp_Hash);
            this.Controls.Add(this.label47);
            this.Controls.Add(this.textBox_SubTopic);
            this.Controls.Add(this.textBox_Message);
            this.Controls.Add(this.groupBox9);
            this.Controls.Add(this.checkBox_ShowMsg);
            this.Controls.Add(this.button_JSON_Validate_Pub);
            this.Controls.Add(this.button_JSON_Validate_Resp);
            this.Controls.Add(this.groupBox8);
            this.Controls.Add(this.groupBox7);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.buttonClearResponse);
            this.Controls.Add(this.buttonConnect);
            this.Controls.Add(this.textBoxMQTT_Server_Port);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.textBoxMQTT_Server_URL);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.buttonClear);
            this.Controls.Add(this.labelResponse);
            this.Controls.Add(this.labelPublish);
            this.Controls.Add(this.textBoxPublish);
            this.Controls.Add(this.txtConsole);
            this.Controls.Add(this.groupBox1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "Form1";
            this.Text = "IoT Server Simulator Binary Edition V.2.5";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox7.ResumeLayout(false);
            this.groupBox7.PerformLayout();
            this.groupBox10.ResumeLayout(false);
            this.groupBox10.PerformLayout();
            this.groupBox8.ResumeLayout(false);
            this.groupBox8.PerformLayout();
            this.groupBox9.ResumeLayout(false);
            this.groupBox9.PerformLayout();
            this.groupBox11.ResumeLayout(false);
            this.groupBox11.PerformLayout();
            this.groupBox12.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.TextBox txtConsole;
        private System.Windows.Forms.Button buttonChangeCredential;
        private System.Windows.Forms.Button buttonReadValue;
        private System.Windows.Forms.TextBox textBoxPublish;
        private System.Windows.Forms.Label labelPublish;
        private System.Windows.Forms.Label labelResponse;
        private System.Windows.Forms.Button buttonClear;
        private System.Windows.Forms.Button buttonScanLine;
        private System.Windows.Forms.Button buttonSet_Devs_Config;
        private System.Windows.Forms.Button buttonSet_Lines_Config;
        private System.Windows.Forms.Button buttonUpdate_ca_cerficates;
        private System.Windows.Forms.Button buttonUpdate_dev_firmware;
        private System.Windows.Forms.Button buttonUpdate_gw_firmware;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxMQTT_Server_URL;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBoxMQTT_Server_Port;
        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button buttonClearResponse;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox textBox_MB_HR_W_Func;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBox_HR_Val;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBox_MB_Addr;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox textBox_Alias;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBox_MB_Dim;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBox_A;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox textBox_MB_Len;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox textBox_MB_Pos;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox textBox_B;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TextBox textBox_Flags;
        private System.Windows.Forms.Button button_MB_Write_HR;
        private System.Windows.Forms.Button button_MB_Read_HR;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button button_MB_Write_COIL;
        private System.Windows.Forms.Button button_MB_Read_COIL;
        private System.Windows.Forms.TextBox textBox_MB_Addr_Coil;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.TextBox textBox_Alias_Coil;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.TextBox textBox_MB_COIL_R_Func;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.TextBox textBox_MB_COIL_Val;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.TextBox textBox_MB_HR_R_Func;
        private System.Windows.Forms.TextBox textBox_MB_COIL_W_Func;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.TextBox textBox6;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.TextBox textBox7;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.TextBox textBox8;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.TextBox textBox9;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.TextBox textBox10;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.TextBox textBox11;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.TextBox textBox12;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.TextBox textBox13;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.TextBox textBox14;
        private System.Windows.Forms.Label label27;
        private System.Windows.Forms.TextBox textBox15;
        private System.Windows.Forms.Label label28;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.GroupBox groupBox7;
        private System.Windows.Forms.GroupBox groupBox10;
        private System.Windows.Forms.TextBox textBox26;
        private System.Windows.Forms.Button button11;
        private System.Windows.Forms.Button button12;
        private System.Windows.Forms.TextBox textBox27;
        private System.Windows.Forms.Label label37;
        private System.Windows.Forms.TextBox textBox28;
        private System.Windows.Forms.Label label38;
        private System.Windows.Forms.TextBox textBox29;
        private System.Windows.Forms.Label label39;
        private System.Windows.Forms.TextBox textBox30;
        private System.Windows.Forms.Label label40;
        private System.Windows.Forms.Button button_MB_Read_DI;
        private System.Windows.Forms.TextBox textBox_MB_Addr_DI;
        private System.Windows.Forms.Label label41;
        private System.Windows.Forms.TextBox textBox_Alias_DI;
        private System.Windows.Forms.Label label42;
        private System.Windows.Forms.TextBox textBox_MB_DI_R_Func;
        private System.Windows.Forms.Label label43;
        private System.Windows.Forms.GroupBox groupBox8;
        private System.Windows.Forms.TextBox textBox_MB_IR_R_Func;
        private System.Windows.Forms.Button button_MB_Read_IR;
        private System.Windows.Forms.Label label29;
        private System.Windows.Forms.TextBox textBox_Flags_IR;
        private System.Windows.Forms.Label label30;
        private System.Windows.Forms.TextBox textBox_B_IR;
        private System.Windows.Forms.TextBox textBox_A_IR;
        private System.Windows.Forms.Label label31;
        private System.Windows.Forms.TextBox textBox_MB_Len_IR;
        private System.Windows.Forms.Label label32;
        private System.Windows.Forms.TextBox textBox_MB_Pos_IR;
        private System.Windows.Forms.Label label33;
        private System.Windows.Forms.TextBox textBox_MB_Addr_IR;
        private System.Windows.Forms.Label label34;
        private System.Windows.Forms.TextBox textBox_Alias_IR;
        private System.Windows.Forms.Label label35;
        private System.Windows.Forms.TextBox textBox_MB_Dim_IR;
        private System.Windows.Forms.Label label36;
        private System.Windows.Forms.Label label44;
        private System.Windows.Forms.Label label45;
        private System.Windows.Forms.Button button_JSON_Validate_Resp;
        private System.Windows.Forms.Button button_JSON_Validate_Pub;
        private System.Windows.Forms.CheckBox checkBox_ShowMsg;
        private System.Windows.Forms.Button button_send_mb_adu;
        private System.Windows.Forms.GroupBox groupBox9;
        private System.Windows.Forms.TextBox textBox_timer_pub2res;
        private System.Windows.Forms.Label label46;
        private System.Windows.Forms.TextBox textBox_Message;
        private System.Windows.Forms.TextBox textBox_SubTopic;
        private System.Windows.Forms.Label label47;
        private System.Windows.Forms.Label label48;
        private System.Windows.Forms.TextBox textBox_Res2Res;
        private System.Windows.Forms.Button button_res2res_reset;
        private System.Windows.Forms.TextBox textBox_Resp_Hash;
        private System.Windows.Forms.Label label50;
        private System.Windows.Forms.Label label49;
        private System.Windows.Forms.TextBox textBox_Target;
        private System.Windows.Forms.Button button_test_set_gw_config_req;
        private System.Windows.Forms.Button button_start_engine;
        private System.Windows.Forms.Button button_stop_engine;
        private System.Windows.Forms.Label label51;
        private System.Windows.Forms.CheckBox checkBox_TLS;
        private System.Windows.Forms.Button button_Save_settings;
        private System.Windows.Forms.Button button_Load_settings;
        private System.Windows.Forms.CheckBox checkBox_Cfg_Dbg_Rel;
        private System.Windows.Forms.CheckBox checkBox_Split_Resp;
        private System.Windows.Forms.Button button_CLS_Infos;
        private System.Windows.Forms.Button button_reboot;
        private System.Windows.Forms.Label label52;
        private System.Windows.Forms.TextBox textBox_MQTT_ID;
        private System.Windows.Forms.TextBox textBox_MQTT_PWD;
        private System.Windows.Forms.Label label53;
        private System.Windows.Forms.Button button_flush;
        private System.Windows.Forms.Button buttonSet_Lines_Config_TTL;
        private System.Windows.Forms.Button button_ReqDBGInfo;
        private System.Windows.Forms.Button button_SetFlags;
        private System.Windows.Forms.Button button_SetFlags_IR;
        private System.Windows.Forms.RadioButton radioButton_qos0;
        private System.Windows.Forms.GroupBox groupBox11;
        private System.Windows.Forms.RadioButton radioButton_qos1;
        private System.Windows.Forms.Timer timer_check_is_alive;
        private System.Windows.Forms.RadioButton radioButton_alive;
        private System.Windows.Forms.CheckBox checkBox_mqtt_alive;
        private System.Windows.Forms.GroupBox groupBox12;
        private System.Windows.Forms.Button buttonDevUpdateData;
        private System.Windows.Forms.Button buttonSet_UpdateGwFW;
        private System.Windows.Forms.Button buttonUpdateCAData;
        private System.Windows.Forms.Button button_setDevsConfig;
    }
}

