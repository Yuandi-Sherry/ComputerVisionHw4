#ifndef COLORTRANSFER_H
#define COLORTRANSFER_H
#include <iostream>
#include <cmath>
#include "CImg.h"
using namespace std;
using namespace cimg_library;

class ColorTransfer {
	CImg<unsigned char> refImg; // 参考图像
	CImg<unsigned char> opeImg; // 待处理图像

public:
	ColorTransfer(string refFilePath, string opeFilePath) {
		CImg<unsigned char> temp1(refFilePath.c_str());
		refImg = temp1;
		CImg<unsigned char> temp2(opeFilePath.c_str());
		opeImg = temp2;
		CImg<double> refLab = RGBtoLab(refImg);
		CImg<double> opeLab = RGBtoLab(opeImg);
		opeLab.display();
		//cout << "debug1" << endl;
		double refMean[3], refStdVar[3], opeMean[3], opeStdVar[3];
		calculateMeanAndStdV(refLab, refMean, refStdVar);
		calculateMeanAndStdV(opeLab, opeMean, opeStdVar);
		//cout << "debug2" << endl;
		CImg<double> opeLabStar = subMean(opeLab, opeMean);
		opeLabStar.display();
		CImg<double> opeLabPrime = scaleDataPoint(opeLabStar, refStdVar, opeStdVar);
		opeLabStar.display();
		CImg<double> opeLabResult = addMean(opeLabPrime, refMean);
		opeLabResult.display();
		//cout << "debug3" << endl;
		CImg<unsigned char> result = LabToRGB(opeLabResult);
		result.display();
	}

	CImg<double> RGBtoLab(const CImg<unsigned char>& inputImg) {
		CImg<double> LMS = CImg<double>(inputImg.width(), inputImg.height(), 1, 3);
		cimg_forXY(inputImg, x, y) {
			LMS(x,y,0,0) = log10(0.3811 * inputImg(x,y,0,0) + 0.5783 * inputImg(x,y,0,1) + 0.0402 * inputImg(x,y,0,2));
			LMS(x,y,0,1) = log10(0.1967 * inputImg(x,y,0,0) + 0.7244 * inputImg(x,y,0,1) + 0.0782 * inputImg(x,y,0,2));
			LMS(x,y,0,2) = log10(0.0241 * inputImg(x,y,0,0) + 0.1228 * inputImg(x,y,0,1) + 0.8444 * inputImg(x,y,0,2));
		}

		CImg<double> lab = CImg<double>(inputImg.width(), inputImg.height(), 1, 3);
		cimg_forXY(LMS, x, y) {
			lab(x,y,0,0) = (double)1/sqrt(3) * (LMS(x,y,0,0) +LMS(x,y,0,1) + LMS(x,y,0,2));
			lab(x,y,0,1) = (double)1/sqrt(6) * (LMS(x,y,0,0) +LMS(x,y,0,1) + (-2) * LMS(x,y,0,2));
			lab(x,y,0,2) = (double)1/sqrt(2) * (LMS(x,y,0,0) +(-1)*LMS(x,y,0,1));
		}
		return lab;
	}

