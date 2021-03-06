#include "pch.h"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/core.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/calib3d.hpp>

using namespace std;
using namespace cv;
namespace {
	const char* about = "Create an ArUco grid board image";
	const char* keys =
		"{@outfile |<none> | Output image }"
		"{w        |       | Number of markers in X direction }"
		"{h        |       | Number of markers in Y direction }"
		"{l        |       | Marker side length (in pixels) }"
		"{s        |       | Separation between two consecutive markers in the grid (in pixels)}"
		"{d        |       | dictionary: DICT_4X4_50=0, DICT_4X4_100=1, DICT_4X4_250=2,"
		"DICT_4X4_1000=3, DICT_5X5_50=4, DICT_5X5_100=5, DICT_5X5_250=6, DICT_5X5_1000=7, "
		"DICT_6X6_50=8, DICT_6X6_100=9, DICT_6X6_250=10, DICT_6X6_1000=11, DICT_7X7_50=12,"
		"DICT_7X7_100=13, DICT_7X7_250=14, DICT_7X7_1000=15, DICT_ARUCO_ORIGINAL = 16}"
		"{m        |       | Margins size (in pixels). Default is marker separation (-s) }"
		"{bb       | 1     | Number of bits in marker borders }"
		"{si       | false | show generated image }";
}

// -------------------------------------------------Other Functions---------------------------------------------------------------------------

static bool readDetectorParameters(string filename, Ptr<aruco::DetectorParameters> &params) {
	FileStorage fs(filename, FileStorage::READ);
	if (!fs.isOpened())
		return false;
	fs["adaptiveThreshWinSizeMin"] >> params->adaptiveThreshWinSizeMin;
	fs["adaptiveThreshWinSizeMax"] >> params->adaptiveThreshWinSizeMax;
	fs["adaptiveThreshWinSizeStep"] >> params->adaptiveThreshWinSizeStep;
	fs["adaptiveThreshConstant"] >> params->adaptiveThreshConstant;
	fs["minMarkerPerimeterRate"] >> params->minMarkerPerimeterRate;
	fs["maxMarkerPerimeterRate"] >> params->maxMarkerPerimeterRate;
	fs["polygonalApproxAccuracyRate"] >> params->polygonalApproxAccuracyRate;
	fs["minCornerDistanceRate"] >> params->minCornerDistanceRate;
	fs["minDistanceToBorder"] >> params->minDistanceToBorder;
	fs["minMarkerDistanceRate"] >> params->minMarkerDistanceRate;
	fs["cornerRefinementMethod"] >> params->cornerRefinementMethod;
	fs["cornerRefinementWinSize"] >> params->cornerRefinementWinSize;
	fs["cornerRefinementMaxIterations"] >> params->cornerRefinementMaxIterations;
	fs["cornerRefinementMinAccuracy"] >> params->cornerRefinementMinAccuracy;
	fs["markerBorderBits"] >> params->markerBorderBits;
	fs["perspectiveRemovePixelPerCell"] >> params->perspectiveRemovePixelPerCell;
	fs["perspectiveRemoveIgnoredMarginPerCell"] >> params->perspectiveRemoveIgnoredMarginPerCell;
	fs["maxErroneousBitsInBorderRate"] >> params->maxErroneousBitsInBorderRate;
	fs["minOtsuStdDev"] >> params->minOtsuStdDev;
	fs["errorCorrectionRate"] >> params->errorCorrectionRate;
	return true;
}

static bool saveCameraParams(const string &filename, Size imageSize, float aspectRatio, int flags,
	const Mat &cameraMatrix, const Mat &distCoeffs, double totalAvgErr) {
	FileStorage fs(filename, FileStorage::WRITE);
	if (!fs.isOpened())
		return false;

	time_t tt =time(0);
	struct tm t1;
	localtime_s(&t1, &tt);
	//struct tm *t2 = localtime(&tt);
	struct tm *t2 = &t1;
	char buf[1024];
	strftime(buf, sizeof(buf) - 1, "%c", t2);

	fs << "calibration_time" << buf;

	fs << "image_width" << imageSize.width;
	fs << "image_height" << imageSize.height;

	if (flags & CALIB_FIX_ASPECT_RATIO) fs << "aspectRatio" << aspectRatio;

	if (flags != 0) {
		sprintf_s(buf, "flags: %s%s%s%s",
			flags & CALIB_USE_INTRINSIC_GUESS ? "+use_intrinsic_guess" : "",
			flags & CALIB_FIX_ASPECT_RATIO ? "+fix_aspectRatio" : "",
			flags & CALIB_FIX_PRINCIPAL_POINT ? "+fix_principal_point" : "",
			flags & CALIB_ZERO_TANGENT_DIST ? "+zero_tangent_dist" : "");
	}

	fs << "flags" << flags;

	fs << "camera_matrix" << cameraMatrix;
	fs << "distortion_coefficients" << distCoeffs;

	fs << "avg_reprojection_error" << totalAvgErr;

	return true;
}

