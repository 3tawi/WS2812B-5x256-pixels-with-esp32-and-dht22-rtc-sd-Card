/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 */

#ifndef PatternClock6_H
#define PatternClock6_H


class PatternClock6 : public Drawable {
private:

public:
    PatternClock6() {
        name = (char *)"Clock6";
    }
    
  void drawMesg(String textmsg) {
    memset(leds, 0, sizeof(leds));
    effects.setFont(GPFont8x8); 
    effects.setCursor(xmsg, 0);
    effects.print(textmsg, co1);
    xmsg -= sp0;
    if (xmsg < text_length){
      effects.setMesg();
      xmsg = 160;
    }
    else if (xmsg > 160) {
      effects.setMesg();
      xmsg = text_length;
    }
  }
  void setHMSTime() {
    hr = Mode24h ? rtc.getTime("%H") : rtc.getTime("%I");
    mn = rtc.getTime("%M");
    dot = flasher ? "" : ":";
    co2 = random(0,255);
  }
  void dimclock(int value) {
    for(int i=0; i<368; i++)
      leds[i+912].nscale8(value);
  }
  void drawHMSTime() {
    sec = rtc.getTime("%S");
    memset(leds, 0, 368 * sizeof(CRGB));
    effects.setFont(GPFont8x8);
    effects.setCursor(0, 0);
    effects.print(hr, co1);
    effects.setCursor(19, 0);
    effects.print(mn,256-co1);
    effects.setCursor(13, 0);
    effects.print(dot);
    effects.setFont(GPFont8x5);
    String texts = (String)sec[0];
    effects.setCursor(32, 2);
    effects.print(texts, co2); 
    texts = (String)sec[1];
    effects.setCursor(37, 2);
    effects.print(texts, co2); 
    //dimclock(240);
  }
    unsigned int drawFrame() {
      if (millis() - lastTime > sp1) {
        drawMesg(drawmsg);
        drawHMSTime();
        FastLED.show();
        lastTime = millis();
      }
      EVERY_N_SECONDS(1){
        setHMSTime();
        flasher = !flasher;
      }
      return 0;
    }
};

#endif
