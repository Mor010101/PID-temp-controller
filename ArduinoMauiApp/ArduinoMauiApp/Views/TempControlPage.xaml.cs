using ArduinoMauiApp.Core;
using ArduinoMauiApp.Models;
using Microcharts;
using SkiaSharp.Views.Maui;
using System.Threading;

namespace ArduinoMauiApp.Views;

public partial class TempControlPage : ContentPage
{
    Models.Chart _chart;
    public TempControlPage()
	{
		InitializeComponent();
        if (BindingContext is Models.Chart chart)
		{
			Chart.Chart = chart.chart;
            Chart.Chart.IsAnimated = false;
            UartComm comm = new UartComm(chart);
            _chart= chart;
        }
    }

    private void sendButtonHandler(object sender, EventArgs e)
    {
        _chart.initChart();
    }
}