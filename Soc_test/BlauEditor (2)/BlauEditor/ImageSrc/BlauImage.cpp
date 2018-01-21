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

//cvPutText(CvArr* img, char* text, CvPoint org, CvFont* font, CvScalar color);
/*

- img: 입력 이미지.
- text: 쓰고자 하는 문구
- org: 첫 글자의 좌측 하단의 좌표.
- font: 글씨체에 관련된 구조체
- color: 글씨의 색.


*/

void B_PutText(ByteImage* Img, char* text, BlauPoint point, CDC* m_pDC, int m_width, int m_height, BlauScalar RGB) {
	CFont font;    //CFont object 확보

	font.CreateFont(          // object의 생성
		m_width,                  // 문자 폭
		m_height,                  // 문자 높이 
		0,                   // 기울기 각도
		0,                   // 문자 방향
		FW_HEAVY,            // 문자 굵기
		FALSE,               // 문자 기울림 모양
		FALSE,               // 밑 줄
		FALSE,               // 취소선
		DEFAULT_CHARSET,     // 문자 셋
		OUT_DEFAULT_PRECIS,  // 출력 정확도
		CLIP_DEFAULT_PRECIS, // 킬립핑 정확도
		DEFAULT_QUALITY,     // 출력의 질
		DEFAULT_PITCH,       // 자간 거리
		"궁서"               // 문자 모양
	);


	m_pDC->SelectObject(&font);     //pDC에 font 등록

	m_pDC->SetBkMode(TRANSPARENT);
	m_pDC->SetTextColor(RGB(255, 255, 255));
	m_pDC->TextOut(point.x, point.y, text);
	font.DeleteObject();    //font 삭제


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

		if (img->GetChannel() == 3) {
			img->GetAt(x1, y1, 0) = Scalar.B;
			img->GetAt(x1, y1, 0) = Scalar.G;
			img->GetAt(x1, y1, 0) = Scalar.R;

			img->GetAt(x1, y2, 0) = Scalar.B;
			img->GetAt(x1, y2, 0) = Scalar.G;
			img->GetAt(x1, y2, 0) = Scalar.R;

			img->GetAt(x2, y1, 0) = Scalar.B;
			img->GetAt(x2, y1, 0) = Scalar.G;
			img->GetAt(x2, y1, 0) = Scalar.R;

			img->GetAt(x2, y2, 0) = Scalar.B;
			img->GetAt(x2, y2, 0) = Scalar.G;
			img->GetAt(x2, y2, 0) = Scalar.R;
		}
		if (img->GetChannel() == 1) {
			img->GetAt(x1, y1, 0) = Scalar.V;
			img->GetAt(x1, y2, 0) = Scalar.V;
			img->GetAt(x2, y1, 0) = Scalar.V;
			img->GetAt(x2, y2, 0) = Scalar.V;
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

/*void HistogramEqualization(ByteImage *ImageIn, ByteImage *ImageOut) {

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
		BYTE *POut = ImageOut->GetPtr(r);

		for (c = 0;c < nWidth;c++) {
			POut[c] = (BYTE)(m_histogram[pIn[c]] + 0.5);
		}
	}

	ShowImage(ImageOut, "히스토그램 평활화 결과 영상");

	delete m_histogram;
	delete m_histogramCdf;
}*/