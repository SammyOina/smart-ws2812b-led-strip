// LED will blink when in config mode

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <WiFiUdp.h>

//neopixel set up
#include <Adafruit_NeoPixel.h>
#define LED_PIN    D6
#define LED_COUNT 150
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//for LED status
#include <Ticker.h>
Ticker ticker;

WiFiUDP Udp;

#define UDP_PORT 3000

#ifndef LED_BUILTIN
#define LED_BUILTIN 13 // ESP32 DOES NOT DEFINE LED_BUILTIN
#endif

int LED = LED_BUILTIN;
char packetBuffer[255];
int actions[28];

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
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50);
  
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
void loop() {
  uint16_t packetSize = Udp.parsePacket();
  if (packetSize) {

    Serial.print("Received packet of size ");
    Serial.print(packetSize);
    Serial.print(" from ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.println(remoteIp);

    while(Udp.available()){
      int len = Udp.read(packetBuffer, 255);
      if (len > 0) {
        packetBuffer[len] = 0;
      }
      int ipos = 0;
      char *tok = strtok(packetBuffer, ",");
      while (tok){
        if (ipos < 28) {
          actions[ipos++] = atoi(tok);
        }
       tok = strtok(NULL, ",");
      }
      //Serial.print(actions[0]);
    }
    for(int i = 0; i<21; i++) {
        strip.setPixelColor(i, strip.Color(actions[1], actions[2], actions[3]));
        strip.show();
      }
      for(int i = 21; i<43; i++) {
        strip.setPixelColor(i, strip.Color(actions[5], actions[6], actions[7]));
        strip.show();
      }
      for(int i = 43; i<65; i++) {
        strip.setPixelColor(i, strip.Color(actions[9], actions[10], actions[11]));
        strip.show();
      }
      for(int i = 65; i<87; i++) {
        strip.setPixelColor(i, strip.Color(actions[13], actions[14], actions[15]));
        strip.show();
      }
      for(int i = 87; i<108; i++) {
        strip.setPixelColor(i, strip.Color(actions[17], actions[18], actions[19]));
        strip.show();
      }
      for(int i = 108; i<129; i++) {
        strip.setPixelColor(i, strip.Color(actions[21], actions[22], actions[23]));
        strip.show();
      }
      for(int i = 129; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(actions[25], actions[26], actions[27]));
        strip.show();
      }
  }
  Udp.flush();

}
