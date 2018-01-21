#pragma once

#include <windows.h>
#include <assert.h>//assert매크로 정의 경고창을 띄워줌

#define CLIP(x) (x < 0)? 0 : x > 255 ? 255 : x
#define IN_IMG(x, lo, hi) (x < lo)? lo : x > hi ? hi : x
#define PI 3.14


class BlauScalar
{
public:
	int B;
	int G;
	int R;

	int V;

	BlauScalar(int b, int g = 0, int r = 0)
	{
		B = b;
		G = g;
		R = r;
	}
	BlauScalar(int value)
	{
		V = value;
	}
};

template<class T>
class BlauImage
{
protected:
	int nChannels;//채널 수
	int Height;//세로 픽셀 수
	int Width;//가로 픽셀 수
	int nWStep;//행당 데이터 원소 수 nWStep = ((Width*nChannels*sizeof(T)+3)&~3)/sizeof(T)
	T* pImageData;//픽셀데이터를 가리키는 포인터

public:
	//Getter 함수
	int GetChannel() const { return nChannels; }
	int GetHeight() const { return Height; }
	int GetWidth() const { return Width; }
	int GetWStep() const { return nWStep; }
	T* GetPtr(int r = 0, int c = 0) const { return pImageData + r*nWStep + c; }

	BlauImage(void) :nChannels(0), Height(0), Width(0), nWStep(0), pImageData(NULL) {}//기본 생성자
	BlauImage(int width, int height, int channels = 1)//할당생성자
		:nChannels(channels), Width(width), Height(height)
		, nWStep(((Width*nChannels*sizeof(T) + 3)&~3) / sizeof(T))
	{
		pImageData = new T[Height*nWStep];
	}
	BlauImage(const BlauImage& img)//복사생성자
	{
		nChannels = img.nChannels;
		Height = img.Height;	 Width = img.Width;	nWStep = img.nWStep;
		pImageData = new T[Height*nWStep];

		memcpy(pImageData, img.pImageData, Height*nWStep*sizeof(T));
	}
	template<class From>
	BlauImage(const BlauImage<From>& img)//변환 생성자
	{
		nChannels = img.GetChannel();
		Height = img.GetHeight();	 Width = img.GetWidth();
		nWStep = ((Width*nChannels*sizeof(T) + 3)&~3) / sizeof(T);//행당 데이터원소수 다시계산
		pImageData = new T[Height*nWStep];

		int wstep = img.GetWStep();

		if (sizeof(T) == 1)
		{
			for (int r = 0; r < Height; r++)
			{
				T* pDst = GetPtr(r);
				From* pSrc = img.GetPtr(r);
				for (int c = 0; c < wstep; c++)
				{
					pDst[c] = (T)CLIP(pSrc[c]);//CLIP-->?
				}
			}
		}
		else
		{
			for (int r = 0; r < Height; r++)
			{
				T* pDst = GetPtr(r);
				From* pSrc = img.GetPtr(r);
				for (int c = 0; c < wstep; c++)
				{
					pDst[c] = (T)pSrc[c];
				}
			}
		}
	}

	~BlauImage(void) { if (pImageData) delete[] pImageData; }//기본소멸자

	//Application
	inline T& GetAt(int x, int y, int c = 0) const//픽셀접근함수
	{
		assert(x >= 0 && x < Width&&y >= 0 && y < Height);
		return pImageData[nWStep*y + nChannels*x + c];
	}

	//Transpose a image
	BlauImage<T> Transpose() const
	{
		// 반환할 결과 영상 생성
		BlauImage<T> dst(Height, Width, nChannels);

		T* pSrc = pImageData;
		T* pDst = dst.GetPtr();

		int nWStepS = GetWStep();
		int nWStepD = dst.GetWStep();
		int nBack = (nWStepS*Height - nChannels);

		for (int r = 0; r<Height; r++)
		{
			int idx = 0;
			for (int c = 0; c<Width; c++)
			{
				// 각 채널 값을 순서대로 대입
				for (int h = 0; h<nChannels; h++)
				{
					pDst[h] = pSrc[idx + h];
				}
				idx += nChannels; // 원본 영상은 오른쪽 픽셀로 이동
				pDst += nWStepD; // 결과 영상은 아래쪽 픽셀로 이동
			}
			pSrc += nWStepS; // 원본 영상은 다음 행으로 이동
			pDst -= nBack; // 결과 영상은 다음 열로 이동
		}

		return dst;
	}

