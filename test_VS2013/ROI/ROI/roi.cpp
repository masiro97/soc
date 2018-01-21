#include <opencv\cv.h>           //영상처리 관련 라이브러리를 사용하기 위한 헤더

#include <opencv\highgui.h>      //영상을 캡쳐 받고 윈도우로 보여주기 위해 사용하는 헤더




void main()

{

	IplImage* image = 0;                           //구조체(?) 이 포인터로 이미지를 받고 이 포인터를 이용해서 이미지를 처리함
	image = cvLoadImage("jolly1.jpg", 1);

	IplImage* view = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,3 );

	int x=140 ,
		y=23 ,
		width=115 ,
		height=142 ;

	CvRect Rec;
	Rec = cvRect(x,y,width,height);
	view = image;

	cvDrawRect(view, cvPoint(x, y), cvPoint(x + width, y + height), cvScalar(0, 255, 0));

	do{
		cvShowImage("ROI Image", view);

		if (cvWaitKey(10) >= 0)

			break;

	

	} while (1);
}

