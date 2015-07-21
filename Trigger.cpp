#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
using namespace cv;
using namespace std;

const int width  = 640*2;
const int height = 480*2;

int pixelValueThreshold = 150;
int numOfPixelThreshold = 50;
int numOfPixel;
int exposure = 10000;

Mat frame, gray, tIm, frame2, tIm2;

int frameNumber = 0;
void save() {
    char fileName[128];
    sprintf(fileName, "output/frame_%d.tiff", frameNumber);
    if(imwrite(fileName, frame)) printf("Image saved: %s\n", fileName);
    else printf("Could not be saved: %s\n", fileName);
    frameNumber ++;
}

double getMS(struct timeval tv){
    return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}

int main() {
    // old time stamp for time counter
    struct timeval tv;
    double old;
  //camera settings  
    system("v4l2-ctl -c exposure_auto_priority=0");
    system("v4l2-ctl -c exposure_auto=1");
/*
    system("v4l2-ctl -c backlight_compensation=0");
    system("v4l2-ctl -c power_line_frequency=0");
    system("v4l2-ctl -c gain=255");
    system("v4l2-ctl -c contrast=255");
    system("v4l2-ctl -c brightness=242");
    system("v4l2-ctl -c saturation=32");
    system("v4l2-ctl --set-fmt-video=width=1280,height=960,pixelformat=0"); 
    system("v4l2-ctl -c white_balance_temprature_auto=0");
    system("v4l2-ctl -c --set-parm=1");
*/

    // cout<<system("v4l2-ctl -d /dev/video0 --list-formats")<<endl;    
    
    VideoCapture stream1 (0);   //0 is the id of video device.0 if you have only one camera.
    
    stream1.set(CV_CAP_PROP_FRAME_WIDTH, width);
    stream1.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    
    namedWindow("Trigger", 1);

    // add trackbar
    createTrackbar("Threshold (Pixel Value):", "Trigger", &pixelValueThreshold, 255, NULL);
    createTrackbar("Num of active pixel:",     "Trigger", NULL, width * height, NULL);
    createTrackbar("Threshold (num of pix):",  "Trigger", &numOfPixelThreshold, 100000, NULL);
    createTrackbar("Exposure:",  "Trigger", &exposure, 10000, NULL);
 
    //unconditional loop
    char exposureCommand[128];
    while (true) {
        sprintf(exposureCommand, "v4l2-ctl -c exposure_absolute=%d", exposure);
        system(exposureCommand);
        //system("v4l2-ctl --get-ctrl=exposure_absolute");
        //system("v4l2-ctl --device=/dev/video0 --get-ctrl=exposure_absolute");
        gettimeofday(&tv, NULL);
        old = getMS(tv);
        stream1.read(frame);
        gettimeofday(&tv, NULL);
        cout << "Time diff(ms): " << getMS(tv) - old << endl;
        // convert image into gray scale
        cvtColor(frame, gray, CV_BGR2GRAY);
        // pass throught threshold
        threshold(gray, tIm, pixelValueThreshold, 255, 0);
        // count active pixels
        numOfPixel = countNonZero(tIm);
        setTrackbarPos("Num of active pixel:", "Trigger", numOfPixel);
        Size size(frame.cols/2.5, frame.rows/2.5);
        resize(tIm, tIm2, size);
        resize(frame, frame2, size);
        imshow("Trigger", tIm2);
        imshow("Webcam", frame2);
        if(numOfPixel > numOfPixelThreshold) 
        save();
        if (waitKey(30) >= 0)
            break;
    }
    return 0;
}
