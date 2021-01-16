

#include "WebServer.h"
#include "LightWebSocket2.h"
#include "LightWifi.h"
#include "LightTimeServer.h"
#include "LightSource.h"
#include "LightAlarmManager.h"
#include <ArduinoJson.h>

WebServer webServer;
LightWebSocket2 webSocket;
LightWifi wifi;
LightTimeServer webTime;
LightSource light;
LightAlarmManager alarmManager;

// control pin for the mosfet controlling the led outpus
int ledPin = 27;

void setup()
{
  Serial.begin(115200);
  delay(10);

  wifi.begin("SSID", "YourSecureWifiPassword", "lightalarm");

  webServer.begin();
  webSocket.setOnMessage(onMsg);
  webSocket.begin();
  webTime.begin();
  alarmManager.begin(&webTime);

  light.begin(ledPin, 0.994f, 0);
  webSocket.setWelcomeMessage("{\"time\":{\"hour\":\"07\",\"minute\":\"00\"}}");
}

void loop()
{
  wifi.loop();
  webServer.loop();
  webSocket.loop();
  light.loop();
  alarmManager.loop();

  float brightness = alarmManager.getBrightnessByTime();
  light.setBrightness(brightness);
}

void onMsg(const char *payload)
{
  StaticJsonDocument<512> doc;
  deserializeJson(doc, payload);
  alarmManager.turnOffForToday(false);

  if (doc["dim"])
  {
    float brightness = atoi(doc["dim"]) / 100.f;
    alarmManager.setBaseBrightness(brightness);
  }

  if (doc["time"])
  {
    int receivedHour = doc["time"]["hour"];
    int receivedMinute = doc["time"]["minute"];
    alarmManager.setAlarmTime(receivedHour, receivedMinute);
    alarmManager.updateAlarmTime();

    const int size = 100;
    char buffer[size];
    serializeJson(doc, buffer, size);
    webSocket.setWelcomeMessage(buffer);
  }

  if (doc["duration"])
  {
    int duration = atoi(doc["duration"]);
    alarmManager.setOnDuration(duration);
  }

  if (doc["transition"])
  {
    int duration = atoi(doc["transition"]);
    alarmManager.setFadeInDuration(duration);
    alarmManager.setFadeOutDuration(duration);
  }

  if (doc["turnOff"])
  {
    alarmManager.turnOffForToday();
  }
}
