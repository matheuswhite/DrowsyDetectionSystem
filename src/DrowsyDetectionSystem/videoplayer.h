#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include "common.h"

typedef void(*DIPalgorithm)(Mat &frame);

class VideoPlayer
{
    VideoCapture vid;
    string file;
    DIPalgorithm algorithm;
    int length;
    bool isLazy, paused;
    int currentFrame;
    vector<Mat> frames;
public:
    VideoPlayer(string file, bool isLazy, DIPalgorithm algorithm);
    ~VideoPlayer();
    void run();

    bool isPaused() const;
    void setPaused(bool value);
    int getCurrentFrame() const;
    void setCurrentFrame(int frame);
};

#endif // VIDEOPLAYER_H
