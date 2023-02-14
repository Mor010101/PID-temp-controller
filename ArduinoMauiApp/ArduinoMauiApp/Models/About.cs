using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArduinoMauiApp.Models
{
    class About
    {
        public string Title => AppInfo.Name;
        public string Version => AppInfo.VersionString;
        public string MoreInfoUrl => "https://www.linkedin.com/in/tudor-morosan-8a23ab238/";
        public string Message => "This is the message uwu";
    }
}
