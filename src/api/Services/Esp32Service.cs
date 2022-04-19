using api.services;

namespace api.Services;

public class Esp32Service : IEsp32Service
{
    private readonly List<TempData> _data;

    public Esp32Service()
    {
        this._data = new List<TempData>();
    }

    public object AddTempAndHum(float temperature, float humidity, float heatIndex, float dewPoint, string comfortStatus)
    {
        _data.Add(new TempData(humidity, temperature, heatIndex, dewPoint, comfortStatus,DateTime.UtcNow));
        return $"ok => {DateTime.UtcNow}: {temperature}, {humidity}";
    }

    public List<TempData> GetAll()
    {
        return _data;
    }
}