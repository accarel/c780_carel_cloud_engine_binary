namespace MqttClientSimulatorBinary
{
    partial class FormUploadFileFromControllerFull
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
            this.buttonUploadFileFromCtrlFull_Cancel = new System.Windows.Forms.Button();
            this.buttonUploadFileFromCtrlFull_Save = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.textBoxUploadFile_rto = new System.Windows.Forms.TextBox();
            this.textBoxUploadFile_fid = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // buttonUploadFileFromCtrlFull_Cancel
            // 
            this.buttonUploadFileFromCtrlFull_Cancel.Location = new System.Drawing.Point(457, 40);
            this.buttonUploadFileFromCtrlFull_Cancel.Name = "buttonUploadFileFromCtrlFull_Cancel";
            this.buttonUploadFileFromCtrlFull_Cancel.Size = new System.Drawing.Size(86, 33);
            this.buttonUploadFileFromCtrlFull_Cancel.TabIndex = 16;
            this.buttonUploadFileFromCtrlFull_Cancel.Text = "Cancel";
            this.buttonUploadFileFromCtrlFull_Cancel.UseVisualStyleBackColor = true;
            this.buttonUploadFileFromCtrlFull_Cancel.Click += new System.EventHandler(this.buttonUploadFileFromCtrlFull_Cancel_Click);
            // 
            // buttonUploadFileFromCtrlFull_Save
            // 
            this.buttonUploadFileFromCtrlFull_Save.Location = new System.Drawing.Point(365, 40);
            this.buttonUploadFileFromCtrlFull_Save.Name = "buttonUploadFileFromCtrlFull_Save";
            this.buttonUploadFileFromCtrlFull_Save.Size = new System.Drawing.Size(86, 33);
            this.buttonUploadFileFromCtrlFull_Save.TabIndex = 15;
            this.buttonUploadFileFromCtrlFull_Save.Text = "Save";
            this.buttonUploadFileFromCtrlFull_Save.UseVisualStyleBackColor = true;
            this.buttonUploadFileFromCtrlFull_Save.Click += new System.EventHandler(this.buttonUploadFileFromCtrlFull_Save_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(8, 17);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(25, 17);
            this.label3.TabIndex = 24;
            this.label3.Text = "rto";
            // 
            // textBoxUploadFile_rto
            // 
            this.textBoxUploadFile_rto.Location = new System.Drawing.Point(58, 12);
            this.textBoxUploadFile_rto.Name = "textBoxUploadFile_rto";
            this.textBoxUploadFile_rto.Size = new System.Drawing.Size(291, 22);
            this.textBoxUploadFile_rto.TabIndex = 23;
            // 
            // textBoxUploadFile_fid
            // 
            this.textBoxUploadFile_fid.Location = new System.Drawing.Point(58, 48);
            this.textBoxUploadFile_fid.Name = "textBoxUploadFile_fid";
            this.textBoxUploadFile_fid.Size = new System.Drawing.Size(291, 22);
            this.textBoxUploadFile_fid.TabIndex = 22;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(8, 48);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(23, 17);
            this.label4.TabIndex = 21;
            this.label4.Text = "fid";
            // 
            // FormUploadFileFromControllerFull
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(555, 90);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.textBoxUploadFile_rto);
            this.Controls.Add(this.textBoxUploadFile_fid);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.buttonUploadFileFromCtrlFull_Cancel);
            this.Controls.Add(this.buttonUploadFileFromCtrlFull_Save);
            this.Name = "FormUploadFileFromControllerFull";
            this.Text = "Upload File from controller Full";
            this.Load += new System.EventHandler(this.FormUploadFileFromControllerFull_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonUploadFileFromCtrlFull_Cancel;
        private System.Windows.Forms.Button buttonUploadFileFromCtrlFull_Save;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBoxUploadFile_rto;
        private System.Windows.Forms.TextBox textBoxUploadFile_fid;
        private System.Windows.Forms.Label label4;
    }
}