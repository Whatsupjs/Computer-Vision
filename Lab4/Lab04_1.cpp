/* 
Lab04-1 Junsup Lee
*/
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	
	for (;;) {
		char _option = '\0';

		std::cout << "Please enter 'A' for brightness/contrast or 'B' for blending of two images: ";
		std::cin >> _option;

		if (toupper(_option) == 'A') {

			//define image variable;
			cv::Mat image;

			// insert image from argument path with colour
			image = cv::imread(argv[1], cv::IMREAD_COLOR);

			//create a window for the image 
			cv::namedWindow("Lab4-1A Original", cv::WINDOW_AUTOSIZE);
			cv::imshow("Lab4-1A Original", image);

			//create new image with higher brightness 
			cv::Mat image2;
			image.copyTo(image2);
			image2 += cv::Scalar(200, 200, 200);

			//create new window for newly created image
			cv::namedWindow("Lab4-1A Brightness", cv::WINDOW_AUTOSIZE);
			cv::imshow("Lab4-1A Brightness", image2);

			//create new image with higher contrast
			cv::Mat image3;
			image.copyTo(image3);
			image3 *= 1.5;

			//create new window for newly created image
			cv::namedWindow("Lab4-1A Contrast", cv::WINDOW_AUTOSIZE);
			cv::imshow("Lab4-1A Contrast", image3);

			std::cout << "*** Values of 3 Channels of colour for each image as BGR ***" << std::endl;
			std::cout << "Original: " << image.at<cv::Vec3b>(100, 200) << std::endl;
			std::cout << "Brightness: " << image2.at<cv::Vec3b>(100, 200) << std::endl;
			std::cout << "Contrast: " << image3.at<cv::Vec3b>(100, 200) << std::endl;

			cv::waitKey(0);

			return 0;
		} if (toupper(_option) == 'B') {
			//image variables;
			cv::Mat image1;
			cv::Mat image2;

			//insert images from argument path with colour
			image1 = cv::imread(argv[1], cv::IMREAD_COLOR);
			image2 = cv::imread(argv[2], cv::IMREAD_COLOR);

			//create a window for the image 
			cv::namedWindow("Lab4-1B image1", cv::WINDOW_AUTOSIZE);
			cv::imshow("Lab4-1B image1", image1);

			cv::namedWindow("Lab4-1B image2", cv::WINDOW_AUTOSIZE);
			cv::imshow("Lab4-1B image2", image2);

			cv::waitKey(33);

			float alpha;

			while (true) {
				std::cout << "Please input a number between 0 and 1: ";
				std::cin >> alpha;

				if (alpha >= 0 && alpha <= 1) break;
				else
					std::cout << "Invalid number, please try again." << std::endl;
			}

			//blend images with alpha value inserted by the user
			cv::Mat blended = (1 - alpha) * image1 + alpha * image2;

			//generate new window and display blended image
			cv::namedWindow("Lab4-1B blended", cv::WINDOW_AUTOSIZE);
			cv::imshow("Lab4-1B blended", blended);

			//split image into 3 planes of BGR to get histogram values of specific colour. 
			std::vector<cv::Mat> bgr_planes;
			cv::split(blended, bgr_planes);

			cv::Mat hist;

			int nimages = 1; // Only 1 image, that is the Mat scene.
			int channels[] = { 0 }; // Index for hue channel
			int dims = 1; // Only 1 channel, the hue channel
			int histSize[] = { 8 }; // 8 bins
			float hranges[] = { 0, 180 }; // hue varies from 0 to 179, see cvtColor
			const float *ranges[] = { hranges };
			
			//calculate histogram values via calcHist
			calcHist(&bgr_planes[1], nimages, channels, cv::Mat(), hist, dims, histSize, ranges, true, false);

			std::cout << "Histogram values of Blue channel only: " << std::endl;
			std::cout << hist << std::endl;

			cv::waitKey(0);

			return 0;
		}
		else {
			std::cout << "Please input either 'A' or 'B'" << std::endl; 
		}
	}
	
}