using Microcharts;
using SkiaSharp;
using System.Collections.ObjectModel;
using System.ComponentModel;

namespace ArduinoMauiApp.Models;

class Chart
{
    public List<DataPoint> DataPoints { get; set; } = new List<DataPoint>();
    public LineChart chart;
    public int maxTemp = 50;
    public Chart()
    {
        //TO DO: populate chart with data
       RandomData(4);
       initChart();
        Thread t = new Thread(new ThreadStart(loop));
        t.Start();
    }

    private void loop()
    {
        while (true)
        {
            updateChart();
            Thread.Sleep(1000);
        }
    }

    //generates count random dataPoints and adds them to the collection
    public void RandomData(int count)
    {
        var random = new Random(); 

        for(int i = 0; i < count; i++)
        {
            DataPoint d = new DataPoint(random.NextDouble()*maxTemp, i);
            DataPoints.Add(d);
        }
    }

    public void viewData()
    {
        for (int i = 0; i < DataPoints.Count;i++)
        {
            System.Diagnostics.Debug.WriteLine(DataPoints[i].Temp + " " + DataPoints[i].Time);
        }
        System.Diagnostics.Debug.WriteLine("");
    }

    public void initChart()
    {
       
        chart = new LineChart()
        {
            Entries = DataPoints.Select(x => new ChartEntry((float)x.Temp)
            {
                Label = x.Time.ToString(),
                //Color = SKColor.Parse($"#{((int)x.Temp)*100000:X6}"),
                Color = SKColor.FromHsv(0,(int)x.Temp*100/maxTemp, 55),
                ValueLabel = x.Time.ToString()
            })
        };

       
    }

    public void updateChart()
    {
        //viewData();
        chart.Entries = DataPoints.Select(x => new ChartEntry((float)x.Temp)
        {
            Label = x.Time.ToString(),
            //Color = SKColor.Parse($"#{((int)x.Temp)*100000:X6}"),
            Color = SKColor.FromHsv(0, (int)x.Temp * 100 / maxTemp, 55),
            ValueLabel = x.Temp.ToString()
        });
    }

    
}
