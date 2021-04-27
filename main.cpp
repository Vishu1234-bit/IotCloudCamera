#include <opencv2/opencv.hpp>
#include <C:\Users\Vishali\source\repos\IoTIoT.in\IoT-Cloud-Camera\ZBarWin64\include\zbar.h>
#include <fstream> // For operating in files
#include <stdio.h> // For strings

using namespace cv;
using namespace std;
using namespace zbar;

void getVideo();						//Captures Video from Cam
void decode(Mat& im);					//Decodes info from QR Code
int save(string var);					// Saves decoded information

int main()
{
	getVideo();
	exit(0);
}

void getVideo()								//Capture Video of QR Code
{
	Mat frame;
	VideoCapture camera(0);
	camera.set(3, 512);						// Setswidth and height of window to 512 and 288 respectively
	camera.set(4, 288);						//to enable faster processing of video
	if (!camera.isOpened())					// If Camera failed to open exit the function
	{
		cout << "Could not capture Video.\n";
		exit(1);
	}
	while (camera.read(frame))
	{
		imshow("op", frame);			//Display Video
		decode(frame);
		if (waitKey(1) == 27)			// Stop when escape key is pressed
			break;
	}
	camera.release();					//Release camera resorce
	destroyAllWindows();
}

void decode(Mat& im)
{
	ImageScanner scanner;				//Create zbar scanner
	string var;							// Stores data
	Mat imGray;							// Stores GrayScale image

	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 0);			//Disable all decoders
	scanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1);		//Enable QR decoder

	cvtColor(im, imGray, COLOR_BGR2GRAY);						// Convert image to grayscale
	Image image(im.cols, im.rows, "Y800", (uchar*)imGray.data, im.cols * im.rows); // Wrap image data in a zbar image

	int n = scanner.scan(image);								// Scan the image for QRCodes

	for (Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
	{
		var = symbol->get_data();
		cout << endl << "Data : " << var << endl << endl; // Prints data
		save(var);
	}
}

int save(string var)
{
	ofstream MyFile;
	MyFile.open("wpa_supplicant.conf", ios::out | ios::app); // Appends data to file
	MyFile << var << endl;
	getVideo();
	exit(0);
}