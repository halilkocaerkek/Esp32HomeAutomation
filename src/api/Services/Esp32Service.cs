using api.services;

namespace Services;

public class Esp32Service : IEsp32Service
{
    private List<TempData> _data;

    public Esp32Service()
    {
        this._data = new List<TempData>();
    }

    public object AddTempAndHum(float temperature, float humidity)
    {
        _data.Add(new TempData(humidity, temperature, DateTime.UtcNow));
        return $"ok => {temperature}, {humidity}";
    }

    public List<TempData> GetAll()
    {
        return _data;
    }
}