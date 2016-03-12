#include <Wire.h>

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <WebSocketsServer.h>
#include <Hash.h>
#include <CMMCInterval.h>

#define USE_SERIAL Serial
const char *ssid = "ESPressoClamp";
const char *password = "12345678";

WebSocketsServer webSocket = WebSocketsServer(81);
CMMCInterval *myInterval2;
String string;
bool isWsConnected = false;
bool isMsgReady = false;
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch (type) {
    case WStype_DISCONNECTED:
      USE_SERIAL.printf("[%u] Disconnected!\n", num);
      isWsConnected = false;
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
        isWsConnected = true;
      }
      break;
    case WStype_TEXT:
      USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);

      // send message to client
      // webSocket.sendTXT(num, "message here");

      // send data to all connected clients
      // webSocket.broadcastTXT("message here");
      break;
    case WStype_BIN:
      USE_SERIAL.printf("[%u] get binary lenght: %u\n", num, lenght);
      hexdump(payload, lenght);

      // send message to client
      // webSocket.sendBIN(num, payload, lenght);
      break;
  }


  webSocket.sendTXT(num, string);
  isMsgReady = false;


}


#define USE_SERIAL Serial
void setup() {
  USE_SERIAL.begin(115200);
  Wire.begin();
  delay(1000);
  // USE_SERIAL.begin(921600);
  USE_SERIAL.begin(115200);

  //USE_SERIAL.setDebugOutput(true);
  USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }
  //
  //  WiFiMulti.addAP("ESPERT-002", "espertap");
  //  WiFiMulti.addAP("Nat", "12345678");
  //
  //  while (WiFiMulti.run() != WL_CONNECTED) {
  //    delay(100);
  //  }

  WiFi.softAP(ssid, password);


  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  myInterval2 = new CMMCInterval(500, [&] {

    Wire.requestFrom(10, 8);
    int c1 = Wire.read();
    int c2 = Wire.read();
    int c3 = Wire.read() << 8 | Wire.read();
    float data = c3 / 1000.00;
    int c5 = Wire.read();
    int c6 = Wire.read();
    int c7 = Wire.read();
    int c8 = Wire.read();


    string = String("");
    if (c7 == 1) {
      string = "AMP,";
      if (c5 == 0) {
        string += String(data) + ",";
      } else {
        string += String(-data) + ",";
      }
      string += "A";
    } else if (c7 == 3) {
      string = "VAC,";
      if (c5 == 0) {
        string += String(data) + ",";
      } else {
        string += String(-data) + ",";
      }
      string += "A";
      if (c8 == 1) {
        string += "V";
      } else {
        string += "mV";
      }
    } else if (c7 == 4) {
      string = "VDC,";
      if (c5 == 0) {
        string += String(data) + ",";
      } else {
        string += String(-data) + ",";
      }
      string += "A";
      if (c8 == 1) {
        string += "V";
      } else {
        string += "mV";
      }
    } else if (c7 == 6) {
      string = "TEMP,";
      if (c5 == 0) {
        string += String(data) + ",";
      } else {
        string += String(-data) + ",";
      }
      string += "C";
    } else {
      USE_SERIAL.print("BUG");
      USE_SERIAL.print(",");
    }

    isMsgReady = true;

    USE_SERIAL.println(string);
    webSocket.broadcastTXT(string);

  });
}

void loop() {
  myInterval2->Update();
  webSocket.loop();

}