	//픽셀 보간법
	BYTE NearestNeighbor(double x, double y)
	{
		int px = IN_IMG((int)(x + 0.5), 0, Width);
		int py = IN_IMG((int)(y + 0.5), 0, Height);
		return pImageData[py*nWStep + px];
	}

	BYTE BiLinearIntp(double x, double y)
	{
		double wLT, wRT, wLB, wRB;

		int xL = IN_IMG((int)x, 0, Width);
		int xR = IN_IMG((int)x + 1, 0, Width);
		int yT = IN_IMG((int)y, 0, Height);
		int yB = IN_IMG((int)y + 1, 0, Height);

		wLT = (xR - x)*(yB - y);
		wRT = (x - xL)*(yB - y);
		wLB = (xR - x)*(y - yT);
		wRB = (x - xL)*(y - yT);

		return (wLT*pImageData[yT*nWStep + xL] + wRT*pImageData[yT*nWStep + xR] +
			wLB*pImageData[yB*nWStep + xL] + wRB*pImageData[yB*nWStep + xR]);
	}

	BYTE BiCubicIntp(double x, double y)
	{
		int x0 = IN_IMG((int)x - 1, 0, Width);
		int x1 = IN_IMG((int)x, 0, Width);
		int x2 = IN_IMG((int)x + 1, 0, Width);
		int x3 = IN_IMG((int)x + 2, 0, Width);

		int y0 = IN_IMG((int)y - 1, 0, Height);
		int y1 = IN_IMG((int)y, 0, Height);
		int y2 = IN_IMG((int)y + 1, 0, Height);
		int y3 = IN_IMG((int)y + 2, 0, Height);

		BYTE p00 = pImageData[y0*nWStep + x0];
		BYTE p01 = pImageData[y1*nWStep + x0];
		BYTE p02 = pImageData[y2*nWStep + x0];
		BYTE p03 = pImageData[y3*nWStep + x0];

		BYTE p10 = pImageData[y0*nWStep + x1];
		BYTE p11 = pImageData[y1*nWStep + x1];
		BYTE p12 = pImageData[y2*nWStep + x1];
		BYTE p13 = pImageData[y3*nWStep + x1];

		BYTE p20 = pImageData[y0*nWStep + x2];
		BYTE p21 = pImageData[y1*nWStep + x2];
		BYTE p22 = pImageData[y2*nWStep + x2];
		BYTE p23 = pImageData[y3*nWStep + x2];

		BYTE p30 = pImageData[y0*nWStep + x3];
		BYTE p31 = pImageData[y1*nWStep + x3];
		BYTE p32 = pImageData[y2*nWStep + x3];
		BYTE p33 = pImageData[y3*nWStep + x3];

		double f00 = p11;
		double f10 = p21;
		double f01 = p12;
		double f11 = p22;
		double fx00 = (p21 - p01) / 2.0;
		double fx10 = (p31 - p11) / 2.0;
		double fx01 = (p22 - p02) / 2.0;
		double fx11 = (p32 - p12) / 2.0;
		double fy00 = (p12 - p10) / 2.0;
		double fy10 = (p22 - p20) / 2.0;
		double fy01 = (p13 - p11) / 2.0;
		double fy11 = (p23 - p21) / 2.0;
		double fxy00 = (p22 - p02 - p20 + p00) / 4.0;
		double fxy10 = (p32 - p12 - p30 + p10) / 4.0;
		double fxy01 = (p23 - p03 - p21 + p01) / 4.0;
		double fxy11 = (p33 - p13 - p31 + p11) / 4.0;

		double a00 = f00;
		double a10 = fx00;
		double a20 = -3 * f00 + 3 * f10 - 2 * fx00 - fx10;
		double a30 = 2 * f00 - 2 * f10 + fx00 + fx10;
		double a01 = fy00;
		double a11 = fxy00;
		double a21 = -3 * fy00 + 3 * fy10 - 2 * fxy00 - fxy10;
		double a31 = 2 * fy00 - 2 * fy10 + fxy00 + fxy10;
		double a02 = -3 * f00 + 3 * f01 - 2 * fy00 - fy01;
		double a12 = -3 * fx00 + 3 * fx01 - 2 * fxy00 - fxy01;
		double a22 = 9 * f00 - 9 * f10 - 9 * f01 + 9 * f11 + 6 * fx00 + 3 * fx10 - 6 * fx01 - 3 * fx11 + 6 * fy00 - 6 * fy10 + 3 * fy01 - 3 * fy11 + 4 * fxy00 + 2 * fxy10 + 2 * fxy01 + fxy11;
		double a32 = -6 * f00 + 6 * f10 + 6 * f01 - 6 * f11 - 3 * fx00 - 3 * fx10 + 3 * fx01 + 3 * fx11 - 4 * fy00 + 4 * fy10 - 2 * fy01 + 2 * fy11 - 2 * fxy00 - 2 * fxy10 - fxy01 - fxy11;
		double a03 = 2 * f00 - 2 * f01 + fy00 + fy01;
		double a13 = 2 * fx00 - 2 * fx01 + fxy00 + fxy01;
		double a23 = -6 * f00 + 6 * f10 + 6 * f01 - 6 * f11 - 4 * fx00 - 2 * fx10 + 4 * fx01 + 2 * fx11 - 3 * fy00 + 3 * fy10 - 3 * fy01 + 3 * fy11 - 2 * fxy00 - fxy10 - 2 * fxy01 - fxy11;
		double a33 = 4 * f00 - 4 * f10 - 4 * f01 + 4 * f11 + 2 * fx00 + 2 * fx10 - 2 * fx01 - 2 * fx11 + 2 * fy00 - 2 * fy10 + 2 * fy01 - 2 * fy11 + fxy00 + fxy10 + fxy01 + fxy11;

		double dx = x - x1;
		double dx2 = dx*dx;
		double dx3 = dx2*dx;
		double dy = y - y1;
		double dy2 = dy*dy;
		double dy3 = dy2*dy;

		return	CLIP(
			a00 + a01*dy + a02*dy2 + a03*dy3 +
			a10*dx + a11*dx*dy + a12*dx*dy2 + a13*dx*dy3 +
			a20*dx2 + a21*dx2*dy + a22*dx2*dy2 + a23*dx2*dy3 +
			a30*dx3 + a31*dx3*dy + a32*dx3*dy2 + a33*dx3*dy3);
	}

