#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace cv;
using namespace std;

const int width  = 640;
const int height = 480;

int pixelValueThreshold = 180;
int numOfPixelThreshold = 5000;
int numOfPixel;
int exposure = 500;

Mat frame, gray, tIm;

int frameNumber = 0;
void save() {
    char fileName[128];
    sprintf(fileName, "output/frame_%d.png", frameNumber);
    if(imwrite(fileName, frame)) printf("Image saved: %s\n", fileName);
    else printf("Could not be saved: %s\n", fileName);
    frameNumber ++;
}

int main() {
    VideoCapture stream1(0);   //0 is the id of video device.0 if you have only one camera.
    system("v4l2-ctl -c exposure_auto_priority=0");
    system("v4l2-ctl -c exposure_auto=1");

 
    if (!stream1.isOpened()) { //check if video device has been initialised
        cout << "cannot open camera";
        return 1;
    }

    namedWindow("Trigger", 1);

    // add trackbar
    createTrackbar("Threshold (Pixel Value):", "Trigger", &pixelValueThreshold, 255, NULL);
    createTrackbar("Num of active pixel:",     "Trigger", NULL, width * height, NULL);
    createTrackbar("Threshold (num of pix):",  "Trigger", &numOfPixelThreshold, width * height, NULL);
    createTrackbar("Exposure:",  "Trigger", &exposure, 50*1000, NULL);
 
    //unconditional loop
    char exposureCommand[128];
    while (true) {
        sprintf(exposureCommand, "v4l2-ctl -c exposure_absolute=%d", exposure);
        system(exposureCommand);
        //cout << stream1.get(CV_CAP_PROP_EXPOSURE) << endl;
        stream1.read(frame);
        //// convert image into gray scale
        cvtColor(frame, gray, CV_BGR2GRAY);
        //// pass throught threshold
        threshold(gray, tIm, pixelValueThreshold, 255, 0);
        //inRange(frame, cv::Scalar(0, 0, 0), cv::Scalar(pixelValueThreshold, pixelValueThreshold, pixelValueThreshold), tIm);
        // count active pixels
        numOfPixel = countNonZero(tIm);
        setTrackbarPos("Num of active pixel:", "Trigger", numOfPixel);
        cout << pixelValueThreshold << endl;
        // cout << "Number of active pixel: " << numOfPixel << endl; 
        imshow("Trigger", tIm);
        imshow("Webcam", frame);
        if(numOfPixel > numOfPixelThreshold) save();
        if (waitKey(30) >= 0)
            break;
    }
    return 0;
}