Mat open_Camera()
{
	char key;
    // initialisations
	// sampling rate
	// sensors active/inactive

	//Camera input
	bool showCameraInput = false;
	bool cameraRGBD = true;
	if (cameraRGBD)
	{
		bool recordDepth = true;
	}
	
	// Opening Camera
	Mat frame;
	VideoCapture cap;
	cap.open(0);
	/* for advanced usage 
	int deviceID = 0; //default camera
	int apiID = CAP_ANY;
	cap.open(deviceID + apiID);
	*/
	if (!cap.isOpened()) {
		cerr << "error:cam not opened. \n";
		return frame;
	}

	//Getting frame from Camera
	for (;;) {
		cap.read(frame);
		if (frame.empty()) {
			cerr << "error: blank frame.\n";
			break;
		}
		imshow("Webcam 1", frame);
		key= waitKey(30);
		if (key == 'a') {
			break;
		}
	}
	
	// Calibrating Camera
	bool needCalibration = true;
	if (needCalibration == true) {
	

	}

	std::cout << "Hello World!\n"; 
	return frame;
}

int create_Board() {
	
	int markersX = 5;
	int markersY = 5; 
	int markerLength = 100;
	int markerSeparation = 10; 
	int dictionaryId = 10; 
	int margins = markerSeparation;
	int borderBits = 5; 
	bool showImage = true; 

	String out = "C:/opencv/opencv_contrib-master/modules/aruco/samples/board.jpg";

	Size imageSize;
	imageSize.width = markersX * (markerLength + markerSeparation) - markerSeparation + 2 * margins;
	imageSize.height =
		markersY * (markerLength + markerSeparation) - markerSeparation + 2 * margins;

	Ptr<aruco::Dictionary> dictionary =
		aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(dictionaryId));

	Ptr<aruco::GridBoard> board = aruco::GridBoard::create(markersX, markersY, float(markerLength),
		float(markerSeparation), dictionary);

	// show created board
	Mat boardImage;
	board->draw(imageSize, boardImage, margins, borderBits);

	if (showImage) {
		imshow("board", boardImage);
		waitKey(0);
	}

	imwrite(out, boardImage);

	return 0;
}

static bool readCameraParameters(string filename, Mat &camMatrix, Mat &distCoeffs) {
	FileStorage fs(filename, FileStorage::READ);
	if (!fs.isOpened())
		return false;
	fs["camera_matrix"] >> camMatrix;
	fs["distortion_coefficients"] >> distCoeffs;
	return true;
}

//--------------------------------------------------CharUco BOARD INFORMATION + Calibration------------------------------------------------------------
int squaresX = 5;
int squaresY = 5;
int squareLength = 101;
int markerLength = 70;
int dictionaryId = 3;
int margins = squareLength - markerLength;
int borderBits = 1;

