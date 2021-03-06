// Background removal.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <opencv2\core\core.hpp>
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include <windows.h>

using namespace cv;
using namespace std;

bool removeImagesBackground(string sourcePath, string processedPath, int imageSize);
bool moveImagesToFolder(string sourcePath, string processedPath);
void createDirectory(string directoryPath);
void createLogFile(string path, map<String, int> folderNames);

int main()
{
	int iMenu = 0;
	bool bMenu = true;
	do 
	{
		iMenu = 0;
		bMenu = false;

		cout << "Menu" << endl;
		cout << "1. Editar imagenes" << endl;
		cout << "2. Guardar imagenes editadas en carpetas" << endl;
		cout << "3. Salir" << endl;
		cin >> iMenu;

		createDirectory("C:/Imagenes/");
		createDirectory("C:/Imagenes/Originales/");
		createDirectory("C:/Imagenes/Procesadas/");
		createDirectory("C:/Imagenes/Procesadas en carpeta/");

		switch(iMenu)
		{
			case 1:
				removeImagesBackground("C:/Imagenes/Originales/", "C:/Imagenes/Procesadas/", 1280);
				break;
			case 2:
				moveImagesToFolder("C:/Imagenes/Procesadas/", "C:/Imagenes/Procesadas en carpeta/");
				break;
			case 3:
				break;
			default:
				bMenu = true;
		}
	} 
	while (bMenu);
	
	return 0;
}

bool removeImagesBackground(string sourcePath, string processedPath, int imageSize) 
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

	vector<String> fileNames;
	string currentFilePath;
	string fileNameComplete;
	string fileName;
	string fileExtension;
	int fileNumber = 1;

	cout << "Cargando imagenes..." << endl;
	glob(sourcePath, fileNames);

	while (fileNames.size() > 0)
	{
		img_source = imread(fileNames.back());

		currentFilePath = fileNames[fileNames.size() - 1];
		fileNameComplete = currentFilePath.substr(sourcePath.size(), fileNames[fileNames.size() - 1].size() - sourcePath.size());

		cout << fileNumber << " - " << fileNameComplete + " en proceso..." << endl;

		if (fileNameComplete.find(".") != -1)
		{
			fileName = fileNameComplete.substr(0, fileNameComplete.find("."));
			fileExtension = fileNameComplete.substr(fileNameComplete.find("."), fileNameComplete.size() - fileNameComplete.find("."));

			if (!img_source.data)
			{
				cout << "Could not open or find the image" << std::endl;
				return false;
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

	cout << "Proceso completado..." << endl;

	return true;
}
bool moveImagesToFolder(string sourcePath, string processedPath)
{
	Mat img_source;

	vector<String> fileNames;
	map<String, int> folderNames;

	string currentFilePath;
	string fileNameComplete;
	string folderName;
	string fileName;
	string fileExtension;
	string fileSKU;

	int fileNumber = 1;
	int folderNumber = 1;

	cout << "Cargando imagenes..." << endl;
	glob(sourcePath, fileNames);

	while (fileNames.size() > 0)
	{
		img_source = imread(fileNames.back());

		if (!img_source.data)
		{
			cout << "Could not open or find the image" << std::endl;
			return false;
		}

		currentFilePath = fileNames[fileNames.size() - 1];
		fileNameComplete = currentFilePath.substr(sourcePath.size(), fileNames[fileNames.size() - 1].size() - sourcePath.size());

		if (fileNameComplete.find(".") != -1)
		{
			fileName = fileNameComplete.substr(0, fileNameComplete.find("."));
			fileExtension = fileNameComplete.substr(fileNameComplete.find("."), fileNameComplete.size() - fileNameComplete.find("."));

			if (fileName.size() > 6 && fileName.substr(0, 2) == "TA")
			{
				folderName = fileNameComplete.substr(0, 6);

				auto it = folderNames.find(folderName);

				if (it == folderNames.end())
				{
					folderNames.insert(pair<String, int>(folderName, 1));
					createDirectory(processedPath + folderName);

					cout << folderNumber++ << " - " << folderName + " creado..." << endl;
				}
				else
					it->second++;

				imwrite(processedPath + folderName + "/" + fileName + fileExtension, img_source);
				fileNames.pop_back();
			}
			else
			{
				imwrite(processedPath + fileName + fileExtension, img_source);
				cout << fileNameComplete + " - Es un nombre incorrecto." << endl;
				fileNames.pop_back();
			}
		}
		else
		{
			fileNames.pop_back();
			cout << fileNumber << " - " << " error en el proceso..." << endl;
		}

		cout << fileNumber << " - " << fileNameComplete + " procesada..." << endl;

		fileNumber++;
	}

	cout << "Proceso completado..." << endl;

	createLogFile("C:/Imagenes/Procesadas en carpeta/", folderNames);

	return true;
}
void createDirectory(string directoryPath) 
{
	int slength = (int) directoryPath.length() + 1;
	int len = MultiByteToWideChar(CP_ACP, 0, directoryPath.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, directoryPath.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	
	CreateDirectory(r.c_str(), NULL);
}
void createLogFile(string path, map<String, int> folderNames)
{
	string line;
	ofstream file(path + "log file.txt");

	int index = 1;

	if (file.is_open())
	{
		for (auto it = folderNames.cbegin(); it != folderNames.cend(); ++it)
			file << index++ << " - " << it->first << " - " << it->second << endl;

		file.close();
	}
}