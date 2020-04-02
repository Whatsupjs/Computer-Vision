// Lab10 - Junsup Lee
// exercise taken from https://github.com/oreillymedia/Learning-OpenCV-3_examples/blob/master/example_02-10.cpp as per course
// 
#define _CRT_SECURE_NO_WARNINGS 
#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <Windows.h>
#include <ctime>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	
	cv::namedWindow("Lab10", cv::WINDOW_AUTOSIZE);   //changed window name
	cv::VideoCapture cap;

	if (argc == 1) {
		cap.open(0); // open the first camera
	}
	else {
		cap.open(argv[1]);
	}

	if (!cap.isOpened()) { // check if we succeeded
		std::cerr << "Couldn't open capture." << std::endl;
		return -1;
	}

	//local variables
	cv::Mat frame, grayframe, background;
	int counter = 0;
	char imgName[50];
	double result, trigger;

	//second capture is needed as initial capture is very dark due to capturing frame before camera adjusts to light. 
	cap >> frame;
	cap >> frame; 

	//convert frame to grayscale
	cvtColor(frame, grayframe, CV_BGR2GRAY);
	//set background reference image
	grayframe.copyTo(background);

	for (;;) {
		cap >> frame;
		
		if (frame.empty()) break; // if no feed is found
		
		Mat diff, diff_thresh;
		cvtColor(frame, grayframe, CV_BGR2GRAY);
		
		absdiff(grayframe, background, diff);		//show difference
		
		threshold(diff, diff_thresh, 128, 1, THRESH_BINARY);	//set threshold of 128
		cv::imshow("Lab10", diff);

		result = sum(diff_thresh)[0];
		trigger = 0.008 * frame.rows * frame.cols;	//changed change trigger from 0.01 to 0.008

		if (result > trigger) {
			Beep(523, 50); //make beep noise
			
			time_t currTime = time(0);				//current system time. 
			tm *now = localtime(&currTime);			//convert to local time.
			
			char* dt = ctime(&currTime);			//string of local time. 

			cout << "Activity detected at " <<
				now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << endl;

			sprintf_s(imgName, "image%02d.jpg", counter);
			cv::imwrite(imgName, frame);

			//update detected frame to background
			grayframe.copyTo(background);
			counter++;

			waitKey(2000); // wait 2 sec before continuing loop.
		}
		//holds key stroke 
		char keystroke = cv::waitKey(33);

		if (keystroke == 27 || keystroke == 32) break;	//if 'esc or space' is pressed, quit
	}

	return 0;

}