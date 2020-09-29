namespace MqttClientSimulatorBinary
{
    partial class FormCBORUpdateCA
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
            this.buttonDevUpdateDataCancel = new System.Windows.Forms.Button();
            this.buttonDevUpdateDataSave = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxDevUpdateData_PWD = new System.Windows.Forms.TextBox();
            this.textBoxDevUpdateData_USR = new System.Windows.Forms.TextBox();
            this.textBoxDevUpdateData_URL = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxDevUpdateData_crc = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // buttonDevUpdateDataCancel
            // 
            this.buttonDevUpdateDataCancel.Location = new System.Drawing.Point(615, 141);
            this.buttonDevUpdateDataCancel.Name = "buttonDevUpdateDataCancel";
            this.buttonDevUpdateDataCancel.Size = new System.Drawing.Size(86, 33);
            this.buttonDevUpdateDataCancel.TabIndex = 31;
            this.buttonDevUpdateDataCancel.Text = "Cancel";
            this.buttonDevUpdateDataCancel.UseVisualStyleBackColor = true;
            this.buttonDevUpdateDataCancel.Click += new System.EventHandler(this.buttonDevUpdateDataCancel_Click);
            // 
            // buttonDevUpdateDataSave
            // 
            this.buttonDevUpdateDataSave.Location = new System.Drawing.Point(523, 141);
            this.buttonDevUpdateDataSave.Name = "buttonDevUpdateDataSave";
            this.buttonDevUpdateDataSave.Size = new System.Drawing.Size(86, 33);
            this.buttonDevUpdateDataSave.TabIndex = 30;
            this.buttonDevUpdateDataSave.Text = "Save";
            this.buttonDevUpdateDataSave.UseVisualStyleBackColor = true;
            this.buttonDevUpdateDataSave.Click += new System.EventHandler(this.buttonDevUpdateDataSave_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(8, 97);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(34, 17);
            this.label3.TabIndex = 29;
            this.label3.Text = "Pwd";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(8, 60);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(38, 17);
            this.label2.TabIndex = 28;
            this.label2.Text = "User";
            // 
            // textBoxDevUpdateData_PWD
            // 
            this.textBoxDevUpdateData_PWD.Location = new System.Drawing.Point(58, 94);
            this.textBoxDevUpdateData_PWD.Name = "textBoxDevUpdateData_PWD";
            this.textBoxDevUpdateData_PWD.Size = new System.Drawing.Size(291, 22);
            this.textBoxDevUpdateData_PWD.TabIndex = 27;
            // 
            // textBoxDevUpdateData_USR
            // 
            this.textBoxDevUpdateData_USR.Location = new System.Drawing.Point(58, 57);
            this.textBoxDevUpdateData_USR.Name = "textBoxDevUpdateData_USR";
            this.textBoxDevUpdateData_USR.Size = new System.Drawing.Size(291, 22);
            this.textBoxDevUpdateData_USR.TabIndex = 26;
            // 
            // textBoxDevUpdateData_URL
            // 
            this.textBoxDevUpdateData_URL.Location = new System.Drawing.Point(58, 12);
            this.textBoxDevUpdateData_URL.Name = "textBoxDevUpdateData_URL";
            this.textBoxDevUpdateData_URL.Size = new System.Drawing.Size(643, 22);
            this.textBoxDevUpdateData_URL.TabIndex = 25;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(8, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(36, 17);
            this.label1.TabIndex = 24;
            this.label1.Text = "URL";
            // 
            // textBoxDevUpdateData_crc
            // 
            this.textBoxDevUpdateData_crc.Location = new System.Drawing.Point(58, 128);
            this.textBoxDevUpdateData_crc.Name = "textBoxDevUpdateData_crc";
            this.textBoxDevUpdateData_crc.Size = new System.Drawing.Size(291, 22);
            this.textBoxDevUpdateData_crc.TabIndex = 34;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(8, 128);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(27, 17);
            this.label4.TabIndex = 32;
            this.label4.Text = "crc";
            // 
            // FormCBORUpdateCA
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(726, 186);
            this.Controls.Add(this.textBoxDevUpdateData_crc);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.buttonDevUpdateDataCancel);
            this.Controls.Add(this.buttonDevUpdateDataSave);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textBoxDevUpdateData_PWD);
            this.Controls.Add(this.textBoxDevUpdateData_USR);
            this.Controls.Add(this.textBoxDevUpdateData_URL);
            this.Controls.Add(this.label1);
            this.Name = "FormCBORUpdateCA";
            this.Text = "Update CA Data";
            this.Load += new System.EventHandler(this.FormCBORUpdateCA_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonDevUpdateDataCancel;
        private System.Windows.Forms.Button buttonDevUpdateDataSave;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxDevUpdateData_PWD;
        private System.Windows.Forms.TextBox textBoxDevUpdateData_USR;
        private System.Windows.Forms.TextBox textBoxDevUpdateData_URL;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxDevUpdateData_crc;
        private System.Windows.Forms.Label label4;
    }
}