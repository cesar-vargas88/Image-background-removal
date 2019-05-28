// Background removal.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <opencv2\core\core.hpp>
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"

using namespace cv;
using namespace std;

int main()
{
	Mat img_source;
	Mat img_cropped;
	Mat img_contrast1;
	Mat img_contrast2;
	Mat img_contrast3;
	Mat img_contrast4;
	Mat img_gray1;
	Mat img_gray2;
	Mat img_gray3;
	Mat img_gray4;
	Mat img_contrast;

	int imageSize = 1280;
	string sourcePath = "C:/Imagenes/Originales/";
	string processedPath = "C:/Imagenes/Procesadas/";
	vector<String> fileNames;
	string currentFilePath;
	string fileNameComplete;
	string fileName;
	string fileExtension;
	int fileNumber = 1;
	int enter;

	cout << "Coloca las imagenes que se van aprocesar en..." + sourcePath << endl;
	cout << "Las imagenes procesadas se van a guardar en..." + processedPath << endl;
	cout << "Ingresa 1 y presiona enter para comenzar..." << endl;
	cin >> enter;

	cout << "Cargando imagenes..." << endl;
	glob(sourcePath, fileNames);

	while (fileNames.size() > 0)
	{
		img_source = imread(fileNames.back());

		currentFilePath = fileNames[fileNames.size() - 1];
		fileNameComplete = currentFilePath.substr(sourcePath.size(), fileNames[fileNames.size() - 1].size() - sourcePath.size());

		cout << fileNumber <<  " - " << fileNameComplete + " en proceso..." << endl;

		if (fileNameComplete.find(".") != -1)
		{
			fileName = fileNameComplete.substr(0, fileNameComplete.find("."));
			fileExtension = fileNameComplete.substr(fileNameComplete.find("."), fileNameComplete.size() - fileNameComplete.find("."));

			if (!img_source.data)
			{
				cout << "Could not open or find the image" << std::endl;
				return -1;
			}

			// Crop image
			if (img_source.cols > img_source.rows)
			{
				Rect myROI((img_source.cols - img_source.rows) / 2, 0, img_source.rows, img_source.rows);
				img_cropped = img_source(myROI);
			}
			else if (img_source.cols < img_source.rows)
			{
				Rect myROI(0, (img_source.rows - img_source.cols) / 2, img_source.cols, img_source.cols);
				img_cropped = img_source(myROI);
			}
			else
				img_cropped = img_source.clone();

			resize(img_cropped, img_cropped, Size(imageSize, imageSize));

			// Get contranst image
			int windowsSize = 10;
			Scalar background;
			float contrast = 1.2;
			float bright = -15;

			//img_cropped.convertTo(img_contrast, -1, contrast, bright);

			do
			{
				img_cropped.convertTo(img_contrast, -1, contrast, bright);

				Rect myROI_1(0, 0, windowsSize, windowsSize);
				Rect myROI_2(0, img_contrast.rows - windowsSize, windowsSize, windowsSize);
				Rect myROI_3(img_contrast.rows - windowsSize, img_contrast.rows - windowsSize, windowsSize, windowsSize);
				Rect myROI_4(img_contrast.rows - windowsSize, 0, windowsSize, windowsSize);

				cvtColor(img_contrast(myROI_1), img_gray1, COLOR_RGB2GRAY);
				cvtColor(img_contrast(myROI_2), img_gray2, COLOR_RGB2GRAY);
				cvtColor(img_contrast(myROI_3), img_gray3, COLOR_RGB2GRAY);
				cvtColor(img_contrast(myROI_4), img_gray4, COLOR_RGB2GRAY);

				GaussianBlur(img_gray1, img_contrast1, Size(9, 9), 1);
				GaussianBlur(img_gray2, img_contrast2, Size(9, 9), 1);
				GaussianBlur(img_gray3, img_contrast3, Size(9, 9), 1);
				GaussianBlur(img_gray4, img_contrast4, Size(9, 9), 1);

				contrast += 0.02;
			} while ((img_contrast1.data[0] + img_contrast2.data[0] + img_contrast3.data[0] + img_contrast4.data[0]) / 4 < 254);

			// Show images
			//imshow("Source", img_source);
			//imshow("Contrast", img_contrast);	

			//imwrite("Images/White_6.jpg", img_contrast);
			imwrite(processedPath + fileName + fileExtension, img_contrast);

			fileNames.pop_back();
		}
		else
		{
			fileNames.pop_back();
			cout << fileNumber << " - " << fileNameComplete + " error en el proceso..." << endl;
		}
			
		cout << fileNumber << " - " << fileNameComplete + " procesada..." << endl;

		fileNumber++;
	}

	cout <<  "Proceso completado..." << endl;

	waitKey(0);
	return 0;
}


/*
// Crop Image
Rect myROI((source.cols - source.rows / 2) / 2, source.rows / 4, source.rows / 2, source.rows / 2);
cropped = source(myROI);
// Resize Image
resize(cropped, resized, Size(1280, 1280));
// Gray Image
cvtColor(resized, gray, COLOR_RGB2GRAY);

//
//GaussianBlur(gray, gausianBlur, Size(3, 3), 1);
//Canny(gausianBlur, canny, 0, 100, 3);
//threshold(gausianBlur, thresh, 160, 255, THRESH_BINARY);

imshow("Cany", canny);
imshow("Blur", gausianBlur);
imshow("Threshold", thresh);
*/