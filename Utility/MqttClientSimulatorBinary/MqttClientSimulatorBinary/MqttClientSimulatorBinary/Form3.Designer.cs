namespace MqttClientSimulatorBinary
{
    partial class FormDevUpdateData
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
            this.textBoxDevUpdateData_URL = new System.Windows.Forms.TextBox();
            this.textBoxDevUpdateData_USR = new System.Windows.Forms.TextBox();
            this.textBoxDevUpdateData_PWD = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.buttonDevUpdateDataSave = new System.Windows.Forms.Button();
            this.buttonDevUpdateDataCancel = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.textBoxDevUpdateData_fid = new System.Windows.Forms.TextBox();
            this.textBoxDevUpdateData_wet = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(15, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(36, 17);
            this.label1.TabIndex = 0;
            this.label1.Text = "URL";
            // 
            // textBoxDevUpdateData_URL
            // 
            this.textBoxDevUpdateData_URL.Location = new System.Drawing.Point(65, 22);
            this.textBoxDevUpdateData_URL.Name = "textBoxDevUpdateData_URL";
            this.textBoxDevUpdateData_URL.Size = new System.Drawing.Size(643, 22);
            this.textBoxDevUpdateData_URL.TabIndex = 1;
            // 
            // textBoxDevUpdateData_USR
            // 
            this.textBoxDevUpdateData_USR.Location = new System.Drawing.Point(65, 67);
            this.textBoxDevUpdateData_USR.Name = "textBoxDevUpdateData_USR";
            this.textBoxDevUpdateData_USR.Size = new System.Drawing.Size(291, 22);
            this.textBoxDevUpdateData_USR.TabIndex = 2;
            // 
            // textBoxDevUpdateData_PWD
            // 
            this.textBoxDevUpdateData_PWD.Location = new System.Drawing.Point(65, 104);
            this.textBoxDevUpdateData_PWD.Name = "textBoxDevUpdateData_PWD";
            this.textBoxDevUpdateData_PWD.Size = new System.Drawing.Size(291, 22);
            this.textBoxDevUpdateData_PWD.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(15, 70);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(38, 17);
            this.label2.TabIndex = 4;
            this.label2.Text = "User";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(15, 107);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(34, 17);
            this.label3.TabIndex = 5;
            this.label3.Text = "Pwd";
            // 
            // buttonDevUpdateDataSave
            // 
            this.buttonDevUpdateDataSave.Location = new System.Drawing.Point(530, 151);
            this.buttonDevUpdateDataSave.Name = "buttonDevUpdateDataSave";
            this.buttonDevUpdateDataSave.Size = new System.Drawing.Size(86, 33);
            this.buttonDevUpdateDataSave.TabIndex = 6;
            this.buttonDevUpdateDataSave.Text = "Save";
            this.buttonDevUpdateDataSave.UseVisualStyleBackColor = true;
            this.buttonDevUpdateDataSave.Click += new System.EventHandler(this.buttonDevUpdateDataSave_Click);
            // 
            // buttonDevUpdateDataCancel
            // 
            this.buttonDevUpdateDataCancel.Location = new System.Drawing.Point(622, 151);
            this.buttonDevUpdateDataCancel.Name = "buttonDevUpdateDataCancel";
            this.buttonDevUpdateDataCancel.Size = new System.Drawing.Size(86, 33);
            this.buttonDevUpdateDataCancel.TabIndex = 7;
            this.buttonDevUpdateDataCancel.Text = "Cancel";
            this.buttonDevUpdateDataCancel.UseVisualStyleBackColor = true;
            this.buttonDevUpdateDataCancel.Click += new System.EventHandler(this.buttonDevUpdateDataCancel_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(15, 138);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(23, 17);
            this.label4.TabIndex = 8;
            this.label4.Text = "fid";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(15, 170);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(29, 17);
            this.label5.TabIndex = 9;
            this.label5.Text = "wet";
            // 
            // textBoxDevUpdateData_fid
            // 
            this.textBoxDevUpdateData_fid.Location = new System.Drawing.Point(65, 138);
            this.textBoxDevUpdateData_fid.Name = "textBoxDevUpdateData_fid";
            this.textBoxDevUpdateData_fid.Size = new System.Drawing.Size(291, 22);
            this.textBoxDevUpdateData_fid.TabIndex = 10;
            // 
            // textBoxDevUpdateData_wet
            // 
            this.textBoxDevUpdateData_wet.Location = new System.Drawing.Point(65, 170);
            this.textBoxDevUpdateData_wet.Name = "textBoxDevUpdateData_wet";
            this.textBoxDevUpdateData_wet.Size = new System.Drawing.Size(291, 22);
            this.textBoxDevUpdateData_wet.TabIndex = 11;
            // 
            // FormDevUpdateData
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(728, 200);
            this.Controls.Add(this.textBoxDevUpdateData_wet);
            this.Controls.Add(this.textBoxDevUpdateData_fid);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.buttonDevUpdateDataCancel);
            this.Controls.Add(this.buttonDevUpdateDataSave);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textBoxDevUpdateData_PWD);
            this.Controls.Add(this.textBoxDevUpdateData_USR);
            this.Controls.Add(this.textBoxDevUpdateData_URL);
            this.Controls.Add(this.label1);
            this.Name = "FormDevUpdateData";
            this.Text = "DEV Update Data";
            this.Load += new System.EventHandler(this.FormDevUpdateData_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxDevUpdateData_URL;
        private System.Windows.Forms.TextBox textBoxDevUpdateData_USR;
        private System.Windows.Forms.TextBox textBoxDevUpdateData_PWD;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button buttonDevUpdateDataSave;
        private System.Windows.Forms.Button buttonDevUpdateDataCancel;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBoxDevUpdateData_fid;
        private System.Windows.Forms.TextBox textBoxDevUpdateData_wet;
    }
}