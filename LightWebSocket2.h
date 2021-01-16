#include <ArduinoWebsockets.h>
#include <vector>
#include <algorithm>

using namespace websockets;

#define log(x)             \
    {                      \
        Serial.println(x); \
        Serial.flush();    \
    }

void (*onWsMessage)(const char *) = nullptr;
char welcomeMessage[100];

void onMessage(WebsocketsClient &client, WebsocketsMessage message)
{
    // digitalWrite(led, lightStatus);
    // lightStatus = !lightStatus;

    String data = message.data();
    if (onWsMessage)
    {
        onWsMessage(data.c_str());
    }
}

void onEventsCallback(WebsocketsClient &client, WebsocketsEvent event, String data)
{
    if (event == WebsocketsEvent::ConnectionOpened)
    {
        Serial.println("WS Connection Opened");
    }
    else if (event == WebsocketsEvent::ConnectionClosed)
    {
        Serial.println("WS Connnection Closed");
        client = {};
    }
    else if (event == WebsocketsEvent::GotPing)
    {
        Serial.println("Got a Ping!");
    }
    else if (event == WebsocketsEvent::GotPong)
    {
        Serial.println("Got a Pong!");
    }
    else
    {
        Serial.println("Different WS event");
    }
}

class LightWebSocket2
{
private:
    WebsocketsServer server;
    WebsocketsClient client = {};

public:
    void begin()
    {
        server.listen(81);
        Serial.print("Websocket server: ");
        Serial.println(server.available());
    }

    void loop()
    {
        if (server.available())
        {
            if (server.poll())
            {
                //accept the connection and register callback
                client = server.accept();
                client.send(welcomeMessage);
                log("callbacks registered");
                client.onMessage(onMessage);
                log("onMessage set");
                client.onEvent(onEventsCallback);
                log("onEvent set");
            }
        }
        client.poll();
    }

    void setOnMessage(void (*onMessage)(const char *))
    {
        onWsMessage = onMessage;
    }

    void setWelcomeMessage(const char *message)
    {
        strcpy(welcomeMessage, message);
        Serial.println("set Welcome");
        log(message);
    }
};