#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/video/tracking.hpp>
#include <Windows.h>

using namespace cv;


int main() {
	POINT mousePos;
	mousePos = { 0,0 };

	KalmanFilter KF(4, 2, 0, CV_64F);
	// intialization of KF...
	// KalmanFilter(intdynamParams, intmeasureParams, intcontrolParams=0, inttype=CV_64F)
	// dynamParams: state 변수 수, measureParams: measurement 변수 수, controlParams: 외부입벽변수 수

	float fltTransitionMatrixValues[4][4] = { { 1, 0, 1, 0 }, { 0, 1, 0, 1 },{ 0, 0, 1, 0 },{ 0, 0, 0, 1 } };
	// Transition Matrix 설정

	KF.transitionMatrix = cv::Mat(4, 4, CV_32F, fltTransitionMatrixValues);
	// Kalmanfilter 객체에 Transition Matrix 할당

	Mat_<float> measurement(2, 1);
	measurement.setTo(Scalar(0));

	KF.statePre.at<float>(0) = mousePos.x;
	KF.statePre.at<float>(1) = mousePos.y;
	KF.statePre.at<float>(2) = 0;
	KF.statePre.at<float>(3) = 0;
	// X^-(k) Priori State 초기화

	// declare an array of floats to feed into KalmanFilter Measurement Matrix, also known as Measurement Model
	float fltMeasurementMatrixValues[2][4] = { { 1, 0, 0, 0}, { 0, 1, 0, 0 } }; // MesurementMatrix 설정
	setIdentity(KF.processNoiseCov, Scalar::all(1e-4)); // w(k)의Covariance Q 최기화
	setIdentity(KF.measurementNoiseCov, Scalar::all(10)); // v(k)의Covariance R 초기화
	setIdentity(KF.errorCovPost, Scalar::all(.1)); //P^+(k-1) 에러Coveriance 초기화

	// First predict, to update the internal statePrevariable
	Mat prediction = KF.predict(); //prediction of Kalmanfilter 과정수행
	
	//x^-(k) priori state update
	Point predictPt(prediction.at<float>(0), prediction.at<float>(1));
	
	// Get mouse point, mesauring the mouse point
	GetCursorPos(&mousePos);
	measurement(0) = mousePos.x;
	measurement(1) = mousePos.y;

	// The update phase, correction of Kalmanfilter
	Mat estimated = KF.correct(measurement);
	Point statePt(estimated.at<float>(0), estimated.at<float>(1));
	Point measPt(measurement(0), measurement(1)); //x^+(k) Posteriori state update

	return 0;
}