

#ifndef Effects_H
#define Effects_H

#define fontbyte(x) Font[x] 

// Params for width and height
const uint8_t kMatrixWidth = 160;
const uint8_t kMatrixHeight = 8;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
#define PIN_LEDS 13
CRGB leds[kMatrixWidth * kMatrixHeight];
CRGB usPalette[256];
CRGB black = CRGB::Black;

String Message = "Great Projects wish you a very nice day.";
String drawmsg;
int text_length;
String datetxt, datee;
bool Mode24h = false;
bool flasher = false;
bool wifiok = false;
bool wifiapok = false;
bool ModeAuto = true;
int Ci, Ca;
long lastTim, lastTime, prevTime;
String hr, mn, sec, dot;
float NewTemp, NewHumi;
float oldTemp, oldHumi;
int Tz = 1, sp0 = 1, sp1 = 3, sp2 = 4, sp3 = 10;
int xmsg = 160, msg = 0;
uint8_t co1, co2;

uint8_t plasma[kMatrixWidth][kMatrixHeight];

#define SWAPint(X,Y) { \
        int temp = X ; \
        X = Y ; \
        Y = temp ; \
    }
   
uint16_t XY( uint8_t x, uint8_t y)
{
  if( x >= kMatrixWidth || x < 0 || y >= kMatrixHeight || y < 0) return -1;
  uint16_t i;
  if( x & 0x01) {
      uint8_t reverseY = (kMatrixHeight - 1) - y;
      i = (x * kMatrixHeight) + reverseY;
  } else {
      i = (x * kMatrixHeight) + y;
  }
  return i;
}


class Effects {
public:

  CRGB HsvToRgb(uint8_t h, uint8_t s, uint8_t v) {
    CHSV hsv = CHSV(h, s, v);
    CRGB rgb;
    hsv2rgb_spectrum(hsv, rgb);
    return rgb;
  }
  
  void SetupPalette() {
    for (int k = 0; k < 256; k++)
      usPalette[k] = HsvToRgb(k, 255, 255);
  }
  
  void FillPlasma() {
  int          value;
  int           x1, x2, x3, x4, y1, y2, y3, y4, sx1, sx2, sx3, sx4;
  unsigned char x, y;
  sx1 = (int)(cos(angle1) * radius1 + centerx1);
  sx2 = (int)(cos(angle2) * radius2 + centerx2);
  sx3 = (int)(cos(angle3) * radius3 + centerx3);
  sx4 = (int)(cos(angle4) * radius4 + centerx4);
  y1  = (int)(sin(angle1) * radius1 + centery1);
  y2  = (int)(sin(angle2) * radius2 + centery2);
  y3  = (int)(sin(angle3) * radius3 + centery3);
  y4  = (int)(sin(angle4) * radius4 + centery4);

  for(y=0; y<kMatrixHeight; y++) {
    x1 = sx1; x2 = sx2; x3 = sx3; x4 = sx4;
    for(x=0; x<kMatrixWidth; x++) {
      value = hueShift
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x1 * x1 + y1 * y1) >> 5))
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x2 * x2 + y2 * y2) >> 5))
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x3 * x3 + y3 * y3) >> 6))
        + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x4 * x4 + y4 * y4) >> 6));
      plasma[x][y] = value/2;
      x1--; x2--; x3--; x4--;
    }
    y1--; y2--; y3--; y4--;
  }
  angle1 += 0.03;
  angle2 -= 0.07;
  angle3 += 0.13;
  angle4 -= 0.15;
  hueShift += 2;
  }

void Pixel(int x, int y) {
  if( x >= kMatrixWidth || x < 0 || y >= kMatrixHeight || y < 0) return;
  uint8_t index = plasma[x][y];
  leds[XY(x, y)] = usPalette[index];
}
  
void Pixel(int x, int y, CRGB color) {
  if( x >= kMatrixWidth || x < 0 || y >= kMatrixHeight || y < 0) return;
  leds[XY(x, y)] = color;
}
  

void fillRect(int x, int y, int w, int h, CRGB c) {
  if (x < 0) {
    w += x;
    x = 0;
    }
  if (y < 0) {
    h += y;
    y = 0;
  }
  if (x + w > kMatrixWidth)
    w = kMatrixWidth - x;
  if (y + h > kMatrixHeight)
    h = kMatrixHeight - y;
  for (int j = y; j < y + h; j++)
    for (int i = x; i < x + w; i++)
      Pixel(i, j, c);
}

void drawLine(int x0, int y0, int x1, int y1, CRGB c)
{
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2;
  for (;;) {
    Pixel(x0, y0, c);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 > dy) {
      err += dy;
      x0 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }
}
void xLine(int x0, int x1, int y, CRGB c)
  {
    if (x0 > x1)
    {
      int xb = x0;
      x0 = x1;
      x1 = xb;
    }
    if (x0 < 0)
      x0 = 0;
    if (x1 > kMatrixWidth)
      x1 = kMatrixWidth;
    for (int x = x0; x < x1; x++)
      Pixel(x, y, c);
  }

