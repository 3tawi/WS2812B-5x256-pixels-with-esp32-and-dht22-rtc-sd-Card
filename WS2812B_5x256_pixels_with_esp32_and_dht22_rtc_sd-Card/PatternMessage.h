/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 */

#ifndef PatternMessage_H
#define PatternMessage_H


class PatternMessage1 : public Drawable {
private:

public:
    PatternMessage1() {
        name = (char *)"Message1";
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
    unsigned int drawFrame() {
        drawMesg(drawmsg);
        FastLED.show();
      return 0;
    }
};

class PatternMessage2 : public Drawable {
private:

public:
    PatternMessage2() {
        name = (char *)"Message2";
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
    unsigned int drawFrame() {
      drawMesg(drawmsg);
      FastLED.show();
      EVERY_N_SECONDS(5){
        effects.FillPlasma();
      }
      return 0;
    }
};

class PatternMessage3 : public Drawable {
private:

public:
    PatternMessage3() {
        name = (char *)"Message3";
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
    unsigned int drawFrame() {
      drawMesg(drawmsg);
      FastLED.show();
      if (millis() - lastTime > sp1) {
        effects.FillPlasma();
        lastTime = millis();
      }
      return 0;
    }
};

#endif
