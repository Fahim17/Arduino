#include <ESP8266WiFi.h>
#include <WiFiClient.h>


// Include the Wi-Fi library
#ifndef APSSID
#define APSSID "ESP8266_R1"
#define APPSK  "123456789"
#endif

const char *ssidMy = APSSID;
const char *password = APPSK;

//const char *ssidMy = "ESP8266_R1";//**********NEEDS TO BE CHANGED PER NODE***************
//const char *password = "123456789";
const uint16_t port = 80;
String ssid = "ESP8266_R";


WiFiServer server(80);              // launches the server

IPAddress local_ip(192, 168, 11, 5); //**********NEEDS TO BE CHANGED PER NODE***************
IPAddress gateway(192, 168, 0, 1);  // WiFi router's IP
IPAddress subnet(255, 255, 255, 0);

String IPtable[] = {"192.168.11.5",
                    "192.168.11.10",
                    "192.168.11.15",
                    "192.168.11.20",
                    "192.168.11.25"
                   };

void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssidMy, password);
  
Serial.println("Bejaalll.............");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Access Point \"");
  Serial.print(ssidMy);
  Serial.println("\" started");
  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());// Start the access point
  // Send the IP address of the ESP8266 to the computer

  server.begin();

}
int poi = 0;
void loop() {
  digitalWrite(2, LOW);
  
    for (poi = 0; poi < 2; poi++) {
      sendMsg(2, "this is msg from R1");
      delay(10000);
    }

  //manageDataClient();

}







//-****************************Function**************************************
void StartAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssidMy, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("AP mode restart:\t");
  Serial.println(WiFi.softAPIP());// Start the access point
  // Send the IP address of the ESP8266 to the computer

  server.begin();
}
//-----------------------------------------------------------------------
void sendMsg(int n, String msg) {
  WiFi.softAPdisconnect(true);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  ssid += n;
  Serial.println(ssid);
  WiFi.begin(String(ssid), password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //sending data to the connected AP.............
  WiFiClient client; Serial.println(IPtable[n - 1]);
  if (!client.connect(IPtable[n - 1], port)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }

  // This will send a string to the server
  Serial.println("sending data to server");
  if (client.connected()) {
    client.println(msg);
  }

  // wait for data to be available
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(60000);
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Serial.print(ch);
  }

  // Close the connection
  Serial.println();
  Serial.println("closing connection");
  client.stop();
  WiFi.disconnect();
  StartAP();
  blnk(3, 300);
}

//-----------------------------------------------------------------------
void manageDataClient() {
  delay(50);
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println(F("new client"));

  client.setTimeout(1000); // default is 1000

  // Read the first line of the request
  String req = client.readStringUntil('\r');

  //  String reqcode = req.substring(0, 2);
  //  req.remove(0, 2);

  //  //response according to reqcode
  //  if (reqcode.equals(protocal[0])) {// this is for message receieved
  //    Serial.println(F("00 request: "));
  //    Serial.println(req);
  //    Serial.println(client.remoteIP().toString().c_str());
  //    blnk(3);
  //  } else if (reqcode.equals(protocal[1])) {// this is for sending message
  //    Serial.println(F("01 request: "));
  //    Serial.println(req);
  //  }
  Serial.println(req);
  Serial.println(F("sending data....."));
  client.print("data receieved thanku from R2");
  blnk(5, 100);
}
//-----------------------------------------------------------------------
void blnk(int a, int t) {
  for (int x = 0; x < a; x++) {
    digitalWrite(2, HIGH);
    delay(t);
    digitalWrite(2, LOW);
    delay(t);
  }
}




