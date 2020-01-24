// Lab02 - Junsup Lee
// exercise taken from https://github.com/oreillymedia/Learning-OpenCV-3_examples/blob/master/example_02-10.cpp as per course
// 
#include <opencv2/opencv.hpp>
#include <iostream>


void help(char** argv) {
	std::cout << "\n"
		<< "\nexample 2-10. The same object can load videos from a camera or a file"
		<< "\nCall:\n"
		<< argv[0] << " [path/image]\n"
		<< "\nor, read from camera:\n"
		<< argv[0]
		<< "\nFor example:\n"
		<< argv[0] << " ../tree.avi\n"
		<< std::endl;
}


int main(int argc, char** argv) {

	help(argv);

	cv::namedWindow("Lab02", cv::WINDOW_AUTOSIZE);   //changed window name - JL 
	cv::VideoCapture cap;

	if (argc == 1) {
		cap.open(0); // open the first camera
	}
	else {
		cap.open(argv[1]);
	}

	if (!cap.isOpened()) { // check if we succeeded
		std::cerr << "Couldn't open capture." << std::endl;
		return -1;
	}

	//local variables
	cv::Mat frame;
	int counter = 0;
	char imgName[50];

	//cout video capture width and height
	std::cout << "\n"
		<< "\nVideo Width: " << cap.get(cv::CAP_PROP_FRAME_WIDTH)
		<< "\nVideo Height: " << cap.get(cv::CAP_PROP_FRAME_HEIGHT)
		<< std::endl;

	for (;;) {

		cap >> frame;

		if (frame.empty()) break; // Ran out of film

		cv::imshow("Lab02", frame);

		//holds key stroke 
		char keystroke = cv::waitKey(33); 
		
		if (keystroke == 13 || keystroke == 32) break;
		else if (keystroke == 120) {

			counter++; //increase filecounter
			std::cout << "screen is captured" << std::endl;

			sprintf_s(imgName, "image%02d.jpg", counter);

			cv::imwrite(imgName, frame);

			//freeze frame for 1 sec. 
			cv::waitKey(1000);
		};
	}

	return 0;

}