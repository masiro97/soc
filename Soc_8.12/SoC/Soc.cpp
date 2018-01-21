#include <opencv\cv.h>        
#include <opencv\highgui.h>   
#include "BlauImage.h"
#include "MyImageFunc.h"
#include "functions.h"
#include <iostream>
using namespace std;
#define Size (120 * 180)

int main() {

	IplImage *img = cvLoadImage("c:\\Test5.bmp");

	int nWidth = img->width;
	int nHeight = img->height;
	BlauPoint p[Size] = { 0.,0. };

	int i = 0;
	double N[7] = { 0. };
	for (int y = 0; y < nHeight; ++y){
		for (int x = 0; x < nWidth; ++x){

			CvScalar v = cvGet2D(img, y, x);
			if (v.val[0] == 255 && v.val[1] == 255 && v.val[2] == 255){
				
				//cout << "point : " << "( " << x << " , " << y << " )" << " ";
				//cout << " B:" << v.val[0];
				//cout << " G:" << v.val[1];
				//cout << " R:" << v.val[2];
				//cout << endl;
					p[i].x = x;
					p[i].y = y;
					i++;
				
			}

		}
	}

	NInvariantMoments(p, N,i);
	for (int j = 0; j < 7; j++){
		cout << "NMoments : " << abs(log(abs(N[j]))) << endl;
	}
	
	//cout << num;

	cvNamedWindow("Test", 1);
	cvShowImage("Test", img);
	cvWaitKey(0);

	cvReleaseImage(&img);
	cvDestroyWindow("Test");

	
	return 0;
}


/*void exSize(BlauPoint points, int Psize)
{
	BlauPoint* newpoints = new BlauPoint[2 * Psize];

	copy(points, points + Psize - 1, newpoints);
	Psize *= 2;
	delete[] points;
	points = newpoints;
}
*/

/*int main(){
	ByteImage imgSrc(180, 120, 3);
	//imgSrc.SaveImage("imgSrc.bmp");
	//imgSrc.CreateImage(180,120);
	//ByteImage imgDst(imgSrc);
	//int nWidth = imgSrc.Width;
	//int nHeight = imgSrc.Height;

	while (1){
		Erosion(imgSrc, imgDst);
		if (imgDst.GetPixels() == 0)
			break;
	}

	return 0;
}*/

/*int main(){


	// 블롭들의 유사성 판단

	// 비슷한 블롭들의 집단 만들고

	//그 집단블롭에서 중심점 구하고

	//중심점들의 기울기(절댓값 씌운거) 비교해서 가로 줄무늬와 세로 줄무늬 판별

}*/
