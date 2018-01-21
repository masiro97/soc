#include "stdafx.h"
#include "BlauImage.h"
#include "MyImageFunc.h"
#include "ImageFrameWndManager.h"
#include <math.h>
B_Mat B_CreateMat(int rows, int cols) {

	B_Mat arr(rows, cols);

	return arr;
}
void B_ReleaseMat(B_Mat** arr)
{
	delete arr;
}
void B_SetZero(B_Mat arr)
{
	for (int i = 0; i < arr.GetRow()*arr.GetCol(); i++)
	{
		arr.GetDataF()[i] = 0;
	}
}
void B_MatMul(B_Mat* src1, B_Mat* src2, B_Mat* dst) {
	int row1 = (*src1).GetRow(), col1 = (*src1).GetCol(); float* psrc1 = (*src1).GetDataF();
	int row2 = (*src2).GetRow(), col2 = (*src2).GetCol(); float* psrc2 = (*src2).GetDataF();
	int row_d = (*dst).GetRow(), col_d = (*dst).GetCol(); float* pdst = (*dst).GetDataF();
	if (col1 != row2 || row1 != row_d || col2 != col_d) {
		std::cerr << "MatMul err\n";
		return;
	}

	for (int i = 0; i < row_d; i++)
	{
		for (int j = 0; j < col_d; j++)
		{
			float val = 0;

			for (int k = 0; k<col1; k++)
			{
				val += psrc1[k + i*col1] * psrc2[k*col2 + j];
			}

			pdst[i*col_d + j] = val;
		}
	}
}
double cvmGet(const B_Mat* mat, int row, int col) {
	if ((unsigned)row < (unsigned)mat->rows && (unsigned)col < (unsigned)mat->cols)
		std::cerr << "get err";
	return mat->data.fl[col + mat->cols*row];
}
void cvmSet(B_Mat* mat, int row, int col, double value) {

	if ((unsigned)row < (unsigned)mat->rows && (unsigned)col < (unsigned)mat->cols)
		std::cerr << "set err";

	mat->data.fl[col + mat->cols*row] = value;
}
void B_Sub(B_Mat* src1, B_Mat* src2, B_Mat* dst) {

	float *psrc1 = src1->data.fl, *psrc2 = src2->data.fl, *pdst = dst->data.fl;

	for (int r = 0; r<dst->rows; r++)
	{
		for (int c = 0; c<dst->cols; c++)
			pdst[c] = psrc1[c] - psrc2[c];

		pdst += dst->cols; psrc1 += src1->cols; psrc2 += src2->cols;
	}
}
void B_SVD(B_Mat* A, B_Mat* W, B_Mat* U = (B_Mat*)0, B_Mat* V = (B_Mat*)0) {
	float* a = A->data.fl; int m = A->rows, n = A->cols;
	float *w = W->data.fl, *v = V->data.fl;
	dsvd(&a, m, n, w, &v);
}



void BlauCircle(ByteImage* img, BlauPoint center, int radius, BlauScalar Scalar) {

	int x1, y1, x2, y2, degree;
	double radian;

	for (degree = 0; degree <= 90; degree++) {

		radian = degree*PI / 180;
		x1 = center.x + radius*cos(radian);
		y1 = center.y + radius*sin(radian);
		x2 = x1 - 2 * radius*cos(radian);
		y2 = y1 - 2 * radius*sin(radian);

		bool comp1 = (x1 >= 0 && x1 < img->Width);
		bool comp2 = (y1 >= 0 && y1 < img->Height);
		bool comp3 = (x2 >= 0 && x2 < img->Width);
		bool comp4  = (y2 >= 0 && y2 < img->Height);

		/*if (img->GetChannel() == 3) {
			if (comp) {
				img->GetAt(x1, y1, 0) = Scalar.B;
				img->GetAt(x1, y1, 0) = Scalar.G;
				img->GetAt(x1, y1, 0) = Scalar.R;
			}
			if (comp) {
				img->GetAt(x1, y2, 0) = Scalar.B;
				img->GetAt(x1, y2, 0) = Scalar.G;
				img->GetAt(x1, y2, 0) = Scalar.R;
			}
			if (comp) {
				img->GetAt(x2, y1, 0) = Scalar.B;
				img->GetAt(x2, y1, 0) = Scalar.G;
				img->GetAt(x2, y1, 0) = Scalar.R;
			}
			if (comp) {
				img->GetAt(x2, y2, 0) = Scalar.B;
				img->GetAt(x2, y2, 0) = Scalar.G;
				img->GetAt(x2, y2, 0) = Scalar.R;
			}
		}*/


		if (img->GetChannel() == 1) {
			if (comp1 &&comp2) img->GetAt(x1, y1, 0) = Scalar.V;
			if (comp1 && comp4) img->GetAt(x1, y2, 0) = Scalar.V;
			if (comp3 && comp2) img->GetAt(x2, y1, 0) = Scalar.V;
			if (comp3 && comp4) img->GetAt(x2, y2, 0) = Scalar.V;
		}
	}
}

ByteImage* B_CreateImage(B_size S, int ch)
{

	ByteImage img(S.width, S.height, ch);
	return &img;
}

