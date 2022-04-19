using api.Services;

var builder = WebApplication.CreateBuilder(args);
var app = builder.Build();
app.Logger.LogInformation("The app started");

app.MapGet("/", () => "Hello World!");

IEsp32Service service = new Esp32Service();
app.MapGet("/Temp/{temperature}/{humidity}/{heatIndex}/{dewPoint}/{comfortStatus}",
    (float temperature, float humidity, float heatIndex, float dewPoint, string comfortStatus) =>
        service.AddTempAndHum(temperature, humidity, heatIndex, dewPoint, comfortStatus));
app.MapGet("/temp", () => service.GetAll());

app.Run("http://localhost:3000");
