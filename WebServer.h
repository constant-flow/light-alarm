#pragma once

#include <WiFi.h>

class WebServer
{
private:
    WiFiServer server;

public:
    void begin(int port = 80)
    {
        server.begin(port);
    }

    void loop()
    {
        WiFiClient client = server.available(); // listen for incoming clients

        if (client)
        {                                  // if you get a client,
            Serial.println("New Client."); // print a message out the serial port
            String currentLine = "";       // make a String to hold incoming data from the client
            while (client.connected())
            { // loop while the client's connected
                if (client.available())
                {                           // if there's bytes to read from the client,
                    char c = client.read(); // read a byte, then
                    Serial.write(c);        // print it out the serial monitorc
                    if (c == '\n')
                    { // if the byte is a newline character

                        // if the current line is blank, you got two newline characters in a row.
                        // that's the end of the client HTTP request, so send a response:
                        if (currentLine.length() == 0)
                        {
                            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                            // and a content-type so the client knows what's coming, then a blank line:
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/html");
                            client.println();

                            // the content of the HTTP response follows the header:
                            // client.print("<!DOCTYPE html><html lang=\"en\"> <head> <meta charset=\"UTF-8\" /> <meta name=\"viewport\" content=\"initial-scale=1.5\" /> <title>Document</title> </head> <body style=\" font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif; background-color: #202020; color: #2e2e2e; \" > <div style=\" width: 100vw; height: 80vh; display: flex; align-items: center; justify-content: space-around; \" > <div id=\"connected\" style=\" margin: auto; width: 50%; background-color: #c0c0c0; padding: 0px 16px 16px 16px; border-radius: 4px; \" > <h2>LightAlarm</h2> <div class=\"slidecontainer\"> <div>Minimum Brightness</div> <input type=\"range\" min=\"0\" max=\"100\" value=\"0\" class=\"slider\" id=\"myRange\" style=\"width: 100%\" /> </div> <br /> <div>Alarm time</div> <input type=\"time\" id=\"time\" name=\"appt\" style=\"width: 100%\" required /> </div> <div id=\"not-connected\" style=\" background-color: cornflowerblue; padding: 16px; border-radius: 128px; \" > <div>Connecting to light ...</div> </div> </div> <ul id=\"logs\" style=\"font-family: 'Courier New', Courier, monospace\"></ul> <script> console.log(\"Code executed\"); const logsElement = document.querySelector(\"#logs\"); window.onload = () => { const showElement = (selector, visible) => { const element = document.querySelector(selector); const visValue = visible ? \"block\" : \"none\"; element.style.display = visValue; }; document.showInterface = (visible) => { showElement(\"#connected\", visible); showElement(\"#not-connected\", !visible); }; document.showInterface(false); var oldLog = console.log; console.log = function (message) { oldLog.apply(console, arguments); const newLogElement = document.createElement(\"li\"); newLogElement.innerHTML = arguments[0]; logsElement.prepend(newLogElement); }; var oldErr = console.error; console.error = function (message) { oldErr.apply(console, arguments); const newErrElement = document.createElement(\"li\"); newErrElement.setAttribute(\"style\", \"color:red;\"); newErrElement.innerHTML = arguments[0]; logsElement.prepend(newErrElement); }; var oldThrow = this.throw; window.onerror = function (message, file, line, position, error) { const newErrElement = document.createElement(\"li\"); newErrElement.setAttribute(\"style\", \"color:red;\"); newErrElement.innerHTML = message; logsElement.prepend(newErrElement); }; }; ws = new WebSocket(\"ws://lightalarm:81\"); ws.onopen = function (event) { console.log(\"ws connected\"); document.showInterface(true); }.bind(this); ws.onerror = function (error) { console.log(`[error] ${error.message}`); }; const sendToArduino = (jsonPayload) => { ws.send(JSON.stringify(jsonPayload)); }; var slider = document.getElementById(\"myRange\"); slider.oninput = function () { sendToArduino({ dim: this.value }); }; var timer = document.getElementById(\"time\"); timer.oninput = function () { let alarmTime = this.value.split(\":\"); let objectToSend = { time: { hour: alarmTime[0], minute: alarmTime[1] }, }; sendToArduino(objectToSend); }; </script> </body></html>");
                            client.print("<!DOCTYPE html><html lang=\"en\"> <head> <meta charset=\"UTF-8\" /> <meta name=\"viewport\" content=\"initial-scale=1.5\" /> <meta name=\"theme-color\" content=\"#000000\" /> <title>Document</title> </head> <style> * { box-sizing: border-box; } body { font-family: -apple-system, BlinkMacSystemFont, \"Segoe UI\", Roboto, Oxygen, Ubuntu, Cantarell, \"Open Sans\", \"Helvetica Neue\", sans-serif; background-image: radial-gradient(circle, #303030, #000000); color: #181815; padding: 0px; margin: 0px; height: 100vh; } #app { width: 100vw; height: 80vh; display: flex; align-items: center; justify-content: space-around; } #connected { margin: auto; width: 50%; min-width: 200px; padding: 0px 16px 16px 16px; border-radius: 4px; box-shadow: 0px 0px 20px #ffffff20; background-color: cornflowerblue; } input { background-color: #000000aa; border: 0px; padding: 4px; color: #ffffff; border-radius: 2px; } #not-connected { background-color: cornflowerblue; padding: 16px; border-radius: 128px; } #logs { font-family: \"Courier New\", Courier, monospace; } .column-container { display: flex; flex-direction: row; justify-content: space-between; } .column { display: flex; flex-direction: column; flex-basis: 800px; } .slider { -webkit-appearance: none; width: 100%; height: 25px; outline: none; opacity: 1; -webkit-transition: 0.2s; transition: opacity 0.2s; } .slider:hover { opacity: 1; } .slider::-webkit-slider-thumb { -webkit-appearance: none; appearance: none; width: 25px; height: 25px; border-radius: 2px; background: #ffffff; box-shadow: 0px 0px 20px #ffffff80; cursor: grab; } .slider::-moz-range-thumb { width: 25px; height: 25px; background: #4caf50; cursor: pointer; } </style> <body> <div id=\"app\"> <div id=\"connected\"> <h2 id=\"title\">LightAlarm</h2> <div class=\"slidecontainer\"> <div>Brightness</div> <input type=\"range\" min=\"0\" max=\"100\" value=\"0\" class=\"slider\" id=\"brightness\" style=\"width: 100%\" /> </div> <br /> <div class=\"column-container\"> <div class=\"column\"> <div>When</div> <input type=\"time\" id=\"time\" style=\"width: 100%; height: 2em\" required /> </div> <div class=\"\" style=\"flex-basis: 16px; flex-shrink: 0\"></div> <div class=\"column\"> <div>On for</div> <input type=\"number\" id=\"duration\" value=\"60\" placeholder=\"minutes\" style=\"width: 100%; height: 2em\" required /> </div> </div> </div> <div id=\"not-connected\"> <div id=\"disconnected-label\">Connecting to light ...</div> </div> </div> <ul id=\"logs\"></ul> <script> console.log(\"Code executed\"); if ( window.performance && window.performance.navigation.type == window.performance.navigation.TYPE_BACK_FORWARD ) { console.log(\"Returned to page\"); } const logsElement = document.querySelector(\"#logs\"); window.onload = () => { console.log(\"window onload\"); const showElement = (selector, visible) => { const element = document.querySelector(selector); const visValue = visible ? \"block\" : \"none\"; element.style.display = visValue; }; document.showInterface = (visible, messageDisconnected) => { showElement(\"#connected\", visible); showElement(\"#not-connected\", !visible); if (messageDisconnected) { const disconnectedLabel = document.querySelector( \"#disconnected-label\" ); disconnectedLabel.textContent = messageDisconnected; } }; document.showInterface(false); var oldLog = console.log; console.log = function (message) { oldLog.apply(console, arguments); const newLogElement = document.createElement(\"li\"); newLogElement.innerHTML = arguments[0]; logsElement.prepend(newLogElement); }; var oldErr = console.error; console.error = function (message) { oldErr.apply(console, arguments); const newErrElement = document.createElement(\"li\"); newErrElement.setAttribute(\"style\", \"color:red;\"); newErrElement.innerHTML = arguments[0]; logsElement.prepend(newErrElement); }; var oldThrow = this.throw; window.onerror = function (message, file, line, position, error) { const newErrElement = document.createElement(\"li\"); newErrElement.setAttribute(\"style\", \"color:red;\"); newErrElement.innerHTML = message; logsElement.prepend(newErrElement); }; }; const startWebsocket = () => { const ws = new WebSocket(\"ws://192.168.178.36:81\"); const inputBrightness = document.getElementById(\"brightness\"); const inputTimer = document.getElementById(\"time\"); const inputDuration = document.getElementById(\"duration\"); const inputTitle = document.getElementById(\"title\"); ws.onopen = function (event) { console.log(\"ws connected\"); document.showInterface(true); }.bind(this); ws.onclose = function (event) { console.log(\"ws diconnected\"); document.showInterface(false, \"Disconnected: Refresh page\"); }.bind(this); ws.onmessage = function (event) { console.log(\"received: \", event); const data = JSON.parse(event.data); receiveState(data); }; ws.onerror = function (error) { console.log(`[error] ${error.message}`); }; const sendToArduino = (jsonPayload) => { ws.send(JSON.stringify(jsonPayload)); }; const sendState = (nonPersistentData) => { let alarmTime = inputTimer.value.split(\":\"); let state = { time: { hour: alarmTime[0], minute: alarmTime[1] }, dim: inputBrightness.value, duration: inputDuration.value, ...nonPersistentData, }; ws.send(JSON.stringify(state)); }; const receiveState = (data) => { if (!data) return; if (data.time && data.time.hour && data.time.minute) { inputTimer.value = data.time.hour + \":\" + data.time.minute; } if (data.dim) { inputBrightness.value = data.dim; } if (data.duration) { inputDuration.value = data.duration; } }; inputBrightness.oninput = () => sendState(); inputTimer.oninput = () => sendState(); inputDuration.oninput = () => sendState(); inputTitle.onclick = () => { sendState({ turnOff: true }); }; }; setTimeout(() => { startWebsocket(); }, 100); </script> </body></html>");

                            // The HTTP response ends with another blank line:
                            client.println();
                            // break out of the while loop:
                            break;
                        }
                    }
                }
            }
            // close the connection:
            client.stop();
            Serial.println("Web client Disconnected.");
        }
    }
};