	CImg<unsigned char> LabToRGB(const CImg<double>& inputImg) {
		CImg<double> LMS = CImg<double>(inputImg.width(), inputImg.height(), 1, 3);
		cimg_forXY(inputImg, x, y) {
			LMS(x,y,0,0) = pow(10,1/sqrt(3) * inputImg(x,y,0,0) + 1/sqrt(6) * inputImg(x,y,0,1) + 1/sqrt(2) * inputImg(x,y,0,2));
			LMS(x,y,0,1) = pow(10, 1/sqrt(3) * inputImg(x,y,0,0) + 1/sqrt(6) * inputImg(x,y,0,1) + 1/sqrt(2) * inputImg(x,y,0,2) * (-1));
			LMS(x,y,0,2) = pow(10, 1/sqrt(3) * inputImg(x,y,0,0) + (-2) * 1/sqrt(6) * inputImg(x,y,0,1));
		}

		CImg<unsigned char> RGBimg = CImg<unsigned char>(inputImg.width(), inputImg.height(), 1, 3);
		int count = 0;
		cimg_forXY(LMS, x, y) {
			if( 4.4679 * LMS(x,y,0,0) + (-1) * 3.5873 * LMS(x,y,0,1) + 0.1193 *  LMS(x,y,0,2) > 255) {
				RGBimg(x,y,0,0) = 255; 
				count++;
			} else if( 4.4679 * LMS(x,y,0,0) + (-1) * 3.5873 * LMS(x,y,0,1) + 0.1193 *  LMS(x,y,0,2) < 0) {
				RGBimg(x,y,0,0) = 0; 
				count++;
			} else {
				RGBimg(x,y,0,0) = 4.4679 * LMS(x,y,0,0) + (-1) * 3.5873 * LMS(x,y,0,1) + 0.1193 *  LMS(x,y,0,2); 
			}
			if( (-1) * 1.2186 * LMS(x,y,0,0) + 2.3809 * LMS(x,y,0,1) + (-1) * 0.1624 *  LMS(x,y,0,2) > 255) {
				RGBimg(x,y,0,1) = 255; 
				count++;
			} else if( (-1) * 1.2186 * LMS(x,y,0,0) + 2.3809 * LMS(x,y,0,1) + (-1) * 0.1624 *  LMS(x,y,0,2) < 0) {
				RGBimg(x,y,0,1) = 0; 
				count++;
			} else {
				RGBimg(x,y,0,1) = (-1) * 1.2186 * LMS(x,y,0,0) + 2.3809 * LMS(x,y,0,1) + (-1) * 0.1624 *  LMS(x,y,0,2); 
			}
			if( 0.0497 * LMS(x,y,0,0) + (-1) * 0.2439 * LMS(x,y,0,1) + 1.2045 *  LMS(x,y,0,2) > 255) {
				RGBimg(x,y,0,2) = 255; 
				count++;
			} else if( 0.0497 * LMS(x,y,0,0) + (-1) * 0.2439 * LMS(x,y,0,1) + 1.2045 *  LMS(x,y,0,2) < 0) {
				RGBimg(x,y,0,2) = 0; 
				count++;
			} else {
				RGBimg(x,y,0,2) = 0.0497 * LMS(x,y,0,0) + (-1) * 0.2439 * LMS(x,y,0,1) + 1.2045 *  LMS(x,y,0,2); 
			}
		}

		cout << count << endl;
		return RGBimg;
	}

	void calculateMeanAndStdV(const CImg<double>& lab, double* mean, double* stdVar) {
		//lab.display();
		for(int i = 0; i < 3; i++) {
			mean[i] = 0;
			stdVar[i] = 0;
		}
		cimg_forXY(lab, x, y) {
			mean[0] += lab(x,y,0,0)/(lab.height()*lab.width());
			mean[1] += lab(x,y,0,1)/(lab.height()*lab.width());
			mean[2] += lab(x,y,0,2)/(lab.height()*lab.width());
		}
		cimg_forXY(lab, x, y) {
			stdVar[0] += pow(lab(x,y,0,0) - mean[0], 2)/(lab.height()*lab.width());
			stdVar[1] += pow(lab(x,y,0,1) - mean[1], 2)/(lab.height()*lab.width());
			stdVar[2] += pow(lab(x,y,0,2) - mean[2], 2)/(lab.height()*lab.width());
		}
		stdVar[0] = sqrt(stdVar[0]);
		stdVar[1] = sqrt(stdVar[1]);
		stdVar[2] = sqrt(stdVar[2]);
	}


	CImg<double> subMean(const CImg<double>& lab, const double* mean) {
		CImg<double> labStar = CImg<double> (lab.width(), lab.height(), 1, 3);
		cimg_forXY(lab, x, y) {
			labStar(x,y, 0 , 0) = lab(x,y,0,0) - mean[0];
			labStar(x,y, 0 , 1) = lab(x,y,0,1) - mean[1];
			labStar(x,y, 0 , 2) = lab(x,y,0,2) - mean[2];
		}
		return labStar;
	}

	CImg<double> scaleDataPoint(const CImg<double>& labStar, const double* refStdVar, const double* opeStdVar) {
		CImg<double> labPrime = CImg<double> (labStar.width(), labStar.height(), 1, 3);
		cimg_forXY(labStar, x, y) {
			labPrime(x,y, 0 , 0) = labStar(x,y,0,0)*refStdVar[0]/opeStdVar[0];
			labPrime(x,y, 0 , 1) = labStar(x,y,0,1)*refStdVar[1]/opeStdVar[1];
			labPrime(x,y, 0 , 2) = labStar(x,y,0,2)*refStdVar[2]/opeStdVar[2];
		}
		return labPrime;
	}

	CImg<double> addMean(const CImg<double>& lab, const double* mean) {
		CImg<double> labResult = CImg<double> (lab.width(), lab.height(), 1, 3);
		cimg_forXY(lab, x, y) {
			labResult(x,y, 0 , 0) = lab(x,y,0,0) + mean[0];
			labResult(x,y, 0 , 1) = lab(x,y,0,1) + mean[1];
			labResult(x,y, 0 , 2) = lab(x,y,0,2) + mean[2];
		}
		return labResult;
	}
};
#endif