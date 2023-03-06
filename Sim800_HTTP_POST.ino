const char apn[] = "web.gprs.mtnnigeria.net"; // APN (example: internet.vodafone.pt) use https://wiki.apnchanger.org
const char gprsUser[] = "";                   // GPRS User
const char gprsPass[] = "";                   // GPRS Password

// SIM card PIN (leave empty, if not defined)
const char simPIN[] = "";

// Server details
// The server variable can be just a domain name or it can have a subdomain. It depends on the service you are using
const char server[] = "leak-detector-futa.herokuapp.com"; // domain name: example.com, maker.ifttt.com, etc
const char resource[] = "/addData";                       // resource path, for example: /post-data.php
const int port = 80;                                      // server port number

// Keep this API Key value to be compatible with the PHP code provided in the project page.
// If you change the apiKeyValue value, the PHP file /post-data.php also needs to have the same key
String apiKeyValue = "tPmAT5Ab3j7F9";


#define MODEM_TX 22
#define MODEM_RX 23
#define SerialMon Serial
// Set serial for AT commands (to SIM800 module)
#define SerialAT Serial1

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800   // Modem is SIM800
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb
#include <Wire.h>
#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif
TinyGsmClient client(modem);

void setup() {
  SerialMon.begin(115200);
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(1000);
  SerialMon.println("Initializing modem...");
  String modemInfo = modem.getModemInfo();
  SerialMon.print("Modem Info: ");
  SerialMon.println(modemInfo);

  SerialMon.print("Connecting to APN: ");
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass))
  {
    SerialMon.println(" fail");
  }
  else
  {
    SerialMon.println(" OK");
  }
    SerialMon.print("Connecting to ");
    SerialMon.print(server);
    if (!client.connect(server, port))
    {
      SerialMon.println(" fail");
    }
    else
    {
      SerialMon.println(" OK");
    }
    if (modem.isGprsConnected()) {
    SerialMon.println("GPRS connected");
  }
}

void loop() {
   SerialMon.println("Performing HTTP POST request...");
    int pressure0 = 40;
      String id0 = "0";
      float flow_rate0 = 1.1;
      int index0 = 0;

      int pressure1 = 38;
      String id1 = "1";
      float flow_rate1 = 1.0;
      int index1 = 1;

      int pressure2 = 38;
      String id2 = "2";
      float flow_rate2 = 1.0;
      int index2 = 2;

      int pressure3 = 37;
      String id3 = "3";
      float flow_rate3 = 0.9;
      int index3 = 3;

      String json = "{\"data\":[{\"pressure\":" + String(pressure0) + ",\"id\":\"" + id0 + "\",\"flow_rate\":" + String(flow_rate0) + ",\"index\":" + String(index0) + "},{\"id\":\"" + id1 + "\",\"pressure\":" + String(pressure1) + ",\"flow_rate\":" + String(flow_rate1) + ",\"index\":" + String(index1) + "},{\"index\":" + String(index2) + ",\"id\":\"" + id2 + "\",\"pressure\":" + String(pressure2) + ",\"flow_rate\":" + String(flow_rate2) + "},{\"index\":" + String(index3) + ",\"id\":\"" + id3 + "\",\"flow_rate\":" + String(flow_rate3) + ",\"pressure\":" + String(pressure3) + "}],\"readable_time\":\"2023-01-12T11:16:57.420318\"}";

      client.print(String("POST ") + resource + " HTTP/1.1\r\n");
      client.print(String("Host: ") + server + "\r\n");
      client.println("Connection: close");
      client.println("Content-Type: application/json");
      client.print("Content-Length: ");
      client.println(json.length());
      client.println();
      client.print(json);

      unsigned long timeout = millis();
      while (client.connected() && millis() - timeout < 10000L)
      {
        // Print available data (HTTP response from server)
        while (client.available())
        {
          char c = client.read();
          SerialMon.print(c);
          timeout = millis();
        }
      }
      SerialMon.println();

      // Close client and disconnect
      // client.stop();
      // SerialMon.println(F("Server disconnected"));
      // modem.gprsDisconnect();
      // SerialMon.println(F("GPRS disconnected"));
    }
