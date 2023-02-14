using ArduinoMauiApp.Models;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ArduinoMauiApp.Core;

internal class UartComm
{
    SerialPort comPort = new SerialPort();
    Chart chart;
    int counter = 0;
    public UartComm()
    {
        comPort = new SerialPort();
        comPort.PortName = "COM10";//Set your board COM
        comPort.BaudRate = 9600;
        comPort.Open();
        Thread t = new Thread(new ThreadStart(loop));
        t.Start();
    }

    public UartComm(Chart chart)
    {
      
        this.chart = chart;
        comPort = new SerialPort();
        comPort.PortName = "COM10";//Set your board COM
        comPort.BaudRate = 9600;
        comPort.Open();
        Thread t = new Thread(new ThreadStart(loop));
        t.Start();

    }

    public void loop()
    {
        System.String[] token = new String[2];

        while (true)
        {
            counter++;
            String a = comPort.ReadLine();
            
            token = a.Split(';');
            if (token.Length > 1)
            {
                
                    var data = new DataPoint();

                    data.Temp = Double.Parse(token[0]);
                    data.Time = counter;
                    if (counter > 48)
                    {
                        chart.DataPoints.RemoveAt(0);
                    }
                
                chart.DataPoints.Add(data);

                //Debug.WriteLine(data.Temp + " " + data.Time);
                Thread.Sleep(500);
            }
        }
    }
}
