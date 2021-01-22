namespace MqttClientSimulatorBinary
{
    partial class FormUploadFileFromControllerRange
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
            this.textBoxUploadFile_fid = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.buttonUploadFileFromController_Cancel = new System.Windows.Forms.Button();
            this.buttonUploadFileFromController_Save = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxUploadFile_fst = new System.Windows.Forms.TextBox();
            this.textBoxUploadFile_fle = new System.Windows.Forms.TextBox();
            this.textBoxUploadFile_rto = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // textBoxUploadFile_fid
            // 
            this.textBoxUploadFile_fid.Location = new System.Drawing.Point(67, 48);
            this.textBoxUploadFile_fid.Name = "textBoxUploadFile_fid";
            this.textBoxUploadFile_fid.Size = new System.Drawing.Size(291, 22);
            this.textBoxUploadFile_fid.TabIndex = 12;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(17, 48);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(23, 17);
            this.label4.TabIndex = 11;
            this.label4.Text = "fid";
            // 
            // buttonUploadFileFromController_Cancel
            // 
            this.buttonUploadFileFromController_Cancel.Location = new System.Drawing.Point(491, 116);
            this.buttonUploadFileFromController_Cancel.Name = "buttonUploadFileFromController_Cancel";
            this.buttonUploadFileFromController_Cancel.Size = new System.Drawing.Size(86, 33);
            this.buttonUploadFileFromController_Cancel.TabIndex = 14;
            this.buttonUploadFileFromController_Cancel.Text = "Cancel";
            this.buttonUploadFileFromController_Cancel.UseVisualStyleBackColor = true;
            // 
            // buttonUploadFileFromController_Save
            // 
            this.buttonUploadFileFromController_Save.Location = new System.Drawing.Point(399, 116);
            this.buttonUploadFileFromController_Save.Name = "buttonUploadFileFromController_Save";
            this.buttonUploadFileFromController_Save.Size = new System.Drawing.Size(86, 33);
            this.buttonUploadFileFromController_Save.TabIndex = 13;
            this.buttonUploadFileFromController_Save.Text = "Save";
            this.buttonUploadFileFromController_Save.UseVisualStyleBackColor = true;
            this.buttonUploadFileFromController_Save.Click += new System.EventHandler(this.buttonUploadFileFromController_Save_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(17, 87);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(23, 17);
            this.label1.TabIndex = 15;
            this.label1.Text = "fst";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(17, 124);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(23, 17);
            this.label2.TabIndex = 16;
            this.label2.Text = "fle";
            // 
            // textBoxUploadFile_fst
            // 
            this.textBoxUploadFile_fst.Location = new System.Drawing.Point(67, 87);
            this.textBoxUploadFile_fst.Name = "textBoxUploadFile_fst";
            this.textBoxUploadFile_fst.Size = new System.Drawing.Size(291, 22);
            this.textBoxUploadFile_fst.TabIndex = 17;
            // 
            // textBoxUploadFile_fle
            // 
            this.textBoxUploadFile_fle.Location = new System.Drawing.Point(67, 124);
            this.textBoxUploadFile_fle.Name = "textBoxUploadFile_fle";
            this.textBoxUploadFile_fle.Size = new System.Drawing.Size(291, 22);
            this.textBoxUploadFile_fle.TabIndex = 18;
            // 
            // textBoxUploadFile_rto
            // 
            this.textBoxUploadFile_rto.Location = new System.Drawing.Point(67, 12);
            this.textBoxUploadFile_rto.Name = "textBoxUploadFile_rto";
            this.textBoxUploadFile_rto.Size = new System.Drawing.Size(291, 22);
            this.textBoxUploadFile_rto.TabIndex = 19;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(17, 17);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(25, 17);
            this.label3.TabIndex = 20;
            this.label3.Text = "rto";
            // 
            // FormUploadFileFromControllerRange
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(590, 162);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.textBoxUploadFile_rto);
            this.Controls.Add(this.textBoxUploadFile_fle);
            this.Controls.Add(this.textBoxUploadFile_fst);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.buttonUploadFileFromController_Cancel);
            this.Controls.Add(this.buttonUploadFileFromController_Save);
            this.Controls.Add(this.textBoxUploadFile_fid);
            this.Controls.Add(this.label4);
            this.Name = "FormUploadFileFromControllerRange";
            this.Text = "Upload File from controller with Range";
            this.Load += new System.EventHandler(this.FormUploadFileFromController_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBoxUploadFile_fid;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button buttonUploadFileFromController_Cancel;
        private System.Windows.Forms.Button buttonUploadFileFromController_Save;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxUploadFile_fst;
        private System.Windows.Forms.TextBox textBoxUploadFile_fle;
        private System.Windows.Forms.TextBox textBoxUploadFile_rto;
        private System.Windows.Forms.Label label3;
    }
}