void create_Board_Charuco(String pathout) {

	bool showImage = true;
	String out = pathout;
	Ptr<aruco::Dictionary> dictionary =
		aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(dictionaryId));

	Size imageSize;
	imageSize.width = squaresX * squareLength + 2 * margins;
	imageSize.height = squaresY * squareLength + 2 * margins;

	Ptr<aruco::CharucoBoard> board = aruco::CharucoBoard::create(squaresX, squaresY, (float)squareLength,
		(float)markerLength, dictionary);

	// show created board
	Mat boardImage;
	board->draw(imageSize, boardImage, margins, borderBits);

	if (showImage) {
		imshow("board", boardImage);
		waitKey(0);
	}

	imwrite(out, boardImage);

	return;
}
int calibrate_camera_charuco() {

	int squaresX = 5;
	int squaresY = 5;
	int squareLength = 101;
	int markerLength = 70;
	int dictionaryId = 3;
	int margins = squareLength - markerLength;

	int borderBits = 1;
	bool showImage = true;

	String outputFile = "C:/opencv/opencv_contrib-master/modules/aruco/samples/param.txt";

	bool showChessboardCorners = true;

	int calibrationFlags = 0;
	float aspectRatio = 1;

	if (false) calibrationFlags |= CALIB_ZERO_TANGENT_DIST;
	if (false) calibrationFlags |= CALIB_FIX_PRINCIPAL_POINT;

	Ptr<aruco::DetectorParameters> detectorParams = aruco::DetectorParameters::create();

	bool refindStrategy = true;
	int camId = 0;
	String video;

	VideoCapture inputVideo;
	int waitTime;
	if (!video.empty()) {
		inputVideo.open(video);
		waitTime = 0;
	}
	else {
		inputVideo.open(camId);
		waitTime = 10;
	}

	Ptr<aruco::Dictionary> dictionary =
		aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(dictionaryId));

	// create charuco board object
	Ptr<aruco::CharucoBoard> charucoboard =
		aruco::CharucoBoard::create(squaresX, squaresY, squareLength, markerLength, dictionary);
	Ptr<aruco::Board> board = charucoboard.staticCast<aruco::Board>();

	// collect data from each frame
	vector< vector< vector< Point2f > > > allCorners;
	vector< vector< int > > allIds;
	vector< Mat > allImgs;
	Size imgSize;

	while (inputVideo.grab()) {
		Mat image, imageCopy;
		inputVideo.retrieve(image);

		vector< int > ids;
		vector< vector< Point2f > > corners, rejected;

		// detect markers
		aruco::detectMarkers(image, dictionary, corners, ids, detectorParams, rejected);

		// refind strategy to detect more markers
		if (refindStrategy) aruco::refineDetectedMarkers(image, board, corners, ids, rejected);

		// interpolate charuco corners
		Mat currentCharucoCorners, currentCharucoIds;
		if (ids.size() > 0)
			aruco::interpolateCornersCharuco(corners, ids, image, charucoboard, currentCharucoCorners,
				currentCharucoIds);

		// draw results
		image.copyTo(imageCopy);
		if (ids.size() > 0) aruco::drawDetectedMarkers(imageCopy, corners);

		if (currentCharucoCorners.total() > 0)
			aruco::drawDetectedCornersCharuco(imageCopy, currentCharucoCorners, currentCharucoIds);

		putText(imageCopy, "Press 'c' to add current frame. 'ESC' to finish and calibrate",
			Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 2);

		imshow("out", imageCopy);
		char key = (char)waitKey(waitTime);
		if (key == 27) break;
		if (key == 'c' && ids.size() > 0) {
			cout << "Frame captured" << endl;
			allCorners.push_back(corners);
			allIds.push_back(ids);
			allImgs.push_back(image);
			imgSize = image.size();
		}
	}

	if (allIds.size() < 1) {
		cerr << "Not enough captures for calibration" << endl;
		return 0;
	}

	Mat cameraMatrix, distCoeffs;
	vector< Mat > rvecs, tvecs;
	double repError;

	if (calibrationFlags & CALIB_FIX_ASPECT_RATIO) {
		cameraMatrix = Mat::eye(3, 3, CV_64F);
		cameraMatrix.at< double >(0, 0) = aspectRatio;
	}

	// prepare data for calibration
	vector< vector< Point2f > > allCornersConcatenated;
	vector< int > allIdsConcatenated;
	vector< int > markerCounterPerFrame;
	markerCounterPerFrame.reserve(allCorners.size());
	for (unsigned int i = 0; i < allCorners.size(); i++) {
		markerCounterPerFrame.push_back((int)allCorners[i].size());
		for (unsigned int j = 0; j < allCorners[i].size(); j++) {
			allCornersConcatenated.push_back(allCorners[i][j]);
			allIdsConcatenated.push_back(allIds[i][j]);
		}
	}

	// calibrate camera using aruco markers
	double arucoRepErr;
	arucoRepErr = aruco::calibrateCameraAruco(allCornersConcatenated, allIdsConcatenated,
		markerCounterPerFrame, board, imgSize, cameraMatrix,
		distCoeffs, noArray(), noArray(), calibrationFlags);

	// prepare data for charuco calibration
	int nFrames = (int)allCorners.size();
	vector< Mat > allCharucoCorners;
	vector< Mat > allCharucoIds;
	vector< Mat > filteredImages;
	allCharucoCorners.reserve(nFrames);
	allCharucoIds.reserve(nFrames);

	for (int i = 0; i < nFrames; i++) {
		// interpolate using camera parameters
		Mat currentCharucoCorners, currentCharucoIds;
		aruco::interpolateCornersCharuco(allCorners[i], allIds[i], allImgs[i], charucoboard,
			currentCharucoCorners, currentCharucoIds, cameraMatrix,
			distCoeffs);

		allCharucoCorners.push_back(currentCharucoCorners);
		allCharucoIds.push_back(currentCharucoIds);
		filteredImages.push_back(allImgs[i]);
	}

	if (allCharucoCorners.size() < 4) {
		cerr << "Not enough corners for calibration" << endl;
		return 0;
	}

	// calibrate camera using charuco
	repError =
		aruco::calibrateCameraCharuco(allCharucoCorners, allCharucoIds, charucoboard, imgSize,
			cameraMatrix, distCoeffs, rvecs, tvecs, calibrationFlags);

	bool saveOk = saveCameraParams(outputFile, imgSize, aspectRatio, calibrationFlags,
		cameraMatrix, distCoeffs, repError);
	if (!saveOk) {
		cerr << "Cannot save output file" << endl;
		return 0;
	}

	cout << "Rep Error: " << repError << endl;
	cout << "Rep Error Aruco: " << arucoRepErr << endl;
	cout << "Calibration saved to " << outputFile << endl;

	// show interpolated charuco corners for debugging
	if (showChessboardCorners) {
		for (unsigned int frame = 0; frame < filteredImages.size(); frame++) {
			Mat imageCopy = filteredImages[frame].clone();
			if (allIds[frame].size() > 0) {

				if (allCharucoCorners[frame].total() > 0) {
					aruco::drawDetectedCornersCharuco(imageCopy, allCharucoCorners[frame],
						allCharucoIds[frame]);
				}
			}

			imshow("out", imageCopy);
			char key = (char)waitKey(0);
			if (key == 27) break;
		}
	}

	return 0;
}

