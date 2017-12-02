#include "videoplayer.h"

void play(int state, void* videoPlayer) {
    VideoPlayer *vp = (VideoPlayer*)videoPlayer;
    vp->setPaused(false);
}

void pause(int state, void* videoPlayer) {
    VideoPlayer *vp = (VideoPlayer*)videoPlayer;
    vp->setPaused(true);
}

void nextFrame(int state, void* videoPlayer) {
    VideoPlayer *vp = (VideoPlayer*)videoPlayer;
    vp->setCurrentFrame(vp->getCurrentFrame()+1);
}

void prevFrame(int state, void* videoPlayer) {
    VideoPlayer *vp = (VideoPlayer*)videoPlayer;
    vp->setCurrentFrame(vp->getCurrentFrame()-1);
}

VideoPlayer::VideoPlayer(string file, bool isLazy, DIPalgorithm algorithm) {
    this->vid.open(file);

    if (!vid.isOpened()) return;

    cout << "Video opened." << endl;

    Mat tempFrame;

    this->file = file;
    this->algorithm = algorithm;
    this->isLazy = isLazy;
    this->paused = false;
    this->currentFrame = 0;
    this->length = int(vid.get(CAP_PROP_FRAME_COUNT));

    namedWindow(file, WINDOW_KEEPRATIO);
    cout << "Window created." << endl;

    cout << "Storing frames..." << endl;
    if (!isLazy) {
        for (int i = 0; i < this->length; ++i) {
            vid >> tempFrame;
            imshow(file, tempFrame);
            this->frames.push_back(tempFrame);
        }
    }
    else {
        for (int i = 0; i < (int)(this->length*0.3); ++i) {
            vid >> tempFrame;
            this->frames.push_back(tempFrame);
        }
    }
    cout << "Frames stored." << endl;

    createTrackbar("Frames", file, &this->currentFrame, this->length, 0, 0);

    cout << "Trackbar created." << endl;

    createButton("Play", play);
    createButton("Pause", pause);

    cout << "Buttons created." << endl;
}

VideoPlayer::~VideoPlayer() {
    if (this->vid.isOpened())
        this->vid.release();
}

void VideoPlayer::run() {
    while (1) {
        imshow(this->file, this->frames.at(this->currentFrame));

        if (waitKey(1) == 'q') break;

        cout << "Run frame " << this->currentFrame << "." << endl;

        if (this->paused) continue;

        //if (this->algorithm != NULL)
        //    this->algorithm(this->frames.at(this->currentFrame));

        nextFrame(0, this);
    }
}

bool VideoPlayer::isPaused() const {
    return this->paused;
}

void VideoPlayer::setPaused(bool value) {
    this->paused = value;
}

int VideoPlayer::getCurrentFrame() const {
    return this->currentFrame;
}

void VideoPlayer::setCurrentFrame(int frame) {
    if (frame < 0 || frame >= this->length) return;

    this->currentFrame = frame;
}
