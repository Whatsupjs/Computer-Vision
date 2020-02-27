/*
Junsup Lee Lab7
*/

#include<opencv2\opencv.hpp>
#include<cmath>

using namespace cv;

void displayImage(Mat img);

int counter = 0;

int main(int argc, char** argv) {

	Mat img, img_vertical, img_horizontal, img_sobel, img_lap, img_canny;
	
	img = imread(argv[1], IMREAD_GRAYSCALE);
	displayImage(img);

	Matx<float, 3, 3> verticalK = { -1,0,1,-2,0,2,-1,0,1 };
	Matx<float, 3, 3> horiztonalK = { -1,-2,-1,0,0,0,1,2,1 };

	filter2D(img, img_vertical, CV_8U, verticalK);
	displayImage(img_vertical);

	filter2D(img, img_horizontal, CV_8U, horiztonalK);
	displayImage(img_horizontal);

	Mat img_mag(img.rows, img.cols, CV_8U);
	//map out magnitude 
	for (int row = 0; row < img.rows; row++) {
		for (int col = 0; col < img.cols; col++) {
			img_mag.at<uchar>(row, col) = sqrt(std::pow(img_horizontal.at<uchar>(row, col),2) + std::pow(img_vertical.at<uchar>(row, col),2));
		}
	}
	displayImage(img_mag);

	/*
	Mat grad_x, grad_y, abs_grad_x, abs_grad_y, grad;
	/// Gradient X
	Sobel(img, grad_x, CV_8U, 1, 0, 3);
	convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	Sobel(img, grad_y, CV_8U, 0, 1);
	convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
	displayImage(grad);
	*/

	Sobel(img, img_sobel, CV_8U, 1, 1);
	displayImage(img_sobel);

	Laplacian(img, img_lap, CV_8U);
	displayImage(img_lap);

	Canny(img, img_canny, 125, 250);
	displayImage(img_canny);

	waitKey(0);

	return 0;
}


void displayImage(Mat img) {	
	namedWindow(std::to_string(counter), WINDOW_AUTOSIZE);
	imshow(std::to_string(counter), img);

	waitKey(33);

	counter++;
}