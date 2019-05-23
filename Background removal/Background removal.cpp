// Background removal.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <opencv2\core\core.hpp>
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

int main()
{
	// Get Image

	Mat source;
	Mat cropped;
	Mat resized;
	Mat gray;
	Mat threshold;

	source = imread("Images/TA0024_2.jpg");

	if (!source.data) 
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	// Crop Image

	Rect myROI((source.cols - source.rows) / 2, 0, source.rows, source.rows);
	cropped = source(myROI);

	// Resize Image

	resize(cropped, resized, Size(800, 800));

	// Gray Image

	cvtColor(resized, gray, COLOR_RGB2GRAY);

	
	// Display Images
	//namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Output", gray);

	waitKey(0);
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
