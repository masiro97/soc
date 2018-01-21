#include <opencv/cv.h>         // 영상 처리를 위한 header. 헤더 전에 opencv/ 꼭 써주자.

#include <opencv/highgui.h>  // 카메라로 영상을 입력받거나 이미지를 읽어들이고 화면에 보여주기 위한 header 



void main()

{

	IplImage* image = 0;  //이미지 구조체IplImage 

	CvCapture* capture = cvCaptureFromCAM(0);  // 0 번째 연결된 카메라로부터 컨트롤 얻기

	cvNamedWindow("T9-camera", 0);            // T9-camera 라는 이름의 윈도우를 생성, 0 은 고정된 윈도우를 생성 

	cvResizeWindow("T9-camera", 320, 240); // T9-camera 라는 이름의 윈도우 크기를 조절 width = 320, height = 240 



	while (1) {  //카메라로부터 매 프레임 받아오려고 while 무한루프돌림 

		cvGrabFrame(capture);  //카메라로부터 한 프레임 잡기 

		image = cvRetrieveFrame(capture);    // 잡은 프레임으로부터 IplImage 형 구조를 리턴 받아 image 에 넣기 

		cvShowImage("T9-camera", image);   // "T9-camera" 윈도우에 image 를 보여주기 

		if (cvWaitKey(10) >= 0) //esc 키가 눌러지면 종료 

			break;

	}



	cvReleaseCapture(&capture);  //capture를 release하여 카메라와의 연결 종료 

	cvDestroyWindow("T9-camera");  //윈도우 소멸 

}


