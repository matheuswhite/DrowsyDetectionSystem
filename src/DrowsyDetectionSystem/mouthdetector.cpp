#include "mouthdetector.h"

MouthDetector::MouthDetector(string mouthCascadeXML) {
    this->mouthCascade.load(mouthCascadeXML);
}

void MouthDetector::detectMouth(Mat &img, const Rect *face, Scalar color) {
    if (face == nullptr) return;

    Rect mouth;
    Mat ROI = img(Rect(face->x, face->y+face->height/2, face->width, face->height/2));
    this->mouthCascade.detectMultiScale(ROI, this->mouths, 1.20, 5, 0|CASCADE_SCALE_IMAGE, Size(30, 30));
    for (int i = 0; i < this->mouths.size(); ++i) {
        mouth = this->mouths[i];
        rectangle(img, Point(face->x+mouth.x, (face->height/2)+face->y+mouth.y), Point(face->x+mouth.x+mouth.width, (face->height/2)+face->y+mouth.y+mouth.height), color, 3, 4);
    }
}

Rect *MouthDetector::getMouth() {
    return (this->mouths.size() > 0) ? &(mouths[0]) : nullptr;
}
