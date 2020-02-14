/*
Junsup Lee - Lab 06 part 1
*/

#include <opencv2\opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {

	Mat image, image_rotate, image_resize, image_perspective, M;
	
	//homography matrix
	Matx<double, 3, 3> H = { 0.4, -0.4, 190,
							0.15, 0.4, 100,
							0, 0, 1 };

	bool close = false;
	
	//read and display image
	image = imread(argv[1], IMREAD_COLOR);

	namedWindow("Image", WINDOW_AUTOSIZE);
	imshow("Image", image);

	waitKey(33);

	while(!close) {
		char option;
		int angle;
		double x_rescale, y_rescale;

		std::cout << "Please select one of following options: rotate (r), resize (s), perspective (p) or exit (x): ";
		std::cin >> option;

		switch (toupper(option))
		{
		case 'R':
			std::cout << "Please enter the angle you wish to rotate. Insert '-' for counter clockwise: ";
			std::cin >> angle;

			//calculate transformation matrix M for rotation
			M = getRotationMatrix2D(Point2f(image.cols/2.0,image.rows/2.0), angle, 1);
			warpAffine(image, image_rotate, M, image.size());

			namedWindow("Rotated", WINDOW_AUTOSIZE);
			imshow("Rotated", image_rotate);

			waitKey(0);
			break;
		case 'S':
			std::cout << "Please enter the value of resizing scale for x and y respectively, separated by space: ";
			std::cin >> x_rescale >> y_rescale;

			resize(image, image_resize, Size(), x_rescale, y_rescale);

			namedWindow("Resized", WINDOW_AUTOSIZE);
			imshow("Resized", image_resize);
			
			waitKey(0);
			break;
		case 'P':
			warpPerspective(image, image_perspective, H, image.size());

			namedWindow("Perspective", WINDOW_AUTOSIZE);
			imshow("Perspective", image_perspective);

			waitKey(0);
			break;
		case 'X':
			close = true;
			break;

		default:
			std::cout << "Invalid option selected, please try again." << std::endl;
			break;
		}
	}


	return 0;
}
