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
			//cout << targetGrayValue[i] << " ";
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
		CImg<unsigned char> targetImg = toTargetImg(img, targetColorValue);
		targetImg.display();
		targetImg.save(targetName.c_str());
	}


	void countForProbabilities(const CImg<unsigned char>& inputImg, double (&probabilities)[3][256]) {
		//cout << "countForProbabilities" << endl;
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
		//cout << "countForTargetValue" << endl;
		for(int k = 0; k < 3; k++) {
			double sum = 0;
			for(int i = 0; i < 256; i++) {	
				sum += probabilities[k][i];
				targetColorValue[k][i] =(int)(sum * 255);
			}
		}
	}

	CImg<unsigned char> toTargetImg(const CImg<unsigned char>& inputImg, const int (&targetColorValue)[3][256]) {
		//cout << "toTargetImg" << endl;
		CImg<unsigned char> targetImg = CImg<unsigned>(width, height, 1, 3);
		cimg_forXY(img, x, y) {
			targetImg(x,y, 0, 0) = targetColorValue[0][inputImg(x,y,0,0)];
			targetImg(x,y, 0, 1) = targetColorValue[1][inputImg(x,y,0,1)];
			targetImg(x,y, 0, 2) = targetColorValue[2][inputImg(x,y,0,2)];
		}
		return targetImg;
	}

};


class HistogramEqualizationForColorinHSL {
	CImg<unsigned char> img;
	int width;
	int height;
public:
	HistogramEqualizationForColorinHSL(string filepath, string targetName) {
		CImg<unsigned char> temp(filepath.c_str());
		img = temp;
		width = img.width();
		height = img.height();
		// 将图片从RGB色彩空间转换为HSL色彩空间
		CImg<double> labImg = RGBtoHSL(img);
		labImg.display();
		double probabilities[3][256];
		// 计算每个通道各个数值出现的概率
		countForProbabilities(labImg, probabilities);
		int targetColorValue[3][256];
		// 根据累计概率分布计算每个值对应的值
		countForTargetValue(probabilities, targetColorValue);
		CImg<unsigned char> targetImg = toTargetImg(labImg, targetColorValue);
		targetImg.display();
		CImg<unsigned char> resultRGB = HSLtoRGB(targetImg);
		resultRGB.display();
		string name = targetName + " in HSL.bmp";
		resultRGB.save(name.c_str());
	}

	CImg<unsigned char> RGBtoHSL(const CImg<unsigned char> & inputImg) {
		CImg<unsigned char> hsl = CImg<unsigned char> (width, height, 1, 3);
		int count = 0;
		cimg_forXY(img, x, y) {
			double r =(double)inputImg(x,y,0,0)/255;
			double g =(double)inputImg(x,y,0,1)/255;
			double b =(double)inputImg(x,y,0,2)/255;

			double max = 0, min = 2;
			double s = 0;
			double h = 0;
			int maxChannel = 0; // 1-> r, 2->g, 3->b
			if(r > max) {
				max = r;
				maxChannel = 1;
			}
			if(r < min) {
				min = r;
			}
			if(g > max) {
				max = g;
				maxChannel = 2;
			}
			if(g < min) {
				min = g;
			}
			if(b > max) {
				max = b;
				maxChannel = 3;
			}
			if(b < min) {
				min = b;
			}
			double l = (max + min)/2.0;
			if(max - min != 0) {
				if(l < 0.5) {
					s = (max-min)/(max+min);
				} else {
					s = (max-min)/(2.0-max-min);
				}
				if(maxChannel == 1) {
					h = (g-b)/(max-min);
				} else if(maxChannel == 2) {
					h = 2+(b-r)/(max-min);
				} else if(maxChannel == 3) {
					h = 4+(r-g)/(max-min);
				}
				if(h<0) {
					h += 360;
				}
			}
			hsl(x,y,0,0) = (int)(h * 255 / 360);
			hsl(x,y,0,1) = (int)(s * 255);
			hsl(x,y,0,2) = (int)(l * 255);
			if(hsl(x,y,0,1) == 0) {
				
			}
		}
		//cout << "-------------" << count << endl;
		return hsl;
	}

