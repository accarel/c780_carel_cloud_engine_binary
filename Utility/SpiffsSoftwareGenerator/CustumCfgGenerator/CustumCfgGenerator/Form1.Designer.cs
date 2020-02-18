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
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.SystemColors.GrayText;
            this.button1.Location = new System.Drawing.Point(337, 384);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(226, 92);
            this.button1.TabIndex = 9;
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
            this.label1.Size = new System.Drawing.Size(91, 17);
            this.label1.TabIndex = 1;
            this.label1.Text = "mqtt broker";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // textMqttBroker
            // 
            this.textMqttBroker.Location = new System.Drawing.Point(89, 222);
            this.textMqttBroker.Name = "textMqttBroker";
            this.textMqttBroker.Size = new System.Drawing.Size(186, 22);
            this.textMqttBroker.TabIndex = 3;
            this.textMqttBroker.TextChanged += new System.EventHandler(this.textMqttBroker_TextChanged);
            // 
            // textMqttPort
            // 
            this.textMqttPort.Location = new System.Drawing.Point(89, 309);
            this.textMqttPort.Name = "textMqttPort";
            this.textMqttPort.Size = new System.Drawing.Size(186, 22);
            this.textMqttPort.TabIndex = 4;
            this.textMqttPort.TextChanged += new System.EventHandler(this.textMqttPort_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(86, 274);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(73, 17);
            this.label2.TabIndex = 3;
            this.label2.Text = "mqtt port";
            // 
            // textMqttPassword
            // 
            this.textMqttPassword.Location = new System.Drawing.Point(380, 309);
            this.textMqttPassword.Name = "textMqttPassword";
            this.textMqttPassword.Size = new System.Drawing.Size(183, 22);
            this.textMqttPassword.TabIndex = 6;
            this.textMqttPassword.TextChanged += new System.EventHandler(this.textMqttPassword_TextChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(377, 274);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(112, 17);
            this.label3.TabIndex = 7;
            this.label3.Text = "mqtt password";
            // 
            // textMqttUser
            // 
            this.textMqttUser.Location = new System.Drawing.Point(380, 222);
            this.textMqttUser.Name = "textMqttUser";
            this.textMqttUser.Size = new System.Drawing.Size(183, 22);
            this.textMqttUser.TabIndex = 5;
            this.textMqttUser.TextChanged += new System.EventHandler(this.textMqttUser_TextChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(377, 187);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(76, 17);
            this.label4.TabIndex = 5;
            this.label4.Text = "mqtt user";
            // 
            // textNtpPort
            // 
            this.textNtpPort.Location = new System.Drawing.Point(656, 309);
            this.textNtpPort.Name = "textNtpPort";
            this.textNtpPort.Size = new System.Drawing.Size(184, 22);
            this.textNtpPort.TabIndex = 8;
            this.textNtpPort.TextChanged += new System.EventHandler(this.textNtpPort_TextChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(652, 274);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(65, 17);
            this.label5.TabIndex = 11;
            this.label5.Text = "ntp port";
            // 
            // textNtpAddress
            // 
            this.textNtpAddress.Location = new System.Drawing.Point(656, 222);
            this.textNtpAddress.Name = "textNtpAddress";
            this.textNtpAddress.Size = new System.Drawing.Size(184, 22);
            this.textNtpAddress.TabIndex = 7;
            this.textNtpAddress.TextChanged += new System.EventHandler(this.textNtpAddress_TextChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(652, 187);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(94, 17);
            this.label6.TabIndex = 9;
            this.label6.Text = "ntp address";
            // 
            // textVersion
            // 
            this.textVersion.Location = new System.Drawing.Point(89, 129);
            this.textVersion.Name = "textVersion";
            this.textVersion.Size = new System.Drawing.Size(88, 22);
            this.textVersion.TabIndex = 1;
            this.textVersion.TextChanged += new System.EventHandler(this.textVersion_TextChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(86, 94);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(179, 17);
            this.label7.TabIndex = 13;
            this.label7.Text = "versione config. default";
            // 
            // textBetaVersion
            // 
            this.textBetaVersion.Location = new System.Drawing.Point(207, 129);
            this.textBetaVersion.Name = "textBetaVersion";
            this.textBetaVersion.Size = new System.Drawing.Size(88, 22);
            this.textBetaVersion.TabIndex = 2;
            this.textBetaVersion.TextChanged += new System.EventHandler(this.textBetaVersion_TextChanged);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.Location = new System.Drawing.Point(183, 129);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(18, 17);
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
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(907, 564);
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
    }
}

