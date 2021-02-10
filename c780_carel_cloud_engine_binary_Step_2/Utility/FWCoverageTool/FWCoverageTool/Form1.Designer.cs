namespace FWCoverageTool
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
            this.textBox_base_dir = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.button_analyze_src = new System.Windows.Forms.Button();
            this.textBox_Info_Errors = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.richTextBox_coverage = new System.Windows.Forms.RichTextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label_Row = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label_Col = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.textBox_Src_Dir = new System.Windows.Forms.TextBox();
            this.label_src_name = new System.Windows.Forms.Label();
            this.richTextBox_Dir_list = new System.Windows.Forms.RichTextBox();
            this.SuspendLayout();
            // 
            // textBox_base_dir
            // 
            this.textBox_base_dir.Location = new System.Drawing.Point(104, 14);
            this.textBox_base_dir.Name = "textBox_base_dir";
            this.textBox_base_dir.Size = new System.Drawing.Size(502, 22);
            this.textBox_base_dir.TabIndex = 0;
            this.textBox_base_dir.Text = "C:\\_CANCELLABILI\\main\\putty.log";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 17);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(95, 17);
            this.label1.TabIndex = 1;
            this.label1.Text = "Coverage File";
            // 
            // button_analyze_src
            // 
            this.button_analyze_src.Location = new System.Drawing.Point(622, 8);
            this.button_analyze_src.Name = "button_analyze_src";
            this.button_analyze_src.Size = new System.Drawing.Size(75, 28);
            this.button_analyze_src.TabIndex = 2;
            this.button_analyze_src.Text = "Analyze";
            this.button_analyze_src.UseVisualStyleBackColor = true;
            this.button_analyze_src.Click += new System.EventHandler(this.button_analyze_src_Click);
            // 
            // textBox_Info_Errors
            // 
            this.textBox_Info_Errors.Location = new System.Drawing.Point(6, 784);
            this.textBox_Info_Errors.Multiline = true;
            this.textBox_Info_Errors.Name = "textBox_Info_Errors";
            this.textBox_Info_Errors.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox_Info_Errors.Size = new System.Drawing.Size(1604, 40);
            this.textBox_Info_Errors.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 764);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(74, 17);
            this.label2.TabIndex = 4;
            this.label2.Text = "Info/Errors";
            // 
            // richTextBox_coverage
            // 
            this.richTextBox_coverage.Location = new System.Drawing.Point(264, 97);
            this.richTextBox_coverage.Name = "richTextBox_coverage";
            this.richTextBox_coverage.Size = new System.Drawing.Size(1346, 635);
            this.richTextBox_coverage.TabIndex = 5;
            this.richTextBox_coverage.Text = "";
            this.richTextBox_coverage.MouseClick += new System.Windows.Forms.MouseEventHandler(this.richTextBox_coverage_MouseClick);
            this.richTextBox_coverage.CursorChanged += new System.EventHandler(this.richTextBox_coverage_CursorChanged);
            this.richTextBox_coverage.TextChanged += new System.EventHandler(this.richTextBox_coverage_TextChanged);
            this.richTextBox_coverage.KeyDown += new System.Windows.Forms.KeyEventHandler(this.richTextBox_coverage_KeyDown);
            this.richTextBox_coverage.KeyUp += new System.Windows.Forms.KeyEventHandler(this.richTextBox_coverage_KeyUp);
            this.richTextBox_coverage.MouseDown += new System.Windows.Forms.MouseEventHandler(this.richTextBox_coverage_MouseDown);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(261, 745);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(35, 17);
            this.label3.TabIndex = 6;
            this.label3.Text = "Row";
            // 
            // label_Row
            // 
            this.label_Row.AutoSize = true;
            this.label_Row.Location = new System.Drawing.Point(302, 745);
            this.label_Row.Name = "label_Row";
            this.label_Row.Size = new System.Drawing.Size(23, 17);
            this.label_Row.TabIndex = 7;
            this.label_Row.Text = "---";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(344, 745);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(28, 17);
            this.label4.TabIndex = 8;
            this.label4.Text = "Col";
            // 
            // label_Col
            // 
            this.label_Col.AutoSize = true;
            this.label_Col.Location = new System.Drawing.Point(378, 745);
            this.label_Col.Name = "label_Col";
            this.label_Col.Size = new System.Drawing.Size(23, 17);
            this.label_Col.TabIndex = 9;
            this.label_Col.Text = "---";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(3, 44);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(51, 17);
            this.label5.TabIndex = 10;
            this.label5.Text = "Src Dir";
            // 
            // textBox_Src_Dir
            // 
            this.textBox_Src_Dir.Location = new System.Drawing.Point(104, 44);
            this.textBox_Src_Dir.Name = "textBox_Src_Dir";
            this.textBox_Src_Dir.Size = new System.Drawing.Size(502, 22);
            this.textBox_Src_Dir.TabIndex = 11;
            this.textBox_Src_Dir.Text = "C:\\_CANCELLABILI\\main";
            // 
            // label_src_name
            // 
            this.label_src_name.AutoSize = true;
            this.label_src_name.Location = new System.Drawing.Point(261, 77);
            this.label_src_name.Name = "label_src_name";
            this.label_src_name.Size = new System.Drawing.Size(70, 17);
            this.label_src_name.TabIndex = 12;
            this.label_src_name.Text = "Src Name";
            // 
            // richTextBox_Dir_list
            // 
            this.richTextBox_Dir_list.Location = new System.Drawing.Point(12, 97);
            this.richTextBox_Dir_list.Name = "richTextBox_Dir_list";
            this.richTextBox_Dir_list.Size = new System.Drawing.Size(225, 635);
            this.richTextBox_Dir_list.TabIndex = 14;
            this.richTextBox_Dir_list.Text = "";
            this.richTextBox_Dir_list.MouseClick += new System.Windows.Forms.MouseEventHandler(this.richTextBox_Dir_list_MouseClick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1622, 834);
            this.Controls.Add(this.richTextBox_Dir_list);
            this.Controls.Add(this.label_src_name);
            this.Controls.Add(this.textBox_Src_Dir);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label_Col);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label_Row);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.richTextBox_coverage);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textBox_Info_Errors);
            this.Controls.Add(this.button_analyze_src);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBox_base_dir);
            this.Name = "Form1";
            this.Text = "FWCoverageTool Analyzer V.1.0";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.CursorChanged += new System.EventHandler(this.Form1_CursorChanged);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox_base_dir;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button_analyze_src;
        private System.Windows.Forms.TextBox textBox_Info_Errors;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.RichTextBox richTextBox_coverage;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label_Row;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label_Col;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBox_Src_Dir;
        private System.Windows.Forms.Label label_src_name;
        private System.Windows.Forms.RichTextBox richTextBox_Dir_list;
    }
}