	CImg<unsigned char> HSLtoRGB(const CImg<unsigned char> & inputImg) {
		int count = 0;
		CImg<unsigned char> rgbImg = CImg<unsigned char> (width, height, 1, 3);
		cimg_forXY(img, x, y) {
			double h =(double)inputImg(x,y,0,0)/255;
			double s =(double)inputImg(x,y,0,1)/255;
			double l =(double)inputImg(x,y,0,2)/255;
			double r = 0, g = 0, b = 0;
			double temp1 = 0, temp2 = 0;
			double temp3[3];
			if(inputImg(x,y,0,1) == 0) {
				r = l;
				g = l;
				b = l;
				count++;
			} else {
				if(l < 0.5) {
					temp2 = l*(1+s);
				} else {
					temp2 = l+s-l*s;
				}
				temp1 = 2*l - temp2;
				temp3[0] = h + 1.0/3;
				temp3[1] = h;
				temp3[2] = h - 1.0/3;
				for(int i = 0; i < 3; i++) {
					if(temp3[i] < 0) {
						temp3[i] += 1;
					}
					else if(temp3[i] > 1) {
						temp3[i] -= 1;
					}
					if(6*temp3[i] < 1) {
						temp3[i] = temp1 + (temp2 - temp1)*6.0*temp3[i];
					} else if(2*temp3[i] < 1) {
						temp3[i] = temp2;
					} else if(3*temp3[i] < 2) {
						temp3[i] = temp1 + (temp2 - temp1) * (2.0/3 - temp3[i])*6.0;
					} else {
						temp3[i] = temp1;
					}
				}
				r = temp3[0];
				g = temp3[1];
				b = temp3[2];
			}
			if((int)(r * 255) > 255) {
				rgbImg(x,y,0,0) = 255;
			} else if((int)(r * 255) < 0) {
				rgbImg(x,y,0,0) = 0;
			 
			} else {
				rgbImg(x,y,0,0) = (int)(r * 255);
			}

			if((int)(g * 255) > 255) {
				rgbImg(x,y,0,1) = 255;
			} else if((int)(g * 255) < 0) {
				rgbImg(x,y,0,1) = 0;
			 
			} else {
				rgbImg(x,y,0,1) = (int)(g * 255);
			}

			if((int)(b * 255) > 255) {
				rgbImg(x,y,0,2) = 255;
			} else if((int)(b * 255) < 0) {
				rgbImg(x,y,0,2) = 0;
			 
			} else {
				rgbImg(x,y,0,2) = (int)(b * 255);
			}
		}
		//cout << "-------------" << count << endl;
		return rgbImg;
	}
	void countForProbabilities(const CImg<double>& inputImg, double (&probabilities)[3][256]) {
		// 初始化
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 256; j++) {
				probabilities[i][j] = 0;
			}
		}
		cimg_forXY(img, x, y) {
			probabilities[0][(int)inputImg(x,y,0,0)]++;
			probabilities[1][(int)inputImg(x,y,0,1)]++;
			probabilities[2][(int)inputImg(x,y,0,2)]++;
		}
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 256; j++) {
				probabilities[i][j]  /= (double) height*width;
			}
		}
	}

	void countForTargetValue(const double (&probabilities)[3][256], int (&targetColorValue)[3][256]) {
		for(int k = 0; k < 3; k++) {
			double sum = 0;
			for(int i = 0; i < 256; i++) {	
				sum += probabilities[k][i];
				targetColorValue[k][i] =(int)(sum * 255);
			}
		}
	}

	CImg<unsigned char> toTargetImg(const CImg<unsigned char>& inputImg, const int (&targetColorValue)[3][256]) {
		//cout << "toTargetImg" << endl;
		CImg<unsigned char> targetImg = CImg<unsigned char>(width, height, 1, 3);
		cimg_forXY(img, x, y) {
			targetImg(x,y, 0, 0) = targetColorValue[0][inputImg(x,y,0,0)];
			targetImg(x,y, 0, 1) = targetColorValue[1][inputImg(x,y,0,1)];
			targetImg(x,y, 0, 2) = inputImg(x,y,0,2);
		}
		return targetImg;
	}
};

class HistogramEqualizationForColorinYCbCr {
	CImg<unsigned char> img;
	int width;
	int height;
public:
	HistogramEqualizationForColorinYCbCr(string filepath, string targetName) {
		CImg<unsigned char> temp(filepath.c_str());
		img = temp;
		width = img.width();
		height = img.height();
		// 将图片从RGB色彩空间转换为HSL色彩空间
		img.RGBtoYCbCr();
		img.display();
		double probabilities[3][256];
		// 计算每个通道各个数值出现的概率
		countForProbabilities(img, probabilities);
		int targetColorValue[3][256];
		// 根据累计概率分布计算每个值对应的值
		countForTargetValue(probabilities, targetColorValue);
		CImg<unsigned char> targetImg = toTargetImg(img, targetColorValue);
		targetImg.YCbCrtoRGB();
		targetImg.display();
		string name = targetName + " in HSL.bmp";
		targetImg.save(name.c_str());
	}

	void countForProbabilities(const CImg<double>& inputImg, double (&probabilities)[3][256]) {
		// 初始化
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 256; j++) {
				probabilities[i][j] = 0;
			}
		}
		cimg_forXY(img, x, y) {
			probabilities[0][(int)inputImg(x,y,0,0)]++;
			probabilities[1][(int)inputImg(x,y,0,1)]++;
			probabilities[2][(int)inputImg(x,y,0,2)]++;
		}
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 256; j++) {
				probabilities[i][j]  /= (double) height*width;
			}
		}
	}

	void countForTargetValue(const double (&probabilities)[3][256], int (&targetColorValue)[3][256]) {
		for(int k = 0; k < 3; k++) {
			double sum = 0;
			for(int i = 0; i < 256; i++) {	
				sum += probabilities[k][i];
				targetColorValue[k][i] =(int)(sum * 255);
			}
		}
	}

	CImg<unsigned char> toTargetImg(const CImg<unsigned char>& inputImg, const int (&targetColorValue)[3][256]) {
		//cout << "toTargetImg" << endl;
		CImg<unsigned char> targetImg = CImg<unsigned char>(width, height, 1, 3);
		cimg_forXY(img, x, y) {
			targetImg(x,y, 0, 0) = targetColorValue[0][inputImg(x,y,0,0)];
			targetImg(x,y, 0, 1) = inputImg(x,y,0,1);
			targetImg(x,y, 0, 2) = inputImg(x,y,0,2);
		}
		return targetImg;
	}
};

#endif