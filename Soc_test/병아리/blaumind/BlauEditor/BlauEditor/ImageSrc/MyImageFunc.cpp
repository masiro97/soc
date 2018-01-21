#include "stdafx.h"
#include "MyImageFunc.h"
#include <iostream>
#include <algorithm>
#include "ImageFrameWndManager.h"

// 논리 연산자 정의
void ANDImage(const ByteImage& src1, const ByteImage& src2, ByteImage& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;

	for (r=0 ; r<nHeight ; r++)
	{
		BYTE* pSrc1 = src1.GetPtr(r);
		BYTE* pSrc2 = src2.GetPtr(r);
		BYTE* pDst  = dst.GetPtr(r);

		for (c=0 ; c<nWStep ; c++)
		{
			pDst[c] = (pSrc1[c] & pSrc2[c]);
		}
	}
}

ByteImage operator&(const ByteImage& src1, const ByteImage& src2)
{
	int nWidth1  = src1.GetWidth();
	int nHeight1 = src1.GetHeight();
	int nWidth2  = src2.GetWidth();
	int nHeight2 = src2.GetHeight();
	int nChan1   = src1.GetChannel();
	int nChan2   = src2.GetChannel();

	if(nWidth1==nWidth2 && nHeight1==nHeight2 && nChan1==nChan2) std::cerr<<"operator & err\n";

	ByteImage ret(nWidth1, nHeight1, nChan1);
	ANDImage(src1, src2, ret);
	return ret;
}

void ANDConst(const ByteImage& src, const BYTE val, ByteImage& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;


	for (r=0 ; r<nHeight ; r++)
	{
		BYTE* pSrc = src.GetPtr(r);
		BYTE* pDst = dst.GetPtr(r);

		for (c=0 ; c<nWStep ; c++)
		{
			pDst[c] = (pSrc[c] & val);
		}
	}
}

ByteImage operator&(const ByteImage& src, const BYTE val)
{
	ByteImage ret(src.GetWidth(), src.GetHeight(), src.GetChannel());
	ANDConst(src, val, ret);
	return ret;
}

void ORImage(const ByteImage& src1, const ByteImage& src2, ByteImage& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;

	for (r=0 ; r<nHeight ; r++)
	{
		BYTE* pSrc1 = src1.GetPtr(r);
		BYTE* pSrc2 = src2.GetPtr(r);
		BYTE* pDst  = dst.GetPtr(r);

		for (c=0 ; c<nWStep ; c++)
		{
			pDst[c] = (pSrc1[c] | pSrc2[c]);
		}
	}
}

ByteImage operator|(const ByteImage& src1, const ByteImage& src2)
{
	int nWidth1  = src1.GetWidth();
	int nHeight1 = src1.GetHeight();
	int nWidth2  = src2.GetWidth();
	int nHeight2 = src2.GetHeight();
	int nChan1   = src1.GetChannel();
	int nChan2   = src2.GetChannel();

	if(nWidth1==nWidth2 && nHeight1==nHeight2 && nChan1==nChan2) std::cerr<<"operator|\n";

	ByteImage ret(nWidth1, nHeight1, nChan1);
	ORImage(src1, src2, ret);
	return ret;
}

void ORConst(const ByteImage& src, const BYTE val, ByteImage& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;


	for (r=0 ; r<nHeight ; r++)
	{
		BYTE* pSrc = src.GetPtr(r);
		BYTE* pDst = dst.GetPtr(r);

		for (c=0 ; c<nWStep ; c++)
		{
			pDst[c] = (pSrc[c] | val);
		}
	}
}

ByteImage operator|(const ByteImage& src, const BYTE val)
{
	ByteImage ret(src.GetWidth(), src.GetHeight(), src.GetChannel());
	ORConst(src, val, ret);
	return ret;
}

void NOTImage(const ByteImage& src, ByteImage& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;

	for (r=0 ; r<nHeight ; r++)
	{
		BYTE* pSrc1 = src.GetPtr(r);
		BYTE* pDst  = dst.GetPtr(r);

		for (c=0 ; c<nWStep ; c++)
		{
			pDst[c] = (~pSrc1[c]);
		}
	}
}