void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, CRGB c) {
  drawLine(x0, y0, x1, y1, c);
  drawLine(x1, y1, x2, y2, c);
  drawLine(x2, y2, x0, y0, c);
}

void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, CRGB c) {
  int16_t a, b, y, last;
  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    SWAPint(y0, y1);
    SWAPint(x0, x1);
  }
  if (y1 > y2) {
    SWAPint(y2, y1);
    SWAPint(x2, x1);
  }
  if (y0 > y1) {
    SWAPint(y0, y1);
    SWAPint(x0, x1);
  }

  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a)
      a = x1;
    else if (x1 > b)
      b = x1;
    if (x2 < a)
      a = x2;
    else if (x2 > b)
      b = x2;
    drawFastHLine(a, y0, b - a + 1, c);
    return;
  }
  int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
          dx12 = x2 - x1, dy12 = y2 - y1;
  int32_t sa = 0, sb = 0;
  if (y1 == y2)
    last = y1; // Include y1 scanline
  else
    last = y1 - 1; // Skip it
  for (y = y0; y <= last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    if (a > b)
      SWAPint(a, b);
    drawFastHLine(a, y, b - a + 1, c);
  }
  sa = (int32_t)dx12 * (y - y1);
  sb = (int32_t)dx02 * (y - y0);
  for (; y <= y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    if (a > b)
      SWAPint(a, b);
    drawFastHLine(a, y, b - a + 1, c);
  }
}

void drawFastHLine(int x, int y, int w, CRGB c) {
  drawLine(x, y, x + w - 1, y, c);
}
void drawFastVLine(int x, int y, int h, CRGB c) {
  drawLine(x, y, x, y + h - 1, c);
}

void fillEllipse(int x, int y, int rx, int ry, CRGB c) {
  if(ry == 0)
    return;
  float f = float(rx) / ry;
  f *= f;   
  for(int i = 0; i < ry + 1; i++) {
    float s = rx * rx - i * i * f;
    int xr = (int)sqrt(s <= 0 ? 0 : s);
    xLine(x - xr, x + xr + 1, y + i, c);
    if(i) 
      xLine(x - xr, x + xr + 1, y - i, c);
    }
  }
  
void print(String st)
{
  char buf[st.length()+1];
  st.toCharArray(buf, st.length()+1);
  print(buf);
}

void print(char *st)
{
  int x = cursor_x;
  int y = cursor_y;
  int stl = strlen(st);
  for (int cnt=0; cnt<stl; cnt++){
    printchar(*st++, x + (cnt*(x_size)), y);
  }
}

void printchar(unsigned char c, int x, int y) {
  int font_idx = ((c - offset)*((x_size*y_size/8)))+4;
  int cbyte=fontbyte(font_idx);
  int cbit=7;
  for (int cy=0; cy<y_size; cy++) {
    for (int cx=0; cx<x_size; cx++) {
      if ((cbyte & (1<<cbit)) != 0)
          Pixel(x+cx, y+cy);
      cbit--;
      if (cbit<0) {
        cbit=7;
        font_idx++;
        cbyte=fontbyte(font_idx);
      }
    }
  }
}

  
void print(String st, uint8_t ind)
{
  char buf[st.length()+1];
  st.toCharArray(buf, st.length()+1);
  print(buf, ind);
}

void print(char *st, uint8_t ind)
{
  int x = cursor_x;
  int y = cursor_y;
  int stl = strlen(st);
  for (int cnt=0; cnt<stl; cnt++){
    CRGB color = usPalette[ind];
    printchar(*st++, x + (cnt*(x_size)), y, color);
    ind += sp2;
  }
}
void printchar(unsigned char c, int x, int y, CRGB col) {
  int font_idx = ((c - offset)*((x_size*y_size/8)))+4;
  int cbyte=fontbyte(font_idx);
  int cbit=7;
  for (int cy=0; cy<y_size; cy++) {
    for (int cx=0; cx<x_size; cx++) {
      if ((cbyte & (1<<cbit)) != 0)
          Pixel(x+cx, y+cy, col);
      cbit--;
      if (cbit<0) {
        cbit=7;
        font_idx++;
        cbyte=fontbyte(font_idx);
      }
    }
  }
}

void setCursor(int x, int y) 
{
    cursor_x = x;
    cursor_y = y;
}  

void setFont(uint8_t* font_c)
{
  Font=font_c;
  x_size=fontbyte(0);
  y_size=fontbyte(1);
  offset=fontbyte(2);
  numchars=fontbyte(3);
}

void getAPWifi() { 
  WiFi.softAPConfig(ip, gatewayap, subnet);
  WiFi.softAP(Apssid, Appassword);
  drawmsg = "Local Wifi : ";
  drawmsg += Apssid;
  drawmsg += " Password: ";
  drawmsg += Appassword;
  drawmsg += " IP Address: http://";
  drawmsg += WiFi.softAPIP().toString().c_str();
  text_length = -(drawmsg.length() * 8);
  xmsg = (sp0 < 0) ? text_length : 110;
  wifiapok = true;
}

