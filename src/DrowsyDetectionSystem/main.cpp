#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>

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

        //*
        kernel1 = getStructuringElement(MORPH_ELLIPSE, Size( threshold1+1, threshold1+1));
        morphologyEx(img5, img5, MORPH_OPEN, kernel1);
        kernel1 = getStructuringElement(MORPH_ELLIPSE, Size( threshold2+1, threshold2+1));
        morphologyEx(img5, img5, MORPH_CLOSE, kernel1);
        //*/

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

int main(int argc, char *argv[])
{
    namedWindow("img", WINDOW_KEEPRATIO);

    VideoCapture vid;
    vid.open(0);
    if (!vid.isOpened()) return 1;

    Mat img;
    vector<Rect_<int> > faces;
    CascadeClassifier face_cascade;
    face_cascade.load("haarcascade_frontalface_alt.xml");

    CascadeClassifier mouth_cascade;
    mouth_cascade.load("haarcascade_mcs_mouth.xml");
    vector<Rect_<int> > mouth;

    //img = imread("yawning2.png", IMREAD_COLOR);
    for (;;) {
        vid >> img;

        /**********************FACE******************************/
        //face_cascade.detectMultiScale(img, faces, 1.15, 3, 0|CASCADE_SCALE_IMAGE, Size(30, 30));
        face_cascade.detectMultiScale(img, faces, 1.05, 8, 0|CASCADE_SCALE_IMAGE, Size(55, 55));
        Rect face = faces[0];
        rectangle(img, Point(face.x, face.y), Point(face.x+face.width, face.y+face.height), Scalar(255, 0, 0), 3, 4);

        /*********************MOUTH*******************************/
        Mat ROI = img(Rect(face.x, face.y+face.height/2, face.width, face.height/2));
        rectangle(img, Point(face.x, face.y+face.height/2), Point(face.x+face.width, face.y+face.height), Scalar(0, 255, 0), 2, 4);
        mouth_cascade.detectMultiScale(ROI, mouth, 1.20, 5, 0|CASCADE_SCALE_IMAGE, Size(30, 30));

        Rect m = mouth[0];
        rectangle(img, Point(face.x+m.x, (face.height/2)+face.y+m.y), Point(face.x+m.x+m.width, (face.height/2)+face.y+m.y+m.height), Scalar(0, 0, 255), 2, 4);

        imshow("img", img);
    //for(;;) {
        if (waitKey(5) == 'q') break;
    }
}
