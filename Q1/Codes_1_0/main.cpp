#include "HistogramEqualization.h"
#include <iostream>
using namespace std;

int main() {
	//HistogramEqualizationForGray histogramEqualizationForGray("Lena.bmp", "LenaGrayHE.bmp");
	HistogramEqualizationForColor histogramEqualizationForColor("Lena.bmp", "LenaColorHE.bmp");
}