ByteImage operator~(const ByteImage& src)
{
	ByteImage ret(src.GetWidth(), src.GetHeight(), src.GetChannel());
	NOTImage(src, ret);
	return ret;
}

void XORImage(const ByteImage& src1, const ByteImage& src2, ByteImage& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;

	for (r=0 ; r<nHeight ; r++)
	{
		BYTE* pSrc1 = src1.GetPtr(r);
		BYTE* pSrc2 = src2.GetPtr(r);
		BYTE* pDst  = dst.GetPtr(r);

		for (c=0 ; c<nWStep ; c++)
		{
			pDst[c] = (pSrc1[c] ^ pSrc2[c]);
		}
	}
}

ByteImage operator^(const ByteImage& src1, const ByteImage& src2)
{
	int nWidth1  = src1.GetWidth();
	int nHeight1 = src1.GetHeight();
	int nWidth2  = src2.GetWidth();
	int nHeight2 = src2.GetHeight();
	int nChan1   = src1.GetChannel();
	int nChan2   = src2.GetChannel();

	if(nWidth1==nWidth2 && nHeight1==nHeight2 && nChan1==nChan2) std::cerr<<"operator^ err\n";

	ByteImage ret(nWidth1, nHeight1, nChan1);
	XORImage(src1, src2, ret);
	return ret;
}

void XORConst(const ByteImage& src, const BYTE val, ByteImage& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;

	for (r=0 ; r<nHeight ; r++)
	{
		BYTE* pSrc = src.GetPtr(r);
		BYTE* pDst = dst.GetPtr(r);

		for (c=0 ; c<nWStep ; c++)
		{
			pDst[c] = (pSrc[c] ^ val);
		}
	}
}

ByteImage operator^(const ByteImage& src, const BYTE val)
{
	ByteImage ret(src.GetWidth(), src.GetHeight(), src.GetChannel());
	XORConst(src, val, ret);
	return ret;
}




void B_Sobel(ByteImage* ImgIn, ByteImage* ImgOut, int ori )
{
	double sobelX[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
	double sobelY[9] = { 1, 0, -1, 2, 0, -2, 1, 0, -1 };
	double laplaican4[9] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 };

	double *pMask;
	if (ori == 0) pMask = sobelX;
	else pMask = sobelY;

	int nWidth = ImgIn->GetWidth();
	int nHeight = ImgIn->GetHeight();
	int nChnnl = ImgIn->GetChannel();
	int nWStep = ImgIn->GetWStep();

	BYTE* pIn = ImgIn->GetPtr();
	BYTE* pOut = ImgOut->GetPtr();

	int r, c;
	for (r = 0; r<nHeight; r++) // 행 이동
	{

		for (c = 0; c<nWidth; c++) // 열 이동
		{
			double dSum = 0.0; // (마스크*픽셀) 값의 합

			for (int y = 0; y<3; y++)
			{
				for (int x = 0; x<3; x++)
				{
					int py = r - 1 + y;
					int px = c - 1 + x;

					if (px >= 0 && px<nWidth && py >= 0 && py<nHeight)
						dSum += pMask[y * 3 + x] * pIn[py*nWStep + px];
				}
			}

			pOut[r*nWStep + c] = CLIP(dSum + 128);
		} // 열 이동 끝
	} // 행 이동 끝
}

void B_Laplacian4(ByteImage* ImgIn, ByteImage* ImgOut)
{
	double laplaican4[9] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 };

	double *pMask = laplaican4;

	int nWidth = ImgIn->GetWidth();
	int nHeight = ImgIn->GetHeight();
	int nChnnl = ImgIn->GetChannel();
	int nWStep = ImgIn->GetWStep();

	BYTE* pIn = ImgIn->GetPtr();
	BYTE* pOut = ImgOut->GetPtr();

	int r, c;
	for (r = 0; r<nHeight; r++) // 행 이동
	{

		for (c = 0; c<nWidth; c++) // 열 이동
		{
			double dSum = 0.0; // (마스크*픽셀) 값의 합

			for (int y = 0; y<3; y++)
			{
				for (int x = 0; x<3; x++)
				{
					int py = r - 1 + y;
					int px = c - 1 + x;

					if (px >= 0 && px<nWidth && py >= 0 && py<nHeight)
						dSum += pMask[y * 3 + x] * pIn[py*nWStep + px];
				}
			}

			pOut[r*nWStep + c] = CLIP(dSum + 128);
		} // 열 이동 끝
	} // 행 이동 끝
}



