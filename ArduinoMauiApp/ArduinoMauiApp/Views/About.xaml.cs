namespace ArduinoMauiApp.Views;

public partial class About : ContentPage
{
	public About()
	{
		InitializeComponent();
	}

    public async void LearnMore_Clicked(object sender, EventArgs e)
    {
        if (BindingContext is Models.About about)
        {
            await Launcher.Default.OpenAsync(about.MoreInfoUrl);
        }
    }
}