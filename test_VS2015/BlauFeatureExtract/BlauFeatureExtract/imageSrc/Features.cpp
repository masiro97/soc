#include "stdafx.h"
#include "Features.h"
#include "MyImageFunc.h"
#include "BlauImage.h"
#include <math.h>

void DrawLine(ByteImage& canvas, int x1, int y1, int x2, int y2, BYTE val)
{
	ASSERT(canvas.GetChannel()==1);

	int xs, ys, xe, ye;
	if (x1 == x2) // 수직선
	{
		if (y1 < y2) {ys = y1; ye = y2;}
		else		 {ys = y2; ye = y1;}
		for (int r=ys ; r<=ye ; r++)
		{
			canvas.GetAt(x1, r) = val;
		}
		return;
	}

	double a = (double)(y2-y1)/(x2-x1); // 기울기
	int nHeight = canvas.GetHeight();

	if ((a>-1) && (a<1)) // 가로축에 가까움
	{
		if (x1 < x2) {xs = x1; xe = x2; ys = y1; ye = y2;}
		else		 {xs = x2; xe = x1; ys = y2; ye = y1;}
		for (int c=xs ; c<=xe ; c++)
		{
			int r = (int)(a*(c-xs) + ys + 0.5);
			if (r<0 || r>=nHeight)
				continue;
			canvas.GetAt(c, r) = val;
		}
	}
	else // 세로축에 가까움
	{
		double invA = 1.0 / a;
		if (y1 < y2) {ys = y1; ye = y2; xs = x1; xe = x2; }
		else		 {ys = y2; ye = y1; xs = x2; xe = x1; }
		for (int r=ys ; r<=ye ; r++)
		{
			int c = (int)(invA*(r-ys) + xs + 0.5);
			if (r<0 || r>=nHeight)
				continue;
			canvas.GetAt(c, r) = val;
		}
	}
}

void DrawLine(ByteImage& canvas, int x1, int y1, int x2, int y2, BYTE R, BYTE G, BYTE B)
{
	ASSERT(canvas.GetChannel()==3);

	int xs, ys, xe, ye;
	if (x1 == x2) // 수직선
	{
		if (y1 < y2) {ys = y1; ye = y2;}
		else		 {ys = y2; ye = y1;}
		for (int r=ys ; r<=ye ; r++)
		{
			canvas.GetAt(x1, r, 0) = B;
			canvas.GetAt(x1, r, 1) = G;
			canvas.GetAt(x1, r, 2) = R;
		}
		return;
	}

	double a = (double)(y2-y1)/(x2-x1); // 기울기
	int nHeight = canvas.GetHeight();

	if ((a>-1) && (a<1)) // 가로축에 가까움
	{
		if (x1 < x2) {xs = x1; xe = x2; ys = y1; ye = y2;}
		else		 {xs = x2; xe = x1; ys = y2; ye = y1;}
		for (int c=xs ; c<=xe ; c++)
		{
			int r = (int)(a*(c-xs) + ys + 0.5);
			if (r<0 || r>=nHeight)
				continue;
			canvas.GetAt(c, r, 0) = B;
			canvas.GetAt(c, r, 1) = G;
			canvas.GetAt(c, r, 2) = R;
		}
	}
	else // 세로축에 가까움
	{
		double invA = 1.0 / a;
		if (y1 < y2) {ys = y1; ye = y2; xs = x1; xe = x2; }
		else		 {ys = y2; ye = y1; xs = x2; xe = x1; }
		for (int r=ys ; r<=ye ; r++)
		{
			int c = (int)(invA*(r-ys) + xs + 0.5);
			if (r<0 || r>=nHeight)
				continue;
			canvas.GetAt(c, r, 0) = B;
			canvas.GetAt(c, r, 1) = G;
			canvas.GetAt(c, r, 2) = R;
		}
	}
}

