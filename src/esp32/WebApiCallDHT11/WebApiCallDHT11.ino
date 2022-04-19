/**
   OnDemandConfigPortal.ino
   example of running the configPortal AP manually, independantly from the captiveportal
   trigger pin will start a configPortal AP for 120 seconds then turn it off.

*/
#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <HTTPClient.h>

String serverName = "http://192.168.5.10:3000/temp";

DHTesp dht;
int dhtPin = 17;
/** Comfort profile */
ComfortState cf;

bool initTemp() {
  byte resultValue = 0;
  // Initialize temperature sensor
  dht.setup(dhtPin, DHTesp::DHT11);
  Serial.println("DHT initiated");
  return true;
}

String getTemperature() {

  // Reading temperature for humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
  TempAndHumidity newValues = dht.getTempAndHumidity();
  // Check if any reads failed and exit early (to try again).
  if (dht.getStatus() != 0) {
    return ("DHT11 error status: " + String(dht.getStatusString()));
    //return false;
  }

  float heatIndex = dht.computeHeatIndex(newValues.temperature, newValues.humidity);
  float dewPoint = dht.computeDewPoint(newValues.temperature, newValues.humidity);
  float cr = dht.getComfortRatio(cf, newValues.temperature, newValues.humidity);

  String comfortStatus;
  switch (cf) {
    case Comfort_OK:
      comfortStatus = "Comfort_OK";
      break;
    case Comfort_TooHot:
      comfortStatus = "Comfort_TooHot";
      break;
    case Comfort_TooCold:
      comfortStatus = "Comfort_TooCold";
      break;
    case Comfort_TooDry:
      comfortStatus = "Comfort_TooDry";
      break;
    case Comfort_TooHumid:
      comfortStatus = "Comfort_TooHumid";
      break;
    case Comfort_HotAndHumid:
      comfortStatus = "Comfort_HotAndHumid";
      break;
    case Comfort_HotAndDry:
      comfortStatus = "Comfort_HotAndDry";
      break;
    case Comfort_ColdAndHumid:
      comfortStatus = "Comfort_ColdAndHumid";
      break;
    case Comfort_ColdAndDry:
      comfortStatus = "Comfort_ColdAndDry";
      break;
    default:
      comfortStatus = "Unknown:";
      break;
  };

  return "/" + String(newValues.temperature) + "/" + String(newValues.humidity) + "/" + String(heatIndex) + "/" + String(dewPoint) + "," + comfortStatus;
}


// select which pin will trigger the configuration portal when set to LOW
#define TRIGGER_PIN 0

int timeout = 120; // seconds to run for

const int led = 13;

int ApiIsReady = 0;

void setup() {
  initTemp();
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("\n Starting");
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
}

void startApi() {
  WiFiManager wm;
  bool res;

  res = wm.autoConnect("AutoConnectAP", "password"); // password protected ap

  if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  }

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  ApiIsReady = 1;
}

void loop() {
  // is configuration portal requested?
  if ( digitalRead(TRIGGER_PIN) == LOW) {
    WiFiManager wm;

    //reset settings - for testing
    //wm.resetSettings();

    // set configportal timeout
    wm.setConfigPortalTimeout(timeout);

    if (!wm.startConfigPortal("OnDemandAP")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.restart();
      delay(5000);
    }

    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");

  }

  //Serial.println("Api is ready");
  // put your main code here, to run repeatedly:

  if (ApiIsReady == 0) {
    startApi();
  } else {

    HTTPClient http;
    String serverPath = serverName + getTemperature() ;
    Serial.println(serverPath);
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);

    }
    delay(5000);

  }

}
