using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArduinoMauiApp.Models;

class ControlParameters
{
    public int SetTemp { get; set; }
    public Double Kp { get; set; }
    public Double Ki { get; set; }
    public Double Kd { get; set; }
    public int TimpIncalzire { get; set; }
    public int TimpMentinere { get; set; }
    public int TimpRacire { get; set; }

}
