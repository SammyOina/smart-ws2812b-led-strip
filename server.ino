// LED will blink when in config mode

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <WiFiUdp.h>

//for LED status
#include <Ticker.h>
Ticker ticker;

WiFiUDP Udp;

#define UDP_PORT 3000

#ifndef LED_BUILTIN
#define LED_BUILTIN 13 // ESP32 DOES NOT DEFINE LED_BUILTIN
#endif

int LED = LED_BUILTIN;

void tick()
{
  //toggle state
  digitalWrite(LED, !digitalRead(LED));     // set pin to the opposite state
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}

void setup() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  //set led pin as output
  pinMode(LED, OUTPUT);
  // start ticker with 0.5 because we start in AP mode and try to connect
  ticker.attach(0.6, tick);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;
  //reset settings - for testing
  // wm.resetSettings();

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wm.setAPCallback(configModeCallback);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wm.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  ticker.detach();
  //keep LED on
  digitalWrite(LED, LOW);

  Udp.begin(UDP_PORT);
}
uint8_t a2iRes=0;
void a2i(char c) {
  a2iRes*=10;
  a2iRes+=(uint8_t) c - '0';
}
uint16_t c2iRes=0;
void c2i(char c) {
  c2iRes*=255;
  c2iRes+=(uint8_t) c - 1;
}
void loop() {
  char c;
  uint8_t state=0;
  uint16_t packetSize = Udp.parsePacket();
  uint16_t led=0;
  uint8_t colors[3]={0,0,0};
  if (packetSize) {

    Serial.print("Received packet of size ");
    Serial.print(packetSize);
    Serial.print(" from ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.println(remoteIp);
  }
  Udp.flush();

}