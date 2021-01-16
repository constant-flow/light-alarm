static const int LIGHT_FREQ = 200;
static const int LEDC_TIMER_13_BIT = 13;

class LightSource
{
private:
    int channel;
    int pin;
    float targetBrightness = 0.0f, brightness = 0.0f;
    float smoothing;

public:
    void begin(int ledPin = 13, float smoothing = 0.99f, int ledChannel = 0)
    {
        this->smoothing = smoothing;
        channel = ledChannel;
        pin = ledPin;
        ledcSetup(channel, LIGHT_FREQ, LEDC_TIMER_13_BIT);
        ledcAttachPin(pin, channel);
    }
    void loop()
    {
        brightness = smoothing * brightness + (1 - smoothing) * targetBrightness;
        // calculate duty, 8191 from 2 ^ 13 - 1
        uint32_t duty = min((int)(8191 * brightness), 8191);

        // write duty to LEDC
        ledcWrite(channel, duty);
    }

    void setBrightness(float brightness)
    {
        targetBrightness = brightness * brightness;
    }
};