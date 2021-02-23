namespace MqttClientSimulatorBinary
{
    partial class Form_Abort_Upload
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form_Abort_Upload));
            this.label3 = new System.Windows.Forms.Label();
            this.textBoxUploadFile_rid = new System.Windows.Forms.TextBox();
            this.buttonUploadFileFromCtrlFull_Cancel = new System.Windows.Forms.Button();
            this.buttonUploadFileFromCtrlFull_Save = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 27);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(25, 17);
            this.label3.TabIndex = 28;
            this.label3.Text = "rto";
            // 
            // textBoxUploadFile_rid
            // 
            this.textBoxUploadFile_rid.Location = new System.Drawing.Point(58, 27);
            this.textBoxUploadFile_rid.Name = "textBoxUploadFile_rid";
            this.textBoxUploadFile_rid.Size = new System.Drawing.Size(291, 22);
            this.textBoxUploadFile_rid.TabIndex = 27;
            // 
            // buttonUploadFileFromCtrlFull_Cancel
            // 
            this.buttonUploadFileFromCtrlFull_Cancel.Location = new System.Drawing.Point(459, 16);
            this.buttonUploadFileFromCtrlFull_Cancel.Name = "buttonUploadFileFromCtrlFull_Cancel";
            this.buttonUploadFileFromCtrlFull_Cancel.Size = new System.Drawing.Size(86, 33);
            this.buttonUploadFileFromCtrlFull_Cancel.TabIndex = 26;
            this.buttonUploadFileFromCtrlFull_Cancel.Text = "Cancel";
            this.buttonUploadFileFromCtrlFull_Cancel.UseVisualStyleBackColor = true;
            this.buttonUploadFileFromCtrlFull_Cancel.Click += new System.EventHandler(this.buttonUploadFileFromCtrlFull_Cancel_Click);
            // 
            // buttonUploadFileFromCtrlFull_Save
            // 
            this.buttonUploadFileFromCtrlFull_Save.Location = new System.Drawing.Point(367, 16);
            this.buttonUploadFileFromCtrlFull_Save.Name = "buttonUploadFileFromCtrlFull_Save";
            this.buttonUploadFileFromCtrlFull_Save.Size = new System.Drawing.Size(86, 33);
            this.buttonUploadFileFromCtrlFull_Save.TabIndex = 25;
            this.buttonUploadFileFromCtrlFull_Save.Text = "Save";
            this.buttonUploadFileFromCtrlFull_Save.UseVisualStyleBackColor = true;
            this.buttonUploadFileFromCtrlFull_Save.Click += new System.EventHandler(this.buttonUploadFileFromCtrlFull_Save_Click);
            // 
            // Form_Abort_Upload
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(562, 67);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.textBoxUploadFile_rid);
            this.Controls.Add(this.buttonUploadFileFromCtrlFull_Cancel);
            this.Controls.Add(this.buttonUploadFileFromCtrlFull_Save);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form_Abort_Upload";
            this.Text = "Upload ABORT ";
            this.Load += new System.EventHandler(this.Form_Abort_Upload_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBoxUploadFile_rid;
        private System.Windows.Forms.Button buttonUploadFileFromCtrlFull_Cancel;
        private System.Windows.Forms.Button buttonUploadFileFromCtrlFull_Save;
    }
}