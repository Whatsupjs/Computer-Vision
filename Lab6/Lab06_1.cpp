/*
Junsup Lee - Lab 06 part 1
*/

#include <opencv2\opencv.hpp>

using namespace cv;

int main1(int argc, char** argv) {

	Mat image, binary_image, morphed_open3, morphed_close3, morphed_open7, morphed_close7; 
	image = imread(argv[1], IMREAD_GRAYSCALE);

	namedWindow("Source", WINDOW_AUTOSIZE);
	imshow("Source", image);
	imwrite("Source.jpg", image);

	//convert grayscale to binary with 128 as threshold w max 255
	threshold(image, binary_image, 128, 255, THRESH_BINARY);
	namedWindow("Binary", WINDOW_AUTOSIZE);
	imshow("Binary", binary_image);
	imwrite("Binary.jpg", binary_image);

	//kernel used for morphing
	Mat kernel_3x = 255 * Mat::ones(3, 3, CV_8UC1);

	//opening morph
	morphologyEx(binary_image, morphed_open3, MORPH_OPEN, kernel_3x);
	namedWindow("Morphed_Open3", WINDOW_AUTOSIZE);
	imshow("Morphed_Open3", morphed_open3);
	imwrite("Morphed_Open3.jpg", morphed_open3);

	//closing morph
	morphologyEx(binary_image, morphed_close3, MORPH_CLOSE, kernel_3x);
	namedWindow("Morphed_Close3", WINDOW_AUTOSIZE);
	imshow("Morphed_Close3", morphed_close3);
	imwrite("Morphed_Close3.jpg", morphed_close3);

	//repeat open and close morph with 7x7 kernel
	Mat kernel_7x = 255 * Mat::ones(7, 7, CV_8UC1);

	morphologyEx(binary_image, morphed_open7, MORPH_OPEN, kernel_7x);
	namedWindow("Morphed_Open7", WINDOW_AUTOSIZE);
	imshow("Morphed_Open7", morphed_open7);
	imwrite("Morphed_Open7.jpg", morphed_open7);

	morphologyEx(binary_image, morphed_close7, MORPH_CLOSE, kernel_7x);
	namedWindow("Morphed_Close7", WINDOW_AUTOSIZE);
	imshow("Morphed_Close7", morphed_close7);
	imwrite("Morphed_Close7.jpg", morphed_close7);

	waitKey(0);

	return 0;

}
