using api.Services;

var builder = WebApplication.CreateBuilder(args);
var app = builder.Build();
app.Logger.LogInformation("The app started");

app.MapGet("/", () => "Hello World!");

IEsp32Service service = new Esp32Service();
app.MapGet("/Temp/{temperature}/{humidity}", (float temperature, float humidity) => service.AddTempAndHum(temperature, humidity));
app.MapGet("/temp", () => service.GetAll());

app.Run("http://localhost:3000");
