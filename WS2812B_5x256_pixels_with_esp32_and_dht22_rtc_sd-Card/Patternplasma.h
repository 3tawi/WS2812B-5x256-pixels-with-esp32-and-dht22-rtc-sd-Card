/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 */

#ifndef Patternplasma_H
#define Patternplasma_H


class Patternplasma : public Drawable {
private:

public:
    Patternplasma() {
        name = (char *)"Plasma";
    }
    
  void drawPlasma() {
    for(int j=0; j<kMatrixHeight; j++) 
      for(int i=0; i<kMatrixWidth; i++){
        uint8_t index = plasma[i][j];
        leds[XY(i, j)] = usPalette[index];
      }
  }

    unsigned int drawFrame() {
      if (millis() - lastTime > sp1) {
        effects.FillPlasma();
        drawPlasma();
        FastLED.show();
        lastTime = millis();
      }
      return 0;
    }
};

#endif
