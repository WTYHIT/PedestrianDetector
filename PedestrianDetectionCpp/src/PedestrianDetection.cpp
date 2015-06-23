//============================================================================
// Name        : PedestrianDetection.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

// Setting up: C++11
// http://stackoverflow.com/questions/9131763/eclipse-cdt-c11-c0x-support
// Setting up opencv:
// http://docs.opencv.org/doc/tutorials/introduction/linux_eclipse/linux_eclipse.html
// Matio is required to read and write mat-files.
// http://sourceforge.const double PI  =3.141592653589793238463;net/projects/matio/

#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include "PedestrianDetector.hpp"
#include <dirent.h>
#include <string.h>
#include <math.h>
#include <iomanip>

using namespace std;
using namespace cv;

void makeOwnConvImplementation() {
	int imageSize = 100;
	int kernelSize = 3;
	Mat inputImage(imageSize,imageSize,CV_32FC1);
	Mat outputImage(imageSize,imageSize,CV_32FC1);
	Mat outputImage2(imageSize,imageSize,CV_32FC1);
	Mat kernelFilter(kernelSize,kernelSize,CV_32FC1);
	Mat kernelConv(kernelSize,kernelSize,CV_32FC1);

	clock_t start, end;


	bool showData = 0;

	cv::randn(inputImage,  0, 1);
	cv::randn(kernelFilter,  0, 1);

	flip(kernelFilter,kernelConv,-1);
	Point anchor(kernelFilter.cols - kernelFilter.cols/2 -1, kernelFilter.rows - kernelFilter.rows/2 -1);
	int borderMode = BORDER_CONSTANT;

	//	cout << inputImage << endl;
	//	float* data = (float*)inputImage.data;
	//	for(int iiCol = 0; iiCol < imageSize; iiCol++) {
	//		for(int iRow = 0; iRow < imageSize; iRow++) {
	//			cout << data[iRow + iiCol*inputImage.rows] << ", ";
	//		}
	//		cout << endl;
	//	}

	start = clock();
	filter2D(inputImage,outputImage,inputImage.depth(),kernelFilter,anchor, 0,borderMode);
	end = clock();
	double t1 = (double) (end-start);
	cout << "\n t1:" <<  t1/ CLOCKS_PER_SEC * 1000.0 << " ms\n";

	if(showData)
		cout << outputImage << endl;




	int nKR = kernelConv.rows;
	int nKC = kernelConv.cols;
	int nIR = inputImage.rows;
	int nIC = inputImage.cols;
	int shiftKernelIndexR = (nKR-1)/2;
	int shiftKernelIndexC = (nKC-1)/2;
	int cRowIndex;
	int cColIndex;
	int cColIndex2;
	int cColIndex3;
	int iImageC = 0;
	int iImage = 0;
	float* pInputImage = (float*)inputImage.data;
	float* pOutputImage2 = (float*)outputImage2.data;
	float* pKernel = (float*) kernelConv.data;
	start = clock();
	int loopupKernelRow[nKR];
	for(int n = 0; n < nKR; n++) {
		loopupKernelRow[n] = -n+shiftKernelIndexR;
	}

	int loopupKernelCol[nKC];
	for(int n = 0; n < nKC; n++) {
		loopupKernelCol[n] = -n+shiftKernelIndexC;
		cout << loopupKernelCol[n] << endl;
	}

	for(int iiCol = 0; iiCol < nIC; iiCol++) {
		iImageC = iiCol*outputImage2.rows;
		for(int iRow = 0; iRow < nIR; iRow++) {
			iImage = iRow + iiCol*outputImage2.rows;

			for(int iiiiKernelCol = 0; iiiiKernelCol < nKC; iiiiKernelCol++) {
				cColIndex = iiCol+loopupKernelCol[iiiiKernelCol];
				cColIndex2 = cColIndex*inputImage.rows;
				cColIndex3 = iiiiKernelCol*kernelConv.rows;
				//cColIndex = iiCol-iiiiKernelCol+shiftKernelIndexC;
				for(int iiiKernelRow = 0; iiiKernelRow < nKR; iiiKernelRow++) {

					cRowIndex = iRow+loopupKernelRow[iiiKernelRow];
					//cRowIndex = iRow-iiiKernelRow+shiftKernelIndexR;
					//if(cRowIndex>=0 && cRowIndex<nIR && cColIndex>=0 && cColIndex<nIC) {
					//outputImage2.at<float>(iRow,iiCol) = outputImage2.at<float>(iRow,iiCol)+ kernelConv.at<float>(iiiKernelRow,iiiiKernelCol)*inputImage.at<float>(cRowIndex,cColIndex);
					//cout << outputImage2.at<float>(iRow,iiCol) << ", " << endl;
					//pOutputImage2[iRow + iiCol*outputImage2.rows]= pOutputImage2[iRow + iiCol*outputImage2.rows] + pKernel[iiiKernelRow + iiiiKernelCol*kernelConv.rows]*pInputImage[cRowIndex + cColIndex*inputImage.rows];
					pOutputImage2[iImage]= pOutputImage2[iImage] + pKernel[iiiKernelRow +cColIndex3]*pInputImage[cRowIndex + cColIndex2];
					//}
				}
			}
		}
	}

	end = clock();
	double t2 = (double) (end-start);
	cout << "\n t1:" << t2 / CLOCKS_PER_SEC * 1000.0 << " ms\n";
	cout << "ratio:" << t1/t2 << endl;;

	if(showData)
		cout << outputImage2 << endl;
}
void my2DConv() {

}
void showBB(vector<bbType> bbs, Mat image, bool wait){
	double alpha = 0.3;
	double threshold = 50;
	for(int iBbs = 0; iBbs < bbs.size(); iBbs++) {
		//printf("n%d, x: %f, %f, %f, %f, %f, Dist: %f \n",iBbs,bbs[iBbs].x1,bbs[iBbs].y2, bbs[iBbs].width3, bbs[iBbs].height4, bbs[iBbs].score5, bbs[iBbs].distance );

		Scalar useColor(0,0,0);
		if(bbs[iBbs].score5 > threshold) {
			alpha = 0.3;
			useColor[1] = 0; // G
			useColor[2] = 255; // R
		}
		else {
			alpha = 0.1;
			useColor[1] = 255; // G
			useColor[2] = 0; // R
		}
		Mat rectangleImage(image.size[0],image.size[1], CV_8UC3, cv::Scalar(0, 0,0));
		rectangle(rectangleImage, Rect(bbs[iBbs].x1,bbs[iBbs].y2,bbs[iBbs].width3, bbs[iBbs].height4), useColor, CV_FILLED, 8, 0 );

		stringstream strsScore; strsScore.precision(3);
		strsScore << bbs[iBbs].score5;
		putText(image, strsScore.str() + "p", Point(bbs[iBbs].x1,bbs[iBbs].y2), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(255,255,255)); //, int thickness=1, int lineType=8, bool bottomLeftOrigin=false

		stringstream strsDistance; strsDistance.precision(3);
		strsDistance << bbs[iBbs].distance;
		putText(image, strsDistance.str() + "m", Point(bbs[iBbs].x1,bbs[iBbs].y2+bbs[iBbs].height4), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(255,255,255)); //, int thickness=1, int lineType=8, bool bottomLeftOrigin=false

		stringstream strsAngle; strsAngle.precision(3);
		strsAngle << bbs[iBbs].angle;
		putText(image, "Angle" + strsAngle.str(), Point(bbs[iBbs].x1,bbs[iBbs].y2+bbs[iBbs].height4 +25), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(255,255,255)); //, int thickness=1, int lineType=8, bool bottomLeftOrigin=false

		//putText(image, to_string((int)(round(bbs[iBbs].score5))), Point(bbs[iBbs].x1,bbs[iBbs].y2), FONT_HERSHEY_SIMPLEX, 1, Scalar(255,255,255)); //, int thickness=1, int lineType=8, bool bottomLeftOrigin=false
		//putText(image, "d:" + to_string((int)(round(bbs[iBbs].distance))), Point(bbs[iBbs].x1,bbs[iBbs].y2+bbs[iBbs].height4), FONT_HERSHEY_SIMPLEX, 1, Scalar(255,255,255)); //, int thickness=1, int lineType=8, bool bottomLeftOrigin=false
		addWeighted(rectangleImage, alpha, image, 1, 0.0, image );
	}

	if(wait)
		namedWindow("BoundingsBox", CV_WINDOW_AUTOSIZE);
	imshow("BoundingsBox",image);

	//printf("Code is done!!");



}
int main(int argc, char** argv) {


	string dirImage = "pedmodels/InriaTest.png";
	//makeOwnConvImplementation();

	vector<bbType> bbs;
	double FOV_verticalDeg = 47; // Vertical field-of-view of camera.
	double FOV_horizontalDeg = 50; //83;
	double angleTiltDegrees = 7; // Downward tilt in degrees.
	double cameraHeight = 1.9; // Height Position of camera.

	double imageResize = 0.75;
	//Mat image = imread(dirImage, 1);

	Mat image, inputImage;
	struct dirent *ent;
	//string dirImages = "/home/pistol/Desktop/DataFolder/2014-11-03-14-37-11/WebCam";
	//string dirImages = "/home/pistol/Desktop/DataFolder/2014-10-16-12-18-30/WebCam";
	string dirImages = "/home/pistol/Desktop/DataFolder/WebCam";

	DIR *dir = opendir(dirImages.data());

	char * pch;


	//image = imread(dirImage, 1);

	//cout << image.cols << "x" << image.rows << endl;

	// Making detector object.
	bool fastDetector = 0;
	string dirDetector;
	if(fastDetector) {
		dirDetector = "pedmodels/AcfInriaDetector.mat";
	}
	else {
		dirDetector = "pedmodels/LdcfInriaDetector.mat";
	}

	PedestrianDetector oPedDetector(dirDetector);
	// Providing camera settings.
	oPedDetector.setCameraSetup(FOV_verticalDeg, FOV_horizontalDeg, angleTiltDegrees, cameraHeight);


	bool useExample = true;
	if(useExample) {
		inputImage = imread(dirImage, 1);
		if (!inputImage.data) {
			printf("No image data \n");
			return -1;
		}
		resize(inputImage, image, Size(), imageResize, imageResize);

		clock_t start, end;
		start = clock();
		bbs = oPedDetector.pedDetector(image);
		end = clock();
		double time = (double) (end-start) / CLOCKS_PER_SEC * 1000.0;
		cout << "\n t1:" << time << " ms\n";

		showBB(bbs, image,0);
		waitKey(0);

	}

//	bool useWholeFolder = true;
//	if(useWholeFolder) {
//		struct dirent **namelist;
//		int i;
//		string fileName;
//		//string pch;
//		int n = scandir(dirImages.data(), &namelist, 0, alphasort);
//		if (n < 0)
//			perror("scandir");
//		else {
//			for (i = 200; i < n; i++) {
//				printf("%s\n", namelist[i]->d_name);
//
//
//				//while ((ent = readdir (dir)) != NULL) {
//				fileName = namelist[i]->d_name;
//				string pch=strrchr(namelist[i]->d_name,'.');
//				free(namelist[i]);
//				if(pch.compare(".jpg") == 0)
//				{
//					//printf ("%s\n", fileName.data());
//					string dirImage = (dirImages+ '/' +fileName);
//					inputImage = imread(dirImage.data(), 1);
//					resize(inputImage, image, Size(), imageResize,imageResize);
//
//					clock_t start, end;
//					start = clock();
//					bbs = oPedDetector.pedDetector(image);
//					end = clock();
//					double time = (double) (end-start) / CLOCKS_PER_SEC * 1000.0;
//					cout << "\n t1:" << time << " ms\n";
//					showBB(bbs, image,0);
//					waitKey(0);
//					waitKey(10);
//
//				}
//				//closedir (dir);
//			}
//		}
//		free(namelist);
//	}
	return 0;
}

