#ifndef HISTOGRAMEQUALIZATIONFORGRAY_H
#define HISTOGRAMEQUALIZATIONFORGRAY_H
#include <iostream>
#include "CImg.h"
using namespace std;
using namespace cimg_library;

class HistogramEqualizationForGray {
	CImg<unsigned char> img;
	int width;
	int height;
public:
	HistogramEqualizationForGray(string filepath, string targetName) {
		CImg<unsigned char> temp(filepath.c_str());
		img = temp;
		width = img.width();
		height = img.height();
		CImg<unsigned char> grayImg = toGrayImg();
		grayImg.save("LenaGray.bmp");
		double probabilities[256];
		countForProbabilities(grayImg, probabilities);
		int targetGrayValue[256];
		countForTargetValue(probabilities, targetGrayValue);
		for(int i = 0; i < 256; i++) {
			cout << targetGrayValue[i] << " ";
		}
		CImg<unsigned char> targetImg = toTargetImg(grayImg, targetGrayValue);
		targetImg.display();
		targetImg.save(targetName.c_str());
	}

	CImg<unsigned char> toGrayImg() {
		CImg<unsigned char> grayImg = CImg<unsigned>(width, height, 1, 1);
		cimg_forXY(img, x, y) {
			grayImg(x,y) = img(x,y,0,0) * 0.299 + img(x,y,0,1) * 0.587 + img(x,y,0,2)*0.114;
		}
		return grayImg;
	}

	void countForProbabilities(const CImg<unsigned char>& grayImg, double* probabilities) {
		for(int i = 0; i < 256; i++) {
			probabilities[i] = 0;
		}
		cimg_forXY(img, x, y) {
			probabilities[grayImg(x,y)]++;
		}
		for(int i = 0; i < 256; i++) {
			probabilities[i] /= (double) height*width;
		}
	}

	void countForTargetValue(const double* probabilities, int* targetGrayValue) {
		double sum = 0;
		for(int i = 0; i < 256; i++) {
			sum += probabilities[i];
			targetGrayValue[i] =(int)(sum * 255);
		}
	}

	CImg<unsigned char> toTargetImg(const CImg<unsigned char>& grayImg, const int* targetGrayValue) {
		CImg<unsigned char> targetImg = CImg<unsigned>(width, height, 1, 1);
		cimg_forXY(img, x, y) {
			targetImg(x,y) = targetGrayValue[grayImg(x,y)];
		}
		return targetImg;
	}

};


class HistogramEqualizationForColor {
	CImg<unsigned char> img;
	int width;
	int height;
public:
	HistogramEqualizationForColor(string filepath, string targetName) {
		CImg<unsigned char> temp(filepath.c_str());
		img = temp;
		width = img.width();
		height = img.height();
		double probabilities[3][256];
		countForProbabilities(img, probabilities);
		int targetColorValue[3][256];
		countForTargetValue(probabilities, targetColorValue);
		for(int i = 0; i < 256; i++) {
			cout << targetColorValue[i] << " ";
		}
		CImg<unsigned char> targetImg = toTargetImg(img, targetColorValue);
		targetImg.display();
		targetImg.save(targetName.c_str());
	}

	void countForProbabilities(const CImg<unsigned char>& inputImg, double (&probabilities)[3][256]) {
		cout << "countForProbabilities" << endl;
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 256; j++) {
				probabilities[i][j] = 0;
			}
		}
		cimg_forXY(img, x, y) {
			probabilities[0][inputImg(x,y,0,0)]++;
			probabilities[1][inputImg(x,y,0,1)]++;
			probabilities[2][inputImg(x,y,0,2)]++;
		}
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 256; j++) {
				probabilities[i][j]  /= (double) height*width;
			}
		}
	}

	void countForTargetValue(const double (&probabilities)[3][256], int (&targetColorValue)[3][256]) {
		cout << "countForTargetValue" << endl;
		for(int k = 0; k < 3; k++) {
			double sum = 0;
			for(int i = 0; i < 256; i++) {	
				sum += probabilities[k][i];
				targetColorValue[k][i] =(int)(sum * 255);
			}
		}
	}

	CImg<unsigned char> toTargetImg(const CImg<unsigned char>& inputImg, const int (&targetColorValue)[3][256]) {
		cout << "toTargetImg" << endl;
		CImg<unsigned char> targetImg = CImg<unsigned>(width, height, 1, 3);
		cimg_forXY(img, x, y) {
			targetImg(x,y, 0, 0) = targetColorValue[0][inputImg(x,y,0,0)];
			targetImg(x,y, 0, 1) = targetColorValue[1][inputImg(x,y,0,1)];
			targetImg(x,y, 0, 2) = targetColorValue[2][inputImg(x,y,0,2)];
		}
		return targetImg;
	}

};

#endif