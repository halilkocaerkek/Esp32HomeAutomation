/**
   OnDemandConfigPortal.ino
   example of running the configPortal AP manually, independantly from the captiveportal
   trigger pin will start a configPortal AP for 120 seconds then turn it off.

*/
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

// select which pin will trigger the configuration portal when set to LOW
#define TRIGGER_PIN 0

int timeout = 120; // seconds to run for

WiFiServer server(80);
const int led = 13;

int ApiIsReady = 0;

void setup() {
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
  server.begin();
  Serial.println("HTTP server started");
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



  WiFiClient client = server.available();

    if (client) {                             // if you get a client,
      Serial.println("New Client.");           // print a message out the serial port
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected()) {            // loop while the client's connected
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          Serial.write(c);                    // print it out the serial monitor
          if (c == '\n') {                    // if the byte is a newline character

            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();

              // the content of the HTTP response follows the header:
              client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
              client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");

              // The HTTP response ends with another blank line:
              client.println();
              // break out of the while loop:
              break;
            } else {    // if you got a newline, then clear currentLine:
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }

          // Check to see if the client request was "GET /H" or "GET /L":
          if (currentLine.endsWith("GET /H")) {
            digitalWrite(led, HIGH);               // GET /H turns the LED on
          }
          if (currentLine.endsWith("GET /L")) {
            digitalWrite(led, LOW);                // GET /L turns the LED off
          }
        }
      }
      // close the connection:
      client.stop();
      Serial.println("Client Disconnected.");
    }
  }
}
