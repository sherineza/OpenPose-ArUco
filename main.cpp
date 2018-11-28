
#include "pch.h"
#include <opencv2/dnn.hpp>
#include<chrono>
#include<random>
#include<set>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
using namespace std;
using namespace cv;
using namespace cv::dnn;

//-------------------------------------------------------------Function declarations-------------------------------------------------------------//
//markers
Mat open_Camera();
int create_Board();
void create_Board_Charuco(String pathout);
int calibrate_camera_charuco();
int detect_markers(String video_path);
void create_marker(String marker_path, int nb_markers);
//pose
int multipose();
int openpose();
int detect_pose(String video_path);
//----------------------------------------------------- Executable function + general variables ----------------------------------------------------------------
//Pick one of the functions to be executed in the main script
//#define RECORD_DEMO
//#define CALIBRATE_CAMERA
#define TEST_ARUCO
//#define TEST_OPENPOSE

char key='0';
int x = 1;
//------------------------------------------------------------- Main---------------------------------------------------------------------------//
int main() {

	// Initialisations
	
	
#ifdef CALIBRATE_CAMERA;
	// first, print out the board created by create_Board_Charuco() if needed
	bool generate_board = true;
	cout << endl << "Do you need to generate a Charuco board for calibration? y/n" << endl;
		if (generate_board) {
			String board_path = "C:/opencv/opencv_contrib-master/modules/aruco/samples/board.jpg";
			create_Board_Charuco(board_path);
			cout << endl << "The board has been outputed to " << board_path << ". Press any key to continue after printing the board. " << endl;
			waitKey();
		}
	//second, run the calibration function
	calibrate_camera_charuco();
#endif // CALIBRATE_CAMERA

#ifdef TEST_ARUCO
	// Determine if test is run on demonstration video or live video
	bool run_live =  true;
	bool generate_markers = false;

	if (generate_markers) {
		String marker_path="C:/opencv/opencv_contrib-master/modules/aruco/samples/";
		cout << " How many markers do you need to generate? " << endl;
		cin >> x;
		create_marker(marker_path, x);
		
		cout << endl << "The markers has been outputed to " << marker_path;// << ". Press any key to continue after printing them. " << endl;
	}
	if (run_live==false){	
		String video_path = "outcpp.avi";
		detect_markers(video_path);
	}
	else { 
		detect_markers(""); 
	}
#endif // TEST_ARUCO

#ifdef TEST_OPENPOSE
	bool run_live = true;
	if (run_live == false) {
		String video_path = "outcpp.avi";
		detect_pose(video_path);
	}
	else {
		detect_pose("");
	}
#endif // TEST_OPENPOSE

#ifdef RECORD_DEMO
//In this section, we record a video and log the information from the robot in a file.
	//create the folder and initialise  
	VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened()) {
		cout << "Error opening video stream or file" << endl;
		return -1;
	}
	int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
	// start recording
	//cout << "Press any key to start recording. " << endl;
	cout << "Recording Started" << endl;
	VideoWriter video("outcpp.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(frame_width, frame_height));
	cout << "Press a to exit. " << endl;
	while (1) {
		Mat frame;
		cap >> frame;
		if (frame.empty()) { break; }
		video.write(frame);
		imshow("Frame", frame);
		char c =(char)waitKey(1);
		if (c == 'a') 
			break;
	}
	cap.release();
	video.release();
	destroyAllWindows();
#endif // RECORD_DEMO
	return 2;
}
