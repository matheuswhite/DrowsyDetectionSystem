#include "facedetector.h"

FaceDetector::FaceDetector(string faceCascadeXML) {
    this->faceCascade.load(faceCascadeXML);
}

void FaceDetector::detectFace(Mat &img, Scalar color) {
    Rect face;
    this->faceCascade.detectMultiScale(img, this->faces, 1.05, 8, 0|CASCADE_SCALE_IMAGE, Size(55, 55));
    for (int i = 0; i < this->faces.size(); ++i) {
        face = faces[i];
        rectangle(img, Point(face.x, face.y), Point(face.x+face.width, face.y+face.height), color, 3, 4);
    }
}

Rect *FaceDetector::getFace() {
    return (this->faces.size() > 0) ? &(faces[0]) : nullptr;
}
