#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
void MotionDetect(); // Function to Capture Video from Cam
void SaveVideo();
int main()
{
	MotionDetect();
	exit(0);
}
void MotionDetect()
{
	Mat ref_frame, frame, difference, gray;
	int count = 5; // Holds assumed frame count
	VideoCapture camera(0);
	// Sets width and height of window to 512 and 288 respectively to enable faster processing of video
	camera.set(3, 512);
	camera.set(4, 288);
	// If Camera failed to open exit the function
	if (!camera.isOpened())
	{
		cout << "Could not capture Video.\n";
		exit(1);
	}
	//Reads first frame
	camera.read(ref_frame);
	cvtColor(ref_frame, ref_frame, COLOR_BGR2GRAY);
	GaussianBlur(ref_frame, ref_frame, Size(5, 5), 0);
	//Reads subsequent frames
	while (camera.read(frame))
	{
		//imshow("Original Video", frame);
		//Detects motion by comparing current frame with reference frame and store result in difference
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		GaussianBlur(gray, gray, Size(5, 5), 0);
		absdiff(ref_frame, gray, difference);
		threshold(difference, difference, 25, 255, THRESH_BINARY);
		dilate(difference, difference, Mat(), Point(-1, -1), 2);
		imshow("Difference", difference);
		// Difference frame will be black unless there is motion.
		if (countNonZero(difference))
		{
			camera.release();
			cout << "\nMotion Detected\n";
			destroyWindow("Difference");
			SaveVideo();
		}
		// Reinitialise reference frame once every 5 frames
		if (count % 5 == 0)
			gray.copyTo(ref_frame);
		count++;
		// Stop when escape key is pressed
		if (waitKey(1) == 27)
			break;
	}
	camera.release();
	destroyWindow("Difference");
	exit(0);
}
void SaveVideo()
{
	VideoCapture cam(0);
	int frameno = cam.get(CAP_PROP_POS_FRAMES);
	int maxframeno = frameno + 150;
	if (!cam.isOpened())
	{
		cout << "Error opening video stream" << endl;
		exit(-1);
	}
	int frame_width = cam.get(CAP_PROP_FRAME_WIDTH);
	int frame_height = cam.get(CAP_PROP_FRAME_HEIGHT);
	VideoWriter video("Video.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 30,Size(frame_width, frame_height));
	while (frameno < maxframeno)
	{
		Mat frame;
		cam >> frame;
		if (frame.empty())
			break;
		video.write(frame);
		imshow("Save", frame);
		frameno++;
		if (waitKey(1) == 27) {
			video.release();
			cam.release();
			destroyWindow("Save");
			exit(0);
		}
	}
	video.release();
	cam.release();
	destroyWindow("Save");
	MotionDetect();
}