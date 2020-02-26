using System;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;




namespace MqttClientSimulatorBinary
{
    class IniFile
    {
        string Path;
        string EXE = Assembly.GetExecutingAssembly().GetName().Name;

        [DllImport("kernel32", CharSet = CharSet.Unicode)]
        static extern long WritePrivateProfileString(string Section, string Key, string Value, string FilePath);

        [DllImport("kernel32", CharSet = CharSet.Unicode)]
        static extern int GetPrivateProfileString(string Section, string Key, string Default, StringBuilder RetVal, int Size, string FilePath);

        public IniFile(string IniPath = null)
        {
            Path = new FileInfo(IniPath ?? EXE + ".ini").FullName.ToString();
        }

        public string Read(string Key, string Section = null)
        {
            var RetVal = new StringBuilder(255);
            GetPrivateProfileString(Section ?? EXE, Key, "", RetVal, 255, Path);
            return RetVal.ToString();
        }

        public void Write(string Key, string Value, string Section = null)
        {
            WritePrivateProfileString(Section ?? EXE, Key, Value, Path);
        }

        public void DeleteKey(string Key, string Section = null)
        {
            Write(Key, null, Section ?? EXE);
        }

        public void DeleteSection(string Section = null)
        {
            Write(null, null, Section ?? EXE);
        }

        public bool KeyExists(string Key, string Section = null)
        {
            return Read(Key, Section).Length > 0;
        }
    }


}

#if EXAMPLE==TRUE

    /* 
    code snippet on how to use theabove
    */

    // Creates or loads an INI file in the same directory as your executable
    // named EXE.ini (where EXE is the name of your executable)
    //var MyIni = new IniFile();

    // Or specify a specific name in the current dir
    //var MyIni = new IniFile("MqttClientSimulatorBinary_settings.ini");

    // Or specify a specific name in a specific dir
    //var MyIni = new IniFile(@"C:\Settings.ini");

    //this example 1
    //MyIni.Write("DefaultVolume", "100");
    //MyIni.Write("HomePage", "http://www.google.com");


    //generate this example 1
    //[MyProg]
    //DefaultVolume=100
    //HomePage=http://www.google.com


    //this example 2
    //var DefaultVolume = IniFile.Read("DefaultVolume");
    //var HomePage = IniFile.Read("HomePage");

    //will read this example 2
    //[Audio]
    //DefaultVolume=100

    //[Web]
    //HomePage=http://www.google.com



    //Example 3 You can also check for the existence of a key like so:
    //if(!MyIni.KeyExists("DefaultVolume", "Audio"))
    //{
    //    MyIni.Write("DefaultVolume", "100", "Audio");
    //}


    //You can delete a key like so:
    //MyIni.DeleteKey("DefaultVolume", "Audio");

    //You can also delete a whole section (including all keys) like so:
    //MyIni.DeleteSection("Web");

#endif


