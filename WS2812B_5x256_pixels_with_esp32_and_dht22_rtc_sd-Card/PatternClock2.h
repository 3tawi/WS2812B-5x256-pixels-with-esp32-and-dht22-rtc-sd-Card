/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 */

#ifndef PatternClock2_H
#define PatternClock2_H


class PatternClock2 : public Drawable {
private:

public:
    PatternClock2() {
        name = (char *)"Clock2";
    }
    
  void drawMesg(String textmsg) {
    memset(leds, 0, sizeof(leds));
    effects.setFont(GPFont8x8); 
    effects.setCursor(xmsg, 0);
    effects.print(textmsg);
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
    effects.print(hr);
    effects.setCursor(19, 0);
    effects.print(mn);
    effects.setCursor(13, 0);
    effects.print(dot);
    effects.setFont(GPFont8x5);
    String texts = (String)sec[0];
    effects.setCursor(32, 2);
    effects.print(texts); 
    texts = (String)sec[1];
    effects.setCursor(37, 2);
    effects.print(texts); 
    //dimclock(240);
  }
    unsigned int drawFrame() {
      drawMesg(drawmsg);
      drawHMSTime();
      FastLED.show();
      if (millis() - lastTime > sp1) {
        effects.FillPlasma();
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
