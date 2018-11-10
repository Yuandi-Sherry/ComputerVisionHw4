#include "HistogramEqualization.h"
//#include "ColorTransfer.h"
#include <iostream>
using namespace std;

int main() {
	//HistogramEqualizationForGray histogramEqualizationForGray("Lena.bmp", "LenaGrayHE.bmp");
	HistogramEqualizationForColorinYCbCr histogramEqualizationForColorinHSL("Lena.bmp", "Lena_L_");
	//ColorTransfer colorTransfer("4.bmp", "3.bmp");
}