//============================================================================
// Name        : PedestrianDetection.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include "PedestrianDetector.hpp"


using namespace std;
using namespace cv;

int main(int argc, char** argv) {

	Mat image = imread(argv[1], 1);

	if (argc != 2 || !image.data) {
		printf("No image data \n");
		return -1;
	}

	PedestrianDetector oPedDetector("/home/pistol/MatlabCode/piotr_toolbox/toolbox/detector/models/AcfInriaDetector.mat");
	clock_t start, end;
	start = clock();
	vector<bbType> bbs = oPedDetector.pedDetector(image);
	end = clock();
	double time = (double) (end-start) / CLOCKS_PER_SEC * 1000.0;
	cout << "\n t1:" << time << " ms\n";

	for(int iBbs = 0; iBbs < bbs.size(); iBbs++) {
		printf("n%d, x: %f, %f, %f, %f, %f \n",iBbs,bbs[iBbs].x1,bbs[iBbs].y2, bbs[iBbs].width3, bbs[iBbs].height4, bbs[iBbs].score5 );
		rectangle(image, Rect(bbs[iBbs].x1,bbs[iBbs].y2,bbs[iBbs].width3, bbs[iBbs].height4), Scalar(0,0,255,125), 2, 8, 0 );
		putText(image, to_string((int)(round(bbs[iBbs].score5))), Point(bbs[iBbs].x1,bbs[iBbs].y2), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0)); //, int thickness=1, int lineType=8, bool bottomLeftOrigin=false
	}

	//namedWindow("BoundingsBox", CV_WINDOW_AUTOSIZE);
	//imshow("BoundingsBox",image);

	printf("Code is done!!");

	waitKey(0);

	return 0;
}
