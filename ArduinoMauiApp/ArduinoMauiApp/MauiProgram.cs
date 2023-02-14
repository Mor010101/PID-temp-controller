using ArduinoMauiApp.Core;
using ArduinoMauiApp.Models;
using SkiaSharp.Views.Maui.Controls.Hosting;
using System.Diagnostics;
using System.IO.Ports;

namespace ArduinoMauiApp;

public static class MauiProgram
{
	public static MauiApp CreateMauiApp()
	{
        var builder = MauiApp.CreateBuilder();
		builder
			.UseMauiApp<App>()
			.UseSkiaSharp()
			.ConfigureFonts(fonts =>
			{
				fonts.AddFont("OpenSans-Regular.ttf", "OpenSansRegular");
				fonts.AddFont("OpenSans-Semibold.ttf", "OpenSansSemibold");
			});


		return builder.Build();

		
    }
}
