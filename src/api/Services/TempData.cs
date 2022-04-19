namespace api.services;

public record TempData(float Humidity, float Temperature, float heatIndex, float dewPoint, string comfortStatus, DateTime Time = default);