void readSttWifi() {
  File file = SPIFFS.open("/Stt-wifi.txt");
  if(file) {
    String textwifi = "";
    while (file.available()) {
      int savednum = file.read();
      textwifi += (char)savednum;
     }
    file.close();
    readsource(textwifi);
  }
}

void readSttspeed() {
  File file = SPIFFS.open("/Sttingsp.txt");
  if(file) {
    String text = "";
    while (file.available()) {
      int savednum = file.read();
      text += (char)savednum;
     }
    file.close();
    readsource(text);
  }
}

void readsource(String source) {
  String line = "";
  int k = source.length();
  for(int j=0; j<k; j++) {
    if (source[j] == '\n') {
      readLine(line);
      line = "";
      } else
    line += source[j];
  }
}
    void readLine(String Line) {
      if (Line.startsWith("speed0=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          sp0 = Line.toInt();
        }
      }
      else if (Line.startsWith("speed1=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          sp1 = Line.toInt();
        }
      }
      else if (Line.startsWith("speed2=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          sp2 = Line.toInt();
        }
      }
      else if (Line.startsWith("speed3=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          sp3 = Line.toInt();
        }
      }
      else if (Line.startsWith("mytimz=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          Tz = Line.toInt();
        }
      }
      else if (Line.startsWith("mymsge=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          Message = Line;
        }
      }
      else if (Line.startsWith("Cindex=")) {
        Line = Line.substring(7);
        if (Line.length() > 0) {
          Ci = Line.toInt();
        }
      }
      else if (Line.startsWith("mAuto=")) {
        Line = Line.substring(6);
        if (Line.length() > 0) {
          ModeAuto = Line.toInt();
        }
      }
      else if (Line.startsWith("Ssid=")) {
        Line = Line.substring(5);
        if (Line.length() > 0) {
          Ssid = Line;
        }
      }
      else if (Line.startsWith("Pass=")) {
        Line = Line.substring(5);
        if (Line.length() > 0) {
          Pass = Line;
        }
      }
  }
  
void Savewifi() {
    File file = SPIFFS.open("/Stt-wifi.txt", FILE_WRITE);
    if (file) {
      String textw =  "\nSsid=";
             textw += Ssid;
             textw += "\nPass=";
             textw += Pass;
             textw += "\n";
      file.print(textw);
      file.close();
    }
}
  
void SaveStting() {
  File file = SPIFFS.open("/Sttingsp.txt", FILE_WRITE);
  if (file) {
    String text =  "speed0=";
           text += sp0;
           text += "\nspeed1=";
           text += sp1;
           text += "\nspeed2=";
           text += sp2;
           text += "\nspeed3=";
           text += sp3;
           text += "\nmytimz=";
           text += Tz;
           text += "\nCindex=";
           text += Ci;
           text += "\nmAuto=";
           text += ModeAuto;
           text += "\nmymsge=";
           text += Message;
           text += "\n";
    file.print(text);
    file.close();
  }
}
void readTempHumi() {
  dht.readSensor();
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  if (!isnan(temperature)) NewHumi = humidity;
  if (!isnan(humidity)) NewTemp = temperature;
  if (oldTemp != NewTemp || oldHumi != NewHumi) {
    wirttemphu();
    oldTemp = NewTemp;
    oldHumi = NewHumi;
  }
}
void wirttemphu() {
    String ddate = "/Temhumi/";
    ddate += rtc.getTime("%F");
    ddate += ".txt";
    String text = rtc.getTime();
    text += " Temperature: ";
    text += NewTemp;
    text += " *C - Humidity: ";
    text += NewHumi;
    text += " %RH";
    writeFileSD(ddate, text);
}
void writeFileSD(const String path, const String mesg){
    File fileth = SD.open(path, FILE_APPEND);
     if(fileth){ fileth.println(mesg); } 
    fileth.close();
}

void prepacard() { 
    File root = SD.open("/Temhumi");
    if(!root.isDirectory()){
        SD.mkdir("/Temhumi");
    }
}

void updateTime()
{
  rtc.DSgetdatime(); 
  configTime(Tz * 3600, 0, "");
} 
  
void writeTime()
{
  configTime(0, 0, ""); 
  rtc.DSsetdatime(); 
  configTime(Tz * 3600, 0, "");
}
    
  void setMesg() {
    co1 = random(0,255);
    switch(msg) {
      case 0 :
             drawmsg = Message;
             text_length = -(drawmsg.length() * 8);
             msg++;
             break;
      case 1 :
             drawmsg = "Temperature: ";
             drawmsg += NewTemp;
             drawmsg += (char)127;
             drawmsg += " - Humidity: ";
             drawmsg += NewHumi;
             drawmsg += "%";
             text_length = -(drawmsg.length() * 8);
             msg++;
             break;
      case 2 :
             drawmsg = rtc.getTime("%A, %d %B %Y - %p %I:%M");
             text_length = -(drawmsg.length() * 8);
             msg=0;
             break;
    }
  }
  
protected:
  uint8_t* Font;
  uint8_t x_size;
  uint8_t y_size;
  uint8_t offset;
  uint8_t numchars;
  int cursor_x;     ///< x location to start print()ing text
  int cursor_y;     ///< y location to start print()ing text

};

#endif