int HoughLines(const ByteImage& imageIn, int nTNum, int nTVal, double resTheta, int numLine, double* pRho, double* pTheta)
{
	int nWidth	= imageIn.GetWidth();
	int nHeight = imageIn.GetHeight();
	int nWStep	= imageIn.GetWStep();

	int numRhoH = (int)(sqrt((double)(nWidth*nWidth + nHeight*nHeight))); // 영상 대각선의 길이
	int numRho = numRhoH*2; // rho의 음수 영역을 위해 2를 곱함
	int numThe = 180 / resTheta;
	int numTrans = numRho*numThe; // rho와 theta 조합의 출현 횟수를 저장하는 공간

	double* sinLUT = new double[numThe]; // sin 함수 룩업 테이블
	double* cosLUT = new double[numThe]; // cos 함수 룩업 테이블
	double toRad = M_PI/numThe;
	for (int theta=0 ; theta<numThe ; theta++)
	{
		sinLUT[theta] = (double)sin(theta*toRad);
		cosLUT[theta] = (double)cos(theta*toRad);
	}

	int* pCntTrans = new int[numTrans];
	memset(pCntTrans, 0, numTrans*sizeof(int));

	for (int r=0 ; r<nHeight ; r++)
	{
		BYTE* pIn = imageIn.GetPtr(r);
		for (int c=0 ; c<nWidth ; c++)
		{
			if (pIn[c] > nTVal) // 경계선 픽셀
			{
				for (int theta=0 ; theta<numThe ; theta++) 
				{
					int rho = (int)(c*sinLUT[theta] + r*cosLUT[theta] + numRhoH + 0.5);
					pCntTrans[rho*numThe+theta]++;
				}
			}
		}
	}

	// nThreshold을넘는 결과 저장
	int nLine = 0;
	for (int i=0 ; i<numTrans && nLine<numLine ; i++)
	{
		if (pCntTrans[i] > nTNum)
		{
			pRho[nLine]	  = (int)(i/numThe); // rho의 인덱스
			pTheta[nLine] = (i - pRho[nLine]*numThe)*resTheta; //theta 의 인덱스
			pRho[nLine]	  -= numRhoH; // 음수 값이 차지하는 위치만큼 뺄셈
			nLine++;
		}
	}

	delete [] pCntTrans;
	delete [] sinLUT;
	delete [] cosLUT;

	return nLine;
}

void SobelEdge(const ByteImage& imageIn, ByteImage& imageOut)
{
	int nWidth	= imageIn.GetWidth();
	int nHeight = imageIn.GetHeight();
	imageOut = ByteImage(nWidth, nHeight);
	imageOut.SetConstValue(0);

	int nWStep = imageIn.GetWStep();

	// Sobel 마스크
	int Gx[9], Gy[9];
	Gx[0] = -1; Gx[1] = 0; Gx[2] = 1;
	Gx[3] = -2; Gx[4] = 0; Gx[5] = 2;
	Gx[6] = -1; Gx[7] = 0; Gx[8] = 1;

	Gy[0] =  1; Gy[1] =  2; Gy[2] =  1;
	Gy[3] =  0; Gy[4] =  0; Gy[5] =  0;
	Gy[6] = -1; Gy[7] = -2; Gy[8] = -1;

	BYTE* pIn  = imageIn.GetPtr();
	BYTE* pOut = imageOut.GetPtr();

	for (int r=1 ; r<nHeight-1 ; r++) // 영상 경계는 제외
	{
	for (int c=1 ; c<nWidth-1 ; c++) // 영상 경계는 제외
	{
		int sumX = 0;
		int sumY = 0;
		for (int y=0 ; y<3 ; y++)
		{
		for (int x=0 ; x<3 ; x++)
		{
			int py = r-1+y;
			int px = c-1+x;
			if (px>=0 && px<nWidth && py>=0 && py<nHeight)
			{
				sumX += Gx[y*3+x]*pIn[py*nWStep+px];
				sumY += Gy[y*3+x]*pIn[py*nWStep+px];
			}
		}
		}
//		pOut[c] = sqrt((double)(sumX*sumX + sumY*sumY)/32); // 경계선의 세기
		pOut[c] = MIN(sqrt((double)(sumX*sumX + sumY*sumY)/4), 255);
	}
	pOut += nWStep;
	}
}

