#include "yawningdetector.h"

YawningDetector::YawningDetector() {
    this->clearYawnCounter();
}

void YawningDetector::detectYawn(Mat &img, const Rect *face, Scalar color) {
    if (face == nullptr) return;

    Mat gray, ROI = img(Rect(face->x, face->y+face->height/2, face->width, face->height/2));

    cvtColor(ROI, gray, CV_BGR2GRAY);
    medianBlur(gray, gray, 5);

    HoughCircles(gray, this->yawns, CV_HOUGH_GRADIENT, 1, gray.rows/4, 100, 35, 20, 40);

    for (int i = 0; i < this->yawns.size(); ++i) {
        Point center(cvRound(this->yawns[i][0])+face->x, (face->height/2)+face->y+cvRound(this->yawns[i][1]));
        circle(img, center, cvRound(this->yawns[i][2]), color, 3, 8, 0);
    }

    if (this->yawns.size() > 0)
        this->yawnCounter++;
}

int YawningDetector::getYawnCounter(void) const {
    return yawnCounter;
}

int YawningDetector::clearYawnCounter(void) {
    this->yawnCounter = 0;
}