//--------------------------------------------------Detect Markers------------------------------------------------------------------------------
int marker_dictionaryId = 0;
void create_marker(String marker_path, int nb_markers) {
	dictionaryId = marker_dictionaryId;
	int borderBits = 1;
	int markerSize = 400;
	bool showImage = true;	
	String out = marker_path;
	String file_path;
	Ptr<aruco::Dictionary> dictionary =
		aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(dictionaryId));
	Mat markerImg;
	for (int markerId = 0; markerId < nb_markers; markerId++) {
		cout << " marker " << markerId;
		aruco::drawMarker(dictionary, markerId, markerSize, markerImg, borderBits);
		file_path = out + "marker" + to_string(markerId) + ".jpg";
		cout << file_path;
		imwrite(file_path, markerImg);
		if (showImage) {
			imshow("marker", markerImg);
			waitKey(1000);
		}
	}


	return;
}
int detect_markers(String video_path) {

	dictionaryId = marker_dictionaryId;
	bool showRejected = true;
	bool estimatePose = true;
	float markerLength = 400;
	Ptr<aruco::DetectorParameters> detectorParams = aruco::DetectorParameters::create();

	std::cout << "Corner refinement method (0: None, 1: Subpixel, 2:contour, 3: AprilTag 2): " << detectorParams->cornerRefinementMethod << std::endl;

	int camId = 0;
	String video=video_path;

	Ptr<aruco::Dictionary> dictionary =
		aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(dictionaryId));

	Mat camMatrix, distCoeffs;
	String filename = "C:/opencv/opencv_contrib-master/modules/aruco/samples/param.txt";
	if (estimatePose) {
		bool readOk = readCameraParameters(filename, camMatrix, distCoeffs);
		if (!readOk) {
			cerr << "Invalid camera file. Try running the calibration module first." << endl;
			return 0;
		}
	}

	VideoCapture inputVideo;
	int waitTime;
	if (!video.empty()) {
		inputVideo.open(video);
		waitTime = 10;
	}
	else {
		inputVideo.open(camId);
		waitTime = 10;
	}

	double totalTime = 0;
	int totalIterations = 0;
	int frame_width = inputVideo.get(cv::CAP_PROP_FRAME_WIDTH);
	int frame_height = inputVideo.get(cv::CAP_PROP_FRAME_HEIGHT);
	VideoWriter outputvid("outAruco.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(frame_width, frame_height));
	while (inputVideo.grab()) {
		Mat image, imageCopy;
		inputVideo.read(image);
		
		double tick = (double)getTickCount();

		vector< int > ids;
		vector< vector< Point2f > > corners, rejected;
		vector< Vec3d > rvecs, tvecs;

		// detect markers and estimate pose
		aruco::detectMarkers(image, dictionary, corners, ids, detectorParams, rejected);
		if (estimatePose && ids.size() > 0)
			aruco::estimatePoseSingleMarkers(corners, markerLength, camMatrix, distCoeffs, rvecs,
				tvecs);

		double currentTime = ((double)getTickCount() - tick) / getTickFrequency();
		totalTime += currentTime;
		totalIterations++;
		if (totalIterations % 30 == 0) {
			cout << "Detection Time = " << currentTime * 1000 << " ms "
				<< "(Mean = " << 1000 * totalTime / double(totalIterations) << " ms)" << endl;
		}

		// draw results
		image.copyTo(imageCopy);
		if (ids.size() > 0) {
			aruco::drawDetectedMarkers(imageCopy, corners, ids);

			if (estimatePose) {
				for (unsigned int i = 0; i < ids.size(); i++)
					aruco::drawAxis(imageCopy, camMatrix, distCoeffs, rvecs[i], tvecs[i],
						markerLength * 0.5f);
			}
		}

		if (showRejected && rejected.size() > 0)
			aruco::drawDetectedMarkers(imageCopy, rejected, noArray(), Scalar(100, 0, 255));

		imshow("out", imageCopy);
		outputvid.write(imageCopy);
		char key = (char)waitKey(waitTime);
		if (key == 27) break;
	}

	return 0;
}