DoubleImage _Gaussian5x5(const IntImage& imageIn)
{
	int nWidth	= imageIn.GetWidth();
	int nHeight = imageIn.GetHeight();
	int nWStep = imageIn.GetWStep();

	DoubleImage imageGss(nWidth, nHeight);
	imageGss.SetConstValue(0);
	int nWStepG = imageGss.GetWStep();

	int nWStep2 = 2*nWStep;

	for (int r=2 ; r<nHeight-2 ; r++)
	{
		double* pGss = imageGss.GetPtr(r);
		int*    pIn  = imageIn.GetPtr(r);
		for (int c=2 ; c<nWidth-2 ; c++)
		{
			pGss[c] = (2*(pIn[c-nWStep2-2] + pIn[c-nWStep2+2] + pIn[c+nWStep2-2] + pIn[c+nWStep2+2]) + 
					  4*(pIn[c-nWStep2-1] + pIn[c-nWStep2+1] + pIn[c-nWStep-2] + pIn[c-nWStep+2] +
						 pIn[c+nWStep-2] + pIn[c+nWStep+2] + pIn[c+nWStep2-1] + pIn[c+nWStep2+1]) + 
					  5*(pIn[c-nWStep2] + pIn[c-2] + pIn[c+2] + pIn[c+nWStep2]) + 
					  9*(pIn[c-nWStep-1] + pIn[c-nWStep+1] + pIn[c+nWStep-1] + pIn[c+nWStep+1]) + 
					  12*(pIn[c-nWStep] + pIn[c+nWStep] + pIn[c-1] + pIn[c+1]) + 
					  15*(pIn[c]))/159.0;
		}
	}
	
	return imageGss;
}