	BYTE CubicConvIntp(double x, double y)
	{
		int px[4], py[4];
		px[0] = IN_IMG((int)x - 1, 0, Width);
		px[1] = IN_IMG((int)x, 0, Width);
		px[2] = IN_IMG((int)x + 1, 0, Width);
		px[3] = IN_IMG((int)x + 2, 0, Width);
		py[0] = IN_IMG((int)y - 1, 0, Height);
		py[1] = IN_IMG((int)y, 0, Height);
		py[2] = IN_IMG((int)y + 1, 0, Height);
		py[3] = IN_IMG((int)y + 2, 0, Height);

		double dx = x - px[1];
		double dy = y - py[1];
		double dx2 = dx*dx;
		double dx3 = dx2*dx;
		double dy2 = dy*dy;
		double dy3 = dy2*dy;

		BYTE   p0, p1, p2, p3;  // 네 정수 좌표의 픽셀 값
		double c1, c2, c3, c4;  // 네 정수 좌표의 가중치
		double C[4];			// 가로 방향 보간 결과 

		for (int i = 0; i<4; i++)
		{
			p0 = pImageData[py[i] * nWStep + px[0]];
			p1 = pImageData[py[i] * nWStep + px[1]];
			p2 = pImageData[py[i] * nWStep + px[2]];
			p3 = pImageData[py[i] * nWStep + px[3]];
			c1 = p1;
			c2 = -p0 + p2;
			c3 = 2 * (p0 - p1) + p2 - p3;
			c4 = -p0 + p1 - p2 + p3;
			C[i] = c1 + c2*dx + c3*dx2 + c4*dx3;
		}

		c1 = C[1];
		c2 = -C[0] + C[2];
		c3 = 2 * (C[0] - C[1]) + C[2] - C[3];
		c4 = -C[0] + C[1] - C[2] + C[3];
		return CLIP(c1 + c2*dy + c3*dy2 + c4*dy3);
	}

