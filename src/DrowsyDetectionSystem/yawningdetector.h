#ifndef YAWNINGDETECTOR_H
#define YAWNINGDETECTOR_H

#include "common.h"

class YawningDetector
{
    vector<Vec3f> yawns;
    int yawnCounter;
public:
    YawningDetector();

    void detectYawn(Mat &img, const Rect *face, Scalar color);
    int getYawnCounter(void) const;
    int clearYawnCounter(void);
};

#endif // YAWNINGDETECTOR_H