void B_ReleaseImage(ByteImage **image) {

	delete image;
}

void B_SetImageROI(ByteImage* image, B_Rect rect)
{
	B_ROI* roi = image->roi;
	roi->width = rect.width; roi->height = rect.height;
	roi->xoffset = rect.left_x; roi->yoffset = rect.left_y;
}

/*void HistogramEqualization(ByteImage *ImageIn, ByteImage ImageOut) {

if (ImageIn->GetChannel() != 1) {
AfxMessageBox("회색조 영상을 입력하세요.");
return;
}

int nWidth = ImageIn->GetWidth();
int nHeight = ImageIn->GetHeight();
int *m_histogram = new int[256];
int *m_histogramCdf = new int[256];

memset(m_histogram, 0, 256 * sizeof(int));

int r, c;
for (r = 0;r < nHeight;r++) {
BYTE *pIn = ImageIn->GetPtr(r);
for (c = 0;c < nWidth;c++) {
m_histogram[pIn[c]]++;
}
}
double dNormFactor = 255.0 / (nWidth * nHeight);

for (int i = 0;i < 256;i++)
m_histogramCdf[i] = m_histogram[i] * dNormFactor;
for (int i = 1;i < 256;i++)
m_histogramCdf[i] = m_histogramCdf[i - 1] + m_histogramCdf[i];


for (r = 0;r < nHeight;r++) {
BYTE *pIn = ImageIn->GetPtr(r);
BYTE *POut = ImageOut.GetPtr(r);

for (c = 0;c < nWidth;c++) {
POut[c] = (BYTE)(m_histogram[pIn[c]] + 0.5);
}
}

ShowImage(ImageOut, "히스토그램 평활화 결과 영상");

delete m_histogram;
delete m_histogramCdf;
}
*/

void AutoContrastControl(ByteImage *a_ImageIn, ByteImage *a_ImageOut) {

	int nWidth = a_ImageIn->GetWidth();
	int nHeight = a_ImageIn->GetHeight();

	BYTE maxG = 0;
	BYTE minG = 0;

	int r, c;
	for (r = 0;r < nHeight;r++) {
		BYTE* pImage = a_ImageIn->GetPtr(r);

		for (c = 0;c < nWidth;c++) {

			if (pImage[c] > maxG)
				maxG = pImage[c];
			if (pImage[c] < minG)
				minG = pImage[c];
		}
	}

	a_ImageOut = &((*a_ImageIn - minG) * (255.0 / (maxG - minG)));
	ShowImage(*a_ImageOut,"Image");
	
	CString str;
	str.Format("Min : %d, Max : %d", minG, maxG);
	AfxMessageBox(str);

}

void GammaContrast(double dGamma, ByteImage *ImageIn, ByteImage *ImageOut) {

	double invGamma = 1.0 / dGamma;

	BYTE arrPow[255];
	for (int n = 0; n < 255;n++) {
		arrPow[n] = (BYTE)(pow(n / 255.0, invGamma) * 255 + 0.5);

	}

	int nWStep = ImageIn->GetWidth() * ImageIn->GetChannel();
	int nHeight = ImageIn->GetHeight();

	int r, c;
	for (r = 0;r < nHeight;r++) {

		BYTE *pIn = ImageIn->GetPtr(r);
		BYTE *pOut = ImageIn->GetPtr(r);

		for (c = 0;c < nWStep;c++) {
			pOut[c] = arrPow[pIn[c]];
		}
	}

	ShowImage(*ImageOut, "Image");
}

void DrawLines(ByteImage& canvas, int x1, int y1, int x2, int y2, int k,BYTE val) {

	ASSERT(canvas.GetChannel() == 1);


	int xs, ys, xe, ye;

	if (x1 == x2) //수직선
	{

		if (y1 < y2) { ys = y1;ye = y2; }
		else { ys = y2;ye = y1; }

		for (int r = ys;r <= ye;r++) {
				for(int n = x1;n<x1+k;n++)
					canvas.GetAt(n, r) = val;
		}
		return;
	}

	double a = (double)(y2 - y1) / (x2 - x1);
	int nHeight = canvas.GetHeight();

	if ((a > -1) && (a < 1)) {

		if (x1 < x2) { x2 = x1;xe = x2;ys = y1;ye = y2; }
		else { xs = x2;xe = x1;ys = y2;ye = y1; }

		for (int c = xs;c <= xe;c++) {

			int r = (int)(a*(c - xs) + ys + 0.5);
			if (r < 0 || r >= nHeight)
				continue;
			for (int n = r;n<r + k;n++)
				canvas.GetAt(c, n) = val;
		}
	}

	else {

		double invA = 1.0 / a;
		if (y1 < y2) { ys = y1;ye = y2;xs = x1;xe = x2; }
		else { ys = y2;ye = y1;xs = x2;xe = x1; }

		for (int r = ys;r <= ye;r++) {

			int c = (int)(invA*(r - ys) + xs + 0.5);
			if (r < 0 || r >= nHeight)
				continue;
			for (int n = r;n<r + k;n++)
				canvas.GetAt(c, n) = val;
		}
	}
}