
// MDIImageViewerView.cpp : CMDIImageViewerView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MDIImageViewer.h"
#endif

#include "MDIImageViewerDoc.h"
#include "MDIImageViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMDIImageViewerView

IMPLEMENT_DYNCREATE(CMDIImageViewerView, CView)

BEGIN_MESSAGE_MAP(CMDIImageViewerView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMDIImageViewerView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMDIImageViewerView 생성/소멸

CMDIImageViewerView::CMDIImageViewerView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMDIImageViewerView::~CMDIImageViewerView()
{
}

BOOL CMDIImageViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMDIImageViewerView 그리기
bool _DrawImage(HDC hdc, const ByteImage& image, int nSrcx, int nSrcy, int nSrcw, int nSrcH, int nDstX, int nDstY, int nDstW, int nDstH) {

	if (image.IsEmpty()) return false;//빈 영상이면 바로 종료
	static BYTE bmiBuf[sizeof(BITMAPINFOHEADER) + 1024];
	int nWidth = image.GetWStep() / image.GetChannel();
	int nHeight = image.GetHeight();
	int nBPP = image.GetChannel() * 8;

	//256색 회색조 또는 트루 컬러 영상만 출력 가능

	ASSERT((BITMAPINFO*)bmiBuf && nWidth > 0 && nHeight > 0 && (nBPP == 8 || nBPP == 24));
	BITMAPINFOHEADER *bmih = &(((BITMAPINFO*)bmiBuf)->bmiHeader);
	memset(bmih, 0, sizeof(bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = nWidth;
	bmih->biHeight = -nHeight;  //위에서 아래로 출력
	bmih->biPlanes = 1;
	bmih->biBitCount = nBPP;
	bmih->biCompression = BI_RGB; //압축 사용 안함

	static const RGBQUAD GrayPalette[256] = { {0,0,0,0},{1,1,1,0},{255,255,255,0} };
	if (nBPP == 8) {
		memcpy(((BITMAPINFO*)bmiBuf)->bmiColors, GrayPalette, 256 * sizeof(RGBQUAD));

		SetStretchBltMode(hdc, COLORONCOLOR);
		::StretchDIBits(hdc, nDstX, nDstY, nDstW, nDstH, nSrcX, nSrcY, nSrcW, nSrcH, image.GetPtr(), (LPBITMAPINFO)&(((BITMAPINFO*)bmiBuf)->bmiHEADER), DIB_RGB_COLORS, SRCCOPY);
		return true;
	}
}
void CMDIImageViewerView::OnDraw(CDC* /*pDC*/)
{
	CMDIImageViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	ByteImage& m_image = pDoc->m_image;
	if (m_image.IsEmpty()) return;

	_DrawImage(pDC->m_hDC, m_image, 0, 0, m_image.GetWidth(), m_image.GetHeight(), 0, 0, m_image.GetWidth(), m_image.GetHeight());
}


// CMDIImageViewerView 인쇄


void CMDIImageViewerView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMDIImageViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMDIImageViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMDIImageViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMDIImageViewerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMDIImageViewerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMDIImageViewerView 진단

#ifdef _DEBUG
void CMDIImageViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CMDIImageViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMDIImageViewerDoc* CMDIImageViewerView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMDIImageViewerDoc)));
	return (CMDIImageViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CMDIImageViewerView 메시지 처리기
