
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;
Mat img; Mat templ; Mat rot_img; Mat result_gt; Mat result_rotate;
const char* image_window = "Source Image";
const char* result_window = "Result window";
const char* result2_window = "Rotated Result";

void MatchingMethod(Mat*, Mat*);

int main(int, char** argv)
{
	img = imread(argv[1], IMREAD_GRAYSCALE);
	templ = imread(argv[2], IMREAD_GRAYSCALE);

	img.copyTo(rot_img);
	//Rotate image 30 degrees
	int angle = 30;
	Mat rM; 
	rM = getRotationMatrix2D(Point2f(img.cols / 2.0, img.rows / 2.0), angle, 1);
	warpAffine(img, rot_img, rM, img.size());

	//testing to see if better template yields better result for rotated image. 
	/*Mat rM2;
	rM2 = getRotationMatrix2D(Point2f(templ.cols / 2.0, templ.rows / 2.0), angle, 1);
	warpAffine(templ, templ, rM2, templ.size());*/

	namedWindow(image_window, WINDOW_AUTOSIZE);
	namedWindow(result_window, WINDOW_AUTOSIZE);

	MatchingMethod(&img, &result_gt);
	//MatchingMethod(&rot_img, &result_rotate);


	
	waitKey(0);
	return 0;
}

void MatchingMethod(Mat* img, Mat* result)
{
	Mat img_display;
	img->copyTo(img_display);

	int result_cols = img->cols - templ.cols + 1;
	int result_rows = img->rows - templ.rows + 1;

	result->create(result_rows, result_cols, CV_32FC1);
	matchTemplate(*img, templ, *result, TM_CCORR_NORMED);	//CM_TM_CCORR_NOMRED is changed in opencv 4.0 
	normalize(*result, *result, 0, 1, NORM_MINMAX, -1, Mat());

	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;
	minMaxLoc(*result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	if (TM_CCORR_NORMED == TM_SQDIFF || TM_CCORR_NORMED == TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}
	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	rectangle(*result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	imshow(image_window, img_display);
	imshow(result_window, *result);
	return;
}

