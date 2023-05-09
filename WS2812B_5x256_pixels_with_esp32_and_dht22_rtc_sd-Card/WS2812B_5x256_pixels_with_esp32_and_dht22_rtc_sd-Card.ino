/*
// - Lien vidéo: https://youtu.be/t8EIbCcjmhI
//   REQUIRES the following Arduino libraries:
// - FastLED Library: https://github.com/pixelmatix/FastLED
// - DS1307ESP Library: https://github.com/3tawi/DS1307ESPTime
// - DHT11 DHT22 Library: https://github.com/3tawi/EspDHT
// - Getting Started ESP32 : https://www.youtube.com/watch?v=9b0Txt-yF7E
// - Find All "Great Projects" Videos : https://www.youtube.com/c/GreatProjects
*/

#include <WiFi.h>
#include <WebServer.h>
#include <FastLED.h>
#include <DS1307ESPTime.h>
#include "EspDHT.h"
#include "SPIFFS.h"
#include <SD.h>
#include "index.h"
#include "GPFont.h"
#include "Plasma.h"

WebServer server(80);
DS1307ESPTime rtc;
EspDHT dht;

const char* Apssid = "wifi-AP";
const char* Appassword = "GP-3tawi";
String  Ssid, Pass;
IPAddress ip(192,168,1,31);
IPAddress gateway(192, 168,1,1);
IPAddress gatewayap(192,168,4,1);
IPAddress subnet(255, 255, 255, 0);

#include "Effects.h"
Effects effects;
#include "Drawable.h"
#include "Playlist.h"
#include "Patterns.h"
Patterns patterns;

void(* resetFunc) (void) = 0;//declare reset function at address 0

void getWifi() { 
  char user[30], pasw[30];
  int lengthSsid = Ssid.length() + 1;
  int lengthPass = Pass.length() + 1;
  Ssid.toCharArray(user, lengthSsid);
  Pass.toCharArray(pasw, lengthPass);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(user, pasw);
  int xc = 0;
  while (WiFi.status() != WL_CONNECTED && xc < 10) {
    delay(1000);
    xc++;
  }
  if (WiFi.status() == WL_CONNECTED) { 
    drawmsg = "CONNECTED To: ";
    drawmsg += Ssid;
    drawmsg += " IP Address: http://";
    drawmsg += WiFi.localIP().toString().c_str();
    text_length = -(drawmsg.length() * 8);
    xmsg = (sp0 < 0) ? text_length : 110;
    wifiok = true;
    wifiapok = false;
  } else { 
    effects.getAPWifi();
    wifiok = false;
  }
}
void getSttWifi(){
  effects.readSttWifi();
  WiFi.mode(WIFI_AP_STA);
  getWifi();
  server.on("/", handleRoot);
  server.on("/mymessg", handleMesg);
  server.on("/mydtime", handleMyTime);
  server.on("/localdatime", handlelocaltime);
  server.on("/mytimezon", handlezoneTime);
  server.on("/mySpeed0", handleSpe0);
  server.on("/mySpeed1", handleSpe1);
  server.on("/mySpeed2", handleSpe2);
  server.on("/mySpeed3", handleSpe3);
  server.on("/mode24h", handleMode24h);
  server.on("/modeauto", handleModeAuto);
  server.on("/modepre", handleModePre);
  server.on("/modenex", handleModeNex);
  server.on("/readstat", handlestate);
  server.on("/sendate", handlesendate);
  server.on("/readdata", handledata);
  server.on("/readtext", handletxt);
  server.on("/Mywifi", handleWifi);
  server.on("/modesave", handleSave);
  server.on("/restesp", resetFunc);
  server.begin();
}

void setup() {
  Serial.begin(115200);
  Serial.println("resetting!");
  delay(3000);
  FastLED.addLeds<WS2812B,PIN_LEDS,RGB>(leds,NUM_LEDS);
  FastLED.setBrightness(160);
  effects.SetupPalette();
  rtc.DSbegin();
  effects.updateTime();
  delay(3000);
  SPIFFS.begin();
  SD.begin(5);
  getSttWifi();
  effects.prepacard();
  effects.readSttspeed();
  dht.setup(12, EspDHT::DHT22); // data pin 12
  effects.readTempHumi();
  patterns.setPattern(Ci);
  patterns.start();
}


