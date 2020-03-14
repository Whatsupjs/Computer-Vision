#include<opencv2\opencv.hpp>
#include<iostream>
#include<fstream>

int calcPR(bool *pred, bool *gt, int len, double* P, double* R);
double calcFb(double P, double R, double beta);
void thresh_v(double *A, bool *B, int len, double thresh);
void thresh_img(cv::Mat img, bool *B, double thresh);

using namespace std;

int main(int argc, char** argv) {
	int option = -1;
	while (option != 0 ) {

		ifstream gtFile;
		int len = 0;
		gtFile.open("gt.csv");
		if (!gtFile) {
			cerr << "Error opening the gt file" << endl;
			exit(1);
		}
		std::string line;
		while (getline(gtFile, line)) {
			len++;
		}
		gtFile.close();

		bool* gt = new bool[len];
		bool gtData;
		gtFile.open("gt.csv");
		for (int i = 0; i < len; i++) {
			gtFile >> gt[i];
		}
		gtFile.close();

		cout << "Please input a number between 1 and 4: ";
		cin >> option;

		if (option == 1) {
			int counter = 0;
			double precision = 0;
			double recall = 0;
			
			ifstream predFile;
			// READING CSV FILE vvvvvvvvvvv
			predFile.open("alg_bin.csv");
			if (!predFile) {
				cerr << "Error opening the pred file" << endl;
				exit(1);
			}
			std::string line;
			while (getline(predFile, line)) {
				counter++;
			}
			predFile.close();
			
			bool* pred = new bool[counter];
			bool predData;
			predFile.open("alg_bin.csv");
			for (int i = 0; i < counter; i++) {
				predFile >> pred[i];
			}
			predFile.close();
			// FINISH READING FILE ^^^^^^^^

			int belong = calcPR(pred, gt, counter, &precision, &recall);

			double F1 = 0;
			if (belong == 1) { 
				F1 = calcFb(precision, recall, 1);
			}
			cout << "Recall: " << recall << endl;
			cout << "Precision: " << precision << endl;
			cout << "F1: " << F1 << endl;
		}
		else if (option == 2) {
			int counter = 0;

			ifstream probFile;
			probFile.open("alg_dbl.csv");
			if (!probFile) {
				cerr << "Error opening the thresh file" << endl;
				exit(1);
			}
			std::string line;
			while (getline(probFile, line)) {
				counter++;
			}
			probFile.close();
			probFile.open("alg_dbl.csv");

			double* prob = new double[counter];
			for (int i = 0; i < counter; i++) {
				probFile >> prob[i];
			}
			probFile.close();

			double threshold = 0.1;
			ofstream PRFile;
			PRFile.open("PR.csv");
			PRFile << "Precision," << "Recall," << "F1" << "\n";
			while (PRFile.is_open()) {
				bool* B = new bool[counter];
				thresh_v(prob, B, counter, threshold);

				double precision = 0;
				double recall = 0;
				int belong = calcPR(B, gt, counter, &precision, &recall);
				double F1 = 0;
				if (belong == 1) {
					F1 = calcFb(precision, recall, 1);
					PRFile << precision << "," << recall << "," << F1 << "\n";
				}

				cout << "THRESHOLD IS: " << threshold <<
					" PRECISION: " << precision << " RECALL: " << recall << endl;
				
				threshold += 0.1;
				B = nullptr;
				delete[] B;

				if (threshold > 1) {
					PRFile.close();
				}
			}
		}
		else if (option == 3) {
			cv::Mat img_gt, img1;
			img1 = cv::imread("bitmap_A1.png", cv::IMREAD_GRAYSCALE);
			img_gt = cv::imread("bitmap_gt.png", cv::IMREAD_GRAYSCALE);
			int threshold = 254;
			int SIZE = img_gt.cols * img_gt.rows;
			double precision = 0, recall = 0;
			double F1 = 0;

			bool* gt = new bool[SIZE];
			thresh_img(img_gt, gt, threshold);

			bool* B = new bool[SIZE];
			thresh_img(img1, B, threshold);

			int belong = calcPR(B, gt, SIZE, &precision, &recall);
			if (belong == 1) {
				F1 = calcFb(precision, recall, 1);
			}
			cout << "F1: " << F1 << endl;
		}
		else if (option == 4) {
			cv::Mat img2, img_gt;
			img2 = cv::imread("bitmap_A2.png", cv::IMREAD_GRAYSCALE);
			img_gt = cv::imread("bitmap_gt.png", cv::IMREAD_GRAYSCALE);
			int threshold = 25;
			int SIZE = img_gt.cols * img_gt.rows;

			bool* gt = new bool[SIZE];
			thresh_img(img_gt, gt, 254);

			ofstream PRFile;
			PRFile.open("PR_img.csv");
			PRFile << "Precision," << "Recall," << "F1" << "\n";
			while (PRFile.is_open()) {
				bool* B = new bool[SIZE];
				thresh_img(img2, B, threshold);

				double precision = 0;
				double recall = 0;
				int belong = calcPR(B, gt, SIZE, &precision, &recall);
				double F1 = 0;
				if (belong == 1) {
					F1 = calcFb(precision, recall, 1);
					PRFile << precision << "," << recall << "," << F1 << "\n";
				}

				cout << "THRESHOLD IS: " << threshold <<
					" PRECISION: " << precision << " RECALL: " << recall << endl;

				threshold += 25;
				B = nullptr;
				delete[] B;

				if (threshold > 250) {
					PRFile.close();
				}
			}

		}
		else {
			cout << "Invalid number, try again." << endl;
		}
	}
	
}

int calcPR(bool *pred, bool *gt, int len, double *P, double *R) {
	int result = 1;
	double TP = 0, FP = 0, TN = 0, FN = 0;
	
	if (P != nullptr && R != nullptr) {
		for (int i = 0; i < len; i++) {
			if (pred[i] && gt[i]) {
				TP++;
			}
			else if (pred[i] && !gt[i]) {
				FP++;
			}
			else if (!pred[i] && gt[i]) {
				FN++;
			}
			else {
				TN++;
			}
		}

		*R = TP / (TP + FN);
		if ((TP == 0 && FN == 0) || (TP==0 && FP == 0)) {
			*P = 1;
		}
		else {
			*P = TP / (TP + FP);
		}
		//cout << "TP: " << TP << " FP: " << FP << " FN: " << FN << endl;
	}
	else {
		result = 0;		
	}	
	return result;
}

double calcFb(double P, double R, double beta) {
	double Fb = (beta + 1) * ((P * R) / ((beta * P) + R));
	return Fb; 
}

void thresh_v(double *A, bool *B, int len, double thresh) {
	for (int i = 0; i < len; i++) {
		B[i] = (A[i] > thresh);
	}
}

void thresh_img(cv::Mat img, bool *B, double thresh) {
	int index = 0;
	for (int row = 0; row < img.rows; row++) {
		for (int col = 0; col < img.cols; col++) {
			B[index] = (img.at<uchar>(row, col) > thresh);
			index++;
		}
	}
}