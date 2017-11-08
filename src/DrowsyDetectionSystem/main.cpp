#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

uchar max3(uchar b, uchar g, uchar r) {
    uchar ret = b >= g ? b : g;

    return ret >= r ? ret : r;
}

uchar min3(uchar b, uchar g, uchar r) {
    uchar ret = b <= g ? b : g;

    return ret <= r ? ret : r;
}

int main(int argc, char *argv[])
{
    namedWindow("img", WINDOW_KEEPRATIO);
    namedWindow("BGR", WINDOW_KEEPRATIO);
    namedWindow("YCrCb", WINDOW_KEEPRATIO);
    namedWindow("HSV", WINDOW_KEEPRATIO);

    Mat img, temp, img2, img3, img4;
    vector<Mat> bgr, ycrcb, hsv;
    VideoCapture vid;
    vid.open(0);
    if (!vid.isOpened()) return 1;

    bool firstCondition, secondCondition, thirdCondition;

    for(;;)
    {
        vid >> img;

        split(img, bgr);
        img2 = Mat::zeros(img.rows, img.cols, CV_8UC1);

        cvtColor(img, temp, CV_BGR2YCrCb);
        split(temp, ycrcb);
        img3 = Mat::zeros(img.rows, img.cols, CV_8UC1);

        cvtColor(img, temp, CV_BGR2HSV_FULL);
        split(temp, hsv);
        img4 = Mat::zeros(img.rows, img.cols, CV_8UC1);

        for (int i = 0; i < img.rows; ++i) {
            for (int j = 0; j < img.cols; ++j) {
                //BGR
                firstCondition = (bgr[2].at<uchar>(i, j) > 95) && (bgr[1].at<uchar>(i, j) > 40) && (bgr[1].at<uchar>(i, j) > 20)
                                 && (max3(bgr[0].at<uchar>(i, j), bgr[1].at<uchar>(i, j), bgr[2].at<uchar>(i, j)) - min3(bgr[0].at<uchar>(i, j), bgr[1].at<uchar>(i, j), bgr[2].at<uchar>(i, j)))
                                 && (abs(bgr[2].at<uchar>(i, j) - bgr[1].at<uchar>(i, j)) > 15)
                                 && (bgr[2].at<uchar>(i, j) > bgr[1].at<uchar>(i, j)) && (bgr[2].at<uchar>(i, j) > bgr[0].at<uchar>(i, j))
                                 //&& bgr[2].at<uchar>(i, j) > 220) && (bgr[1].at<uchar>(i, j) > 210) && (bgr[0].at<uchar>(i, j) > 170)
                                 ;//&& (bgr[2].at<uchar>(i, j) > bgr[0].at<uchar>(i, j)) && (bgr[1].at<uchar>(i, j) > bgr[0].at<uchar>(i, j));
                img2.at<uchar>(i, j) = firstCondition * 255;
                //YCrCb
                secondCondition = ;
                img3.at<uchar>(i, j) = secondCondition * 255;
                //HSV
                thirdCondition = ;
                img4.at<uchar>(i, j) = thirdCondition * 255;

            }
        }

        imshow("img", img);
        imshow("BGR", img2);
        imshow("YCrCb", img3);
        imshow("HSV", img4);

        if (waitKey(5) == 'q') break;
    }

    return 0;
}
