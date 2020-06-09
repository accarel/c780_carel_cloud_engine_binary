namespace CustumCfgGenerator
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.button1 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.textMqttBroker = new System.Windows.Forms.TextBox();
            this.textMqttPort = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textMqttPassword = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.textMqttUser = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textNtpPort = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.textNtpAddress = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.textVersion = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.textBetaVersion = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.button2 = new System.Windows.Forms.Button();
            this.textApnName = new System.Windows.Forms.TextBox();
            this.textApnUser = new System.Windows.Forms.TextBox();
            this.textApnPassword = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.groupBox1apn = new System.Windows.Forms.GroupBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.groupBox1apn.SuspendLayout();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.SystemColors.GrayText;
            this.button1.Location = new System.Drawing.Point(337, 449);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(226, 92);
            this.button1.TabIndex = 12;
            this.button1.Text = "Generator";
            this.button1.UseVisualStyleBackColor = false;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(86, 187);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(71, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "mqtt broker";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // textMqttBroker
            // 
            this.textMqttBroker.Location = new System.Drawing.Point(89, 207);
            this.textMqttBroker.Name = "textMqttBroker";
            this.textMqttBroker.Size = new System.Drawing.Size(186, 19);
            this.textMqttBroker.TabIndex = 3;
            this.textMqttBroker.TextChanged += new System.EventHandler(this.textMqttBroker_TextChanged);
            // 
            // textMqttPort
            // 
            this.textMqttPort.Location = new System.Drawing.Point(89, 252);
            this.textMqttPort.Name = "textMqttPort";
            this.textMqttPort.Size = new System.Drawing.Size(186, 19);
            this.textMqttPort.TabIndex = 4;
            this.textMqttPort.TextChanged += new System.EventHandler(this.textMqttPort_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(86, 232);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(57, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "mqtt port";
            // 
            // textMqttPassword
            // 
            this.textMqttPassword.Location = new System.Drawing.Point(89, 365);
            this.textMqttPassword.Name = "textMqttPassword";
            this.textMqttPassword.Size = new System.Drawing.Size(183, 19);
            this.textMqttPassword.TabIndex = 6;
            this.textMqttPassword.TextChanged += new System.EventHandler(this.textMqttPassword_TextChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(86, 345);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(88, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "mqtt password";
            // 
            // textMqttUser
            // 
            this.textMqttUser.Location = new System.Drawing.Point(89, 306);
            this.textMqttUser.Name = "textMqttUser";
            this.textMqttUser.Size = new System.Drawing.Size(183, 19);
            this.textMqttUser.TabIndex = 5;
            this.textMqttUser.TextChanged += new System.EventHandler(this.textMqttUser_TextChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(86, 286);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(59, 13);
            this.label4.TabIndex = 5;
            this.label4.Text = "mqtt user";
            // 
            // textNtpPort
            // 
            this.textNtpPort.Location = new System.Drawing.Point(352, 269);
            this.textNtpPort.Name = "textNtpPort";
            this.textNtpPort.Size = new System.Drawing.Size(184, 19);
            this.textNtpPort.TabIndex = 8;
            this.textNtpPort.TextChanged += new System.EventHandler(this.textNtpPort_TextChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(349, 249);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(51, 13);
            this.label5.TabIndex = 11;
            this.label5.Text = "ntp port";
            // 
            // textNtpAddress
            // 
            this.textNtpAddress.Location = new System.Drawing.Point(352, 207);
            this.textNtpAddress.Name = "textNtpAddress";
            this.textNtpAddress.Size = new System.Drawing.Size(184, 19);
            this.textNtpAddress.TabIndex = 7;
            this.textNtpAddress.TextChanged += new System.EventHandler(this.textNtpAddress_TextChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(349, 187);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(73, 13);
            this.label6.TabIndex = 9;
            this.label6.Text = "ntp address";
            // 
            // textVersion
            // 
            this.textVersion.Location = new System.Drawing.Point(89, 129);
            this.textVersion.Name = "textVersion";
            this.textVersion.Size = new System.Drawing.Size(88, 19);
            this.textVersion.TabIndex = 1;
            this.textVersion.TextChanged += new System.EventHandler(this.textVersion_TextChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(86, 94);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(141, 13);
            this.label7.TabIndex = 13;
            this.label7.Text = "versione config. default";
            // 
            // textBetaVersion
            // 
            this.textBetaVersion.Location = new System.Drawing.Point(207, 129);
            this.textBetaVersion.Name = "textBetaVersion";
            this.textBetaVersion.Size = new System.Drawing.Size(88, 19);
            this.textBetaVersion.TabIndex = 2;
            this.textBetaVersion.TextChanged += new System.EventHandler(this.textBetaVersion_TextChanged);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.Location = new System.Drawing.Point(183, 129);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(15, 13);
            this.label8.TabIndex = 16;
            this.label8.Text = "B";
            // 
            // button2
            // 
            this.button2.BackColor = System.Drawing.SystemColors.GrayText;
            this.button2.Location = new System.Drawing.Point(741, 108);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(99, 43);
            this.button2.TabIndex = 17;
            this.button2.Text = "Clear data";
            this.button2.UseVisualStyleBackColor = false;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // textApnName
            // 
            this.textApnName.Location = new System.Drawing.Point(649, 207);
            this.textApnName.Name = "textApnName";
            this.textApnName.Size = new System.Drawing.Size(184, 19);
            this.textApnName.TabIndex = 9;
            this.textApnName.TextChanged += new System.EventHandler(this.textApnName_TextChanged);
            // 
            // textApnUser
            // 
            this.textApnUser.Location = new System.Drawing.Point(13, 26);
            this.textApnUser.Name = "textApnUser";
            this.textApnUser.Size = new System.Drawing.Size(184, 19);
            this.textApnUser.TabIndex = 10;
            this.textApnUser.TextChanged += new System.EventHandler(this.textApnUser_TextChanged);
            // 
            // textApnPassword
            // 
            this.textApnPassword.Location = new System.Drawing.Point(13, 81);
            this.textApnPassword.Name = "textApnPassword";
            this.textApnPassword.Size = new System.Drawing.Size(184, 19);
            this.textApnPassword.TabIndex = 11;
            this.textApnPassword.TextChanged += new System.EventHandler(this.textApnPassword_TextChanged);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label9.Location = new System.Drawing.Point(646, 187);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(62, 13);
            this.label9.TabIndex = 21;
            this.label9.Text = "apn name";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.Location = new System.Drawing.Point(10, 10);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(56, 13);
            this.label10.TabIndex = 22;
            this.label10.Text = "apn user";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label11.Location = new System.Drawing.Point(10, 65);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(85, 13);
            this.label11.TabIndex = 23;
            this.label11.Text = "apn password";
            // 
            // groupBox1apn
            // 
            this.groupBox1apn.Controls.Add(this.label11);
            this.groupBox1apn.Controls.Add(this.label10);
            this.groupBox1apn.Controls.Add(this.textApnPassword);
            this.groupBox1apn.Controls.Add(this.textApnUser);
            this.groupBox1apn.Location = new System.Drawing.Point(649, 272);
            this.groupBox1apn.Name = "groupBox1apn";
            this.groupBox1apn.Size = new System.Drawing.Size(215, 121);
            this.groupBox1apn.TabIndex = 24;
            this.groupBox1apn.TabStop = false;
            this.groupBox1apn.Enabled = false;
            
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(649, 249);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(180, 17);
            this.checkBox1.TabIndex = 25;
            this.checkBox1.Text = "Enable apn User/Password";
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.Checked = false;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(907, 564);
            this.Controls.Add(this.checkBox1);
            this.Controls.Add(this.groupBox1apn);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.textApnName);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.textBetaVersion);
            this.Controls.Add(this.textVersion);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.textNtpPort);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.textNtpAddress);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.textMqttPassword);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.textMqttUser);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.textMqttPort);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textMqttBroker);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button1);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "GME Default Parameter Generator";
            this.groupBox1apn.ResumeLayout(false);
            this.groupBox1apn.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textMqttBroker;
        private System.Windows.Forms.TextBox textMqttPort;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textMqttPassword;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textMqttUser;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textNtpPort;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textNtpAddress;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textVersion;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox textBetaVersion;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.TextBox textApnName;
        private System.Windows.Forms.TextBox textApnUser;
        private System.Windows.Forms.TextBox textApnPassword;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.GroupBox groupBox1apn;
        private System.Windows.Forms.CheckBox checkBox1;
    }
}

