#ifndef MOUTHDETECTOR_H
#define MOUTHDETECTOR_H

#include "common.h"

class MouthDetector
{
    vector<Rect_<int> > mouths;
    CascadeClassifier mouthCascade;
public:
    MouthDetector(string mouthCascadeXML);

    void detectMouth(Mat &img, const Rect *face, Scalar color);
    Rect *getMouth();
};

#endif // MOUTHDETECTOR_H
