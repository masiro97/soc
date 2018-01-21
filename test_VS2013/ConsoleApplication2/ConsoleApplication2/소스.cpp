#include <opencv\cxcore.h>
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv2\imgproc\imgproc.hpp>
#include "FAST.h"

int main()
{
	CvCapture* capture = cvCaptureFromCAM(0); //현재 인식된 웹캠 찾기
	//cvNamedWindow("BlauCV_Test", 0); // 화면을 그려줄 윈도우를 생성
	//cvResizeWindow("BlauCV_Test", 640, 480); // 사이즈를 조절한다.

	IplImage* CamImg = 0;
	IplImage* IGray = cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 1);
	IplImage* ImgOut = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);

	while (1) {
		cvGrabFrame(capture);
		CamImg = cvRetrieveFrame(capture); // 현재 인식된 장면을 받아오고image에 넣는다.

		cvCvtColor(CamImg, IGray, CV_RGB2GRAY);

		FAST(IGray, ImgOut);

		cvShowImage("CAM", CamImg); // image에 있는 장면을 윈도우에 그린다.
		cvShowImage("ImgOUt", ImgOut); // 
		if (cvWaitKey(10) >= 0) // 이게 가장 중요한데 이 WaitKey 함수가 없으면 아무 것도 안그린다.
			break;
	}

	cvReleaseCapture(&capture); // 할당받았던 웹캠을 해제하고,
	cvDestroyWindow("OpenCvCamtest"); // 윈도우를 종료한다. 

	return 0;
}