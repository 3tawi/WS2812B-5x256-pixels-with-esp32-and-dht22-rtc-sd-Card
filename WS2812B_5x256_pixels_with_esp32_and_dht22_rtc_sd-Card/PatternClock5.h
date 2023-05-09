/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 */

#ifndef PatternClock5_H
#define PatternClock5_H


class PatternClock5 : public Drawable {
private:

public:
    PatternClock5() {
        name = (char *)"Clock5";
    }
    
  void drawMesg(String textmsg) {
    memset(leds, 0, 912 * sizeof(CRGB));
    effects.setFont(GPFont8x8); 
    effects.setCursor(xmsg, 0);
    effects.print(textmsg, co1);
    xmsg -= sp0;
    if (xmsg < text_length){
      effects.setMesg();
      xmsg = 114;
    }
    else if (xmsg > 114) {
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
  void drawHMSTime() {
    sec = rtc.getTime("%S");
    memset((leds+912), 0, 368 * sizeof(CRGB));
    effects.setFont(GPFont8x8);
    effects.setCursor(116, 0);
    effects.print(hr, co1);
    effects.setCursor(135, 0);
    effects.print(mn,256-co1);
    effects.setCursor(129, 0);
    effects.print(dot);
    effects.setFont(GPFont8x5);
    String texts = (String)sec[0];
    effects.setCursor(148, 2);
    effects.print(texts, co2); 
    texts = (String)sec[1];
    effects.setCursor(153, 2);
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
