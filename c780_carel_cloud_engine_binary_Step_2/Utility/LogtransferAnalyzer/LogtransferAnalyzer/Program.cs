using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LogtransferAnalyzer
{
    static class Program
    {
        /// <summary>
        /// Punto di ingresso principale dell'applicazione.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            

            if (args.Length != 0)
            {
                             
               Form theform = new Form1(args[0], args[1]);
               Application.Run(theform);
            }
            else 
            {
                Form theform = new Form1();
                Application.Run(theform);
            }

            
        }
    }
}
