/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 */

#ifndef Playlist_H
#define Playlist_H

class Playlist : public Drawable {
public:
    virtual bool isPlaylist() {
        return true;
    }
    
    boolean isCurrentItemFinished = true;

    virtual void move(int step) = 0;
    virtual void moveRandom(int step) = 0;
    virtual int getCurrentIndex();
};

#endif
