#include "HistogramEqualization.h"
//#include "ColorTransfer.h"
#include <iostream>
using namespace std;

int main() {
	for(int i  = 1; i < 6; i++) {
		string filename = "test";
		filename = filename + to_string(i) + ".bmp";
		string target = to_string(i);
		HistogramEqualizationForGray histogramEqualizationForGray(filename, target);
		HistogramEqualizationForColorinHSL histogramEqualizationForColorinHSL(filename, target);
		HistogramEqualizationForColorinYCbCr histogramEqualizationForColorinYCbCr(filename, target);
		HistogramEqualizationForColor histogramEqualizationForColor(filename, target);
	}
}