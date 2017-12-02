#include "facedetector.h"
#include "mouthdetector.h"
#include "yawningdetector.h"
#include "videoplayer.h"

/*
int oldCode(void) {
    namedWindow("img", WINDOW_KEEPRATIO);
    namedWindow("BGR", WINDOW_KEEPRATIO);
    namedWindow("YCrCb", WINDOW_KEEPRATIO);
    namedWindow("HSV", WINDOW_KEEPRATIO);
    namedWindow("Total", WINDOW_KEEPRATIO);
    namedWindow("Cr2", WINDOW_KEEPRATIO);
    namedWindow("Cr/Cb", WINDOW_KEEPRATIO);
    namedWindow("MouthMap", WINDOW_KEEPRATIO);

    Mat img, temp, img2, img3, img4, img5, img6, img7, img8, kernel1;
    vector<Mat> bgr, ycrcb, hsv;
    VideoCapture vid;
    bool firstCondition, secondCondition, thirdCondition;
    int threshold1 = 4, threshold2 = 15;

    vid.open(0);
    if (!vid.isOpened()) return 1;

    createTrackbar("threshold1", "Total", &threshold1, 255, 0, 0);
    createTrackbar("threshold2", "Total", &threshold2, 255, 0, 0);

    for(;;)
    {
        //vid >> img;
        img = imread("yawning2.png", IMREAD_COLOR);

        split(img, bgr);
        img2 = Mat::zeros(img.rows, img.cols, CV_8UC1);

        cvtColor(img, temp, CV_BGR2YCrCb);
        split(temp, ycrcb);
        img3 = Mat::zeros(img.rows, img.cols, CV_32FC1);

        cvtColor(img, temp, CV_BGR2HSV);
        split(temp, hsv);
        img4 = Mat::zeros(img.rows, img.cols, CV_8UC1);

        img5 = Mat::zeros(img.rows, img.cols, CV_8UC1);

        img6 = Mat::zeros(img.rows, img.cols, CV_8UC1);
        img7 = Mat::zeros(img.rows, img.cols, CV_8UC1);

        img8 = Mat::zeros(img.rows, img.cols, CV_32FC1);

        double sum1 = 0, sum2 = 0;
        for (int i = 0; i < img.rows; ++i) {
            for (int j = 0; j < img.cols; ++j) {
                sum1 += ycrcb[1].at<uchar>(i, j) * ycrcb[1].at<uchar>(i, j);
                sum2 += ycrcb[1].at<uchar>(i, j)/ycrcb[2].at<uchar>(i, j);
            }
        }

        double n_inv = 1/(img.rows*img.cols);
        double ni = 0.95 * (n_inv * sum1)/(n_inv * sum2);
        double cr2, cr_cb;

        for (int i = 0; i < img.rows; ++i) {
            for (int j = 0; j < img.cols; ++j) {
                //BGR
                firstCondition = ((bgr[2].at<uchar>(i, j) > 95) && (bgr[1].at<uchar>(i, j) > 40) && (bgr[1].at<uchar>(i, j) > 20)
                                 && (max3(bgr[0].at<uchar>(i, j), bgr[1].at<uchar>(i, j), bgr[2].at<uchar>(i, j)) - min3(bgr[0].at<uchar>(i, j), bgr[1].at<uchar>(i, j), bgr[2].at<uchar>(i, j)) > 15)
                                 && (abs(bgr[2].at<uchar>(i, j) - bgr[1].at<uchar>(i, j)) > 15)
                                 && (bgr[2].at<uchar>(i, j) > bgr[1].at<uchar>(i, j)) && (bgr[2].at<uchar>(i, j) > bgr[0].at<uchar>(i, j)))
                                 || ((abs(bgr[2].at<uchar>(i, j) - bgr[1].at<uchar>(i, j)) <= 15) && (bgr[2].at<uchar>(i, j) > 220) && (bgr[1].at<uchar>(i, j) > 210) && (bgr[0].at<uchar>(i, j) > 170)
                                 && (bgr[2].at<uchar>(i, j) > bgr[0].at<uchar>(i, j)) && (bgr[1].at<uchar>(i, j) > bgr[0].at<uchar>(i, j)));
                img2.at<uchar>(i, j) = firstCondition * 255;
                //YCrCb
                secondCondition = (ycrcb[1].at<uchar>(i, j) <= 1.5862 * ycrcb[2].at<uchar>(i, j) + 20)
                                  && (ycrcb[1].at<uchar>(i, j) >=  0.3448 * ycrcb[2].at<uchar>(i, j) +  76.2069)
                                  && (ycrcb[1].at<uchar>(i, j) >= -4.5652 * ycrcb[2].at<uchar>(i, j) + 234.5652)
                                  && (ycrcb[1].at<uchar>(i, j) <= -1.1500 * ycrcb[2].at<uchar>(i, j) + 301.7500)
                                  && (ycrcb[1].at<uchar>(i, j) <= -2.2857 * ycrcb[2].at<uchar>(i, j) + 432.8500);
                img3.at<float>(i, j) = secondCondition * 255.0;
                //HSV
                thirdCondition = (hsv[0].at<uchar>(i, j) < 25) || (hsv[0].at<uchar>(i, j) > 230);
                img4.at<uchar>(i, j) = thirdCondition * 255;
                //Total
                img5.at<uchar>(i, j) = (img2.at<uchar>(i, j) && img3.at<uchar>(i, j) && img4.at<uchar>(i, j)) * 255;
                //Mouth Map
                img6.at<uchar>(i, j) = ((ycrcb[1].at<uchar>(i, j)*ycrcb[1].at<uchar>(i, j))/65025.0)*255;
                img7.at<uchar>(i, j) = ((float)ycrcb[1].at<uchar>(i, j)/ycrcb[2].at<uchar>(i, j)/3)*255;
                img8.at<float>(i, j) = ( img6.at<uchar>(i, j) * (img6.at<uchar>(i, j) - img7.at<uchar>(i, j)) * (img6.at<uchar>(i, j) - img7.at<uchar>(i, j)) );
            }
        }

        kernel1 = getStructuringElement(MORPH_ELLIPSE, Size( threshold1+1, threshold1+1));
        morphologyEx(img5, img5, MORPH_OPEN, kernel1);
        kernel1 = getStructuringElement(MORPH_ELLIPSE, Size( threshold2+1, threshold2+1));
        morphologyEx(img5, img5, MORPH_CLOSE, kernel1);

        imshow("img", img);
        imshow("BGR", img2);
        imshow("YCrCb", img3);
        imshow("HSV", img4);
        imshow("Total", img5);
        imshow("Cr2", img6);
        imshow("Cr/Cb", img7);
        imshow("MouthMap", img8);

        if (waitKey(5) == 'q') break;
    }

    return 0;
}
//*/

time_t startTime, endTime;
string yawningPerSecond;
Rect *face;
int secondCounter;

FaceDetector faceDetector("haarcascade_frontalface_alt.xml");
MouthDetector mouthDetector("haarcascade_mcs_mouth.xml");
YawningDetector yawningDetector;

void drowsyDetection(Mat &img) {
    time(&startTime);

    faceDetector.detectFace(img, Scalar(255, 0, 0));
    face = faceDetector.getFace();

    mouthDetector.detectMouth(img, face, Scalar(0, 0, 255));

    yawningDetector.detectYawn(img, face, Scalar(0, 255, 0));

    putText(img, yawningPerSecond, Point(img.cols - 200, 50), FONT_HERSHEY_PLAIN, 1.5, Scalar(255, 255, 0), 2, 8);

    time(&endTime);

    secondCounter += difftime(endTime, startTime);

    if (secondCounter >= 1) {
        secondCounter = 0;
        yawningPerSecond = "YAWNING/s = " + to_string(yawningDetector.getYawnCounter());
        yawningDetector.clearYawnCounter();
    }
}

int main(int argc, char *argv[])
{
    VideoPlayer *player = new VideoPlayer("1-FemaleNoGlasses.avi", false, drowsyDetection);

    player->run();
}
