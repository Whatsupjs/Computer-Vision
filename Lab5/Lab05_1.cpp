/*
Lab 5 part 1 Junsup Lee
*/

#include <opencv2/opencv.hpp>

using namespace cv; 

int main1(int argc, char** argv) {

	Mat image, img_gray;

	image = imread(argv[1], IMREAD_COLOR);
	cvtColor(image, img_gray, COLOR_BGR2GRAY);	//convert color image to grayscale

	equalizeHist(img_gray, img_gray);			//equalize histogram values of grayscale image

	namedWindow("Before", WINDOW_AUTOSIZE);
	imshow("Before", image);

	namedWindow("After", WINDOW_AUTOSIZE);
	imshow("After", img_gray);

	waitKey(0);

	return 0;
}