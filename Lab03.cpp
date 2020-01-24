// Lab03 - Junsup Lee
#include <opencv2/opencv.hpp>
#include <iostream>

bool valid_co(cv::Point_<int> tp, cv::Point_<int> bt, int wx, int wy);
cv::Scalar getColour();


int main(int argc, char** argv) {
	
	//define window and image 
	cv::namedWindow("Lab3", cv::WINDOW_AUTOSIZE);
	cv::Mat image = cv::Mat::zeros(480,640, CV_8UC3);
	cv::Size _size = image.size();
	
	char imgName[50];				//for screenshot name

	std::vector<cv::Rect> rec_stack; // this is not needed but just incase we need to track objects. 
	std::srand(1);					 //(1) to refresh rand each loop 

	for (;;) {

		//display image
		cv::imshow("Lab3", image);
		
		cv::waitKey(33);

		bool validator = false;
		cv::Point_<int> top_co, bottom_co, check_co;
		cv::Scalar colour;
		char _continue;

		while (!validator) {
			std::cout << "Enter the coordinates of the top left corner (x y): ";
			std::cin >> top_co.x >> top_co.y;

			std::cout << "Enter the coordinates of the bottom right corner (x y): ";
			std::cin >> bottom_co.x >> bottom_co.y;

			validator = valid_co(top_co, bottom_co, _size.width, _size.height);
		}

		//get colour from user input - made it to a function since it's long
		colour = getColour();
		
		//colour in pixels according coordinates provided
		for (int i = top_co.y; i < bottom_co.y; i++) {
			for (int j = top_co.x; j < bottom_co.x; j++) {
				//Vec3b to use 3 channels of colour AND at() is in height by width, therefore (y,x) 
				image.at<cv::Vec3b>(i, j)[0] = colour[0]; //blue
				image.at<cv::Vec3b>(i, j)[1] = colour[1]; //green
				image.at<cv::Vec3b>(i, j)[2] = colour[2]; //red
			}
		}

		//update image to show rectangle
		cv::imshow("Lab3", image);
		cv::waitKey(33);

		//create Rect object
		cv::Rect rectangle = cv::Rect(top_co, bottom_co);

		//print out newly created Rect object's area , width , height
		std::cout << "Area: " << rectangle.area() << ", width: " << rectangle.width << ", height: " << rectangle.height << std::endl;

		std::cout << "Enter the coordinates of a point: ";
		std::cin >> check_co.x >> check_co.y;

		//check if new coordinate is inside newly created rect
		if (rectangle.contains(check_co)) {
			std::cout << "This point (" << check_co.x << ", " << check_co.y << ") is inside the rectangle" << std::endl;
		}
		else {
			std::cout << "This point (" << check_co.x << ", " << check_co.y << ") is not inside the rectangle" << std::endl;
		}

		//add to the stack in case we need it later on
		rec_stack.push_back(rectangle);


		std::cout << "Continue (y/n)? ";
		std::cin >> _continue; 

		if (toupper(_continue) == 'N') {

			sprintf_s(imgName, "Lab03.jpg");
			cv::imwrite(imgName, image);
			break;
		};
		
	}

	return 0;

}

//check if the the coordinates are within the image size limit
bool valid_co(cv::Point_<int> tp, cv::Point_<int> bt, int wx, int wy) {

	bool result = false;

	//check if coordinates are as top left and bottom right 
	if (tp.x < bt.x && tp.y < bt.y) {
		//check if it's within size limit 
		if (((tp.x <= wx && tp.x >= 0) && (tp.y <= wy && tp.y >= 0)) && ((bt.x <= wx && bt.x >= 0) && (bt.y <= wy && bt.y >= 0))) {
			result = true;
		}
		else {
			std::cout << "Invalid coordinates. Please enter coordinates within (0,0) and (" << wx << "," << wy << ")." << std::endl;
		}
	}
	else {
		std::cout << "Invalid coordinates. Bottom right coordinates cannot be less than top left coordinates." << std::endl;
	}

	return result;
}

//get colour from user input
cv::Scalar getColour() {
	cv::Scalar value; 
	char input;
	bool valid = false;
	
	while (!valid) {
		std::cout << "Enter a letter for color (x:random, r:red, g:green, b:blue, k:black, w:white, y:yellow, c:cyan, m:mangenta): ";
		std::cin >> input;
		switch (toupper(input)) {
		//scalar in BGR order
		//blue
		case 'B':
			value = cv::Scalar(255, 0, 0);
			valid = true;
			break;
		//cyan
		case 'C':
			value = cv::Scalar(255, 255, 0);
			valid = true;
			break;
		//green
		case 'G':
			value = cv::Scalar(0, 255, 0);
			valid = true;
			break;
		//black
		case 'K':
			value = cv::Scalar(0, 0, 0);
			valid = true;
			break;
		//magenta
		case 'M':
			value = cv::Scalar(255, 0, 255);
			valid = true;
			break;
		//red
		case 'R':
			value = cv::Scalar(0, 0, 255);
			valid = true;
			break;
		//white
		case 'W':
			value = cv::Scalar(255, 255, 255);
			valid = true;
			break;
		//random 0 - 255
		case 'X':
			value = cv::Scalar(rand() % 256, rand() % 256, rand() % 256);
			valid = true;
			break;
		//yellow
		case 'Y':
			value = cv::Scalar(0, 255, 255);
			valid = true;
			break;
		default:
			std::cout << "Please enter a letter provided in the instruction above" << std::endl;
		}
	}
	
	return value;
}