void CannyEdge(const ByteImage& imageIn, ByteImage& imageOut, int nThresholdHi, int nThresholdLo)
{
	int nWidth	= imageIn.GetWidth();
	int nHeight = imageIn.GetHeight();
	int nWStep = imageIn.GetWStep();

	// 가우시안 마스크
	DoubleImage  imageGss(nWidth, nHeight);
	imageGss = _Gaussian5x5(imageIn);

	// 소벨 마스크
	int Gx[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
	int Gy[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

	DoubleImage  imageMag(nWidth, nHeight);
	ByteImage imageAng(nWidth, nHeight);
	int nWStepG = imageGss.GetWStep();
	int nWStepA = imageAng.GetWStep();

	double* pGss = imageGss.GetPtr();

	// 미분 구하기
	for (int r=0 ; r<nHeight ; r++)
	{
	double* pMag = imageMag.GetPtr(r);
	BYTE*   pAng = imageAng.GetPtr(r);
	for (int c=0 ; c<nWidth ; c++)
	{
		double sumX = 0.0;
		double sumY = 0.0;
		for (int y=0 ; y<3 ; y++)
		{
			for (int x=0 ; x<3 ; x++)
			{
				int py = r-1+y;
				int px = c-1+x;
				if (px>=0 && px<nWidth && py>=0 && py<nHeight)
				{
					sumX += Gx[y*3+x]*pGss[py*nWStepG+px];
					sumY += Gy[y*3+x]*pGss[py*nWStepG+px];
				}
			}
		}

		pMag[c] = sqrt(sumX*sumX + sumY*sumY); // 경계선의 세기
		double theta;					 // 경계선의 수직 방향
		if (pMag[c] == 0)
		{
			if(sumY == 0)
			{
				theta = 0;
			}
			else
			{
				theta = 90;
			}
		}
		else
		{
			theta = atan2((float)sumY, (float)sumX)*180.0/M_PI;
		}

		if ((theta > -22.5 && theta < 22.5) || theta > 157.5 || theta < -157.5)
		{
			pAng[c] = 0;
		}
		else if ((theta >= 22.5 && theta < 67.5) || (theta >= -157.5 && theta < -112.5))
		{
			pAng[c] = 45;
		}
		else if ((theta >= 67.5 && theta <= 112.5) || (theta >= -112.5 && theta <= -67.5))
		{
			pAng[c] = 90;
		}
		else
		{
			pAng[c] = 135;
		}
	} // 열 이동 끝
	} // 행 이동 끝

	// 비 최대값 억제
	ByteImage imageCand(nWidth, nHeight);
	imageCand.SetConstValue(0);

	for (int r=1 ; r<nHeight-1 ; r++)
	{
	BYTE*	pCand = imageCand.GetPtr(r);
	double* pMag  = imageMag.GetPtr(r);
	BYTE*   pAng  = imageAng.GetPtr(r);
	for (int c=1 ; c<nWidth-1 ; c++)
	{
		switch (pAng[c])
		{
		case 0:		// 0도 방향 비교
			if (pMag[c] > pMag[c-1] && pMag[c] > pMag[c+1])
			{
				pCand[c] = 255;
			}
			break;
		case 45:	// 45도 방향 비교
			if (pMag[c] > pMag[c-nWStepG+1] && pMag[c] > pMag[c+nWStepG-1])
			{
				pCand[c] = 255;
			}
			break;
		case 90:		// 90도 방향 비교
			if (pMag[c] > pMag[c-nWStepG] && pMag[c] > pMag[c+nWStepG])
			{
				pCand[c] = 255;
			}
			break;
		case 135:	// 135도 방향 비교
			if (pMag[c] > pMag[c-nWStepG-1] && pMag[c] > pMag[c+nWStepG+1])
			{
				pCand[c] = 255;
			}
			break;
		}
	} // 열 이동 끝
	} // 행 이동 끝

	imageCand.SaveImage("Cand.bmp");
	
	// 문턱값 검사
	imageOut = ByteImage(nWidth, nHeight);
	imageOut.SetConstValue(0);
	for (int r=1 ; r<nHeight-1 ; r++)
	{
	BYTE*	pOut  = imageOut.GetPtr(r);
	BYTE*	pCand = imageCand.GetPtr(r);
	double* pMag  = imageMag.GetPtr(r);
	BYTE*   pAng  = imageAng.GetPtr(r);
	for (int c=1 ; c<nWidth-1 ; c++)
	{
		if (pCand[c])
		{
			if (pMag[c] > nThresholdHi)
			{
				pOut[c] = 255;
			}
			else if (pMag[c] > nThresholdLo) // 연결된 픽셀 검사
			{
				bool bIsEdge = true;
				switch (pAng[c])
				{
				case 0:		// 90도 방향 검사
					if ((pMag[c-nWStepG] > nThresholdHi) || 
						(pMag[c+nWStepG] > nThresholdHi))
					{
						pOut[c] = 255;
					}
					break;
				case 45:	// 135도 방향 검사
					if ((pMag[c-nWStepG-1] > nThresholdHi) || 
						(pMag[c+nWStepG+1] > nThresholdHi))
					{
						pOut[c] = 255;
					}
					break;
				case 90:		// 0도 방향 검사
					if ((pMag[c-1] > nThresholdHi) || 
						(pMag[c+1] > nThresholdHi))
					{
						pOut[c] = 255;
					}
					break;
				case 135:	// 45도 방향 검사
					if ((pMag[c-nWStepG+1] > nThresholdHi) || 
						(pMag[c+nWStepG-1] > nThresholdHi))
					{
						pOut[c] = 255;
					}
					break;
				}
			}
		}
	} // 열 이동 끝
	} // 행 이동 끝
}

int HarrisCorner(const ByteImage& imageIn,  double dThreshold, double paramK, int nMaxCorner, double* posX, double* posY)
{
	int nWidth	= imageIn.GetWidth();
	int nHeight = imageIn.GetHeight();
	int nWStep = imageIn.GetWStep();

	IntImage imageDxx(nWidth, nHeight); imageDxx.SetConstValue(0);
	IntImage imageDxy(nWidth, nHeight); imageDxy.SetConstValue(0);
	IntImage imageDyy(nWidth, nHeight); imageDyy.SetConstValue(0);
	int nWStepD = imageDxx.GetWStep();

	// 미분 계산
	int dx, dy;

	for (int r=1 ; r<nHeight-1 ; r++)
	{
		BYTE* pIn = imageIn.GetPtr(r);
		int* pDxx = imageDxx.GetPtr(r);
		int* pDxy = imageDxy.GetPtr(r);
		int* pDyy = imageDyy.GetPtr(r);
		for (int c=1 ; c<nWidth-1 ; c++)
		{
			dx = pIn[c-nWStep-1] + 2*pIn[c-1] + pIn[c+nWStep-1] 
			   - pIn[c-nWStep+1] - 2*pIn[c+1] - pIn[c+nWStep+1];
			dy = pIn[c-nWStep-1] + 2*pIn[c-nWStep] + pIn[c-nWStep+1] 
			   - pIn[c+nWStep-1] - 2*pIn[c+nWStep] - pIn[c+nWStep+1];
			pDxx[c] = dx*dx;
			pDxy[c] = dx*dy;
			pDyy[c] = dy*dy;
		}
	}

	// 가우시안 필터링
	DoubleImage imageGDxx = _Gaussian5x5(imageDxx);
	DoubleImage imageGDxy = _Gaussian5x5(imageDxy);
	DoubleImage imageGDyy = _Gaussian5x5(imageDyy);

	// 코너 응답 함수
	DoubleImage imageCornerScore(nWidth, nHeight);

	for (int r=2 ; r<nHeight-2 ; r++)
	{
		double *pScore = imageCornerScore.GetPtr(r);
		double* pGDxx = imageGDxx.GetPtr(r);
		double* pGDxy = imageGDxy.GetPtr(r);
		double* pGDyy = imageGDyy.GetPtr(r);
		for (int c=2 ; c<nWidth-2 ; c++)
		{
			pScore[c] = (pGDxx[c]*pGDyy[c] - pGDxy[c]*pGDxy[c]) 
				- paramK*(pGDxx[c]+pGDyy[c])*(pGDxx[c]+pGDyy[c]);
		}
	}

	// 지역 최대값 추출
	int numCorner = 0;
	for (int r=2 ; r<nHeight-2 ; r++)
	{
		double *pScore = imageCornerScore.GetPtr(r);
		for (int c=2 ; c<nWidth-2 ; c++)
		{
			if (pScore[c] > dThreshold)
			{
				if (pScore[c] > pScore[c-nWStepD] &&
					pScore[c] > pScore[c+nWStepD] &&
					pScore[c] > pScore[c-1] &&
					pScore[c] > pScore[c+1])
				{
					posX[numCorner] = c;
					posY[numCorner] = r;
					numCorner++;

					if (numCorner >= nMaxCorner)
						return nMaxCorner;
				}
			}
		}
	}

	return numCorner;
}

void DrawCross(ByteImage& image, int x, int y, BYTE color[3])
{
	image.GetAt(x, y, 0) = color[0];
	image.GetAt(x, y, 1) = color[1];
	image.GetAt(x, y, 2) = color[2];

	image.GetAt(x, y-1, 0) = color[0];
	image.GetAt(x, y-1, 1) = color[1];
	image.GetAt(x, y-1, 2) = color[2];

	image.GetAt(x, y+1, 0) = color[0];
	image.GetAt(x, y+1, 1) = color[1];
	image.GetAt(x, y+1, 2) = color[2];

	image.GetAt(x-1, y, 0) = color[0];
	image.GetAt(x-1, y, 1) = color[1];
	image.GetAt(x-1, y, 2) = color[2];

	image.GetAt(x, y+1, 0) = color[0];
	image.GetAt(x, y+1, 1) = color[1];
	image.GetAt(x, y+1, 2) = color[2];
}

