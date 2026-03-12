/*
UNO R4 WiFi version of PushingBox Google Spreadsheet example
*/

#include <WiFiS3.h>
#include <ArduinoHttpClient.h>
#include "arduino_secrets.h"

// Add your PushingBox Scenario DeviceID here
const char devid[] = "v125962FAE5A95AC";

const char serverName[] = "api.pushingbox.com";
const int serverPort = 80;

bool DEBUG = true;

// Create WiFi + HTTP clients
WiFiClient wifi;
HttpClient client(wifi, serverName, serverPort);

void connectWiFi()
{
  if (DEBUG) {
    Serial.print("Connecting to ");
    Serial.println(SECRET_SSID);
  }

  int status = WiFi.begin(SECRET_SSID, SECRET_PASS);

  while (status != WL_CONNECTED) {
    delay(1000);
    if (DEBUG) Serial.print(".");
    status = WiFi.status();
  }

  if (DEBUG) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(9600);
  delay(2000);

  connectWiFi();

  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  int sensorValue = analogRead(A0);

  String path = "/pushingbox?devid=" + String(devid) +
                "&IDtag=100&TimeStamp=50&TempC=" + String(sensorValue);

  if (DEBUG) {
    Serial.print("Request: ");
    Serial.println(path);
  }

  client.get(path);

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  if (DEBUG) {
    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
  }

  Serial.print("Uploaded value: ");
  Serial.println(sensorValue);

  delay(5000);
}