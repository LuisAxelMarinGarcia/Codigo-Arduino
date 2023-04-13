#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Pushbutton.h>

#define Button 34
#define led 2
#define led_blink 22
#define led_alarm 21
#define Relay 23

 /*
Los pines declarados en el diagrama de thinkercad son únicamente como demostración de que deben de funcionar como salidas y/o 
entradas de pines digitales.
Originalmente iran a pines de una placa ESP-32 y su cableado está especificado en el código, al inicio de este
*/

const char *ssid = "YourSSIDHere";
const char *password = "YourPSKHere";

WebServer server(80);

const int cont = 0;
unsigned long previousMillis = 0;
const long interval = 1000;
const long FillTime = 10000;
int ledState = LOW;
Pushbutton button(Button);

void handleRoot() {

 if (button.getSingleDebouncedRelease()) {
    cont++;
    digitalWrite(Relay, 1);
    digitalWrite(led_alarm, 1);
    digitalWrite(led_blink, 0);
    unsigned long inicio = millis();
    unsigned long tiempo_transcurrido = 0;
    while (tiempo_transcurrido < FillTime) {
      tiempo_transcurrido = millis() - inicio;
      Serial.println("while");
    }
    digitalWrite(Relay, 0);
    digitalWrite(led_alarm, 0);
    digitalWrite(led_blink, 1);
  } else {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      if (ledState == LOW) {
        ledState = HIGH;  // turn the LED on
      } else {
        ledState = LOW;  // turn the LED off
      }
      digitalWrite(led_blink, ledState);
      digitalWrite(led_alarm, ledState);
    }
  }
      digitalWrite(led, 1);
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 400,

           "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP32 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP32!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <img src=\"/test.svg\" />\
    <h2>Contador de chebe</h2>\
    <p>Cuenta: %02d</p>\
  </body>\
</html>",

           hr, min % 60, sec % 60,cont
          );
  server.send(200, "text/html", temp);
  digitalWrite(led, 0);

}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
    }
  
  
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  pinMode(led_blink, OUTPUT);
  pinMode(led_alarm, OUTPUT);
  pinMode(Relay, OUTPUT);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/test.svg", drawGraph);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}

void drawGraph() {

}