double m_bufGss[17];
void B_Gaussian(ByteImage* ImgIn,ByteImage* ImgOut,int G_size)
{
	int nWidth = ImgIn->GetWidth();
	int nHeight = ImgIn->GetHeight();
	int nChnnl = ImgIn->GetChannel();
	int nWStep = ImgIn->GetWStep();

	BYTE* pIn = ImgIn->GetPtr();
	BYTE* pOut = ImgOut->pImageData;

	// 1차원 가우스 마스크 생성
	int nHalf = (G_size-1)/2;
	double sig;
	if (G_size == 7) sig = G_size / 6;
	else sig = 0.8;
	for (int n = 0; n <= nHalf; n++)
	{
		m_bufGss[nHalf - n] = m_bufGss[nHalf + n]
			= exp(-n*n / (2 * sig*sig));
	}

	int r, c, l;
	DoubleImage tmpConv(nWidth, nHeight, nChnnl);
	double* pTmp = tmpConv.GetPtr();

	// 가로 방향 회선
	for (r = 0; r<nHeight; r++) // 행 이동
	{
		for (c = 0; c<nWidth; c++) // 열 이동
		{
			for (l = 0; l<nChnnl; l++) // 채널 이동
			{
				double dSum = 0; // (마스크*픽셀) 값의 합
				double dGss = 0; // 마스크 값의 합
				for (int n = -nHalf; n <= nHalf; n++)
				{
					int px = c + n;

					if (px >= 0 && px<nWidth)
					{
						dSum += (pIn[nWStep*r + nChnnl*px + l] * m_bufGss[nHalf + n]);
						dGss += m_bufGss[nHalf + n];
					}
				}
				pTmp[nWStep*r + nChnnl*c + l] = dSum / dGss;
			} // 채널 이동 끝
		} // 열 이동 끝
	} // 행 이동 끝

	  // 세로 방향 회선
	for (r = 0; r<nHeight; r++) // 행 이동
	{
		for (c = 0; c<nWidth; c++) // 열 이동
		{
			for (l = 0; l<nChnnl; l++) // 채널 이동
			{
				double dSum = 0; // 픽셀 값의 합
				double dGss = 0; // 마스크 값의 합
				for (int n = -nHalf; n <= nHalf; n++)
				{
					int py = r + n;

					if (py >= 0 && py<nHeight)
					{
						int absN = abs(n);
						dSum += pTmp[nWStep*py + nChnnl*c + l] * m_bufGss[nHalf + n];
						dGss += m_bufGss[nHalf + n];
					}
				}
				pOut[nWStep*r + nChnnl*c + l] = (BYTE)(dSum / dGss);
			} // 채널 이동 끝
		} // 열 이동 끝
	} // 행 이동 끝
}

