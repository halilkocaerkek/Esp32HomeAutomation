using api.services;

namespace api.Services;

public interface IEsp32Service
{
 
    List<TempData> GetAll(); 
    object AddTempAndHum(float temperature, float humidity, float heatIndex, float dewPoint, string comfortStatus);
}