	//이미지를 특정값으로 초기화하는 함수
	void SetConstValue(T val)
	{
		if (val == 0)
		{
			memset(pImageData, 0, nWStep*Height*sizeof(T));
			return;
		}

		if (sizeof(T) == 1)//데이터의 형식이 BYTE
		{
			memset(pImageData, val, nWStep*Height*sizeof(T));
		}
		else
		{
			T* pData = pImageData;
			for (int r = 0; r < Height; r++)
			{
				for (int c = 0; c < Width; c++)//왜 nwstep이 아닌 width인가?
				{
					pData[c] = val;
				}
				pData += nWStep;
			}
		}
	}

	bool IsEmpty() const {  return pImageData ? false : true; }

	//Operator overloading
	BlauImage& operator=(const BlauImage& img)
	{
		if (this == &img) return *this;

		nChannels = img.nChannels;
		Height = img.Height;	Width = img.Width;	nWStep = img.nWStep;

		if (pImageData) delete[] pImageData;

		if (img.pImageData != NULL)
		{
			pImageData = new T[Height*nWStep];
			memcpy(pImageData, img.pImageData, Height*nWStep*sizeof(T));
		}

		return *this;
	}

	//Save&Load
	bool LoadImage(const char* filename)
	{
		assert(sizeof(T) == 1);//BYTE형의 경우만 가능

		if (strcmp(".BMP", &filename[strlen(filename) - 4]))
		{
			FILE* pFile = NULL;
			fopen_s(&pFile, filename, "rb");//바이너리 읽기모드
			if (!pFile) return false;

			BITMAPFILEHEADER fileHeader;
			if (!fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, pFile))
			{
				fclose(pFile);
				return false;
			}
			if (fileHeader.bfType != 0x4D42)//BM문자검사
			{
				fclose(pFile);
				return false;
			}

			BITMAPINFOHEADER infoHeader;
			if (!fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, pFile))
			{
				fclose(pFile);
				return false;
			}
			if (infoHeader.biBitCount != 8 && infoHeader.biBitCount != 24)//회색조 또는 트루컬러만 인식
			{
				fclose(pFile);
				return false;
			}

			if (Width != infoHeader.biWidth || Height != infoHeader.biHeight || nChannels != infoHeader.biBitCount / 8)
			{
				if (pImageData) delete[] pImageData;

				nChannels = infoHeader.biBitCount / 8;
				Height = infoHeader.biHeight;
				Width = infoHeader.biWidth;
				nWStep = (Width*nChannels*sizeof(T) + 3)&~3;

				pImageData = new T[Height*nWStep];
			}

			fseek(pFile, fileHeader.bfOffBits, SEEK_SET);

			int r;
			for (r = Height - 1; r >= 0; r--)
			{
				if (!fread(&pImageData[r*nWStep], sizeof(BYTE), nWStep, pFile))
				{
					fclose(pFile);
					return false;
				}
			}

