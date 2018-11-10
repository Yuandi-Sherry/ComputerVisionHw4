#include "ColorTransfer.h"
#include <iostream>
using namespace std;

int main() {
	for(int i = 1; i <= 6; i++) {
		string refName = "ref";
		refName += to_string(i) + ".bmp";
		string opeName = "ope";
		opeName += to_string(i) + ".bmp";
		ColorTransfer colorTransfer(refName, opeName, to_string(i));
	}
	//HistogramEqualizationForGray histogramEqualizationForGray("Lena.bmp", "LenaGrayHE.bmp");
	//HistogramEqualizationForColor histogramEqualizationForColor("Lena.bmp", "LenaColorHE.bmp");
	
}