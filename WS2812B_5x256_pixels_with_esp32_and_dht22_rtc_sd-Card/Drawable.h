/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 */

#ifndef Drawable_H
#define Drawable_H

class Drawable{
public:
    char* name;

    virtual bool isRunnable() {
        return false;
    }
    
    virtual bool isPlaylist() {
        return false;
    }

    // a single frame should be drawn as fast as possible, without any delay or blocking
    // return how many millisecond delay is requested before the next call to drawFrame()
    virtual unsigned int drawFrame() {
        return 0;
    };

    virtual void start() {};
    virtual void stop() {};
};

#endif
