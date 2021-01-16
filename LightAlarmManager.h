#pragma once
#include "LightTimeServer.h"

class LightAlarmManager
{
public:
    void
    begin(LightTimeServer *timeServer)
    {
        webTime = timeServer;
        tuneRTC();
    }

    void loop()
    {
        if (millis() > slowUpdateTimer)
        {
            slowUpdateTimer = millis() + slowUpdate;
            checkIfDayHasChanged();
        }
    }

    // lowest brightness of the light (useful for manual dimming)
    void setBaseBrightness(float brightness)
    {
        baseBrightness = brightness;
    }

    void setAlarmTime(int hour, int minute)
    {
        alarmOffsetFromDayStart = toSecondsOfDay(hour, minute);
    }

    void setOnDuration(int durationInMinutes)
    {
        alarmRunningTime = 60 * durationInMinutes;
    }

    void setFadeInDuration(int durationInMinutes)
    {
        alarmRampUpTime = 60 * durationInMinutes;
    }

    void setFadeOutDuration(int durationInMinutes)
    {
        alarmRampDownTime = 60 * durationInMinutes;
    }

    float getBrightnessByTime()
    {
        if (deactivateForToday)
            return baseBrightness;

        const long timeRampUpStart = alarmTimeNext - alarmRampUpTime;
        const long timeRampUpEnd = alarmTimeNext;
        const long timeRampDownStart = alarmTimeNext + alarmRunningTime;
        const long timeRampDownEnd = timeRampDownStart + alarmRampDownTime;

        const double now = millisRTC() / 1000.0d;

        if (now < timeRampUpStart)
            return baseBrightness;
        else if (now < timeRampUpEnd)
            return baseBrightness + float(now - timeRampUpStart) / alarmRampUpTime * (1 - baseBrightness);
        else if (now < timeRampDownStart)
            return 1;
        else if (now < timeRampDownEnd)
            return baseBrightness + float(timeRampDownEnd - now) / alarmRampDownTime * (1 - baseBrightness);
        else
        {
            Serial.println("Set timer for the next day!");
            updateAlarmTime(1);
            return baseBrightness;
        }
    }

    // schedules the next alarm
    void updateAlarmTime(long daysOffset = 0)
    {
        tuneRTC();
        alarmTimeNext = alarmOffsetFromDayStart + daysOffset * 24 * 60 * 60;
    }

    void turnOffForToday(boolean turnOff = true)
    {
        deactivateForToday = turnOff;
    }

private:
    // time in seconds before the alarm time to ramp up the light
    long alarmRampUpTime = 15 * 60;
    // time in seconds from alarm time until it starts to fade off
    long alarmRunningTime = 60 * 60;
    // time in seconds to fade of
    long alarmRampDownTime = 10;

    // minimal brightness, when set manually by the slide
    float baseBrightness = 0;

    // defines the second of the day when the alarm starts
    long alarmOffsetFromDayStart = 0;
    // defines the next scheduled alarm
    long alarmTimeNext = 0;
    // calibrated offset from ntp to adjust millis to the actual second of the day
    long rtcOffset = 0;

    // number of the current day, used to detect, when a alarm need to be reset,
    // for the current day
    int currentDay = -1;

    // check less frequently certain values, e.g day changed

    // interval in millis how often to check certain values
    long slowUpdate = 60000;
    // timer for the slowUpdates
    long slowUpdateTimer = 0;

    // if true, the light is off, was canceled by user
    bool deactivateForToday = false;

    LightTimeServer *webTime;

    // converts provided hour and minute of the day to the total passed seconds of the day
    static long toSecondsOfDay(int hour, int minute)
    {
        return (minute + hour * 60) * 60; //s
    }

    // like millis() except, it returns the passed millis since the day has begun
    long millisRTC()
    {
        return millis() + rtcOffset * 1000;
    }

    // tunes the clock to be in sync with the rest of the world
    void tuneRTC()
    {
        webTime->update();
        long realSeconds = webTime->getSecondsToday();
        Serial.printf("web %ld and millis(): %ld\n", realSeconds, millis());
        rtcOffset = realSeconds - (millis() / 1000);
    }

    bool checkIfDayHasChanged()
    {
        if (webTime->getDay() != currentDay)
        {
            updateAlarmTime();
            currentDay = webTime->getDay();
            turnOffForToday(false);
        }
    }
};
