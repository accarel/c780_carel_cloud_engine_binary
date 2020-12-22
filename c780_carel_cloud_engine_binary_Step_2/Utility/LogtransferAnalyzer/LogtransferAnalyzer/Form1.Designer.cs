namespace LogtransferAnalyzer
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
            this.label1 = new System.Windows.Forms.Label();
            this.textBox_logfile_in = new System.Windows.Forms.TextBox();
            this.button_analyze = new System.Windows.Forms.Button();
            this.textBox_message = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textBox_log_file_out = new System.Windows.Forms.TextBox();
            this.textBox_log_file_bin = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.textBox_Disassembled = new System.Windows.Forms.TextBox();
            this.textBox_Show_Disassembled = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(0, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(45, 17);
            this.label1.TabIndex = 0;
            this.label1.Text = "File In";
            // 
            // textBox_logfile_in
            // 
            this.textBox_logfile_in.Location = new System.Drawing.Point(67, 13);
            this.textBox_logfile_in.Name = "textBox_logfile_in";
            this.textBox_logfile_in.Size = new System.Drawing.Size(1085, 22);
            this.textBox_logfile_in.TabIndex = 1;
            this.textBox_logfile_in.Text = "putty.log";
            // 
            // button_analyze
            // 
            this.button_analyze.Location = new System.Drawing.Point(1162, 12);
            this.button_analyze.Name = "button_analyze";
            this.button_analyze.Size = new System.Drawing.Size(92, 35);
            this.button_analyze.TabIndex = 2;
            this.button_analyze.Text = "Analyze";
            this.button_analyze.UseVisualStyleBackColor = true;
            this.button_analyze.Click += new System.EventHandler(this.button_analyze_Click);
            // 
            // textBox_message
            // 
            this.textBox_message.Location = new System.Drawing.Point(11, 667);
            this.textBox_message.Multiline = true;
            this.textBox_message.Name = "textBox_message";
            this.textBox_message.Size = new System.Drawing.Size(1243, 77);
            this.textBox_message.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(0, 55);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(57, 17);
            this.label2.TabIndex = 4;
            this.label2.Text = "File Out";
            // 
            // textBox_log_file_out
            // 
            this.textBox_log_file_out.Location = new System.Drawing.Point(67, 55);
            this.textBox_log_file_out.Name = "textBox_log_file_out";
            this.textBox_log_file_out.Size = new System.Drawing.Size(1085, 22);
            this.textBox_log_file_out.TabIndex = 5;
            this.textBox_log_file_out.Text = "Datasample_PROCESSED.txt";
            // 
            // textBox_log_file_bin
            // 
            this.textBox_log_file_bin.Location = new System.Drawing.Point(67, 99);
            this.textBox_log_file_bin.Name = "textBox_log_file_bin";
            this.textBox_log_file_bin.Size = new System.Drawing.Size(1085, 22);
            this.textBox_log_file_bin.TabIndex = 6;
            this.textBox_log_file_bin.Text = "Datasample_PROCESSED.bin";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(0, 99);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(54, 17);
            this.label3.TabIndex = 7;
            this.label3.Text = "File Bin";
            this.label3.Click += new System.EventHandler(this.label3_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(0, 153);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(96, 17);
            this.label4.TabIndex = 8;
            this.label4.Text = "Disassembled";
            // 
            // textBox_Disassembled
            // 
            this.textBox_Disassembled.AcceptsTab = true;
            this.textBox_Disassembled.Location = new System.Drawing.Point(67, 173);
            this.textBox_Disassembled.Name = "textBox_Disassembled";
            this.textBox_Disassembled.Size = new System.Drawing.Size(1085, 22);
            this.textBox_Disassembled.TabIndex = 9;
            this.textBox_Disassembled.Text = "Log_DISASSEMBLED.txt";
            // 
            // textBox_Show_Disassembled
            // 
            this.textBox_Show_Disassembled.Font = new System.Drawing.Font("Courier New", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox_Show_Disassembled.Location = new System.Drawing.Point(9, 217);
            this.textBox_Show_Disassembled.Multiline = true;
            this.textBox_Show_Disassembled.Name = "textBox_Show_Disassembled";
            this.textBox_Show_Disassembled.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBox_Show_Disassembled.Size = new System.Drawing.Size(1244, 440);
            this.textBox_Show_Disassembled.TabIndex = 10;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1262, 753);
            this.Controls.Add(this.textBox_Show_Disassembled);
            this.Controls.Add(this.textBox_Disassembled);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.textBox_log_file_bin);
            this.Controls.Add(this.textBox_log_file_out);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textBox_message);
            this.Controls.Add(this.button_analyze);
            this.Controls.Add(this.textBox_logfile_in);
            this.Controls.Add(this.label1);
            this.Name = "Form1";
            this.Text = "Log transfer Analyzer V.0.1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBox_logfile_in;
        private System.Windows.Forms.Button button_analyze;
        private System.Windows.Forms.TextBox textBox_message;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBox_log_file_out;
        private System.Windows.Forms.TextBox textBox_log_file_bin;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBox_Disassembled;
        private System.Windows.Forms.TextBox textBox_Show_Disassembled;
    }
}

