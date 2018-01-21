#include "stdafx.h"
#include "BlauImage.h"


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
	if (!((unsigned)row < (unsigned)mat->rows && (unsigned)col < (unsigned)mat->cols))
		std::cerr << "get err";
	return mat->data.fl[col + mat->cols*row];
}
void cvmSet(B_Mat* mat, int row, int col, double value) {

	if (!((unsigned)row < (unsigned)mat->rows && (unsigned)col < (unsigned)mat->cols))
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
		bool comp4 = (y2 >= 0 && y2 < img->Height);

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
			if (comp1 && comp2) img->GetAt(x1, y1, 0) = Scalar.V;
			if (comp1 && comp4) img->GetAt(x1, y2, 0) = Scalar.V;
			if (comp3 && comp2) img->GetAt(x2, y1, 0) = Scalar.V;
			if (comp3 && comp4) img->GetAt(x2, y2, 0) = Scalar.V;
		}
	}
}

void B_CreateImage(ByteImage* src,B_size S, int ch)
{
	ByteImage dst(S.width, S.height, ch);
	dst.nWStep = ((S.width*ch * sizeof(BYTE) + 3)&~3) / sizeof(BYTE);

	dst.roi = NULL;

	dst.pImageData = new BYTE[src->Height*src->nWStep];
	src = &dst;
}

void B_ReleaseImage(ByteImage **image) {

	delete image;
}

void B_SetImageROI(ByteImage* image, B_Rect rect)
{
	image->roi = new B_ROI;
	image->roi->width = rect.width; 
	image->roi->height = rect.height;
	image->roi->xoffset = rect.left_x; image->roi->yoffset = rect.left_y;
}

void B_Copy(const ByteImage* src, ByteImage* dst)
{
	BYTE *psrc; BYTE *pdst; int rows, cols, rowd, cold;

	if (src->roi == NULL) { psrc = src->pImageData; rows = src->Height; cols = src->Width; }
	else {
		psrc = src->pImageData + src->roi->yoffset*src->nWStep + src->roi->xoffset - 1;
		rows = src->roi->height; cols = src->roi->width;
	}
	if (dst->roi == NULL) { pdst = dst->pImageData; rowd = dst->Height; cold = dst->Width; }
	else {
		pdst = dst->pImageData + dst->roi->yoffset*dst->nWStep + dst->roi->xoffset - 1;
		rowd = dst->roi->height; cold = dst->roi->width;
	}
	if (rows != rowd || cols != cold) std::cout << "b_copy err" << std::endl;;

	for (int r = 0; r < rows; r++) {
		memcpy(pdst, psrc, cols*sizeof(BYTE));
		pdst += dst->nWStep;
		psrc += src->nWStep;
	}
}

void B_Copy(B_Mat* src, B_Mat* dst)
{
	if (src->rows != dst->rows || src->cols != dst->cols) std::cout << "b_copy err"<<std::endl;
	memcpy(dst->data.fl, src->data.fl,src->cols*src->rows*sizeof(float));
}