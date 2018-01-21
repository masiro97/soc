// test_cv.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <opencv\cv.h>           //영상처리 관련 라이브러리를 사용하기 위한 헤더
#include <opencv\highgui.h>      //영상을 캡쳐 받고 윈도우로 보여주기 위해 사용하는 헤더

	void main()

	{

		IplImage* image = 0;                           //구조체(?) 이 포인터로 이미지를 받고 이 포인터를 이용해서 이미지를 처리함

		CvCapture* capture = cvCaptureFromCAM(0);              //카메라로부터 컨트롤(?)을 얻어옴. (0번 카메라연결?)

		cvNamedWindow("rabbit", 0);                 //윈도우 생성. 숫자는 윈도우의 크기를 나타냄 0:고정된 윈도우, 1:자동 조절 윈도우

		cvResizeWindow("rabbit", 320, 240);        //윈도우의 크기를 조정할 수 있음



		while (1) {                                       //카메라로 부터 매 프레임을 받아오기 위해서 무한루프를 돌린다.

			cvGrabFrame(capture);                  //입력된 영상의 한 프레임을 잡고,

			image = cvRetrieveFrame(capture);    //잡은 프레임으로부터 이미지를 생성


			cvShowImage("rabbit", image);        //생성된 이미지를 윈도우에 보여줌


			if (cvWaitKey(10) >= 0)

				break;

		}


		cvReleaseCapture(&capture);             //카메라와의 연결 종료

		cvDestroyWindow("rabbit");               //윈도우 종료

	}

	




