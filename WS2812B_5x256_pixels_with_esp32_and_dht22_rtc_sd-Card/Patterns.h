/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 */

#ifndef Patterns_H
#define Patterns_H


#include "PatternClock1.h"
#include "PatternClock2.h"
#include "PatternClock3.h"
#include "PatternClock4.h"
#include "PatternClock5.h"
#include "PatternClock6.h"
#include "PatternMessage.h"
#include "Patternplasma.h"

class Patterns : public Playlist {
  private:
    PatternClock1 clock1;
    PatternClock2 clock2;
    PatternClock3 clock3;
    PatternClock4 clock4;
    PatternClock5 clock5;
    PatternClock6 clock6;
    PatternMessage1 message1;
    PatternMessage2 message2;
    PatternMessage3 message3;
    Patternplasma plazma;

    Drawable* currentItem;


    static const int PATTERN_COUNT = 10;

    Drawable* shuffledItems[PATTERN_COUNT];

    Drawable* items[PATTERN_COUNT] = {
      &clock1,
      &clock2,
      &clock3,
      &clock4,
      &clock5,
      &clock6,
      &message1,
      &message2,
      &message3,
      &plazma
    };

  
  public:
    int currentIndex = 0;
    int getCurrentIndex() {
      return currentIndex;
    }
    Patterns() {
      // add the items to the shuffledItems array
      for (int a = 0; a < PATTERN_COUNT; a++) {
        shuffledItems[a] = items[a];
      }
      shuffleItems();
      this->currentItem = items[0];
      this->currentItem->start();
    }

    char* Drawable::name = (char *)"Patterns";

    void stop() {
      if (currentItem)
        currentItem->stop();
    }

    void start() {
      if (currentItem)
        currentItem->start();
    }

    void move(int step) {
      currentIndex += step;

      if (currentIndex >= PATTERN_COUNT) currentIndex = 0;
      else if (currentIndex < 0) currentIndex = PATTERN_COUNT - 1;


      moveTo(currentIndex);

      //if (!isTimeAvailable && currentItem == &analogClock)
     //   move(step);
    }

    void moveRandom(int step) {
      currentIndex += step;

      if (currentIndex >= PATTERN_COUNT) currentIndex = 0;
      else if (currentIndex < 0) currentIndex = PATTERN_COUNT - 1;


      if (currentItem)
        currentItem->stop();

      currentItem = shuffledItems[currentIndex];

      if (currentItem)
        currentItem->start();
    }

    void shuffleItems() {
      for (int a = 0; a < PATTERN_COUNT; a++)
      {
        int r = random(a, PATTERN_COUNT);
        Drawable* temp = shuffledItems[a];
        shuffledItems[a] = shuffledItems[r];
        shuffledItems[r] = temp;
      }
    }


    unsigned int drawFrame() {
      return currentItem->drawFrame();
    }

    void listPatterns() {
      Serial.println(F("{"));
      Serial.print(F("  \"count\": "));
      Serial.print(PATTERN_COUNT);
      Serial.println(",");
      Serial.println(F("  \"results\": ["));

      for (int i = 0; i < PATTERN_COUNT; i++) {
        Serial.print(F("    \""));
        Serial.print(i, DEC);
        Serial.print(F(": "));
        Serial.print(items[i]->name);
        if (i == PATTERN_COUNT - 1)
          Serial.println(F("\""));
        else
          Serial.println(F("\","));
      }

      Serial.println("  ]");
      Serial.println("}");
    }

    char * getCurrentPatternName()
    {
      return currentItem->name;      
    }

    void moveTo(int index) {
      if (currentItem)
        currentItem->stop();
      currentIndex = index;
      currentItem = items[currentIndex];
      if (currentItem)
        currentItem->start();
    }    

    bool setPattern(String name) {
      for (int i = 0; i < PATTERN_COUNT; i++) {
        if (name.compareTo(items[i]->name) == 0) {
          moveTo(i);
          return true;
        }
      }

      return false;
    }


    bool setPattern(int index) {
      if (index >= PATTERN_COUNT || index < 0)
        return false;

      moveTo(index);

      return true;
    }
};

#endif
