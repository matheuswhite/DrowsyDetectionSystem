#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include "common.h"

class FaceDetector
{
    vector<Rect_<int> > faces;
    CascadeClassifier faceCascade;
public:
    FaceDetector(string faceCascadeXML);

    void detectFace(Mat &img, Scalar color);
    Rect *getFace();
};

#endif // FACEDETECTOR_H
