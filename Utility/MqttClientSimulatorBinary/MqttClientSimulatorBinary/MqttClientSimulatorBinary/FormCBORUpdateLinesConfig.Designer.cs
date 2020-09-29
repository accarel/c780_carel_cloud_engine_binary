namespace MqttClientSimulatorBinary
{
    partial class FormCBORUpdateLinesConfig
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
            this.textBox_bau = new System.Windows.Forms.TextBox();
            this.textBox_con = new System.Windows.Forms.TextBox();
            this.textBox_del = new System.Windows.Forms.TextBox();
            this.buttonUpdLinesConigCancel = new System.Windows.Forms.Button();
            this.buttonUpdLinesConigSave = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(23, 14);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(32, 17);
            this.label1.TabIndex = 0;
            this.label1.Text = "bau";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(23, 55);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(31, 17);
            this.label2.TabIndex = 1;
            this.label2.Text = "con";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(23, 91);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(27, 17);
            this.label3.TabIndex = 2;
            this.label3.Text = "del";
            // 
            // textBox_bau
            // 
            this.textBox_bau.Location = new System.Drawing.Point(71, 13);
            this.textBox_bau.Name = "textBox_bau";
            this.textBox_bau.Size = new System.Drawing.Size(104, 22);
            this.textBox_bau.TabIndex = 3;
            // 
            // textBox_con
            // 
            this.textBox_con.Location = new System.Drawing.Point(71, 55);
            this.textBox_con.Name = "textBox_con";
            this.textBox_con.Size = new System.Drawing.Size(104, 22);
            this.textBox_con.TabIndex = 4;
            // 
            // textBox_del
            // 
            this.textBox_del.Location = new System.Drawing.Point(71, 91);
            this.textBox_del.Name = "textBox_del";
            this.textBox_del.Size = new System.Drawing.Size(104, 22);
            this.textBox_del.TabIndex = 5;
            // 
            // buttonUpdLinesConigCancel
            // 
            this.buttonUpdLinesConigCancel.Location = new System.Drawing.Point(345, 77);
            this.buttonUpdLinesConigCancel.Name = "buttonUpdLinesConigCancel";
            this.buttonUpdLinesConigCancel.Size = new System.Drawing.Size(86, 33);
            this.buttonUpdLinesConigCancel.TabIndex = 44;
            this.buttonUpdLinesConigCancel.Text = "Cancel";
            this.buttonUpdLinesConigCancel.UseVisualStyleBackColor = true;
            this.buttonUpdLinesConigCancel.Click += new System.EventHandler(this.buttonUpdLinesConigCancel_Click);
            // 
            // buttonUpdLinesConigSave
            // 
            this.buttonUpdLinesConigSave.Location = new System.Drawing.Point(253, 77);
            this.buttonUpdLinesConigSave.Name = "buttonUpdLinesConigSave";
            this.buttonUpdLinesConigSave.Size = new System.Drawing.Size(86, 33);
            this.buttonUpdLinesConigSave.TabIndex = 43;
            this.buttonUpdLinesConigSave.Text = "Save";
            this.buttonUpdLinesConigSave.UseVisualStyleBackColor = true;
            this.buttonUpdLinesConigSave.Click += new System.EventHandler(this.buttonUpdLinesConigSave_Click);
            // 
            // FormCBORUpdateLinesConfig
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(453, 126);
            this.Controls.Add(this.buttonUpdLinesConigCancel);
            this.Controls.Add(this.buttonUpdLinesConigSave);
            this.Controls.Add(this.textBox_del);
            this.Controls.Add(this.textBox_con);
            this.Controls.Add(this.textBox_bau);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Name = "FormCBORUpdateLinesConfig";
            this.Text = "CBOR Update Lines Config";
            this.Load += new System.EventHandler(this.FormCBORUpdateLinesConfig_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBox_bau;
        private System.Windows.Forms.TextBox textBox_con;
        private System.Windows.Forms.TextBox textBox_del;
        private System.Windows.Forms.Button buttonUpdLinesConigCancel;
        private System.Windows.Forms.Button buttonUpdLinesConigSave;
    }
}