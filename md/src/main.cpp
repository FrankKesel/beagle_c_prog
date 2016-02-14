/* Project: Motion detection with Webcam
 * Autor: F.Kesel
 * Date: 29.1.2016
 * Last Changed: 2.2.2016
 *
 * Purpose: Grab two frames from an webcam attached to USB and detect motion
 * by subtracting the two frames, converting to binary images an calculating
 * the mean value of the number of white pixels. If there is no motion this
 * value should be near zero. If the value is above a threshold flag a detected
 * motion by writing true to a file, otherwise write false.
 */


#include <iostream>
#include <fstream>
#include <time.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>

#include "opencv2/opencv.hpp"   // C++ OpenCV include file
using namespace std;
using namespace cv;

//#define DEBUG

//Cam: Logitech C270, max. 1280 x 720
#define WIDTH 800
#define HEIGHT 600
//Define wait time between the two frames in sec
#define WAIT_TIME 2
//Define threshold for binary filter, i.e. the grey value
#define BINARY_THRESHOLD 64
//Define threshold for mean value of motion detection
#define MOTION_THRESHOLD 5

int main(int argc, char *argv[])
{
	Mat frame, g1, g2, result, filteredResult;
	double imageMetric;
	double binaryThreshold, motionThreshold;
	ofstream resFile;

	VideoCapture capture(0); //Cap object

	//Read thresholds from command line or use defaults
	switch(argc) {
	case 1: binaryThreshold = BINARY_THRESHOLD;
			motionThreshold = MOTION_THRESHOLD;
			break;
	case 2: binaryThreshold = double(atoi(argv[1]));
			motionThreshold = MOTION_THRESHOLD;
			break;
	case 3: binaryThreshold = double(atoi(argv[1]));
			motionThreshold = double(atoi(argv[2]));
			break;
	default: cout << "Usage: md [binaryThreshold] [motionThreshold]" << endl;
	}

	// check if cam is connected
	if(!capture.isOpened()){
		cout << "Failed to connect to the camera." << endl;
		return -1;
	}

	#ifdef DEBUG
	cout << "Capturing with H = "<<HEIGHT<<", W = "<<WIDTH;
	cout << ", pixels = "<< HEIGHT*WIDTH<<endl;
	cout << "Wait time = "<<WAIT_TIME<<", Binary Threshold = "<<binaryThreshold;
	cout << ", Motion Threshold = "<< motionThreshold <<endl;
	#endif

	//Capture 1st frame
	// Set properties in the VideoCapture object: resolution
	capture.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);   // width pixels
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);   // height pixels
	capture.read(frame); //Capture 1st image from cam, 3 color channels
	if(frame.empty()){
		cout << "Failed to capture an image" << endl;
		return -1;
	}
	cvtColor(frame, g1, CV_BGR2GRAY); // convert to grayscale, 1 channel
	capture.release(); //Release cam

	sleep(WAIT_TIME); //Wait between the two frames

	//Capture 2nd frame
	capture.open(0); //Open cam again
	if(!capture.isOpened()){
		cout << "Failed to connect to the camera." << endl;
		return -1;
	}
	// Set properties in the VideoCapture object: resolution
	capture.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);   // width pixels
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);   // height pixels
	capture.read(frame); //Capture 2nd image from cam
	if(frame.empty()){
		cout << "Failed to capture an image" << endl;
		return -1;
	}
	cvtColor(frame, g2, CV_BGR2GRAY); // convert to grayscale

	//Calculate result frame, storing absolute difference of each pixel
	absdiff(g1, g2, result);
	//Convert difference image to binary image, i.e. pixels are 0 (black) or 255 (white)
	threshold(result, filteredResult, binaryThreshold, 255, THRESH_BINARY);
	//Calculate mean value, get value on channel 0 (the only one on grey image)
	imageMetric = mean(filteredResult)[0];

	#ifdef DEBUG
	cout << "Number of non-zero pixels = "<<countNonZero(filteredResult)<<endl;
	cout <<"Pixel mean value on difference: "<< imageMetric <<endl;
	#endif

	//Make a decision if motion was detected
	if (imageMetric > motionThreshold) {
		resFile.open("motion_detected");
		resFile << "TRUE";
		resFile.close();
		imwrite("detected.png", frame);
		#ifdef DEBUG
		cout << "Motion detected!"<<endl;
		#endif
	}

	imwrite("webcam.png", frame);
	#ifdef DEBUG
	imwrite("g1.png", g1);
	imwrite("g2.png", g2);
	imwrite("result.png", filteredResult);
	#endif

	return 0;
}