			fclose(pFile);
			return true;
		}
		else
		{
			return false;
		}
	}
	bool SaveImage(const char* filename)
	{
		assert(sizeof(T) == 1);

		if (strcmp(".BMP", &filename[strlen(filename) - 4]))
		{
			FILE* pFile = NULL;
			fopen_s(&pFile, filename, "wb");
			if (!pFile) return false;

			BITMAPFILEHEADER fileHeader;
			fileHeader.bfType = 0x4D42;//"BM"
			fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nWStep*Height + (nChannels == 1) * 1024;
			fileHeader.bfReserved1 = 0; fileHeader.bfReserved2 = 0;
			fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (nChannels == 1) * 256 * sizeof(RGBQUAD);

			fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);

			BITMAPINFOHEADER infoHeader;
			infoHeader.biSize = sizeof(BITMAPINFOHEADER);
			infoHeader.biWidth = Width;
			infoHeader.biHeight = Height;
			infoHeader.biPlanes = 1;
			infoHeader.biBitCount = nChannels * 8;
			infoHeader.biCompression = BI_RGB;
			infoHeader.biSizeImage = nWStep*Height;
			infoHeader.biClrImportant = 0;
			infoHeader.biClrUsed = 0;
			infoHeader.biXPelsPerMeter = 0;
			infoHeader.biYPelsPerMeter = 0;

			fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);

			if (nChannels == 1)
			{
				for (int l = 0; l < 256; l++)
				{
					RGBQUAD GrayPalette = { l,l,l,0 };
					fwrite(&GrayPalette, sizeof(RGBQUAD), 1, pFile);
				}
			}

			int r;
			for (r = Height; r >= 0; r--)	fwrite(&pImageData[r*nWStep], sizeof(BYTE), nWStep, pFile);

			fclose(pFile);
			return true;
		}
		else
		{
			return false;
		}
	}

	//Get & Put channel
	void PutChannelImg(const BlauImage<T>& src, int nChannel)
	{
		int Width = src.GetWidth();
		int Height = src.GetHeight();

		for (int r = 0; r<Height; r++)
		{
			T* pSrc = src.GetPtr(r);
			T* pDst = GetPtr(r);

			int idx = nChannel;
			for (int c = 0; c<Width; c++)
			{
				pDst[idx] = pSrc[c];
				idx += nChannels;
			}
		}
	}

	BlauImage<T> GetChannelImg(int nChannel) const
	{
		BlauImage<T> dst(Width, Height);

		for (int r = 0; r<Height; r++)
		{
			T* pSrc = GetPtr(r);
			T* pDst = dst.GetPtr(r);

			int idx = nChannel;
			for (int c = 0; c<Width; c++)
			{
				pDst[c] = pSrc[idx];
				idx += nChannels;
			}
		}

		return dst;
	}


	void BlauCircle(ByteImage& img, BlauPoint center, int radius, BlauScalar Scalar) {

		int x1, y1, x2, y2, degree;
		double radian;

		for (degree = 0; degree <= 90;degree++) {

			radian = degree*PI / 180;
			x1 = center.x + radius*cos(radian);
			y1 = center.y + radius*sin(radian);
			x2 = x1 - 2 * radius*cos(radian);
			y2 = y1 - 2 * radius*sin(radian);

			if (img.GetChannel() == 3) {
				img.GetAt(x1, y1, 0) = Scalar.B;
				img.GetAt(x1, y1, 0) = Scalar.G;
				img.GetAt(x1, y1, 0) = Scalar.R;

				img.GetAt(x1, y2, 0) = Scalar.B;
				img.GetAt(x1, y2, 0) = Scalar.G;
				img.GetAt(x1, y2, 0) = Scalar.R;

				img.GetAt(x2, y1, 0) = Scalar.B;
				img.GetAt(x2, y1, 0) = Scalar.G;
				img.GetAt(x2, y1, 0) = Scalar.R;

				img.GetAt(x2, y2, 0) = Scalar.B;
				img.GetAt(x2, y2, 0) = Scalar.G;
				img.GetAt(x2, y2, 0) = Scalar.R;
			}
			if (img.GetChannel() == 1) {
				img.GetAt(x1, y1, 0) = Scalar.V;
				img.GetAt(x1, y2, 0) = Scalar.V;
				img.GetAt(x2, y1, 0) = Scalar.V;
				img.GetAt(x2, y2, 0) = Scalar.V;
			}
		}
	}
};


class BlauPoint
{
public:
	int x, y;
	BlauPoint(int X = 0, int Y = 0)
	{
		x = X;
		y = Y;
	}
};




typedef BlauImage <BYTE   > ByteImage;
typedef BlauImage <int    > IntImage;
typedef BlauImage <float  > FloatImage;
typedef BlauImage <double > DoubleImage;
