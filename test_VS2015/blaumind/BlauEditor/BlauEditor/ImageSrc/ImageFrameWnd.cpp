// ImageFrameWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageFrameWnd.h"

static int nFrameWndCount = 0;

// CImageFrameWnd

IMPLEMENT_DYNCREATE(CImageFrameWnd, CFrameWnd)


CImageFrameWnd::CImageFrameWnd()
{
}

CImageFrameWnd::CImageFrameWnd(const ByteImage &image, const char *name)
{
	m_view.SetImage(image);

	CString wndName;
	if (name)
		wndName = name;
	else
		wndName.Format("Image view %d",nFrameWndCount);

	CRect rect(30 * nFrameWndCount, 30 * nFrameWndCount,
		image.GetWidth() + 30 * nFrameWndCount, image.GetHeight() + 30 * nFrameWndCount);
	Create(NULL, wndName, WS_OVERLAPPEDWINDOW, rect);

	nFrameWndCount++;

	ShowWindow(SW_SHOW);
}

CImageFrameWnd::~CImageFrameWnd()
{
}


BEGIN_MESSAGE_MAP(CImageFrameWnd, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CImageFrameWnd 메시지 처리기입니다.


int CImageFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_view.CreateEx(this);
	return 0;
}

#include "ImageFrameWndManager.h"
extern CImageFrameWndManager gImageFrameWndManager;
void CImageFrameWnd::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	gImageFrameWndManager.Delete(this);
	//	CFrameWnd::PostNcDestroy();
}
