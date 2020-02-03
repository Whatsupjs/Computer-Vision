// taken from https://github.com/oreillymedia/Learning-OpenCV-3_examples for lab purposes and edited. 
// Lab4_2 - Junsup Lee
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

// Define our callback which we will install for
// mouse events
void my_mouse_callback(
	int event, int x, int y, int flags, void* param
);

cv::Rect box;

//used global variables for shape properties for simplicity. 
cv::RotatedRect rbox;
cv::Point p1, p2;
bool drawing_box = false;

//for circle
int roi_x0 = 0, roi_y0 = 0, roi_x1 = 0, roi_y1 = 0;
int radius = std::max(abs(box.width), abs(box.height));

//from lab3
void getColour();

//default colour
cv::Scalar colour = cv::Scalar(255, 255, 255);

char shape = '\0';
int thickness = 2;

// A little subroutine to draw a box onto an image
//
void draw_box(cv::Mat& img, cv::Rect box, char shape) {
		
	switch (shape) {
	case'c':
		cv::circle(img, p1, radius, colour, thickness);
		break;
	case'e':
		cv::ellipse(img, rbox, colour, thickness);
		break;
	case'r': 
		cv::rectangle(img,box.tl(), box.br(), colour, thickness);
		break;
	}
	
}

void help(char** argv) {
	std::cout << "Example 9-2. Toy program for using a mouse to draw boxes on the screen"
		<< "\nCall:\n" << argv[0] <<
		"\n\nshows how to use a mouse to draw regions in an image. Esc to quit\n" << std::endl;
}
int main(int argc, char** argv) {
	help(argv);
		
	cv::Mat image(480, 640, CV_8UC3), temp;

	// setting up text overlay on the image  ****
	cv::String title = "My Drawing Application";
	int fontFace = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;
	double fontScale = 1;
	int thickness = 1;
	int baseline = 0;

	cv::Size textSize = cv::getTextSize(title, fontFace,
		fontScale, thickness, &baseline);

	baseline += thickness;

	cv::Point textOrg((image.cols - textSize.width) / 2, (40 + textSize.height));
	// *****

	image.copyTo(temp);
	box = cv::Rect(-1, -1, 0, 0);
	image = cv::Scalar::all(0);
	
	cv::namedWindow("Lab4_2");
	cv::moveWindow("Lab4_2", 10, 30);

	// Here is the crucial moment where we actually install
	// the callback. Note that we set the value of 'params' to
	// be the image we are working with so that the callback
	// will have the image to edit.
	//
	cv::setMouseCallback(
		"Lab4_2",
		my_mouse_callback,
		(void*)&image
	);
	

	// The main program loop. Here we copy the working image
	// to the temp image, and if the user is drawing, then
	// put the currently contemplated box onto that temp image.
	// Display the temp image, and wait 15ms for a keystroke,
	// then repeat.
	//
	for (;;) {
		
		//show baseline + text overlay
		cv::line(image, textOrg + cv::Point(0, thickness),
			textOrg + cv::Point(textSize.width, thickness),
			cv::Scalar(0, 0, 255));
		cv::putText(image, title, textOrg, fontFace, fontScale,
			cv::Scalar::all(255), thickness, 8);

		image.copyTo(temp);

		if (drawing_box) draw_box(temp, box, shape);
		
		cv::imshow("Lab4_2", temp);
		
		
		if (cv::waitKey(15) == 27) {
			char s;

			cv::destroyWindow("Lab4_2");
			std::cout << "Would you like to save the image before exiting? (y / n): ";
			std::cin >> s;

			if (toupper(s) == 'Y') {
				char imgName[50];
				sprintf_s(imgName, "Lab04_2.jpg");
				cv::imwrite(imgName, temp);
			}

			break;
		}
	}
	return 0;
}

// This is our mouse callback. If the user
// presses the left button, we start a box.
// When the user releases that button, then we
// add the box to the current image. When the
// mouse is dragged (with the button down) we
// resize the box.
//
void my_mouse_callback(int event, int x, int y, int flags, void* param)
{
	cv::Mat& image = *(cv::Mat*) param;

	if (flags & cv::EVENT_FLAG_SHIFTKEY) {
		switch (event) {

		case cv::EVENT_MOUSEMOVE:
			if (drawing_box) {
				radius = std::max(abs(p1.x - x), abs(p1.y - y));
			}
			break;

		case cv::EVENT_LBUTTONDOWN:
			drawing_box = true;
			p1 = cv::Point(x, y);
			shape = 'c';
			break;

		case cv::EVENT_LBUTTONUP:
			drawing_box = false;
			p2 = cv::Point(x, y);
			getColour();
			draw_box(image, box, shape);
			break;
		}

	}
	else if (flags & cv::EVENT_FLAG_CTRLKEY) {
		switch (event) {
		case cv::EVENT_MOUSEMOVE:
			if (drawing_box) {
				rbox.size.width = abs(x - rbox.center.x);
				rbox.size.height = abs(y - rbox.center.y);
			}
		    break;

		case cv::EVENT_LBUTTONDOWN:
			drawing_box = true;
			rbox.center.x = x;
			rbox.center.y = y;
			
			shape = 'e';
			break;
		
		case cv::EVENT_LBUTTONUP: 
			drawing_box = false;
			getColour();
			draw_box(image, box, shape);
		  break;
		}
	}
	else {
		switch (event) {
		case cv::EVENT_MOUSEMOVE:
			if (drawing_box) {
				box.width = x - box.x;
				box.height = y - box.y;
			}
			break;

		case cv::EVENT_LBUTTONDOWN:
			drawing_box = true;
			box = cv::Rect(x, y, 0, 0);
			shape = 'r';
			break;

		case cv::EVENT_LBUTTONUP:
			drawing_box = false;
			if (box.width < 0) {

				box.x += box.width;
				box.width *= -1;
			}
			if (box.height < 0) {
				box.y += box.height;
				box.height *= -1;
			}
			getColour();
			draw_box(image, box, shape);
			break;
		}
	}
}

//get colour from user (taken from lab3) 
void getColour() {
	char input;
	bool valid = false;

	std::srand(time(NULL));

	while (!valid) {
		std::cout << "Enter a letter for color (x:random, r:red, g:green, b:blue, k:black, w:white, y:yellow, c:cyan, m:mangenta): ";
		std::cin >> input;
		switch (toupper(input)) { //use toupper to cater to both upper and lower case.
								  //scalar in BGR order
								  //blue
		case 'B':
			colour = cv::Scalar(255, 0, 0);
			valid = true;
			break;
			//cyan
		case 'C':
			colour = cv::Scalar(255, 255, 0);
			valid = true;
			break;
			//green
		case 'G':
			colour = cv::Scalar(0, 255, 0);
			valid = true;
			break;
			//black
		case 'K':
			colour = cv::Scalar(0, 0, 0);
			valid = true;
			break;
			//magenta
		case 'M':
			colour = cv::Scalar(255, 0, 255);
			valid = true;
			break;
			//red
		case 'R':
			colour = cv::Scalar(0, 0, 255);
			valid = true;
			break;
			//white
		case 'W':
			colour = cv::Scalar(255, 255, 255);
			valid = true;
			break;
			//random 0 - 255
		case 'X':
			colour = cv::Scalar(rand() % 256, rand() % 256, rand() % 256);
			valid = true;
			break;
			//yellow
		case 'Y':
			colour = cv::Scalar(0, 255, 255);
			valid = true;
			break;
		default:
			std::cout << "Please enter a letter provided in the instruction above" << std::endl;
		}
	}
}