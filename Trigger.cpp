#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
using namespace cv;
using namespace std;

int pixelValueThreshold;
int numOfPixelThreshold;
int numOfPixel;

Mat frame, gray, tIm;

int frameNumber = 0;
void save() {
    char fileName[128];
    sprintf(fileName, "output/%d.png", frameNumber);
    if(imwrite(fileName, frame)) printf("Image saved: %s\n", fileName);
    else printf("Could not be saved: %s\n", fileName);
    frameNumber ++;
}

int main() {
    VideoCapture stream1(0);   //0 is the id of video device.0 if you have only one camera.
 
    if (!stream1.isOpened()) { //check if video device has been initialised
        cout << "cannot open camera";
    }

    namedWindow("Trigger", 1);

    // add trackbar
    createTrackbar("Threshold (Pixel Value):", "Trigger", &pixelValueThreshold, 255, NULL);
    createTrackbar("Num of active pixel:",     "Trigger", NULL, 1080*900, NULL);
    createTrackbar("Threshold (num of pix):",  "Trigger", &numOfPixelThreshold, 1080*900, NULL);
 
    //unconditional loop
    while (true) {
        stream1.read(frame);
        //// convert image into gray scale
        ////cvtColor(frame, gray, CV_BGR2GRAY);
        //// pass throught threshold
        ////threshold(gray, tIm, pixelThresholdValue, 255, 0);
        inRange(frame, cv::Scalar(0, 0, 0), cv::Scalar(pixelValueThreshold, pixelValueThreshold, pixelValueThreshold), tIm);
        // count active pixels
        numOfPixel = countNonZero(tIm);
        setTrackbarPos("Num of active pixel:", "Trigger", numOfPixel);
        cout << "Number of active pixel: " << numOfPixel << endl; 
        imshow("Trigger", tIm);
        if(numOfPixel > numOfPixelThreshold) save();
        if (waitKey(30) >= 0)
            break;
    }
    return 0;
}
