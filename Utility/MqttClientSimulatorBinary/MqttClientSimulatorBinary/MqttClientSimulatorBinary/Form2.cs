using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MqttClientSimulatorBinary
{
    public partial class FormFlags : Form
    {

        public string ReturnText { get; set; }

        public FormFlags()
        {
            InitializeComponent();
        }

        private void FormFlags_Load(object sender, EventArgs e)
        {
            this.CenterToScreen();
        }

        private void checkBox5_CheckedChanged(object sender, EventArgs e)
        {
            this.Close();
        }

        private void button_Flag_Confirm_Click(object sender, EventArgs e)
        {
            int flag_value;
            
            flag_value = 0;
            if (checkBox_Flag_0.Checked) flag_value = 0x01;
            if (checkBox_Flag_1.Checked) flag_value |= 0x02;
            if (checkBox_Flag_2.Checked) flag_value |= 0x04;
            if (checkBox_Flag_3.Checked) flag_value |= 0x08;
            if (checkBox_Flag_4.Checked) flag_value |= 0x10;
            if (checkBox_Flag_5.Checked) flag_value |= 0x20;
            if (checkBox_Flag_6.Checked) flag_value |= 0x40;
            if (checkBox_Flag_7.Checked) flag_value |= 0x80;

            this.ReturnText = flag_value.ToString();
            this.Visible = false;

        }

        private void button_Flag_Cancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
