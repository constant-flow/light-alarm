#pragma once
#include <NTPClient.h>

class LightTimeServer
{
private:
    long lastReportedTime = 0;
    long timeReportInterval = 30000;
    WiFiUDP ntpUDP;
    NTPClient *timeClient = NULL;

public:
    void begin()
    {
        // timeClient = new NTPClient(ntpUDP, "0.de.pool.ntp.org", 7200); // Germany summer time
        timeClient = new NTPClient(ntpUDP, "0.de.pool.ntp.org", 3600); // Germany winter time
    }

    void update()
    {
        Serial.println(timeClient->getFormattedTime());
        timeClient->update();
    }

    int getHourOfDay()
    {
        return timeClient->getHours();
    }

    int getMinuteOfDay()
    {
        return timeClient->getMinutes();
    }

    long getSecondsToday()
    {
        return (timeClient->getHours() * 60 * 60 + timeClient->getMinutes() * 60 + timeClient->getSeconds());
    }

    int getDay()
    {
        timeClient->getDay();
    }
};
