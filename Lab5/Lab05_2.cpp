/*
Lab 5 part 1 Junsup Lee
*/

#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {

	Mat image, noise, blurred, filtered, denoised;
	float probability;

	image = imread(argv[1], IMREAD_COLOR);
	image.copyTo(noise);							//copy src image to noise;

	namedWindow("Source Image", WINDOW_AUTOSIZE);
	imshow("Source Image", image);

	waitKey(33);

	//ask for probability 
	std::cout << "Please enter probability value between 0 and 1: ";
	std::cin >> probability;

	while (probability < 0 || probability > 1) {
		std::cout << "Invalid probability, please enter probability value between 0 and 1: ";
		std::cin >> probability;
	};

	//create a new matrix for random values
	Mat x(image.rows, image.cols, CV_32FC1);
	
	//set random uniform values to x.
	RNG rng;
	rng.fill(x, RNG::UNIFORM, 0, 1.0);

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			
			if (x.at<float>(i, j) < probability) {	 //use <float> since x is type of CV_32F ** see cv::Mat document 
				float y = rng.uniform(0.0f, 1.0f);   //In 8-bit images, Imin=0 and Imax=255 therefore, Inoise = Imin + y(Imax - Imin) == y(Imax)
				
				noise.at<Vec3b>(i, j)[0] = y * 255;
				noise.at<Vec3b>(i, j)[1] = y * 255;
				noise.at<Vec3b>(i, j)[2] = y * 255;
			}
			else {
				noise.at<Vec3b>(i, j)[0] = image.at<Vec3b>(i, j)[0];
				noise.at<Vec3b>(i, j)[1] = image.at<Vec3b>(i, j)[1];
				noise.at<Vec3b>(i, j)[2] = image.at<Vec3b>(i, j)[2];
			}			
		}
	}

	namedWindow("Noise", WINDOW_AUTOSIZE);
	imshow("Noise", noise);

	//smooth noised image with 3x3 boxfilter
	boxFilter(noise, blurred, CV_8U, Size(3, 3));

	namedWindow("Blurred", WINDOW_AUTOSIZE);
	imshow("Blurred", blurred);

	//Display blue channel of any pixel and it's neighbours AND compare with blurred image. 
	// it's 3x3 box kernel thus we only need to grab 9 surrounding values from specified point. 
	std::cout << "Blue channel values from point (400,300) and it's surrounding neighbour pixels of Noisy and Blurred image" << std::endl;
	std::cout << "\nNoisy image: " << std::setw(32) << "Blurred image: " << std::endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int row = 400 + i - 1;
			int col = 300 + j - 1; 

			//cast short since Vec3b is uchar type and cout don't know how to handle this
			std::cout << (short)noise.at<Vec3b>(row, col)[0] << std::setw(32) << (short)blurred.at<Vec3b>(row,col)[0] << std::endl;
		}
	}

	//Bilinear filter
	//define kernel;
	Matx<float, 1, 3> horizontalK = { 1,2,1 };
	Matx<float, 3, 1> verticalK = { 1,2,1 };
	Matx<float, 3, 3> bilinearK = cv::Matx<float, 3, 3>::all(1 / 16.0f);

	//bilinear kernel is convolving with 2 tent functions
	bilinearK = bilinearK.mul(verticalK * horizontalK);
	filter2D(noise, filtered, CV_8U, bilinearK);
	
	namedWindow("Filtered", WINDOW_AUTOSIZE);
	imshow("Filtered", filtered);

	//De-noise using medianblur
	medianBlur(noise, denoised, 3);

	namedWindow("De-noised", WINDOW_AUTOSIZE);
	imshow("De-noised", denoised);
	
	waitKey(0);

	return 0;
}