namespace MqttClientSimulatorBinary
{
    partial class FormSetGWConfig
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
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.buttonSetGWConfig_Cancel = new System.Windows.Forms.Button();
            this.buttonSetGWConfig_Save = new System.Windows.Forms.Button();
            this.textBox_PVA = new System.Windows.Forms.TextBox();
            this.textBox_PST = new System.Windows.Forms.TextBox();
            this.textBox_MKA = new System.Windows.Forms.TextBox();
            this.textBox_LSS = new System.Windows.Forms.TextBox();
            this.textBox_HSS = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(22, 25);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 17);
            this.label1.TabIndex = 0;
            this.label1.Text = "PVA";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(22, 59);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 17);
            this.label2.TabIndex = 1;
            this.label2.Text = "PST";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(21, 89);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(37, 17);
            this.label3.TabIndex = 2;
            this.label3.Text = "MKA";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(22, 120);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(34, 17);
            this.label4.TabIndex = 3;
            this.label4.Text = "LSS";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(21, 150);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(36, 17);
            this.label5.TabIndex = 4;
            this.label5.Text = "HSS";
            // 
            // buttonSetGWConfig_Cancel
            // 
            this.buttonSetGWConfig_Cancel.Location = new System.Drawing.Point(367, 146);
            this.buttonSetGWConfig_Cancel.Name = "buttonSetGWConfig_Cancel";
            this.buttonSetGWConfig_Cancel.Size = new System.Drawing.Size(86, 33);
            this.buttonSetGWConfig_Cancel.TabIndex = 21;
            this.buttonSetGWConfig_Cancel.Text = "Cancel";
            this.buttonSetGWConfig_Cancel.UseVisualStyleBackColor = true;
            this.buttonSetGWConfig_Cancel.Click += new System.EventHandler(this.buttonDevUpdateDataCancel_Click_1);
            // 
            // buttonSetGWConfig_Save
            // 
            this.buttonSetGWConfig_Save.Location = new System.Drawing.Point(275, 146);
            this.buttonSetGWConfig_Save.Name = "buttonSetGWConfig_Save";
            this.buttonSetGWConfig_Save.Size = new System.Drawing.Size(86, 33);
            this.buttonSetGWConfig_Save.TabIndex = 20;
            this.buttonSetGWConfig_Save.Text = "Save";
            this.buttonSetGWConfig_Save.UseVisualStyleBackColor = true;
            this.buttonSetGWConfig_Save.Click += new System.EventHandler(this.buttonDevUpdateDataSave_Click_1);
            // 
            // textBox_PVA
            // 
            this.textBox_PVA.Location = new System.Drawing.Point(63, 20);
            this.textBox_PVA.Name = "textBox_PVA";
            this.textBox_PVA.Size = new System.Drawing.Size(138, 22);
            this.textBox_PVA.TabIndex = 22;
            // 
            // textBox_PST
            // 
            this.textBox_PST.Location = new System.Drawing.Point(63, 50);
            this.textBox_PST.Name = "textBox_PST";
            this.textBox_PST.Size = new System.Drawing.Size(138, 22);
            this.textBox_PST.TabIndex = 23;
            // 
            // textBox_MKA
            // 
            this.textBox_MKA.Location = new System.Drawing.Point(64, 83);
            this.textBox_MKA.Name = "textBox_MKA";
            this.textBox_MKA.Size = new System.Drawing.Size(137, 22);
            this.textBox_MKA.TabIndex = 24;
            // 
            // textBox_LSS
            // 
            this.textBox_LSS.Location = new System.Drawing.Point(63, 118);
            this.textBox_LSS.Name = "textBox_LSS";
            this.textBox_LSS.Size = new System.Drawing.Size(138, 22);
            this.textBox_LSS.TabIndex = 25;
            // 
            // textBox_HSS
            // 
            this.textBox_HSS.Location = new System.Drawing.Point(63, 151);
            this.textBox_HSS.Name = "textBox_HSS";
            this.textBox_HSS.Size = new System.Drawing.Size(138, 22);
            this.textBox_HSS.TabIndex = 26;
            // 
            // FormSetGWConfig
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(465, 199);
            this.Controls.Add(this.textBox_HSS);
            this.Controls.Add(this.textBox_LSS);
            this.Controls.Add(this.textBox_MKA);
            this.Controls.Add(this.textBox_PST);
            this.Controls.Add(this.textBox_PVA);
            this.Controls.Add(this.buttonSetGWConfig_Cancel);
            this.Controls.Add(this.buttonSetGWConfig_Save);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Name = "FormSetGWConfig";
            this.Text = "SetGWConfig";
            this.Load += new System.EventHandler(this.FormSetGWConfig_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button buttonSetGWConfig_Cancel;
        private System.Windows.Forms.Button buttonSetGWConfig_Save;
        private System.Windows.Forms.TextBox textBox_PVA;
        private System.Windows.Forms.TextBox textBox_PST;
        private System.Windows.Forms.TextBox textBox_MKA;
        private System.Windows.Forms.TextBox textBox_LSS;
        private System.Windows.Forms.TextBox textBox_HSS;
    }
}