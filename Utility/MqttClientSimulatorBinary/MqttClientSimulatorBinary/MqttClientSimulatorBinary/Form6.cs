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
    public partial class FormDecodeCMD17 : Form
    {

        private const int MODBUS_OFFSET = 10;

        private string data_to_decode = @"";
        public FormDecodeCMD17()
        {
            InitializeComponent();
        }


        public void set_data(string data) 
        {
            data_to_decode = data;
        }



        private static int GetHexVal(char hex)
        {
            int val = (int)hex;
            //For uppercase A-F letters:
            //return val - (val < 58 ? 48 : 55);
            //For lowercase a-f letters:
            //return val - (val < 58 ? 48 : 87);
            //Or the two combined, but a bit slower:
            return val - (val < 58 ? 48 : (val < 97 ? 55 : 87));
        }

        private void Decoding_1() 
        {

            string sub_low = @"";
            string sub_high = @"";
            short tmp_l = 0;
            short tmp_h = 0;
            UInt16 value = 0 ;
            bool pass_fail = false;
            byte val=0;

            /* --------------------------------------- */
            sub_high = data_to_decode.Substring(MODBUS_OFFSET + 0, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val) << 8);

            sub_low = data_to_decode.Substring(MODBUS_OFFSET+2, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_low[i << 1]) << 4) + (GetHexVal(sub_low[(i << 1) + 1])));
            }
            value += val;

            String s0 = String.Format("{0:X}", value);
            textBox_cmd17_Machine_Type.Text = s0.ToString();

            /* --------------------------------------- */
            value = 0;
            sub_high = data_to_decode.Substring(MODBUS_OFFSET + 4, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val) << 8);

            sub_low = data_to_decode.Substring(MODBUS_OFFSET + 6, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_low[i << 1]) << 4) + (GetHexVal(sub_low[(i << 1) + 1])));
            }
            value += val;

            String s1 = String.Format("{0:X}", value);
            textBox_CMD17_FW_Rel.Text = s1;

            /* --------------------------------------- */
            value = 0;
            sub_high = data_to_decode.Substring(MODBUS_OFFSET + 8, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val) << 8);

            sub_low = data_to_decode.Substring(MODBUS_OFFSET + 10, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_low[i << 1]) << 4) + (GetHexVal(sub_low[(i << 1) + 1])));
            }
            value += val;

            String s2 = String.Format("{0:X}", value);
            textBox_CMD17_HW_Code.Text = s2;

            /* --------------------------------------- */
            value = 0;
            sub_high = data_to_decode.Substring(MODBUS_OFFSET + 12, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val) << 8);

            sub_low = data_to_decode.Substring(MODBUS_OFFSET + 14, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_low[i << 1]) << 4) + (GetHexVal(sub_low[(i << 1) + 1])));
            }
            value += val;
            String s3 = String.Format("{0:X}", value);
            textBox_cmd17_pcap.Text = s3;

            if ((value & 0x01) != 0) ckBox_cmd17_pcap_0.Checked = true;
            if ((value & 0x02) != 0) ckBox_cmd17_pcap_1.Checked = true;
            if ((value & 0x04) != 0) ckBox_cmd17_pcap_2.Checked = true;
            if ((value & 0x08) != 0) ckBox_cmd17_pcap_3.Checked = true;
            if ((value & 0x10) != 0) ckBox_cmd17_pcap_4.Checked = true;
            if ((value & 0x20) != 0) ckBox_cmd17_pcap_5.Checked = true;
            if ((value & 0x40) != 0) ckBox_cmd17_pcap_6.Checked = true;
            if ((value & 0x80) != 0) ckBox_cmd17_pcap_7.Checked = true;
            if ((value & 0x100) != 0) ckBox_cmd17_pcap_8.Checked = true;
            if ((value & 0x200) != 0) ckBox_cmd17_pcap_9.Checked = true;
            if ((value & 0x400) != 0) ckBox_cmd17_pcap_10.Checked = true;
            if ((value & 0x800) != 0) ckBox_cmd17_pcap_11.Checked = true;
            /* --------------------------------------- */
            value = 0;
            sub_high = data_to_decode.Substring(MODBUS_OFFSET + 14, 2);         
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val));
            String s4 = String.Format("{0:X}", value);
            textBox_cmd17_reg_max_var.Text = s4;

            /* --------------------------------------- */
            value = 0;
            sub_high = data_to_decode.Substring(MODBUS_OFFSET + 16, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val) << 8);

            sub_low = data_to_decode.Substring(MODBUS_OFFSET + 18, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_low[i << 1]) << 4) + (GetHexVal(sub_low[(i << 1) + 1])));
            }
            value += val;

            String s5 = String.Format("{0}", value);
            textBox_cmd17_coil_max_var.Text = s5;

            /* --------------------------------------- */
            value = 0;
            sub_high = data_to_decode.Substring(MODBUS_OFFSET + 20, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val) << 8);

            sub_low = data_to_decode.Substring(MODBUS_OFFSET + 22, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_low[i << 1]) << 4) + (GetHexVal(sub_low[(i << 1) + 1])));
            }
            value += val;

            String s6 = String.Format("{0}", value);
            textBox_cmd17_option_code.Text = s6;
            /* --------------------------------------- */
            value = 0;
            sub_high = data_to_decode.Substring(MODBUS_OFFSET + 24, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val));
            String s7 = String.Format("{0}", value);
            textBox_cmd17_export_code.Text = s7;

            /* --------------------------------------- */
            value = 0;
            sub_high = data_to_decode.Substring(MODBUS_OFFSET + 26, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val));
            String s8 = String.Format("{0}", value);
            textBox_cmd17_app_release.Text = s8;
            /* --------------------------------------- */
            value = 0;
            sub_high = data_to_decode.Substring(MODBUS_OFFSET + 28, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val) << 8);

            sub_low = data_to_decode.Substring(MODBUS_OFFSET + 30, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_low[i << 1]) << 4) + (GetHexVal(sub_low[(i << 1) + 1])));
            }
            value += val;

            String s9 = String.Format("{0}", value);
            textBox_cmd17_app_code.Text = s9;
            /* --------------------------------------- */
            value = 0;
            sub_high = data_to_decode.Substring(MODBUS_OFFSET + 32, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val) << 8);

            sub_low = data_to_decode.Substring(MODBUS_OFFSET + 34, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_low[i << 1]) << 4) + (GetHexVal(sub_low[(i << 1) + 1])));
            }
            value += val;

            String s10 = String.Format("{0}", value);
            textBox_cmd17_customer_code.Text = s10;
            /* --------------------------------------- */
        }

        private void Decoding_2()
        {
            string sub_low = @"";
            string sub_high = @"";
            short tmp_l = 0;
            short tmp_h = 0;
            UInt16 value = 0;
            bool pass_fail = false;
            byte val = 0;

            /* --------------------------------------- */
            sub_high = data_to_decode.Substring(MODBUS_OFFSET + 36, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val));

            String s0 = String.Format("{0:X}", value);
            textBox_cmd17_back_comp.Text = s0.ToString();

            /* --------------------------------------- */
            value = 0;
            sub_high = data_to_decode.Substring(MODBUS_OFFSET + 38, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val) << 8);

            sub_low = data_to_decode.Substring(MODBUS_OFFSET + 40, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_low[i << 1]) << 4) + (GetHexVal(sub_low[(i << 1) + 1])));
            }
            value += val;

            String s1 = String.Format("{0}", value);
            textBox_Protocol_Rev.Text = s1;
            /* --------------------------------------- */
            value = 0;
            sub_high = data_to_decode.Substring(MODBUS_OFFSET + 42, 16);

            textBox_UID_Carel.Text = sub_high;
        }





        private int Decoding_3(int offset) 
        {
            string sub_low = @"";
            string sub_high = @"";
            short tmp_l = 0;
            short tmp_h = 0;
            UInt16 value = 0;
            bool pass_fail = false;
            byte val = 0;
            int retval;
            int offsetrel;

            offsetrel = offset + 2;
            retval = offsetrel;

            try
            {
                /* --------------------------------------- */
                sub_high = data_to_decode.Substring(offsetrel, 2);
                for (int i = 0; i < sub_high.Length >> 1; ++i)
                {
                    val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
                }
                value = (UInt16)(((UInt16)val));

                String s0 = String.Format("{0:X}", value);
                textBox_cmd17_back_comp.Text = s0.ToString();

                if (value == 1)
                {  // APPLICA BLOCK detected
                    retval = Decode_Applica_Block(offset);
                }

                if (value == 3)
                {  // cpCO BLOCK detected
                    retval = Decode_cpCO_Block(offset);
                }

                if (value == 6)
                {  // cpCO BLOCK detected
                   //retval = Decode_cpco_Block(offset);
                }
            }
            catch 
            {
            
            }

            return retval;
        }


        private int Decode_Applica_Block(int offset) 
        {
            string sub_low = @"";
            string sub_high = @"";
            short tmp_l = 0;
            short tmp_h = 0;
            UInt16 value = 0;
            bool pass_fail = false;
            byte val = 0;
            int retval = 0;

            int offset_rel;


            /* --------------------------------------- */
            value = 0;
            offset_rel = offset;
            sub_high = data_to_decode.Substring(offset_rel, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val));

            String s0 = String.Format("{0}", value);
            textBox_applica_lenght.Text = s0.ToString();

            /* --------------------------------------- */
            value = 0;
            offset_rel += 2;
            sub_high = data_to_decode.Substring(offset_rel, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val));

            String s1 = String.Format("{0}", value);
            textBox_applica_type.Text = s1.ToString();

            /* --------------------------------------- */
            value = 0;
            offset_rel += 2;
            sub_high = data_to_decode.Substring(offset_rel, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val));

            String s10 = String.Format("{0}", value);            
            textBox_applica_version.Text = s10.ToString();


            /* --------------------------------------- */
            offset_rel += 2; 
            sub_high = data_to_decode.Substring(offset_rel, 32);
            textBox_applica_prj_guid.Text = sub_high;
            /* --------------------------------------- */
            offset_rel += 32;
            sub_high = data_to_decode.Substring(offset_rel, 8);
            textBox_applica_prj_release.Text = sub_high;
            /* --------------------------------------- */
            offset_rel += 8; 
            sub_high = data_to_decode.Substring(offset_rel, 32);
            textBox_applica_OEM_Id.Text = sub_high;
            /* --------------------------------------- */
            offset_rel += 32;
            sub_high = data_to_decode.Substring(offset_rel, 32);
            textBox_Applica_Machine_Code.Text = sub_high;
            /* --------------------------------------- */
            offset_rel += 32;
            sub_high = data_to_decode.Substring(offset_rel, 32);
            textBox_applica_ov_oem_id.Text = sub_high;
            /* --------------------------------------- */
            offset_rel += 32;
            sub_high = data_to_decode.Substring(offset_rel, 32);
            textBox_applica_ov_mach_code.Text = sub_high;
            /* --------------------------------------- */
            offset_rel += 32;
            sub_high = data_to_decode.Substring(offset_rel, 4);
            textBox_applica_layout_id.Text = sub_high;
            /* --------------------------------------- */
            offset_rel += 4;
            sub_high = data_to_decode.Substring(offset_rel, 8);
            textBox_applica_layout_crc.Text = sub_high;
            /* --------------------------------------- */
            offset_rel += 8;
            sub_high = data_to_decode.Substring(offset_rel, 2);
            textBox_applica_zone_idx.Text = sub_high;

            offset_rel += 2;           
            return offset_rel;
        }


        private int Decode_cpCO_Block(int offset)
        {
            string sub_low = @"";
            string sub_high = @"";
            short tmp_l = 0;
            short tmp_h = 0;
            UInt16 value = 0;
            bool pass_fail = false;
            byte val = 0;
            int retval = 0;

            int offset_rel;


            /* --------------------------------------- */
            value = 0;
            offset_rel = offset;
            sub_high = data_to_decode.Substring(offset_rel, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val));

            String s0 = String.Format("{0}", value);
            textBox_cpco_lenght.Text = s0.ToString();

            /* --------------------------------------- */
            value = 0;
            offset_rel += 2;
            sub_high = data_to_decode.Substring(offset_rel, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val));

            String s1 = String.Format("{0}", value);
            textBox_cpco_type.Text = s1.ToString();

            /* --------------------------------------- */
            value = 0;
            offset_rel += 2;
            sub_high = data_to_decode.Substring(offset_rel, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val));

            String s10 = String.Format("{0}", value);
            textBox_cpco_version.Text = s10.ToString();
            /* --------------------------------------- */
            value = 0;
            offset_rel += 2;  

            sub_high = data_to_decode.Substring(offset_rel, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val) << 8);

            offset_rel += 2;
            sub_low = data_to_decode.Substring(offset_rel, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_low[i << 1]) << 4) + (GetHexVal(sub_low[(i << 1) + 1])));
            }
            value += val;

            String s11 = String.Format("{0}", value);

            textBox_cpco_tool_version.Text = s11;
            /* --------------------------------------- */
            offset_rel += 2;
            string sprjname = ""; 

            for (int c=0; c<40; c += 2) 
            {
                val=0;
                sub_high = data_to_decode.Substring(offset_rel, 2);
                for (int i = 0; i < sub_high.Length >> 1; ++i)
                {
                    val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
                }

                sprjname += Convert.ToChar(val); 
                offset_rel += 2;
            }

            textBox_cpco_prj_name.Text = sprjname;


            /* --------------------------------------- */
            string scfgname = "";

            for (int c = 0; c < 40; c += 2)
            {
                val = 0;
                sub_high = data_to_decode.Substring(offset_rel, 2);
                for (int i = 0; i < sub_high.Length >> 1; ++i)
                {
                    val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
                }

                scfgname += Convert.ToChar(val);
                offset_rel += 2;
            }

            textBox_cpco_cfg_name.Text = scfgname;

            /* --------------------------------------- */
            sub_high = data_to_decode.Substring(offset_rel, 16);
            textBox_cpco_active_plug_in.Text = sub_high;
            /* --------------------------------------- */
            value = 0;
            offset_rel += 16;

            sub_high = data_to_decode.Substring(offset_rel, 2);
            for (int i = 0; i < sub_high.Length >> 1; ++i)
            {
                val = (byte)((GetHexVal(sub_high[i << 1]) << 4) + (GetHexVal(sub_high[(i << 1) + 1])));
            }
            value = (UInt16)(((UInt16)val));

            String s12 = String.Format("{0}", value);
            textBox_cpco_core_type.Text = s12.ToString();

            offset_rel += 2;

            return offset_rel;
        }

        private void FormDecodeCMD17_Load(object sender, EventArgs e)
        {
            this.CenterToScreen();

            ckBox_cmd17_pcap_0.Checked  = false;
            ckBox_cmd17_pcap_1.Checked  = false;
            ckBox_cmd17_pcap_2.Checked  = false;
            ckBox_cmd17_pcap_3.Checked  = false;
            ckBox_cmd17_pcap_4.Checked  = false;
            ckBox_cmd17_pcap_5.Checked  = false;
            ckBox_cmd17_pcap_6.Checked  = false;
            ckBox_cmd17_pcap_7.Checked  = false;
            ckBox_cmd17_pcap_8.Checked  = false;
            ckBox_cmd17_pcap_9.Checked  = false;
            ckBox_cmd17_pcap_10.Checked = false;
            ckBox_cmd17_pcap_11.Checked = false;

            if (data_to_decode.Length > 0)
            {
                Decoding_1();
                Decoding_2();

                int len = data_to_decode.Length ;
                int retv = 70; //end of fixed part

                //start decoding dynamic part
                while (retv < len)
                {
                    retv = Decoding_3(retv);
                }
                

                
                


            }
            else 
            {
                DialogResult result1 = MessageBox.Show("Error buffer is empty! ",
                                                       "Please do a Scan Line",
                                                        MessageBoxButtons.OK);
            }

        }
    }
}
