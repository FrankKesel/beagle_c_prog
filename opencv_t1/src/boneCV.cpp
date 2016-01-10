
#include<iostream>
#include<opencv2/opencv.hpp>   // C++ OpenCV include file
using namespace std;
using namespace cv;            // using the cv namespace too

int main()
{
	VideoCapture capture(0);   // capture object, 0: only one cam available
   Mat frame, g1, g2, result;    

   cout << "Started Processing - Capturing Image" << endl;
   // Set properties in the VideoCapture object: resolution
   capture.set(CV_CAP_PROP_FRAME_WIDTH,800);   // width pixels
   capture.set(CV_CAP_PROP_FRAME_HEIGHT,600);   // height pixels
   if(!capture.isOpened()){   // check if cam is connected
   	cout << "Failed to connect to the camera." << endl;
   	return -1;
   }

	capture.read(frame); //Capture image from cam    
   if(frame.empty()){         
      cout << "Failed to capture an image" << endl;
      return -1;
   }
   cvtColor(frame, g1, CV_BGR2GRAY); // convert to grayscale

	capture.read(frame); //Capture image from cam    
   if(frame.empty()){         
      cout << "Failed to capture an image" << endl;
      return -1;
   }
   cvtColor(frame, g2, CV_BGR2GRAY); // convert to grayscale
   
   subtract(g1, g2, result);
   
   Mat D(result, Rect(400,400,10,10));
   cout << "D: " << endl << D << endl;

	double s = sum(result)[0];
	s = s/480000;
	cout <<"Charakteristik: "<< s <<endl;
	  

   imwrite("../img/capture.png", frame);
   imwrite("../img/g1.png", g1);
   imwrite("../img/g2.png", g2);
   imwrite("../img/result.png", result);
   return 0;
}
