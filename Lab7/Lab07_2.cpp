/*
Junsup Lee Lab7
*/
#include<opencv2\opencv.hpp>
#include<vector>

using namespace cv;

void displayImage(Mat img);
int counter = 0;

int main(int argc, char** argv) {
	Mat img, img_canny, img_houghLine, img_houghCircle;

	//read image as grayscale;
	img = imread(argv[1], IMREAD_GRAYSCALE);
	displayImage(img);

	Canny(img, img_canny, 125, 250);
	displayImage(img_canny);

	cvtColor(img_canny, img_houghLine, COLOR_GRAY2BGR);

	std::vector<Vec2f> lines;

	cv::HoughLines(img_canny, lines, 1, CV_PI / 180, 200, 0, 0);
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(img_houghLine, pt1, pt2, Scalar(0, 0, 255), 2, CV_AA);
	}

	// ^calculating vanishing point with 2 intersecting lines from above ^
	float rho1 = lines[0][0], theta1 = lines[0][1];
	float rho2 = lines[1][0], theta2 = lines[1][1];
	double m1 = -tan(theta1), m2 = -tan(theta2);
	float b1 = rho1 / cos(theta1), b2 = rho2 / cos(theta2);
	//intersecting point = (ci, ri) col, row 
	float ri = (b2 - b1) / (m1 - m2);
	float ci = (m1*b2 - b1 * m2) / (m1 - m2);

	Point vanishingP = Point(ci, ri);
	//draw in the intersecting point of the 2 lines
	circle(img_houghLine, vanishingP, 4, Scalar(0, 255, 0), -1);
	displayImage(img_houghLine);

	//copy over image to find circles. 
	img_houghLine.copyTo(img_houghCircle);
	
	std::vector<Vec3f> circles;

	HoughCircles(img_canny, circles, cv::HOUGH_GRADIENT, 1, 30, 250, 30, 10, 50);
	for (size_t i = 0; i < circles.size(); i++) {
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// circle center
		circle(img_houghCircle, center, radius, Scalar(0, 255, 255));
	}
	displayImage(img_houghCircle);

	waitKey(0);

	return 0;
}

void displayImage(Mat img) {
	namedWindow(std::to_string(counter), WINDOW_AUTOSIZE);
	imshow(std::to_string(counter), img);

	waitKey(33);

	counter++;
}