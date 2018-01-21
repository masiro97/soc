#include <windows.h>
#include <windef.h>//BYTE가 정의되어 있음
#include <assert.h>//assert매크로 정의 경고창을 띄워줌


typedef BlauImage<BYTE> ByteImage;
typedef BlauImage<int> IntImage;
typedef BlauImage<float> FloatImage;
typedef BlauImage<double> DoubleImage;

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

	void SetConstValue(T val)//이미지를 특정값으로 초기화하는 함수
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

	bool IsEmpty() const return pImageData ? false : true;

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

			fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, pFIle);

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
					fwrite(&GraPalette, sizeof(RGBQUAD), 1, pFile);
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
};
