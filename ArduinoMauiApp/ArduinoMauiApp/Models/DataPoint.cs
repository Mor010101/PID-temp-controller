using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArduinoMauiApp.Models;

class DataPoint
{
    public Double Temp { get; set; }
    public int Time { get; set; }
    public DataPoint() { }
    public DataPoint(double temp, int time)
    {
        Temp = temp;
        Time = time;
    }
}
