namespace MqttClientSimulatorBinary
{
    partial class WebTest
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
            this.textBox_webtest_result = new System.Windows.Forms.TextBox();
            this.button_get_fsize = new System.Windows.Forms.Button();
            this.textBox_web_test_url = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.textBox_dly_value = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // textBox_webtest_result
            // 
            this.textBox_webtest_result.Location = new System.Drawing.Point(118, 79);
            this.textBox_webtest_result.Multiline = true;
            this.textBox_webtest_result.Name = "textBox_webtest_result";
            this.textBox_webtest_result.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox_webtest_result.Size = new System.Drawing.Size(670, 121);
            this.textBox_webtest_result.TabIndex = 0;
            // 
            // button_get_fsize
            // 
            this.button_get_fsize.Location = new System.Drawing.Point(10, 79);
            this.button_get_fsize.Name = "button_get_fsize";
            this.button_get_fsize.Size = new System.Drawing.Size(96, 37);
            this.button_get_fsize.TabIndex = 1;
            this.button_get_fsize.Text = "get_fsize";
            this.button_get_fsize.UseVisualStyleBackColor = true;
            this.button_get_fsize.Click += new System.EventHandler(this.button_get_fsize_Click);
            // 
            // textBox_web_test_url
            // 
            this.textBox_web_test_url.Location = new System.Drawing.Point(118, 33);
            this.textBox_web_test_url.Name = "textBox_web_test_url";
            this.textBox_web_test_url.Size = new System.Drawing.Size(670, 22);
            this.textBox_web_test_url.TabIndex = 2;
            this.textBox_web_test_url.Text = "https://test-gwfs.remotepro.io/gwfs/v1/firmwares/48";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 32);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(36, 17);
            this.label1.TabIndex = 3;
            this.label1.Text = "URL";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(10, 210);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(96, 37);
            this.button1.TabIndex = 4;
            this.button1.Text = "Download";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // textBox_dly_value
            // 
            this.textBox_dly_value.Location = new System.Drawing.Point(174, 208);
            this.textBox_dly_value.Name = "textBox_dly_value";
            this.textBox_dly_value.Size = new System.Drawing.Size(64, 22);
            this.textBox_dly_value.TabIndex = 5;
            this.textBox_dly_value.Text = "200";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(121, 209);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(44, 17);
            this.label2.TabIndex = 6;
            this.label2.Text = "Delay";
            // 
            // WebTest
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 259);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textBox_dly_value);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBox_web_test_url);
            this.Controls.Add(this.button_get_fsize);
            this.Controls.Add(this.textBox_webtest_result);
            this.Name = "WebTest";
            this.Text = "WebTest";
            this.Load += new System.EventHandler(this.WebTest_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox_webtest_result;
        private System.Windows.Forms.Button button_get_fsize;
        private System.Windows.Forms.TextBox textBox_web_test_url;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox textBox_dly_value;
        private System.Windows.Forms.Label label2;
    }
}