void loop() {
  server.handleClient(); 
  patterns.drawFrame();
    if (ModeAuto  &&  (millis() - lastTim > (sp3*1000))) { 
      ModeNex();
      lastTim = millis();
    }
  EVERY_N_SECONDS(60){
    effects.readTempHumi();
  }
}

void ModeNex() { 
  patterns.move(1);
  Ci = patterns.getCurrentIndex();
}
void ModePre() {
  patterns.move(-1);
  Ci = patterns.getCurrentIndex();
}

void handleRoot() {
  server.send(200, "text/html", MAIN_page); //Send web page
}
void handleSave() {
    handleRoot();
    xmsg = -1, msg = 0, drawmsg  = "";
    effects.SaveStting();
}
void handleMesg() { 
    handleRoot();
    Message = server.arg("Message");
    xmsg = -1, msg = 0, drawmsg  = "";
}
void handleMyTime() { 
    handleRoot();
    int hh = server.arg("htmie").toInt();
    int mm = server.arg("mtmie").toInt();
    int ss = server.arg("stmie").toInt();
    int dd = server.arg("ddate").toInt();
    int mo = server.arg("mdate").toInt();
    int yr = server.arg("ydate").toInt();
    rtc.setTime(ss, mm, hh, dd, mo, yr);
    xmsg = -1, msg = 2, drawmsg  = "";
    effects.writeTime();
}
void handlelocaltime() { 
    handleRoot();
    rtc.setTime(server.arg("locadt").toInt());
    xmsg = -1, msg = 2, drawmsg  = "";
    effects.writeTime();
}
void handlezoneTime(){ 
    handleRoot();
    Tz = server.arg("locatz").toInt(); 
    configTime(Tz * 3600, 0, "time.nist.gov", "pool.ntp.org");
    xmsg = -1, msg = 2, drawmsg  = "";
}
void handleSpe0() {
    handleRoot();
    sp0 = server.arg("Speed0").toInt();
}
void handleSpe1() {
    handleRoot();
    sp1 = server.arg("Speed1").toInt();
}
void handleSpe2() {
    handleRoot();
    sp2 = server.arg("Speed2").toInt();
}
void handleSpe3() {
    handleRoot();
    sp3 = server.arg("Speed3").toInt();
}
void handleMode24h() {
    handleRoot(); 
    Mode24h = !Mode24h;
    xmsg = -1, msg = 2, drawmsg  = "";
}
void handleModeAuto() {
    handleRoot();
    ModeAuto = !ModeAuto;
}
void handleModeNex() { 
  handleRoot();
  ModeNex();
}
void handleModePre() { 
  handleRoot();
  ModePre();
}
void handlesendate() { 
    handleRoot();
    datetxt = server.arg("bday");
    datee = "/Temhumi/";
    datee += datetxt;
    datee += ".txt";
}
void setdatetxt() {
    datetxt = rtc.getTime("%F");
    datee = "/Temhumi/";
    datee += datetxt;
    datee += ".txt";
}
void handletxt() {
  if (datetxt == "") { setdatetxt(); }
  File fdate = SD.open(datee);
  if (fdate) {
    size_t sent = server.streamFile(fdate, "text/plain");
  } else {
    server.send(200, "text/plain", "file not fond");
  }
    fdate.close();
}
void handleWifi() { 
  handleRoot(); 
  Ssid = server.arg("usname");
  Pass = server.arg("pssw");
  if (wifiapok) WiFi.softAPdisconnect(true);
  getWifi();
  effects.Savewifi();
}
void handledata() {
  String content = "<?xml version = \"1.0\" ?>";
  content += "<inputs><data>";
  content += datetxt;
  content += "</data></inputs>";
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/xml", content); //Send web page
}
void handlestate() {
  String content = "<?xml version = \"1.0\" ?>";
  content += "<inputs><analog>";
  content += NewTemp;
  content += "</analog><analog>";
  content += NewHumi;
  content += "</analog><analog>";
  content += Tz;
  content += "</analog><analog>";
  content += Mode24h;
  content += "</analog><analog>";
  content += sp0;
  content += "</analog><analog>";
  content += sp1;
  content += "</analog><analog>";
  content += sp2;
  content += "</analog><analog>";
  content += sp3;
  content += "</analog><analog>";
  content += ModeAuto;
  content += "</analog><analog>";
  content += (Ci+1);
  content += "</analog></inputs>";
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/xml", content); //Send web page
}
