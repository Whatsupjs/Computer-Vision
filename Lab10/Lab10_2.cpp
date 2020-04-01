
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
const char* rotated_window = "Rotated Image";
const char* result2_window = "Rotated Result";

Point MatchingMethod(Mat*, Mat*, const char*, const char*);

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
	
	namedWindow(image_window, WINDOW_AUTOSIZE);
	namedWindow(result_window, WINDOW_AUTOSIZE);
	namedWindow(rotated_window, WINDOW_AUTOSIZE);
	namedWindow(result2_window, WINDOW_AUTOSIZE);

	Point gtLoc, fLoc; 

	gtLoc = MatchingMethod(&img, &result_gt, image_window, result_window);
	fLoc = MatchingMethod(&rot_img, &result_rotate, rotated_window, result2_window);

	int gtlx = gtLoc.x, flx = fLoc.x, gthx = gtLoc.x + templ.cols, fhx = fLoc.x + templ.cols;
	int gtly = gtLoc.y, fly = fLoc.y, gthy = gtLoc.y + templ.rows, fhy = fLoc.y + templ.rows;
	
	float TP = 0, TN = 0, FP = 0, FN = 0;
	for (int i = min(gtlx, flx); i < max(gthx, fhx); i++) {
		for (int j = min(gtly, fly); j < max(gthy, fhy); j++) {
			bool withinGT = (i >= gtlx) && (i < gthx) && (j >= gtly) && (j < gthy);
			bool withinF = (i >= flx) && (i < fhx) && (j >= fly) && (j < fhy);

			if (withinGT && withinF) TP++;
			else if (withinGT && !withinF) FN++;
			else if (!withinGT && withinF) FP++;
			else if (!withinGT && !withinF) TN++;
		}
	}
	
	float precision = 0, recall = 0;
	recall = TP/(TP + FN);
	precision = TP/(TP + FP);

	cout << "TP: " << TP << endl
		<< "FN: " << FN << endl
		<< "FP: " << FP << endl
		<< "TN: " << TN << endl
		<< "Recall: " << recall << endl
		<< "Precision: " << precision << endl;

	waitKey(0);
	return 0;
}

Point MatchingMethod(Mat* img, Mat* result, const char* image_window, const char* result_window)
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
	
	matchLoc = maxLoc;

	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	rectangle(*result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	imshow(image_window, img_display);
	imshow(result_window, *result);
	return matchLoc;
}