void B_GaussianSmoothing_7x7(ByteImage* src, ByteImage* dst, IntImage* int_buffer)
{
	const int w = src->Width;
	const int h = src->Height;
	const int bw = int_buffer->nWStep / 4;

	if (bw == 704) {
		if (w == 704 && h == 544) B_GaussianSmoothing_7x7_standard_weights(src, dst, 704, 544, int_buffer, 704);
		else if (w == 352 && h == 272) B_GaussianSmoothing_7x7_standard_weights(src, dst, 352, 272, int_buffer, 704);
		else if (w == 176 && h == 136) B_GaussianSmoothing_7x7_standard_weights(src, dst, 176, 136, int_buffer, 704);
		else                           B_GaussianSmoothing_7x7_standard_weights(src, dst, w, h, int_buffer, 704);
	}
	else {
		if (w == 784 && h == 640) B_GaussianSmoothing_7x7_standard_weights(src, dst, 784, 640, int_buffer, bw);
		else if (w == 704 && h == 544) B_GaussianSmoothing_7x7_standard_weights(src, dst, 352, 272, int_buffer, bw);
		else if (w == 640 && h == 480) B_GaussianSmoothing_7x7_standard_weights(src, dst, 640, 480, int_buffer, bw);
		else if (w == 392 && h == 320) B_GaussianSmoothing_7x7_standard_weights(src, dst, 392, 320, int_buffer, bw);
		else if (w == 352 && h == 272) B_GaussianSmoothing_7x7_standard_weights(src, dst, 352, 272, int_buffer, bw);
		else if (w == 320 && h == 240) B_GaussianSmoothing_7x7_standard_weights(src, dst, 320, 240, int_buffer, bw);
		else if (w == 196 && h == 160) B_GaussianSmoothing_7x7_standard_weights(src, dst, 196, 160, int_buffer, bw);
		else if (w == 176 && h == 136) B_GaussianSmoothing_7x7_standard_weights(src, dst, 176, 136, int_buffer, bw);
		else                           B_GaussianSmoothing_7x7_standard_weights(src, dst, w, h, int_buffer, bw);
	}
}

inline void B_GaussianSmoothing_7x7_standard_weights(ByteImage * im_src, ByteImage * im_dst,
	const int w, const int h,
	IntImage * im_int_buffer,
	const int width_int_buffer)
{
	// First pass: make use of intermediate_int_image

	for (int y = 0; y < h; y++) {
		unsigned char * src = B_Row(im_src, y, unsigned char);
		int * ints = B_Row(im_int_buffer, y, int);

		for (int x = 3; x < w - 3; x++)
			ints[x] =
			2 * (9 * int(src[x]) + 7 * (int(src[x - 1]) + int(src[x + 1])) + int(src[x - 3]) + int(src[x + 3])) +
			7 * (int(src[x - 2]) + int(src[x + 2]));

		ints[0] = ints[1] = ints[2] = ints[3];
		ints[w - 1] = ints[w - 2] = ints[w - 3] = ints[w - 4];
	}

	// Second pass:
	const int delta = 1 << (12 - 1);
	const int D = width_int_buffer;
	for (int y = 3; y < h - 3; y++) {
		int * row0 = B_Row(im_int_buffer, y - 3, int);
		unsigned char * dest = B_Row(im_dst, y, unsigned char);

		for (int x = 0; x < w; x++)
			dest[x] = (unsigned char)((2 * (9 * (row0 + 3 * D)[x] +
				7 * ((row0 + 2 * D)[x] + (row0 + 4 * D)[x]) +
				row0[x] + (row0 + 6 * D)[x]) +
				7 * ((row0 + D)[x] + (row0 + 5 * D)[x]) + delta) >> 12);
	}

	// Second pass: borders...
	unsigned char * dest0 = B_Row(im_dst, 0, unsigned char);
	unsigned char * dest1 = dest0 + im_dst->nWStep;
	unsigned char * dest2 = dest1 + im_dst->nWStep;
	unsigned char * dest3 = dest2 + im_dst->nWStep;
	unsigned char * destw1 = B_Row(im_dst, h - 1, unsigned char);
	unsigned char * destw2 = destw1 - im_dst->nWStep;
	unsigned char * destw3 = destw2 - im_dst->nWStep;
	unsigned char * destw4 = destw3 - im_dst->nWStep;

	for (int x = 0; x < w; x++) {
		dest0[x] = dest1[x] = dest2[x] = dest3[x];
		destw1[x] = destw2[x] = destw3[x] = destw4[x];
	}
}

void B_PyrDown(ByteImage* src, ByteImage* dst)
{
	int height1 = src->Height, height2 = dst->Height;
	int wstep1 = src->nWStep, wstep2 = dst->nWStep;
	BYTE *pdst = dst->pImageData;

	//ShowImage(*src);
	ByteImage temp = ByteImage(src->Width,src->Height);
	B_Gaussian(src,&temp,5);
	BYTE *psrc = temp.pImageData;

	for (int r=0,i=0;r<height1;r+=2)
		for (int c =0;c<wstep1;c+=2,i++)
			pdst[i] = psrc[r*wstep1+c];
}
