using api.services;

namespace Services;

public interface IEsp32Service
{
    object AddTempAndHum(float temperature, float humidity);
    List<TempData> GetAll();
}